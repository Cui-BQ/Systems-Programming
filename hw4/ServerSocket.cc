/*
 * Copyright ©2020 Hal Perkins.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Fall Quarter 2020 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>       // for snprintf()
#include <unistd.h>      // for close(), fcntl()
#include <sys/types.h>   // for socket(), getaddrinfo(), etc.
#include <sys/socket.h>  // for socket(), getaddrinfo(), etc.
#include <arpa/inet.h>   // for inet_ntop()
#include <netdb.h>       // for getaddrinfo()
#include <errno.h>       // for errno, used by strerror()
#include <string.h>      // for memset, strerror()
#include <iostream>      // for std::cerr, etc.

#include "./ServerSocket.h"

extern "C" {
  #include "libhw1/CSE333.h"
}

namespace hw4 {

ServerSocket::ServerSocket(uint16_t port) {
  port_ = port;
  listen_sock_fd_ = -1;
}

ServerSocket::~ServerSocket() {
  // Close the listening socket if it's not zero.  The rest of this
  // class will make sure to zero out the socket if it is closed
  // elsewhere.
  if (listen_sock_fd_ != -1)
    close(listen_sock_fd_);
  listen_sock_fd_ = -1;
}

bool ServerSocket::BindAndListen(int ai_family, int *listen_fd) {
  // Use "getaddrinfo," "socket," "bind," and "listen" to
  // create a listening socket on port port_.  Return the
  // listening socket through the output parameter "listen_fd".

  // STEP 1:

  // Prepare hints.
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = ai_family;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = IPPROTO_TCP;

  // Get the address info.
  struct addrinfo *res;
  if (getaddrinfo (nullptr, (std::to_string(port_)).c_str() ,&hints, &res) != 0) {
    return false;
  }

  // Open a socket
  listen_sock_fd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  while (listen_sock_fd_ == -1){
    res = res->ai_next;
    if (res != nullptr) {
      listen_sock_fd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    } else {
      close(listen_sock_fd_);
      freeaddrinfo(res);
      return false;
    }
  }

  // Make the same port became reusable. 
  int optval = 1;
  Verify333(setsockopt(listen_sock_fd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == 0);

  // Bind the socket and the address together.
  bind(listen_sock_fd_, res->ai_addr, res->ai_addrlen);
  sock_family_ = res->ai_family; 
  freeaddrinfo(res);

  // Let the socket to start listening for incoming connections.
  if (listen(listen_sock_fd_, SOMAXCONN) != 0) {
    close(listen_sock_fd_);
    return false;
  }

  *listen_fd = listen_sock_fd_;
  return true;
}

bool ServerSocket::Accept(int *accepted_fd,
                          std::string *client_addr,
                          uint16_t *client_port,
                          std::string *client_dnsname,
                          std::string *server_addr,
                          std::string *server_dnsname) {
  // Accept a new connection on the listening socket listen_sock_fd_.
  // (Block until a new connection arrives.)  Return the newly accepted
  // socket, as well as information about both ends of the new connection,
  // through the various output parameters.

  // STEP 2:

  // Prepare for calling accept().
  struct sockaddr_in peer4;
  struct sockaddr_in6 peer6;
  struct sockaddr *peer;
  socklen_t peerlen;

  if (sock_family_ == AF_INET){
    peer = (struct sockaddr *) &peer4;
    peerlen = sizeof(peer4);
  } else {
    peer = (struct sockaddr *) &peer6;
    peerlen = sizeof(peer6);
  }

  // Keep trying to connect untill success or return false if errno != EAGAIN.
  while (true){
    *accepted_fd = accept(listen_sock_fd_, peer, &peerlen);
    if (errno == 0) {
      break;
    } else if (errno == EAGAIN) {
      continue;
    } else {
      return false;
    }  
  }

  // Extract information about the client end.
  char hname[1024];
  // For IP_V4
  if (sock_family_ == AF_INET) {
    char addrbuf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &peer4.sin_addr, addrbuf, INET_ADDRSTRLEN);
    *client_addr = std::string(addrbuf);
    *client_port = ntohs(peer4.sin_port);
    if (getnameinfo(reinterpret_cast<struct sockaddr *>(&peer4), peerlen, hname, 1024, nullptr, 0, 0) == 0) {
      *client_dnsname = std::string(hname);
    } else {
      *client_dnsname = *client_addr;
    }
  } else {
    // For IP_V6
    char addrbuf[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &peer6.sin6_addr, addrbuf, INET6_ADDRSTRLEN);
    *client_addr = std::string(addrbuf);
    *client_port = ntohs(peer6.sin6_port);
    if (getnameinfo(reinterpret_cast<struct sockaddr *>(&peer6), peerlen, hname, 1024, nullptr, 0, 0) == 0) {
      *client_dnsname = std::string(hname);
    } else {
      *client_dnsname = *client_addr;
    }
  }

  // Extract information about the server end.
      // For IP_V4
      if (sock_family_ == AF_INET) {
          struct sockaddr_in srvr;
          socklen_t srvrlen = sizeof(srvr);
          char addrbuf[INET_ADDRSTRLEN];
          getsockname(*accepted_fd, (struct sockaddr *) &srvr, &srvrlen);
          inet_ntop(AF_INET, &srvr.sin_addr, addrbuf, INET_ADDRSTRLEN);
          *server_addr = std::string(addrbuf);
          getnameinfo(reinterpret_cast<struct sockaddr *>(&srvr), srvrlen, hname, 1024, nullptr, 0, 0);
          *server_dnsname = std::string(hname);
      } else {
          // For IP_V6
          struct sockaddr_in6 srvr;
          socklen_t srvrlen = sizeof(srvr);
          char addrbuf[INET6_ADDRSTRLEN];
          getsockname(*accepted_fd, (struct sockaddr *) &srvr, &srvrlen);
          inet_ntop(AF_INET6, &srvr.sin6_addr, addrbuf, INET6_ADDRSTRLEN);
          *server_addr = std::string(addrbuf);
          getnameinfo(reinterpret_cast<struct sockaddr *>(&srvr), srvrlen, hname, 1024, nullptr, 0, 0);
          *server_dnsname = std::string(hname);
      }
  return true;
}

}  // namespace hw4

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

#include "./QueryProcessor.h"

#include <iostream>
#include <algorithm>

extern "C" {
  #include "./libhw1/CSE333.h"
}

using std::list;
using std::sort;
using std::string;
using std::vector;

namespace hw3 {

static vector<QueryProcessor::QueryResult> 
LookUpThisTable(const vector<string> &query, DocTableReader* dtr, IndexTableReader* itr);

QueryProcessor::QueryProcessor(const list<string> &indexlist, bool validate) {
  // Stash away a copy of the index list.
  indexlist_ = indexlist;
  arraylen_ = indexlist_.size();
  Verify333(arraylen_ > 0);

  // Create the arrays of DocTableReader*'s. and IndexTableReader*'s.
  dtr_array_ = new DocTableReader *[arraylen_];
  itr_array_ = new IndexTableReader *[arraylen_];

  // Populate the arrays with heap-allocated DocTableReader and
  // IndexTableReader object instances.
  list<string>::const_iterator idx_iterator = indexlist_.begin();
  for (int i = 0; i < arraylen_; i++) {
    FileIndexReader fir(*idx_iterator, validate);
    dtr_array_[i] = fir.NewDocTableReader();
    itr_array_[i] = fir.NewIndexTableReader();
    idx_iterator++;
  }
}

QueryProcessor::~QueryProcessor() {
  // Delete the heap-allocated DocTableReader and IndexTableReader
  // object instances.
  Verify333(dtr_array_ != nullptr);
  Verify333(itr_array_ != nullptr);
  for (int i = 0; i < arraylen_; i++) {
    delete dtr_array_[i];
    delete itr_array_[i];
  }

  // Delete the arrays of DocTableReader*'s and IndexTableReader*'s.
  delete[] dtr_array_;
  delete[] itr_array_;
  dtr_array_ = nullptr;
  itr_array_ = nullptr;
}

// This structure is used to store a index-file-specific query result.
typedef struct {
  DocID_t docid;  // The document ID within the index file.
  int rank;       // The rank of the result so far.
} IdxQueryResult;

vector<QueryProcessor::QueryResult>
QueryProcessor::ProcessQuery(const vector<string> &query) {
  Verify333(query.size() > 0);

  // STEP 1.
  // (the only step in this file)
  vector<QueryProcessor::QueryResult> finalresult;

  if (query.size() == 0){
    //No word in query.
    return finalresult;
  }

  for (int i = 0; i < arraylen_; i++) {
    vector<QueryProcessor::QueryResult> thisTable;
    DocTableReader *dtr = dtr_array_[i];
    IndexTableReader *itr = itr_array_[i];
    thisTable = LookUpThisTable(query, dtr, itr);
    finalresult.insert(finalresult.end(), thisTable.begin(), thisTable.end());
  }

  // Sort the final results.
  sort(finalresult.begin(), finalresult.end());
  return finalresult;
}

static vector<QueryProcessor::QueryResult> 
LookUpThisTable(const vector<string> &query, DocTableReader* dtr, IndexTableReader* itr) {
  vector<QueryProcessor::QueryResult> res; 

  // Search the first word, and return empty vector if no file contains the word.
  DocIDTableReader *firstRead = itr->LookupWord(query[0]);
  if (firstRead == nullptr) {
    delete firstRead;
    return res;
  }

  // Store the qualified Files to qualifiedFilesList
  list<DocIDElementHeader> qualifiedFilesList = firstRead->GetDocIDList();
  delete firstRead;

  // if there has more than 1 word ro search.
  for (uint8_t i = 1; i < query.size(); ++i) {

    //Rearch next word, return empty vector if no file contains the next word.
    DocIDTableReader *docID = itr->LookupWord(query[i]);
    if (docID == nullptr) {
      delete docID;
      return res;
    }
        
    // Update the qualifiedFilesList
    for (auto it = qualifiedFilesList.begin(); it != qualifiedFilesList.end(); it++) {
      list<DocPositionOffset_t> val;
      // Found, add val.size() to this file
      if (docID->LookupDocID(it->docID, &val)) {
        it->numPositions += val.size();
        // Missed, remove this file from qualifiedFilesList
      } else {
        it = qualifiedFilesList.erase(it);
        it--;
      }
    }
    delete docID;
  }
   
    // Return Search result of this table.
    string filename;
    for (auto it = qualifiedFilesList.begin(); it != qualifiedFilesList.end(); it++) {
      Verify333(dtr->LookupDocID(it->docID, &filename));
      res.push_back({filename, it->numPositions});
    }
    return res;
}

}  // namespace hw3

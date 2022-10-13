Build using my memmove like this:  
      make -C c-timer-lib-master/  
      gcc -DMEMMOVE=my_memmove -g -Wall -Wextra -std=gnu99 *.c c-timer-lib-master/timer.o -o memmove_test.out

Build using standard library memmove like this;  
      make -C c-timer-lib-master/  
      gcc -DMEMMOVE=memmove -g -Wall -Wextra -std=gnu99 *.c c-timer-lib-master/timer.o -o memmove_test.out


Performance of my implementation vs. standard library:  
      My experiment did the example that we have learned from class.  
      I created two string, first one was src[] = "1234567890" and second one was dest[] = "abcdefghij".  
      Then I called MEMMOVE(dest, src+1, 9) and MEMMOVE(dest, src+2, 1) while timing the runtime.  
      The result was that the standard library memmove runs twice as fast as my memmove.  

    Reminder: the -O3 switch to gcc will turn on aggressive
    compiler optimizations.  They can make using gdb more
    confusing while debugging, but can also substantially
    reduce execution time once the code works.>

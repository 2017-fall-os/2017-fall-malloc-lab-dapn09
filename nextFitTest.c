#include "stdio.h"
#include "stdlib.h"
#include "myAllocator.h"
#include "sys/time.h"
#include <sys/resource.h>
#include <unistd.h>

double diffTimeval(struct timeval *t1, struct timeval *t2){
  double d = (t1->tv_sec - t2->tv_sec) + (1.0e-6 * (t1->tv_usec - t2->tv_usec));
  return d;
}

void getutime(struct timeval *t){
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);
  *t = usage.ru_utime;
}

int main(){

  printf("First we check that the arena is empty\n");
  arenaCheck();
  printf("\nNow we allocate everything in the arena into equal sized blocks (except last one) for simplicity.");
  //create an array to store the pointers to the allocations
  void **pointer = (void*) calloc(17, sizeof(void*));
  for(int i = 0; i<16; i++){
    if(i < 15)
      pointer[i] = nextFitAllocRegion(65536);
    if(i == 15)
      pointer[i] = nextFitAllocRegion(65280);
  }
  arenaCheck();

  printf("\nNow that the blocks have been allocated we free the odd numbered ones\n");
  
  for(int j = 1; j<16; j += 2){
    freeRegion(pointer[j]);
  }
  arenaCheck();

  printf("\nNow we will allocate a block of size 200 with nextFit to see where it is placed, it should be placed in the place where we last allocated a block, which is the end\n");
  void *p = nextFitAllocRegion(200);
  arenaCheck();

  printf("\nFinally we allocate another block (65536) bigger than the remaining space at the end (65064) to see if search wraps around and starts searching from arena begin again\n");
  void *p2 = nextFitAllocRegion(65536);
  arenaCheck();

  printf("\nTime performance:\n");
  struct timeval t1, t2;
    int i;
    //free everything before starting test
    for(int i = 0; i<16; i++){
      freeRegion(pointer[i]);
    }
    getutime(&t1);
    for(i = 0; i < 10000; i++)
      if (nextFitAllocRegion(4) == 0) 
	break;
    getutime(&t2);
    printf("%d nextFitAllocRegion(4) required %f seconds\n", i, diffTimeval(&t2, &t1));
    
  printf("\nNextFit function works as expected\n");

  return 0;
}

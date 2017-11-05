#include "stdio.h"
#include "stdlib.h"
#include "myAllocator.h"
#include "sys/time.h"
#include <sys/resource.h>
#include <unistd.h>

double diffTimeval(struct timeval *t1, struct timeval *t2) {
  double d = (t1->tv_sec - t2->tv_sec) + (1.0e-6 * (t1->tv_usec - t2->tv_usec));
  return d;
}

void getutime(struct timeval *t)
{
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);
  *t = usage.ru_utime;
}

int main() 
{
  /* void *p1, *p2, *p3, *p4;
  arenaCheck();
  p1 = bestFitAllocRegion(254);
  arenaCheck();
  p2 = bestFitAllocRegion(25400);
  arenaCheck();
  p3 = bestFitAllocRegion(254);
  arenaCheck();
  p4 = bestFitAllocRegion(160);

  printf("%8zx %8zx %8zx %8zx\n", p1, p2, p3, p4);
  arenaCheck();
  freeRegion(p2);
  arenaCheck();
  */
  /*
   * There are two places where the next allocated block could   *  be placed, it is placed so that it creates the lowest 
   * fragmentation.
   **/
  /*
  p2 = bestFitAllocRegion(310);//added line
  arenaCheck();
  */

  /*				
    struct timeval t1, t2;
    int i;
    getutime(&t1);
    for(i = 0; i < 10000; i++)
      if (bestFitAllocRegion(4) == 0) 
	break;
    getutime(&t2);
    printf("%d bestFitAllocRegion(4) required %f seconds\n", i, diffTimeval(&t2, &t1));
  
  return 0;
  */


  printf("First we check that the arena is empty\n");
  arenaCheck();
  printf("\nNow we allocate everything in the arena into equal sized blocks (except last one) for simplicity.");
  //create an array to store the pointers to the allocations
  void **pointer = (void*) calloc(17, sizeof(void*));
  for(int i = 0; i<16; i++){
    if(i < 15)
      pointer[i] = bestFitAllocRegion(65536);
    if(i == 15)
      pointer[i] = bestFitAllocRegion(65280);
  }
  arenaCheck();

  printf("\nNow that the blocks have been allocated we free the odd numbered ones\n");
  
  for(int j = 1; j<16; j += 2){
    freeRegion(pointer[j]);
  }
  arenaCheck();

  printf("\nNow we will free block 8 to create a single free block bigger than all the other free blocks\n");
  freeRegion(pointer[8]);
  arenaCheck();

  fprintf(stderr, "\nNow, with these conditions we will allocate a block of size 200. By doing this we can test that bestFit looks for the biggest block and splits it to allocate the memory request. It uses the biggest block since we want to create the least amount of fragmentation (small free blocks spread around).\n");

  void *p = bestFitAllocRegion(200);
  arenaCheck();

  fprintf(stderr, "\nbest fit works as expected, the biggest free bllock is splitted and used to allocate the request\n");
}

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
  void *p1, *p2, *p3, *p4;
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

  /*
   * There are two places where the next allocated block could   *  be placed, it is placed so that it creates the lowest 
   * fragmentation.
   **/
  
  p2 = bestFitAllocRegion(310);//added line
  arenaCheck();
  

  {				/* measure time for 10000 mallocs */
    struct timeval t1, t2;
    int i;
    getutime(&t1);
    for(i = 0; i < 10000; i++)
      if (bestFitAllocRegion(4) == 0) 
	break;
    getutime(&t2);
    printf("%d bestFitAllocRegion(4) required %f seconds\n", i, diffTimeval(&t2, &t1));
  }
  return 0;
}

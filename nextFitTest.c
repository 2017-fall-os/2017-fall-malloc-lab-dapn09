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
  void *p1, *p2, *p3, *p4;
  arenaCheck();
  p1 = nextFitAllocRegion(254);
  arenaCheck();
  p2 = nextFitAllocRegion(25400);
  arenaCheck();
  p3 = nextFitAllocRegion(254);
  arenaCheck();
  p4 = nextFitAllocRegion(160);

  printf("%8zx %8zx %8zx %8zx\n", p1, p2, p3, p4);
  arenaCheck();
  freeRegion(p3);
  arenaCheck();

  /** 
   * these next two lines check that nextFitAllocRegion keeps    * track of the last allocated block and resumes searches
   * from it. In the code before we have created the situation
   * in which there is a block that can fit the request at the
   * beginning of the arena but since we start search in the 
   * last block allocated we place the 200 right next to 
   * the last. This wouldn't happen with firstFitAllocRegion.
  **/
  
  p3 = nextFitAllocRegion(200);//added line
  arenaCheck();

  /* measure time for 10000 mallocs */
    struct timeval t1, t2;
    int i;
    getutime(&t1);
    for(i = 0; i < 10000; i++)
      if (nextFitAllocRegion(4) == 0) 
	break;
    getutime(&t2);
    printf("%d nextFitAllocRegion(4) required %f seconds\n", i, diffTimeval(&t2, &t1));
  
  return 0;
}

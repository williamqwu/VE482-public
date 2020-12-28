// Assume that we're using a computer with physical memory of 16 GB, and some programs have already taken 2GB memory.
#include <stdio.h>
#include <stdlib.h>

typedef struct _Big{
  int *d1; // 1,073,741,824 * 4 = 4GB
  int *d2; // 4GB
  int *d3; // 4GB
  int *d4; // 4GB
  int ok1,ok2,ok3;
} Big;

void initBit(Big *data);

int main(){
  Big data;
  data.ok1=data.ok2=data.ok3=0;
  initBit(&data);
  free(data.d1);
  free(data.d2);
  free(data.d3);
  free(data.d4);
  return 0;
}

void initBit(Big *data){
  data->d1 = (int *)malloc(sizeof(int)*1073741824);
  printf("d1 init ok\n");
  data->d2 = (int *)malloc(sizeof(int)*1073741824);
  printf("d2 init ok\n");
  data->d3 = (int *)malloc(sizeof(int)*1073741824);
  printf("d3 init ok\n");
  // critical point, after that thrashing happens
  data->d4 = (int *)malloc(sizeof(int)*1073741824);
  printf("d4 init ok\n");
  for(int i=0;i<1073741824;i++) (data->d1)[i]=i;
  printf("d1 calc ok\n");
  for(int i=0;i<1073741824;i++) (data->d2)[i]=i;
  printf("d2 calc ok\n");
  for(int i=0;i<1073741824;i++) (data->d3)[i]=i;
  printf("d3 calc ok\n");
  // critical point, after that thrashing happens
  for(int i=0;i<1073741824;i++) (data->d4)[i]=i;
  printf("d4 calc ok\n");
}
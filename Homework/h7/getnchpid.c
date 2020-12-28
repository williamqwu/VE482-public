#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#include "mproc.h"

#define OK 0
/**
 * @brief helper function that retrieves the n^{th} child process
 * 
 * @param n next_child
 * @param childpid array that stores the pid of the child processes
 * @return int 
 */
int getnchpid(int n, pid_t *childpid){
  register struct mproc *rmc; // pointer to child
  if (childpid == NULL) return -1;
  if (n > NR_PROCS) return -1;
  rmc = &mproc[n];
  if (rmc->mp_parent != who_p) return -1;
  *childpid = rmc->mp_pid;

  return OK;
  // on error return -1
}

/**
 * @brief write the pids of up to n children of the current process into *childpid
 * 
 * @param n 
 * @param childpid 
 * @return int 
 */
int getchpids(int n, pid_t *childpid){
  int index;
  for (index = 0; index < n; index++){
    if (getnchpid(index, childpid+index) != OK){
      index = -1;
      break;
    }
  }

  return index;
}

int main(){
  pid_t childpid[10];
  int childNum = 0;
  pid_t childpidRef[10];

  while(childNum <= 4){
    pid_t pid = fork();
    if (pid == 0){
      childpidRef[childNum++] = pid;
    }
    else{
      exit (0);
    }
  }
  
  getchpid(childNum, childpid);
  
  for(int i=0;i<childNum;i++){
    assert(childpidRef[i] == childpid[i]);
  }

  return 0;
}
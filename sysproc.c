#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "thread.h"

int
sys_tim(void)
{
  return ticks;
}

int
sys_thread(void)
{
  int pid;
  int stack;
  int routine;
  int args;
  struct proc *np;
 
 if(argint(0, &stack) < 0 || argint(1, &routine) < 0 || argint(2, &args) < 0)
    return -1;
 
  if((np = thread(cp, (int)stack, (int)routine, (int)args)) == 0){
    return -2;
   }
 
  np->state = RUNNABLE;
  pid = np->pid;
  return pid;
}

int sys_thread_wait(void)
{
  while(thread_wait() != -1);
  return;
}

int
sys_fork(void)
{
  int pid;
  struct proc *np;

  if((np = copyproc(cp)) == 0)
    return -1;
  pid = np->pid;
  np->state = RUNNABLE;
  return pid;
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return cp->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  if((addr = growproc(n)) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n, ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(cp->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

int
sys_cond_sleep(void)
{
  void *chan;
  argptr(0, &chan, 4);
  sleep_cond(chan);
  return 0;
}

int
sys_cond_wake(void)
{
  void *chan;
  argptr(0, &chan, 4);
  wake_cond(chan);
  return 0;
}

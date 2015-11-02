#include <stdio.h>
#include <ucontext.h>
#include <stdlib.h>

#define MEM 64000 //memory allocated to each thread
/*


The ucontext_t type is a structure that has at least the following fields:

           typedef struct ucontext {
               struct ucontext *uc_link;
               sigset_t         uc_sigmask;
               stack_t          uc_stack;
               mcontext_t       uc_mcontext;
               ...
           } ucontext_t;

       with sigset_t and stack_t defined in <signal.h>.  Here uc_link points to the context that will be resumed when the current context terminates (in  case
       the  current  context was created using makecontext(3)), uc_sigmask is the set of signals blocked in this context (see sigprocmask(2)), uc_stack is the
       stack used by this context (see sigaltstack(2)), and uc_mcontext is the machine-specific representation of the saved context, that includes the calling
       thread's machine registers.


*/
ucontext_t t1,t2,a,Main;
/* 
t1 for function 1
t2 for function 2
a for function 1
Main for the Main context

*/



void function1(){
	printf("This is function 1\n");
	setcontext(&Main);
}

void function2(){
	printf("This is function 2. Switching to function 1 \n");
	setcontext(&a);
	printf("Switched over to function 1\n"); // never gets printed as context is switched

}


void init(){
	getcontext(&Main);
	getcontext(&a);
	a.uc_link = 0;
	a.uc_stack.ss_sp = malloc(MEM);
	a.uc_stack.ss_size = MEM;
	a.uc_stack.ss_flags = 0;
	makecontext(&a, (void*)&function1, 0);
}


int main(){
	init();
	//thread 1
	getcontext(&t1);
	t1.uc_link = 0;
	t1.uc_stack.ss_sp = malloc(MEM);
	t1.uc_stack.ss_size = MEM;
	t1.uc_stack.ss_flags = 0;
	makecontext(&t1, (void*)&function1, 0);
	swapcontext(&Main, &t1);
	//thread 2
	getcontext(&t2);
	t2.uc_link = 0;
	t2.uc_stack.ss_sp = malloc(MEM);
	t2.uc_stack.ss_size = MEM;
	t2.uc_stack.ss_flags = 0;
	makecontext(&t2, (void*)&function2, 0);
	swapcontext(&Main, &t2);

	printf("Completed\n");
	exit(0);
}

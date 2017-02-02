#ifndef PORT_H
#define PORT_H

// these macros provide the wrappers for critical regions, where ints will be off
// but the state of the interrupt enable prior to entry will be restored.
extern unsigned char _CCR_temp;

#define EnterCritical()     
#define ExitCritical()      


#endif

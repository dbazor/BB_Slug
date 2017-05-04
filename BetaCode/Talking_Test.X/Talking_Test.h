/* 
 * File:   Talking_Test.h
 * Author: Daniel
 *
 * Created on February 2, 2017, 3:38 PM
 */

#ifndef TALKING_TEST_H
#define	TALKING_TEST_H

#include <xc.h>                     // processor SFR definitions
#include <sys/attribs.h>            // __ISR macro


#define SYS_FREQ 80000000ul    // 80 million Hz

//void NU32_Startup(void);
//void ReadUART1(char * string, int maxLength);
//void WriteUART1(const char * string);

//suppresses annoying warnings
#ifndef _SUPPRESS_PLIB_WARNING
#define _SUPPRESS_PLIB_WARNING
#endif

#ifndef _DISABLE_OPENADC10_CONFIGPORT_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#endif

#endif	/* TALKING_TEST_H */


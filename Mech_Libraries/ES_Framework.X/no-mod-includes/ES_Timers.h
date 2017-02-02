/****************************************************************************
 Module
         ES_Timers.h

 Revision
         1.0.1

 Description
         Header File for the ME218 Timer Module

 Notes


 History
 When           Who	What/Why
 -------------- ---	--------
 01/15/12 16:43 jec  converted for Gen2 of the Events & Services Framework
 10/21/11 18:33 jec  Begin conversion for use with the new Event Framework
 09/01/05 12:29 jec  Converted rates and return values to enumerated constants
 06/15/04 09:58 jec  converted all prefixes to EF_Timer
 05/27/04 15:04 jec  revised TMR_RATE macros for the S12
 11/24/00 13:17 jec  revised TMR_RATE macros for the HC12
 02/20/99 14:11 jec  added #defines for TMR_ERR, TMR_ACTIVE, TMR_EXPIRED
 02/24/97 14:28 jec  Began Coding
****************************************************************************/

#ifndef ES_Timers_H
#define ES_Timers_H



typedef enum { ES_Timer_ERR           = -1,
               ES_Timer_ACTIVE        =  1,
               ES_Timer_OK            =  0,
               ES_Timer_NOT_ACTIVE    =  0
} ES_TimerReturn_t;


/**
 * @Function ES_Timer_Init(void)
 * @param none
 * @return None.
 * @brief  Initializes the timer module
 * @author Max Dunne, 2011.11.15 */
void             ES_Timer_Init(void);

/**
 * @Function ES_Timer_InitTimer(uint8_t Num, uint32_t NewTime)
 * @param Num -  the number of the timer to start
 * @param NewTime - the number of tick to be counted
 * @return ERROR or SUCCESS
 * @brief  sets the NewTime into the chosen timer and clears any previous event flag
 * and sets the timer actice to begin counting.
 * @author Max Dunne 2011.11.15 */
ES_TimerReturn_t ES_Timer_InitTimer(uint8_t Num, uint32_t NewTime);

/**
 * @Function ES_Timer_SetTimer(uint8_t Num, uint32_t NewTime)
 * @param Num - the number of the timer to set.
 * @param NewTime -  the number of milliseconds to be counted
 * @return ERROR or SUCCESS
 * @brief  sets the time for a timer, but does not make it active.
 * @author Max Dunne  2011.11.15 */
ES_TimerReturn_t ES_Timer_SetTimer(uint8_t Num, uint32_t NewTime);

/**
 * @Function ES_Timer_StartTimer(uint8_t Num)
 * @param Num - the number of the timer to start
 * @return ERROR or SUCCESS
 * @brief  simply sets the active flag in TMR_ActiveFlags to resart a stopped timer.
 * @author Max Dunne, 2011.11.15 */
ES_TimerReturn_t ES_Timer_StartTimer(uint8_t Num);

/**
 * @Function ES_Timer_StopTimer(unsigned char Num)
 * @param Num - the number of the timer to stop.
 * @return ERROR or SUCCESS
 * @brief  simply clears the bit in TimerActiveFlags associated with this timer. This
 * will cause it to stop counting.
 * @author Max Dunne 2011.11.15 */
ES_TimerReturn_t ES_Timer_StopTimer(uint8_t Num);

/**
 * Function: ES_Timer_GetTime(void)
 * @param None
 * @return FreeRunningTimer - the current value of the module variable FreeRunningTimer
 * @remark Provides the ability to grab a snapshot time as an alternative to using
 * the library timers. Can be used to determine how long between 2 events.
 * @author Max Dunne, 2011.11.15  */
uint32_t         ES_Timer_GetTime(void);

#endif   /* ES_Timers_H */
/*------------------------------ End of file ------------------------------*/



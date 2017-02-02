/****************************************************************************
 Module
     EaS_LookupTables.c
 Description
     This is the home for a set of constant lookup tables that are used in
     multiple places in the framework and beyond. 
     
 Notes
     As a rule, I don't approve of global variables for a host of reasons.
     In this case I decided to make them global in the interests of
     efficiency. These tables will be references very often in the timer
     functions (as often as 8 times/ms) and on every pass through the event
     scheduler/dispatcher. As a result I decided to simply make them global.
     Since they are constant, they are not subject to the multiple access
     point issues associated with modifiable global variables.

 History
 When           Who     What/Why
 -------------- ---     --------
 01/15/12 09:07 jec      change of heart: converted to global vars for the
                         tables and removed the access functions
 01/12/12 10:25 jec      adding the Ready variable and management functions.
 01/03/12 11:16 jec      started coding
*****************************************************************************/
/*----------------------------- Include Files -----------------------------*/

#include <inttypes.h>

/*----------------------------- Module Defines ----------------------------*/

/*---------------------------- Module Functions ---------------------------*/

/*---------------------------- Module Variables ---------------------------*/

/*
  this table is used to go from a bit number (0-7) to the mask used to clear
  that bit in a byte. Technically, this is not necessary (as you could always
  complement the SetMask) but it will save the complement operation every 
  time it is used to clear a bit. If we clear a bit with it in more than 8
  places, then it is a win on code size and speed.
*/
uint8_t const BitNum2ClrMask[] = {
  0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F
};

/*
  this table is used to go from a bit number (0-7) to the mask used to set
  that bit in a byte.
*/
uint8_t const BitNum2SetMask[] = {
  0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

/*
  this table is used to go from an unsigned 8bit value to the most significant
  bit that is set in that byte. It is used in the determination of priorities
  from the Ready variable and in determining active timers in 
  the timer interrupt response. Index into the array with (ByteVal-1) to get 
  the correct MS Bit num.
*/
uint8_t const Byte2MSBitNum[255] = {
    0U, 1U, 1U, 2U, 2U, 2U, 2U, 3U, 3U, 3U, 3U, 3U, 3U, 3U, 3U, 4U,
    4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U, 5U,
    5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U,
    5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 5U, 6U,
    6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U,
    6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U,
    6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U,
    6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 6U, 7U,
    7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U,
    7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U,
    7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U,
    7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U,
    7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U,
    7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U,
    7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U,
    7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U, 7U
};

/*------------------------------ Module Code ------------------------------*/


/***************************************************************************
 private functions
 ***************************************************************************/

/*------------------------------ End of File ------------------------------*/


  
/****************************************************************************
 Module
     ES_LookupTables.h
 Description
     Extern declarations for a set of constant lookup tables that are used in
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
 01/15/12 13:03 jec      started coding
*****************************************************************************/
/*
  this table is used to go from a bit number (0-7) to the mask used to clear
  that bit in a byte. Technically, this is not necessary (as you could always
  complement the SetMask) but it will save the complement operation every 
  time it is used to clear a bit. If we clear a bit with it in more than 8
  places, then it is a win on code size and speed.
*/
extern uint8_t const BitNum2ClrMask[];

/*
  this table is used to go from a bit number (0-7) to the mask used to set
  that bit in a byte.
*/
extern uint8_t const BitNum2SetMask[];

/*
  this table is used to go from an unsigned 8bit value to the most significant
  bit that is set in that byte. It is used in the determination of priorities
  from the Ready variable and in determining active timers in 
  the timer interrupt response. Index into the array with (ByteVal-1) to get 
  the correct MS Bit num.
*/
extern uint8_t const Byte2MSBitNum[255];

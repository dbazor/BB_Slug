/****************************************************************************
 Module
     ES_Events.h
 Description
     header file with definitions for the event structure 
 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
 01/15/12 11:46 jec      moved event enum to config file, changed prefixes to ES
 10/23/11 22:01 jec      customized for Remote Lock problem
 10/17/11 08:47 jec      changed event prefixes to EF_ to match new framework
 08/09/06 14:30 jec      started coding
*****************************************************************************/

#ifndef ES_Events_H
#define ES_Events_H

//#include "stdint.h"
#include <inttypes.h>

typedef struct ES_Event_t {
    ES_EventTyp_t EventType;    // what kind of event?
    uint16_t   EventParam;      // parameter value for use w/ this event
}ES_Event;

#define INIT_EVENT  (ES_Event){ES_INIT,0x0000}
#define ENTRY_EVENT (ES_Event){ES_ENTRY,0x0000}
#define EXIT_EVENT  (ES_Event){ES_EXIT,0x0000}
#define NO_EVENT (ES_Event){ES_NO_EVENT,0x0000}

#endif /* ES_Events_H */

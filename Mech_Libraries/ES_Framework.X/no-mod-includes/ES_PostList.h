/****************************************************************************
 Module
     EF_PostList.h
 Description
     header file for use with the module to post events to lists of state
     machines
 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
 01/15/12 11:57 jec      modified includes to match Events & Services
 10/16/11 12:28 jec      started coding
*****************************************************************************/
#ifndef ES_PostList_H
#define ES_PostList_H


#include "ES_Events.h"
#include <inttypes.h>

typedef uint8_t PostFunc_t( ES_Event );

typedef PostFunc_t (*pPostFunc);

uint8_t ES_PostList00( ES_Event);
uint8_t ES_PostList01( ES_Event);
uint8_t ES_PostList02( ES_Event);
uint8_t ES_PostList03( ES_Event);
uint8_t ES_PostList04( ES_Event);
uint8_t ES_PostList05( ES_Event);
uint8_t ES_PostList06( ES_Event);
uint8_t ES_PostList07( ES_Event);

#endif // ES_PostList_H
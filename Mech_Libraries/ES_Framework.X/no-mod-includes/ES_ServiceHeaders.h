/****************************************************************************
 Module
     ES_ServiceHeaders.h
 Description
     This file serves to keep the clutter down in ES_Framework.h
 Notes
 History
 When           Who     What/Why
 -------------- ---     --------
 01/15/12 10:35 jec      started coding
*****************************************************************************/

#include "ES_Configure.h"

#include SERV_0_HEADER

#if NUM_SERVICES > 1
#include SERV_1_HEADER
#endif


#if NUM_SERVICES > 2
#include SERV_2_HEADER
#endif

#if NUM_SERVICES > 3
#include SERV_3_HEADER
#endif

#if NUM_SERVICES > 4
#include SERV_4_HEADER
#endif

#if NUM_SERVICES > 5
#include SERV_5_HEADER
#endif

#if NUM_SERVICES > 6
#include SERV_6_HEADER
#endif

#if NUM_SERVICES > 7
#include SERV_7_HEADER
#endif


/****************************************************************************
 Module
     EF_PostList.c
 Description
     source file for the module to post events to lists of state
     machines
 Notes
     
 History
 When           Who     What/Why
 -------------- ---     --------
 01/15/12 15:55 jec      re-coded for Gen2 with conditional declarations
 10/16/11 12:32 jec      started coding
*****************************************************************************/
/*----------------------------- Include Files -----------------------------*/

#include "ES_Configure.h"
#include "ES_General.h"
#include "ES_PostList.h"
#include "ES_ServiceHeaders.h"

/*---------------------------- Module Functions ---------------------------*/
static uint8_t PostToList(  PostFunc_t *const*FuncList, unsigned char ListSize, ES_Event NewEvent);

/*---------------------------- Module Variables ---------------------------*/
// Fill in these arrays with the lists of posting funcitons for the state
// machines that will have common events delivered to them.

#if NUM_DIST_LISTS > 0
static PostFunc_t * const DistList00[] = {DIST_LIST0 };
// the endif for NUM_DIST_LISTS > 0 is at the end of the file
#if NUM_DIST_LISTS > 1
static PostFunc_t * const DistList01[] = {DIST_LIST1 };
#endif
#if NUM_DIST_LISTS > 2
static PostFunc_t * const DistList02[] = {DIST_LIST2 };
#endif
#if NUM_DIST_LISTS > 3
static PostFunc_t * const DistList03[] = {DIST_LIST3 };
#endif
#if NUM_DIST_LISTS > 4
static PostFunc_t * const DistList04[] = {DIST_LIST4 };
#endif
#if NUM_DIST_LISTS > 5
static PostFunc_t * const DistList05[] = {DIST_LIST5 };
#endif
#if NUM_DIST_LISTS > 6
static PostFunc_t * const DistList06[] = {DIST_LIST6 };
#endif
#if NUM_DIST_LISTS > 7
static PostFunc_t * const DistList07[] = {DIST_LIST7 };
#endif

/*------------------------------ Module Code ------------------------------*/

// Each of these list-specific functions is a wrapper that calls the generic
// function to walk through the list, calling the listed posting functions

/****************************************************************************
 Function
   PostListxx
 Parameters
   ES_Event NewEvent : the new event to be passed to each of the state machine
   posting functions in list xx
 Returns
   TRUE if all the post functions succeeded, FALSE if any failed
 Description
   Posts NewEvent to all of the state machines listed in the list
 Notes
   
 Author
   J. Edward Carryer, 10/24/11, 07:48
****************************************************************************/
uint8_t ES_PostList00( ES_Event NewEvent) {
  return PostToList( DistList00, ARRAY_SIZE(DistList00), NewEvent);
}

#if NUM_DIST_LISTS > 1
uint8_t ES_PostList01( ES_Event NewEvent) {
  return PostToList( DistList01, ARRAY_SIZE(DistList01), NewEvent);
}
#endif

#if NUM_DIST_LISTS > 2
uint8_t ES_PostList02( ES_Event NewEvent) {
  return PostToList( DistList02, ARRAY_SIZE(DistList02), NewEvent);
}
#endif

#if NUM_DIST_LISTS > 3
uint8_t ES_PostList03( ES_Event NewEvent) {
  return PostToList( DistList03, ARRAY_SIZE(DistList03), NewEvent);
}
#endif

#if NUM_DIST_LISTS > 4
uint8_t ES_PostList04( ES_Event NewEvent) {
  return PostToList( DistList04, ARRAY_SIZE(DistList04), NewEvent);
}
#endif

#if NUM_DIST_LISTS > 5
uint8_t ES_PostList05( ES_Event NewEvent) {
  return PostToList( DistList05, ARRAY_SIZE(DistList05), NewEvent);
}
#endif

#if NUM_DIST_LISTS > 6
uint8_t ES_PostList06( ES_Event NewEvent) {
  return PostToList( DistList06, ARRAY_SIZE(DistList06), NewEvent);
}
#endif

#if NUM_DIST_LISTS > 7
uint8_t ES_PostList07( ES_Event NewEvent) {
  return PostToList( DistList07, ARRAY_SIZE(DistList07), NewEvent);
}
#endif

// Implementations for private functions
/****************************************************************************
 Function
   PostToList
 Parameters
   PostFunc *const*List : pointer to the list of posting functions
   unsigned char ListSize : number of elements in the list array 
   EF_Event NewEvent : the new event to be passed to each of the state machine
   posting functions in the list
 Returns
   TRUE if all the post functions succeeded, FALSE if any failed
 Description
   Posts NewEvent to all of the state machines listed in the list
 Notes
   
 Author
   J. Edward Carryer, 10/24/11, 07:52
****************************************************************************/
static uint8_t PostToList( PostFunc_t *const*List, unsigned char ListSize, ES_Event NewEvent){
  unsigned char i;
  // loop through the list executing the post functions
  for ( i=0; i< ListSize; i++) {
    if ( List[i](NewEvent) != TRUE )
      break; // this is a failed post
  }
  if ( i != ListSize ) // if no failures, i = ListSize
    return (FALSE);
  else
    return(TRUE);
}
#endif /* NUM_DIST_LISTS > 0*/

/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/

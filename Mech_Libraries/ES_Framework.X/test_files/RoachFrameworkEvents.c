#include <BOARD.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "RoachFrameworkEvents.h"
#ifdef DEV
#include "ES_ServiceHeaders.h"
#include "ES_Events.h"
#endif
#include "roach.h"


#define DARK_THRESHOLD 234
#define LIGHT_THRESHOLD 88

uint8_t CheckLightLevel(void)
{
    static lightstate_t lastLightState = DARK;
    ES_Event thisEvent;
    lightstate_t currentLightState;
    unsigned int currentLightValue;
    uint8_t returnVal = FALSE;
    // check the light level and assign LIGHT or DARK
    currentLightValue = Roach_LightLevel();
    if (currentLightValue > DARK_THRESHOLD) {
        currentLightState = DARK;
    }
    if (currentLightValue < LIGHT_THRESHOLD) {
        currentLightState = LIGHT;
    }
    if (currentLightState != lastLightState) { //event detected
        if (currentLightState == LIGHT) {
            thisEvent.EventType = WAS_DARK_NOW_LIGHT;
        } else {
            thisEvent.EventType = WAS_LIGHT_NOW_DARK;
        }
        thisEvent.EventParam = (uint16_t) currentLightState;
        PostFancyRoachHSM(thisEvent);
        returnVal = TRUE;
        lastLightState = currentLightState;
    }
    return returnVal;
}

uint8_t CheckBumps(void)
{
    static int lastBumpState = 0;
    int currentBumpState;
    ES_Event thisEvent;

    uint8_t returnVal = FALSE;
    // check the light level and assign LIGHT or DARK
    currentBumpState = Roach_ReadBumpers();
    if (currentBumpState != lastBumpState) { //event detected
        thisEvent.EventType = BUMPED;
        thisEvent.EventParam = (uint16_t) currentBumpState;
        PostFancyRoachHSM(thisEvent);
        returnVal = TRUE;
        lastBumpState = currentBumpState;
    }

    return returnVal;
}


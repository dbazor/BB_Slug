
#include <BOARD.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "ES_TattleTale.h"
#include "ES_KeyboardInput.h"
#include "serial.h"
#include <stdio.h>
#include <peripheral/timer.h>



void ParseCommand(void);
/*---------------------------- Module Defines ---------------------------*/
#define COMMANDSTRINGLENGTH 20
#define TERMINATION_CHARACTER ';'

#define STRINGIFY(x) SSTRINGIFY(x)
#define SSTRINGIFY(x) #x

/*---------------------------- Module Variables ---------------------------*/
// with the introduction of Gen2, we need a module level Priority variable
static uint8_t MyPriority;



static char CommandString[COMMANDSTRINGLENGTH] = {0};

/**
 * @Function InitKeyboardInput(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief this initializes the keyboard input system which
 *        Returns TRUE if successful, FALSE otherwise
 * @author Max Dunne , 2013.09.26 */
uint8_t InitKeyboardInput(uint8_t Priority)
{
    ES_Event ThisEvent;

    MyPriority = Priority;
    // post the initial transition event
    ThisEvent.EventType = ES_INIT;
    if (ES_PostToService(MyPriority, ThisEvent) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @Function PostKeyboardInput(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief Used to post events to keyboard input
 *        Returns TRUE if successful, FALSE otherwise
 * @author Max Dunne , 2013.09.26 */
uint8_t PostKeyboardInput(ES_Event ThisEvent)
{
    return ES_PostToService(MyPriority, ThisEvent);
}

/**
 * @Function RunKeyboardInput(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return ES_NO_EVENT
 * @brief Keyboard input only accepts the ES_KEYINPUT event and will always return
 * ES_NO_EVENT. it parses strings of the form EVENTNUM->EVENTPARAMHEX or
 * EVENTNUM and passes them to the state machine defined by
 * POSTFUNCTION_FOR_KEYBOARD_INPUT.
 * @note WARNING: you must have created the EventNames Array to use this module
 * @author Max Dunne , 2013.09.26 */
ES_Event RunKeyboardInput(ES_Event ThisEvent)
{
#ifdef USE_KEYBOARD_INPUT
    ES_Event GeneratedEvent;
    uint8_t stringPos = 0;
    uint8_t numbersParsed = 0;
    static uint8_t curCommandLength = 0;
    GeneratedEvent.EventType = ES_NO_EVENT;
    GeneratedEvent.EventParam = 0;
    /********************************************
     in here you write your service code
     *******************************************/
    switch (ThisEvent.EventType) {
    case ES_INIT:
        for (stringPos = 0; stringPos < COMMANDSTRINGLENGTH; stringPos++) {
            CommandString[stringPos] = 0;
        }
        curCommandLength = 0;
        while (!IsTransmitEmpty());
        KeyboardInput_PrintEvents();
        while (!IsTransmitEmpty());
        printf("\r\n\r\nKeyboard input is active, "
                "no other events except timer activations will be processed. "
                "You can redisplay the event list by sending a %d event.\r\n"
                "Send an event using the form [event#]; "
                "or [event#]->[EventParam];\r\n", ES_LISTEVENTS);
        break;

    case ES_KEYINPUT:
        if (ThisEvent.EventParam < 127) {
            CommandString[curCommandLength] = (char) ThisEvent.EventParam;
            curCommandLength++;
            if (ThisEvent.EventParam == TERMINATION_CHARACTER) {
                numbersParsed = sscanf(CommandString, "%d -> %X", &GeneratedEvent.EventType, &GeneratedEvent.EventParam);
                if (numbersParsed != 0) {

                    if (GeneratedEvent.EventType < NUMBEROFEVENTS) {
                        switch (GeneratedEvent.EventType) {
                        case ES_LISTEVENTS:
                            KeyboardInput_PrintEvents();
                            break;
                        default:
                            printf("\r\n%s with parameter %X was passed to %s", EventNames[GeneratedEvent.EventType], GeneratedEvent.EventParam, STRINGIFY(POSTFUNCTION_FOR_KEYBOARD_INPUT));
                            POSTFUNCTION_FOR_KEYBOARD_INPUT(GeneratedEvent);
                            break;
                        }
                    } else {
                        printf("\r\nEvent #%d is Invalid, Please try again\r\n", GeneratedEvent.EventType);
                    }
                }
                for (stringPos = 0; stringPos < COMMANDSTRINGLENGTH; stringPos++) {
                    CommandString[stringPos] = 0;

                }
                curCommandLength = 0;
            }
        }
    default:
        break;
    }
#endif
    return (NO_EVENT);
}

/**
 * @Function KeyboardInput_PrintEvents(void)
 * @param None
 * @return None
 * @brief  Lists out all Events in the EventNames array.
 * @author Max Dunne, 2013.09.26 */
void KeyboardInput_PrintEvents(void)
{
#ifdef USE_KEYBOARD_INPUT
    int curEvent = 0;
    printf("\r\nPrinting all events available in the system\n");
    for (curEvent = 0; curEvent < NUMBEROFEVENTS; curEvent++) {
        printf("%2d: %-25s", curEvent, EventNames[curEvent]);
        //while(!IsTransmitEmpty());
        if (((curEvent + 3) % 3) == 0) {
            printf("\r\n");
        }

    }
    printf("\n");
#endif
}
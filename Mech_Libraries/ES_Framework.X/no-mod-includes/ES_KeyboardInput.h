/* 
 * File:   ES_KeyboardInput.h
 * Author: mdunne
 *
 * Created on September 9, 2013, 6:47 PM
 */

#ifndef ES_KEYBOARDINPUT_H
#define	ES_KEYBOARDINPUT_H




/**
 * @Function InitKeyboardInput(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief this initializes the keyboard input system which
 *        Returns TRUE if successful, FALSE otherwise
 * @author Max Dunne , 2013.09.26 */
uint8_t InitKeyboardInput(uint8_t Priority);


/**
 * @Function PostKeyboardInput(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief Used to post events to keyboard input
 *        Returns TRUE if successful, FALSE otherwise
* @author Max Dunne , 2013.09.26 */
uint8_t PostKeyboardInput(ES_Event ThisEvent);


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
ES_Event RunKeyboardInput(ES_Event ThisEvent);


/**
 * @Function KeyboardInput_PrintEvents(void)
 * @param None
 * @return None
 * @brief  Lists out all Events in the EventNames array.
 * @author Max Dunne, 2013.09.26 */
void KeyboardInput_PrintEvents(void);



#endif	/* ES_KEYBOARDINPUT_H */


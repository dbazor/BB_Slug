/* 
 * File:   RoachFrameworkEvents.h
 * Author: mdunne
 *
 * Created on January 3, 2013, 4:53 PM
 */

#ifndef ROACHFRAMEWORKEVENTS_H
#define	ROACHFRAMEWORKEVENTS_H


typedef enum {
    DARK, LIGHT
} lightstate_t;

uint8_t CheckLightLevel(void);
uint8_t CheckBumps(void);


#endif	/* ROACHFRAMEWORKEVENTS_H */


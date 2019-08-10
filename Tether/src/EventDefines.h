/*
 * EventDefines.h
 *
 *  Created on:	09.08.2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_EVENTDEFINES_H_
#define SRC_EVENTDEFINES_H_

//event IDs
#define EVENT_ID_KEY_F3 1111
#define EVENT_ID_KEY_R (1024+('R'-'A'))
#define EVENT_ID_KEY_E (1024+('E'-'A'))
#define EVENT_ID_KEY_ARROW_UP 1097
#define EVENT_ID_KEY_ARROW_DOWN 1098


//status/condition IDs
#define STATUS_ID_DEBUG_SCREEN_ACTIVE 1
#define STATUS_ID_RESTART 2
#define STATUS_ID_INVENTORY 3
#define CONDITION_SELECTION_ACTIVE 4
#define STATUS_ID_SELECTION_UP 5
#define STATUS_ID_SELECTION_DOWN 6

//event values
#define EVENT_VALUE_KEY_PRESSED 1


#endif /* SRC_EVENTDEFINES_H_ */

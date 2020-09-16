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
#define EVENT_ID_KEY_W (1024+('W'-'A'))
#define EVENT_ID_KEY_A (1024+('A'-'A'))
#define EVENT_ID_KEY_S (1024+('S'-'A'))
#define EVENT_ID_KEY_D (1024+('D'-'A'))
#define EVENT_ID_KEY_R (1024+('R'-'A'))
#define EVENT_ID_KEY_E (1024+('E'-'A'))
#define EVENT_ID_KEY_B (1024+('B'-'A'))
#define EVENT_ID_KEY_T (1024+('T'-'A'))
#define EVENT_ID_KEY_P (1024+('P'-'A'))
#define EVENT_ID_KEY_Z (1024+('Z'-'A'))
#define EVENT_ID_KEY_C (1024+('C'-'A'))
#define EVENT_ID_KEY_V (1024+('V'-'A'))
#define EVENT_ID_KEY_ARROW_UP 1097
#define EVENT_ID_KEY_ARROW_DOWN 1098
#define EVENT_ID_KEY_SHIFT 1062
#define EVENT_ID_KEY_CTRL 1061
#define EVENT_ID_KEY_ESCAPE 1060
#define EVENT_ID_MOUSE_LMB 2052
#define EVENT_ID_MOUSE_RMB 2053
#define EVENT_ID_MOUSE_WHEEL 2050
#define EVENT_ID_MOUSE_X 2048
#define EVENT_ID_MOUSE_Y 2049


//status/condition IDs
#define STATUS_ID_DEBUG_SCREEN_ACTIVE 1
#define STATUS_ID_RESTART 2
#define STATUS_ID_INVENTORY 3
#define CONDITION_SELECTION_ACTIVE 4
#define STATUS_ID_SELECTION_UP 5
#define STATUS_ID_SELECTION_DOWN 6
#define STATUS_ID_DRAW_AABBs 7
#define STATUS_ID_GO_UP 8
#define STATUS_ID_GO_DOWN 9
#define STATUS_ID_BENCHMARK 10
#define STATUS_ID_SLOMO 11
#define STATUS_ID_PAUSE 12
#define STATUS_ID_MENU 13
#define STATUS_ID_TRIGGER 14
#define STATUS_ID_WEAPON_SWITCH 15
#define STATUS_ID_ZOOM 16
#define STATUS_ID_WALK_Z 17
#define STATUS_ID_WALK_X 18

//event values
#define EVENT_VALUE_KEY_PRESSED 1


#endif /* SRC_EVENTDEFINES_H_ */

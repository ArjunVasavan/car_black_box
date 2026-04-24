#include <xc.h>

#ifndef BB_H
#define BB_H

/* NOTE: EEPROM LOGGING
 *
 * EEPROM can store upto 255 bytes
 * In this project we are storing maximum 10 logs only. and each log needs 5 pages
 * example: 
 * Page 0  → speed of log 1
 * Page 1  → gear of log 1
 * Page 2  → hours of log 1
 * Page 3  → minutes of log 1
 * Page 4  → seconds of log 1
 * ---------------------------
 * Page 5  → speed of log 2
 * Page 6  → gear of log 2
 * Page 7  → hours of log 2
 * Page 8  → minutes of log 2
 * Page 9  → seconds of log 2
 * ---------------------------
 * ...and so on until page 49 (10 logs × 5 pages = 50 pages)
 * */

// each log record = 5 bytes
#define SPEED_ADDR   0   // offset 0 → speed
#define GEAR_ADDR    1   // offset 1 → gear
#define HOURS_ADDR   2   // offset 2 → hours
#define MINS_ADDR    3   // offset 3 → minutes
#define SECS_ADDR    4   // offset 4 → seconds

#define GEAR_COLLISION 7   // special value to mark collision event

/* NOTE: Control Addresses
 * It is used to remember where it stopped write_index and is the EEPROM full
 */
#define WRITE_INDEX_ADDR  100  // where write index is stored
#define WRITE_FLAG_ADDR   101  // full flag ( 0 = not full, 1 = full )
#define INIT_FLAG_ADDR    102  // its an secret code to know if EEPROM is used before
#define INIT_FLAG_VAL     0xBB // magic number to detect if it is used before or not

/* Enum for maintaining the app state */
typedef enum {
    e_dashboard, e_main_menu, e_view_log, e_set_time, e_download_log, e_clear_log
} State_t;


extern unsigned char write_index;
extern unsigned char write_flag;
extern unsigned char gear;
extern unsigned char speed_val;
extern unsigned char hours;
extern unsigned char minutes;
extern unsigned char seconds;

extern State_t state; // App state

//Function declarations


void convert_speed_to_string(unsigned char* speed, unsigned char spd );

//Dashboard function declaration
void view_dashboard(void);

//Storing events function declaration
void event_store(void);

//Password function declaration
void password(void);

//main menu function declaration
void display_main_menu(void);

//View log function declaration
void view_log(void);

//Reading events function declaration
void event_reader(void);

//Change password function declaration
void change_password(void);

//Set time function declaration
void set_time(void);

//Download log function _decleration
void download_log(void);

//Clear log function declaration
void clear_log(void);

#endif // !BB_H

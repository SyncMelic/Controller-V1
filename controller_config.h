/*
  Controller_config.h - define hardware pin settings and globals.

  Copyright (c) 2020 Chen Sun.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef __CONTROLLER_CONFIG_H__
#define __CONTROLLER_CONFIG_H__
 
//#include <Arduino.h>
//#include <esp_now.h>
//#include <WiFi.h>
 
/**************************************
 * Define the pinout based on hardware
 *************************************/
#define PDL       (36)
#define PDR       (39)
#define JSLV      (34)
#define JSLH      (35)
#define JSRV      (32)
#define JSRH      (33)
#define A         (25)
#define B         (26)
#define C         (27)
#define D         (14)
#define VMR       (12)
#define RT1       (13)
#define LT1       (23)
#define SCL       (22)
#define RT2       (1)
#define LT2       (3)
#define SDA       (21)
#define FWD       (19)
#define RT        (18)
#define BWD       (5)
#define LT        (17)
#define JSRS      (16)
#define VML       (4)
#define START     (2)
#define JSLS      (15)
 
/**************************************
 * Define all the globals
 *************************************/
typedef struct struct_data{
    /* Two paddle stick */
    int   PDL_value;
    int   PDR_value;
 
    /* Two joystick with two axis and one button each */
    int   JSRV_value;
    int   JSRH_value;
    bool  JSRS_stat;
 
    int   JSLV_value;
    int   JSLH_value;
    bool  JSLS_stat;
 
    /* Buttons on the back of the controller */
    bool  LT1_stat;
    bool  LT2_stat;
    bool  RT1_stat;
    bool  RT2_stat;
 
    /* Buttons on the right side */
    bool  A_stat;
    bool  B_stat;
    bool  C_stat;
    bool  D_stat;
 
    /* Buttons on the left side */
    bool  FW_stat;
    bool  BW_stat;
    bool  RT_stat;
    bool  LT_stat;
 
    /* Start button */
    bool start_stat;
 
    /* Battery Status*/
    int batt_stat;
} struct_data;
 
struct_data data_out;
String send_stat;
 
/* Vibration Motor */
int freq = 5000;
int resolution = 8; // 0-255
int PWM_channel_0 = 0;
int PWM_channel_1 = 1;
int VML_value = 0;  // 0-255
int VMR_value = 0;  // 0-255
int VML_value_buff = 0;
int VMR_value_buff = 0;
int VM_cnt = 0;
 
 
 
/**************************************
 * Declare all the functions
 *************************************/
void extern controller_init();      // Init esp32 pin settings
void extern controller_update();    // Update all input/ouput pins
void extern display_update();       // Update display
void extern OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
 
#endif

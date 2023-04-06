/*
  Controller.ino - Main file to test ESP32 Controller Hardware

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

#include <esp_now.h>
#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "controller_config.h"
 
/* Define all golbal variables */
#define serial_debug false            // Set ture will enable serial debug functio but disable LT2/RT2 button function
 
#define SCREEN_WIDTH 128              // OLED display width, in pixels
#define SCREEN_HEIGHT 64              // OLED display height, in pixels
#define OLED_RESET    -1              // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
uint8_t broadcast_addr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
 
/**********************
 * Init all settings
 *********************/
void setup() {
  /* Init all the hardware pins within esp32 */
  controller_init();
 
  /* LCD screen init */
  /* I2C address are either 3D or 3C */
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
 
  /* Set up Wifi station mode */
  WiFi.mode(WIFI_STA);
 
  /* init esp now */
  esp_now_init();
  esp_now_register_send_cb(OnDataSent);
 
  /* Add recv device */
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcast_addr, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

 /* Vibration Test */
  ledcWrite(PWM_channel_0, 1024);
  delay(1000);
  ledcWrite(PWM_channel_0, 0);
  delay(1000);
  ledcWrite(PWM_channel_1, 1024);
  delay(1000);
  ledcWrite(PWM_channel_1, 0);
}
 
/**********************
 * Main start here
 **********************/
 
void loop() {
  /* less than 50ms may need special debounce logic for buttons */
  delay(100);
 
  /* This function update all the esp32 input and output based on globals */
  controller_update();
 
  /* Show info on display */
  display_update();
 
  /* Transmit data */
  esp_err_t result = esp_now_send(broadcast_addr, (uint8_t*) &data_out, sizeof(data_out));
  send_stat = result == ESP_OK ? " Connection OK":" Connection Failed!";
 
}
 

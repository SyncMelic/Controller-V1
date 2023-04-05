/*
  Controller_config.ino - Provide basic function to main

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

#include "controller_config.h"

void controller_init()
{
    
    /* IO1 and IO3 are needed for serial communication during debug */
    if(serial_debug)
    {
        Serial.begin(115200);
    }
    else
    {
        pinMode(LT2,INPUT);
        pinMode(RT2, INPUT);
    }
    
    /* Set pins need internal pull-up */
    pinMode(FWD,INPUT_PULLUP);
    pinMode(LT,INPUT_PULLUP);
    pinMode(RT,INPUT_PULLUP);
    pinMode(BWD,INPUT_PULLUP);
 
    pinMode(A,INPUT_PULLUP);
    pinMode(B,INPUT_PULLUP);
    pinMode(C,INPUT_PULLUP);
    pinMode(D,INPUT_PULLUP);
 
    pinMode(JSLS,INPUT_PULLUP);
    pinMode(JSRS,INPUT_PULLUP);
    pinMode(LT1,INPUT_PULLUP);
    pinMode(RT1,INPUT_PULLUP);
 
    pinMode(START, INPUT);  // Pulled down on ESP32 board
    
    /** AnalogWrite() in Arduino is not supported by ESP32 */
 
    /* ADC pins do NOT require a init routine */
    // pinMode(VML, OUTPUT);
    // pinMode(VMR, OUTPUT);
 
    ledcSetup(PWM_channel_0, freq, resolution);
    ledcAttachPin(VML, PWM_channel_0);
 
    ledcSetup(PWM_channel_1, freq, resolution);
    ledcAttachPin(VMR, PWM_channel_1);
 
    /* I2C pins are init in LCD routine */
}
 
void controller_update()
{
    /* update ADC pins stat */
    data_out.JSLV_value = analogRead(JSLV);
    data_out.JSLH_value = analogRead(JSLH);
    data_out.JSLS_stat  = digitalRead(JSLS);
 
    data_out.JSRV_value = analogRead(JSRV);
    data_out.JSRH_value = analogRead(JSRH);
    data_out.JSRS_stat  = digitalRead(JSRS);
 
    data_out.PDL_value = analogRead(PDL);
    data_out.PDR_value = analogRead(PDR);
 
    /* update buttons stat */
    data_out.A_stat = digitalRead(A);
    data_out.B_stat = digitalRead(B);
    data_out.C_stat = digitalRead(C);
    data_out.D_stat = digitalRead(D);
 
    data_out.FW_stat = digitalRead(FWD);
    data_out.BW_stat = digitalRead(BWD);
    data_out.LT_stat = digitalRead(LT);
    data_out.RT_stat = digitalRead(RT);
 
    data_out.LT1_stat = digitalRead(LT1);
    data_out.RT1_stat = digitalRead(RT1);
 
    data_out.start_stat = digitalRead(START);
    
    if (serial_debug)
    {
        data_out.LT2_stat = 0;
        data_out.RT2_stat = 0; 
    }
    else
    {
        data_out.LT2_stat = digitalRead(LT2);
        data_out.RT2_stat = digitalRead(RT2);
    }
 
    /* Update vibration motor output */
    ledcWrite(PWM_channel_0, VML_value);
    ledcWrite(PWM_channel_1, VMR_value);
 
    /* Update Battery Status */
    data_out.batt_stat = data_out.JSRH_value/6 + data_out.JSRV_value/6 + data_out.JSLH_value/6
                        + data_out.JSLV_value/6 + data_out.PDR_value/6 + data_out.PDL_value/6;
}
 
void display_update()
{
    display.clearDisplay();
    display.setRotation(2);      // Enable to invert display if needed 
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.cp437(true);         // Use full 256 char 'Code Page 437' font
        
    display.println("   System State: " + String(data_out.start_stat) );
    display.print(String(" FRBL:"+ String(data_out.FW_stat) + String(data_out.RT_stat) 
                        + String(data_out.BW_stat) + String(data_out.LT_stat)));
    display.println(String(" ABCD:"+ String(data_out.A_stat) + String(data_out.B_stat) 
                        + String(data_out.C_stat) + String(data_out.D_stat)));
 
    display.print(String(" PDL :" + String(data_out.PDL_value)));
    display.setCursor(60,16);
    display.println(String(" PDR :" + String(data_out.PDR_value)));
 
    display.print(String(" JSLV:" + String(data_out.JSLV_value)));
    display.setCursor(60,24);
    display.println(String(" JSRV:" + String(data_out.JSRV_value)));
 
    display.print(String(" JSLH:" + String(data_out.JSLH_value)));
    display.setCursor(60,32);
    display.println(String(" JSRH:" + String(data_out.JSRH_value)));
 
    display.print(String(" L012:"+ String(data_out.JSLS_stat)+ String(data_out.LT1_stat) + String(data_out.LT2_stat)));
    display.println(String("  R012:"+ String(data_out.JSRS_stat) + String(data_out.RT1_stat)+ String(data_out.RT2_stat)));
 
    display.print(String(" VML:"+ String(VML_value) + " VMR:"+ String(VMR_value)));
    display.println(String(" PW:"+ String(data_out.batt_stat)));
 
    display.println(send_stat);
 
    display.display();
}

/* send data to ESP32 Driver Board */ 
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

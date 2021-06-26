/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "platform/mbed_thread.h"
#include "XNucleoIKS01A2.h"


// Create a DigitalOut objects for the LED
DigitalOut led(PA_5);

// Create a Serial objects to communicate via USB
Serial pc(USBTX, USBRX);

// Create 2 Ticker objects for recurring interrupts. One for blink a LED and the other one to update the sensor lectures periodicly
Ticker blinky;
Ticker measurementy;

// Initialize variables

float Temp_Celsius;
float Temp_Fahrenheit;
float Temp_Kelvin;
float Humidity;
float Pressure;
bool flag=true;

// Handler for the aliveness LED; to be called every 0.5s
void blink(){
 led = !led;
}
// Handler for the measurements update; rise a flag every 3 seconds
void measurement()
{
 flag=true;   
}
 

int main() {
/* Create sensor shield object */
static XNucleoIKS01A2 *mems_expansion_board = XNucleoIKS01A2::instance(D14, D15, D4, D5);
static HTS221Sensor *hum_temp = mems_expansion_board->ht_sensor;
static LPS22HBSensor *press_temp = mems_expansion_board->pt_sensor;

/* Attach a function to be called by the Ticker objects at a specific interval in seconds */
blinky.attach(&blink, 0.5);
measurementy.attach(&measurement,.5);
   
/* Enable all sensors */
hum_temp->enable();
press_temp->enable();
  
  
  printf("\r\n--- Starting new run ---\r\n");
 
  
  
 
  while(1) {
    if(flag==true)
    {
 
        hum_temp->get_temperature(&Temp_Celsius);
        hum_temp->get_humidity(&Humidity);
        press_temp->get_pressure(&Pressure);
        /* Calculate temperature in Fahrenheit and Kelvins */
        Temp_Fahrenheit=(Temp_Celsius*1.8)+32;
        Temp_Kelvin=Temp_Celsius+273.15;
        printf("temperature in Celsius   = %.2f C\r\n", Temp_Celsius);
        printf("temperature in Fahrenheit   = %.2f F\r\n", Temp_Fahrenheit);
        printf("temperature in Kelvin   = %.2f K\r\n", Temp_Kelvin);
        printf("pressure = %.2f mbar\r\n", Pressure);
        printf("humidity = %.2f %% \r\n", Humidity);
 
        printf("---\r\n");
 
        /* Turn down the measurements update flag */
        flag=false;
    
    }
  }
}
 

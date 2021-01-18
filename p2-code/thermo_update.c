#include <string.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "thermo.h"

int set_temp_from_ports(temp_t *temp){
    int m = 0x01;
    if(THERMO_SENSOR_PORT > 64000 || THERMO_SENSOR_PORT < 0){ //error check
        return 1;
    }
    int actual_degrees;
    int remainder = THERMO_SENSOR_PORT % 64;//get the remainder
    short final_temp;
    if(remainder >= 32){ //check if we should round up or down, this case round up
        int roundoff = 64-remainder;
        final_temp = (THERMO_SENSOR_PORT + roundoff)/64;
    }
    else{
        int roundoff = 64-remainder;
        if(THERMO_SENSOR_PORT < 32000){ //round down
            final_temp = (THERMO_SENSOR_PORT - remainder)/64;
        }
        else if(THERMO_SENSOR_PORT == 64000){//stays the same, just convert
            final_temp = (THERMO_SENSOR_PORT + roundoff -1)/64;
        }
        else if(THERMO_SENSOR_PORT > 32000){ //round up
            final_temp = (THERMO_SENSOR_PORT + roundoff)/64 - 1;
        }
        else{
            final_temp = THERMO_SENSOR_PORT/64;
        }

    }
    actual_degrees = final_temp - 500;
    if(THERMO_STATUS_PORT & m){ //check if it should be converted to farenheit
        int farenheit = ((actual_degrees * 9)/5) +320;
        temp->tenths_degrees = farenheit;
        temp->is_fahrenheit = 1;
    }
    else{
        temp->tenths_degrees = actual_degrees;
        temp->is_fahrenheit = 0;
    }

    return 0;


}
// Uses the two global variables (ports) THERMO_SENSOR_PORT and
// THERMO_STATUS_PORT to set the temp structure. If THERMO_SENSOR_PORT
// is above its maximum trusted value, associated with +50.0 deg C,
// does not alter temp and returns 1.  Otherwise, sets fields of temp
// based on converting sensor value to degrees and checking whether
// Celsius or Fahrenheit display is in effect. Returns 0 on successful
// set. This function DOES NOT modify any global variables but may
// access global variables.
//
// CONSTRAINT: Uses only integer operations. No floating point
// operations are used as the target machine does not have a FPU.

int set_display_from_temp(temp_t temp, int *display){
    if(temp.is_fahrenheit != 0 && temp.is_fahrenheit != 1){ //error checks
        return 1;
    }
    if(temp.is_fahrenheit == 0){
    if(temp.tenths_degrees > 500){
        return 1;
    } if(temp.tenths_degrees < -500){
        return 1;
    }
    }
    else if(temp.is_fahrenheit == 1){
    if(temp.tenths_degrees > 1220){
        return 1;
    }
    if(temp.tenths_degrees < -580){
        return 1; 
    }
    }

    int mask0 = 0b1111110; //all masks for the array
    int mask1 = 0b0001100;
    int mask2 = 0b0110111;
    int mask3 = 0b0011111;
    int mask4 = 0b1001101;
    int mask5 = 0b1011011;
    int mask6 = 0b1111011;
    int mask7 = 0b0001110;
    int mask8 = 0b1111111;
    int mask9 = 0b1011111;
    int masknegative;
    int mask_celcius = 0b1;
    int mask_farenheit = 0b1;
    *display = 0;
    
    if(temp.tenths_degrees < 0){ //adds negative mask if number is negative
        masknegative = 0b0000001;
        temp.tenths_degrees = temp.tenths_degrees * -1;
    }
    else{
        masknegative = 0b0000000;
    }
    int temp_tenths = temp.tenths_degrees % 10; //get individual digits of the temp
    int temp_ones = (temp.tenths_degrees/10) % 10;
    int temp_tens = (temp.tenths_degrees/100) % 10;
    int temp_hundreds = (temp.tenths_degrees/1000) % 10;
    int digit_bitmasks[10];
    digit_bitmasks[0] = mask0; //put the masks into the array
    digit_bitmasks[1] = mask1;
    digit_bitmasks[2] = mask2;
    digit_bitmasks[3] = mask3;
    digit_bitmasks[4] = mask4;
    digit_bitmasks[5] = mask5;
    digit_bitmasks[6] = mask6;
    digit_bitmasks[7] = mask7;
    digit_bitmasks[8] = mask8;
    digit_bitmasks[9] = mask9;

    *display |=  digit_bitmasks[temp_tenths];//add last digit to the display
    if(temp_hundreds != 0){//if there is a digit in the hundreds spot
    *display |= masknegative << (7*4);
    *display |= digit_bitmasks[temp_hundreds] << (7 * 3);
    *display |= digit_bitmasks[temp_tens] << (7 * 2);
    *display |= digit_bitmasks[temp_ones] << (7 * 1);
    }
    else if (temp_hundreds == 0 && temp_tens == 0){//if there is no hundreds digit or tens digit
        *display |= masknegative << (7 * 2);
        *display |= digit_bitmasks[temp_ones] << (7 * 1);
    }
    else if (temp_hundreds == 0){ //only if there is no hundreds digit, but there is a tens digit
    *display |= masknegative << (7 *3);
    *display |= digit_bitmasks[temp_tens] << (7 * 2);
    *display |= digit_bitmasks[temp_ones] << (7 * 1);
    }
    if(temp.is_fahrenheit == 0){//check to see if celcius mask or farenheit mask needs to be set
        *display |= mask_celcius << 28;
    }
    else{
        *display |= mask_farenheit << 29;
    }

    

    
    return 0;

    
    }

    






// Alters the bits of integer pointed to by display to reflect the
// temperature in struct arg temp.  If temp has a temperature value
// that is below minimum or above maximum temperature allowable or if
// an improper indication of celsius/fahrenheit is given, does nothing
// and returns 1. Otherwise, calculates each digit of the temperature Qq
// and changes bits at display to show the temperature according to
// the pattern for each digit.  This function DOES NOT modify any
// global variables but may access global variables.
int thermo_update(){
    temp_t temp;
    int display = -1;
    if(set_temp_from_ports(&temp) == 1){//error check if function doesn't work
        return 1; 
    }
    int display_return = set_display_from_temp(temp, &display);
    if(display_return == 1){//error check if function doesn't work
        return 1;
    }
    THERMO_DISPLAY_PORT = display; //updates the display
    return 0;


}
// Called to update the thermometer display.  Makes use of
// set_temp_from_ports() and set_display_from_temp() to access
// temperature sensor then set the display. Checks these functions and
// if they indicate an error, makes not changes to the display.
// Otherwise modifies THERMO_DISPLAY_PORT to set the display.
// 
// CONSTRAINT: Does not allocate any heap memory as malloc() is NOT
// available on the target microcontroller.  Uses stack and global
// memory only.
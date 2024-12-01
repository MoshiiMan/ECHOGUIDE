 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.2
 *
 * @version Package Version: 3.1.2
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "usart.h"

//----------------USER DEFINED VARIABLES----------------//
#define TRIG_PIN PIN6_bm
#define ECHO_PIN PIN2_bm
#define SOUND_SPEED 0.034 // Speed of sound in cm/us
//------------------------------------------------------//
volatile float HIGH_TIME = 0;
volatile float distance = 0;
volatile bool measurement_done = false;

void TRIGER_SEQUENS(){
    PORTD.OUTCLR = TRIG_PIN;
    _delay_us(2); // Ensure a clean low state
    PORTD.OUTSET = TRIG_PIN;
    _delay_us(10); // Pulse high for 10us
    PORTD.OUTCLR = TRIG_PIN;
}

void CAPTURE_CALLBACK(){
    //HIGH_TIME = TCB0_CounterGet();
    HIGH_TIME = TCB0_PeriodGet();    
    //HIGH_TIME = TCB0.CCMP;
    measurement_done = true;   
}

void CALCULATE_DISTANCE(){
    if(measurement_done){
        measurement_done = false; // Clear the flag
        // Convert clock cycles to microseconds
        float time_us = HIGH_TIME * (1.0 / F_CPU) * 1e6;
        // Calculate distance (divide by 2 for round trip)
        distance = (time_us * SOUND_SPEED) / 2.0;
        printf("Time: %.f us\n", time_us);
        printf("Distance: %.2f cm\n", distance);
    }
}

void SETUP(){
    PORTD.DIRSET = TRIG_PIN;
    PORTA.DIRCLR = ECHO_PIN;
    
    SYSTEM_Initialize();
    TCB0_Initialize();                                // Call generated initialization function
    TCB0_CaptureCallbackRegister(CAPTURE_CALLBACK);   // Register capture callback
    TCB0_CAPTInterruptEnable();                       // Enable capture interrupt
    EVSYS_Initialize();
    
    //EVSYS.CHANNEL0 = EVSYS_CHANNEL0_PORTA_PIN2_gc; // Route PA2 to Event Channel 0
    
    //TCB0.CTRLB = TCB_CNTMODE_PW_gc;               // Pulse Width mode
    //TCB0.EVCTRL = TCB_CAPTEI_bm | TCB_EDGE_bm;    // Enable capture input and edge detection
    
    TCB0.CCMP = 0;                                // Clear CCMP register
    TCB0.CNT = 0;                                 // Reset counter
    TCB0_Start();
    
    USART3_init(9600);
}


int main(void){
    //----------------SETUP--------------//
    SETUP();    // Initialize system and UART
    sei();      // Enable global interrupts
    while(1){
        //----------------LOOP--------------//
        TRIGER_SEQUENS();   // Send trigger pulse
        _delay_ms(60);      // Wait for echo to complete
        CAPTURE_CALLBACK();
        CALCULATE_DISTANCE(); // Process the measured pulse width
        if (TCB0.INTFLAGS & TCB_CAPT_bm) {
            printf("TCB0 captured pulse width!\n");
            TCB0.INTFLAGS = TCB_CAPT_bm; // Clear Capture Interrupt flag
        }
    }    
}

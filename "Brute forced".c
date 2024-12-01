/*
 * File:   main.c
 * Author: gabri
 *
 * Created on October 25, 2024, 11:37 AM
 */

#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "usart.h"
#include "peripherals.h"

//-------------------Userdefined parameters-------------------
#define SOUND_SPEED 0.0343 //The speed of sound in cm/us
#define TRIG_PIN PIN0_bm   // Define trigger pin
#define ECHO_PIN PIN1_bm   // Define echo pin
//-------------------------------------------------------------

// Initialize ultrasonic sensor pins
void ultrasonic_init() {
    // Set TRIG_PIN as output
    PORTB.DIRSET = TRIG_PIN;
    // Set ECHO_PIN as input
    PORTB.DIRCLR = ECHO_PIN;
}

// Send 10us pulse to HC-SR04 TRIG_PIN
void ultrasonic_trigger() {
    PORTB.OUTCLR = TRIG_PIN; // Clear TRIG_PIN
    _delay_us(2);
    PORTB.OUTSET = TRIG_PIN;  // Set TRIG_PIN high
    _delay_us(10);             // 10us high pulse
    PORTB.OUTCLR = TRIG_PIN; // Set TRIG_PIN low
}

// Measure pulse width in microseconds on ECHO_PIN
uint32_t measure_pulse_width() {
    uint32_t pulse_width = 0;

    // Wait for ECHO_PIN to go high
    while (!(PORTB.IN & ECHO_PIN)){
    }

    // Measure duration of high pulse on ECHO_PIN
    while (PORTB.IN & ECHO_PIN) {
        _delay_us(1);          // Wait 1µs
        pulse_width++;         // Increment counter
    }
    return pulse_width;
}

// Calculate distance in cm based on pulse width
uint16_t calculate_distance(uint32_t pulse_width) {
    // Speed of sound is ~343 m/s or 0.0343 cm/µs, divide by 2 for round trip
    uint16_t avstand = (pulse_width * SOUND_SPEED / 2);
    pulse_width = 0;
    return avstand;
}

int main(void) {
    uint16_t pulse_width, distance;
    
    ultrasonic_init();  // Initialize sensor
    
    USART3_init(9600);
    //sei();

    while (1) {
        ultrasonic_trigger();           // Trigger the ultrasonic sensor
        pulse_width = measure_pulse_width(); // Measure pulse width
        distance = calculate_distance(pulse_width); // Calculate distance in cm
        printf("Avstanden er: %f \n",distance);
        
        // Here, you could add code to display or use the distance value
        _delay_ms(100); // Wait before the next reading
    }
}

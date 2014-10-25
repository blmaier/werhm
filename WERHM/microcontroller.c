/*
 * microcontroller.c
 * Driver for the MSP-EXP430G2
 *
 *  Created on: Oct 20, 2014
 *      Author: Brandon
 */


#include <msp430.h>
#include "microcontroller.h"

#define INTERRUPT_PRAGMA

void disableWatchdog() {
	/**
	 * Stop watchdog timer
	 */
	WDTCTL = WDTPW | WDTHOLD;
}

void sleepMode() {
	/**
	 * Set status register to sleep with interrupt
	 *  LMP4_bits = Low power mode 4 (disable everything)
	 *  GIE = Global interrupts enabled
	 */
	_bis_SR_register(LPM4_bits + GIE);
}

void setOutputPins(int pins) {
	P1DIR |= pins;
}

void setInputPins(int pins) {
	P1DIR &= ~pins;
}

void setInterruptPins(int pins) {
	P1IE |= pins;

}

void clearInterruptPins(int pins) {
	P1IFG &= ~pins;
}

void setFrequency() {

	/* <0.15Mhz */
	BCSCTL1 &= ~(BIT3 + BIT2 + BIT1 + BIT0);
	//BCSCTL1 |= BIT1;

	DCOCTL &= ~(BIT7 + BIT6 + BIT5);
	//DCOCTL |= BIT5 + BIT6;
}

void flashLED() {
	P1OUT ^= 0x01;

	int j = 10;
	for(;j != 0; j--) {
		volatile unsigned int i;	// volatile to prevent optimization
		//volatile unsigned int j;

		P1OUT ^= 0x41;				// Toggle P1.0 using exclusive-OR


		i = 5000;					// SW Delay
		do i--;
		while(i != 0);
	}

	P1OUT = 0x00;
}
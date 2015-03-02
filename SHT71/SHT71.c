/*
 * SHT71.c
 *
 *  Created on: Oct 14, 2014
 *      Author: mdbalke
 */


#include <msp430.h>
#include "SHT71.h"

float read_humidity(){

	float linear_value;
	float raw_humidity;

	/*
	 * Conversion constants as defined in the datasheet
	 * 	for linearlizing sensor value
	 *
	 * 	H(linear) = C1 + C2*H(raw) + C3*H(raw)^2
	 */
	const float C1 = -2.0468;
	const float C2 = 0.0367;
	const float C3 = .0000015955;

	raw_humidity = read_humidity_raw();

	linear_value = C1 + C2 * raw_humidity + C3 * raw_humidity * raw_humidity;

	return linear_value;
}

float read_humidity_raw(){

	int out;

	const int command = 0b00000101;

	send_SHT_command(command);

	DATA_IN;

	while(DATA_READ){
		_delay_cycles(10);
	}

	out = read_two_bytes_SHT();

	skipCrcSHT();

	return out;
}

float read_temperature(){

	int raw_value;
	float temperature;

	const float D1 = -40.0;
	const float D2 = 0.018;

	raw_value = read_temperature_raw();

	temperature = (raw_value * D2) + D1;

	return temperature;
}

int read_temperature_raw(){

	int out;

	const int command = 0b00000011;

	send_SHT_command(command);

	DATA_IN;

	while(DATA_READ){
		_delay_cycles(10);
	}

	out = read_two_bytes_SHT();

	skipCrcSHT();

	return out;
}

void send_SHT_command(int command){

	int ack;

	DATA_OUT;
	SCL_OUT;

	//Send start sequence
	DATA_HIGH;
	SCL_HIGH;
	DATA_LOW;
	SCL_LOW;
	SCL_HIGH;
	DATA_HIGH;
	SCL_LOW;

	//Send command byte
	send_byte_SHT(command);

	//Look for ack
	SCL_HIGH;
	DATA_IN;
	ack = DATA_READ;

	if(ack != 0){
		//no ack
	}

	SCL_LOW;
	ack = DATA_READ;
	if(ack == 0){
		//ack error
	}
}

void send_byte_SHT(int command){
	int i;
	for(i = 7; i >= 0; i--){
		if(command & (1 << i)){
			DATA_HIGH;
		}else{
			DATA_LOW;
		}
		SCL_HIGH;
		SCL_LOW;
	}

}

int read_two_bytes_SHT(){

	int out;

	DATA_IN;

	out = read_byte_SHT();
	out = out << 8;

	//Send ack to sensor for first byte
	DATA_OUT;
	DATA_HIGH;
	DATA_LOW;
	SCL_HIGH;
	SCL_LOW;

	DATA_IN;
	out |= read_byte_SHT();

	return out;

}

unsigned char read_byte_SHT(){

	unsigned char out = 0;
	int i;
	for(i = 7; i >= 0; i--){
		SCL_HIGH;
		if(DATA_READ){
			out |= (1 << i);
		}
		SCL_LOW;
	}
	return out;
}

void skipCrcSHT(){

	DATA_OUT;
	SCL_OUT;

	DATA_HIGH;
	SCL_HIGH;
	SCL_LOW;
}

void blink_red(){
	int i;
	for(i = 5; i >= 0; i--){
		P1OUT ^= BIT0;
		_delay_cycles(40000);
	}
	P1OUT &= ~BIT0;
}
void blink_green(){
	int i;
	for(i = 5; i >= 0; i--){
		P1OUT ^= BIT6;
		_delay_cycles(40000);
	}
	P1OUT &= ~BIT6;
}

void SHT_data_in(){
	P1DIR &= ~SDA_PIN;
	P1REN |= SDA_PIN;
}



/*
float read_temperature(){

	float raw_value;
	float temperature;

	//Constants defined in datasheet for raw data conversion at 3.5V supply
	const float SHT_D1 = -39.7;
	const float SHT_D2 = 0.018;

	raw_value = read_temperature_raw();

	temperature = (raw_value * SHT_D2) + SHT_D1;

	return temperature;
}

int read_temperature_raw(){

	int out;

	const int command = 0b00000011;

	send_SHT_command(command);

	DATA_IN;

	unsigned int i = 0;

	while(DATA_READ){

		//Eventually replace this code with a timeout
		_delay_cycles(100);
		i++;
		if(i > 40000){
			return 0;
		}
	}

	out = read_two_bytes_SHT();

	skip_crc_SHT();

	return out;
}

void send_SHT_command(int command){

	int ack;

	DATA_OUT;
	SCL_OUT;

	//Send start sequence
	DATA_HIGH;
	SCL_HIGH;
	DATA_LOW;
	SCL_LOW;
	SCL_HIGH;
	DATA_HIGH;
	SCL_LOW;

	//Send command byte
	send_byte_SHT(command);

	//Look for ack
	SCL_HIGH;
	DATA_IN;
	ack = DATA_READ;

	if(ack != 0){
		//no ack
	}

	SCL_LOW;
	ack = DATA_READ;

	if(ack == 0){
		//ack error
	}
}

void send_byte_SHT(int command){
	int i;
	for(i = 7; i >= 0; i--){
		if(command & (1 << i)){
			DATA_HIGH;
		}else{
			DATA_LOW;
		}
		SCL_HIGH;
		SCL_LOW;
	}
}

int read_two_bytes_SHT(){

	int out;

	DATA_IN;

	out = read_byte_SHT();
	out = out << 8;

	//Send ack to sensor for first byte
	DATA_OUT;
	DATA_HIGH;
	DATA_LOW;
	SCL_HIGH;
	SCL_LOW;

	DATA_IN;
	out |= read_byte_SHT();

	return out;
}

unsigned char read_byte_SHT(){

	unsigned char out = 0;
	int i;
	for(i = 7; i >= 0; i--){
		SCL_HIGH;
		if(DATA_READ){
			out |= (1 << i);
		}
		SCL_LOW;
	}
	return out;
}

void skip_crc_SHT(){

	DATA_OUT;
	SCL_OUT;

	DATA_HIGH;
	SCL_HIGH;
	SCL_LOW;
}

//Set Data pin as input and enable internal pullup resistor
void SHT_data_in(){
	P2DIR &= ~BIT0;
	P2REN |= BIT0;
}

*/

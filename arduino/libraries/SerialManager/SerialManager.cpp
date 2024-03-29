/*
  SerialManager.cpp - Library for doing packetized serial comm with Arduinos.
  Created by Sigurdur Orn, May 23, 2010.
  siggi@mit.edu
 */

#include "WProgram.h"
#include "SerialManager.h"
#include "ByteBuffer.h"


SerialManager::SerialManager(unsigned int in_buf_size, unsigned int out_buf_size){
	serial_in_checksum = 0;

	incoming_buffer = (ByteBuffer*)malloc(sizeof(ByteBuffer));
	outgoing_buffer = (ByteBuffer*)malloc(sizeof(ByteBuffer));
	temp_buffer = (ByteBuffer*)malloc(sizeof(ByteBuffer));

	incoming_buffer->init(in_buf_size);
	outgoing_buffer->init(out_buf_size);
	temp_buffer->init(10);

	byte1 = 0;
	byte2 = 0;
	byte3 = 0;
	byte4 = 0;

	handlePacketFunction = 0;
}

void SerialManager::init(int serial_port, int baud_rate){
	_serial_port = serial_port;

	if( serial_port == 0)
		Serial.begin(baud_rate);
#if defined(__AVR_ATmega1280__)
	else if( serial_port == 1)
		Serial1.begin(baud_rate);
	else if( serial_port == 2)
		Serial2.begin(baud_rate);
	else if( serial_port == 3)
		Serial3.begin(baud_rate);
#endif
}



bool SerialManager::isBusySending(){
	return ( outgoing_buffer->getSize() > 0 );
}

// Sends a single byte
int SerialManager::sendSerialByte(byte b){
	return outgoing_buffer->put(b);
}

// Sends a packet with a header and a checksum
int SerialManager::sendSerialPacket(ByteBuffer* packet_buffer){
	// Copy buffer and calc checksum
	byte checksum = 0;
	while( packet_buffer->getSize() > 0 ){
		byte b = packet_buffer->get();
		outgoing_buffer->put(b);
		checksum += b;
	}

	outgoing_buffer->put( checksum );
	outgoing_buffer->put( 1 );
	outgoing_buffer->put( 2 );
	outgoing_buffer->put( 3 );
	outgoing_buffer->put( 4 );

	return 0;
}

// Sends a raw packet with no header or checksum
int SerialManager::sendRawSerial(ByteBuffer* packet_buffer){
	while( packet_buffer->getSize() > 0 ){
		byte b = packet_buffer->get();
		outgoing_buffer->put(b);
	}

	return 0;
}

void SerialManager::update(){

	// If we have received stuff
	if( _serial_port == 0 ){
		while( Serial.available() != 0 ) {
			byte incoming = Serial.read();
			handleIncomingByte(incoming);
		}
	}

#if defined(__AVR_ATmega1280__)
	else if( _serial_port == 1 ){
		while( Serial1.available() != 0 ) {
			byte incoming = Serial1.read();
			handleIncomingByte(incoming);
		}
	}
	else if( _serial_port == 2 ){
		while( Serial2.available() != 0 ) {
			byte incoming = Serial2.read();
			handleIncomingByte(incoming);
		}
	}
	else if( _serial_port == 3 ){
		while( Serial3.available() != 0 ) {
			byte incoming = Serial3.read();
			handleIncomingByte(incoming);
		}
	}
#endif


	// If we should be sending stuff
	if( outgoing_buffer->getSize() > 0 ){
		// If serial port not currently busy
		if( _serial_port == 0 )
			if(((UCSRA) & (1 << UDRE)) ) Serial.write( outgoing_buffer->get() );

#if defined(__AVR_ATmega1280__)
		else if( _serial_port == 1 )
			if(((UCSR1A) & (1 << UDRE1)) ) Serial1.write( outgoing_buffer->get() );
		else if( _serial_port == 2 )
			if(((UCSR2A) & (1 << UDRE2)) ) Serial2.write( outgoing_buffer->get() );
		else if( _serial_port == 3 )
			if(((UCSR3A) & (1 << UDRE3)) ) Serial3.write( outgoing_buffer->get() );
#endif
	}
}

void SerialManager::setPacketHandler(void (*packetHandlerFunction)(ByteBuffer*)){
	handlePacketFunction = packetHandlerFunction;
}


void SerialManager::handleIncomingByte(byte incoming){

	// If buffer overflows then reset (we could do something smarter here)
	if( incoming_buffer->getSize() == incoming_buffer->getCapacity() ){
		incoming_buffer->clear();
	}

	incoming_buffer->put( incoming );
	serial_in_checksum += incoming;

	byte1 = byte2;
	byte2 = byte3;
	byte3 = byte4;
	byte4 = incoming;

	// If we have a full packet ready
	if( byte1==1 && byte2==2 && byte3==3 && byte4==4  ){
		// Remove header from buffer
		incoming_buffer->getFromBack();
		incoming_buffer->getFromBack();
		incoming_buffer->getFromBack();
		incoming_buffer->getFromBack();
		
		byte checksum_received = incoming_buffer->getFromBack();

		// Remove the whole header from the calculated checksum
		serial_in_checksum -= checksum_received + 10;

		// If checksums don't match, then do something ()
		if( checksum_received != serial_in_checksum ){
			;	// We could do something about this here
		}

		// We have a successful packet
		else{
		
			if( handlePacketFunction != 0 )
				handlePacketFunction(incoming_buffer);
			else
				handlePacketDefault(incoming_buffer);
		}

		// Clear variables
		incoming_buffer->clear();
		serial_in_checksum = 0;
	}

}

void SerialManager::handlePacketDefault(ByteBuffer* packet){
	// We could do something here like send the data to the host again for debug
	// Or just do nothing
	return;
}

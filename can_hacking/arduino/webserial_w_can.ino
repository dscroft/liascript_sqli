// Include the library
#include <SimpleWebSerial.h>

// === CAN =============
#include <SPI.h>

#define CAN_2515
// #define CAN_2518FD

#if defined(SEEED_WIO_TERMINAL) && defined(CAN_2518FD)
const int SPI_CS_PIN  = BCM8;
const int CAN_INT_PIN = BCM25;
#else
const int SPI_CS_PIN = 9;
const int CAN_INT_PIN = 2;
#endif

#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
#endif

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif
// =====================

// Create an instance of the library
SimpleWebSerial WebSerial;

bool ledState = false;

void setup() 
{
  // Initialize serial communication
  Serial.begin( 57600 );

  // CAN init
  while( CAN_OK != CAN.begin(CAN_500KBPS) ) 
  {
    Serial.println( "CAN init fail, retry..." );
    delay( 100 );
  }
  Serial.println( "CAN init ok!" );
  // end

  WebSerial.on( "can-send", canSendCallback );
}

/* 
["can-send",[203,[196,0,0,0,0,0,0,0]]]
*/
void canSendCallback( JSONVar msg )
{
  Serial.println( "[\"logger\",\"Called canSendCallback()\"]" );

  if( JSON.typeof(msg) != "array" ) return;
  if( msg.length() != 2 ) return; 
  if( JSON.typeof(msg[1]) != "array" ) return;

  uint8_t frameid = msg[0];
  uint8_t buffer[8];
  for( int i = 0; i < 8; i++ )
    buffer[i] = msg[1][i];

  Serial.println( "[\"logger\",\"Sending\"]" );

  CAN.sendMsgBuf( frameid, 0, msg[1].length(), buffer );
}


unsigned long aliveTimer = 0;

// bodge, but JSONVar isn't working on Uno boards.
void print_json( char* topic, int id, uint8_t* data, int len )
{
  Serial.print( "[\"" );
  Serial.print( topic );
  Serial.print( "\",[" );
  Serial.print( id );
  Serial.print( ",[" );
  for( int i = 0; i < len; i++ )
  {
    if( i != 0 ) Serial.print( "," );
    Serial.print( data[i], DEC );
  }
  Serial.println( "]]]" );
}

void loop() 
{
    const unsigned long now = millis();
    
    // send alive message
    if( now - aliveTimer > 3000 )
    {
        WebSerial.send( "logger", JSONVar(now) );
        aliveTimer = now;
    }

    if( CAN_MSGAVAIL == CAN.checkReceive() ) 
    {
        uint8_t canRecv[8];
        uint8_t canLen = 0;
        CAN.readMsgBuf( &canLen, canRecv );    // read data,  len: data length, buf: data buf

        uint8_t canId = CAN.getCanId();

        print_json( "can-recv", canId, canRecv, canLen );
    }

    // Check for new serial data every loop
    WebSerial.check();
    delay(5);  
}

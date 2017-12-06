/* **********************************************************************************
 * 13/12/2016
 * Marco Panato
 * 
 * Christmas Lights Program (relay with on/off time)
 * 
 * Behavior:
 *    - system starts at 17.00 with canRun = true.
 *    - system runs until 00.00, then canRun = false.
 *    - system is stopped until 17.00, then canRun = true.
 * ********************************************************************************** 
 */

// **********************************************************************************
// Delay library
// **********************************************************************************
#include <AsyncDelay.h>

// 7 hours
#define DELAYON 25200000 
// 24 hours
#define DELAYOFF 86400000

AsyncDelay delayOn;
AsyncDelay delayOff;

bool canRun;
// **********************************************************************************

// **********************************************************************************
// Pins
// **********************************************************************************
#define RELAYPIN A5
/// Define if the lights are connected to the NC port of the relay.
//#define RELAY_INVERTED
// **********************************************************************************

// **********************************************************************************
// Array of T_ON, T_OFF, T_ON, T_OFF, ... MUST BE ODD!
// **********************************************************************************
int delays[] = {
  1000,
  500,
  2000,
  1000,
  500,
  1000,
  1000,
  2000,
  4000,
  1000,
  5000,
  2000,
  3000,
  500,
  6000,
  3000,
  8000,
  1000
};
int delays_len = sizeof(delays)/sizeof(int);

#define ON 1
#define OFF 0
int state;

int pos;
// **********************************************************************************


// **********************************************************************************
//  Arduino initialization
// **********************************************************************************
void setup() {  
  // Relay pin
  pinMode(RELAYPIN, OUTPUT);

  canRun = true; // Now are 17.00

  delayOn.start(DELAYON, AsyncDelay::MILLIS);
  delayOff.start(DELAYOFF, AsyncDelay::MILLIS);

  // System starts OFF
  pos = 0;
  state = OFF;
  applyState();
  
  delay(100);
}


// **********************************************************************************
//  Arduino loop
// **********************************************************************************
void loop() {
  if (canRun) {
    
    if (delayOn.isExpired()) {
      canRun = false;

      state = OFF;
      pos = 0;
      applyState();
      
    } else {      
      
      switchState();
      delay(delays[pos]);
      pos = (pos+1)%delays_len;
      
    }
    
  } else {
    
    if (delayOff.isExpired()) {
      
      canRun = true;

      delayOff.repeat();
      delayOn.start(DELAYON, AsyncDelay::MILLIS);
         
    }
    
  }
}

void switchState() { 
  state = !state;
  applyState();
}

void applyState() {  
  #ifdef RELAY_INVERTED
    digitalWrite(RELAYPIN, state == ON ? HIGH : LOW);
  #else
    digitalWrite(RELAYPIN, state == ON ? LOW : HIGH);
  #endif
}


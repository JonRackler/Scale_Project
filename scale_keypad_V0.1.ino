#include <Keypad.h>
#include <Wire.h>

const byte ROWS = 5; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'7','8','9','G'}, // 7, 8, 9, trgt
  {'4','5','6','S'}, // 4, 5, 6, smpl
  {'1','2','3','P'}, // 1, 2, 3, UWT PST
  {'0','P','C','U'}, // 0, ./print, CE, Unit WT
  {'Z','T','M','+'}  // zero/unit, tare, MC, M+
};
byte rowPins[ROWS] = {6, 7, 8, 9, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4, 5}; //connect to the column pinouts of the keypad

const int intPin = 13; // pin to generate interrupt on main board

// data transfer pins to main board
// const int rxPin = 11;
// const int txPin = 12;

//bool intPinState = false; // keep track of the state to know if main board is read for data

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

//SoftwareSerial object(rxPin, txPin, inverse_logic)
// SoftwareSerial mySerial(rxPin, txPin);

void setup(){
//  pinMode(rxPin, INPUT);
//  pinMode(txPin, OUTPUT);
//  pinMode(intPin, OUTPUT);
  // mySerial.begin(9600);
  Wire.begin(5);
  
}
  
void loop(){
  char customKey = customKeypad.getKey();

  if ( customKey )
  {
    Wire.beginTransmission(4);
    Wire.write(customKey);
    Wire.endTransmission();

  // --thought-- write interrrupt high and wait for main board to write
  //  any information to the serial port. That data is thrown away. Its
  //  only purpose is the get the main board into its keypressed ISR to
  //  make sure it receives all the keypresses
//  if (customKey){
//    // digitalWrite(intPin, HIGH);
//    // if(mySerial.available() > 0)
//    // {
//       mySerial.read(); // throw away
//      mySerial.write(customKey);
//      digitalWrite(intPin, LOW);
//    }
    
    
  }
}

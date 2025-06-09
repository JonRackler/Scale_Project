#include "HX711.h"
#include <LiquidCrystal.h>
#include <Wire.h>

/* Prototypes */
void receiveEvent();
void readScale();
void displayWeight(float weight);
/**************/

/***** LCD ****/
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 6, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
/**************/

/* Load Cell Amp */
const int DOUT = 8;
const int CLK = 7;
HX711 scale;
/*****************/


/* Timing */
long currentMillis = 0;
const long lcdUpdateInterval = 500;
long prevLcdUpdate = 0;

const long scaleReadInterval = 500;
long prevScaleRead = 0;
/*****************/


/* Buzzer */
const int buzzer = 5;
const int frequency = 2000; // In hertz
const int duration = 100; // length of buzz in milliseconds
/****************/

float currentWeight = 0.0000f;
volatile float unitWeight = 0; // currentWeight/10 when sample button is pressed
const float UNIT_THRESH_HOLD_UPPER = 0.0200f;// if unit weight is under this weight increase scale data points to help with floating
const float UNIT_THRESH_HOLD_LOWER = 0.0100f;
const int UNDER_THRESH_HOLD_SAMPLE_COUNT_UPPER = 25;
const int UNDER_THRESH_HOLD_SAMPLE_COUNT_LOWER = 35;
volatile bool threshHoldMet = true;
float pcs = 0;

const float CONVERSION = 96990; //95000

//char units[2][4] = {"kgs", "lbs"};

/* button states */
volatile bool sampled = false;
volatile bool zerod = false;
/****************/


void setup()
{
  // create program objects
  lcd.begin(16, 2);
  scale.begin(DOUT, CLK);
  Wire.begin(4); // Address to talk to keypad driver ATMega328
  Wire.onReceive(receiveEvent);

  //wait for power to stablize before interacting with sensors
  //delay(1500); // Not sure if this is needed because of wait_ready(3500)

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Oh boy,");
  lcd.setCursor(6, 1);
  lcd.print("Fasteners");
  
  scale.wait_ready(2500);  //this is a void function
  scale.set_scale(CONVERSION);
  scale.tare(5);
}

void loop()
{
  currentMillis = millis();
  readScale();
  
  
  displayWeight(currentWeight);   
}

void displayWeight(float weight)
{
  if ( currentMillis - prevLcdUpdate >= lcdUpdateInterval) {
    prevLcdUpdate += lcdUpdateInterval;
    
    lcd.clear();
    lcd.setCursor(0 ,0);
    lcd.print("Weight in ");
    lcd.print("kgs");
    lcd.setCursor(0, 1);
    lcd.print(currentWeight);

    if ( sampled )
    {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pcs: ");
      lcd.print(pcs);
      lcd.setCursor(0, 1);
      lcd.print(currentWeight);
    }
  }
}
void readScale()
{
  // HX711::get_units() does not wait for sensor to be ready
  if ( currentMillis - prevScaleRead >= scaleReadInterval )
  {
    prevScaleRead += scaleReadInterval;
    float currentWeight = scale.get_units(10);
  }
  if (sampled)
   {
      float gross = currentWeight / unitWeight;

      //pcs = round(gross);
      pcs = ((int)gross)+1;
   }
}



bool is_steady()
{
    float average = 0;
    float nextValue = 0;
    float tolerance = 0;
    
    // wait 250ms to let user move from scale
    delay(250);
    
    // -thought- maybe take an average of the next 5 mesurements
    //           and then take one after that, if those two numbers
    //           are within 5% of each other then return true, 
    //           else wait for a stable measurement
    
    do
    {
        average = scale.read_average(5);
        tolerance = 0.05 * average;
        
        nextValue = scale.get_units();
    }
   while( (average - tolerance <= nextValue) || (nextValue <= average - tolerance) );
    
}

void receiveEvent(int bytes)
{
  while( Wire.available())
  {
    char c = Wire.read();
    switch ( c )
    {
      case 'Z':
        //tone(buzzer, frequency, duration);
        is_steady();
        scale.tare(5);
      break;

      case 'S':
        //tone(buzzer, frequency, duration);
        
        sampled = true; // volatile bool
      break;
      
      // setup clear function
      case 'C':
        sampled = false;
        threshHoldMet = true;
      break;
    }
  }
}

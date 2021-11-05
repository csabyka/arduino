#include "FastLED.h"
#include <DFPlayerMini_Fast.h>

//#if !defined(UBRR1H)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
//#endif

//distance sensor
#define trigPin 7 //zold for the FIRST distance module 
#define echoPin 6 //sarga
#define trigPin2 5// zold for the SECOND  distance module 
#define echoPin2 4
int firstTime=0;//we need to declare firstTime outside the loop 
long Distance,auxDistance,gap=0;
long Distance2,auxDistance2,gap2=0;

#define LED_TYPE WS2812
#define NUM_LEDS 7
#define DATA_PIN 8
#define COLOR_ORDER GRB
#define MASTER_BRIGHTNESS 200
CRGB leds[NUM_LEDS];
//uint8_t num = 0;  // stores a number when animating grad


DEFINE_GRADIENT_PALETTE( celticsun_gp ) {
    0, 120, 19,  8,
  255, 255, 97,  5};


DEFINE_GRADIENT_PALETTE( autumnrose_gp ) {
    0,  71,  3,  1,
   45, 128,  5,  2,
   84, 186, 11,  3,
  127, 215, 27,  8,
  153, 224, 69, 13,
  188, 229, 84,  6,
  226, 242,135, 17,
  255, 247,161, 79};


DEFINE_GRADIENT_PALETTE( halloween_gp ) {
    0, 173, 53,  1,
  127,   0,  0,  0,
  191, 173, 53,  1,
  255, 173, 53,  1};


CRGBPalette16 celtic = celticsun_gp;  
CRGBPalette16 autumn = autumnrose_gp;  
CRGBPalette16 halloween = halloween_gp;  


DFPlayerMini_Fast myMP3;

void setup()
{
  Serial.begin(115200);

//#if !defined(UBRR1H)
  mySerial.begin(9600);
  myMP3.begin(mySerial, true);
//#else
//  Serial1.begin(9600);
//  myMP3.begin(Serial1, true);
//#endif
  //Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  //delay(1000);
  
  //Serial.println("Setting volume to max");
  myMP3.volume(30);
  
  //Serial.println("Playing track 1");
  //myMP3.loop(1);
  //myMP3.play(1);

pinMode(trigPin, OUTPUT); 
pinMode(echoPin, INPUT); 
pinMode(trigPin2, OUTPUT); 
pinMode(echoPin2, INPUT); 
       
FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
FastLED.setBrightness(MASTER_BRIGHTNESS);

}
// Set unused pins to INPUT and LOW to save power
//void inputsLow ()
//{
//  pinMode(0, INPUT);
//  pinMode(1, INPUT);  
//  pinMode(3, INPUT); 
//  pinMode(4, INPUT); 
//  pinMode(5, INPUT); 
//  digitalWrite(0, LOW);
//  digitalWrite(1, LOW);  
//  digitalWrite(3, LOW); 
//  digitalWrite(4, LOW); 
//  digitalWrite(5, LOW); 
//}

void loop()
{
//inputsLow();  
//pixels.clear();

Distance=measureDistance(trigPin,echoPin);//measure distance1 and store
Distance2=measureDistance(trigPin2,echoPin2);//measure distance2 and store
gap=abs(Distance-auxDistance);// calculate the difference between now and last reading
gap2=abs(Distance2-auxDistance2);// calculate the difference between now and last reading

if(firstTime==0){//necesary for stability things
auxDistance=Distance;
auxDistance2=Distance2;
gap=0;
gap2=0;
//does it only the first time after play a song to avoid first loop malfuntcion
firstTime++; 
delay(2000);
}

if(gap>20 and gap2<20 ){ //if distance variation is 20cm
//myMP3.play(1);//play the first song of the second folder 
firstTime=0;//avoid errors!!we dont like errors
Serial.println("RIGHT MOVEMENT");
for (uint8_t i=0; i<NUM_LEDS; i++) {
  uint8_t paletteIndex = map(i,0,NUM_LEDS-1,0,240);  //index is now based on pixel number
  leds[i] = ColorFromPalette( celtic, paletteIndex, MASTER_BRIGHTNESS, LINEARBLEND);
 }
FastLED.show();  //display the pixels

delay(2000); 
} 
if(gap2>20 and gap<20){ //if distance variation is 20cm
//myMP3.play(2);//play the second song of the second folder 
firstTime=0;//avoid errors!!we dont like errors
Serial.println("LEFT MOVEMENT");
for (uint8_t i=0; i<NUM_LEDS; i++) {
  uint8_t paletteIndex = map(i,0,NUM_LEDS-1,0,240);  //index is now based on pixel number
  leds[i] = ColorFromPalette( autumn, paletteIndex, MASTER_BRIGHTNESS, LINEARBLEND);
 }
FastLED.show();  //display the pixels

delay(2000); 
} 
Serial.println("\\\\\\\\\\\\\\\\\\\\\\");//debugggggg
Serial.print(" New Distace:");//debugggggg
Serial.print(Distance);
Serial.print("   Old Distance: ");
Serial.print(auxDistance);
Serial.print("   GAP ");
Serial.println(gap);

Serial.print("New Distace2:");//debugggggg
Serial.print(Distance2);
Serial.print("  Old Distance2: ");
Serial.print(auxDistance2);
Serial.print("  GAP2 ");
Serial.println(gap2);
Serial.println("\\\\\\\\\\\\\\\\\\\\\\");//debugggggg

delay(300); 
auxDistance=Distance;//store the value for the if() in the next loop
auxDistance2=Distance2;//store the value for the if() in the next loop
}

long measureDistance(int trigger,int echo){
   long duration, distance;
  
  digitalWrite(trigger, LOW);  //PULSE ___|---|___
  delayMicroseconds(2); 
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigger, LOW);
  
  duration = pulseIn(echo, HIGH);
  distance = (duration/2) / 29.1;

  return distance;
}

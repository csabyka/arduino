#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include <DFPlayerMini_Fast.h>
#if !defined(UBRR1H)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
#endif

#define trigPin 7 //zold for the FIRST distance module 
#define echoPin 6 //sarga

#define trigPin2 5// zold for the SECOND  distance module 
#define echoPin2 4

#define LED_PIN 8 //LED
#define LED_COUNT 7

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int firstTime=0;//we need to declare firstTime outside the loop 
long Distance,auxDistance,gap=0;
long Distance2,auxDistance2,gap2=0;

DFPlayerMini_Fast myMP3;

void setup()
{
  Serial.begin(115200);

#if !defined(UBRR1H)
  mySerial.begin(9600);
  myMP3.begin(mySerial, true);
#else
  Serial1.begin(9600);
  myMP3.begin(Serial1, true);
#endif
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

pixels.begin();
pixels.clear();
//pixels.show();
//pixels.setBrightness(200);

}

void loop()
{
pixels.clear();

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
pixels.clear();
pixels.setBrightness(255);
pixels.setPixelColor(0, pixels.Color(0, 255, 0)); //zold
pixels.setPixelColor(1, pixels.Color(0, 255, 0)); //zold
pixels.setPixelColor(2, pixels.Color(0, 255, 0)); //zold
pixels.setPixelColor(3, pixels.Color(0, 255, 0)); //zold
pixels.setPixelColor(4, pixels.Color(0, 255, 0)); //zold
pixels.setPixelColor(5, pixels.Color(0, 255, 0)); //zold
pixels.setPixelColor(6, pixels.Color(0, 255, 0)); //zold

//pixels.setPixelColor(0, pixels.Color(255, 255, 255)); //white
//pixels.fill((255, 255, 0, 1));
// Comment this line out if you have RGBW/GRBW NeoPixels
//pixels.fill((0, 255, 0));
// Uncomment this line if you have RGBW/GRBW NeoPixels
//pixels.fill((0, 255, 0));

//delay(100);
//pixels.fill((255, 255, 255, 0));
//pixels.clear();

pixels.show();
delay(2000); 
} 
if(gap2>20 and gap<20){ //if distance variation is 20cm
//myMP3.play(2);//play the second song of the second folder 
firstTime=0;//avoid errors!!we dont like errors
Serial.println("LEFT MOVEMENT");
pixels.clear();
pixels.setBrightness(100);
pixels.setPixelColor(0, pixels.Color(255, 50, 0)); //piros
pixels.setPixelColor(1, pixels.Color(255, 50, 0)); //piros
pixels.setPixelColor(2, pixels.Color(255, 50, 0)); //piros
pixels.setPixelColor(3, pixels.Color(255, 50, 0)); //piros
pixels.setPixelColor(4, pixels.Color(255, 50, 0)); //piros
pixels.setPixelColor(5, pixels.Color(255, 50, 0)); //piros
pixels.setPixelColor(6, pixels.Color(255, 50, 0)); //piros
pixels.show();

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

//pixels.clear();
//pixels.setBrightness(10);
//pixels.setPixelColor(0, pixels.Color(255, 255, 255)); //feher
//pixels.setPixelColor(1, pixels.Color(255, 0, 0)); //piros
//pixels.setPixelColor(2, pixels.Color(0, 255, 0)); //zold
//pixels.setPixelColor(3, pixels.Color(0, 0, 255)); //kek
//pixels.setPixelColor(4, pixels.Color(255, 0, 255)); //lila
//pixels.setPixelColor(5, pixels.Color(255, 255, 0)); // sarga
//pixels.setPixelColor(6, pixels.Color(0, 255, 255)); //babakek
//pixels.show();

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
//   Serial.println("distance:");
//   Serial.println(distance);
  return distance;

}

//#define F_CPU 8000000ul
#include "TM1637.h"

#define CLK 2//pins definitions for TM1637 and can be changed to other ports 2 1
#define DIO 1

#define ledPin 7

#define RPMMAX 9999
#define RPMMIN 0
#define RPMLIGHT 5000
TM1637 tm1637(CLK,DIO);

const byte interruptPin = 3;
volatile byte state = LOW;
volatile unsigned int rev=0;      
int rpm=0;
int pre=1;

int val;
float volt;

void displayNumber(int num){   
    tm1637.display(3, num % 10);   
    tm1637.display(2, num / 10 % 10);   
    tm1637.display(1, num / 100 % 10);   
    tm1637.display(0, num / 1000 % 10);
}
void isr();


void setup() {
  CLKPR = (1<<CLKPCE);  // enable change of the clock prescaler
  CLKPR = (1<<CLKPS1)|(1<<CLKPS0);  // divide frequency by 2
  
  pinMode(ledPin,OUTPUT);
  for(int i=0;i<3;i++){
   digitalWrite(7,HIGH);
   delay(200);
   digitalWrite(7,LOW);
   delay(200);
  }
  
   tm1637.init();
   tm1637.set(7);//BRIGHT_T
   pinMode(interruptPin, INPUT_PULLUP);
   attachInterrupt(digitalPinToInterrupt(interruptPin), isr, RISING);
   
   int val=analogRead(A3);
   float volt=val*3*0.00488;
   int v1=(int)volt;
   float f2=volt-v1;
   int v2=f2*100;
   int a=12*100+v2;
   tm1637.point(POINT_ON);
   displayNumber(a);
   delay(1000);
   tm1637.point(POINT_OFF);
   displayNumber(0);
}
 
void loop() {
   delay(500);
   detachInterrupt(interruptPin);
   rpm=(((rev*2)*60)/4)/pre;  
   rev=0;
   if(rpm>RPMMAX)
    rpm=RPMMAX;
   if(rpm<RPMMIN)
    rpm=0;
   displayNumber(rpm);
   if(rpm>=RPMLIGHT){
    digitalWrite(ledPin,HIGH);
   }
   else{
    digitalWrite(ledPin,LOW);
   }
   
    if(analogRead(A4)<400)
    pre=2;
    else if(analogRead(A4)<800)
    pre=1;
    else 
    pre=4;
    
   attachInterrupt(digitalPinToInterrupt(interruptPin), isr, RISING);
}
void isr() {
   rev++;
}  

#include "Arduino.h"
#include "Comands.h"

#include "MotorBase.h"

#include "ESP8266WiFi.h"
#include "WLAN.h"

int counterserial=0;


WLAN Netzwerkverbindung;
Comands orderComands;
MotorBase testMotor;
MotorBase motor2;

NewComand command;
String commandText="";


//pin-4,1; //Rest --High


//pin-4,1; //Rest --High
//pin-5,1; //Sleep --High
//to-6,1000,320; //Steps

void setup() 
{
  Serial.begin(115200);
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW); 
  Serial.println("Start Programm");
  command.whatComand=0;
  testMotor.Initializieren(1, D2,D1);
  motor2.Initializieren(2, D3,D4);
  Netzwerkverbindung.Initializieren(2);
  
  //motor1.ModusDesMotors=3;
}

void loop()
{  
  counterserial++;
  if(counterserial==25)
  {
    counterserial=0;
    if (Serial.available() > 0)
    {
      commandText=Serial.readString();
      commandText.replace("\n","");       
    }
    Netzwerkverbindung.GetLastCommand(&commandText);
    Netzwerkverbindung.WebserverTick(&command,&commandText);
    orderComands.AnalyseOrder(commandText,&command);

    commandText="";
    motor2.CheckComand(&command);
    testMotor.CheckComand(&command);
    command.whatComand=0;

    if(testMotor.ModusDesMotors==0&&
      motor2.ModusDesMotors==0)
    {
      digitalWrite(D0, LOW);
    }
    else
    {
      digitalWrite(D0, HIGH);
    }
    
  }
  
  
  orderComands.Tick();
  testMotor.Tick();
  motor2.Tick();
  Netzwerkverbindung.Tick();
  
  delayMicroseconds(1);
}

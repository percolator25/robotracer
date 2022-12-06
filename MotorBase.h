//#include "Comands.h"

class MotorBase
{
    public:
    void Tick();
    void CheckComand(NewComand* com);
    void Initializieren(uint16_t index,uint16_t pinDirection, uint16_t pinSteps);
    void Stop();
    int getPosition();

    uint16_t ModusDesMotors=0;     
    //Modus in dem der Motor gerade fährt
    //0 Motor steht
    //1 Motor bewegt sich im Test Modus
    //2 Motor bewegt sich auf eine Position
    //3 Motor bewegt sich dauerhaft

    private:
    uint16_t motorIndex=0;
    uint16_t motorPinDirection=0;
    uint16_t motorPinSteps=0;
    bool lastStatus=LOW;
    
    

    int counterIntervall=0;
    int motorSpeed=50; //Geschwindikeit 
    
    int targetCounter=0;   //ziel wo der Motor hinfahren soll
    bool forward=false;   //true vorwärts false ruckwärts
    int stepCounter=0;    //Anzahl an vergangen Schritte hierraus kann man die Position berechnen
    
};

void MotorBase::Initializieren(uint16_t index,uint16_t pinDirection, uint16_t pinSteps)
{
    motorIndex=index;
    motorPinDirection=pinDirection;
    motorPinSteps=pinSteps;

    pinMode(motorPinDirection, OUTPUT);
    pinMode(motorPinSteps, OUTPUT);
}

void MotorBase::CheckComand(NewComand* com)
{
  if(com->whatComand!=0)//es ist ein Beweg dich comando
  {
    if(com->motorNumber==motorIndex||com->motorNumber2==motorIndex)//der befehl geht zu meinem motor
    {
      switch(com->whatComand)
      {
        case 1:
              ModusDesMotors=2;
              motorSpeed=50;
              counterIntervall=0;
              targetCounter=targetCounter+com->motorTarget;
              if(targetCounter>stepCounter)
              {
                forward=true;
                digitalWrite(motorPinDirection, true);
              }
              else if(targetCounter<stepCounter)
              {
                forward=false;
                digitalWrite(motorPinDirection, false);
              }
              else
              {
                Serial.println("Motor am ziel");
              }
              //stepCounter=0;
              
              break;
        case 2:
              ModusDesMotors=3;
              counterIntervall=0;
              motorSpeed=1000-(com->motorSpeed*6);
              Serial.print("neue Geschwindigkeit :  ");
              Serial.println(motorSpeed);
              break;

        case 3:
              ModusDesMotors=0;
              Serial.println("Motor Stop");
              break;

        case 4:
              Serial.print("ModusDesMotors :  ");
              Serial.println(ModusDesMotors);
              Serial.print("Motor steps :  ");
              Serial.println(stepCounter);
              break;
      }
    }
    
  }
}

int MotorBase::getPosition()
{
  return stepCounter;
}

void MotorBase::Stop()
{
  ModusDesMotors=0;
}

///Muss zeitlich immer exakt abgearbeitet werden
void MotorBase::Tick()
{
  if(ModusDesMotors==0)
  {
    return; //Motor steht nix machen
  }

  counterIntervall++;
  if(counterIntervall==motorSpeed)// es ist zeit zum schalten
  {
    digitalWrite(motorPinSteps, lastStatus);
    //Serial.println(stepCounter);
    counterIntervall=0;
    if(lastStatus==LOW)
    {
      lastStatus=HIGH;
    }
    else
    {
      lastStatus=LOW;
    }
    if(forward)
    {
      stepCounter++;     
    }
    else
    {
       stepCounter--; 
    }
    if(stepCounter==targetCounter && ModusDesMotors==2)
    {
      Serial.println("Motor am ziel bewegung");
      ModusDesMotors=0;
    }
  }
}

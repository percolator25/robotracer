struct NewComand
{
  int whatComand;
  int motorNumber;
  int motorNumber2;

  int motorTarget;
  int motorSpeed;
};

class Comands
{
    public:
      void AnalyseOrder(String Order,NewComand* com);
      void Tick();

    private:
      uint16_t pins[9] = {D0, D1, D2, D3, D4, D5, D6, D7, D8};
      int tooglePIN=-1;
      long activeSteeps=0;
      int steepsMax=0;
      bool lastStatus=LOW;
      int counterIntervall=0;
      uint16_t interval_us = 1;
};

void Comands::Tick()
{
  if(tooglePIN!=-1)
  {
    counterIntervall++;
    if(counterIntervall==interval_us)
    {
      counterIntervall=0;
      digitalWrite(pins[tooglePIN], lastStatus);
      activeSteeps++;
      if(lastStatus==LOW)
      {
        lastStatus=HIGH;
        //Serial.println(activeSteeps);
      }
      else
      {
        lastStatus=LOW;
      }
      if(steepsMax!=-1)
      {
        if(activeSteeps>steepsMax)
        {
          tooglePIN=-1;
          activeSteeps=0;
        }
      }
    }
  }
}

void Comands::AnalyseOrder(String Order,NewComand* com)
{
  if(Order.length()==0)
  {
    return;
  }
  Serial.print("I received: ");
  Serial.println(Order);
  if(Order.indexOf("testpin") == 0)
  {
    //seperate nachricht
    // "Comando"-"pinnumber","0oder1";
    int cmdPos = Order.indexOf("-");
    int pinNummerPos = Order.indexOf(",");
  
    String cmdText = Order.substring(0,cmdPos);
    String pinNummerText= Order.substring(cmdPos + 1, pinNummerPos);
    String zustandsText = Order.substring(pinNummerPos + 1, Order.length());
    
    int pinNummer = pinNummerText.toInt();
    int highorlow = zustandsText.toInt();
    Serial.println(pinNummer);
    Serial.println(highorlow);
    if(highorlow==0)
    {
      pinMode(pins[pinNummer], OUTPUT);
      digitalWrite(pins[pinNummer], LOW);
    }
    if(highorlow==1)
    {
      pinMode(pins[pinNummer], OUTPUT);
      digitalWrite(pins[pinNummer], HIGH);
    }
    
  }
  if(Order.indexOf("testto") == 0)
  {
    //seperate nachricht
    // "Comando"-"pinnumber","speed","anzahlsteps";
    int cmdPos = Order.indexOf("-");
    int pinNummerPos = Order.indexOf(",");
    int speedNummerPos = Order.indexOf(",",pinNummerPos+1);
  
    String cmdText = Order.substring(0,cmdPos);
    String pinNummerText= Order.substring(cmdPos + 1, pinNummerPos);
    String speedText= Order.substring(pinNummerPos + 1, speedNummerPos);
    String steepsText = Order.substring(speedNummerPos + 1, Order.length()-1);
    
    int pinNummer = pinNummerText.toInt();
    int speedTimer = speedText.toInt();        
    int steepsNumber = steepsText.toInt();
    Serial.println(pinNummerText);
    Serial.println(speedTimer);
    Serial.println(steepsNumber);
    tooglePIN=pinNummer;
    interval_us=speedTimer;    
    steepsMax= steepsNumber;
  }
  //Motorfahren
  if(Order.indexOf("to") == 0)
  {
    //seperate nachricht
    // "Comando"-"motorNumber","motorTarget";
    int cmdPos = Order.indexOf("-");
    int pinMotorPos = Order.indexOf(",");
  
    String pinMotorText= Order.substring(cmdPos + 1, pinMotorPos);
    String targetText = Order.substring(pinMotorPos + 1, Order.length()-1);

    com->whatComand=1;
    com->motorNumber=pinMotorText.toInt();
    com->motorTarget = targetText.toInt(); 
  }

  if(Order.indexOf("newspeed") == 0)
  {
    //seperate nachricht
    // "Comando"-"motorNumber","motorSpeed";
    int cmdPos = Order.indexOf("-");
    int pinMotorPos = Order.indexOf(",");
  
    String pinMotorText= Order.substring(cmdPos + 1, pinMotorPos);
    String speedText = Order.substring(pinMotorPos + 1, Order.length()-1);

    com->whatComand=2;
    com->motorNumber=pinMotorText.toInt();
    com->motorSpeed = speedText.toInt(); 
  }

  if(Order.indexOf("stop") == 0)
  {
    //seperate nachricht
    // "Comando"-"motorNumber";
    int cmdPos = Order.indexOf("-");
  
    String pinMotorText= Order.substring(cmdPos + 1, Order.length()-1);

    com->whatComand=3;
    com->motorNumber=pinMotorText.toInt();
  }

  if(Order.indexOf("getPos") == 0)
  {
    //seperate nachricht
    // "Comando"-"motorNumber";
    int cmdPos = Order.indexOf("-");
  
    String pinMotorText= Order.substring(cmdPos + 1, Order.length()-1);

    com->whatComand=4;
    com->motorNumber=pinMotorText.toInt();
  }

  if(Order.indexOf("syn") == 0)
  {
    //seperate nachricht
    // "Comando"-"motorNumber"-"motorNumber2","motorTarget";
    int cmdPos = Order.indexOf("-");
    int cmdPos2 = Order.indexOf("-",cmdPos+1);
    int pinMotorPos = Order.indexOf(",");
  
    //String cmdText = Order.substring(0,cmdPos);
    String pinMotorText= Order.substring(cmdPos + 1, cmdPos2);
    String pinMotorText2= Order.substring(cmdPos2 + 1, pinMotorPos);
    String targetText = Order.substring(pinMotorPos + 1, Order.length()-1);

    com->whatComand=1;
    com->motorNumber=pinMotorText.toInt();
    com->motorNumber2=pinMotorText2.toInt();
    com->motorTarget = targetText.toInt(); 
  }

}

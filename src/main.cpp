#include <Arduino.h>
#include "Motor.h"
#include "Data.h"
#include "Wifi.h"
#include "Web.h"
#include "Com.h"

Motor left;
Motor right;

void receiver(WebsocketsClient &who, String where, String what)
{
  if (where == "rf")
  {
    if (what == "down")
    {
      right.move_forwards(100);
    }
    else
    {
      right.stop();
    }
  }
  if (where == "rb")
  {
    if (what == "down")
    {
      right.move_backwards(100);
    }
    else
    {
      right.stop();
    }
  }

  if (where == "lf")
  {
    if (what == "down")
    {
      left.move_forwards(100);
    }
    else
    {
      left.stop();
    }
  }
  if (where == "lb")
  {
    if (what == "down")
    {
      left.move_backwards(100);
    }
    else
    {
      left.stop();
    }
  }

}

void setup()
{
  Serial.begin(9600);

  Web::receive(&receiver);

  Web::startHTTP();
  Web::startWS();

  left.init(1, D2, D1, true);
  right.init(2, D3, D4, true);

  //Com::start(&left, &right);

  Serial.println("Setup finished");

}

void loop()
{
  Web::tick();
  left.tick();
  right.tick();
  delayMicroseconds(10);
}
#include <Arduino.h>
#include "Motor.h"
#include "Data.h"
#include "Wifi.h"
#include "Web.h"

Motor left;
Motor right;

void receiver(WebsocketsClient &who, String where, String what)
{
  if (where == "rf")
  {
    if (what == "down")
    {
      right.move_forwards(10);
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
      right.move_backwards(10);
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
      left.move_forwards(10);
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
      left.move_backwards(10);
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

  Wifi::development();

  Web::startHTTP();
  Web::startWS();

  digitalWrite(D0, LOW);

  left.init(1, D2, D1, false);
  right.init(2, D3, D4, true);

  Serial.println("Setup finished");

}

void loop()
{
  Web::tick();
  left.tick();
  right.tick();
  delayMicroseconds(100);
}
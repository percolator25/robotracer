#pragma once
#include <Arduino.h>
#include <Motor.h>
#include <Wire.h>

class Com
{
public:
    static Motor *left;
    static Motor *right;

    static void start(Motor *left, Motor *right)
    {
        Wire.begin(11);
        Wire.onReceive(handleReceive);
        Wire.onRequest(handleRequest);
        Serial.println("Com started");

        Com::left = left;
        Com::right = right;
    }

    static void handleReceive(int numBytes)
    {
        int operation = Wire.read();
        int direction = Wire.read();
        int motor = Wire.read();
        int speed = Wire.read();
        int target = Wire.read();

        Motor *selected = nullptr;

        if (motor == 0)
        {
            selected = left;
        }

        if (motor == 1)
        {
            selected = right;
        }

        if (selected != nullptr)
        {
            if (operation == 0) // Stop
            {
                selected->stop();
            }

            if (operation == 1) // Set motors
            {

                if (direction == 0) // Left
                {

                    if (target > 0)
                    {
                        selected->move_forwards(target, speed); // Specific
                    }
                    else
                    {
                        selected->move_forwards(speed); // Unspecific
                    }
                }
                else if (direction == 1) // Right
                {

                    if (target > 0)
                    {
                        selected->move_backwards(target, speed); // Specific
                    }
                    else
                    {
                        selected->move_backwards(speed); // Unspecific
                    }
                }
            }
        }
    }

    static void handleRequest()
    {
    }
};


Motor *Com::left = nullptr;
Motor *Com::right = nullptr;
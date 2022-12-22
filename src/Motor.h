#include <Arduino.h>

class Motor
{
public:
    void tick();
    void init(uint16_t index, uint16_t pinDirection, uint16_t pinSteps, bool forward);
    void stop();
    int get_position();
    void move_forwards(int target, uint16_t speed);
    void move_forwards(uint16_t speed);

    void move_backwards(int target, uint16_t speed);
    void move_backwards(uint16_t speed);
    int get_mode();
    int get_stepps();

    /**
     * @brief 0 = No movement; 1 = <Reservated>; 2 = Move specific; 3 = Move endlessly;
     *
     */
    uint16_t mode = 0;

private:
    uint16_t motor_index = 0;
    uint16_t motor_direction = 0;
    uint16_t motor_pin_steps = 0;
    bool lastStatus = LOW;

    int counter_intervall = 0;
    int motor_speed = 50; // Geschwindikeit

    int targetCounter = 0; // ziel wo der Motor hinfahren soll
    bool forward = true;   // true vorwärts false ruckwärts
    int stepCounter = 0;   // Anzahl an vergangen Schritte hierraus kann man die Position berechnen

    void move(int target, uint16_t speed);
    void move(uint16_t speed);
    bool initial_direction = true;
};

void Motor::init(uint16_t index, uint16_t pinDirection, uint16_t pinSteps, bool initial_direction)
{
    motor_index = index;
    motor_direction = pinDirection;
    motor_pin_steps = pinSteps;
    this->initial_direction = initial_direction;

    pinMode(motor_direction, OUTPUT);
    pinMode(motor_pin_steps, OUTPUT);
}

void Motor::move_forwards(int target, uint16_t speed)
{
    forward = initial_direction;
    move(target, speed);
}

void Motor::move_forwards(uint16_t speed)
{
    Serial.println(speed);
    forward = initial_direction;
    move(speed);
}

void Motor::move_backwards(int target, uint16_t speed)
{
    forward = !initial_direction;
    move(target, speed);
}

void Motor::move_backwards(uint16_t speed)
{
    Serial.println(speed);
    forward = !initial_direction;
    move(speed);
}

/**
 * @brief Move the motor a specific time with a specific speed
 *
 * @param target Stepps to make
 * @param speed Ticks to skip
 */
void Motor::move(int target, uint16_t speed)
{

    Serial.print("Moving to target ");
    Serial.print(target);
    Serial.print("at speed ");
    Serial.print(speed);

    mode = 2;
    counter_intervall = 0;

    if (forward)
    {
        targetCounter = targetCounter + target;
        digitalWrite(motor_direction, initial_direction);
        Serial.print(" with direction ");
        Serial.println(initial_direction);
    }
    else
    {
        targetCounter = targetCounter - target;
        digitalWrite(motor_direction, !initial_direction);
        Serial.print(" with direction ");
        Serial.println(initial_direction);
    }
}

/**
 * @brief Endlessly spin the motor
 *
 * @param speed Ticks to skip
 */
void Motor::move(uint16_t speed)
{
    mode = 3;
    counter_intervall = 0;
    motor_speed = speed;

    if (forward)
    {
        Serial.print("Moving forwards at speed ");
        Serial.print(speed);
        digitalWrite(motor_direction, true);
        Serial.print(" with direction ");
        Serial.println(initial_direction);
    }
    else
    {
        Serial.print("Moving backwards at speed ");
        Serial.print(speed);
        digitalWrite(motor_direction, false);
        Serial.print(" with direction ");
        Serial.println(!initial_direction);
    }
}

void Motor::stop()
{
    Serial.println("Stopped");
    mode = 0;
}

int Motor::get_position()
{
    return stepCounter;
}

/**
 * @brief Updates the motor
 *
 */
void Motor::tick()
{
    if (mode == 0)
    {
        return; // Nothing to do
    }

    counter_intervall++;
    if (counter_intervall == motor_speed) // Time to move
    {
        digitalWrite(motor_pin_steps, lastStatus);
        counter_intervall = 0;
        if (lastStatus == LOW)
        {
            lastStatus = HIGH;
        }
        else
        {
            lastStatus = LOW;
        }
        if (forward == initial_direction)
        {
            stepCounter++;
        }
        else
        {
            stepCounter--;
        }
        if (stepCounter == targetCounter && mode == 2)
        {
            // Taret reached
            mode = 0;
        }
    }
}
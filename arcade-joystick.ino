/*
  arcade-joystick

  Simple joystick routine for use with recalbox. 
  Includes control for servo motor used to switch between 4 and 8 way restrictor plate.
  
*/

#include <Joystick.h>
#include <Servo.h>

#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )

const int inputs[]      = {2,3,4,5,6,7,8,9,21,20,19,18,16};  //inputs used on the arduino
const int inputs_length = arr_len(inputs);

const int servo_signal  = 10;     //output used to signal servo
const int servo_min     = 500;    //minimum pulsewidth for servo 
const int servo_max     = 2600;   //maximum pulsewidth for servo

int state[inputs_length];     //button state

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  8, 0,                  // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

Servo servo;

void setup() {
  //configure inputs and set original state
  for (int pin = 0; pin < inputs_length; pin++) {
    pinMode(inputs[pin], INPUT_PULLUP);
    state[pin] = LOW;
  }
  
  Joystick.begin();
  Joystick.setXAxisRange(-1, 1);
  Joystick.setYAxisRange(-1, 1);

  toggleServo();
}

void loop() {
  for (int i = 0; i < inputs_length; i++) {
    int currentState = !digitalRead(inputs[i]);
    if (state[i] != currentState) {
      switch (inputs[i]) {
        case 21:
          Joystick.setYAxis(currentState * -1);
          break;
        case 20:
          Joystick.setYAxis(currentState);
          break;
        case 19:
          Joystick.setXAxis(currentState * -1);
          break;
        case 18:
          Joystick.setXAxis(currentState);
          break;
        case 16:
          if (currentState == HIGH) toggleServo();
          currentState = LOW;
          break;
        default:
          Joystick.setButton(i, currentState);  
      }
      state[i] = currentState;
    }
  }
  delay(10);
}

void toggleServo() {
  servo.attach(servo_signal, servo_min, servo_max);
  delay(200);
  if(servo.read() != 179) {
    servo.write(179);
  } else {
    servo.write(9);
  }
  delay(1500);
  servo.detach();
}

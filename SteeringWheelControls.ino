// by samirh. 
// 04-26-2019

#include "HID-Project.h"

const int buttons[][4] = {
  {1, 6, 1},                    // next
  {15, 24, 2},                  // play/pause 
  {7, 13, 3},                   // previous 
  {25, 35, 4},                  // mute 
  {60, 70, 5},                  // vol_down   
  {40, 50, 6},                  // vol_up
};

const int analogInPin = A0;     // analog input pin that the stepped resistor circuit is attached to

int x = 0;                      // value read from the pot
int i;                          // button loop-counter
int c = 0;                      // button streak-counter
int v = 5;                      // verify necessary detection length in loops to press button
int vr = 5;                     // verify necessary detection length in loops to release button
int d = 10;                     // check-loop duration in ms

int d2 = 10;                    // button-hold-loop duration in ms
int pressed = false;            // loop break condition for holding the button

void setup() {
  Serial.begin(9600);           // initialize serial communications at 9600 bps
  Consumer.begin();
}

void loop() {
  x = analogRead(analogInPin);  // read the analog in value and print to serial
  Serial.println(x);

  for(i = 0; i <= 5; i = i + 1) {                   // loop through all possible buttons
    if(x <= buttons[i][0] || x >= buttons[i][1]) {  // if this button is not detected: skip to next iteration
      continue;
    }

    Serial.print("button detected for value ");     // button is detected
    Serial.print(x);

    c = c + 1;

    Serial.print(", c = ");
    Serial.println(c);

    if(c < v) {                                     // if button is not detected enough times: break
      break;
    }

    buttonPress(i);                                 // send button press event
    c = 0;
    break;
  }

  delay(d);                                         // delay next read
}

void buttonPress(int i){
  Serial.print("going to send press for button int ");
  Serial.println(i);

  Consumer.begin();                                 // sends a clean report to the host
  int ascii = buttons[i][2];
  c = 0;
  
  Serial.print("pressed button ");
  Serial.println(ascii);

  pressType(ascii);
  pressed = true;

  while(pressed) {
    x = analogRead(analogInPin);

    if(x <= buttons[i][0] || x >= buttons[i][1]) {
      Serial.print("outvalue detected: ");
      Serial.println(x);

      c = c + 1;
    } else {
      c = 0;
    }

    if(c >= vr) {
      pressed = false;
    }

    delay(d2);
  }

  releaseType(ascii);

  Serial.print("released button ");
  Serial.println(ascii);
}

void pressType(int ascii) {
  switch (ascii) {
  case 1:
    Consumer.press(MEDIA_NEXT);
    break;
  case 2:
    Consumer.press(MEDIA_PLAY_PAUSE);
    break;
  case 3:
    Consumer.press(MEDIA_PREVIOUS);
    break;
  case 4:
    Consumer.press(MEDIA_VOLUME_MUTE);
    break;
  case 5:
    Consumer.press(MEDIA_VOLUME_DOWN);
    break;
  case 6:
    Consumer.press(MEDIA_VOLUME_UP);
    break;
  default:
    break;
  }
}

void releaseType(int ascii) {
  switch (ascii) {
  case 1:
    Consumer.release(MEDIA_NEXT);
    break;
  case 2:
    Consumer.release(MEDIA_PLAY_PAUSE);
    break;
  case 3:
    Consumer.release(MEDIA_PREVIOUS);
    break;
  case 4:
    Consumer.release(MEDIA_VOLUME_MUTE);
    break;
  case 5:
    Consumer.release(MEDIA_VOLUME_DOWN);
    break;
  case 6:
    Consumer.release(MEDIA_VOLUME_UP);
    break;
  default:
    break;
  }
}
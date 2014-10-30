#include "button_handler.h"
#include <Arduino.h>

ButtonHandler::ButtonHandler(int p, int db)
: pin(p), debounce_threshold(db) {
}

void ButtonHandler::init() {
  pinMode(pin, INPUT);
  // connect internal pull-up resistor (will read LOW when pressed)
  digitalWrite(pin, HIGH);
  state = false;
  was_pressed = false;
  last_debounce_time = 0;
}

// check for state change and handle debounce
int ButtonHandler::handle() {
  int now_pressed = !digitalRead(pin);

  if (now_pressed != was_pressed) {
    last_debounce_time = millis();
  }

  if ((millis() - last_debounce_time) > debounce_threshold) {
    if (now_pressed != state) {
      state = now_pressed;
    }
  }

  was_pressed = now_pressed;
  return state;
}
#include <SoftwareSerial.h>
#include "button_handler.h"

enum Modes {
  PLAY, // play current sequence
  STOP // do nothing
};

Modes active_mode = STOP;

const int DEBOUNCE_THRESHOLD = 10;
const int START_BUTTON_PIN = 2;
boolean last_start_button_state = false;

int active_program = 0; // stored program # 0-3

// TODO SequencePlayer class for sequence management
// sequence will eventually be loaded from EEPROM
int sequence[] = {127, 0};
int current_step = 0;
long seq_speed = 150; // milliseconds per step
long last_seq_step_time = 0;

SoftwareSerial midi_serial(10, 11); // RX, TX

ButtonHandler start_button(START_BUTTON_PIN, DEBOUNCE_THRESHOLD);

// the setup function runs once when you press reset or power the board
void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
  
  //  Set MIDI baud rate:
  midi_serial.begin(31250);

  start_button.init();
}

// the loop function runs over and over again forever
void loop() {
  start_button.handle();

  // switch mode if start button is pressed
  if (start_button.state && !last_start_button_state) {
    active_mode = active_mode == STOP ? PLAY : STOP;

    if (active_mode == STOP) {
      reset_sequence();
    }
  }
  last_start_button_state = start_button.state;

  switch (active_mode) {
    case PLAY:
      play_loop();
      break;
    case STOP:
      stop_loop();
      break;
  }

}

void play_loop() {
  if ((millis() - last_seq_step_time) > seq_speed) {
    last_seq_step_time = millis();

    send_whammy_position((byte) sequence[current_step]);

    current_step += 1;
    current_step = current_step % (sizeof(sequence) / sizeof(sequence[0]));
  }
}

void stop_loop() {
  // nothing to do here at the moment
}

void reset_sequence() {
  send_whammy_position((byte) 0);
  current_step = 0;
  last_seq_step_time = 0;
}

void send_whammy_position(byte value) {
  #ifdef DEBUG
  Serial.print("set position:");
  Serial.print(value);
  Serial.print("\n");
  #endif

  midi_serial.write((byte) 176); // CC on Channel 1 (10110000)
  midi_serial.write((byte) 11); // Controller 11
  midi_serial.write(value);
}




enum Modes {
  PLAY, // play current sequence
  STOP, // do nothing
  PROGRAM // allow programming sequences
};

Modes active_mode = STOP;

int active_program = 0; // stored program # 0-3
int current_step = 0;

// sequence will eventually be loaded from EEPROM
int sequence[] = {0, 127};


// the setup function runs once when you press reset or power the board
void setup() {
  //  Set MIDI baud rate:
  Serial.begin(31250);
}

// the loop function runs over and over again forever
void loop() {
  // there will eventually be loops for "stop" and "program" modes
  play_loop();
}

void play_loop() {
  send_whammy_position((byte) sequence[current_step]);

  // TODO get rid of delay and use millis()
  delay(250);

  // next step
  current_step += 1;
  current_step = current_step % (sizeof(sequence) / sizeof(sequence[0]));
}

void send_whammy_position(byte value) {
  Serial.write((byte) 176); // CC on Channel 1 (10110000)
  Serial.write((byte) 11); // Controller 11
  Serial.write(value); // value
}


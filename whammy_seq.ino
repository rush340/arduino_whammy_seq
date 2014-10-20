

enum Modes {
  PLAY, // play current sequence
  STOP, // do nothing
  PROGRAM // allow programming sequences
};

Modes active_mode = STOP;

int active_program = 0; // stored program # 0-3
int current_step = 0;

// sequence will eventually be loaded from EEPROM
int sequence[] = {0, 127, 0, 127, 0, 127, 0, 127};


// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  blink();

  // there will eventually be loops for "stop" and "program" modes
  play_loop();
}

void blink() {
  // blink built-in LED to show program is running
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(50);
}

void play_loop() {
  send_whammy_position(sequence[current_step]);

  // next step
  current_step += 1;
  current_step = current_step % (sizeof(sequence) / sizeof(sequence[0]));
}

void send_whammy_position(int value) {
  // TODO
}
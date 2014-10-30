#include <SoftwareSerial.h>

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

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// TODO move ButtonHandler to its own file

// button class that takes care of setup and debounce
class ButtonHandler {
  public:
    ButtonHandler(int pin, int debounce_threshold);
    void init();
    int handle();
    boolean state;

  protected:
    boolean was_pressed;
    long last_debounce_time;
    const int pin;
    const long debounce_threshold;
};

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

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


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

    // if changed to STOP, reset sequence
    if (active_mode == STOP) {
      send_whammy_position((byte) 0);
      current_step = 0;
      last_seq_step_time = 0;
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


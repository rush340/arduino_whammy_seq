#ifndef BUTTON_HANDLER_H_
#define BUTTON_HANDLER_H_

// button class that takes care of setup and debounce
class ButtonHandler {
  public:
    ButtonHandler(int pin, int debounce_threshold);
    void init();
    int handle();
    bool state;

  protected:
    bool was_pressed;
    long last_debounce_time;
    const int pin;
    const long debounce_threshold;
};

#endif  // BUTTON_HANDLER_H_
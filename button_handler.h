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
    const int pin;
    const long debounce_threshold;
    bool was_pressed;
    long last_debounce_time;
};

#endif  // BUTTON_HANDLER_H_
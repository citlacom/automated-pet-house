#include <LedControl.h>

// Digit screen connections.
#define DATA_PIN A0
#define CS_PIN A1
#define CLOCK_PIN A2

LedControl lc = LedControl(DATA_PIN, CLOCK_PIN, CS_PIN, 1);

void setup() {
    lc.shutdown(0, false);
    lc.setIntensity(0, 10);
    lc.clearDisplay(0);
}

void loop() {
    left_screen_print_number(1313);
    right_screen_print_number(1313);
}

// Print multi-digit number on the left screen.
void left_screen_print_number(int v) {
    int ones;
    int tens;
    int hundreds;
    int thousands;

    ones = v%10;
    v = v/10;
    tens = v%10;
    v = v/10;
    hundreds = v%10;
    thousands = v/10;

    // Now print the number digit by digit
    lc.setDigit(0, 3, (byte)thousands, false);
    lc.setDigit(0, 2, (byte)hundreds, false);
    lc.setDigit(0, 1, (byte)tens, false);
    lc.setDigit(0, 0, (byte)ones, false);
}

// Print multi-digit number on the right screen.
void right_screen_print_number(int v) {
    int ones;
    int tens;
    int hundreds;
    int thousands;

    ones = v%10;
    v = v/10;
    tens = v%10;
    v = v/10;
    hundreds = v%10;
    thousands = v/10;

    // Now print the number digit by digit
    lc.setDigit(0, 7, (byte)thousands, false);
    lc.setDigit(0, 6, (byte)hundreds, false);
    lc.setDigit(0, 5, (byte)tens, false);
    lc.setDigit(0, 4, (byte)ones, false);
}

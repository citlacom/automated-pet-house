#include <LedControl.h>
#include <idDHTLib.h>

// DHT11 sensor connection.
#define DHT_PIN 2

// Digit screen connections.
#define DATA_PIN A0
#define CS_PIN A1
#define CLOCK_PIN A2

LedControl lc = LedControl(DATA_PIN, CLOCK_PIN, CS_PIN, 1);

// Lib instantiate
idDHTLib DHTLib(DHT_PIN, idDHTLib::DHT11);

void setup() {
    lc.shutdown(0, false);
    lc.setIntensity(0, 10);
    lc.clearDisplay(0);
}

void loop() {
    int result = DHTLib.acquireAndWait();

    // There is a lecture error.
    if (result != IDDHTLIB_OK) {
        screen_alert_error();
    }
    else {
        // There is a lecture error.
        left_screen_print_number((int) DHTLib.getCelsius());
        right_screen_print_number((int) DHTLib.getHumidity());

        // DHT11 sampling rate is 1HZ.
    }

    delay(2000);
}

void screen_alert_error() {
    lc.setChar(0, 0, 'E', false);
    lc.setChar(0, 1, 'E', false);
    lc.setChar(0, 2, 'E', false);
    lc.setChar(0, 3, 'E', false);
    lc.setChar(0, 4, 'E', false);
    lc.setChar(0, 5, 'E', false);
    lc.setChar(0, 6, 'E', false);
    lc.setChar(0, 7, 'E', false);
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

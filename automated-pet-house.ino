#include <LedControl.h>
#include <idDHTLib.h>

// Relay connection.
#define RELAY_PIN A3

// DHT11 sensor connection.
#define DHT_PIN 2
// Maximum tolerated DHT lecture failures before react to stop heat.
#define MAX_LECTURE_FAILURES 30

// Digit screen connections.
#define DATA_PIN A0
#define CS_PIN A1
#define CLOCK_PIN A2

/////////////////////////////////////////////////
// Control tresholds of humidity and temperature.
/////////////////////////////////////////////////

// Minimum temperature to trigger heat on.
#define MIN_TEMPERATURE 25
// Maximum temperature to trigger heat off.
#define MAX_TEMPERATURE 35
// Min humidity to trigger heat off.
#define MIN_HUMIDITY 30

LedControl lc = LedControl(DATA_PIN, CLOCK_PIN, CS_PIN, 1);

// Lib instantiate
idDHTLib DHTLib(DHT_PIN, idDHTLib::DHT11);
int count_fail_lectures = 0;

void setup() {
    lc.shutdown(0, false);
    lc.setIntensity(0, 10);
    lc.clearDisplay(0);
    // Enable the RELAY pin.
    pinMode(RELAY_PIN, OUTPUT);
    // Start the RELAY closed in normally as is connected NC mode.
    digitalWrite(RELAY_PIN, LOW);
}

void loop() {
    int result = DHTLib.acquireAndWait();
    int temperature = 0;
    int humidity = 0;

    // There is a lecture error.
    if (result != IDDHTLIB_OK) {
        screen_alert_error();
        count_fail_lectures++;
    }
    else {
        // Get temperature and humidity lecture.
        temperature = (int) DHTLib.getCelsius();
        humidity = (int) DHTLib.getHumidity();
        left_screen_print_number(temperature);
        right_screen_print_number(humidity);
        // Good lecture, reset counter so we protect false detection of sensor
        // failure that force to stop heat to prevent burn your pet.
        count_fail_lectures = 0;
    }

    // When humidity or temperature reach a threshold stop heat
    // due starts to be an incomfortable levels.
    if (humidity < MIN_HUMIDITY || temperature > MAX_TEMPERATURE) {
        digitalWrite(RELAY_PIN, LOW);
    }

    // Something is going wrong with DHT sensor so we need to fallback
    // on turning relay off.
    if (count_fail_lectures > MAX_LECTURE_FAILURES) {
        digitalWrite(RELAY_PIN, LOW);
    }

    // Ensure that we have a minimum of humidity so when temperature
    // reach hour low threshold is time to turn on heat.
    if (humidity >= MIN_HUMIDITY && temperature < MIN_TEMPERATURE) {
        digitalWrite(RELAY_PIN, HIGH);
    }

    // DHT11 sampling rate is 1HZ.
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

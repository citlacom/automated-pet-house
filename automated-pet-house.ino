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

// Flame sensor.
#define FLAME_PIN 3

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
int screen_position = 7;
int temperature = 0;
int humidity = 0;

void setup() {
    lc.shutdown(0, false);
    lc.setIntensity(0, 4);
    lc.clearDisplay(0);
    // Enable the RELAY pin.
    pinMode(RELAY_PIN, OUTPUT);
    // Start the RELAY closed in normally as is connected NC mode.
    digitalWrite(RELAY_PIN, LOW);
    // Start the flame sensor PIN.
    pinMode(FLAME_PIN, INPUT);
    // Start serial communication.
    Serial.begin(9600);
}

void loop() {
    int result = DHTLib.acquireAndWait();
    int flame = digitalRead(FLAME_PIN);
    Serial.print("Flame: ");
    Serial.println(flame);

    // Clean screen.
    screen_clear();

    // There is a lecture error.
    if (result != IDDHTLIB_OK) {
        count_fail_lectures++;
    }
    else {
        // Get temperature and humidity lecture.
        temperature = (int) DHTLib.getCelsius();
        humidity = (int) DHTLib.getHumidity();
        // Good lecture, reset counter so we protect false detection of sensor
        // failure that force to stop heat to prevent burn your pet.
        count_fail_lectures = 0;
    }

    screen_print_number(temperature);
    screen_print_number(humidity);
    screen_print_number(flame);

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

// Print multi-digit number on the left screen.
void screen_print_number(int number) {
    if (screen_position < 0) {
        Serial.println("No more slots in the screen.");
        return;
    }

    // Fallback when number is zero.
    if (number == 0) {
        lc.setDigit(0, screen_position, number, false);
        screen_position++;
    }

    Serial.println(number);
    while (number != 0) {
        short int digit = (number % 10);
        // Print digit on next screen position.
        lc.setDigit(0, screen_position, (byte)digit, false);
        number /= 10;
        // Increment screen position.
        screen_position++;
        Serial.println(screen_position);
    }

    // Print dash separator.
    Serial.println(screen_position);
    lc.setChar(0, screen_position, '-', false);
    screen_position++;
}

void screen_clear() {
    lc.clearDisplay(0);
    screen_position = 0;
}

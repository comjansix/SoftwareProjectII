#define PIN_LED 7
unsigned int count, toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200); // Initialize serial port
  while (!Serial) {
    ; // wait for the serial port to connect.
  }
  count = toggle = 0;
  digitalWrite(PIN_LED, toggle); // turn off LED.
}

void loop() {
  digitalWrite(PIN_LED, LOW); // update LED status.
  delay(1000); // wait for 1,000 milliseconds
  for(int i = 0; i<10; i++) {
    toggle = toggle_state(toggle); // toggle LED value.
    digitalWrite(PIN_LED, toggle); // update LED status.
    delay(100);
  }


  while(1) {
  digitalWrite(PIN_LED, HIGH);
  }
}

int toggle_state(int toggle) {
  
  return !toggle;
}

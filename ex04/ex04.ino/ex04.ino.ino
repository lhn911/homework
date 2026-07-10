const int ledPin = 2;
const int touchPin = T0;

bool ledState = false;
bool lastTouch = false;
unsigned long debounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  digitalWrite(ledPin, LOW);
}

void loop() {
  unsigned long now = millis();
  int touchValue = touchRead(touchPin);
  bool currTouch = (touchValue < 40);

  if (currTouch != lastTouch) {
    debounceTime = now;
  }

  if (now - debounceTime > debounceDelay) {
    if (currTouch == true && lastTouch == false) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
    }
  }

  lastTouch = currTouch;
}

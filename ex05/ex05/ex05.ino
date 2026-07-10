const int ledPin = 2;
const int touchPin = T0;

bool lastTouch = false;
unsigned long debounceTime = 0;
const unsigned long debounceDelay = 50;

int speedLevel = 1;
int brightness = 0;
int fadeDir = 1;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  unsigned long now = millis();
  int touchValue = touchRead(touchPin);
  bool currTouch = (touchValue < 40);

  if (currTouch != lastTouch) {
    debounceTime = now;
  }

  if (now - debounceTime > debounceDelay) {
    if (currTouch && !lastTouch) {
      speedLevel++;
      if (speedLevel > 3) {
        speedLevel = 1;
      }
    }
  }
  lastTouch = currTouch;

  int step;
  switch(speedLevel){
    case 1: step = 1; break;
    case 2: step = 3; break;
    case 3: step = 6; break;
    default: step = 1;
  }

  brightness += fadeDir * step;
  if(brightness <= 0 || brightness >= 255){
    fadeDir = -fadeDir;
  }

  analogWrite(ledPin, brightness);
  delay(15);
}

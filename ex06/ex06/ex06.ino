const int ledA = 18;
const int ledB = 19;

int pwmVal = 0;
int step = 2;

void setup() {
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
}

void loop() {
  analogWrite(ledA, pwmVal);
  analogWrite(ledB, 255 - pwmVal);

  pwmVal += step;
  if(pwmVal <= 0 || pwmVal >= 255){
    step = -step;
  }

  delay(20);
}

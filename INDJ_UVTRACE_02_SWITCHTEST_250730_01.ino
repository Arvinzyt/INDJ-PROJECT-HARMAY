const int stepPin = 2;
const int dirPin  = 3;
const int switchPin = 4; 

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);  

  digitalWrite(dirPin, HIGH); 
}

void loop() {
  bool switchState = digitalRead(switchPin);

  if (switchState == HIGH) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  } else {
    
  }
}

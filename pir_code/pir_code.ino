#define PIR_PIN 13   // Pin connected to the output of PIR sensor


void setup() {
  pinMode(PIR_PIN, INPUT);  // PIR sensor pin as input

  Serial.begin(9600);       // Start serial communication
}

void loop() {
  int pirState = digitalRead(PIR_PIN); // Read PIR sensor state
  
  if (pirState == HIGH) { // If motion detected

    Serial.println("Motion detected!");
    delay(1000); // Delay for stability
  } else {
  
  }
}
void setup() {
  Serial.begin(9600);
}

void loop() {
  if ( Serial.available() ) {
    int lu = Serial.read();
    Serial.println(lu);
  } else {
    Serial.println("Rien");
  }
  delay(2000);
}

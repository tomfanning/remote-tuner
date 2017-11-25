void setup() {

  Serial.begin(9600);
  for (int i=2;i<=9;i++){
    pinMode(i, OUTPUT);
  }
}

int sensorValue,scaled;

void loop() {
  delay(50);
  
  sensorValue = analogRead(A3);
  scaled = sensorValue/4;
  
  for (int i=0;i<8;i++){
    digitalWrite(i+2, scaled <= i*32);
  }
}

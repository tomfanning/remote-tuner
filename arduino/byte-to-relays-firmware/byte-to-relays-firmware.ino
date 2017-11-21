
// simple firmware that sets the state of pins 2-9 to 
// whatever byte was received on the serial port last.

void setup() {
  Serial.begin(9600);
 
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

int incomingByte = 0;
int bitv;

void loop() {
 
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    for (int i=0;i<8;i++) {
      bitv = bitRead(incomingByte,i);
      digitalWrite(i+2,bitv);
    }
  } 
}

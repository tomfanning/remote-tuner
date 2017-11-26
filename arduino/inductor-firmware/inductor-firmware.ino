#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {

  Serial.begin(9600);
  
  for (int i=2;i<=9;i++){
    pinMode(i, OUTPUT);
  }

  Serial.println("Send a 0 to 8 and a newline to set that number of bits (D2-9) on.\nUse hardware serial or D10 (TX) / D11 (RX) software serial.");

  mySerial.begin(9600);
}

String buf1 = "";
String buf2 = "";
char c;

void loop() {

  while (true) {  
    // read a line of text from either the hardware serial port or d10/d11 software serial
    if (Serial.available()) {
      c = Serial.read();
      if (c != '\r') {
        if (c == '\n') {
          processCommand(buf1);
          buf1 = "";
          break;
        }
        buf1 += c;
      }
    } else if (mySerial.available()) {
      c = mySerial.read();
      if (c != '\r') {
        if (c == '\n') {
          processCommand(buf2);
          buf2 = "";
          break;
        }
        buf2 += c;
      }
    } 
  } 
}

const int validEntries = 8;

void processCommand(String cmd) {
  
  int setting = cmd.toInt();

  if (setting <= validEntries && setting >= 0){

    Serial.println(setting);
    mySerial.println(setting);

    for (int bitNo=0;bitNo<8;bitNo++){     
      digitalWrite(bitNo+2, setting > bitNo);
    }
  }
}


#include <SoftwareSerial.h>

SoftwareSerial indSerial(2, 3); // RX, TX
SoftwareSerial cap1Serial(4, 5); // RX, TX
SoftwareSerial cap2Serial(6, 7); // RX, TX

String buf = "";
const int maxCapsPossible = 256;
int capsPossible1[maxCapsPossible];
int capsPossible2[maxCapsPossible];

void initialiseVariables() {
  for (int i=0;i<maxCapsPossible; i++) {
    capsPossible1[i]=-1;
    capsPossible2[i]=-1;
  }
}

void setup() {
  //indSerial.begin(9600);
  cap1Serial.begin(9600);
  cap2Serial.begin(9600);
  Serial.begin(9600);

  Serial.println("starting");

  initialiseVariables();
  
  cap1Serial.listen();
  cap1Serial.println("10");
  delay(500);
  cap1Serial.println("0");

  cap1Serial.listen();

  bool go=true;
  String line1,line2;
  while (go) {
    cap1Serial.println("q");
    char arr[255];
    int numChars = getLine(1000, arr);
    if (numChars != 0) {
      // we have a string
      for (int i=0;i<numChars;i++) {
        line1 += arr[i];
        go=false;
      }
    }
    if (!go) {
      numChars = getLine(1000, arr);
      for (int i=0;i<numChars;i++) {
        line2 += arr[i];
      }
    }
  }

  Serial.println(line1);
  Serial.println(line2);

  Serial.println("setup ended");
}

// Read a line from a serial port, within the specified timeout (ms), into the referenced array.
// If a line (terminated in \n) has not been received within the timeout, return 0, else, return the number of chars on the line.
int getLine(int timeout, char arr[]) {
  long started;
  int cur=0;
  while (true) {
    started = millis();
    cap1Serial.println("q");
    while (millis() < started + timeout) {
      if (cap1Serial.available()) {
        int b = cap1Serial.read();
        if (b == '\n') {
          return cur;
        }
        
        arr[cur] = b;
        cur++;
      }
    }
  }
}

int parseInto(String buf, int arr[]) {
  int len = buf.length();
  int cur=0;
  String wrd="";
  for (int i=0;i<len;i++) {
    char c = buf[i];
    if (c != '*' && c != '#'){
      if (c == ' ') {
        if (wrd.length() > 0) {
          arr[cur] = wrd.toInt();
          wrd = "";
          cur++;
        }
      } else {
        wrd += c;
      }
    }
  }

  return cur;
}

int parseCapsInstalled(String line) {
  if (line[0] == '*' && line[1] == '*'){
    Serial.println(line);
    return 1;
  }

  return 0;
}

int getCapsPossible(String line) {
  if (line[0] == '#' && line[1] == '#'){
    Serial.println(line);
    return 1;
  }

  return 0;
}

void loop() {

}

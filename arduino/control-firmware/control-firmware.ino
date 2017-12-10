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

  int capsInstalled1[8];
  int numInstalled1 = test(0, '*', capsInstalled1);
  Serial.print("got caps installed 1: ");
  Serial.println(numInstalled1);

  int numCapsPossible1 = test(0, '#', capsPossible1);
  Serial.print("got caps possible 1: ");
  Serial.println(numCapsPossible1);


  cap2Serial.listen();

  int capsInstalled2[8];
  int numInstalled2 = test(1, '*', capsInstalled2);
  Serial.print("got caps installed 2: ");
  Serial.println(numInstalled2);

  int numCapsPossible2 = test(1, '#', capsPossible2);
  Serial.print("got caps possible 2: ");
  Serial.println(numCapsPossible2);


  Serial.println("setup ended");
}

int test(int port, char waitFor, int arr[]) {

  while (true) {
    if ((port == 0 && cap1Serial.available()) ||(port == 1 && cap2Serial.available()) ) {
      char c; 
      if (port == 0) {
        c = cap1Serial.read();
      } else {
        c = cap2Serial.read();
      }
      
      if (c == '\n') {
        if (buf[0] == waitFor && buf[1] == waitFor) {
          int result = parseInto(buf,arr);
          buf = "";
          return result;
        }
      } else if (c != '\r'){
        buf += c;
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

//int setting;
//int isetting;

void loop() {
  
  //indSerial.println(isetting);
  //cap1Serial.println(setting);
  //cap2Serial.println(setting);

  
  
}

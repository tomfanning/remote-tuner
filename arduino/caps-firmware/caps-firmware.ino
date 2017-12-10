#include <SoftwareSerial.h>


//            d2                       d9
int caps[] = {330,220,100,100,33,27,20,18};

int sensorValue, lastVal, scaled, validEntries;
float scalingFactor;
const int pcaps = 240;
int possibleCaps[pcaps], valueIndex[pcaps];
SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  
  for (int i=2;i<=9;i++){
    pinMode(i, OUTPUT);
  }

  Serial.println("Calculating optimal solutions...");
  calculateSolutions();
  displaySolutions();

  Serial.print("Number of values: ");
  Serial.print(validEntries);
  Serial.print(" (max ");
  Serial.print(pcaps);
  Serial.println(")");
  
  sensorValue = analogRead(A3); // maybe garbage 
}

void calculateSolutions(){
  
  // initialise a pair of arrays to hold solutions - where -1 means "unused slot"
  for (int i=0;i<pcaps;i++) {
    possibleCaps[i] = -1;
    valueIndex[i] = -1;
  }

  // iterate through all possible combinations of 8 bits
  for (int solutionIndex=0;solutionIndex<256;solutionIndex++){

    // calculate the resultant capacitance if the capacitors corresponding to true bits are connected in series
    // 1/Ct = 1/C1 + 1/C2 + 1/Cn
    float sum=0;
    int bitsRequired=0;
    for (int bitNo=0;bitNo<8;bitNo++) {
      
      int bitVal = bitRead(solutionIndex, bitNo);

      if (bitVal){
        int capVal = caps[bitNo];
        float v = 1/(float)capVal;
        sum += v;
        bitsRequired++;
      }
    }
    float inv = 1/sum;

    // "round" the capacitance, since we're not interested in being able to set tiny differences - 1pF at worst
    int solutionCapacitance = roundFloat(inv);
    
    // look in our array of possible capacitances, see if this capacitance has already been made up another way
    int foundAt = lookFor(possibleCaps, pcaps, solutionCapacitance);
    
    if (foundAt == -1){

      int bestSolutionIndex = findBestSolution(solutionCapacitance);
      
      // no it hasn't, record the solution
      insertAtEnd(possibleCaps, pcaps, solutionCapacitance);

      // and record which combination of capacitors needs to be set to reach that solution
      // remember - 1 bit per capacitor - an 8 bit number (0-255 decimal) is enough to record precisely what combination is set
      insertAtEnd(valueIndex, pcaps, bestSolutionIndex); // or solutionIndex
    }
  }

  // count how many distinct solutions are possible given this set of caps
  validEntries = countValidEntries(possibleCaps, pcaps);

  doubleBubbleSort(possibleCaps, valueIndex, validEntries);
}

int findBestSolution(int capacitance) {

  int bestBitCount=99;
  int bestIndex;
  
  for (int i=0;i<256;i++){
    float sum=0;
    int bitsRequired=0;
    for (int bitNo=0;bitNo<8;bitNo++) {
      
      int bitVal = bitRead(i, bitNo);
      if (bitVal){
        int capVal = caps[bitNo];
        float v = 1/(float)capVal;
        sum += v;
        bitsRequired++;
      }
    }
    float inv = 1/sum;
    int j = roundFloat(inv);

    if (j == capacitance) {
      if (bitsRequired < bestBitCount) {
        bestIndex = i;
        bestBitCount = bitsRequired;
      }
    }
  }

  return bestIndex;
}

int roundFloat(float in) {
  return (int)(in+0.5);
}

void displaySolutionsSS(){

  // out of the software serial port

  // installed capacitors
  mySerial.print("** ");
  for (int i=0;i<8;i++) {
    mySerial.print(caps[i]);
    mySerial.print(" ");
  }
  mySerial.println();

  // possible capacitances
  mySerial.print("## ");
  for (int i=0;i<validEntries;i++) {
    mySerial.print(possibleCaps[i]);
    mySerial.print(" ");
  }
  mySerial.println();
}

void displaySolutions(){
  Serial.println("***");
  Serial.println("For the capacitors, switched in by relays on D9 to D2, installed in that order:");
  
  Serial.print("** ");
  for (int i=0;i<8;i++) {
    Serial.print(caps[i]);
    Serial.print(" ");
  }
  Serial.println();

  Serial.println("the following capacitances can be made up:");
  
  Serial.print("## ");
  for (int i=0;i<validEntries;i++) {
    Serial.print(possibleCaps[i]);
    Serial.print(" ");
  }
  Serial.println();
 
  Serial.println("by setting:");
    for (int i=0;i<validEntries;i++) {
    Serial.print(valueIndex[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.println("***");
  
}

void doubleBubbleSort(int primary[], int secondary[], int arrLen) {
  for (int i=0;i<(arrLen-1);i++){
    for (int o=0;o<(arrLen-(i+1));o++){
      if (primary[o] > primary[o+1]){
        int t = primary[o];
        primary[o] = primary[o+1];
        primary[o+1] = t;

        int s = secondary[o];
        secondary[o] = secondary[o+1];
        secondary[o+1] = s;
      }
    }
  }
}

int countValidEntries(int arr[], int arrLen){
  int cnt = 0;
  for (int i=0;i<arrLen;i++) {
    if (arr[i] != -1){
      cnt++;
    }
  }
  return cnt;
}

void insertAtEnd(int arr[], int arrLen, int toInsert){
  for (int i=0; i< arrLen; i++){
    if (arr[i] == -1) {
      arr[i] = toInsert;
      return;
    }
  }
}

int lookFor(int arr[], int arrLen, int lookFor){
  for (int i=0;i<arrLen;i++) {
    if (arr[i] == lookFor) {
      return i;
    }
  }
  return -1;
}

String buf1 = "";
String buf2 = "";
char c;

void loop() {

  while (true) {  
    // read a line of text from either the hardware serial port or d10/d11 software serial
    if (mySerial.available()) {
      c = mySerial.read();
      if (c != '\r') {
        if (c == '\n') {
          processCommand(buf1);
          buf1 = "";
          break;
        }
        buf1 += c;
      }
    } else if (Serial.available()) {
      c = Serial.read();
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

void processCommand(String cmd) {

  //Serial.print("processing ");
  //Serial.println(cmd);

  if (cmd == "q") {
    displaySolutionsSS();
    return;
  } 
  
  int setting = cmd.toInt();

  if (setting < validEntries && setting >= 0){

    Serial.println(possibleCaps[setting]);

    for (int bitNo=0;bitNo<8;bitNo++){
      int bitVal = bitRead(valueIndex[setting], bitNo);
      digitalWrite(bitNo+2, bitVal);
    }
  }
}


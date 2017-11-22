
//            d2                       d9
int caps[] = {330,220,100,100,33,27,20,18};

int sensorValue, lastVal, scaled, validEntries;
float scalingFactor;
const int pcaps = 128;
int possibleCaps[pcaps], valueIndex[pcaps]

void setup() {
  Serial.begin(9600);
  for (int i=2;i<=9;i++){
    pinMode(i, OUTPUT);
  }

  calculateSolutions();
  displaySolutions();
  
  sensorValue = analogRead(A3); // maybe garbage
}

void calculateSolutions(){
  
  // initialise a pair of arrays to hold solutions - where -1 means "unused slot"
  for (int i=0;i<pcaps;i++) {
    possibleCaps[i] = -1;
    valueIndex[i] = -1;
  }

  // iterate through all possible combinations of 8 bits
  for (int i=0;i<256;i++){

    // calculate the resultant capacitance if the capacitors corresponding to true bits are connected in series
    // 1/Ct = 1/C1 + 1/C2 + 1/Cn
    float sum=0;
    for (int bitNo=0;bitNo<8;bitNo++) {
      
      int bitVal = bitRead(i, bitNo);

      if (bitVal){
        int capVal = caps[bitNo];
        float v = 1/(float)capVal;
        sum += v;
      }
    }
    float inv = 1/sum;

    // "round" the capacitance, since we're not interested in being able to set tiny differences - 1pF at worst
    int j = (int)inv;

    // look in our array of possible capacitances, see if this capacitance has already been made up another way
    int foundAt = lookFor(possibleCaps, pcaps, j);
    
    if (foundAt == -1){
      // no it hasn't record the solution
      insertAtEnd(possibleCaps, pcaps, j);

      // and record which combination of capacitors needs to be set to reach that solution
      // remember - 1 bit per capacitor - an 8 bit number (0-255 decimal) is enough to record precisely what combination is set
      insertAtEnd(valueIndex, pcaps, i);
    }
  }

  // count how many distinct solutions are possible given this set of caps
  validEntries = countValidEntries(possibleCaps, pcaps);

  doubleBubbleSort(possibleCaps, valueIndex, validEntries);
}

void displaySolutions(){
  Serial.println("***");
  Serial.println("For the capacitors, switched in by relays on D9 to D2, installed in that order:");
  for (int i=0;i<8;i++) {
    Serial.print(caps[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.println("the following capacitances can be made up:");
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

void doubleBubbleSort(int a[], int secondary[], int arrLen) {
  for (int i=0;i<(arrLen-1);i++){
    for (int o=0;o<(arrLen-(i+1));o++){
      if (a[o] > a[o+1]){
        int t = a[o];
        a[o] = a[o+1];
        a[o+1] = t;

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

void loop() {

  delay(10);
  
  sensorValue = analogRead(A3);

  scalingFactor = validEntries/1024.0;

  scaled = sensorValue * scalingFactor;
  
  if (lastVal != scaled) {

    Serial.println(possibleCaps[scaled]);

    for (int bitNo=0;bitNo<8;bitNo++){
      int bitVal = bitRead(valueIndex[scaled], bitNo);
      digitalWrite(bitNo+2, bitVal);
    }

    lastVal = scaled;
  }
}

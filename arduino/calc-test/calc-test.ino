int caps[] = {18,20,27,33,100,100,220,330};

//int possibleCapacitances[] = {5,   6, 7, 8,  9, 10, 11, 12, 13, 14, 15,  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 29, 30, 33, 36, 41, 43, 50, 57, 69, 77, 100, 132, 220, 330, 9999};
//int obtainedBySetting[] =    {31, 15, 7, 13, 3, 21,  5,  9, 28, 50, 12, 116, 18,  1, 84, 2, 20, 88, 152, 68, 24, 200, 4, 72, 136, 8, 240, 112, 176, 48, 208, 80, 144, 16, 192, 64, 128, 0};

const int pcaps = 128;
int possibleCaps[pcaps];
int valueIndex[pcaps];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  calculateSolutions();
  displaySolutions();
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
  int validEntries = countValidEntries(possibleCaps, pcaps);

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
  // put your main code here, to run repeatedly:

}

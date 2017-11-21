int caps[] = {18,20,27,33,100,100,220,330};

//int possibleCapacitances[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 29, 30, 33, 36, 41, 43, 50, 57, 69, 77, 100, 132, 220, 330, 9999};
//int obtainedBySetting[] = {31, 15, 7, 13, 3, 21, 5, 9, 28, 50, 12, 116, 18, 1, 84, 2, 20, 88, 152, 68, 24, 200, 4, 72, 136, 8, 240, 112, 176, 48, 208, 80, 144, 16, 192, 64, 128, 0};

const int pcaps = 128;
int possibleCaps[pcaps];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for (int i=0;i<possibleCaps;i++) {
    possibleCaps[i] = -1;
  }
  
  for (int i=0;i<256;i++){
    
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

    int j = (int)inv;

    if (indexOf(possibleCaps, pcaps, j) == -1){
      insertAtEnd(possibleCaps, pcaps, j);
    }
    
    Serial.print(j);
    Serial.print(" ");
  }
}

void insertAtEnd(int[] arr, int arrLen, int toInsert){
  
}

int indexOf(int[] arr, int arrLen, int lookFor){
  for (int i=0;i<arrLen;i++) {
    if (arr[i] == lookFor) {
      return true;
    }
  }
  return false;
}

void loop() {
  // put your main code here, to run repeatedly:

}

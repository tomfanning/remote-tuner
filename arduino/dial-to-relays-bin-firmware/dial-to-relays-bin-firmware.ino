
int caps[] = {18,20,27,33,100,100,220,330};

//int possibleCapacitances[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 29, 30, 33, 36, 41, 43, 50, 57, 69, 77, 100, 132, 220, 330, 9999};
//int obtainedBySetting[] = {31, 15, 7, 13, 3, 21, 5, 9, 28, 50, 12, 116, 18, 1, 84, 2, 20, 88, 152, 68, 24, 200, 4, 72, 136, 8, 240, 112, 176, 48, 208, 80, 144, 16, 192, 64, 128, 0};

float sum;
int b,i,j;
int foundAt;
float inv;

void setup() {
  Serial.begin(9600);
  for (i=2;i<=9;i++){
    pinMode(i, OUTPUT);
  }
}

int sensorValue,scaled;
int lastVal=-1;
int capVal=-1;


void loop() {
  delay(33);
  
  sensorValue = analogRead(A3);
  scaled = sensorValue/27;

  if (lastVal != scaled) {

    //Serial.print
    //Serial.print(scaled);
    //Serial.print(" ");

    float sum;
    for (int i=0;i<8;i++){
      int b = bitRead(obtainedBySetting[scaled],i);
      digitalWrite(i+2, b);
      if (b){
        sum += 1/(float)caps[i];
      }
    }
    float inv = 1/sum;
    Serial.println(inv);

    lastVal = scaled;    
  }
}

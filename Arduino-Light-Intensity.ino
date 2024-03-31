// C++ code
//
int solarInputRaw = 0;
int prevSolarInput = 0;
int solarInput = 0;
int photodiodeInputRaw = 0;
int prevPhotodiodeInput = 0;
int photodiodeInput = 0;
int prevPushButtonInput;
int pushButtonInput;

byte segValue[10][7] = {
   {1,1,1,1,1,1,0}, //0
   {0,1,1,0,0,0,0}, //1
   {1,1,0,1,1,0,1}, //2
   {1,1,1,1,0,0,1}, //3
   {0,1,1,0,0,1,1}, //4
   {1,0,1,1,0,1,1}, //5
   {1,0,1,1,1,1,1}, //6
   {1,1,1,0,0,0,0}, //7
   {1,1,1,1,1,1,1}, //8
   {1,1,1,1,0,1,1}  //9  
};

byte segPins[]={3,4,5,6,7,8,9};
byte digitPins[] = {A5,A4,A3,A2,11};
int commonOn = 0;
int commonOff = 1;
long displayNum=0;
bool primarySensorSelected;

void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(13, OUTPUT);
  pinMode(12, INPUT);
  
  for(int i=0;i<8;i++){
    pinMode(segPins[i], OUTPUT);
  }
  
  for(int j=0;j<5;j++){
    pinMode(digitPins[j], OUTPUT);
    digitalWrite(digitPins[j], commonOff);
  }
  
  for (int number = 9; number >= 0; number--) {
    showNumber(number);
    digitalWrite(A5, commonOn);
    digitalWrite(A4, commonOn);
    digitalWrite(A3, commonOn);
    digitalWrite(A2, commonOn);
    digitalWrite(11, commonOn);
    delay(500);
  }
  
  primarySensorSelected = true;
  Serial.println("Primary Sensor (Solar Panel) Selected");
}

void loop()
{  
  pushButtonInput = digitalRead(12);
  solarInputRaw = analogRead(A0);
  solarInput = (solarInputRaw * (32000.01 / 1023));
  photodiodeInputRaw = analogRead(A1);
  photodiodeInput = (photodiodeInputRaw * (32000.01 / 85));
  
  if(primarySensorSelected){
    displayNum = solarInput;
  } else {
    displayNum = photodiodeInput;
  }
  
  displayFiveDigits(displayNum);
  
  if(pushButtonInput == 0){
    if(pushButtonInput != prevPushButtonInput) {
      Serial.println("Primary Sensor (Solar Panel) Selected");
      primarySensorSelected = true;
    }
    
    if(solarInput != prevSolarInput) {
      Serial.print("Solar Input : ");
      Serial.println(solarInputRaw);
      Serial.print("Solar Light Intensity : ");
      Serial.println(solarInput);
    }
    
    if(solarInput < 5000) {
      digitalWrite(13, HIGH);
    } else {
      digitalWrite(13, LOW);
    }
  } else if(pushButtonInput == 1) {
    if(pushButtonInput != prevPushButtonInput) {
      Serial.println("Backup Sensor (Photodiode) Selected");
      primarySensorSelected = false;
    }
    
    if(photodiodeInput != prevPhotodiodeInput && photodiodeInput != -1) {
      Serial.print("Photodiode Input : ");
      Serial.println(photodiodeInputRaw);
      Serial.print("Photodiode Light Intensity : ");
      Serial.println(photodiodeInput);
    }
    
    if(photodiodeInput < 5000) {
      digitalWrite(13, HIGH);
    } else {
      digitalWrite(13, LOW);
    }
  } else {
      Serial.print("Unkown Button Input : ");
      Serial.println(pushButtonInput);
  }
  prevPushButtonInput = pushButtonInput;
  prevSolarInput = solarInput;
  prevPhotodiodeInput = photodiodeInput;
  delay(10); // Delay a little bit to improve simulation performance
}

void displayFiveDigits(int displayNum){
  showNumber((displayNum/10000)%10);
  digitalWrite(A5, commonOn);
  delay(4);
  digitalWrite(A5, commonOff);

  showNumber((displayNum/1000)%10);
  digitalWrite(A4, commonOn);
  delay(4);
  digitalWrite(A4, commonOff);

  showNumber((displayNum/100)%10);
  digitalWrite(A3, commonOn);
  delay(4);
  digitalWrite(A3, commonOff);

  showNumber((displayNum/10)%10);
  digitalWrite(A2, commonOn);
  delay(4);
  digitalWrite(A2, commonOff);
  
  showNumber(displayNum%10);
  digitalWrite(11, commonOn);
  delay(4);
  digitalWrite(11, commonOff);
}

void showNumber(int number){ 
  for(int i=0;i<7;i++){
    digitalWrite(segPins[i], segValue[number][i]);
  }
}


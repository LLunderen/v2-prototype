/*
  ITPDP F2018 Gruppe 09
  Jesper Glarman, Christoffer Ashorn, Christian Lund
  Copyright af 
*/
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <RDM6300.h>

//Setting up each of the readers
SoftwareSerial RFID1(10, 20);
SoftwareSerial RFID2(11, 21);
SoftwareSerial RFID3(12, 22); 
SoftwareSerial RFID4(13, 23);
SoftwareSerial RFID5(50, 24);
SoftwareSerial RFID6(51, 25);
SoftwareSerial RFID7(62, 26);
SoftwareSerial RFID8(63, 27);
SoftwareSerial RFID9(64, 28);

//Declaring photoresistor pins
int photoRes1 = 30;
int photoRes2 = 31;
int photoRes3 = 32;
int photoRes4 = 33;
int photoRes5 = 34;
int photoRes6 = 35;
int photoRes7 = 36;
int photoRes8 = 37;
int photoRes9 = 38;
int photoResistorPins[] {photoRes1, photoRes2, photoRes3, photoRes4, photoRes5, photoRes6, photoRes7, photoRes8, photoRes9};

//Declaring power pins for the readers, so only one reader is on at a time
int powerPin1 = 40;
int powerPin2 = 41;
int powerPin3 = 42;
int powerPin4 = 43;
int powerPin5 = 44;
int powerPin6 = 45;
int powerPin7 = 46;
int powerPin8 = 47;
int powerPin9 = 48;
int powerTransistorPins[] = {powerPin1, powerPin2, powerPin3, powerPin4, powerPin5, powerPin6, powerPin7, powerPin8, powerPin9};


//Blocks are made into String instead of HEX
String block1 = "770106113251";
String block2 = "8902958128";
String block3 = "770106213106";
String block4 = "770106111229";
String block5 = "770106156123";
String block6 = "89029113118";
String block7 = "77010614372";
String block8 = "8902936100";
String block9 = "750130229191";
String RFIDtagArray[] = {block1, block2, block3, block4, block5, block6, block7, block8, block9};
String winningArray[] = {block1, block2, block3, block4, block5, block6, block7, block8, block9};

//Declaring variables
int button = 2;
int buttonCounter = 1;
int noOfReaders = 9;
int listeningPort = 0;
int buttonState;
int lastButtonState = 0;
int moves = 0;
int tempRollingValue = 1;


uint8_t Payload1[6]; // used for read comparisons
RDM6300 RDM6300(Payload1);

void gameWon() {// When the game is won a message is shown and the game is soft-locked
  Serial.print("You've won the game!");
  Serial.println("To start a new game, press the reset button!");
  while(true) {
  }
}
void compareArrays() { // Compare the current board, to the winning board
  int winning = 0;
  for(int i = 0; i < noOfReaders; i++) {
    if(RFIDtagArray[i] == winningArray[i]) {
      winning++;
    }
  }
  if(winning == 9) {
    gameWon();
  }
}

String getBlock(String b) { //Returns the block in a readable format
  if(b == block1) {
    return "A1";
  }
  if(b == block2) {
    return "A2";
  }
  if(b == block3) {
    return "A3";
  }
  if(b == block4) {
    return "B1";
  }
  if(b == block5) {
    return "B2";
  }
  if(b == block6) {
    return "B3";
  }
  if(b == block7) {
    return "C1";
  }
  if(b == block8) {
    return "C2";
  }
  if(b == block9) {
    return "C3";
  }
  return "";
}
void printPosition() { // Print the position of every block
  for(int i = 0; i < noOfReaders; i++) {
    Serial.print(getBlock(RFIDtagArray[i]));
    Serial.print(" ");
    if((i + 1) % 3 == 0) {
      Serial.println("");
    }
  }
}
void switchPlace(String tag1, String tag2) { //Switches coordinates of the two pins
  int foundTag1;
  int foundTag2;
  for(int i = 0; i < noOfReaders; i++) {
    if (tag1 == RFIDtagArray[i]) {
      foundTag1 = i;
    }
  }
  for(int i = 0; i < noOfReaders; i++) {
    if (tag2 == RFIDtagArray[i]) {
      foundTag2 = i;
    }
  }
  String temp = RFIDtagArray[foundTag1];
  RFIDtagArray[foundTag1] = RFIDtagArray[foundTag2];
  RFIDtagArray[foundTag2] = temp;
  moves++;
  printPosition();
  compareArrays();
}
void shuffleBlocks() {
  for (int i=0; i < noOfReaders; i++) {
    int n = random(0, noOfReaders); 
    String temp = RFIDtagArray[n];
    RFIDtagArray[n] =  RFIDtagArray[i];
    RFIDtagArray[i] = temp;
  }
}
void turnOffAllPower() { // Makes all readers able to read
  for(int i = 0; i < noOfReaders; i++) {
    digitalWrite(powerTransistorPins[i], LOW);
  }
}
void turnOnAllPower() { // Makes so all readers can't read
  for(int i = 0; i < noOfReaders; i++) {
    digitalWrite(powerTransistorPins[i], HIGH);
  }
}
void turnOnOnePin(int pin) {
  digitalWrite(powerTransistorPins[pin], HIGH);
}
void turnOffOnePin(int pin) {
  digitalWrite(powerTransistorPins[pin], LOW);
}
void setup()
{
  randomSeed(analogRead(A0) + digitalRead(LED_BUILTIN));
  pinMode(button, OUTPUT);
  for (int i = 0; i < noOfReaders; i++) {
    pinMode(powerTransistorPins[i], OUTPUT);
  }
  turnOnAllPower();
  Serial.begin(9600);  // start serial to PC 
//Starting serial with each reader
  RFID1.begin(9600);
  RFID2.begin(9600);
  RFID3.begin(9600);
  RFID4.begin(9600);
  RFID5.begin(9600);
  RFID6.begin(9600);
  RFID7.begin(9600);
  RFID8.begin(9600);
  RFID9.begin(9600);
  shuffleBlocks();
  printPosition();
  turnOffAllPower();
}

void loop()
{
  buttonState = digitalRead(button);

//Changing the listening port, so only one is active at a time
  if(tempRollingValue == 1 && listeningPort != 1) {
    turnOffAllPower();
    turnOnOnePin(0);
    RFID1.listen();
    listeningPort = 1;
    Serial.println("1 is reading");
  }
  if(tempRollingValue == 2 && listeningPort != 2) {
    turnOffAllPower();
    turnOnOnePin(1);
    RFID2.listen();
    listeningPort = 2;
    Serial.println("2 is reading");
  } 
  if(tempRollingValue == 3 && listeningPort != 3) {
    turnOffAllPower();
    turnOnOnePin(2);
    RFID3.listen();
    listeningPort = 3;
    Serial.println("3 is reading");
  }
  if(tempRollingValue == 4 && listeningPort != 4) {
    turnOffAllPower();
    turnOnOnePin(3);
    RFID4.listen();
    listeningPort = 4;
    Serial.println("4 is reading");
  }
  if(tempRollingValue == 5 && listeningPort != 5) {
    turnOffAllPower();
    turnOnOnePin(4);
    RFID5.listen();
    listeningPort = 5;
    Serial.println("5 is reading");
  }
  if(tempRollingValue == 6 && listeningPort != 6) {
    turnOffAllPower();
    turnOnOnePin(5);
    RFID6.listen();
    listeningPort = 6;
    Serial.println("6 is reading");
  }
  if(tempRollingValue == 7 && listeningPort != 7) {
    turnOffAllPower();
    turnOnOnePin(6);
    RFID7.listen();
    listeningPort = 7;
    Serial.println("7 is reading");
  }
  if(tempRollingValue == 8 && listeningPort != 8) {
    turnOffAllPower();
    turnOnOnePin(7);
    RFID8.listen();
    listeningPort = 8;
    Serial.println("8 is reading");
  }
  if(tempRollingValue == 9 && listeningPort != 9) {
    turnOffAllPower();
    turnOnOnePin(8);
    RFID9.listen();
    listeningPort = 9;
    Serial.println("9 is reading");
  }

//Reading the RFID readers 
  if(tempRollingValue == 1 && listeningPort == 1) {
    while (RFID1.available() > 0 && RFID1.isListening()) 
    {
      String reading;
      uint8_t c = RFID1.read();
      if (RDM6300.decode(c)) {
        for(int i=0; i < 5; i++){
          reading += Payload1[i];
        } 
        Serial.print("1 reads: ");
        Serial.print(getBlock(reading));
        Serial.println();
      }  
    }
    tempRollingValue = 2;
  }
  while (RFID2.available() > 0 && RFID2.isListening()) 
  {
    String reading;
    uint8_t c = RFID2.read();
    if (RDM6300.decode(c)) {
      for (int i=0; i < 5; i++){
        reading += Payload1[i];
      } 
      Serial.print("2 reads: ");
      Serial.print(getBlock(reading));
      Serial.println();
    }  
    tempRollingValue = 3;
  }
  while (RFID3.available() > 0 && RFID3.isListening()) 
  {
    String reading;
    uint8_t c = RFID3.read();
    if (RDM6300.decode(c)) {
      for (int i=0; i < 5; i++) {
        reading += Payload1[i];
      } 
      Serial.print("3 reads: ");
      Serial.print(getBlock(reading));
      Serial.println();
    }  
    tempRollingValue = 4;
  }
  while (RFID4.available() > 0 && RFID4.isListening()) 
  {
    String reading;
    uint8_t c = RFID4.read(); 
    if (RDM6300.decode(c)) {
      for (int i=0; i < 5; i++){
        reading += Payload1[i];
      } 
      Serial.print("4 reads: ");
      Serial.print(getBlock(reading));
      Serial.println();
    }  
    tempRollingValue = 5;
  }
  while (RFID5.available() > 0 && RFID5.isListening()) 
  {
    String reading;
    uint8_t c = RFID5.read();
    if (RDM6300.decode(c)) {
      for (int i=0; i < 5; i++){
        reading += Payload1[i];
      } 
      Serial.print("5 reads: ");
      Serial.print(getBlock(reading));
      Serial.println();
    }  
    tempRollingValue = 6;
  }
  while (RFID6.available() > 0 && RFID6.isListening()) 
  {
    String reading;
    uint8_t c = RFID6.read(); //Makes sure the tag is only read once
    if (RDM6300.decode(c)) {
      for (int i=0; i < 5; i++){
        reading += Payload1[i];
      } 
      Serial.print("6 reads: ");
      Serial.print(getBlock(reading));
      Serial.println();
    }  
    tempRollingValue = 7;
  }
  while (RFID7.available() > 0 && RFID7.isListening()) 
  {
    String reading;
    uint8_t c = RFID7.read(); //Makes sure the tag is only read once
    if (RDM6300.decode(c)) {
      for (int i=0; i < 5; i++){
        reading += Payload1[i];
      } 
      Serial.print("7 reads: ");
      Serial.print(getBlock(reading));
      Serial.println();
    } 
    tempRollingValue = 8;
  }
  while (RFID8.available() > 0 && RFID8.isListening()) 
  {
    String reading;
    uint8_t c = RFID8.read(); //Makes sure the tag is only read once
    if (RDM6300.decode(c)) {
      for (int i=0; i < 5; i++){
        reading += Payload1[i];
      } 
      Serial.print("8 reads: ");
      Serial.print(getBlock(reading));
      Serial.println();
    }  
    tempRollingValue = 9;
  }
  while (RFID9.available() > 0 && RFID9.isListening()) 
  {
    String reading;
    uint8_t c = RFID9.read(); //Makes sure the tag is only read once
    if (RDM6300.decode(c)) {
      for (int i=0; i < 5; i++){
        reading += Payload1[i];
      } 
      Serial.print("9 reads: ");
      Serial.print(getBlock(reading));
      Serial.println();
    }  
    tempRollingValue = 1;
  }
}
/* Old RFID reading method
while (RFID5.available() > 0 && RFID5.isListening()) 
  {
    uint8_t c = RFID5.read();
    //Serial.print(c);
    if (RDM6300.decode(c)) {
      Serial.print("5 reads: ");
      for (int i=0; i < 5; i++){
        Serial.print(Payload1[i], HEX);
        Serial.print(" ");
      } 
      Serial.println();
    }  
  }
*/
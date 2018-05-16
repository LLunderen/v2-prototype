/*
  ITPDP Forår 2018 Gruppe 09
  Jesper Glarman, Christoffer Ashorn, Christian Lund
  Copyright as-fuck.inc
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
String RFIDtagArray[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I"};
String winningArray[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I"};
String currentBoard[] = {"-", "-", "-", "-", "-", "-", "-", "-", "-"};
String oldBoard[9];

//Declaring variables
int noOfReaders = 9;
int listeningPort = 0;
int posCounter = 1;
int tries = 0;


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
    if(currentBoard[i] == winningArray[i]) {
      winning++;
    }
  }
  if(winning == 9) {
    gameWon();
  }
}

String getBlock(String b) { //Returns the shuffeled blocks 
  if(b == block1) {
    return RFIDtagArray[0];
  }
  if(b == block2) {
    return RFIDtagArray[1];
  }
  if(b == block3) {
    return RFIDtagArray[2];
  }
  if(b == block4) {
    return RFIDtagArray[3];
  }
  if(b == block5) {
    return RFIDtagArray[4];
  }
  if(b == block6) {
    return RFIDtagArray[5];
  }
  if(b == block7) {
    return RFIDtagArray[6];
  }
  if(b == block8) {
    return RFIDtagArray[7];
  }
  if(b == block9) {
    return RFIDtagArray[8];
  }
  return "";
}
void printPosition() { // Print the position of every block on the board
  for(int i = 0; i < noOfReaders; i++) {
    Serial.print(currentBoard[i]);
    Serial.print(" ");
  }
  Serial.println("");
  compareArrays();
}
void shuffleBlocks() { //Randomizes the ID of each block
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
void turnOnAllPower() { // Makes so all readers can read
  for(int i = 0; i < noOfReaders; i++) {
    digitalWrite(powerTransistorPins[i], HIGH);
  }
}
void turnOnOnePin(int position) { // Turns on one specific pin
  int pin = position - 1;
  digitalWrite(powerTransistorPins[pin], HIGH);
}
void turnOffOnePin(int pin) { // Turns off a spceific pin
  digitalWrite(powerTransistorPins[pin], LOW);
}
bool onArray(String foundBlock) {
  for(int i = 0; i < noOfReaders; i++) {
    if(foundBlock == winningArray[i]) {
      return true;
    }
  }
  return false;
}
void listeningFunction(int p) { //Used to listen on the right port
  switch(p) {
    case 1:
      RFID1.listen();
      break;
    case 2:
      RFID2.listen();
      break;
    case 3:
      RFID3.listen();
      break;
    case 4:
      RFID4.listen();
      break;
    case 5:
      RFID5.listen();
      break;
    case 6:
      RFID6.listen();
      break;
    case 7:
      RFID7.listen();
      break;
    case 8:
      RFID8.listen();
      break;
    case 9:
      RFID9.listen();
      break;
  }
}

void prepareReader(int p) { //Used to set which port should listen
  if(p != listeningPort) {
    turnOffAllPower();
    turnOnOnePin(p);
    listeningFunction(p);
    tries = 0;
  }
  listeningPort = p;
}
void setup() { //Using magic to make sure it doesnøt blow up
  randomSeed(analogRead(A0) + digitalRead(LED_BUILTIN));
  for (int i = 0; i < noOfReaders; i++) {
    pinMode(powerTransistorPins[i], OUTPUT);
  }
  turnOnAllPower();
  Serial.begin(9600);
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
  turnOffAllPower();
}
void loop() { //Using another spell to make it work
  prepareReader(posCounter); //Makes the right reader is ready

  if(posCounter == 1 && listeningPort == 1) {
    int arrayPos = posCounter - 1;
    if(tries > 5) {
      tries = 0;
      posCounter = 2;
      currentBoard[arrayPos] = "-";
    }
    while(RFID1.available() > 0 && RFID1.isListening()) {
      tries = 0;
      String reading;
      uint8_t c = RFID1.read();
      posCounter = 2;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(onArray(foundBlock)) {
          currentBoard[arrayPos] = foundBlock;
        }
      } 
    }
  }

  if(posCounter == 2 && listeningPort == 2) {
    int arrayPos = posCounter - 1;
    if(tries > 5) {
      tries = 0;
      posCounter = 3;
      currentBoard[arrayPos] = "-";
    }
    while(RFID2.available() > 0 && RFID2.isListening()) {
      tries = 0;
      String reading;
      uint8_t c = RFID2.read();
      posCounter = 3;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
if(onArray(foundBlock)) {           currentBoard[arrayPos] = foundBlock;         }
      } 
    }
  }
  if(posCounter == 3 && listeningPort == 3) {
    int arrayPos = posCounter - 1;
    if(tries > 5) {
      tries = 0;
      posCounter = 4;
      currentBoard[arrayPos] = "-";
    }
    while(RFID3.available() > 0 && RFID3.isListening()) {
      tries = 0;
      String reading;
      uint8_t c = RFID3.read();
      posCounter = 4;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(onArray(foundBlock)) {           
          currentBoard[arrayPos] = foundBlock;         
        }
      } 
    }
  }
  if(posCounter == 4 && listeningPort == 4) {
    int arrayPos = posCounter - 1;
    if(tries > 5) {
      tries = 0;
      posCounter = 5;
      currentBoard[arrayPos] = "-";
    }
    while(RFID4.available() > 0 && RFID4.isListening()) {
      tries = 0;
      String reading;
      uint8_t c = RFID4.read();
      posCounter = 5;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(onArray(foundBlock)) {           
          currentBoard[arrayPos] = foundBlock;         
        }
      } 
    }
  }
  if(posCounter == 5 && listeningPort == 5) {
    int arrayPos = posCounter - 1;
    if(tries > 5) {
      tries = 0;
      posCounter = 6;
      currentBoard[arrayPos] = "-";
    }
    while(RFID5.available() > 0 && RFID5.isListening()) {
      tries = 0;
      String reading;
      uint8_t c = RFID5.read();
      posCounter = 6;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(onArray(foundBlock)) {           
          currentBoard[arrayPos] = foundBlock;         
        }
      } 
    }
  }
  if(posCounter == 6 && listeningPort == 6) {
    int arrayPos = posCounter - 1;
    if(tries > 5) {
      tries = 0;
      posCounter = 7;
      currentBoard[arrayPos] = "-";
    }
    while(RFID6.available() > 0 && RFID6.isListening()) {
      tries = 0;
      String reading;
      uint8_t c = RFID6.read();
      posCounter = 7;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(onArray(foundBlock)) {           
          currentBoard[arrayPos] = foundBlock;         
        }
      } 
    }
  }
  
  if(posCounter == 7 && listeningPort == 7) {
    int arrayPos = posCounter - 1;
    if(tries > 5) {
      tries = 0;
      posCounter = 8;
      currentBoard[arrayPos] = "-";
    }
    while(RFID7.available() > 0 && RFID7.isListening()) {
      tries = 0;
      String reading;
      uint8_t c = RFID7.read();
      posCounter = 8;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(onArray(foundBlock)) {           
          currentBoard[arrayPos] = foundBlock;         
        }
      }
    }
  }
  
  if(posCounter == 8 && listeningPort == 8) {
    int arrayPos = posCounter - 1;
    if(tries > 5) {
      tries = 0;
      posCounter = 9;
      currentBoard[arrayPos] = "-";
    }
    while(RFID8.available() > 0 && RFID8.isListening()) {
      tries = 0;
      String reading;
      uint8_t c = RFID8.read();
      posCounter = 9;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(onArray(foundBlock)) {           
          currentBoard[arrayPos] = foundBlock;         
        }
      } 
    }
  }
  if(posCounter == 9 && listeningPort == 9) {
    int arrayPos = posCounter - 1;
    if(tries > 5) {
      tries = 0;
      posCounter = 10;
      currentBoard[arrayPos] = "-";
    }
    while(RFID9.available() > 0 && RFID9.isListening()) {
      tries = 0;
      String reading;
      uint8_t c = RFID9.read();
      posCounter = 10;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(onArray(foundBlock)) {           
          currentBoard[arrayPos] = foundBlock;         
        }
      } 
    }
  }
  tries++;
  if(posCounter  >= 10) {
    posCounter = 1;
    printPosition();
    for(int i = 0; i < noOfReaders; i++) {
      oldBoard[i] = currentBoard[i];
    }
  }
  delay(15);
}

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
String RFIDtagArray[] = {block1, block2, block3, block4, block5, block6, block7, block8, block9};
String winningArray[] = {block1, block2, block3, block4, block5, block6, block7, block8, block9};
String currentBoard[] = {"-", "-", "-", "-", "-", "-", "-", "-", "-"};

//Declaring variables
int noOfReaders = 9;
int listeningPort = 0;
int posCounter = 1;


uint8_t Payload1[6]; // used for read comparisons
RDM6300 RDM6300(Payload1);

void gameWon() {// When the game is won a message is shown and the game is soft-locked
  Serial.print("You've won the game!");
  Serial.println("To start a new game, press the reset button!");
  while(true) {
  }
}
void resetPayload() {
  for(int i = 0; i < 6; i++) {
    Payload1[i] = 0;
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
    Serial.print(currentBoard[i]);
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

void prepareReader(int p) {
  if(p != listeningPort) {
    turnOffAllPower();
    turnOnOnePin(p);
    listeningFunction(p);
  }
  listeningPort = p;
}
void setup() {
  randomSeed(analogRead(A0) + digitalRead(LED_BUILTIN));
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
void loop() {
  prepareReader(posCounter);  

  if(posCounter == 1 && listeningPort == 1) {
    int arrayPos = posCounter - 1;
    while(RFID1.available() > 0 && RFID1.isListening()) {
      String reading;
      uint8_t c = RFID1.read();
      posCounter = 2;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(foundBlock.length() > 1) {
          currentBoard[arrayPos] = foundBlock;
        } else {
          currentBoard[arrayPos] = "Blank";
        }
      } 
    }
  }

  if(posCounter == 2 && listeningPort == 2) {
    int arrayPos = posCounter - 1;
    while(RFID2.available() > 0 && RFID2.isListening()) {
      String reading;
      uint8_t c = RFID2.read();
      posCounter = 3;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(foundBlock.length() > 1) {
          currentBoard[arrayPos] = foundBlock;
        } else {
          currentBoard[arrayPos] = "Blank";
        }
      } 
    }
  }
  if(posCounter == 3 && listeningPort == 3) {
    int arrayPos = posCounter - 1;
    while(RFID3.available() > 0 && RFID3.isListening()) {
      String reading;
      uint8_t c = RFID3.read();
      posCounter = 4;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(foundBlock.length() > 1) {
          currentBoard[arrayPos] = foundBlock;
        } else {
          currentBoard[arrayPos] = "Blank";
        }
      } 
    }
  }
  if(posCounter == 4 && listeningPort == 4) {
    int arrayPos = posCounter - 1;
    while(RFID4.available() > 0 && RFID4.isListening()) {
      String reading;
      uint8_t c = RFID4.read();
      posCounter = 5;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(foundBlock.length() > 1) {
          currentBoard[arrayPos] = foundBlock;
        } else {
          currentBoard[arrayPos] = "Blank";
        }
      } 
    }
  }
  if(posCounter == 5 && listeningPort == 5) {
    int arrayPos = posCounter - 1;
    while(RFID5.available() > 0 && RFID5.isListening()) {
      String reading;
      uint8_t c = RFID5.read();
      posCounter = 6;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(foundBlock.length() > 1) {
          currentBoard[arrayPos] = foundBlock;
        } else {
          currentBoard[arrayPos] = "Blank";
        }
      } 
    }
  }
  if(posCounter == 6 && listeningPort == 6) {
    int arrayPos = posCounter - 1;
    while(RFID6.available() > 0 && RFID6.isListening()) {
      String reading;
      uint8_t c = RFID6.read();
      posCounter = 7;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(foundBlock.length() > 1) {
          currentBoard[arrayPos] = foundBlock;
        } else {
          currentBoard[arrayPos] = "Blank";
        }
      } 
    }
  }
  
  if(posCounter == 7 && listeningPort == 7) {
    int arrayPos = posCounter - 1;
    while(RFID7.available() > 0 && RFID7.isListening()) {
      String reading;
      uint8_t c = RFID7.read();
      posCounter = 8;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(foundBlock.length() > 1) {
          currentBoard[arrayPos] = foundBlock;
        } else {
          currentBoard[arrayPos] = "Blank";
        }
      } 
    }
  }
  
  if(posCounter == 8 && listeningPort == 8) {
    int arrayPos = posCounter - 1;
    while(RFID8.available() > 0 && RFID8.isListening()) {
      String reading;
      uint8_t c = RFID8.read();
      posCounter = 9;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(foundBlock.length() > 1) {
          currentBoard[arrayPos] = foundBlock;
        } else {
          currentBoard[arrayPos] = "Blank";
        }
      } 
    }
  }
  if(posCounter == 9 && listeningPort == 9) {
    int arrayPos = posCounter - 1;
    while(RFID9.available() > 0 && RFID9.isListening()) {
      String reading;
      uint8_t c = RFID9.read();
      posCounter = 10;
      if(RDM6300.decode(c)) {
        for(int i=0; i < 5; i++) {
          reading += Payload1[i];
        } 
        String foundBlock = getBlock(reading);
        if(foundBlock.length() > 1) {
          currentBoard[arrayPos] = foundBlock;
        } else {
          currentBoard[arrayPos] = "Blank";
        }
      } 
    }
  }
  if(posCounter  >= 10) {
    posCounter = 1;
    printPosition();
    Serial.println(" ");
  }
  Serial.print(posCounter);
  delay(10);
}

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

void setup()
{
  randomSeed(analogRead(A0) + digitalRead(LED_BUILTIN));
  pinMode(button, OUTPUT);
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
}

void loop()
{
  buttonState = digitalRead(button);

//Chaning the listening port, so only one is active at a time
  if(buttonCounter == 1 && listeningPort != 1) {
    RFID1.listen();
    listeningPort = 1;
    Serial.println("1 is reading");
  }
  if(buttonCounter == 2 && listeningPort != 2){
    RFID2.listen();
    listeningPort = 2;
    Serial.println("2 is reading");
  } 
  if(buttonCounter == 3 && listeningPort != 3) {
    RFID3.listen();
    listeningPort = 3;
    Serial.println("3 is reading");
  }
  if(buttonCounter == 4 && listeningPort != 4) {
    RFID4.listen();
    listeningPort = 4;
    Serial.println("4 is reading");
  }
  if(buttonCounter == 5 && listeningPort != 5) {
    RFID5.listen();
    listeningPort = 5;
    Serial.println("5 is reading");
  }
  if(buttonCounter == 6 && listeningPort != 6) {
    RFID6.listen();
    listeningPort = 6;
    Serial.println("6 is reading");
  }
  if(buttonCounter == 7 && listeningPort != 7) {
    RFID7.listen();
    listeningPort = 7;
    Serial.println("7 is reading");
  }
  if(buttonCounter == 8 && listeningPort != 8) {
    RFID8.listen();
    listeningPort = 8;
    Serial.println("8 is reading");
  }
  if(buttonCounter == 9 && listeningPort != 9) {
    RFID9.listen();
    listeningPort = 9;
    Serial.println("9 is reading");
  }

//Reading the RFID readers 
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
  }
  while (RFID3.available() > 0 && RFID3.isListening()) 
  {
    String reading;
    uint8_t c = RFID3.read();
    if (RDM6300.decode(c)) {
      for (int i=0; i < 5; i++){
        reading += Payload1[i];
      } 
      Serial.print("3 reads: ");
      Serial.print(getBlock(reading));
      Serial.println();
    }  
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
  }
  

//Handling buttons
  if (buttonState != lastButtonState) {
    if(buttonState == HIGH) {
      buttonCounter++;
    }
    delay(10);
  }
  if(buttonCounter > noOfReaders) {
    buttonCounter = 1;
  }
  lastButtonState = buttonState;
  delay(10);
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
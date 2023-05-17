//Arduino code

#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#define RST_PIN         9         
#define SS_1_PIN        8        
#define SS_2_PIN        10       
#define SS_3_PIN        7
#define SS_4_PIN        4

#define NR_OF_READERS  4

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(5, 6);
 
byte ssPins[] = {SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN};
String room = ""; 
String cardid = ""; 
String nam = "";

MFRC522 mfrc522[NR_OF_READERS];

void setup() {
  //This function sends data to NodeMCU code
  Serial.begin(9600);
  nodemcu.begin(9600);
  delay(500);

  Serial.println("Program started");
  SPI.begin();
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
  mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN);
  }
}

void loop() {

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  boolean sand = data_func(); 
 
  data["room"] = room;
  data["cardid"] = nam; 

  if (sand) {
  data.printTo(nodemcu);
  jsonBuffer.clear();
  sand = false;
  }

  delay(2000);
 }

boolean data_func() {

   boolean sand = false;
   for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {

       
      if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      Serial.print(F("Card UID: "));
      cardid = dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();
      Serial.print("Device Name: ");

              //Naming NFC readers or Devices
              if (cardid == "C3A0A30D") {
                nam = "DEVICE-A";
                Serial.print(nam);
              } else if (cardid == "FC054A31") {
                nam = "DEVICE-B";
                Serial.print(nam);
              } else if (cardid == "33FCB70B") {
                nam = "DEVICE-C";
                Serial.print(nam);
              } else if (cardid == "AA533AB3") {
                nam = "DEVICE-D";
                Serial.print(nam);
              } else if (cardid == "D3CF79A3") {
                nam = "DEVICE-E";
                Serial.print(nam);
              } else if (cardid == "839663A3") {
                nam = "DEVICE-F";
                Serial.print(nam);
              } else if (cardid == "339A79A3") {
                nam = "DEVICE-G";
                Serial.print(nam);
              } else {
                nam = "Invalid-Card";
                Serial.print(nam);
              }
              sand = true;

              // Naming RFID Readers
              Serial.println();
              Serial.print("Room name: ");
                      if (reader == 0) {
                      room = "DMF101";
                      Serial.println(room);  
                      }      
                  if (reader == 1 ) {
                      room = "DMF201";
                      Serial.println(room);
                      }

                  if (reader == 2 ) {
                      room = "DMF301";
                      Serial.println(room);
                      }

                  if (reader == 3 ) {
                      room = "DMF401";
                      Serial.println(room);
                      }

                  
                                   
     Serial.println();

      mfrc522[reader].PICC_HaltA();
      mfrc522[reader].PCD_StopCrypto1();
        } 
      } 
   return sand;   
}

// Converting bytes to string
String dump_byte_array(byte *buffer, byte bufferSize) {
  String cardid = "";
  for (byte i = 0; i < bufferSize; i++) {
    cardid = cardid + ((((buffer[i] & 0xF0) >> 4) <= 9) ? (char)(((buffer[i] & 0xF0) >> 4) + '0') : (char)(((buffer[i] & 0xF0) >> 4) + 'A' - 10));
    cardid = cardid + (((buffer[i] & 0x0F) <= 9) ? (char)((buffer[i] & 0x0F) + '0') : (char)((buffer[i] & 0x0F) + 'A' - 10));
  }
  Serial.print(cardid);    
  return cardid;
}

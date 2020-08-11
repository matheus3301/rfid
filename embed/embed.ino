#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h>

#define SS_PIN 4
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char serverName[] = "7f73d1c020b2.ngrok.io";
int serverPort = 80;

EthernetClient client;

void setup(){

    pinMode(7, OUTPUT);

    tone(7, 440);
    delay(50);
    noTone(7);
    delay(50);
    tone(7, 440);
    delay(50);
    noTone(7);
    delay(50);
    tone(7, 440);
    delay(50);
    noTone(7);

    Serial.begin(9600);
    Serial.println("STARTING");
    SPI.begin();     // Init SPI bus
    rfid.PCD_Init(); // Init MFRC522

    for (byte i = 0; i < 6; i++){
        key.keyByte[i] = 0xFF;
    }

    if (Ethernet.begin(mac) == 0){
        while (true)
            ;
    }
    else{
        Serial.println(Ethernet.localIP());
    }
    delay(500);
}

void loop(){
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if (!rfid.PICC_IsNewCardPresent())
        return;

    // Verify if the NUID has been readed
    if (!rfid.PICC_ReadCardSerial())
        return;

    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++){
        nuidPICC[i] = rfid.uid.uidByte[i];
    }
   

    //SENDING

    Serial.println("connecting...");
    if (client.connect(serverName, serverPort)){
        Serial.println("connected");
        client.print("GET /connection?rfid=");
        
        for (byte i = 0; i < rfid.uid.size; i++){
            client.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : "");
            client.print(rfid.uid.uidByte[i], HEX);
        }
        
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.println(serverName);
        client.println("User-Agent: Arduino/1.0");
        client.println("Connection: close");
        client.println();

        //GETING THE RESULT

        //CLOSING RESULT
        Serial.println("insert complete");

        tone(7, 300);
        delay(50);
        noTone(7);
        delay(50);
        tone(7, 300);
        delay(50);
        noTone(7);
        delay(50);
        tone(7, 300);
        delay(50);
        noTone(7);
        delay(50);
        
    }else{
        tone(7, 440);
        delay(500);
        noTone(7);
        Serial.println("connection failed");
    }

    client.stop();
    //END SENDING

    // Halt PICC
    rfid.PICC_HaltA();

    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();
}

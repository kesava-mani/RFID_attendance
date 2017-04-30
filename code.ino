/*------------------------------------------------
RFID BASED ATTENDANCE SYSTEM WITH SMS CAPABILITIES.
-------------------------------------------------*/

#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

SoftwareSerial mySerial(5, 6); //RX TX of GSM

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

int count = 0, a;

void setup()
{
  pinMode(7, INPUT); //PIR SENSOR for Human detection!

  Serial.begin(9600);   // Initiate a serial communication
  mySerial.begin(9600); //Initate GSM 
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  Serial.println("Welcome to RFID attendance system! ");
  Serial.println("Enter your choice: \n 1. Read \n 2.Admin");
  Serial.println();
}

void loop()
{
  if (Serial.available() > 0)
  {
    a = Serial.parseInt();
    switch (a)
    {
      case 1:
        {
          Serial.println("Read Mode activated!");
          delay(1000);
          if (digitalRead(7))
          {
            Serial.println("Entry detected...");
            delay(3000);
            if ( ! mfrc522.PICC_IsNewCardPresent())
            {
              return;
            }
            // Select one of the cards
            if ( ! mfrc522.PICC_ReadCardSerial())
            {
              return;
            }
            //Show UID on serial monitor
            Serial.print("UID tag :");
            String content = "";
            byte letter;
            for (byte i = 0; i < mfrc522.uid.size; i++)
            {
              Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
              Serial.print(mfrc522.uid.uidByte[i], HEX);
              content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
              content.concat(String(mfrc522.uid.uidByte[i], HEX));
            }
            Serial.println();
            Serial.print("Message : ");
            content.toUpperCase();
            if (content.substring(1) == "15 90 5E 45" || content.substring(1) == "DB 01 C8 00" || content.substring(1) == "BB 55 B4 E5") //change here the UID of the card/cards that you want to give access
            {
              Serial.println("Authorized access!");
              Serial.println();
              count++;
              delay(1000);
              Serial.print("Total count: "); Serial.println(count);
            }

            else   {
              Serial.println(" Access denied!");
              delay(5000);
            }

          }

          break;
        }


      case 2:
        {
	  //Initate SMS mode for GSM
          Serial.println("Admin mode activated.");
          mySerial.print("AT+CMGS=\""); // send the SMS
          mySerial.print("9876543210"); //Replace with your Number
          mySerial.println("\"");
          delay(1000);
          mySerial.print("Total count of the students:");
          mySerial.println(count);
          delay(1000);
          mySerial.write(0x1A);
          mySerial.write(0x0D);
          mySerial.write(0x0A);
          delay(10);
          
          break;
        }

      default:
        {
          Serial.println("Please enter the right choice!");
        }

    }

  }



}

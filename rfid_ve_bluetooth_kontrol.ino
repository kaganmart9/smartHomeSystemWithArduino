#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte istenen_kart[4] = {26, 40, 134, 37};
byte okunan_kart[4];

#define RX 7
#define TX 8

#include <SoftwareSerial.h>
SoftwareSerial bluetooth(RX, TX);

#include <Servo.h>

Servo kapi;
Servo perde;
Servo pencere;

#define led_pin 2
#define kapi_pin 3
#define fan_pin 4
#define perde_pin 5
#define pencere_pin 6

void setup()
{
    Serial.begin(115200);
    bluetooth.begin(9600);
    bluetooth.setTimeout(100);
    Serial.setTimeout(100);
    SPI.begin();
    mfrc522.PCD_Init();

    /*for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
    }*/

    pinMode(led_pin, OUTPUT);
    pinMode(fan_pin, OUTPUT);
    kapi.attach(kapi_pin);
    perde.attach(perde_pin);
    pencere.attach(pencere_pin);

    kapi.write(90);
    perde.write(90);
    pencere.write(90);
    Serial.println("\nProgram basladi");
    Serial.println();
}
void loop()
{
    if (bluetooth.available() || Serial.available())
    {
        String gelen = bluetooth.readString();
        gelen.trim();

        if (gelen == "")
        {
            gelen = Serial.readString();
            gelen.trim();
        }
        Serial.print("Gelen: ");
        Serial.println(gelen);
        if (gelen == "1A")
        {
            digitalWrite(led_pin, HIGH);
            Serial.println("Led yandi");
        }
        else if (gelen == "1K")
        {
            digitalWrite(led_pin, LOW);
            Serial.println("Led sondu");
        }
        else if (gelen == "2A")
        {
            digitalWrite(fan_pin, HIGH);
            Serial.println("Fan dondu");
        }
        else if (gelen == "2K")
        {
            digitalWrite(fan_pin, LOW);
            Serial.println("Fan durdu");
        }
        else if (gelen == "3A")
        {
            perde.write(0);
            Serial.println("Perde acildi");
            delay(300);
            perde.write(90);
            Serial.println("Perde kapandi");
        }
        else if (gelen == "3K")
        {
            perde.write(180);
            Serial.println("Perde terse acildi");
            delay(300);
            perde.write(90);
            Serial.println("Perde kapandi");
        }
        else if (gelen == "4A")
        {
            pencere.write(0);
            Serial.println("Pencere acildi");
            delay(300);
            pencere.write(90);
            Serial.println("Pencere kapandi");
        }
        else if (gelen == "4K")
        {
            pencere.write(180);
            Serial.println("Pencere terse acildi");
            delay(300);
            pencere.write(90);
            Serial.println("Pencere kapandi");
        }
    }

    if (!mfrc522.PICC_IsNewCardPresent())
        return;

    // Verify if the NUID has been readed
    if (!mfrc522.PICC_ReadCardSerial())
        return;

    // Serial.print(F("PICC type: "));
    // MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    // Serial.println(mfrc522.PICC_GetTypeName(piccType));

    for (byte i = 0; i < 4; i++)
    {
        okunan_kart[i] = mfrc522.uid.uidByte[i];
    }

    Serial.print("Okunan kart: ");
    for (byte i = 0; i < 4; i++)
    {
        Serial.print(okunan_kart[i]);
        Serial.print(" ");
    }
    Serial.println();

    if (okunan_kart[0] == istenen_kart[0] &&
        okunan_kart[1] == istenen_kart[1] &&
        okunan_kart[2] == istenen_kart[2] &&
        okunan_kart[3] == istenen_kart[3])
    {
        Serial.println("Kart kabul edildi");
        kapi.write(0);
        delay(350);
        kapi.write(90);
    }
    else
    {
        Serial.println("Kart reddedildi");
    }

    mfrc522.PICC_HaltA();

    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
}

void printDec(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(' ');
        Serial.print(buffer[i], DEC);
    }
}
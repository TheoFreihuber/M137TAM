#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*
int motor1 = D5;
int motor2 = D6;
int motor3 = D7;
int motor4 = ;
*/

//LiquidCrystal_I2C lcd(0x27, 20, 4);

ulong tmpservo = 0;
ulong delais1 = 0;
ulong delais2 = 2000;

int servo_plaque1 = D2;

void Ascenseur(int speed);

void Setangle(int servo, int angle);

bool Magnet(int pin_magnet);

void setup() {
    /*
    Serial.begin(115200);
    Wire.begin(D1, D2);
    lcd.begin(20, 4); // Démarre l'afficheur
    lcd.backlight(); // Allume le rétro-éclairage
    lcd.setCursor(4, 1);
    lcd.print("Hello Fucking"); // Affiche "Hello, World!" sur le premier ligne
    lcd.setCursor(8, 2);
    lcd.print("WORLD");
    */
    //pinMode(D1, OUTPUT);
    //pinMode(D2, OUTPUT);
    pinMode(servo_plaque1, OUTPUT);
    pinMode(A0, INPUT);




}

void loop() {
    // Ascenseur(10);


    if (Magnet(A0) == HIGH) {
        Setangle(servo_plaque1, 135);
    }
    else if (Magnet(A0)== LOW) {
        Setangle(servo_plaque1, 180);
    }



}

void Setangle(int servo, int angle)
{
    ulong now = millis();
    if (now - tmpservo > 20)
    {
        digitalWrite(servo, HIGH);
        delayMicroseconds(map(angle,0,180,500,2500));
        digitalWrite(servo, LOW);
        tmpservo = now;
    }

}

/*
void Ascenseur(int speed)
{
    int attente = map(speed, 0, 100, 200,0);

    digitalWrite(motor1, HIGH);
    digitalWrite(motor2, LOW);
    digitalWrite(motor3, LOW);
    digitalWrite(motor4, LOW);

    delay(attente);

    digitalWrite(motor1, digitalRead(motor4));
    digitalWrite(motor2, digitalRead(motor1));
    digitalWrite(motor3, digitalRead(motor2));
    digitalWrite(motor4, digitalRead(motor3));
}

void Attend(int temps) {

}
*/

bool Magnet(int pin_magnet) {
    return (analogRead(pin_magnet) < 100);
}
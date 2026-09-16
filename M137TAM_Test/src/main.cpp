#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*
int motor1 = D5;
int motor2 = D6;
int motor3 = D7;
int motor4 = ;


LiquidCrystal_I2C lcd(0x27, 20, 4);
*/
ulong tmpservo = 0;
ulong delais1 = 0;
ulong delais2 = 2000;

bool notstarted = true;


int servo_plaque1 = D2;

void Ascenseur(int speed);

void Setangle(int servo, int angle);

bool Magnet(int pin_magnet);

void MoteurPPStart(int pin_moteurPP1,  int pin_moteurPP2,  int pin_moteurPP3,  int pin_moteurPP4);
void MoteurPPClockWise( int pin_moteurPP1,  int pin_moteurPP2,  int pin_moteurPP3,  int pin_moteurPP4,  int vitesse);
void MoteurPPCounterClockWise( int pin_moteurPP1,  int pin_moteurPP2,  int pin_moteurPP3,  int pin_moteurPP4,  int vitesse);


void setup() {
    Serial.begin(115200);
    /*
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
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);





}

void loop() {

    // Ascenseur(10);
    /*

    if (Magnet(A0) == HIGH) {
        Setangle(servo_plaque1, 135);
    }
    else if (Magnet(A0)== LOW) {
        Setangle(servo_plaque1, 180);
    }


    analogWrite(D1, 255);
    digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);

    */
    if (notstarted == true) {
        digitalWrite(D1, HIGH);
        digitalWrite(D2, LOW);
        digitalWrite(D5, LOW);
        digitalWrite(D6, LOW);
        notstarted = false;
    }


    //montee
    bool temp = digitalRead(D6);
    digitalWrite(D6, digitalRead(D5));
    digitalWrite(D5, digitalRead(D2));
    digitalWrite(D2, digitalRead(D1));
    digitalWrite(D1, temp);


    /*
    //descente
    bool temp = digitalRead(D1);
    digitalWrite(D1, digitalRead(D2));
    digitalWrite(D2, digitalRead(D5));
    digitalWrite(D5, digitalRead(D6));
    digitalWrite(D6, temp);
    */


    delay(2);





}

void Setangle(const int servo, const int angle)
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

bool Magnet(const int pin_magnet) {
    return (analogRead(pin_magnet) < 100);
}

void MoteurPPStart(const int pin_moteurPP1, const int pin_moteurPP2, const int pin_moteurPP3, const int pin_moteurPP4) {
    digitalWrite(pin_moteurPP1, HIGH);
    digitalWrite(pin_moteurPP2, LOW);
    digitalWrite(pin_moteurPP3, LOW);
    digitalWrite(pin_moteurPP4, LOW);
}

void MoteurPPClockWise(const int pin_moteurPP1, const int pin_moteurPP2, const int pin_moteurPP3, const int pin_moteurPP4, const int vitesse) {

    int attente = map(vitesse, 0, 100, 200,2);

    static unsigned long tmpMoteurPP = 0;
    ulong now = millis();
    if (now - tmpMoteurPP > attente)
    {
        digitalWrite(pin_moteurPP1, digitalRead(pin_moteurPP4));
        digitalWrite(pin_moteurPP2, digitalRead(pin_moteurPP1));
        digitalWrite(pin_moteurPP3, digitalRead(pin_moteurPP2));
        digitalWrite(pin_moteurPP4, digitalRead(pin_moteurPP3));
        tmpMoteurPP = now;
    }
}

void MoteurPPCounterClockWise(const int pin_moteurPP1, const int pin_moteurPP2, const int pin_moteurPP3, const int pin_moteurPP4, const int vitesse) {

    int attente = map(vitesse, 0, 100, 200,2);

    static unsigned long tmpMoteurPP = 0;
    ulong now = millis();
    if (now - tmpMoteurPP > attente)
    {
        digitalWrite(pin_moteurPP1, digitalRead(pin_moteurPP2));
        digitalWrite(pin_moteurPP2, digitalRead(pin_moteurPP3));
        digitalWrite(pin_moteurPP3, digitalRead(pin_moteurPP4));
        digitalWrite(pin_moteurPP4, digitalRead(pin_moteurPP1));
        tmpMoteurPP = now;
    }
}
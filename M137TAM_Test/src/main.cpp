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
void MoteurPPClockWise( int pin_moteurPP1,  int pin_moteurPP2,  int pin_moteurPP3,  int pin_moteurPP4);
void MoteurPPCounterClockWise( int pin_moteurPP1,  int pin_moteurPP2,  int pin_moteurPP3,  int pin_moteurPP4);
void MoteurPPClockWiseFix(int pin1, int pin2, int pin3, int pin4, int &etape) ;
void MoteurPPCounterClockWiseFix(int pin1, int pin2, int pin3, int pin4, int &etape);
void AppliquerEtape(int p1, int p2, int p3, int p4, int etape) ;
void Setangle(int servo, int angle);

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
    //pinMode(D1, OUTPUT);
    //pinMode(D2, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, INPUT_PULLUP);





}

void loop() {
    Serial.println(analogRead(A0));
    /*
    analogWrite(D5, 155);
    digitalWrite(D6, HIGH);
    digitalWrite(D7, LOW);
    */
    // Ascenseur(10);
    /*

    if (Magnet(A0) == HIGH) {
        Setangle(servo_plaque1, 135);
    }
    else if (Magnet(A0)== LOW) {
        Setangle(servo_plaque1, 180);
    }





    if (notstarted == true) {
        digitalWrite(D1, HIGH);
        digitalWrite(D2, LOW);
        digitalWrite(D5, LOW);
        digitalWrite(D6, LOW);
        notstarted = false;
    }


    //montee
    bool temp = digitalRead(pin_moteurPP4);
    digitalWrite(pin_moteurPP4, digitalRead(pin_moteurPP3));
    digitalWrite(pin_moteurPP3, digitalRead(pin_moteurPP2));
    digitalWrite(pin_moteurPP2, digitalRead(pin_moteurPP1));
    digitalWrite(pin_moteurPP1, temp);



    //descente
    bool temp = digitalRead(pin_moteurPP1);
    digitalWrite(pin_moteurPP1, digitalRead(pin_moteurPP2));
    digitalWrite(pin_moteurPP2, digitalRead(pin_moteurPP3));
    digitalWrite(pin_moteurPP3, digitalRead(pin_moteurPP4));
    digitalWrite(pin_moteurPP4, temp);
    */

    if (digitalRead(D2) == HIGH) {
        Setangle(D1,62);
    }
    else {
        Setangle(D1,110);
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

bool Magnet( int pin_magnet) {
    return (analogRead(pin_magnet) < 100);
}

void MoteurPPStart( int pin_moteurPP1,  int pin_moteurPP2,  int pin_moteurPP3,  int pin_moteurPP4) {
    digitalWrite(pin_moteurPP1, HIGH);
    digitalWrite(pin_moteurPP2, LOW);
    digitalWrite(pin_moteurPP3, LOW);
    digitalWrite(pin_moteurPP4, LOW);
}

void MoteurPPClockWise( int pin_moteurPP1,  int pin_moteurPP2,  int pin_moteurPP3,  int pin_moteurPP4) {
    static unsigned long tmpMoteurPP = 0;
    if (millis() - tmpMoteurPP > 2)
    {
        bool temp = digitalRead(pin_moteurPP4);
        digitalWrite(pin_moteurPP4, digitalRead(pin_moteurPP3));
        digitalWrite(pin_moteurPP3, digitalRead(pin_moteurPP2));
        digitalWrite(pin_moteurPP2, digitalRead(pin_moteurPP1));
        digitalWrite(pin_moteurPP1, temp);
        tmpMoteurPP = millis();
    }
}

void MoteurPPCounterClockWise( int pin_moteurPP1,  int pin_moteurPP2,  int pin_moteurPP3,  int pin_moteurPP4) {
    // static unsigned long tmpMoteurPP = 0;
    // if (millis() - tmpMoteurPP > 20)
    // {
        bool temp = digitalRead(pin_moteurPP1);
        digitalWrite(pin_moteurPP1, digitalRead(pin_moteurPP2));
        digitalWrite(pin_moteurPP2, digitalRead(pin_moteurPP3));
        digitalWrite(pin_moteurPP3, digitalRead(pin_moteurPP4));
        digitalWrite(pin_moteurPP4, temp);
    delay(2);
    //    tmpMoteurPP = millis();
    //}
}

void AppliquerEtape(int p1, int p2, int p3, int p4, int etape) {
    digitalWrite(p1, (etape == 0) ? HIGH : LOW);
    digitalWrite(p2, (etape == 1) ? HIGH : LOW);
    digitalWrite(p3, (etape == 2) ? HIGH : LOW);
    digitalWrite(p4, (etape == 3) ? HIGH : LOW);
}

void MoteurPPClockWiseFix(int pin1, int pin2, int pin3, int pin4, int &etape) {
    static unsigned long tmpMoteurPP = 0;
    if (millis() - tmpMoteurPP > 2) {
        etape++;
        if (etape > 3) etape = 0;

        AppliquerEtape(pin1, pin2, pin3, pin4, etape);
        tmpMoteurPP = millis();
    }
}

void MoteurPPCounterClockWiseFix(int pin1, int pin2, int pin3, int pin4, int &etape) {
    static unsigned long tmpMoteurPP = 0;

    // Attendre 20ms entre chaque pas
    if (millis() - tmpMoteurPP > 2) {
        etape--; // On passe à l'étape précédente
        if (etape < 0) etape = 3; // On boucle de 0 à 3

        AppliquerEtape(pin1, pin2, pin3, pin4, etape);
        tmpMoteurPP = millis();
    }
}

void Setangle(int servo, int angle)
{
    static unsigned long tmpservo = 0;
    ulong now = millis();
    if (now - tmpservo > 20)
    {
        digitalWrite(servo, HIGH);
        delayMicroseconds(map(angle,0,180,500,2500));
        digitalWrite(servo, LOW);
        tmpservo = now;
    }

}

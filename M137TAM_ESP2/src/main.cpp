#include <Arduino.h>
#include <chrono>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string>

bool objet_posee = false;
bool teleportation = false;

ulong debut_teleportation = 0;

void Setangle(int servo, int angle);

//region initialisation des constantes
/// BROCHE ///
constexpr int ecran1 = D1;
constexpr int ecran2 = D2;

constexpr int bouton = D6;

constexpr int servo_entree = D0;

constexpr int capteur_proxi = D7;

constexpr int led_entree = D5;

constexpr int enaMoteur = D8;
constexpr int in1Moteur = D3;
constexpr int in2Moteur = D4;

/// ECRAN ///
constexpr int largeur_ecran = 20;
constexpr int hauteur_ecran = 4;

/// POSITION SERVO ///
constexpr int position_haute_plateforme = 137;
constexpr int position_basse_plateforme = 105;

constexpr int temps_pourcentage = 1000;


//endregion

LiquidCrystal_I2C lcd(0x27, largeur_ecran, hauteur_ecran);  // Initialisation de l'écran

void setup() {
    Serial.begin(115200);
    //region Setup Pin
    pinMode(ecran1, OUTPUT);
    pinMode(ecran2, OUTPUT);

    pinMode(bouton, INPUT);

    pinMode(servo_entree, OUTPUT);

    pinMode(capteur_proxi, INPUT);

    pinMode(led_entree, OUTPUT);

    pinMode(enaMoteur, OUTPUT);
    pinMode(in1Moteur, OUTPUT);
    pinMode(in2Moteur, OUTPUT);
    //endregion

    //region Setup Ecran 20*4
    /// Parametrage de l'écran///
    Wire.begin(ecran1, ecran2);
    lcd.begin(largeur_ecran, hauteur_ecran);
    lcd.backlight();

    /// Affichage du message d'accueil du teleporteur ///
    lcd.setCursor(0, 0);
    lcd.print("   Appuyez sur le   ");
    lcd.setCursor(0, 1);
    lcd.print("    bouton pour     ");
    lcd.setCursor(0, 2);
    lcd.print("    commencer la    ");
    lcd.setCursor(0, 3);
    lcd.print("   teleportation.   ");
    //endregion
}

void loop() {
    Serial.println(digitalRead(bouton));
    if (objet_posee) {
        if (digitalRead(bouton) == HIGH) {
            teleportation = true;
            debut_teleportation = millis();
            lcd.setCursor(0, 0);
            lcd.print("Teleportation lancee");
            lcd.setCursor(0, 1);
            lcd.print("L objet a ete envoye");
            lcd.setCursor(0, 2);
            lcd.print("[------------------]");
            lcd.setCursor(0, 3);
            lcd.print("         5%         ");
        }


        if (teleportation) {
            analogWrite(enaMoteur, 255);
            digitalWrite(in1Moteur, HIGH);
            digitalWrite(in2Moteur, LOW);

            digitalWrite(led_entree, LOW);
            if (millis() < debut_teleportation + 3000) {
                Setangle(servo_entree, position_basse_plateforme);
            }
            else {
                Setangle(servo_entree, position_haute_plateforme);
            }

            uint pourcentage = (millis() - debut_teleportation)/temps_pourcentage;
            if (pourcentage > 0 and pourcentage < 19) {
                lcd.setCursor(pourcentage, 2);
                lcd.print("=");
            }

            if(pourcentage > 0 and pourcentage < 20){
                lcd.setCursor(8-pourcentage/19, 3);
                lcd.print(pourcentage*5+5);
            }

            if(pourcentage == 19){
                lcd.setCursor(0, 0);
                lcd.print("Objet arrive"); // texte a revoir
                lcd.setCursor(0, 1);
                lcd.print("Recuperez votre objet"); // same
            }

        }

        else {
            Setangle(servo_entree, position_haute_plateforme);
            lcd.setCursor(0, 0);
            lcd.print("Object detecte, vous");
            lcd.setCursor(0, 1);
            lcd.print("pouvez effectuer une");
            lcd.setCursor(0, 2);
            lcd.print("   teleportation    ");
            lcd.setCursor(0, 3);
            lcd.print(" PRESSEZ LE BOUTON  ");

            digitalWrite(led_entree, HIGH);
        }
    }

    else {
        Setangle(servo_entree, position_haute_plateforme);
        if (digitalRead(bouton) == HIGH) {
            lcd.setCursor(0, 0);
            lcd.print("Veuillez poser un ");
            lcd.setCursor(0, 1);
            lcd.print("object sur le socle");
            lcd.setCursor(0, 2);
            lcd.print("avant d effectuer ");
            lcd.setCursor(0, 3);
            lcd.print("une teleportation. ");
        }

        if (digitalRead(capteur_proxi) == LOW) {
            objet_posee = true;
        }
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


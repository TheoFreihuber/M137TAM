#include <Arduino.h>
#include <chrono>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

bool objet_posee = false;
bool teleportation = false;

ulong debut_teleportation = 0;

void Setangle(int servo, int angle);

//region initialisation des constantes
/// BROCHE ///
constexpr int ecran1 = D1;
constexpr int ecran2 = D2;

constexpr int bouton = D3;

constexpr int servo_entree = D6;

constexpr int capteur_proxi = D7;

constexpr int led_entree = D5;

/// ECRAN ///
constexpr int largeur_ecran = 20;
constexpr int hauteur_ecran = 4;

/// POSITION SERVO ///
constexpr int position_haute_plateforme = 140;
constexpr int position_basse_plateforme = 90;



//endregion

LiquidCrystal_I2C lcd(0x27, largeur_ecran, hauteur_ecran);  // Initialisation de l'écran

void setup() {
    //region Setup Pin
    pinMode(ecran1, OUTPUT);
    pinMode(ecran2, OUTPUT);

    pinMode(bouton, INPUT_PULLUP);

    pinMode(servo_entree, OUTPUT);

    pinMode(capteur_proxi, INPUT);

    pinMode(led_entree, OUTPUT);
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
    if (objet_posee) {
        if (digitalRead(bouton) == LOW) {
            teleportation = true;
            debut_teleportation = millis();

        }
        if (teleportation) {
            lcd.setCursor(0, 0);
            lcd.print("Teleportation lancee");
            lcd.setCursor(0, 1);
            lcd.print("L objet a ete envoye");
            lcd.setCursor(0, 2);
            lcd.print("[=-----------------]");
            lcd.setCursor(0, 3);
            lcd.print("         5%         ");

            digitalWrite(led_entree, LOW);
            if (millis() < debut_teleportation + 3000) {
                Setangle(servo_entree, position_basse_plateforme);
            }
            else {
                Setangle(servo_entree, position_haute_plateforme);
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
        if (digitalRead(bouton) == LOW) {
            lcd.setCursor(0, 0);
            lcd.print("Veuillez poser un ");
            lcd.setCursor(0, 1);
            lcd.print("object sur le socle");
            lcd.setCursor(0, 2);
            lcd.print("avant d effectuer ");
            lcd.setCursor(0, 3);
            lcd.print("une teleportation. ");
            // probablement mettre un son de buzzer pour améliorer l'UI
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

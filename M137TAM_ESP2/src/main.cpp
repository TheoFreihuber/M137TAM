#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

bool objet_posee = false;



void Setangle(int servo, int angle);

//region initialisation des broches
/// BROCHE ///
constexpr int ecran1 = 0;
constexpr int ecran2 = 0;

constexpr int bouton = 0;

constexpr int servo_entree = 0;

constexpr int capteur_proxi = 0;

constexpr int led_entree = 0;

/// ECRAN ///
constexpr int largeur_ecran = 20;
constexpr int hauteur_ecran = 4;
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
    lcd.setCursor(3, 0);
    lcd.print("Appuyez sur le");
    lcd.setCursor(4, 1);
    lcd.print("bouton pour");
    lcd.setCursor(4, 2);
    lcd.print("commencer la");
    lcd.setCursor(3, 3);
    lcd.print("teleportation.");
    //endregion

}

void loop() {
    if (objet_posee) {

        lcd.setCursor(0, 0);
        lcd.print("Teleportation lancee");
        lcd.setCursor(0, 1);
        lcd.print("L objet a ete envoye");
        lcd.setCursor(0, 2);
        lcd.print("█");
        lcd.setCursor(9, 3);
        lcd.print("5%");

        digitalWrite(led_entree, LOW);

        Setangle(servo_entree, 135);





    }
    else {
        Setangle(servo_entree, 180);
        if (digitalRead(bouton) == LOW) {
            lcd.setCursor(1, 0);
            lcd.print("Veuillez poser un");
            lcd.setCursor(0, 1);
            lcd.print("object sur le socle");
            lcd.setCursor(1, 2);
            lcd.print("avant d effectuer");
            lcd.setCursor(1, 3);
            lcd.print("une teleportation.");
            // probablement mettre un son de buzzer pour améliorer l'UI
        }

        if (digitalRead(capteur_proxi) == LOW) {
            lcd.setCursor(0, 0);
            lcd.print("Object detecte, vous ");
            lcd.setCursor(0, 1);
            lcd.print("pouvez effectuer une");
            lcd.setCursor(3, 2);
            lcd.print("teleportation.");
            lcd.setCursor(0, 3);
            lcd.print("↓PRESSEZ LE BOUTON↓");

            objet_posee = true;

            digitalWrite(led_entree, HIGH);
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
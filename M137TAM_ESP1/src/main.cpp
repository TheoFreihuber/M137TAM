#include <Arduino.h>

//region initialisation variable
bool teleportation = false;

bool haut = false;

bool fini = false;
//endregion

//region setup constante
constexpr int cale_ouverte = 60;
constexpr int cale_fermer = 105;

constexpr int temps_plateforme = 2000;

constexpr int temps_fin = 12000;

constexpr int temps_tapis = 8000;

int monEtapeMoteur = 0;

ulong debut_teleportation = 0;
ulong moment_haut = 0;
//endregion

//region setup pin

constexpr int moteurP1 = D1;
constexpr int moteurP2 = D2;
constexpr int moteurP3 = D5;
constexpr int moteurP4 = D0;

constexpr int servo_cale = D3;

constexpr int capteur_proxi = D7;

constexpr int bouton = D6;

constexpr int bouton_reset = D4;

//endregion


void MoteurPPClockWiseFix(int pin1, int pin2, int pin3, int pin4, int &etape) ;
void MoteurPPCounterClockWiseFix(int pin1, int pin2, int pin3, int pin4, int &etape);
void AppliquerEtape(int p1, int p2, int p3, int p4, int etape);
void Setangle(int servo, int angle);

void setup() {
    Serial.begin(115200);
    pinMode(moteurP1, OUTPUT);
    pinMode(moteurP2, OUTPUT);
    pinMode(moteurP3, OUTPUT);
    pinMode(moteurP4, OUTPUT);

    pinMode(servo_cale, OUTPUT);

    pinMode(capteur_proxi, INPUT);

    pinMode(bouton, INPUT);
    
    pinMode(bouton_reset, INPUT_PULLUP);

}

void loop() {

    if (digitalRead(bouton_reset) == LOW) {
        MoteurPPCounterClockWiseFix(moteurP1, moteurP2, moteurP3, moteurP4, monEtapeMoteur);
        haut = false;
        teleportation = false;
    }
    //TODO Regler le bouton
    if (digitalRead(bouton) == HIGH or true) { /// /!\ REGLER LE BOUTON ET DONC LE IF ///
        if (!teleportation) {
            debut_teleportation = millis();
        }
        teleportation = true;
    }

    if (teleportation and debut_teleportation + temps_tapis < millis()) {
        if (digitalRead(capteur_proxi) == HIGH) { // l'ascenseur est monté plus haut que le capteur
            if (moment_haut == 0) {
                moment_haut = millis();
            }

            if (moment_haut + temps_plateforme < millis()) {
                haut = true;
            }
        }

        if (!haut) {
            MoteurPPClockWiseFix(moteurP1, moteurP2, moteurP3, moteurP4, monEtapeMoteur);
        }

        else {
            if (moment_haut + temps_plateforme < millis() and !fini) {
                MoteurPPClockWiseFix(moteurP1, moteurP1, moteurP3, moteurP4, monEtapeMoteur);
                if (moment_haut + temps_fin < millis()) {
                    fini = true;
                }
            }
        }
    }

    if (!haut) {
        Setangle(servo_cale, cale_ouverte);
    }
    else {
        Setangle(servo_cale, cale_fermer);
    }

}

/// Fonction permettant de fixer un angle donner sur un servo donné
/// @param servo n° de pin du servo concerné
/// @param angle angle désirer sur le servo
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

/// Permet d'activer le bon pin du moteur en fonction de l'étape
/// @param p1 n° de pin du in1 moteur
/// @param p2 n° de pin du in2 moteur
/// @param p3 in3
/// @param p4 in4
/// @param etape etape actuel du moteur, le pin du moteur a activer
void AppliquerEtape(int p1, int p2, int p3, int p4, int etape) {
    digitalWrite(p1, (etape == 0) ? HIGH : LOW);
    digitalWrite(p2, (etape == 1) ? HIGH : LOW);
    digitalWrite(p3, (etape == 2) ? HIGH : LOW);
    digitalWrite(p4, (etape == 3) ? HIGH : LOW);
}

/// Permet de changer l'étape moteur pour tourner dans le sens des aiguilles d'une montre
/// @param pin1 n° de pin du in1 moteur
/// @param pin2 n° de pin du in2 moteur
/// @param pin3 in3
/// @param pin4 in4
/// @param etape etape actuel du moteur, le pin du moteur a activer
void MoteurPPClockWiseFix(int pin1, int pin2, int pin3, int pin4, int &etape) {
    static unsigned long tmpMoteurPP = 0;
    if (millis() - tmpMoteurPP > 2) {
        etape++;
        if (etape > 3) etape = 0;

        AppliquerEtape(pin1, pin2, pin3, pin4, etape);
        tmpMoteurPP = millis();
    }
}

/// Permet de changer l'étape moteur pour tourner dans le sens des aiguilles d'une montre
/// @param pin1 n° de pin du in1 moteur
/// @param pin2 n° de pin du in2 moteur
/// @param pin3 in3
/// @param pin4 in4
/// @param etape etape actuel du moteur, le pin du moteur a activer
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
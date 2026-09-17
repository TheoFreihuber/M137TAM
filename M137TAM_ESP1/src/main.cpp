#include <Arduino.h>

bool teleportation = false;

bool haut = false;

bool fini = false;

const int cale_ouverte = 63;
const int cale_fermer = 110;

int monEtapeMoteur = 0;

ulong debut_teleportation = 0;
ulong moment_haut = 0;

constexpr int moteurP1 = D1;
constexpr int moteurP2 = D2;
constexpr int moteurP3 = D5;
constexpr int moteurP4 = D0;

constexpr int servo_cale = D3;

constexpr int capteur_proxi = D7;

constexpr int bouton = D6;

constexpr int bouton_reset = D4;



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
    Serial.println(digitalRead(capteur_proxi));
    
    if (digitalRead(bouton_reset) == LOW) {
        MoteurPPCounterClockWiseFix(moteurP1, moteurP2, moteurP3, moteurP4, monEtapeMoteur);
        haut = false;
        teleportation = false;
    }

    if (digitalRead(bouton) == HIGH or true) {
        if (!teleportation) {
            debut_teleportation = millis();
        }
        teleportation = true;
    }

    if (!haut) {
        Setangle(servo_cale, cale_ouverte);
    }
    else {
        Setangle(servo_cale, cale_fermer);
    }

    if (teleportation) {
        if (debut_teleportation + 5000 < millis()) {
            if (digitalRead(capteur_proxi) == HIGH) { // /!\ CODE LOW/HIGH A VERIFIER //
                if (moment_haut == 0) {
                    moment_haut = millis();
                }

                if (moment_haut + 2000 < millis()) {
                    haut = true;
                }
            }
            if (!haut) {
                MoteurPPClockWiseFix(moteurP1, moteurP2, moteurP3, moteurP4, monEtapeMoteur);
            }
            else {
                if (moment_haut + 2000 < millis()) {
                    if (fini) {


                    }
                    else {
                        MoteurPPClockWiseFix(moteurP1, moteurP1, moteurP3, moteurP4, monEtapeMoteur);
                        if (moment_haut + 10000 < millis()) {
                            fini = true;
                        }
                    }


                }
            }
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
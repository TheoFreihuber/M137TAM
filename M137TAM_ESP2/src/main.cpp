#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int ecran1;
int ecran2;

int bouton;

int servo_entree;

int capteur_proxi;

int led_entree;


void setup() {

    pinMode(ecran1, OUTPUT);
    pinMode(ecran2, OUTPUT);

    pinMode(bouton, INPUT_PULLUP);

    pinMode(servo_entree, OUTPUT);

    pinMode(capteur_proxi, INPUT);

    pinMode(led_entree, OUTPUT);



}

void loop() {




}
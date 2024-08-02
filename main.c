//MODO ARDUINO 
//Universidad del Valle de Guatemala 
//BE3023 Digital 2
//Marcela Tobar 22297
//Cristopher Portillo 
//Micro: ESP32 
//Enciende LEDs

#include <Arduino.h>
//Declaracion de Variables
const int boton1 = 12; // Pin del botón para avanzar el contador
const int boton2 = 25; // Pin del botón para cambiar el modo
const int boton3 = 13; // Pin del botón para reducir el contador
const int ledPins[] = {32, 33, 26,27}; // Pines de los LEDs
const int numLeds = 4; // Número de LEDs

int Estadoboton1 = 0; // Estado actual del botón 1
int Estadoultimo1 = 0; // Último estado del botón 1
int Estadoboton2 = 0; // Estado actual del botón 2
int Estadoultimo2 = 0; // Último estado del botón 2
int Estadoboton3 = 0; // Estado actual del botón 3
int Estadoultimo3 = 0; // Último estado del botón 3
int mode = 0; // Modo actual del contador (0: un LED a la vez, 1: contador binario)
int ledIndex = -1; // Índice del LED actual (-1 para el estado "ninguna luz encendida")
int counter = 0; // Contador para el modo binario
void setup() {
  // Inicializa los pines de los LEDs como salida
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  // Inicializa los pines de los botones como entrada con resistencia pull-up
  pinMode(boton1, INPUT_PULLUP);
  pinMode(boton2, INPUT_PULLUP);
  pinMode(boton3, INPUT_PULLUP);
  // Inicializa la comunicación serie
  Serial.begin(9600);
}
void loop() {
  // Lee el estado actual de los botones
  Estadoboton1 = digitalRead(boton1);
  Estadoboton2 = digitalRead(boton2);
  Estadoboton3 = digitalRead(boton3);

  // Verifica si el estado del botón 2 ha cambiado
  if (Estadoboton2 != Estadoultimo2) {
    // Si el estado del botón 2 ha cambiado a presionado (LOW)
    if (Estadoboton2 == LOW) {
      // Cambia el modo
      mode = !mode;
      // Resetea los contadores
      ledIndex = -1;
      counter = 0;
      // Apaga todos los LEDs
      for (int i = 0; i < numLeds; i++) {
        digitalWrite(ledPins[i], LOW);
      }
// Imprime el modo actual en el monitor serie
      if (mode == 0) {
        Serial.println("Mode: One LED at a time");
      } else {
        Serial.println("Mode: Binary Counter");
      }
    }
  }
// Verifica si el estado del botón 1 ha cambiado
  if (Estadoboton1 != Estadoultimo1) {
    // Si el estado del botón 1 ha cambiado a presionado (LOW)
    if (Estadoboton1 == LOW) {
      if (mode == 0) {
        // Modo 0: Un LED a la vez
        // Apaga todos los LEDs
        for (int i = 0; i < numLeds; i++) {
          digitalWrite(ledPins[i], LOW);
        }
        // Incrementa el índice del LED y lo ajusta para que sea cíclico
        ledIndex = (ledIndex + 1) % (numLeds + 1);
        // Enciende el LED actual si no está en el estado "ninguna luz encendida"
        if (ledIndex != numLeds) {
          digitalWrite(ledPins[ledIndex], HIGH);
        }
      } else {
        // Modo 1: Contador binario
        counter = (counter + 1) % (1 << numLeds);
        for (int i = 0; i < numLeds; i++) {
          digitalWrite(ledPins[i], (counter >> i) & 1);
        }
      }
    }
  }
// Verifica si el estado del botón 3 ha cambiado
  if (Estadoboton3 != Estadoultimo3) {
    // Si el estado del botón 3 ha cambiado a presionado (LOW)
    if (Estadoboton3 == LOW) {
      if (mode == 0) {
        // Modo 0: Un LED a la vez
        // Apaga todos los LEDs
        for (int i = 0; i < numLeds; i++) {
          digitalWrite(ledPins[i], LOW);
        }
        // Decrementa el índice del LED y lo ajusta para que sea cíclico
        ledIndex = (ledIndex - 1 + numLeds + 1) % (numLeds + 1);
        // Enciende el LED actual si no está en el estado "ninguna luz encendida"
        if (ledIndex != numLeds) {
          digitalWrite(ledPins[ledIndex], HIGH);
        }
      } else {
        // Modo 1: Contador binario
        counter = (counter - 1 + (1 << numLeds)) % (1 << numLeds);
        for (int i = 0; i < numLeds; i++) {
          digitalWrite(ledPins[i], (counter >> i) & 1);
        }
      }
    }
  }
 // Almacena los estados actuales de los botones como los últimos estados
  Estadoultimo1 = Estadoboton1;
  Estadoultimo2 = Estadoboton2;
  Estadoultimo3 = Estadoboton3;

delay(50);
}

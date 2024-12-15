/********************************************************************************
 * Practica 6: ADS1115 + Sensor de PH
 ********************************************************************************/

/* Incluir la librería Adafruit_ADS1015.h */
#include <Adafruit_ADS1X15.h>

// Definir el Canal del ADS1115 por el que leeremos la tensión del sensor de pH
#define channelValue 0
#define offset 3.00
#define samplingInterval 20
#define printInterval 800
#define ArrayLength 40 // Número de muestras

int pHArray[ArrayLength]; // Almacena las muestras
int pHArrayIndex = 0;

// Construimos el ADS1115
Adafruit_ADS1115 ads;

void setup() {
    // Inicializamos el monitor serie
    Serial.begin(9600);
    Serial.println("Inicializando el medidor de pH");

    // Inicializamos el ADS1115
    ads.setGain(GAIN_ONE); // Configuramos la ganancia del ADS1115
    ads.begin();
}

// Función para leer el valor del sensor
int readSensorValue(int channel) {
    return ads.readADC_SingleEnded(channel);
}

// Función para calcular el voltaje a partir de la lectura del sensor
float calculateVoltage(int rawValue) {
    return rawValue * 0.1875 / 1000; // 0.1875 mV por bit para GAIN_ONE
}

// Función para calcular el valor de pH
float calculatePH(float voltage) {
    return 3.5 * voltage - offset;
}

// Función para imprimir los valores del sensor
void printPHValues(float voltage, float pHValue) {
    Serial.print("Voltage: ");
    Serial.println(voltage, 2);
    Serial.print("pH value: ");
    Serial.println(pHValue, 2);
}

void loop() {
    static unsigned long samplingTime = millis();
    static unsigned long printTime = millis();
    static float pHValue, voltage;

    if (millis() - samplingTime > samplingInterval) { // Cada samplingTime segundos se toma una muestra
        // Realizar una lectura del ADS1115
        pHArray[pHArrayIndex++] = readSensorValue(channelValue);
        if (pHArrayIndex == ArrayLength) pHArrayIndex = 0;

        // Convertir la lectura a tensión
        voltage = calculateVoltage(readSensorValue(channelValue));

        // Calcular el valor de pH
        pHValue = calculatePH(voltage);

        samplingTime = millis();
    }

    if (millis() - printTime > printInterval) { // Cada printTime segundos se escribe un dato en pantalla
        printPHValues(voltage, pHValue);
        printTime = millis();
    }
}


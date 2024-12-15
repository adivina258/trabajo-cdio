#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Crear una instancia del ADS1115
Adafruit_ADS1115 ads1115;

// Canal del ADS1115 asignado para la temperatura
int channelValueTemperature = 0;

// Variables para almacenar lecturas y cálculos
int sensorValueTemperature = 0;  // Valor leído desde el ADS1115
double temperatureValue = 0;     // Temperatura calculada en °C

void setup() {
    // Inicializar comunicación serial
    Serial.begin(9600);

    // Configurar y comenzar el ADS1115
    ads1115.setGain(GAIN_ONE);  // Ganancia configurada para rango ±4.096V
    ads1115.begin();
}

// Función para leer el valor de temperatura
double readTemperature() {
    // Leer el valor del canal asignado para temperatura
    int rawValue = ads1115.readADC_SingleEnded(channelValueTemperature);

    // Calcular la temperatura basada en el valor leído
    double calculatedTemperature = rawValue * (1.25 * 0.0010); // Fórmula de conversión
    return calculatedTemperature;
}

// Función para imprimir el valor crudo del sensor
void printSensorValue(int rawValue) {
    Serial.println();
    Serial.print("Valor del sensor (raw): ");
    Serial.print(rawValue);
}

// Función para imprimir el valor de temperatura calculado
void printTemperature(double tempValue) {
    Serial.println();
    Serial.print("Temperatura calculada: ");
    Serial.print(tempValue);
    Serial.println(" °C");
}

void loop() {
    // Leer el valor crudo desde el sensor
    sensorValueTemperature = ads1115.readADC_SingleEnded(channelValueTemperature);

    // Imprimir el valor crudo del sensor
    printSensorValue(sensorValueTemperature);

    // Calcular la temperatura
    temperatureValue = readTemperature();

    // Imprimir la temperatura calculada
    printTemperature(temperatureValue);

    // Retardo de 1 segundo entre lecturas
    delay(1000);
}



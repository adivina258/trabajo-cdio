#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Crear una instancia del ADS1115
Adafruit_ADS1115 ads1115;

// Canal del ADS1115 asignado al sensor de humedad
int channelValueHumidity = 0;

// Variables para almacenar lecturas y cálculos
int sensorValue = 0;       // Valor leído desde el ADS1115
int humidityValue = 0;     // Humedad calculada en porcentaje

void setup() {
    // Inicializar comunicación serial
    Serial.begin(9600);

    // Configurar y comenzar el ADS1115
    ads1115.setGain(GAIN_ONE);  // Ganancia configurada para rango ±4.096V
    ads1115.begin();
}

// Función para leer el valor crudo del sensor
int readSensorValue() {
    // Leer el valor del canal asignado
    return ads1115.readADC_SingleEnded(channelValueHumidity);
}

// Función para calcular la humedad
int calculateHumidity(int rawValue) {
    // Aplicar la fórmula para calcular el porcentaje de humedad
    int humidity = -0.0098 * rawValue + 197.02;

    // Ajustar valores al rango de 0% a 100%
    if (humidity > 100) {
        humidity = 100;
    }
    if (humidity < 0) {
        humidity = 0;
    }
    return humidity;
}

// Función para imprimir la humedad
void printHumidity(int humidity) {
    Serial.println();
    Serial.print("Humedad: ");
    Serial.print(humidity);
    Serial.println("%");
}

// Función para imprimir el valor crudo del sensor
void printSensorValue(int rawValue) {
    Serial.println();
    Serial.print("Valor del sensor (raw): ");
    Serial.print(rawValue);
}

void loop() {
    // Leer el valor del sensor
    sensorValue = readSensorValue();

    // Calcular la humedad
    humidityValue = calculateHumidity(sensorValue);

    // Imprimir los resultados
    printHumidity(humidityValue);
    printSensorValue(sensorValue);

    // Retardo de 1 segundo entre lecturas
    delay(1000);
}


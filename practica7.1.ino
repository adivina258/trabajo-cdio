#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Crear una instancia del ADS1115
Adafruit_ADS1115 ads1115;

// Canal del ADS1115 asignado para la luz
int channelValueLight = 0;

// Rango del sensor
const int minLightValue = 23;
const int maxLightValue = 5500;

void setup() {
    // Inicializar comunicación serial
    Serial.begin(9600);

    // Configurar y comenzar el ADS1115
    ads1115.setGain(GAIN_ONE); // Ganancia configurada para rango ±4.096V
    ads1115.begin();
}

// Función para leer el valor del sensor
int readSensorValue(int channel) {
    return ads1115.readADC_SingleEnded(channel);
}

// Función para calcular el porcentaje de luz
float calculatePercentage(int rawValue, int minValue, int maxValue) {
    float percentage = (rawValue - minValue) * 100.0 / (maxValue - minValue);
    if (percentage < 0) percentage = 0;
    if (percentage > 100) percentage = 100;
    return percentage;
}

// Función para imprimir los valores del sensor
void printSensorValue(int rawValue, float percentage) {
    Serial.println();
    Serial.print("Valor del sensor (raw): ");
    Serial.print(rawValue);
    Serial.print(" | Porcentaje de luz: ");
    Serial.print(percentage, 2); // Imprime con 2 decimales
    Serial.println(" %");
}

void loop() {
    // Leer valor del sensor
    int sensorValueLight = readSensorValue(channelValueLight);

    // Calcular porcentaje de luz
    float percentageLight = calculatePercentage(sensorValueLight, minLightValue, maxLightValue);

    // Imprimir valores
    printSensorValue(sensorValueLight, percentageLight);

    // Esperar 1 segundo antes de la siguiente lectura
    delay(1000);
}

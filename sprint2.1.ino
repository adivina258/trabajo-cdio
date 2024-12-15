#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Crear una instancia del ADS1115
Adafruit_ADS1115 ads1115;

// Canales del ADS1115 para los sensores
int channelValueHumidity = 0;     // Canal A0 para humedad
int channelValueTemperature = 1;  // Canal A1 para temperatura
int channelValueLight = 3;        // Canal A3 para luz
int channelValuePH = 2;           // Canal A2 para pH

// Valores para el medidor de pH
#define offset 3.00                // Offset para calibración de pH
#define samplingInterval 20        // Intervalo de muestreo en ms
#define printInterval 800          // Intervalo de impresión en ms
#define ArrayLength 40             // Número de muestras
int pHArray[ArrayLength];         // Array para almacenar muestras de pH
int pHArrayIndex = 0;             // Índice actual para el array de pH

// Valores de referencia para el sensor de luz
const int minLightValue = 23;
const int maxLightValue = 5500;

void setup() {
    // Inicializar comunicación serial
    Serial.begin(9600);

    // Configurar el ADS1115
    ads1115.setGain(GAIN_ONE);  // Ganancia configurada a 1 (±4.096V)
    ads1115.begin();            // Inicializar ADS1115
}

// Función para leer el voltaje desde un canal específico
float readVoltage(int channel) {
    return ads1115.readADC_SingleEnded(channel) * 0.1875 / 1000; // 0.1875 mV por bit para GAIN_ONE
}

// Función para calcular el valor de pH
float calculatePH(float voltage) {
    return 3.5 * voltage - offset; // Fórmula genérica para calcular pH
}

// Función para imprimir los valores de pH
void printValues(float voltage, float pHValue) {
    Serial.print("Voltage: ");
    Serial.println(voltage, 2);
    Serial.print("pH value: ");
    Serial.println(pHValue, 2);
}

// Función para calcular el porcentaje de luz basado en el valor leído
float calculateLightPercentage(int rawValue) {
    float percentage = (rawValue - minLightValue) * 100.0 / (maxLightValue - minLightValue);

    // Limitar el porcentaje al rango 0% - 100%
    if (percentage < 0) percentage = 0;
    if (percentage > 100) percentage = 100;

    return percentage;
}

// Función para leer la temperatura desde el canal correspondiente
double readTemperature(int channelValueTemperature) {
    int sensorValueTemperature = ads1115.readADC_SingleEnded(channelValueTemperature);
    float va = (4.096 / 32767) * sensorValueTemperature;
    double temperature = ((va - 0.79) / 0.033) - 3;
    return temperature;
}

// Función para leer la humedad desde el canal correspondiente
int readHumidity(int channelValueHumidity) {
    int sensorValueHumidity = ads1115.readADC_SingleEnded(channelValueHumidity);
    int humidity = -0.0098 * sensorValueHumidity + 197.02;

    // Limitar el valor de humedad al rango de 0% a 100%
    if (humidity > 100) humidity = 100;
    if (humidity < 0) humidity = 0;
    return humidity;
}

// Función para imprimir la temperatura
void printTemperature(double tempValue) {
    Serial.println();
    Serial.print("Temperatura: ");
    Serial.print(tempValue);
    Serial.println(" °C");
}

// Función para imprimir la humedad
void printHumidity(int humidityValue) {
    Serial.println();
    Serial.print("Humedad: ");
    Serial.print(humidityValue);
    Serial.println("%");
}

// Función para leer el valor bruto desde un canal del ADS1115
int readSensorValue(int channel) {
    return ads1115.readADC_SingleEnded(channel);
}

// Función para imprimir los valores de luz
void printSensorValue(int rawValue, float percentage) {
    Serial.println();
    Serial.print("Valor del sensor (raw): ");
    Serial.print(rawValue);
    Serial.print(" | Porcentaje de luz: ");
    Serial.print(percentage, 2);
    Serial.println(" %");
}

void loop() {
    // Leer y calcular los valores de los sensores
    double tempValue = readTemperature(channelValueTemperature);  // Leer temperatura
    int humidityValue = readHumidity(channelValueHumidity);       // Leer humedad

    int sensorValueLight = readSensorValue(channelValueLight);    // Leer valor de luz
    float percentageLight = calculateLightPercentage(sensorValueLight); // Calcular porcentaje de luz

    // Leer el voltaje para calcular el pH
    float voltage = readVoltage(channelValuePH);
    float pHValue = calculatePH(voltage);

    // Mostrar los valores en el monitor serie
    printTemperature(tempValue);
    printHumidity(humidityValue);
    printSensorValue(sensorValueLight, percentageLight);
    printValues(voltage, pHValue);

   
    

    // Retardo de 1 segundo para evitar lecturas muy frecuentes
    delay(1000);
}


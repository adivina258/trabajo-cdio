#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Crear una instancia del ADS1115
Adafruit_ADS1115 ads1115;

// Canales del ADS1115 para los sensores
int channelValueHumidity = 0;     // Canal A0 para humedad
int channelValueTemperature = 1;  // Canal A1 para temperatura

void setup() {
  // Inicializar comunicación serial
  Serial.begin(9600);

  // Configurar el ADS1115
  ads1115.setGain(GAIN_ONE);  // Ganancia configurada a 1 (para voltaje completo de ±4.096V)
  ads1115.begin();            // Inicializar ADS1115
}

// Función para leer la temperatura
double readTemperature(int channelValueTemperature) {
  // Leer el valor del canal de temperatura
  int sensorValueTemperature = ads1115.readADC_SingleEnded(channelValueTemperature);
  float va = (4.096/32767)*sensorValueTemperature;
  // Convertir el valor del sensor a temperatura (en °C)
  double temperature = ((va-0.79)/0.033)-3;
  return temperature;
}

// Función para leer la humedad
int readHumidity(int channelValueHumidity) {
  // Leer el valor del canal de humedad
  int sensorValueHumidity = ads1115.readADC_SingleEnded(channelValueHumidity);

  // Calcular la humedad relativa en porcentaje usando una fórmula específica
  int humidity = -0.0098 * sensorValueHumidity + 197.02;

  // Limitar el valor de humedad al rango de 0% a 100%
  if (humidity > 100) {
    humidity = 100;
  }
  if (humidity < 0) {
    humidity = 0;
  }
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

void loop() {
  
  // Leer y calcular los valores de los sensores
  double tempValue = readTemperature(channelValueTemperature);  // Leer temperatura
  int humidityValue = readHumidity(channelValueHumidity);    // Leer humedad

  // Mostrar los valores en el monitor serie
  printTemperature(tempValue);
  printHumidity(humidityValue);

  // Retardo de 1 segundo para evitar lecturas muy frecuentes
  delay(1000);
}

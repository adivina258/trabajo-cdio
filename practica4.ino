#include <Wire.h>
#include <Adafruit_ADS1X15.h>

class MedidorDeSalinidad {
private:
    Adafruit_ADS1115 ads; // Instancia para el ADS1115
    int powerPin;

public:
    // Constructor
    MedidorDeSalinidad(int pin) : powerPin(pin) {}

    // Inicializar el sensor
    void begin() {
        pinMode(powerPin, OUTPUT);
        digitalWrite(powerPin, LOW); // Asegurarse de que el sensor esté apagado inicialmente

        if (!ads.begin()) {
            Serial.println("No se pudo encontrar el ADS1115. Verifique la conexion.");
            while (1);
        }
    }

    // Leer salinidad en g/L
    double leerSalinidad() {
        digitalWrite(powerPin, HIGH); // Encender el sensor
        delay(100); // Dar tiempo al sensor para estabilizarse

        int16_t adc0 = ads.readADC_SingleEnded(0); // Leer desde el canal 0 del ADS1115
        double salinidad = calcularSalinidad(adc0);

        digitalWrite(powerPin, LOW); // Apagar el sensor

        return salinidad;
    }

private:
    // Calcular salinidad a partir de la lectura ADC
    double calcularSalinidad(int16_t adc0) {
        double salinidad = 1 * ((pow(adc0, 2)) - 1160 * adc0 + 336000) / 3200 +
                           5 * ((pow(adc0, 2)) - 1120 * adc0 + 312000) / -1600 +
                           20 * ((pow(adc0, 2)) - 1080 * adc0 + 291200) / 3200;
        return salinidad;
    }
};

// Instancia de la clase MedidorDeSalinidad
MedidorDeSalinidad medidor(5); // El pin 5 se usa para alimentar el sensor

void setup() {
    Serial.begin(9600);
    medidor.begin(); // Inicializar el medidor
}

void loop() {
    double salinidad = medidor.leerSalinidad();

    Serial.print("Salinidad estimada = ");
    Serial.print(salinidad, 2);
    Serial.println(" g/L");

    delay(1000); // Esperar un segundo antes de la siguiente lectura
}

// sprint 3 equipo 4

#include <ESP8266WiFi.h>
#include <Adafruit_ADS1X15.h>

// Comentar/Descomentar para ver mensajes de depuración en el monitor serie
#define PRINT_DEBUG_MESSAGES

// Comentar/Descomentar para conexión dentro/fuera de UPV
//#define WiFi_CONNECTION_UPV

// Selección de servidor REST
//#define REST_SERVER_THINGSPEAK
#define REST_SERVER_DWEET

// Configuración WiFi
#ifdef WiFi_CONNECTION_UPV
const char WiFiSSID[] = "GTI1";
const char WiFiPSK[] = "1PV.arduino.Toledo";
#else
const char WiFiSSID[] = "HUAWEI Y5 2018";  // Cambiar
const char WiFiPSK[] = "123456788";  // Cambiar
#endif

// Configuración del servidor REST
#ifdef REST_SERVER_THINGSPEAK
const char Server_Host[] = "api.thingspeak.com";
const int Server_HttpPort = 80;
#else
const char Server_Host[] = "dweet.io";
const int Server_HttpPort = 80;
#endif

WiFiClient client;

// Configuración de REST
#ifdef REST_SERVER_THINGSPEAK
String MyWriteAPIKey = "9OS5XBXFJV283GRD";
#else
String MyWriteAPIKey = "cdiocurso2024g04";
#endif

#define NUM_FIELDS_TO_SEND 6 // Número de datos a enviar al servidor REST

// Pin LED
const int LED_PIN = 5;
#define power_pin 5 // Pin para alimentar el sensor de salinidad

// Instancia del ADS1115
Adafruit_ADS1115 ads1115;

// Clase base Sensor
class Sensor {
protected:
    int channel;
public:
    Sensor(int channel) : channel(channel) {}
    virtual void read() = 0;
    virtual String getData() = 0;
};

// Sensor de temperatura
class TemperatureSensor : public Sensor {
private:
    double temperature;
public:
    TemperatureSensor(int channel) : Sensor(channel), temperature(0) {}
    void read() override {
        int sensorValue = ads1115.readADC_SingleEnded(channel);
        float va = (4.096 / 32767) * sensorValue;
        temperature = ((va - 0.79) / 0.033) - 3;
    }
    String getData() override {
        return String(temperature, 2);
    }
};

// Sensor de humedad
class HumiditySensor : public Sensor {
private:
    int humidity;
public:
    HumiditySensor(int channel) : Sensor(channel), humidity(0) {}
    void read() override {
        int sensorValue = ads1115.readADC_SingleEnded(channel);
        humidity = -0.0098 * sensorValue + 197.02;
        humidity = constrain(humidity, 0, 100);
    }
    String getData() override {
        return String(humidity);
    }
};

// Sensor de luz
class LightSensor : public Sensor {
private:
    float lightPercentage;
    const int minValue, maxValue;
public:
    LightSensor(int channel, int minValue, int maxValue)
        : Sensor(channel), lightPercentage(0), minValue(minValue), maxValue(maxValue) {}
    void read() override {
        int rawValue = ads1115.readADC_SingleEnded(channel);
        lightPercentage = (float)(rawValue - minValue) * 100.0 / (maxValue - minValue);
        lightPercentage = constrain(lightPercentage, 0, 100);
    }
    String getData() override {
        return String(lightPercentage, 2);
    }
};

// Sensor de pH
class PHSensor : public Sensor {
private:
    float pHValue;
    const float offset;
public:
    PHSensor(int channel, float offset) : Sensor(channel), pHValue(0), offset(offset) {}
    void read() override {
        float voltage = ads1115.readADC_SingleEnded(channel) * 0.1875 / 1000.0;
        pHValue = 3.5 * voltage - offset;
    }
    String getData() override {
        return String(pHValue, 2);
    }
};

// Canales y Instancias de sensores
TemperatureSensor tempSensor(1); 
HumiditySensor humiditySensor(2);
LightSensor lightSensor(3, 23, 5500);
PHSensor phSensor(0, 3.0);
 // No me ha dado tiempo a crear la clase para el medidor de salinidad . Pero se ha implementado como una funcion normal.
double calcularSalinidad(int16_t adc0) {  // función para calcular la salinidad
    double salinidad = 1 * ((pow(adc0, 2)) - 1250 * adc0 + 390600) / 2000 + 5 * ((pow(adc0, 2)) - 1200 * adc0 + 359600) / 500 + 20 * ((pow(adc0, 2)) - 1210 * adc0 + 365400) / -400;
    return salinidad;
}

void connectWiFi() {
    byte ledStatus = LOW;
    WiFi.begin(WiFiSSID, WiFiPSK);
    while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(LED_PIN, ledStatus);
        ledStatus = (ledStatus == HIGH) ? LOW : HIGH;
        delay(500);
    }
    digitalWrite(LED_PIN, HIGH);
}

void HTTPGet(String fieldData[], int numFields) {
    if (client.connect(Server_Host, Server_HttpPort)) {
        String PostData = "GET http://dweet.io/dweet/for/" + MyWriteAPIKey + "?";
        for (int field = 1; field <= numFields; field++) {
            // Usar los nombres de las variables directamente, sin "field"
            String fieldName = "";
            if (field == 1) fieldName = "Temperatura";    // Nombre de la variable para temperatura
            if (field == 2) fieldName = "Humedad";       // Nombre de la variable para humedad
            if (field == 3) fieldName = "Luminiscencia ";          // Nombre de la variable para luz
            if (field == 4) fieldName = "pH";             // Nombre de la variable para pH
            if (field == 5) fieldName = "Salinidad";       // Nombre de la variable para salinidad

            // Construir la solicitud HTTP con el nombre de las variables
            PostData += fieldName + "=" + fieldData[field];
            if (field < numFields) PostData += "&";  // Añadir '&' entre los campos
        }
        client.print(PostData);
        client.println(" HTTP/1.1");
        client.println("Host: " + String(Server_Host));
        client.println("Connection: close");
        client.println();
    }
}

void setup() {
    Serial.begin(9600);
    ads1115.setGain(GAIN_ONE);
    ads1115.begin();
    pinMode(power_pin, OUTPUT);
    connectWiFi();
}

void loop() {
    tempSensor.read();
    humiditySensor.read();
    lightSensor.read();
    phSensor.read();

    digitalWrite(power_pin, HIGH);
    delay(100);
    int16_t adc0 = analogRead(A0);
    double salinidad = calcularSalinidad(adc0);
/*
    Serial.print("Salinidad estimada = ");
    Serial.print(salinidad, 2);
    Serial.println(" g/L");
    Serial.print("Lectura digital = ");
    Serial.println(adc0, DEC);
*/
// enviar datos a la web 
    String data[NUM_FIELDS_TO_SEND + 1];
    data[1] = tempSensor.getData();
    data[2] = humiditySensor.getData();
    data[3] = lightSensor.getData();
    data[4] = phSensor.getData();
    data[5] = String(adc0); 

    HTTPGet(data, NUM_FIELDS_TO_SEND);
    delay(1000);
}

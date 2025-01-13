#define power_pin 5 // Pin para alimentar el sensor de salinidad

void setup() {
    pinMode(power_pin, OUTPUT); 
    Serial.begin(9600); 
}

void loop() {
    int16_t adc0;
    digitalWrite(power_pin, HIGH);
    delay(100);
    adc0 = analogRead(A0);
    double salinidad = calcularSalinidad(adc0);
    Serial.print("Lectura digital = ");
    Serial.println(adc0, DEC);

    Serial.print("Salinidad estimada = ");
    Serial.print(salinidad, 2); 
    Serial.println(" g/L");
    delay(1000); 
}
double calcularSalinidad(int16_t adc0) {  // funcion para calcular el voltaje de salida
    double salinidad = 1((pow(adc0,2)) - 1250adc0 + 390600)/2000 + 5((pow(adc0,2)) - 1200adc0 + 359600)/500 + 20((pow(adc0,2)) - 1210adc0+365400)/-400;
    return salinidad;
}
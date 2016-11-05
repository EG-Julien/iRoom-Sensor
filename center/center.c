#include <SoftwareSerial.h>
#include <String.h>

const int temp_pin = 0;
const int white = 2;
const int blue = 3;
const int red = 5;
const int green = 4;
const int fan = 9;

int blue_status = 0;
int white_status = 0;
int red_status = 0;
int green_status = 0;

int timer = 0;
int countdown = 0;

String temp_sensor;
String tension;
String light_sensor;
String mesure;
String a_status;
String b_status;

String command;

float current_temp;

SoftwareSerial rpiSerial(10, 11);
SoftwareSerial comSerial(0, 1);

void setup() {
    pinMode(blue, OUTPUT);
    pinMode(white, OUTPUT);
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(fan, OUTPUT);

    rpiSerial.begin(115200);
    rpiSerial.println("Serial iServer Connection Started");
    comSerial.begin(115200);
}

void loop() {

    current_temp = getTemp(temp_pin);

    while (comSerial.available() > 0) {
        char inByte = comSerial.read();
        if (inByte == 't') {
            temp_sensor = command;
            rpiSerial.println(temp_sensor);
            rpiSerial.println(current_temp);
        } else if (inByte == 'v') {
            tension = command;
            rpiSerial.println(tension);
        } else if (inByte == 'l') {
            light_sensor = command;
            rpiSerial.println(light_sensor);
        } else if (inByte == 'm') {
            mesure = command;
            rpiSerial.println(mesure);
        } else if (inByte == 'a') {
            a_status = command;
            rpiSerial.println(a_status);
        } else if (inByte == 'b') {
            b_status = command;
            rpiSerial.println(b_status);
        } else {
            command = command + inByte;
        }
    }

    while (rpiSerial.available() > 0) {
        char inByte = rpiSerial.read();
        rpiSerial.println(inByte);
            if (inByte == '4') {
                if (blue_status == 0) {
                    blue_status = 1;
                } else {
                    blue_status = 0;
                }
            } else if (inByte == '5') {
                if (white_status == 0) {
                    white_status = 1;
                } else {
                    white_status = 0;
                }
            } else if (inByte == '6') {
                if (red_status == 0) {
                    red_status = 1;
                } else {
                    red_status = 0;
                }
            } else if (inByte == '7') {
                if (green_status == 0) {
                    green_status = 1;
                } else {
                    green_status = 0;
                }
            }
    }

    if (current_temp > 30.0) {
        digitalWrite(fan, HIGH);
        countdown = 11;
    }

    if (countdown == 1) {
        digitalWrite(fan, LOW);
    }

    if (blue_status == 1) {
        digitalWrite(blue, HIGH);
    } else {
        digitalWrite(blue, LOW);
    }

    if (white_status == 1) {
        digitalWrite(white, HIGH);
    } else {
        digitalWrite(white, LOW);
    }

    if (red_status == 1) {
        digitalWrite(red, HIGH);
    } else {
        digitalWrite(red, LOW);
    }

    if (green_status == 1) {
        digitalWrite(green, HIGH);
    } else {
        digitalWrite(green, LOW);
    }

    if (b_status == "1" && timer % 2 == 0) {
        red_status = 1;
    } else if (b_status == "1" && timer % 2 != 0){
        red_status = 0;
    }

    if (current_temp > 40 && timer % 2 == 0) {
        red_status = 1;
    } else if (current_temp > 40 && timer % 2 != 0){
        red_status = 0;
    }

    if (countdown != 0 && timer % 2 == 0) {
        countdown--;
    }

    timer++;
    delay(500);
}

float getTemp(int temp_pin) {
    int valeur_brute = analogRead(temp_pin);
    return valeur_brute * (5.0 / 1023.0 * 100.0);
}
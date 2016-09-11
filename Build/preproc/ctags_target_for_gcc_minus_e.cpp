# 1 "D:\\Project\\iRoom\\Arduino\\sensors.c"
# 1 "D:\\Project\\iRoom\\Arduino\\sensors.c"
# 2 "D:\\Project\\iRoom\\Arduino\\sensors.c" 2
# 3 "D:\\Project\\iRoom\\Arduino\\sensors.c" 2
# 4 "D:\\Project\\iRoom\\Arduino\\sensors.c" 2
# 5 "D:\\Project\\iRoom\\Arduino\\sensors.c" 2



int timer = 0;
int a_status = 0;
int b_status = 0;

const int alim = 1;
const int light = 0;
const int RX = 0;
const int therm = 2;
const int TX = 1;
const int vecho = 8;
const int vtrig = 7;
const int buzzer = 13;

LiquidCrystal lcd(12, 11, 10, 4, 3, 2);
SoftwareSerial comSerial(0, 1); //RX, TX

void setup() {
    pinMode(vtrig, 0x1);
    digitalWrite(vtrig, 0x0);
    pinMode(vecho, 0x0);
    Serial.begin(115200);
    lcd.begin(16, 2);
    lcd.print("----------------");
    lcd.setCursor(0,1);
    lcd.print("   iRoom v1.0   ");
    comSerial.begin(9600);
    comSerial.println("----------------------------------------------------------------------------------------");
    comSerial.println("                           iServer - iRoom Devices - iSensors                    ");
    comSerial.println("----------------------------------------------------------------------------------------");
}

void loop() {

    while (comSerial.available() > 0) {
        String readed;
        char inByte = comSerial.read();
        if (inByte != "/") {
            readed = readed + inByte;
        } else {

            if (readed == "alarm::on") {
                a_status = 1;
            } else if (readed == "alarm::off") {
                a_status = 0;
            } else if (readed == "buzzer::on") {
                b_status = 1;
            } else if (readed == "buzzer::off") {
                b_status = 0;
                a_status = 0;
            }
        }
    }

    lcd.clear();
    lcd.print(getTemperature(therm));
    Serial.println(getTemperature(therm));

    if (b_status == 1 && timer % 2 != 0) {
        digitalWrite(buzzer, 0x1);
    } else {
        digitalWrite(buzzer, 0x0);
    }

    if (a_status == 1 && getMesurim(vtrig, vecho) > 10) {
        b_status = 1;
    }

    timer = timer + 1;
    delay(500);
}

float getMesurim(int trig, int echo) {
    digitalWrite(trig, 0x1);
    delayMicroseconds(10);
    digitalWrite(trig, 0x0);
    int lecture_echo = pulseIn(echo, 0x1);
    float cm = lecture_echo / 58;

    return cm;
}

float getTemperature(int therm_pin) {
    float U = analogRead(therm_pin);
    U = (5*U)/1023;

    float R = ((2200*5.0)/U) - 2200;

    float temp = (0.0000000000007*R*R*R*R) - (0.000000004*R*R*R) -( 0.000006*R*R) + (0.1113*R) - 154.89 - 4;

    return temp;
}

float getVoltage(int alim_pin) {
    float U = analogRead(alim_pin);
    return (5*U)/1023;
}

float getLighting(int light_pin) {
    float U = analogRead(light_pin);

    return (100*U)/1023;
}

void sendData() {
    comSerial.println(getTemperature(therm));
    comSerial.println(getVoltage(alim));
    comSerial.println(getLighting(light));
    comSerial.println(getMesurim(vtrig, vecho));
    comSerial.println(a_status);
    comSerial.println(b_status);
}

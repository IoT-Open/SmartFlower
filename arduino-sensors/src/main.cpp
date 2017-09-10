#include <ArduinoJson.h>
#include <dht.h>

dht DHT;

int DHT11_PIN = 2;
int DHT11_VCC = 3;

int led_out = 13;

byte humidity_sensor_pin = A0;
byte humidity_sensor_vcc = 6;

int water_bomb = 7;

int greenPin = 10;
int redPin = 9;
int bluePin = 8;

int read_humidity_sensor()
{
    digitalWrite(humidity_sensor_vcc, HIGH);
    delay(500);
    int value = analogRead(humidity_sensor_pin);
    digitalWrite(humidity_sensor_vcc, LOW);
    return 1003 - value;
}

#define COMMON_ANODE
void setColor(int red, int green, int blue)
{

    #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
    #endif

    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);

}

void setup()
{
    pinMode(humidity_sensor_vcc, OUTPUT);
    pinMode(DHT11_VCC, OUTPUT);
    pinMode(led_out, OUTPUT);
    pinMode(water_bomb, OUTPUT);

    digitalWrite(humidity_sensor_vcc, LOW);

    while (!Serial);
    delay(1000);
    Serial.begin(9600);
}

void loop()
{

    digitalWrite(DHT11_VCC, HIGH);
    delay(500);
    int chk = DHT.read11(DHT11_PIN);
    digitalWrite(DHT11_VCC, LOW);

    int solo_humidity = read_humidity_sensor();

    float temperature = DHT.temperature;
    float air_humidity = DHT.humidity;

    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["solo_humidity"] = solo_humidity;
    root["temperature"] = temperature;
    root["air_humidity"] = air_humidity;

    String json_output;

    root.printTo(json_output);

    Serial.println(json_output);

    if (solo_humidity >= 200)
        setColor(0, 255, 0);
    else if (solo_humidity > 50 && solo_humidity < 200)
        setColor(0, 0, 255);
    else
        setColor(255, 0, 0);

    if(solo_humidity < 50)
    {

        digitalWrite(water_bomb, HIGH);

        while(true)
        {
            solo_humidity = read_humidity_sensor();
            if(solo_humidity > 200)
            {break;}
            delay(300);
        }

        digitalWrite(water_bomb, LOW);

    }
        
    digitalWrite(led_out, HIGH);
    delay(500);
    digitalWrite(led_out, LOW);
    delay(500);

}
#include <ArduinoJson.h>
#include <dht.h>

int DELAY = 30; //in seconds

dht DHT;
int DHT11_PIN = 2;
int DHT11_VCC = 3;

byte HUMDITY_SENSOR = A0;
byte HUMDITY_SENSOR_VCC = 6;

int WATER = 7;

int LED_RED = 9;
int LED_GREEN = 10;
int LED_BLUE = 8;

void readDHT11Sensor()
{
    digitalWrite(DHT11_VCC, HIGH);
    
    delay(500);
    int chk = DHT.read11(DHT11_PIN);

    digitalWrite(DHT11_VCC, LOW);
}

int readHumiditySensor()
{
    digitalWrite(HUMDITY_SENSOR_VCC, HIGH);
    
    delay(500);

    int value = analogRead(HUMDITY_SENSOR);
    digitalWrite(HUMDITY_SENSOR_VCC, LOW);

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

    analogWrite(LED_RED, red);
    analogWrite(LED_GREEN, green);
    analogWrite(LED_BLUE, blue);

}

void defineColor(int humidity_sensor_value)
{

    if (humidity_sensor_value >= 200) 
    { setColor(0, 255, 0); }
    else if (humidity_sensor_value > 50 && humidity_sensor_value < 200) 
    { setColor(0, 0, 255); }
    else
    { setColor(255, 0, 0); }

}

void waterBombBehavior(int humidity_sensor_value)
{
    if(humidity_sensor_value < 150)
    {

        digitalWrite(WATER, HIGH);

        while(true)
        {
            humidity_sensor_value = readHumiditySensor();
            if(humidity_sensor_value > 500)
            {break;}
            delay(300);
        }

        digitalWrite(WATER, LOW);

    }
}

String formatJson(int humidity_sensor_value, int temperature, int air_humidity)
{
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["solo_humidity"] = humidity_sensor_value;
    root["temperature"] = temperature;
    root["air_humidity"] = air_humidity;

    String json_output;
    root.printTo(json_output);

    return json_output;
}

void setup()
{
    
    pinMode(DHT11_VCC, OUTPUT);
    pinMode(WATER, OUTPUT);
    pinMode(HUMDITY_SENSOR_VCC, OUTPUT);

    while (!Serial);
    delay(1000);
    Serial.begin(9600);

}

void loop()
{

    int humidity_sensor_value = readHumiditySensor();

    readDHT11Sensor();
    float temperature = DHT.temperature;
    float air_humidity = DHT.humidity;

    String json_output = formatJson(humidity_sensor_value, temperature, air_humidity);
    
    Serial.println(json_output);

    defineColor(humidity_sensor_value);
    waterBombBehavior(humidity_sensor_value);
        
    delay(DELAY * 1000);

}
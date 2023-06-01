#include <WiFi.h>
#include <PubSubClient.h>
#define TOKEN “BBFF - 44guxZ68tQFWqxGENx4IyzevZymFUj”
#define MQTT_CLIENT_NAME “ESP32 "
#define VARIABLE_NAME “dbm”
#define DEVICE_LABEL “esp32 "
const char *ssid = “Inteli - COLLEGE”;
const char *password = “QazWsx @123 ";
    char mqttbroker[] = “industrial.api.ubidots.com”;
char payload[100];
char topic[150];
char topicSubscribe[100];
WiFiClient ubidots;
PubSubClient client(ubidots);
void reconnect();
void setup()
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.println(“Conectando ao Wi - Fi...“);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(“.”);
    }
    Serial.println(“Conexão Wi - Fi estabelecida !“);
    Serial.print(“Endereço IP
                 : “);
    Serial.println(WiFi.localIP());
    client.setServer(mqttbroker, 1883);
}
void loop()
{
    if (!client.connected())
    {
        reconnect();
    }
    int signalStrength = WiFi.RSSI();
    Serial.print(“Intensidade do sinal
                 : “);
    Serial.print(signalStrength);
    Serial.println(” dBm”);
    snprintf(topic, sizeof(topic), “/ v1 .6 / devices / % s”, DEVICE_LABEL);
    snprintf(
        payload, sizeof(payload), “{ \“% s\“: % d }“, VARIABLE_NAME, signalStrength);
    client.publish(topic, payload);
    delay(2000);
}
void reconnect()
{
    while (!client.connected())
    {
        Serial.print(“Conectando ao broker MQTT...“);
        if (client.connect(MQTT_CLIENT_NAME, TOKEN, “”))
        {
            Serial.println(“Conectado !“);
            snprintf(topicSubscribe, sizeof(topicSubscribe), “/ v1 .6 / devices / % s / config”, DEVICE_LABEL);
            client.subscribe(topicSubscribe);
        }
        else
        {
            Serial.print(“Falha na conexão, rc =“);
            Serial.print(client.state());
            Serial.println(” Tentando novamente em 5 segundos...“);
            delay(5000);
        }
    }
}

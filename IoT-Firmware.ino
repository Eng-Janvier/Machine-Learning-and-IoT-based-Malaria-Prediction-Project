#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#define DHTPIN 12
#define DHTTYPE DHT22

DHT dht(DHTPIN,DHTTYPE);

#define analogpin A0

const char* ssid = "nijas2020";
const char* password = "urwombonye";
//const char* mqtt_server = "192.168.0.50";
const char* mqtt_server = "broker.mqttdashboard.com";
//const char* mqtt_server = "iot.eclipse.org";
WiFiClient espClient;
PubSubClient client(espClient);        
long randNumber;
void callback(char* topic, byte* payload, unsigned int length) 
{
} //end callback
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("Doctor_message");
      //client.publish("Attendance","Janvier Detected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end reconnect()

void setup() {
  Serial.begin(15200);
  delay(100);
  dht.begin();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 1883);
  randomSeed(analogRead(0));
}
void loop() {
  if (!client.connected()) {
  reconnect();
  }
  client.loop();
  client.setCallback(callback);
  //float t=dht.readTemperature();
  //float h=dht.readHumidity();
  float t=36.0;
  float h=120.0;
  randNumber = random(0,20);
  Serial.println(randNumber);
  char csrain[16];
  itoa(randNumber,csrain,10);
  char cshum[16];
  
  char cstemp[16];
  itoa(t,cstemp,10);
  Serial.print("Temperature = ");
  Serial.print(cstemp);
  Serial.print("   ");
  
  itoa(h,cshum,10);
  Serial.print("  Humidity = ");
  Serial.print(cshum);
  Serial.print("   Rain = ");
  Serial.println(csrain);
  Serial.println("Publish a message Temperature ,Rain and Humidity:");
  client.publish("ACEIoT/janvier/temperature/db",cstemp);
  client.publish("ACEIoT/janvier/humidity/db",cshum);
  client.publish("ACEIoT/janvier/rain/db",csrain);
  delay(5000);
}


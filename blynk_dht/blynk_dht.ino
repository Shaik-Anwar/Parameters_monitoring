
//Weather monitoring station project created by Shaik Anwar, 
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>
BlynkTimer timer;
 
const char *ssid =  "";  
const char *pass =  "";
char auth[] = "";

int ldr = A0;
int rain = D0;
int rain_data;
int ldr_data;
 
DHT dht(D1, DHT11);
 
WiFiClient client;
 
long myChannelNumber = 241392;
const char myWriteAPIKey[] = "";

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  ldr_data = analogRead(ldr)/10;
  rain_data = digitalRead(rain);
 
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
  Blynk.virtualWrite(V3, rain_data);
  Blynk.virtualWrite(V4, ldr_data);
}
 
void setup() {
  // put your setup code here, to run once:
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(D0,INPUT);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("..");
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());
  dht.begin();
  ThingSpeak.begin(client);
}
 
void loop() {
  // put your main code here, to run repeatedly:
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  ldr_data = analogRead(ldr)/10;
  rain_data = digitalRead(rain);
  Serial.println("Temperature: " + (String) t);
  Serial.println("Humidity: " + (String) h);
  Serial.println("Light: " + (String) ldr_data);
  Serial.println("rain: " + (String) rain_data);
  ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2, h, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 3, rain_data, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 4, ldr_data, myWriteAPIKey);
  delay(2000);
  Blynk.run();
  timer.run();
}

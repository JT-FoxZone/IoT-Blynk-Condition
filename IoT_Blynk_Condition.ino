#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
#define BLYNK_TEMPLATE_NAME         "Device"
#define BLYNK_AUTH_TOKEN            "YourAuthToken"

#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

#define DHTPIN D2          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Global variable
int Temp_Condition = 0;
float Humidity, Temperature;

BLYNK_WRITE(V1)
{
  Temp_Condition = param.asInt();
}

void dhtEvent()
{
  Humidity = dht.readHumidity();
  Temperature = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(Humidity) || isnan(Temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, Humidity);
  Blynk.virtualWrite(V6, Temperature);
}

void TempController()
{
  if(Temperature > Temp_Condition) {
    digitalWrite(D1, HIGH);
    Blynk.virtualWrite(V2, 1);
  } else {
    digitalWrite(D1, LOW);
    Blynk.virtualWrite(V2, 0);
  }
}

void setup()
{
  pinMode(D1, OUTPUT);
  // Debug console
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, dhtEvent);
  timer.setInterval(1000L, TempController);
}

void loop()
{
  Blynk.run();
  timer.run();
}

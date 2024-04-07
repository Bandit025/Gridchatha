#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h> // เพิ่มไลบรารี PubSubClient

#define PIR_PIN 13 
#define DHTPIN 4       // Pin connected to the AM2302 sensor
#define DHTTYPE DHT22  // DHT22 sensor type

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "KNJPA_2.4G"; // ชื่อของ WiFi network
const char* password = "11121150"; // รหัสผ่านของ WiFi network
const char* mqtt_server = "192.168.199.43"; // ที่อยู่ IP ของ MQTT broker
const int mqtt_port = 1883; // Port ของ MQTT broker
const char* mqtt_username = ""; // ชื่อผู้ใช้ MQTT broker
const char* mqtt_password = ""; // รหัสผ่าน MQTT broker
const char* mqtt_topic_temp = "sensors/temperature"; // ชื่อ topic สำหรับอุณหภูมิ
const char* mqtt_topic_pir = "sensors/pir"; // ชื่อ topic สำหรับความชื้น

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi..");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void callback(char* topic, byte* payload, unsigned int length) {
  // การดำเนินการเมื่อมีข้อมูลส่งมาที่ MQTT subscriber
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ArduinoClient", mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200); // Start serial communication
  dht.begin();        // Initialize DHT sensor
  pinMode(PIR_PIN, INPUT);  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int pirState = digitalRead(PIR_PIN); 
  Serial.print("PIR = ");
  Serial.println(pirState);

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT22 sensor!");
    return;
  }

  // Print temperature and humidity to Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Publish temperature and humidity data to MQTT broker
  client.publish(mqtt_topic_temp, String(temperature).c_str(), true);
  client.publish(mqtt_topic_pir, String(pirState).c_str(), true);
}

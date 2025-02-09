#define BLYNK_TEMPLATE_ID "TMPL4xzVmY4Sv"
#define BLYNK_TEMPLATE_NAME "Intelligent Room"
#define BLYNK_AUTH_TOKEN "Cv8CvCvbmdQf-If4V8qlH7as_1XDnGHg"

#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>

char ssid[] = "iPhone - Karina";
char pass[] = "123456789";

#define GREEN_LED_PIN     2
#define BLUE_LED_PIN      4
#define TRIG_PIN          12
#define SERVO_PIN         13
#define ECHO_PIN          14
#define RED_LED_PIN       15
#define FAN_PIN           22
#define BUZZER_PIN        23
#define AIR_QUALITY_PIN   34
#define DHT_PIN           26

Servo servoMotor;
DHT dht(DHT_PIN, DHT11);

float durationUs, distanceCm;
int impuritiesReading, airQuality;
int fanSpeed = 100;
bool fanStatus = false, fanStarted = false;
float temperature, humidity;

BlynkTimer timer;

void sendAirQualityData() {
  Blynk.virtualWrite(V4, airQuality);
}

void sendTemperatureAndHumidity() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  Blynk.virtualWrite(V7, temperature);
  Blynk.virtualWrite(V8, humidity);
}

void measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  durationUs = pulseIn(ECHO_PIN, HIGH);
  distanceCm = (durationUs * 0.034) / 2;
}

void readAirQuality() {
  impuritiesReading = analogRead(AIR_QUALITY_PIN);
  airQuality = map(impuritiesReading, 0, 4095, 100, 10);
}

void handleAirQuality() {
  if (airQuality < 60) {
    digitalWrite(BLUE_LED_PIN, HIGH);
    Blynk.virtualWrite(V2, 1);
    analogWrite(BUZZER_PIN, 128);
    analogWrite(FAN_PIN, 255);
    Blynk.virtualWrite(V5, 1);
    fanStarted = true;
    delay(500);
  } else {
    resetComponents();
  }
}

void handleDistance() {
  if (distanceCm < 15) {
    servoMotor.write(90);
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V1, 1);
  } else {
    servoMotor.write(0);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
    Blynk.virtualWrite(V0, 1);
    Blynk.virtualWrite(V1, 0);
  }
}

void resetComponents() {
  if(fanStarted) {
    analogWrite(FAN_PIN, 0);
    Blynk.virtualWrite(V5, 0);
    fanStarted = false;
  }
  analogWrite(BUZZER_PIN, 0);
  digitalWrite(BLUE_LED_PIN, LOW);
  Blynk.virtualWrite(V2, 0);
}

BLYNK_WRITE(V5) {
  fanStatus = param.asInt();
  controlFan();
}

BLYNK_WRITE(V6) {
  int fanInput = param.asInt();
  fanSpeed = map(fanInput, 0, 10, 0, 255);
  controlFan();
}

void controlFan() {
  if (fanStatus == 1) {
    analogWrite(FAN_PIN, fanSpeed);
  } else {
    analogWrite(FAN_PIN, 0);
  }
}

void setup() {
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(FAN_PIN, OUTPUT);
  analogWrite(FAN_PIN, 0);

  pinMode(SERVO_PIN, OUTPUT);
  servoMotor.attach(SERVO_PIN);
  servoMotor.write(0);

  dht.begin();
  delay(2000);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, sendAirQualityData);
  timer.setInterval(2000L, sendTemperatureAndHumidity);
}

void loop() {
  Blynk.run();
  timer.run();

  measureDistance();
  handleDistance();
  readAirQuality();
  handleAirQuality();

  delay(500);
}

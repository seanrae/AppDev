#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// Pin Definitions
const int ledPin = 13;
const int inputPin = 8;
const int pinSpeaker = 7;

bool alarmActive = false;
bool motionSuppressed = false;
unsigned long suppressionStart = 0;
const unsigned long suppressionDuration = 15000; // 15 seconds

// Fingerprint sensor setup
SoftwareSerial fingerSerial(2, 3);
Adafruit_Fingerprint finger(&fingerSerial);

// SIM800L setup
SoftwareSerial sim800(9, 10); // SIM800L TX = D9, RX = D10

String phoneNumber = "+639073061321";  // ✅ Updated phone number

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(inputPin, INPUT);
  pinMode(pinSpeaker, OUTPUT);

  Serial.begin(9600);
  fingerSerial.begin(57600);
  sim800.begin(9600);

  digitalWrite(ledPin, LOW);
  noTone(pinSpeaker);

  // Fingerprint setup
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Fingerprint sensor detected.");
  } else {
    Serial.println("Fingerprint sensor not detected. Check wiring.");
    while (1);
  }

  finger.getTemplateCount();
  Serial.print("Fingerprints enrolled: ");
  Serial.println(finger.templateCount);

  // SIM800L init
  delay(1000);
  sim800.println("AT");
  delay(500);
  sim800.println("AT+CMGF=1");
  delay(500);
}

void loop() {
  unsigned long currentMillis = millis();

  // Handle suppression timer expiration
  if (motionSuppressed && (currentMillis - suppressionStart >= suppressionDuration)) {
    motionSuppressed = false;
    Serial.println("15-second suppression ended. Motion will now trigger alarm.");
  }

  // Handle fingerprint scan
  if (checkFingerprint()) {
    if (alarmActive) {
      // Deactivate alarm if it was sounding
      stopAlarm();
      sendSMS("Alarm deactivated by authorized fingerprint.");
    } else {
      // Suppress future motion-triggered alarm for 15 seconds
      motionSuppressed = true;
      suppressionStart = currentMillis;
      Serial.println("Authorized fingerprint detected. Suppressing motion alarm for 15 seconds.");
    }
  }

  // Read PIR sensor
  int motion = digitalRead(inputPin);

  if (motion == HIGH) {
    if (!alarmActive && !motionSuppressed) {
      Serial.println("Motion detected! Alarm triggered.");
      triggerAlarm();
      sendSMS("Motion detected! Alarm triggered.");
    } else if (motionSuppressed) {
      Serial.println("Motion detected but alarm suppressed.");
    }
  }
}

// Triggers the alarm
void triggerAlarm() {
  digitalWrite(ledPin, HIGH);
  tone(pinSpeaker, 1000);
  alarmActive = true;
}

// Stops the alarm
void stopAlarm() {
  digitalWrite(ledPin, LOW);
  noTone(pinSpeaker);
  alarmActive = false;
}

// Fingerprint check
bool checkFingerprint() {
  int ret = finger.getImage();
  if (ret != FINGERPRINT_OK) return false;

  ret = finger.image2Tz();
  if (ret != FINGERPRINT_OK) return false;

  ret = finger.fingerSearch();
  return (ret == FINGERPRINT_OK);
}

// Sends an SMS and logs it to Serial Monitor
void sendSMS(String message) {
  sim800.println("AT+CMGF=1"); // Set SMS text mode
  delay(100);
  sim800.print("AT+CMGS=\"");
  sim800.print(phoneNumber);
  sim800.println("\"");
  delay(300);
  sim800.println(message);    // Send message content
  delay(300);
  sim800.write(26);           // Ctrl+Z to send
  delay(1000);

  // Log to Serial Monitor
  Serial.print("SMS sent to ");
  Serial.print(phoneNumber);
  Serial.print(": ");
  Serial.println(message);
}

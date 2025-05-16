


# Thief No More: An Arduino-Based Smart Security System

---

## Introduction

**Thief No More** is a smart anti-theft security system developed using the **Arduino Uno** microcontroller. The system integrates biometric verification, motion detection, and real-time SMS notifications to provide enhanced security for homes and small businesses. Designed as a cost-effective alternative to traditional security methods, this system aims to immediately detect unauthorized access attempts and notify the property owner for timely response.

---

## System Functionality

1. **Motion Detection**  
   A Passive Infrared (PIR) motion sensor is placed near the entry point to detect any movement.

2. **Alarm Activation**  
   Upon detecting motion, the system activates a buzzer alarm and prompts the person to verify their identity via the fingerprint sensor.

3. **Fingerprint Verification**  
   - If a **registered fingerprint** is scanned, the alarm is deactivated.
   - If the fingerprint **is not recognized** or no attempt is made to verify, the buzzer remains active.

4. **SMS Alert Notification**  
   A **GSM module** sends an SMS notification to the registered mobile number, alerting the owner of a possible intrusion attempt. If GSM compatibility issues arise, a serial monitor alert is used for demonstration purposes.

---


## How to Upload and Test

1. Connect all components to the Arduino Uno according to the system circuit diagram.
2. Upload the final code using Arduino IDE.
3. Test the system:
   - Move in front of the PIR sensor.
   - Try authorized and unauthorized fingerprints.
   - Observe buzzer activation and SMS notification behavior.

---


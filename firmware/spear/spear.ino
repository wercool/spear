#include "ReceiveOnlySoftwareSerial.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Adafruit_BMP085.h>
#include <HMC5883L_Simple.h>

#define LED_PIN 13
#define RX_SOFT_SERIAL_PIN 16
#define RADIO_TRANSMIT_PIN 2
#define BUZZER_PIN 9
#define START_PIN 4
#define PARACHUTE1_PIN 7
#define PARACHUTE2_PIN 8

ReceiveOnlySoftwareSerial RXSoftSerial(RX_SOFT_SERIAL_PIN);

enum states { READY, START, CALIBRATION, LAUNCH, FLIGHT, PARACHUTE1, PARACHUTE2, LANDING };

states curState = READY;

MPU6050 accelgyro;
Adafruit_BMP085 bmp;
HMC5883L_Simple compass;

int16_t ax, ay, az;
int16_t gx, gy, gz;

/*
 * Incoming commands
 */
const char *cmd_PING = "PING";
const char *cmd_START = "START";
const char *cmd_STOP = "STOP";

String rxSoftSerialStrBuffer;
int startCTagPos = -1;
int endCTagPos = -1;
char messageBuffer[64];

unsigned long currentMillis;
unsigned long startTime = 10;
unsigned long prevStepTime = 0;

float calibrationReadings = 0;
float calibrationAltitude = 0;
float launchAltitude = 0;

unsigned long launchTime;

float fAltitude = 0;
float maxfAltitude = -1;
unsigned long maxfAltitudeTime = 0;
unsigned long parachute1Time = 0;
unsigned long parachute2Time = 0;
unsigned long landingBuzzerTime = 0;
boolean buzzer = false;

void setup() {
  pinMode(RADIO_TRANSMIT_PIN, OUTPUT);
  digitalWrite(RADIO_TRANSMIT_PIN, false);

  pinMode(START_PIN, OUTPUT);
  pinMode(PARACHUTE1_PIN, OUTPUT);
  pinMode(PARACHUTE2_PIN, OUTPUT);  
  digitalWrite(START_PIN, false);
  digitalWrite(PARACHUTE1_PIN, false);
  digitalWrite(PARACHUTE2_PIN, false);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, false);

  Wire.begin();

  Serial.begin(9600);
  RXSoftSerial.begin(9600);

  // initialize devices
  Serial.println("Initializing I2C devices...");

  // initialize bmp085
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    tone(BUZZER_PIN, 1000);
    while (1) {}
  }

  // initialize mpu6050
  accelgyro.initialize();
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  accelgyro.setI2CBypassEnabled(true); // set bypass mode for gateway to hmc5883L

  // initialize hmc5883l
  compass.SetDeclination(23, 35, 'E');
  compass.SetSamplingMode(COMPASS_SINGLE);
  compass.SetScale(COMPASS_SCALE_130);
  compass.SetOrientation(COMPASS_HORIZONTAL_X_NORTH);

  // configure Arduino LED for checking activity
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  currentMillis = millis();

  if (curState == READY || curState == START) {
    while (RXSoftSerial.available() > 0) {
      char rxSoftSerialInChar = RXSoftSerial.read();
      rxSoftSerialStrBuffer += rxSoftSerialInChar;
  
      startCTagPos = rxSoftSerialStrBuffer.indexOf("<c>");
  
      if (startCTagPos > -1) {
        endCTagPos = rxSoftSerialStrBuffer.indexOf("</c>", startCTagPos + 3);
        if (endCTagPos > -1) {
          String inCommand = rxSoftSerialStrBuffer.substring(startCTagPos + 3, endCTagPos);
  
          if (curState == READY) {
            if (strcmp(inCommand.c_str(), cmd_PING) == 0) {
              sendMessage(500, 100, 10, "PONG");
              break;
              loop();
            } else if (strcmp(inCommand.c_str(), cmd_START) == 0) {
              curState = START;
              sendMessage(500, 50, 10, "LAUNCH_IN_10s");
              prevStepTime = currentMillis;
              break;
              loop();
            }
          } else if (curState == START) {
            if (strcmp(inCommand.c_str(), cmd_STOP) == 0) {
              curState = READY;
              startTime = 10;
              prevStepTime = 0;
              sendMessage(2000, 500, 10, "LAUNCH_STOPPED");
              break;
              loop();
            }
          }        
        }
      }
      
      if (rxSoftSerialStrBuffer.length() > 32) {
        rxSoftSerialStrBuffer = "";
        startCTagPos = -1;
        endCTagPos = -1;
      }
      break;
      loop();
    }
    if (curState == START) {
      startProcedure();
    }
  } else if (curState == CALIBRATION) {
    if (currentMillis - prevStepTime < 2000) {
      float altitude = bmp.readAltitude();
      calibrationAltitude += altitude;
      calibrationReadings++;

      launchAltitude = (calibrationAltitude / calibrationReadings);
      float calibrationAltitude = altitude - launchAltitude;

      sprintf(messageBuffer, "<m>%lu,CALIBRATION,%s%d.%02d</m>", currentMillis, calibrationAltitude > 0 ? "" : "-", abs((int)calibrationAltitude), abs((int)(calibrationAltitude * 100) % 100));          
      Serial.print(messageBuffer);
      memset(messageBuffer, 0, sizeof(messageBuffer));
    } else {
      curState = LAUNCH;
    }
  } else if (curState == LAUNCH) {
    sendMessage(0, 100, 10, "LAUNCH!");
    digitalWrite(START_PIN, true);
    curState = FLIGHT;
    launchTime = currentMillis;
    digitalWrite(RADIO_TRANSMIT_PIN, true);
  }


  /*
   * FLIGHT
   */
  if (curState == FLIGHT || curState == PARACHUTE1 || curState == PARACHUTE2 || curState == LANDING) {

    if (curState == FLIGHT) {
      if (currentMillis - launchTime > 4000) {
        digitalWrite(START_PIN, false);
      }
    }

    /*
     * Barometer and MEMS readings
     */
    fAltitude = bmp.readAltitude() - launchAltitude;
    HMC5883L_Simple::MagnetometerSample magSample = compass.GetMagnetometerSample();
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    if (curState == FLIGHT) {
      if (fAltitude > maxfAltitude) {
        maxfAltitude = fAltitude;
        maxfAltitudeTime = currentMillis;
      }

      if (maxfAltitude > 2.0 && fAltitude < maxfAltitude - 2.0 && currentMillis - maxfAltitudeTime > 2000) {
        curState = PARACHUTE1;
        parachute1Time = currentMillis;
      }
    }

    if (curState == PARACHUTE1) {
      digitalWrite(PARACHUTE1_PIN, true);
      if (currentMillis - parachute1Time > 3000) {
        digitalWrite(PARACHUTE1_PIN, false);
        curState = PARACHUTE2;
        parachute2Time = currentMillis;
      }
    }

    if (curState == PARACHUTE2) {
      digitalWrite(PARACHUTE2_PIN, true);
      if (currentMillis - parachute2Time > 2000) {
        digitalWrite(PARACHUTE2_PIN, false);
        curState = LANDING;
        landingBuzzerTime = currentMillis;
      }
    }

    if (curState == LANDING) {
      if (!buzzer) {
        if (currentMillis - landingBuzzerTime > 1000) {
          tone(BUZZER_PIN, 4000);
          buzzer = true;
          landingBuzzerTime = currentMillis;
        }
      } else {
        if (currentMillis - landingBuzzerTime > 2000) {
          noTone(BUZZER_PIN);
          buzzer = false;
          landingBuzzerTime = currentMillis;
        }
      }

      if (fAltitude < 5.0) {
        RXSoftSerial.end();
      }
    }

    sprintf(messageBuffer, "<m>%lu,F,%s%d.%02d,%d,%d,%d,%d,%d,%d</m>", 
                            currentMillis - launchTime, fAltitude > 0 ? "" : "-", abs((int)fAltitude), abs((int)(fAltitude * 100) % 100),
                            magSample.X,
                            magSample.Y,
                            magSample.Z,
                            ax,
                            ay,
                            az
          );          
    Serial.print(messageBuffer);
    memset(messageBuffer, 0, sizeof(messageBuffer));
  }
}

void sendMessage(int preSendDelay, int repeat, int repeatDelay, char *msg) {
  RXSoftSerial.flush();
  rxSoftSerialStrBuffer = "";
  startCTagPos = -1;
  endCTagPos = -1;

  delay(preSendDelay);
  digitalWrite(RADIO_TRANSMIT_PIN, true);
  for (int i = 0; i < repeat; i++) {
    unsigned long messageTime = millis();
    sprintf(messageBuffer, "<m>%lu,%s</m>", messageTime, msg);
    Serial.print(messageBuffer);
    memset(messageBuffer, 0, sizeof(messageBuffer));
    delay(repeatDelay);
  }
  digitalWrite(RADIO_TRANSMIT_PIN, false);
  RXSoftSerial.flush();
  rxSoftSerialStrBuffer = "";
  startCTagPos = -1;
  endCTagPos = -1;
}

void startProcedure() {
    unsigned long curTime = millis();
    if (curTime - prevStepTime > 1000) {
      if (startTime > 1) {
        startTime -= 1;
        char dynMsgPartBuffer[64];
        sprintf(dynMsgPartBuffer, "LAUNCH_IN_%ds", startTime);
        sendMessage(0, 25, 10, dynMsgPartBuffer);
        memset(dynMsgPartBuffer, 0, sizeof(dynMsgPartBuffer));
        prevStepTime = curTime;
        RXSoftSerial.flush();
        rxSoftSerialStrBuffer = "";
        startCTagPos = -1;
        endCTagPos = -1;
      } else {
        prevStepTime = curTime;
        digitalWrite(RADIO_TRANSMIT_PIN, true);
        curState = CALIBRATION;
      }
    }
}

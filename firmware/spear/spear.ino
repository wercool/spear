#include "ReceiveOnlySoftwareSerial.h"
#include "I2Cdev.h"

#define RX_SOFT_SERIAL_PIN 16
#define RADIO_TRANSMIT_PIN 2
#define BUZZER_PIN 9
#define START_PIN 4
#define FIRST_STAGE_PIN 7
#define SECOND_STAGE_PIN 8

ReceiveOnlySoftwareSerial RXSoftSerial(RX_SOFT_SERIAL_PIN);

enum states { READY, START, LAUNCH, FLIGHT };

states curState = READY;

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

void setup() {
  pinMode(RADIO_TRANSMIT_PIN, OUTPUT);
  digitalWrite(RADIO_TRANSMIT_PIN, false);

  pinMode(START_PIN, OUTPUT);
  pinMode(FIRST_STAGE_PIN, OUTPUT);
  pinMode(SECOND_STAGE_PIN, OUTPUT);  
  digitalWrite(START_PIN, false);
  digitalWrite(FIRST_STAGE_PIN, false);
  digitalWrite(SECOND_STAGE_PIN, false);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, false);

  Wire.begin();

  Serial.begin(9600);
  RXSoftSerial.begin(9600);
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
  } else if (curState == LAUNCH) {
    RXSoftSerial.end();
    curState = FLIGHT;
    tone(BUZZER_PIN, 3500);
    digitalWrite(START_PIN, true);
    sendMessage(0, 200, 10, "LAUNCH!");
    digitalWrite(START_PIN, false);
    noTone(BUZZER_PIN);
  } else if (curState == FLIGHT) {
    
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
    sprintf(messageBuffer, "<m>%lu,%s</m>\n", messageTime, msg);
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
      } else {
        curState = LAUNCH;
      }
    }
}

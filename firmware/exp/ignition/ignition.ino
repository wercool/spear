#define START_PIN 4
#define FIRST_STAGE_PIN 7
#define SECOND_STAGE_PIN 8

void setup() {
  pinMode(START_PIN, OUTPUT);
  pinMode(FIRST_STAGE_PIN, OUTPUT);
  pinMode(SECOND_STAGE_PIN, OUTPUT);
  
  digitalWrite(START_PIN, false);
  digitalWrite(FIRST_STAGE_PIN, false);
  digitalWrite(SECOND_STAGE_PIN, false);
}

void loop() {
  delay(5000);
  digitalWrite(START_PIN, true);
  delay(5000);
  digitalWrite(FIRST_STAGE_PIN, true);
  delay(5000);
  digitalWrite(SECOND_STAGE_PIN, true);

  tone(9, 4000);
}

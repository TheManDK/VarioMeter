/*

 circuit:
 * 8-ohm speaker on digital pin 8

 */
 
#define SPEAKER_PIN 8

void setup() {
  
  Serial.begin(57600);
  Serial.write("Starting\n");
  noTone(8);
}
int rate = 0;
int rounds = 0;
const int waitingRounds = 100;
void loop() {
  
  while (Serial.available() > 0) 
  {
    rate = Serial.parseInt();
    Serial.write("read!\n");
  }
  int roundsToPlay = rateToRounds(rate);
  if (roundsToPlay == 0) {
    tone(SPEAKER_PIN, rateToFrequency(rate));
    rounds = 0;
    return;
  }
  if (rounds <= roundsToPlay) {
  
    tone(SPEAKER_PIN, rateToFrequency(rate));
  }
  else if (roundsToPlay != 0 && rounds < (roundsToPlay + waitingRounds)) {
    noTone(SPEAKER_PIN);
  }
  else {
    rounds = 0;
  }
  delay(1);
  rounds = rounds + 1;
  
}

int rateToFrequency(int rate) {
  return map(rate, -25, 25, 200, 3000);
}

int rateToRounds(int rate) {
  if (rate < 0) {
    return 0;
  }
  else {
    return map(rate, 0, 25, 700, 50);
  }
}

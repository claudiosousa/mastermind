#define DEBUG false

void setup() {
  initializeLeds();
  setupLeds();
  randomSeed(analogRead(0));
#if(DEBUG)
  setupComm();
#endif
  setupButons();
  initializeTimers();
}

void loop() {
  showNextRow();
  playNextTone();
  checkIfButtonPressed();
}





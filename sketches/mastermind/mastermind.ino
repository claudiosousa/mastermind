#define DEBUG false

void setup() {
  randomSeed(analogRead(0));

  setupLeds();
  initializeGame();

  initializeConfig();
  initializeSound();
  initializeGameSettings();
  
#if(DEBUG)
  setupComm();
#endif

  setupButons();
  initializeTimers();
  playStartSound();
}

void loop() {
  showNextRow();
  playNextTone();
  checkIfButtonPressed();
}





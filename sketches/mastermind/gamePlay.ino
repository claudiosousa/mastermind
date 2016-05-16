#define nocolor 0B000
#define color0 0B100
#define color1 0B010
#define color2 0B001
#define color3 0B101
#define color4 0B110
#define color5 0B111
#define maxTrials 10
#define secretLength 4

byte numberOfColors = 4;
byte repeatColors = 0;
  
byte colors[] = {
  color0, color1, color2, color3, color4, color5
};

byte board_secret[secretLength];
byte board_trial_colors[secretLength];
byte board_trial;
bool board_isInConfiguration = false;
bool board_isGameStarted = false;
bool board_isGameFinished = false;
bool board_isGameWon = false;

void initializeLeds() {
  resetSrs();
}

void newGame() {
  board_isGameFinished = false;
  board_isGameWon = false;
  board_isGameStarted = true;
  board_isInConfiguration = false;
  for (byte i = 0; i < secretLength; i++) {
    byte newColor = 0;
    byte colorIsUsed;
    do {
      colorIsUsed = 0;
      newColor = random(numberOfColors);
      if (!repeatColors) {
        for (byte j = 0; j < i; j++) {
          if (board_secret[j] == newColor) {
            colorIsUsed = true;
            break;
          }
        }
      }
    } while (colorIsUsed);
    board_secret[i] = newColor;
  }
  memset(board_trial_colors, 255, secretLength);

#if(DEBUG)
  Serial.print("Secret: ");
  for (byte i = 0; i < secretLength; i++) {
    if (i > 0)
      Serial.print(", ");
    Serial.print(String(board_secret[i]));
  }
  Serial.println();
#endif
  board_trial = 0;

  initializeLeds();
}

void modifyColorButton(byte buttonIndex) {
  if (board_isGameFinished)
    return;

  if (!board_isGameStarted)
    newGame();

  board_trial_colors[buttonIndex] += 1;
  byte newButtonColorIndex = board_trial_colors[buttonIndex] % numberOfColors;

#if(DEBUG)
  Serial.println("modifyColorButton: " + String(buttonIndex) + ", colorIndex: " + String(newButtonColorIndex));
#endif

  board_trial_colors[buttonIndex] = newButtonColorIndex;
  setRowPositionColor(board_trial, buttonIndex, colors[newButtonColorIndex]);
}

void verifyTrialCombination() {
  if (board_isGameFinished)
    return;

  for (byte i = 0; i < secretLength; i++) {
    if (board_trial_colors[i] == 255)
      return;
  }

  byte correctedPositions = 0;
  byte correctedPositionsCount = 0;
  byte nonFoundPositionsColors[6] = {  0, 0, 0, 0, 0, 0  };
  for (byte i = 0; i < secretLength; i++) {
    byte secretColorI = board_secret[i];
    if (board_trial_colors[i] == secretColorI) {
      correctedPositions |= (1 << i);
      correctedPositionsCount++;
    }
    else {
      nonFoundPositionsColors[secretColorI]++;
    }
  }
  byte rightColorsWrongPosition = 0;
  if (correctedPositionsCount < secretLength) {
    for (byte i = 0; i < secretLength; i++) {
      if ((correctedPositions & (1 << i)) > 0)
        continue;
      byte colorTrial = board_trial_colors[i];
      if (nonFoundPositionsColors[colorTrial] > 0) {
        nonFoundPositionsColors[colorTrial]--;
        rightColorsWrongPosition++;
      }
    }
  }
  else {
    board_isGameFinished = true;
    board_isGameWon = true;
  }

  byte colorValues[secretLength];
  for (byte i = 0; i < secretLength; i++)
    colorValues[i] = colors[board_trial_colors[i]];
  setTrialColorsAndResult(board_trial, correctedPositionsCount, rightColorsWrongPosition);

#if(DEBUG)
  Serial.println("setTrialResult -> Correct: " + String(correctedPositionsCount) + ", Wrong pos" + String(rightColorsWrongPosition));
#endif

  memset(board_trial_colors, 255, secretLength);

  board_trial++;
  if (board_trial == maxTrials)
    board_isGameFinished = true;

  if (!board_isGameFinished)
    playSubmitMoveSound();
  else {
    if (board_isGameWon) {
    } else
      playGameOverSound();
  }
}









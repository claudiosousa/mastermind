
void changeNbColorsButton() {
  if (board_isGameStarted)
    return;
  if (!board_isInConfiguration) {
    showCurrentConfiguration();
    return;
  }
  numberOfColors++;
  if (numberOfColors > 6)
    numberOfColors = 4;

  displayNbColors();
}

void changeRepeatColorsButton() {
  if (board_isGameStarted)
    return;

  if (!board_isInConfiguration) {
    showCurrentConfiguration();
    return;
  }
  repeatColors = !repeatColors;
  displayColorsRepeation();
}

byte repeatColor[] = {
  0b00001110,
  0b00001001,
  0b00001110,
  0b00001001
};

void displayNbColors() {
  for (int r = 0; r < 6; r++) {
    byte color = numberOfColors <= r ? nocolor : colors[r] ;
    for (int c = 0; c < 4; c++) {
      setRowPositionColor(r, c, color);
    }
  }
}

void displayColorsRepeation() {
  for (int r = 0; r < 4; r++) {
    byte row = repeatColor[r];
    for (int c = 0; c < 4; c++) {
      byte color = repeatColors && (row & (1 << (3-c))) ? colors[0] : nocolor;
      setRowPositionColor(maxTrials - r - 1, c, color);
    }
  }
}

void showCurrentConfiguration() {
  displayNbColors();
  displayColorsRepeation();
  board_isInConfiguration = true;
}



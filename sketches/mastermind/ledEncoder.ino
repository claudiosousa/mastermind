#define SrsCount 4
#define led_latchPin 2
#define led_clockPin 3
#define led_dataPin 4

#define led_LATCH_HIGH sbi(PORTD, led_latchPin)
#define led_LATCH_LOW cbi(PORTD, led_latchPin)
#define led_DATA_HIGH sbi(PORTD, led_dataPin)
#define led_DATA_LOW cbi(PORTD, led_dataPin)
#define led_CLOCK_HIGH sbi(PORTD, led_clockPin)
#define led_CLOCK_LOW cbi(PORTD, led_clockPin)


const byte colorLedsMapping[] = {
  /* Mapping breadboard
    19, 18, 17,
    16, 15, 14,
    13, 12, 11,
    10, 9, 8
  */
  24, 25 , 27,
  28, 17 , 21,
  8, 11 , 15,
  1, 3 , 5
};



const byte resultLedsMapping[] = {
  /* Mapping breadboard
    7, 6, 5, 4,
    3, 2, 1, 0
  */
  31, 18,
  12, 23,
  30, 19,
  13, 20
};

const byte nbOfRows = 11; //including trial row
const byte rowsBits[nbOfRows] = {
   /* Mapping breadboard
  23, 22, 20, 21, 25, 26, 27, 28, 29, 30, 31 */
  4, 2, 0, 14, 10, 9, 22, 16, 29, 26
};



byte emptyArray[SrsCount] = {0, 0, 0, 0};
byte rowsOutputBuffer[nbOfRows][SrsCount];

void resetSrs() {
  for (int i = 0; i < nbOfRows; i++) {
    for (int j = 0; j < SrsCount; j++)
      rowsOutputBuffer[i][j] = 0;

    setRowBitValue(i, rowsBits[i], true);
  }
}

// each row is a result row with exception of the first row which is the trial row
void setRowBitValue(byte row, byte bit, bool isOn) {
  byte* rowOutputBuffer = rowsOutputBuffer[row];
  byte ibyte = bit / 8;
  byte ibit = bit % 8;
  if (isOn)
    rowOutputBuffer[ibyte] |= 1 << ibit;
  else
    rowOutputBuffer[ibyte] &= ~(1 << ibit);
}

void setRowPositionColor(byte row, byte pos, byte color) {
  byte redPosition = pos * 3 ;
  for (byte i = 0; i < 3; i++) {
    setRowBitValue(row, colorLedsMapping[redPosition + i], (color & (1 << (2 - i))));
  }
}

void setTrialColorsAndResult(byte row, byte righPosition, byte rightColorWrongPos) {
  for (byte i = 0; i < righPosition; i++) {
    setRowBitValue(row, resultLedsMapping[i * 2 + 1], true); // +1 == blue
  }

  for (byte i = 0; i < rightColorWrongPos; i++) {
    setRowBitValue(row, resultLedsMapping[(i + righPosition) * 2] , true);
  }
}


byte  currentRowBeeingShow = -1;
volatile bool shouldShowNextRow = false;
void showNextRow() {

  if (!shouldShowNextRow)
    return;
  shouldShowNextRow = false;

  currentRowBeeingShow = (currentRowBeeingShow + 1) % nbOfRows;
  writeBytes(emptyArray);
  writeBytes(rowsOutputBuffer[currentRowBeeingShow]);
}

void ISR_showNextRow() {
  shouldShowNextRow = true;
}


void setupLeds() {
  pinMode(led_latchPin, OUTPUT);
  pinMode(led_clockPin, OUTPUT);
  pinMode(led_dataPin, OUTPUT);

}

void writeBytes(byte* bytes2write) {
  led_LATCH_LOW;
  for (int i = SrsCount - 1; i >= 0 ; i--) {
    //shiftOut(led_dataPin, led_clockPin, MSBFIRST,  bytes2write[i]);
    byte byte2write = bytes2write[i];
    for  (int j = 7; j >= 0; j--) {
      led_CLOCK_LOW;
      if (byte2write & (1 << j))
        led_DATA_LOW;
      else
        led_DATA_HIGH;

      led_CLOCK_HIGH;
    }
  }
  led_LATCH_HIGH;
}


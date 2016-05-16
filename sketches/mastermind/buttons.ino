/*
  IC  pins:
  1-> parallel load
  2-> clock
  8-> gnd
  9-> data
  15-> gnd (CE)
  16->vcc

  Parallel in into to IC pin:
  0-> 11
  1-> 12
  2-> 13
  3-> 14
  4-> 3
  5-> 4
  6-> 5
  7-> 6
*/

#define btn_ploadPin 7
#define btn_dataPin 5
#define btn_clockPin 6

#define btn_LATCH_HIGH sbi(PORTD, btn_ploadPin)
#define btn_LATCH_LOW cbi(PORTD, btn_ploadPin)
#define btn_DATA_READ PIND & 1<<btn_dataPin
#define btn_CLOCK_HIGH sbi(PORTD, btn_clockPin)
#define btn_CLOCK_LOW cbi(PORTD, btn_clockPin)


const byte dataIn2check[] = { 0, 1, 2, 3, 4, 5, 6, 7}; //upload
byte dataIn2checkMask = 0;

void setupButons() {
  pinMode(btn_ploadPin, OUTPUT);
  pinMode(btn_clockPin, OUTPUT);
  pinMode(btn_dataPin, INPUT);

  btn_CLOCK_LOW;
  btn_LATCH_HIGH;

  for (byte i = 0; i < sizeof(dataIn2check) / sizeof(byte); i++)
    dataIn2checkMask |= 1 << dataIn2check[i];
}
byte previousPinValues = 0;
volatile bool shouldCheckForButtonPress = false;
void checkIfButtonPressed() {
  if (!shouldCheckForButtonPress)
    return;
  shouldCheckForButtonPress = false;
  byte pinValues = readButtonsByte();

  pinValues &= dataIn2checkMask;

  if (pinValues == previousPinValues)
    return;

  previousPinValues = pinValues;

  if (pinValues == 0)
    return;

  for (byte i = 0; i < sizeof(dataIn2check) / sizeof(byte); i++)
  {
    byte iIn = dataIn2check[i];
    if ((pinValues & (1 << iIn)) == 0)
      continue;

    if (i < 4)
      modifyColorButton(i);
    else {
      switch (i) {
        case 4:
          verifyTrialCombination();
          break;
        case 5:
          changeRepeatColorsButton();
          break;
        case 6:
          changeNbColorsButton();
          break;
        case 7:
          soundEnabledButton();
          break;
      }
    }
    
    return;
  }
}

void ISR_checkIfButtonPressed() {
  shouldCheckForButtonPress = true;
}

byte readButtonsByte() {
  btn_LATCH_LOW;
  //delayMicroseconds(5);
  btn_LATCH_HIGH;
  // delayMicroseconds(5);

  byte byteRes = 0;
  for (byte i = 0; i < 8; i++)
  {
    if (btn_DATA_READ)      {
      byteRes |= 1 << (7 - i);
      return byteRes;
    }
    btn_CLOCK_HIGH;
    // delayMicroseconds(20);
    btn_CLOCK_LOW;
  }
  return byteRes;
}

















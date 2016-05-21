#include <EEPROM.h>

int address = 200;
byte config = 0;

void initializeConfig() {
  config = EEPROM.read(address);
}

bool setSoundConfig(bool sound) {
  if (sound)
    sbi(config, 7);
  else
    cbi(config, 7);

  EEPROM.write(address, config);
}


bool setColorsConfig(byte nbColors, bool repeatColors) {
  config &= 0b11111000;
  config |= nbColors;
  if (repeatColors)
    sbi(config, 6);
  else
    cbi(config, 6);
    
  EEPROM.write(address, config);
}

bool getSoundConfig() {
  return config & 0b10000000;
}

byte getNbColorsConfig() {
  return config & 0b00000111;
}

bool getRepeatColorsConfig() {
  return config & 0b01000000;
}


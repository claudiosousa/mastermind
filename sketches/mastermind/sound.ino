#include <toneAC.h>

typedef struct {
  int length;
  float * notes;
  int * durations;
} Sound;

/*
  float gameOverSoundNotes[] = {  note_Bb5, note_C6, note_Ab5, note_Ab4, note_Eb5, note_Bb4, note_C5,  note_Ab4, note_Ab3, note_Eb4, note_Bb3, note_C4,  note_Ab3, note_Ab2, note_Eb3};
  int gameOverSoundDurations[] = {300, 50,  300, 50, 300, 50,  300, 50,  500, 500, 300, 100, 300, 100, 300, 100, 300, 100, 500, 500, 300, 200, 300, 200, 300, 500, 300, 550, 500};
*/

float gameOverSoundNotes[] = {  note_B3, note_F4, note_F4, note_F4, note_E4, note_D4,  note_C4, note_E3, note_E3, note_C3};
int gameOverSoundDurations[] = {162, 5,  162, 162,  162, 5,  217, 5, 217, 5, 217, 5, 162, 5, 162, 162, 162, 5, 162, 5};
Sound gameOverSound =  {.length = sizeof(gameOverSoundNotes) / sizeof(float), .notes = gameOverSoundNotes, .durations = gameOverSoundDurations};


float submitMoveSoundNotes[] =  { 300,   435,   631,   915,   1326,  1923};
int submitMoveSoundDurations[] = {60, 0, 60, 0, 60, 0, 60, 0, 60, 0, 60, 0};
Sound submitMoveSound =  {.length = sizeof(submitMoveSoundNotes) / sizeof(float), .notes = submitMoveSoundNotes, .durations = submitMoveSoundDurations};

float startSoundNotes[] =  { note_E5, note_G5, note_E6, note_C6, note_D6, note_G6};
int startSoundDurations[] = {130, 5,  130, 5,  130, 5,  130, 5,  130, 5,  130};
Sound startSound =  {.length = sizeof(startSoundNotes) / sizeof(float), .notes = startSoundNotes, .durations = startSoundDurations};

float winSoundNotes[] =  { note_G2, note_C3, note_E3, note_G3, note_C4, note_E4, note_G4, note_E4, note_Ab2, note_C3, note_Eb3, note_Ab3, note_C4, note_Eb4,
                           note_Ab4, note_Eb4, note_Bb2, note_D3, note_F3, note_Bb3, note_D4, note_F4, note_Bb4, note_B4, note_B4, note_B4, note_C5
                         };
int winSoundDurations[] = {130, 2,  130, 2,  130, 2,  130, 2,  130, 2,  130, 2,  433, 2,  433, 2,  130, 2,   130, 2,  130, 2,   130, 2,   130, 2,  130, 5,
                           433, 2,  433, 2,  130, 2,  130, 2,  130, 2,  130, 2,  130, 2,  130, 2,  433, 2,   130, 2,  130, 2,   130, 2,   650
                          };
Sound winSound =  {.length = sizeof(winSoundNotes) / sizeof(float), .notes = winSoundNotes, .durations = winSoundDurations};


float soundOnNotes[] =  { note_C5, note_Bb5};
int soundOnDurations[] = {30, 20,  70, 2};
Sound soundOnSound =  {.length = sizeof(soundOnNotes) / sizeof(float), .notes = soundOnNotes, .durations = soundOnDurations};

float soundOffNotes[] =  {note_Bb5, note_C5, };
int soundOffDurations[] = {30, 20,  70, 2};
Sound soundOffSound =  {.length = sizeof(soundOffNotes) / sizeof(float), .notes = soundOffNotes, .durations = soundOffDurations};

Sound * currentSound = NULL;

byte nextNote = 0;
unsigned long nextToneTime;
byte soundIsEnabled = 1;

void initializeSound(){
  soundIsEnabled = getSoundConfig();
}

void resetSound() {
  toneAC(0);
  nextNote = 0;
  nextToneTime = 0;
}

void soundEnabledButton() {
  soundIsEnabled = !soundIsEnabled;
  setSoundConfig(soundIsEnabled);
  resetSound();
  currentSound = NULL;
  if (soundIsEnabled)
    playSoundOnSound();
  else
    playSoundOffSound();
}

void playStartSound() {
  if (!soundIsEnabled)
    return;
  currentSound = &startSound;
  resetSound();
}

void playSoundOnSound() {
  currentSound = &soundOnSound;
  resetSound();
}

bool isSoundPlaying() {
  return currentSound != NULL;
}

void playSoundOffSound() {
  currentSound = &soundOffSound;
  resetSound();
}

void playWinSound() {
  if (!soundIsEnabled)
    return;
  currentSound = &winSound;
  resetSound();
}

void playSubmitMoveSound() {
  if (!soundIsEnabled)
    return;
  currentSound = &submitMoveSound;
  resetSound();
}

void playGameOverSound() {
  if (!soundIsEnabled)
    return;
  currentSound = &gameOverSound;
  resetSound();
}

void playNextTone() {
  if (!currentSound)
    return;
  if (millis() < nextToneTime)
    return;
  if (nextNote >= currentSound->length * 2) {
    currentSound = NULL;
    resetSound();
  } else {
    float f = 0;
    if (!(nextNote % 2))
      f = currentSound->notes[nextNote / 2];
    toneAC(f);
    nextToneTime = millis() + currentSound->durations[nextNote];
    nextNote++;
  }
}



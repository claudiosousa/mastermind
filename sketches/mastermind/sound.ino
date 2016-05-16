#include <toneAC.h>

typedef struct {
  int length;
  float * notes;
  int * durations;
} Sound;


float gameOverSoundNotes[] = {  note_Bb5, note_C6, note_Ab5, note_Ab4, note_Eb5, note_Bb4, note_C5,  note_Ab4, note_Ab3, note_Eb4, note_Bb3, note_C4,  note_Ab3, note_Ab2, note_Eb3};
int gameOverSoundDurations[] = {300, 50,  300, 50, 300, 50,  300, 50,  500, 500, 300, 100, 300, 100, 300, 100, 300, 100, 500, 500, 300, 200, 300, 200, 300, 500, 300, 550, 500};
Sound gameOverSound =  {.length = sizeof(gameOverSoundNotes) / sizeof(float), .notes = gameOverSoundNotes, .durations = gameOverSoundDurations};

float submitMoveSoundNotes[] =  { 300,   435,   631,   915,   1326,  1923};
int submitMoveSoundDurations[] = {60, 0, 60, 0, 60, 0, 60, 0, 60, 0, 60};
Sound submitMoveSound =  {.length = sizeof(submitMoveSoundNotes) / sizeof(float), .notes = submitMoveSoundNotes, .durations = submitMoveSoundDurations};

Sound * currentSound = NULL;

byte nextNote = 0;
unsigned long nextToneTime;
byte soundIsEnabled = 1;

void resetSound() {
  toneAC(0);
  nextNote = 0;
  nextToneTime = 0;
}

void soundEnabledButton() {
  soundIsEnabled = !soundIsEnabled;
  if (!soundIsEnabled) {
    resetSound();
    currentSound = NULL;
  }
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
  if (!soundIsEnabled)
    return;
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



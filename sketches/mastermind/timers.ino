//http://www.instructables.com/id/Arduino-Timer-Interrupts/

#define BUTTONS_FREQUENCY_DIVIDER 100


//100HZ * 12 rows  (3ms)
void initializeTimer2() {
  TCCR2A = 0; //reset timer2 config register
  TCCR2B = 0; //reset timer2 config register
  TCNT2  = 0; //reset timer2 counter register

  OCR2A = 60; //(t=(x+1)*16μs)100Hz=155, 200Hz=77 61HZ=255 (must be <=255)
  TCCR2A |= (1 << WGM21);   // CTC mode
  //  TCCR2B |= (1 << CS22) | (1 << CS21)| (1 << CS20);    // 1024 prescaler  (64μs tick)
  TCCR2B |= (1 << CS22) | (1 << CS21);    // 256 prescaler  (16μs tick)
  TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt
}

/*
  //20HZ (50ms)
  void  initializeTimer1(){
  TCCR1A = 0; //reset timer1 config register
  TCCR1B = 0; //reset timer1 config register
  TCNT1  = 0; //reset timer1 counter register
  OCR1A = 781;// (t=(x+1)*64μs)100Hz=155, 200Hz=77 61HZ=255 10HZ=1562 (must be <=65535)
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10);   // Set CS12 and CS10 bits for 1024 prescaler
  TIMSK1 |= (1 << OCIE1A);   // enable timer compare interrupt
  }
*/

void initializeTimers()
{
  cli();           // disable all interrupts

  initializeTimer2();
  //initializeTimer1();

  sei();             // enable all interrupts
}
/*
  //Buttons timer
  ISR(TIMER1_COMPA_vect)
  {
  ISR_checkIfButtonPressed();
  }
*/

volatile int buttonsFrequencyCounter = 0;

//Leds timer
ISR(TIMER2_COMPA_vect)
{
  buttonsFrequencyCounter++;
  if (buttonsFrequencyCounter == BUTTONS_FREQUENCY_DIVIDER) {
    ISR_checkIfButtonPressed();
    buttonsFrequencyCounter = 0;
  }
  ISR_showNextRow();
}



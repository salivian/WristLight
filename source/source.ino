/*
  Blink
 Turns on an LED on for one second, then off for one second, repeatedly.
 
 This example code is in the public domain.
 */

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

int ACC_X = A0;
int ACC_Y = A1;
int ACC_Z = A2;

int bar[4][7] = {
{1,1,1,1,1,1,1},
{0,1,1,1,1,1,0},
{0,0,1,1,1,0,0},
{0,0,0,1,0,0,0}
};

int bars[7] = {0,1,2,3,2,1,0};

int irFreq = 109286;

// the setup routine runs once when you press reset:
void setup() {                
  Serial.begin(9600);

  // initialize the digital pins as output
  for(int i=0; i<=13; i++) pinMode(i,OUTPUT); 

  // initialize timer1
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = irFreq; // preload timer 65536-16MHz/256/2Hz
  TCCR1B |= (1 << CS12); // 256 prescaler
  TIMSK1 |= (1 << TOIE1); // enable timer overflow interrupt
  interrupts(); // enable all interrupts
}

// the loop routine runs over and over again forever:
void loop() {
  /*  for(int i=0;i<8;i++) {
   delay(1000);
   digitalWrite(i, LOW);
   }*/
  // for each column...
  for(int i=0;i<7;i++) {
    // set column state
    for(int i2=0; i2<7; i2++) { if(i!=i2) digitalWrite(i2,LOW); }
    digitalWrite(i, HIGH);
    // set row state
    for(int j=0;j<7;j++){
      digitalWrite(j+7, !bar[bars[i]][j]);
    }
   delay(2);
  }
  /*
  int x = analogRead(ACC_X);
  int y = analogRead(ACC_Y);
  int z = analogRead(ACC_Z);

  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(z);
  Serial.println("");*/
  //  reset();
  //  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  //  delay(1000);               // wait for a second
}


ISR(TIMER1_OVF_vect) {
  TCNT1 = irFreq; // preload timer
  // update bars
  for(int j=0;j<7;j++){
    int x=bars[j];
    if(x==0) x=3; else x-=1;
    bars[j]=x;
  }
  
}

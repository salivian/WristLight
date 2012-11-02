int ACC_X = A0;
int ACC_Y = A1;
int ACC_Z = A2;
int PRESSURE = A3; // Let's hope this works...

// 0: center bar
// 1: long bar
// 2: pattern
int visType = 2;

// 0: increment bars, wrap at corner
// 1: time flow (from left to right)
// 2: flip
int animType = 2;

int bar[4][7] = {
  {1,1,1,1,1,1,1},
  {0,1,1,1,1,1,0},
  {0,0,1,1,1,0,0},
  {0,0,0,1,0,0,0}
};

#define PAT_HEART 0
#define PAT_RIGHT 1
#define PAT_LEFT  2
#define PAT_UP    3
#define PAT_DOWN  4

int patternList[5][7][7] = 
  {
    { // heart
	  {0,1,1,0,1,1,0},
	  {1,1,1,1,1,1,1},
	  {1,1,1,1,1,1,1},
	  {0,1,1,1,1,1,0},
	  {0,1,1,1,1,1,0},
	  {0,0,1,1,1,0,0},
	  {0,0,0,1,0,0,0}
	},
	{ // right
	  {0,0,0,1,0,0,0},
	  {0,0,1,1,1,0,0},
	  {0,1,1,1,1,1,0},
	  {1,1,1,1,1,1,1},
	  {0,0,1,1,1,0,0},
	  {0,0,1,1,1,0,0},
	  {0,0,1,1,1,0,0}
	},
	{ // left
	  {0,0,1,1,1,0,0},
	  {0,0,1,1,1,0,0},
	  {0,0,1,1,1,0,0},
	  {1,1,1,1,1,1,1},
	  {0,1,1,1,1,1,0},
	  {0,0,1,1,1,0,0},
	  {0,0,0,1,0,0,0}
	},
	{ // up
	  {0,0,0,1,0,0,0},
	  {0,0,1,1,0,0,0},
	  {0,1,1,1,1,1,1},
	  {1,1,1,1,1,1,1},
	  {0,1,1,1,1,1,1},
	  {0,0,1,1,0,0,0},
	  {0,0,0,1,0,0,0}
	},
	{ // down
	  {0,0,0,1,0,0,0},
	  {0,0,0,1,1,0,0},
	  {1,1,1,1,1,1,0},
	  {1,1,1,1,1,1,1},
	  {1,1,1,1,1,1,0},
	  {0,0,0,1,1,0,0},
	  {0,0,0,1,0,0,0}
	}
  };

// empty pattern, will need to init it to something
int pattern[7][7] = {
  {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0},
};

void initPattern(int t){
	for(int i=0; i<7 ; i++){
		for(int j=0; j<7 ; j++){
			pattern[i][j] = patternList[t][i][j];
		}
	}
}

int barLong[7][7] = {
  {1,0,0,0,0,0,0},
  {1,1,0,0,0,0,0},
  {1,1,1,0,0,0,0},
  {1,1,1,1,0,0,0},
  {1,1,1,1,1,0,0},
  {1,1,1,1,1,1,0},
  {1,1,1,1,1,1,1}
};

// the state of each 7 bars
//int bars[7] = {0,1,2,3,2,1,0};
int bars[7] = {0,2,0,2,0,2,3};

// animation updatefrequency
int irFreq = 109286;

int mapCol[7] = {1,3,5,7,12,10,8};
int mapRow[7] = {0,2,4,6,13,11,9};

void setup() {                
//  Serial.begin(9600);
  
  initPattern(PAT_LEFT);

  // initialize all digital pins as output
  for(int i=0; i<=13; i++) pinMode(i,OUTPUT); 

  // initialize timer1
  noInterrupts(); // disable all interrupts
  TCCR1A = TCCR1B = 0;
  TCNT1 = irFreq; // preload time
  TCCR1B |= (1 << CS12); // 256 prescaler
  TIMSK1 |= (1 << TOIE1); // enable timer overflow interrupt
  interrupts(); // enable all interrupts
  
}

void loop() {
  // refresh whole screen: for each column...
  for(int i=0;i<7;i++) {
	// column : low / row : high => light up!
	for(int j=0;j<7;j++){digitalWrite(mapCol[j], HIGH);}
    for(int j=0;j<7;j++){digitalWrite(mapRow[j], LOW);}
//	delay(1);
    // set column state (only column i is on, rest is off!
	digitalWrite(mapRow[i], HIGH);
	
    for(int j=0;j<7;j++){
	  int t=bars[i];
	  if(visType==0) digitalWrite(mapCol[j], !bar[t][j]);
	  if(visType==1) digitalWrite(mapCol[j], !barLong[t][j]);
	  if(visType==2) digitalWrite(mapCol[j], !pattern[i][j]);
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
  Serial.println(z);*/
  //  reset();
  //  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  //  delay(1000);               // wait for a second
}


ISR(TIMER1_OVF_vect) {
  TCNT1 = irFreq; // preload timer
  // update bars
  if(animType==0){
	  for(int j=0;j<7;j++){
		int x=bars[j];
		if(visType==0) { if(x==3) x=0; else x+=1; }
		if(visType==1) { if(x==6) x=0; else x+=1; }
		bars[j]=x;
	  }
  } else if(animType==1){
	  for(int j=0;j<6;j++){
		bars[j]=bars[j+1];
	  }
	  if(visType==0) bars[6] = random(4);
	  if(visType==1) bars[6] = random(7);
  } else if(animType==2){
      for(int i=0;i<7;i++){
	    for(int j=0;j<7;j++){
		  int old = pattern[i][j];
		  if(old==0) pattern[i][j] = 1;
		  else       pattern[i][j] = 0;
	    }
	  }
  }
}

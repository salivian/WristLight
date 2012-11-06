//#define DEBUG_SERIAL
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
// 3: none
// 4: expand
int animType;

int bar[4][8] = {
  {
    1,1,1,1,1,1,1,1      }
  ,
  {
    0,1,1,1,1,1,0,1      }
  ,
  {
    0,0,1,1,1,0,0,1      }
  ,
  {
    0,0,0,1,0,0,0,1      }
};
int barLong[8][8] = {
  {
    1,0,0,0,0,0,0,0      }
  ,
  {
    1,1,0,0,0,0,0,0      }
  ,
  {
    1,1,1,0,0,0,0,0      }
  ,
  {
    1,1,1,1,0,0,0,0      }
  ,
  {
    1,1,1,1,1,0,0,0      }
  ,
  {
    1,1,1,1,1,1,0,0      }
  ,
  {
    1,1,1,1,1,1,1,0      }
  ,
  {
    1,1,1,1,1,1,1,1      }
};

// the state of each 7 bars
//int bars[7] = {0,1,2,3,2,1,0};
int bars[8] = {
  0,2,0,2,0,2,3,3};

#define PAT_HEART 0
#define PAT_RIGHT 1
#define PAT_LEFT  2
#define PAT_UP    3
#define PAT_DOWN  4
#define PAT_XPAND_0 5
#define PAT_XPAND_1 6
#define PAT_XPAND_2 7
#define PAT_XPAND_3 8
int activePattern=PAT_HEART;

boolean revert;

int patterns[9][8][8] = 
{
  { // heart
    {
      0,0,0,0,0,1,1,0            }
    ,
    {
      0,0,0,1,1,1,1,1            }
    ,
    {
      0,1,1,1,1,1,1,1            }
    ,
    {
      1,1,1,1,1,1,1,0            }
    ,
    {
      1,1,1,1,1,1,1,0            }
    ,
    {
      0,1,1,1,1,1,1,1            }
    ,
    {
      0,0,0,1,1,1,1,1            }
    ,
    {
      0,0,0,0,0,1,1,0            }
  }
  ,
  { // right
    {
      0,0,0,1,1,0,0,0            }
    ,
    {
      0,0,1,1,1,1,0,0            }
    ,
    {
      0,1,1,1,1,1,1,0            }
    ,
    {
      1,1,1,1,1,1,1,1            }
    ,
    {
      0,0,1,1,1,1,0,0            }
    ,
    {
      0,0,1,1,1,1,0,0            }
    ,
    {
      0,0,1,1,1,1,0,0            }
    ,
    {
      0,0,1,1,1,1,0,0            }
  }
  ,
  { // left
    {
      0,0,1,1,1,1,0,0            }
    ,
    {
      0,0,1,1,1,1,0,0            }
    ,
    {
      0,0,1,1,1,1,0,0            }
    ,
    {
      0,0,1,1,1,1,0,0            }
    ,
    {
      1,1,1,1,1,1,1,1            }
    ,
    {
      0,1,1,1,1,1,1,0            }
    ,
    {
      0,0,1,1,1,1,0,0            }
    ,
    {
      0,0,0,1,1,0,0,0            }
  }
  ,
  { // up
    {
      0,0,0,1,0,0,0,0            }
    ,
    {
      0,0,1,1,0,0,0,0            }
    ,
    {
      0,1,1,1,1,1,1,1            }
    ,
    {
      1,1,1,1,1,1,1,1            }
    ,
    {
      1,1,1,1,1,1,1,1            }
    ,
    {
      0,1,1,1,1,1,1,1            }
    ,
    {
      0,0,1,1,0,0,0,0            }
    ,
    {
      0,0,0,1,0,0,0,0            }
  }
  ,
  { // down
    {0,0,0,0,1,0,0,0},
    {0,0,0,0,1,1,0,0},
    {1,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,0},
    {0,0,0,0,1,1,0,0},
    {0,0,0,0,1,0,0,0}
  },
  { // expand-0
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
  },
  { // expand-1
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
  },
  { // expand-2
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0}
  },
  { // expand-3
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,1,1,0,0,0}
  }
};

// animation updatefrequency
int irFreq = 109286;

int mapCol[8] = {1,3,5,7,12,10,8,A5};
int mapRow[8] = {0,2,4,6,13,11,9,A4};

void setup() {                
#if defined DEBUG_SERIAL
  Serial.begin(9600);
#endif

  // initialize all digital pins as output
  for(int i=0; i<=13; i++) pinMode(i,OUTPUT); 
  pinMode(A4,OUTPUT); 
  pinMode(A5,OUTPUT); 

  // initialize timer1
  noInterrupts(); // disable all interrupts
  TCCR1A = TCCR1B = 0;
  TCNT1 = irFreq; // preload time
  TCCR1B |= (1 << CS12); // 256 prescaler
  TIMSK1 |= (1 << TOIE1); // enable timer overflow interrupt
  interrupts(); // enable all interrupts
}

int lastdir = 0;
int counter=0;
int countdown=0;

void loop() {
  if(countdown==0 && animType!=4){
    activePattern = PAT_XPAND_0;
	revert = false;
	animType = 4;
  } else {
    countdown--;
  }
  //detect dir abd load the right pattern
  counter++;
  if( counter%10==0 || true ){
    int pressure = analogRead(PRESSURE);
	if(pressure>700){
	  animType = 2; // start blinking animation
	  if(activePattern!=PAT_HEART){
        activePattern = PAT_HEART; // change pattern
	    TCNT1 = irFreq*0.2;
	  }
	  countdown = 20;
	}
    int thisdir = detectDir();
//    int rot = detectRot();
//    int change = detectChange();

#if defined DEBUG_SERIAL
    Serial.print(getAvgx());
	Serial.print("-");
    Serial.print(getAvgy());
	Serial.print("-");
    Serial.println(getAvgz());
    switch(thisdir){
    case 0:  Serial.println("U"); break;
    case 1:  Serial.println("R"); break;
    case 2:  Serial.println("D"); break;
    case 3:  Serial.println("L"); break;
    }
#endif
    if(thisdir != lastdir){ //dir change
     switch(thisdir){
     	  case 0: activePattern = PAT_DOWN; break;
     	  case 1: activePattern = PAT_RIGHT; break;
     	  case 2: activePattern = PAT_UP; break;
     	  case 3: activePattern = PAT_LEFT; break;
     	  }
     lastdir = thisdir; //update lastdir
     TCNT1 = irFreq*0.5; // preload time
     countdown = 200;
	 animType=2;
     }
  }

  // ************** REFRESH DISPLAY ****************************
  // refresh whole screen: for each column...
  for(int i=0;i<8;i++) {
    // column : low / row : high => light up!
    for(int j=0;j<8;j++) digitalWrite(mapCol[j], HIGH);
    // set column state (only column i is on, rest is off!
    for(int j=0;j<8;j++) digitalWrite(mapRow[j], j==i);

    for(int j=0;j<8;j++){
      switch(visType) {
      case 0: 
        digitalWrite(mapCol[j], !bar[bars[i]][j]); 
        break;
      case 1: 
        digitalWrite(mapCol[j], !barLong[bars[i]][j]); 
        break;
      case 2:
	    if(revert)
           digitalWrite(mapCol[j], patterns[activePattern][i][j]); 
		else 
		   digitalWrite(mapCol[j], !patterns[activePattern][i][j]); 
        break;
      }
    }
    delay(2);
  }
}

void updateAnim(){
  // no animation
  if(animType==3) return;
  // update bars
  if(animType==0){
    for(int j=0;j<7;j++){
      int x=bars[j];
      if(visType==0) { 
        if(x==3) x=0; 
        else x+=1; 
      }
      if(visType==1) { 
        if(x==6) x=0; 
        else x+=1; 
      }
      bars[j]=x;
    }
  } 
  else if(animType==1){
    for(int j=0;j<7;j++){
      bars[j]=bars[j+1];
    }
    if(visType==0) bars[7] = random(4);
    if(visType==1) bars[7] = random(7);
  } else if(animType==2){
    if(revert==false) revert=true; else revert=false;
	TCNT1 = irFreq*0.5;
  } else if(animType==4){
    switch(activePattern){
    case PAT_XPAND_0: activePattern=PAT_XPAND_1; TCNT1 = irFreq*0.12; break;
    case PAT_XPAND_1: activePattern=PAT_XPAND_2; TCNT1 = irFreq*0.2; break;
    case PAT_XPAND_2: activePattern=PAT_XPAND_3; TCNT1 = irFreq*0.6; break;
    case PAT_XPAND_3: activePattern=PAT_XPAND_0; TCNT1 = irFreq*0.12; break;
    }
  }
}

ISR(TIMER1_OVF_vect) {
  TCNT1 = irFreq; // preload timer
  updateAnim();
}



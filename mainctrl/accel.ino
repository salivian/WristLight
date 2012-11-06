// Simple angle meter using ADXL335 accelerometer
//from electronicsblog.net/

#define ADC_ref 3.3
#define zero_x 1.569
#define zero_y 1.569
#define zero_z 1.569
#define sensitivity_x 0.3
#define sensitivity_y 0.3
#define sensitivity_z 0.3

#define MULT_OLD (0.3)
#define MULT_NEW (0.7)
#define ROT_THRES (10)

unsigned int value_x;
unsigned int value_y;
unsigned int value_z;
float xv;
float yv;
float zv;
float avgx=0;
float avgy=0;
float avgz=0;


void readAccel(){
  value_x = analogRead(0);
  value_y = analogRead(1);
  value_z = analogRead(2);
  xv=(value_x/1024.0*ADC_ref-zero_x)/sensitivity_x;
  yv=(value_y/1024.0*ADC_ref-zero_y)/sensitivity_y;
  zv=(value_z/1024.0*ADC_ref-zero_z)/sensitivity_z;
  angleX();
  angleY();
  angleZ();
}

float angleX(){
  float ax = atan2(-yv,-zv)*57.2957795+180;
  avgx = ax*MULT_OLD+avgx*MULT_NEW;
  return ax;
}

float angleY(){
  float ay = atan2(-xv,-zv)*57.2957795+180;
  avgy = ay*MULT_OLD+avgy*MULT_NEW;
  return ay;
}

float angleZ(){
  float az = atan2(-yv,-xv)*57.2957795+180;
  avgz = az*MULT_OLD+avgz*MULT_NEW;
  return az;
}

int detectDir(){
  readAccel();
  if (abs(avgz-270) < 45){
    return 0;
  }  
  else if(abs(avgz-360) < 45 || abs (avgz-0) < 45){
    return 1;
  }    
  else if(abs(avgz-90) < 45){
    return 2;
  }    
  else if(abs(avgz-180) < 45){
    return 3;
  }    
}

float last_avgx=0;
float last_avgy=0;
float last_avgz=0;
float diff;

int detectRot(){
  readAccel();
  diff=avgy-last_avgy;
  last_avgy=avgy;

  if(abs(diff) > 180){ //pass 0 deg
    diff +=360;
  }
  
  if(diff>-20 && diff<-10){
    return 2;
  }
  if(diff>10 && diff<20){
    return 2;
  }
  
/*
  if(abs(diff > ROT_THRES) ){
    return 1;  //right    
  }
  
  if(abs(diff < -ROT_THRES) ){
    return 2;  //left    
  }
*/  
  return 0;
}

#define CHANGE_THRES 100

int detectChange(){
  readAccel();
  float dx = avgx-last_avgx;
  float dy = avgy-last_avgy;
  float dz = avgz-last_avgz;
  if((dx*dx+dy*dy+dz*dz) > CHANGE_THRES){
    return 1;
  }
  return 0;
}


float getAvgx() { return avgx; }
float getAvgy() { return avgy; }
float getAvgz() { return avgz; }
float getDiff() { return diff; }

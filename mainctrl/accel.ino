// #define DEBUG_SERIAL

// Simple angle meter using ADXL335 accelerometer
//from electronicsblog.net/

#define ADC_ref 3.3
#define zero_x 1.569
#define zero_y 1.569
#define zero_z 1.569
#define sensitivity_x 0.3
#define sensitivity_y 0.3
#define sensitivity_z 0.3

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
  avgx = ax*0.1+avgx*0.9;
  return ax;
}

float angleY(){
  float ay = atan2(-xv,-zv)*57.2957795+180;
  avgy = ay*0.1+avgy*0.9;
  return ay;
}

float angleZ(){
  float az = atan2(-yv,-xv)*57.2957795+180;
  avgz = az*0.1+avgz*0.9;
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

float getAvgx() { return avgx; }
float getAvgy() { return avgy; }
float getAvgz() { return avgz; }

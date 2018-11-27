#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <SoftwareSerial.h>



#define STORE_PRECISION 100.0f
#define SONIC_SENSOR_MAX 2
#define SONIC_BUFFER_SIZE 300
#define DEFAULT_MESSURES 30
#define DEFAULT_HEIGHT_KART 800
#define DEFAULT_HEIGHT_FIXED 1275

#define TILT_DELAY 100
#define TILT_REPEAT 10
#define MAX_TOTAL_MOVMENT 200

SoftwareSerial mySerial(11, 10); // RX, TX
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //Configuration for SainSmart LCD screen (16x2)
LiquidCrystal_I2C lcd_8574(0x20,16,2);
bool lcd_refresh;

int i;
float prev_distance;
bool to_print;



bool is_horizontal(){
  for(int i = TILT_REPEAT; i--; i > 0){
    delay(TILT_DELAY);
    if(!gyro_is_still(MAX_TOTAL_MOVMENT)  ){
       return false;
    }
  }
  return true;
}


void setup() {
   lcd_setup();
   gyro_setup();
   Serial.begin(9600);
   mySerial.begin(9600);
   //button_setup(btnTRIGGER);
   sonic_sensor_setup(0, 2, 3);
   lcd_refresh=true;
   sonic_sensor_clear_buffer(); 
   to_print = true;
   prev_distance = -1;
}

void loop(){
  loop_fixed();
}

void loop_fixed(){
 

    if(is_horizontal()){
    float distance = read_distance_cm(2, 3);
    if(prev_distance != distance){
      lcd_print_distance(distance);
      to_print = true;
      
    }
    prev_distance = distance;
    delay(1000);
    }else{
      if(to_print){
      lcd_print_still();
      to_print=false;
      }
      
    }
   
   
 }
 
  


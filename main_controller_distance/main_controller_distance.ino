#include <SoftwareSerial.h>
#include <LiquidCrystal.h>


#define btnTRIGGER A1

#define STORE_PRECISION 100.0f
#define SONIC_SENSOR_MAX 2
#define SONIC_BUFFER_SIZE 500
#define DEFAULT_MESSURES 50
#define DEFAULT_HEIGHT_KART 800
#define DEFAULT_HEIGHT_FIXED 1275

SoftwareSerial mySerial(11, 10); // RX, TX
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //Configuration for SainSmart LCD screen (16x2)
bool lcd_refresh;
bool moving_platform;
int i;

void setup() {
   lcd_setup();
   Serial.begin(9600);
   mySerial.begin(9600);
   button_setup(btnTRIGGER);
   sonic_sensor_setup(0, 2, 3);
   sonic_sensor_setup(1, 11, 12);
   lcd_refresh=true;
   sonic_sensor_clear_buffer(); 
   moving_platform = lcd_get_moving_platform();
   int  default_height=DEFAULT_HEIGHT_FIXED;   
   if(moving_platform){
     default_height=DEFAULT_HEIGHT_KART;
   }
   
   sonic_sensor_set_offset(lcd_get_sensor_height(default_height));
   lcd_print_waiting();
}

void loop(){
  if(moving_platform){
    loop_kart();
  }else{
    loop_fixed();
  }
}

void loop_fixed(){
  if(button_is_pressed(btnTRIGGER)){
    lcd_print_measuring();
    //Throw the first to messures, as they tend to be errors. 
    sonic_sensor_read(0);
    sonic_sensor_read(1);
    sonic_sensor_clear_buffer();
    for(i = 0; i < DEFAULT_MESSURES; i ++){
      lcd_print_counter(i);
      sonic_sensor_read(0);
      delay(10);
      sonic_sensor_read(1);
      delay(10);
    }
    lcd_print_processing();
    bool store = lcd_print_distance(sonic_sensor_average());
    if(store){
      lcd_print_processing();
      sonic_sensor_print_reads();
    }
    lcd_print_waiting(); 
  }
}

void loop_kart() {
  if(button_is_pressed(btnTRIGGER)){
    if(lcd_refresh){
       lcd_print_measuring();
       lcd_refresh=false;
    }
    sonic_sensor_read(0);
    sonic_sensor_read(1);
   if(sonic_sensor_is_buffer_overflow()){
     lcd_print_full();
     delay(1000);
   } 
  }else{
    if(!sonic_sensor_is_buffer_empty()){     
      if(sonic_sensor_is_buffer_overflow()){
         lcd_print_repeat();
       }else{ 
        lcd_print_processing();
        bool store = lcd_print_distance(sonic_sensor_average());
        if(store){
          lcd_print_processing();
          sonic_sensor_print_reads();
        }
        lcd_print_waiting(); 
       }
      sonic_sensor_clear_buffer();
      lcd_refresh=true;
    }
  }
  delay(10);
  
}

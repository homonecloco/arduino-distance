// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
#define MIN_DELAY 100
#define MAX_DELAY 100

#define MIN_HEIGHT 500
#define MAX_HEIGHT 2000


void lcd_setCursor(int i, int j){
  lcd_8574.setCursor(i,j);
}

void lcd_print(int line, char * text){
  lcd_setCursor(0,line);
  lcd_8574.print(text);
}


void lcd_print( char * text){
  lcd_8574.print(text);
}

//void lcd_print( int text){
//  lcd_8574.print(text);
//}


void lcd_setup(){
   //init LCD on 8574
   lcd_8574.begin();
   lcd_8574.backlight();
   lcd_setCursor(0,0);
   lcd_print(0,"Starting up");
}





// read the buttons
int lcd_read_button()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 // From: http://www.dfrobot.com/wiki/index.php?title=Arduino_LCD_KeyPad_Shield_(SKU:_DFR0009)
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  
 return btnNONE;  // when all others fail, return this...
}

void lcd_clear(){
 lcd_8574.clear(); 
}

bool lcd_print_repeat(){
  lcd_clear();
  lcd_print(0, "Repeat    ");
  lcd_print(1, "meassurement  ");
}

void lcd_print_measuring(){
  lcd_clear();
  lcd_print(0, "Measuring...");
}

void lcd_print_counter(int value){
  lcd_setCursor(0,1);
  lcd_8574.print(value); 
  
};

void lcd_print_full(){
  lcd_clear();
  lcd_print(0, "Temporal memory");
  lcd_print(1, "full");
}


void lcd_print_processing(){
  lcd_clear();
  lcd_print(0, "Processing...");
}

void lcd_print_waiting(){
  lcd_clear();
  lcd_print(0,"Press the");
  lcd_print(1, "red button");
}

void lcd_print_still(){
  lcd_clear();
  lcd_print(0,"Moving sensor");
  lcd_print(1, "");
}

void lcd_print_int_as_float(int value){
   float val = (float)value / 10.0;
   lcd_setCursor(0,1);
   lcd_print("       ");
   lcd_setCursor(0,1);
   if(val < 100.0){
     lcd_print(" ");
   }
   lcd_8574.print(val);
}

bool lcd_get_moving_platform(){
  bool movingPlatform = false;
  bool accepted=false;
  bool selected = false;
  lcd_clear();
  lcd_print(0, ">Fixed");
  lcd_print(1, " Kart ");
  while(!accepted){
   
    lcd_key = lcd_read_button();
    switch(lcd_key){
      case btnUP:{
        movingPlatform = false;
        break;
      }
      case btnDOWN:{
        movingPlatform = true;
        break;
      }
      case btnSELECT:{
        accepted = true;
        delay(500);
        break;
      }
    }

    if(movingPlatform){
      lcd_print(0, " ");
      lcd_print(1, ">");
    }else{
      lcd_print(0, ">");
      lcd_print(1, " ");
    }
    delay(50);
    
    
  }
  
 
  return movingPlatform;  
}


float lcd_get_sensor_height(int default_height){
  bool accepted = false;
  bool pressed = false;
  int value_cm = default_height;
  int lcd_delay = MAX_DELAY;
  lcd_clear();
  
  lcd_print(0,"Sensor height");
  lcd_print(1, "          cm" );
  lcd_print_int_as_float(value_cm);
  int steps = 0; 
  int previous = 0;
  while(!accepted){
    previous = value_cm;
    lcd_key = lcd_read_button();
    //lcd_pressed_button();
    switch (lcd_key){
       case btnUP:{
        value_cm+= steps > 10? 10:1;
        if(value_cm > MAX_HEIGHT){
          value_cm = MIN_HEIGHT;
        }
        steps++;
        break;
      }
      case btnDOWN:{
        value_cm -= steps > 10? 10:1;
        if(value_cm < MIN_HEIGHT){
          value_cm = MAX_HEIGHT;
        }
        steps++;
        break;
      }
      case btnSELECT:{
        accepted=true;
        break;
      }
      case btnNONE:{
        lcd_delay = MAX_DELAY;
        steps = 0;
        break;
      }
    } 
    if(value_cm != previous){
       lcd_print_int_as_float(value_cm);
    } 
    delay(lcd_delay);
  }
  return (float)value_cm/10.0;
}

bool lcd_print_distance(float distance){
  bool readed = false;
  bool result = false;
  lcd_clear();
  lcd_print("Distance: ");
  lcd_8574.print(distance); 
  lcd_setCursor(0,1);
  
  return result;
}


void lcd_pressed_button(){
 //lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
 //lcd.print(millis()/1000);      // display seconds elapsed since power-up


 lcd_setCursor(0,1);            // move to the begining of the second line
// lcd_key = read_LCD_buttons();  // read the buttons

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     lcd_print("RIGHT ");
     break;
     }
   case btnLEFT:
     {
     lcd_print("LEFT   ");
     break;
     }
   case btnUP:
     {
     lcd_print("UP    ");
     break;
     }
   case btnDOWN:
     {
     lcd_print("DOWN  ");
     break;
     }
   case btnSELECT:
     {
     lcd_print("SELECT");
     break;
     }
     case btnNONE:
     {
     lcd_print("NONE  ");
     break;
     }
 }

}

void lcd_gyro_calibrate(){
  bool accepted = false;
  bool pressed = false;
  int lcd_delay = MAX_DELAY;
  int lcd_key;
  
  lcd_clear();
  lcd_print(0, "keep the bar");
  lcd_print(1, "still & press up" );
  
  while(!accepted){
    
    lcd_key = lcd_read_button();
    switch (lcd_key){
       case btnLEFT:{
        break;
      }
      case btnSELECT:{
        accepted=true ;
        lcd_clear();
        lcd_print(0, "Calibrating");
        lcd_print(1, "gyroscope" );
        gyro.zeroCalibrate(200,10);//sample 200 times to calibrate and it will take 200*10ms 
        break;
      }
      case btnNONE:{
         lcd_delay = MAX_DELAY;
        break;
      }
    } 
  }
  return;
}


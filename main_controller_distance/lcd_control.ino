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


void lcd_setup(){
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.clear();
  // Print a message to the LCD.
  lcd.print("Starting up");
}

void lcd_print(int line, char * text){
  lcd.setCursor(0,line);
  lcd.print(text);
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

bool lcd_print_repeat(){
  lcd.clear();
  lcd_print(0, "Repeat    ");
  lcd_print(1, "meassurement  ");
}

void lcd_print_measuring(){
  lcd.clear();
  lcd_print(0, "Measuring...");
}

void lcd_print_counter(int value){
  lcd.setCursor(0,1);
  lcd.print(value); 
  
};

void lcd_print_full(){
  lcd.clear();
  lcd_print(0, "Temporal memory");
  lcd_print(1, "full");
}


void lcd_print_processing(){
  lcd.clear();
  lcd_print(0, "Processing...");
}

void lcd_print_waiting(){
  lcd.clear();
  lcd_print(0,"Press the");
  lcd_print(1, "red button");
}

void lcd_print_int_as_float(int value){
   float val = (float)value / 10.0;
   lcd.setCursor(0,1);
   lcd.print("       ");
   lcd.setCursor(0,1);
   if(val < 100.0){
     lcd.print(" ");
   }
   lcd.print(val);
}

bool lcd_get_moving_platform(){
  bool movingPlatform = false;
  bool accepted=false;
  bool selected = false;
  lcd.clear();
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
    delay(500);
    
    
  }
  
 
  return movingPlatform;  
}


float lcd_get_sensor_height(int default_height){
  bool accepted = false;
  bool pressed = false;
  int value_cm = default_height;
  int lcd_delay = MAX_DELAY;
  lcd.clear();
  
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
  return (float)value_cm/10;
}

bool lcd_print_distance(float distance){
  bool readed = false;
  bool result = false;
  lcd.clear();
  lcd.print("Save? ");
  lcd.print(distance); 
  lcd.setCursor(0,1);
  lcd.print("(Y:up/N:down)");
  
  while(!readed){
    lcd_key = lcd_read_button();
    switch(lcd_key){
      case btnUP:{
        result = true;
        readed = true;
        break;
      }
      case btnDOWN:{
        readed = true;
      }
    }
  }
  
  lcd.clear();
  lcd.setCursor(0,0);
  if(result){
    lcd.print("Saving          ");
  }else{
   lcd.print("Discarded       ");
  }
  lcd.setCursor(0,1);
  lcd.print(distance);
  //display_pressed_button();
  return result;
}

void lcd_pressed_button(){
 //lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
 //lcd.print(millis()/1000);      // display seconds elapsed since power-up


 lcd.setCursor(0,1);            // move to the begining of the second line
// lcd_key = read_LCD_buttons();  // read the buttons

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     lcd.print("RIGHT ");
     break;
     }
   case btnLEFT:
     {
     lcd.print("LEFT   ");
     break;
     }
   case btnUP:
     {
     lcd.print("UP    ");
     break;
     }
   case btnDOWN:
     {
     lcd.print("DOWN  ");
     break;
     }
   case btnSELECT:
     {
     lcd.print("SELECT");
     break;
     }
     case btnNONE:
     {
     lcd.print("NONE  ");
     break;
     }
 }

}


int SONIC_EMIT[SONIC_SENSOR_MAX];
int SONIC_ECHO[SONIC_SENSOR_MAX];
int SONIC_BUFFER[SONIC_BUFFER_SIZE];
int SONIC_BUFFER_USED = 0;
long SONIC_BUFFER_AVG = 0;
int SONIC_ID = 0;
float SONIC_SENSOR_OFFSET;

bool SONIC_BUFFER_OVERFLOW = false;
void sonic_sensor_setup(int id, int emit, int echo){
//  pinMode(port, INPUT);
  SONIC_EMIT[id] = emit;
  SONIC_ECHO[id] = echo;
  pinMode(emit,OUTPUT);
  pinMode(echo,INPUT);
  
}


float read_distance_cm(int trigger, int echo){
  //Serial.print("Sonic from ports:");
  //Serial.print(trigger);
  //Serial.print(echo);
  //Serial.print("\n");
  // Trigger US-100 to start measurement
  // Set up trigger
  float distance;
  digitalWrite(trigger,LOW);
  delayMicroseconds(5);
 
  // Start Measurement
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
 
  // Acquire and convert to cm
  distance=pulseIn(echo,HIGH);
  distance=distance*0.01657;
  return distance;
}

void sonic_sensor_clear_buffer(){
  SONIC_BUFFER_USED = 0;
  SONIC_BUFFER_AVG =0;
  SONIC_BUFFER_OVERFLOW = false;
}

bool sonic_sensor_read(int id){
  SONIC_BUFFER[SONIC_BUFFER_USED] = (int) (read_distance_cm(SONIC_EMIT[id], SONIC_ECHO[id]) * STORE_PRECISION ) ;
  SONIC_BUFFER_AVG += SONIC_BUFFER[SONIC_BUFFER_USED];

  SONIC_BUFFER_USED++;
  if(SONIC_BUFFER_USED >= SONIC_BUFFER_SIZE){
    //Serial.print("Buffer is full! clearing");
    sonic_sensor_clear_buffer();
    SONIC_BUFFER_OVERFLOW = true;
  }
}

void sonic_sensor_print_reads(){
  int i;
//  Serial.print("ID");
  Serial.print(SONIC_ID);
  Serial.print("\n");
  delay(500);
//  Serial.print("AVG");
  Serial.print(sonic_sensor_average());
  Serial.print("\n");
  delay(500);
  for(i=0; i<SONIC_BUFFER_USED; i++){
    Serial.print((float)SONIC_BUFFER[i]/STORE_PRECISION);
    Serial.print(",");
  }
  Serial.print(":\n");
 
 
  delay(500);
  Serial.print(SONIC_SENSOR_OFFSET);
  Serial.print("\n");
  
  SONIC_ID++;
}

bool sonic_sensor_is_buffer_empty(){
  return SONIC_BUFFER_USED == 0;
}

bool sonic_sensor_is_buffer_overflow(){
  return SONIC_BUFFER_OVERFLOW;
}

float sonic_sensor_average(){
  //return (float) SONIC_BUFFER_AVG;
  quicksort(SONIC_BUFFER, 0, SONIC_BUFFER_USED - 1);
  int drop = SONIC_BUFFER_USED * 0.1;
  int i;
  int sum = 0;
  int count = 0;
  for(i = drop; i < SONIC_BUFFER_USED - drop; i++){
    sum += SONIC_BUFFER[i];
    count ++;
  }

  return ( (float)SONIC_SENSOR_OFFSET ) - ((float)sum/(float)count)  / STORE_PRECISION;
}

void sonic_sensor_set_offset(int offset){
  SONIC_SENSOR_OFFSET = offset;
}

/* replaced by luke through quicksort -> seems to be the fastest to process information on Arduino ??? */
////////////////////////////////////////
void quicksort(int a[], int l, int r){            //a=Array, l=left border, r=right border  
  if(r>l){                              //as long that more then 1 followingelement exists do
    int i=l-1, j=r;                  //Initialising the variables with the borders
    int tmp;
    for(;;){                        //endless loop; stops if i>=j
      while(a[++i]<a[r]);                  //inkrem., until found bigger element
      while(a[--j]>a[r] && j>i);            //dekrem., until found smaler element
      if(i>=j) break;                   //stops run if pointer meet
      tmp=a[i];                        //exchange smaller with bigger element
      a[i]=a[j];                       //exchange smaller with bigger element
      a[j]=tmp;                        //exchange smaller with bigger element
    }
    tmp=a[i];           //exchange dividing element
    a[i]=a[r];          //exchange dividing element
    a[r]=tmp;            //exchange dividing element

    quicksort(a, l, i-1);                  //recursive call of function for left
    quicksort(a, i+1, r);                  //recursive call of function for right
  }
}

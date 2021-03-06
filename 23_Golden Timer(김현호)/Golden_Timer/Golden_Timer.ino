int LED = 13;
int Buzzer = 12;
int count = 0;
int Button = 2;
int Sensor_Button = 3;
int Count_Interrupt = 0;
volatile byte state = LOW;
volatile byte waiting_state = HIGH;
int segmentLEDs[] = {10,11,4,5,6,7,8,9};
int segmentLEDsNum = 8;
int digitForNum[10][8] = {
  {0,0,0,0,0,0,1,1}, //0
  {1,0,0,1,1,1,1,1}, //1
  {0,0,1,0,0,1,0,1}, //2
  {0,0,0,0,1,1,0,1}, // 3
  {1,0,0,1,1,0,0,1}, //4
  {0,1,0,0,1,0,0,1}, // 5
  {0,1,0,0,0,0,0,1}, //6
  {0,0,0,1,1,1,1,1}, //7
  {0,0,0,0,0,0,0,1}, //8
  {0,0,0,0,1,0,0,1} //9
};

void counting_func(void){                       // Counting 30 seconds
  count = 0;
  Serial.println("--Counitng_func called---");
  while (count<5){
    if(Count_Interrupt == 1) break;             // When the reset button pushed, recounting the second.
    delay(1000);
    count++;
    Serial.print("Counting...");
    Serial.println(count);
  }
}

void emergency_counting_func(void){             // Counting 9 Seconds
  count = 0;
  Serial.println("---EMG_Counting_func callled---");  
  while (count<5){
    if(Count_Interrupt == 1) break;             // When the reset button pushed, recounting the second.
    delay(1000);
    count++;
    Serial.print("EMG_Counting...");
    Serial.println(count);
  }
}

void segment_reset(void){                       //  OFF the segment lights.
  Serial.println("---Segment reset---");
  for(int i = 0; i < segmentLEDsNum; i++){
    digitalWrite(segmentLEDs[i], HIGH);
  }
}

void segment_ON(void){                          // On the segment lights.
  Serial.println("---Segment ON---");
   for (int i = 0; i < segmentLEDsNum; i++){
    digitalWrite(segmentLEDs[i], digitForNum[3][i]);  // You could turn the specific number if you put the number 0~9 in digitForNum[here][i].
  }
  Serial.println("Segment No. 3");
  while (Count_Interrupt != 1){           //  Buzzer On
    tone(Buzzer, 3951, 500);              // 'Si~'   
    delay(250);
    tone(Buzzer, 2793, 500);              // 'Pa~'
    delay(250);
  }
}

void reset(){                             // Interrupt function of the reset button.
  Count_Interrupt = 1;
  state = !state;
}

void waiting(){                           // Interrupt functioin of the sensor button.
  Count_Interrupt = 1;
  waiting_state = !waiting_state;
}

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);                                                      // LED
  pinMode(Button, INPUT_PULLUP);                                             // Button 
  attachInterrupt(digitalPinToInterrupt(Button), reset, LOW);                // The reset button interrupt setting
  pinMode(Sensor_Button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Sensor_Button), waiting, RISING);    // It occurs the interrupt when the sensor button's state changes Low to HIGH.
  for(int i = 0; i <segmentLEDsNum; i++){                                    // Seven-Segment pinMode initializing 
    pinMode(segmentLEDs[i], OUTPUT);
  }
}

void loop() {
  Serial.println("---Initializing...---");
  digitalWrite(LED, LOW);                         // LED OFF
  segment_reset();                                // Segment OFF
  
  Serial.println("Waiting...");
  delay(1000);
   
  while(digitalRead(Sensor_Button) != HIGH){      // When the sensor button pushed.
    Serial.println("---Initializing...---");
    digitalWrite(LED, LOW);                        // LED OFF
    segment_reset();                              // Segment OFF
    Count_Interrupt = 0;
    counting_func();                              // Counting 30 sec
    digitalWrite(LED, HIGH);                      // LED ON
    Serial.println("Light ON");
    emergency_counting_func();                    // Counting 9 sec  
    segment_ON();                                 // Seven-Segment ON
  }
}

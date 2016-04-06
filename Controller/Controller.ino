const int THERMOMETER_IN = 0;
const int TEMP_HEATING = 18;
const int TEMP_LOW = 19;
const int TEMP_HIGH = 21;

const int SIDE_POWER = 2;
const int SIDE_DIRECTION = 4;
const int MOTOR_ON_INDICATOR = 7;
const int HEATING_PIN = 8;

const int PCT_OPEN = 25;
const int PCT_OPEN_LED0 = 6;
const int PCT_OPEN_LED1 = 9;
const int PCT_OPEN_LED2 = 10;
const int PCT_OPEN_LED3 = 11;
const int NB_OF_STEPS_IN_ANIMATION = 5; // should be a divider of PCT_OPEN

const int CLOSE = HIGH;
const int OPEN = LOW;
const int ON = HIGH;
const int OFF = LOW;
const int ROTATION_TIME = 4000;
const int PAUSE_TIME = 1000;
const int SLEEPTIME = 6000; //60000 for one minute

int opened = 0;
boolean heating = false;
float greenhouseTemperature = 20.0;

void setup() {
  Serial.begin(9600);   // start serial port to show results
  Serial.println("Starting");
  pinMode(SIDE_POWER, OUTPUT); //defines pin as output
  digitalWrite(SIDE_POWER, LOW); //defines starting state of pin
 
  pinMode(SIDE_DIRECTION, OUTPUT);
  digitalWrite(SIDE_DIRECTION, LOW);
  
  pinMode(MOTOR_ON_INDICATOR, OUTPUT);
  digitalWrite(MOTOR_ON_INDICATOR, LOW);
  pinMode(HEATING_PIN, OUTPUT);
  digitalWrite(HEATING_PIN, LOW);
  
  pinMode(PCT_OPEN_LED0, OUTPUT); 
  pinMode(PCT_OPEN_LED1, OUTPUT);   
  pinMode(PCT_OPEN_LED2, OUTPUT); 
  pinMode(PCT_OPEN_LED3, OUTPUT); 
  
  Serial.println("Resetting position");
  opened = 100;
  for (int i=0; i<5; i++){
    closeSides();
    delay(PAUSE_TIME);
  }
  Serial.println("Resetting done");
}

float getTemp(int inPin){
  int vIn = analogRead(inPin);
  return (vIn * 500L) /1024.0; //using lm35 directly
}

void displayStatus(int pct){
  int led0 = 0;
  int led1 = 0;
  int led2 = 0;
  int led3 = 0;
  if (pct <= 25){
    led0 = pct * 4;
  }else{
    led0 = 100;
    if (pct <= 50){
      led1 = (pct-25) * 4;
    }else{
      led1 = 100;
      if (pct <= 75){
        led2 = (pct-50) * 4;
      }else{
        led2 = 100;
        if (pct <= 100){
          led3 = (pct-75) * 4;
        }
      }
    }
  }
  analogWrite(PCT_OPEN_LED0, led0);
  analogWrite(PCT_OPEN_LED1, led1);
  analogWrite(PCT_OPEN_LED2, led2);
  analogWrite(PCT_OPEN_LED3, led3);
}

void setOpenedStatus(int pctIncrease){
  opened += pctIncrease;
  if(opened < 0) {
    opened = 0;
  }else if (opened > 100){
    opened = 100;
  }
  Serial.print("    ");
  Serial.println(opened);
  displayStatus(opened);
}

void animate(int movement){
  for (int i=0; i < NB_OF_STEPS_IN_ANIMATION; i++){
      delay(ROTATION_TIME / NB_OF_STEPS_IN_ANIMATION);
      setOpenedStatus(movement * PCT_OPEN / NB_OF_STEPS_IN_ANIMATION);
    };
}

void openSides(){
  if(opened < 100){
    Serial.println("  Opening");
    digitalWrite(SIDE_POWER,ON);
    digitalWrite(SIDE_DIRECTION, OPEN);
    digitalWrite(MOTOR_ON_INDICATOR, HIGH);
    animate(1);
    digitalWrite(SIDE_POWER, OFF);
    digitalWrite(MOTOR_ON_INDICATOR, LOW);
    Serial.println("  Done opening");
  }
}

void closeSides(){
  if (opened > 0){
    Serial.println("  Closing");
    digitalWrite(SIDE_POWER, ON);
    digitalWrite(SIDE_DIRECTION, CLOSE);
    digitalWrite(MOTOR_ON_INDICATOR, HIGH);
    animate(-1);
    digitalWrite(SIDE_POWER, OFF);
    digitalWrite(MOTOR_ON_INDICATOR, LOW);
    Serial.println("  Done closing");
  }
}

void formatAndPrintTemp(float measuredTemperature){
  char str_temp[6];
  char temperature[10];
  dtostrf(measuredTemperature, 4, 1, str_temp);
  sprintf(temperature, "%s C",str_temp);
  Serial.println(temperature);
}

void setHeater(int heaterCommand){
  if ((heaterCommand == ON) && (heating == false)){
    Serial.println("  Start heating");
    heating = true;
    digitalWrite(HEATING_PIN, heaterCommand);
  }else if ((heaterCommand == OFF) && (heating == true)){
    Serial.println("  Stop heating");
    heating = false;
    digitalWrite(HEATING_PIN, heaterCommand);
  }else{
    Serial.print("  Heating:");
    Serial.println(heating);
  }
}

void loop() {
  greenhouseTemperature = getTemp(THERMOMETER_IN);
  formatAndPrintTemp(greenhouseTemperature);
  
  if (greenhouseTemperature <= TEMP_HEATING){
    setHeater(ON);
  }else{
    setHeater(OFF);
  }
  
  if ((greenhouseTemperature > TEMP_HEATING) && (greenhouseTemperature < TEMP_LOW)){
    closeSides();
  }else if(greenhouseTemperature > TEMP_HIGH){
    openSides();
  }
  Serial.print("  Opened:");
  Serial.println(opened);
  delay(SLEEPTIME);
}

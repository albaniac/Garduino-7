const int THERMOMETER_IN = 0;
const int TEMP_LOW = 19;
const int TEMP_HIGH = 21;
const int SIDE_POWER = 2;
const int SIDE_DIRECTION = 4;
const int MOTOR_ON_INDICATOR = 7;
const int PCT_OPEN_LED0 = 8;
const int PCT_OPEN_LED1 = 9;
const int PCT_OPEN_LED2 = 10;
const int PCT_OPEN_LED3 = 11;

const int CLOSE = HIGH;
const int OPEN = LOW;
const int ON = HIGH;
const int OFF = LOW;
const int ROTATION_TIME = 4000;
const int PAUSE_TIME = 1000;
const int SLEEPTIME = 6000;

const int PCT_OPEN = 25;

int opened = 0;
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
  int led0 = LOW;
  int led1 = LOW;
  int led2 = LOW;
  int led3 = LOW;
  
  if (pct >= 25){
    led0 = HIGH;
  }
  if (pct >= 50){
    led1 = HIGH;
  }
  if (pct >= 75){
    led2 = HIGH;
  }
  if (pct >= 100){
    led3 = HIGH;
  }
  
  digitalWrite(PCT_OPEN_LED0, led0);
  digitalWrite(PCT_OPEN_LED1, led1);
  digitalWrite(PCT_OPEN_LED2, led2);
  digitalWrite(PCT_OPEN_LED3, led3);
}

void setOpenedStatus(int pctIncrease){
  opened += pctIncrease;
  Serial.print("Open %:");
  Serial.println(opened);
  displayStatus(opened);
}

void openSides(){
  if(opened < 100){
    Serial.println("Opening");
    digitalWrite(SIDE_POWER,ON);
    digitalWrite(SIDE_DIRECTION, OPEN);
    digitalWrite(MOTOR_ON_INDICATOR, HIGH);
    delay(ROTATION_TIME);
    digitalWrite(SIDE_POWER, OFF);
    digitalWrite(MOTOR_ON_INDICATOR, LOW);
    setOpenedStatus(PCT_OPEN);
    Serial.println("Done opening");
  }
}

void closeSides(){
  if (opened > 0){
    Serial.println("Closing");
    digitalWrite(SIDE_POWER, ON);
    digitalWrite(SIDE_DIRECTION, CLOSE);
    digitalWrite(MOTOR_ON_INDICATOR, HIGH);
    setOpenedStatus(0);
    delay(ROTATION_TIME);
    digitalWrite(SIDE_POWER, OFF);
    digitalWrite(MOTOR_ON_INDICATOR, LOW);
    setOpenedStatus(-PCT_OPEN); 
    Serial.println("Done closing");
  }
}

void formatAndPrintTemp(float measuredTemperature){
  char str_temp[6];
  char temperature[10];
  dtostrf(measuredTemperature, 4, 1, str_temp);
  sprintf(temperature, "%s C",str_temp);
  Serial.println(temperature);
}

void loop() {
  greenhouseTemperature = getTemp(THERMOMETER_IN);
  formatAndPrintTemp(greenhouseTemperature);
  if (greenhouseTemperature > TEMP_HIGH){
    openSides();
  }else if (greenhouseTemperature < TEMP_LOW){
    closeSides();
  }
  delay(SLEEPTIME);
}

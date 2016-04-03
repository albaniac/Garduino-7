const int THERMOMETER_IN = 0;
const int TEMP_LOW = 15;
const int TEMP_HIGH = 25;
const int SIDE_POWER = 2;
const int SIDE_DIRECTION = 4;
const int OPENED_INDICATOR = 7;
const int MOTOR_ON_INDICATOR = 8;
const int CLOSE = HIGH;
const int OPEN = LOW;
const int ON = HIGH;
const int OFF = LOW;
const int ROTATION_TIME = 4000;
const int SLEEPTIME = 60000;

boolean opened = false;
float greenhouseTemperature = 20.0;

void setup() {
  Serial.begin(9600);   // start serial port to show results
  Serial.println("Starting");
  pinMode(SIDE_POWER, OUTPUT); //defines pin as output
  digitalWrite(SIDE_POWER, LOW); //defines starting state of pin
 
  pinMode(SIDE_DIRECTION, OUTPUT);
  digitalWrite(SIDE_DIRECTION, LOW);
  
  pinMode(OPENED_INDICATOR, OUTPUT);
  digitalWrite(OPENED_INDICATOR, LOW);
  
  pinMode(MOTOR_ON_INDICATOR, OUTPUT);
  digitalWrite(MOTOR_ON_INDICATOR, LOW);
}

float getTemp(int inPin){
  int vIn = analogRead(inPin);
  return (vIn * 500L) /1024.0; //using lm35 directly
}


void setOpenedStatus(boolean isOpened){
  opened = isOpened;
  digitalWrite(OPENED_INDICATOR, isOpened);
  Serial.print("Opened:");
  Serial.println(opened);
}

void openSides(){
  if(!opened){
    Serial.println("Opening");
    digitalWrite(SIDE_POWER,ON);
    digitalWrite(SIDE_DIRECTION, OPEN);
    digitalWrite(MOTOR_ON_INDICATOR, HIGH);
    delay(ROTATION_TIME);
    digitalWrite(SIDE_POWER, OFF);
    digitalWrite(MOTOR_ON_INDICATOR, LOW);
    setOpenedStatus(true);
    Serial.println("Done opening");
  }
}

void closeSides(){
  if (opened){
    Serial.println("Closing");
    digitalWrite(SIDE_POWER, ON);
    digitalWrite(SIDE_DIRECTION, CLOSE);
    digitalWrite(MOTOR_ON_INDICATOR, HIGH);
    delay(ROTATION_TIME);
    digitalWrite(SIDE_POWER, OFF);
    digitalWrite(MOTOR_ON_INDICATOR, LOW);
    setOpenedStatus(false); 
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

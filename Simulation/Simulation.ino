const int THERMOMETER_IN = 0;
const int TEMP_LOW = 15;
const int TEMP_HIGH = 25;
const int SIDE_POWER = 1;
const int SIDE_DIRECTION = 2;
const int CLOSE = HIGH;
const int OPEN = LOW;
const int ON = HIGH;
const int OFF = LOW;
const int ROTATION_TIME = 4000;
const int SLEEPTIME = 60000;

boolean raising = true; //simulate temperature variations
boolean opened = false;
float greenhouseTemperature = 20.0;

void setup() {
  Serial.begin(9600);   // start serial port to show results
  
  pinMode(SIDE_POWER, OUTPUT); //defines pin as output
  digitalWrite(SIDE_POWER, LOW); //defines starting state of pin
 
  pinMode(SIDE_DIRECTION, OUTPUT);
  digitalWrite(SIDE_DIRECTION, LOW);
}

float getTemp(){
  int vIn = analogRead(THERMOMETER_IN);
  greenhouseTemperature = (vIn * 500L) /1024.0; //using lm35 directly
}

void openSides(){
  if(!opened){
    Serial.println("Opening");
    digitalWrite(SIDE_POWER,ON);
    digitalWrite(SIDE_DIRECTION, OPEN);
    delay(ROTATION_TIME);
    digitalWrite(SIDE_POWER,OFF);
    opened = true;
    Serial.println("Done opening");
  }
}

void closeSides(){
  if (opened){
    Serial.println("Closing");
    digitalWrite(SIDE_POWER,ON);
    digitalWrite(SIDE_DIRECTION, CLOSE);
    delay(ROTATION_TIME);
    digitalWrite(SIDE_POWER,OFF);
    opened = false; 
    Serial.println("Done closing");
  }
}

void formatAndPrintTemp(){
  char str_temp[6];
  char temperature[10];
  dtostrf(greenhouseTemperature, 4, 1, str_temp);
  sprintf(temperature, "%s C",str_temp);
  Serial.println(temperature);
}

void loop() {
  getTemp();
  formatAndPrintTemp();
  if (greenhouseTemperature > TEMP_HIGH){
    openSides();
  }else if (greenhouseTemperature < TEMP_LOW){
    closeSides();
  }
  delay(SLEEPTIME);
}

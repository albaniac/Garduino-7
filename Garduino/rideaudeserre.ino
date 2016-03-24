/*-----( Import needed libraries for digital temp sensor)-----*/
// Get 1-wire Library here: http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>

//Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <DallasTemperature.h>

/*-----( Declare Constants and Pin Numbers for Temp Sensor )-----*/
#define ONE_WIRE_BUS_PIN 3

/*-----( Declare objects )-----*/
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS_PIN);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

/*-----( Declare Variables )-----*/
// Assign the addresses of your 1-Wire temp sensors.
// See the tutorial on how to obtain these addresses:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html

DeviceAddress Probe01 = { 0x28, 0x48, 0x71, 0x7E, 0x04, 0x00, 0x00, 0x49 };  // Greenhouse

//define pins
int Cote_power = 1;
int Cote_direction = 2;


void setup()  /****** SETUP: RUNS ONCE ******/
{
  
   Serial.begin(9600);   // start serial port to show results
  
    pinMode(Cote_power, OUTPUT); //defines pin as output
    digitalWrite(Cote_power, LOW); //defines starting state of pin
 
    pinMode(Cote_direction, OUTPUT);
    digitalWrite(Cote_direction, LOW);
    
      // Initialize the Temperature measurement library
  sensors.begin();
  
    // set the resolution to 10 bit (Can be 9 to 12 bits .. lower is faster)
  sensors.setResolution(Probe01, 10);
  
  }//--(end setup )---

void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  
    // Command all devices on bus to read temperature  
  sensors.requestTemperatures();  
  
  Serial.print("Greenhouse temperature is:   ");
  printTemperature(Probe01);
  float Serre_Temp = sensors.getTempC(Probe01);

  Serial.println();
  
   //Open the sides --------------------------
      if(Serre_Temp > 25)
    {
      //Run Motors for 4 seconds
      digitalWrite(Cote_direction, LOW); //start the motors
      digitalWrite(Cote_power, HIGH); //start the motors
          Serial.print("Opening");
          delay(4000); // wait 4 seconds
      digitalWrite(Cote_power, LOW);  //stop the motors

    }

   //Close the sides --------------------------
      if(Serre_Temp < 15)
    {
      //Run Motors for 4 seconds
     digitalWrite(Cote_direction, HIGH); //start the motors
      digitalWrite(Cote_power, HIGH); //start the motors
          Serial.print("Closing");
          delay(4000); // wait 4 seconds
      digitalWrite(Cote_power, LOW);  //stop the motors
    }

          delay(120000); //wait 2 minutes then run the program again
          
}

/*-----( Declare User-written Functions )-----*/
void printTemperature(DeviceAddress deviceAddress)
{

float tempC = sensors.getTempC(deviceAddress);

   if (tempC == -127.00) 
   {
   Serial.print("Error getting temperature  ");
   } 
   else
   {
   Serial.print("C: ");
   Serial.print(tempC);
   Serial.print(" F: ");
   Serial.print(DallasTemperature::toFahrenheit(tempC));
   }
}// End printTemperature
//*********( THE END )***********

/*-----( Import needed libraries for digital temp sensor)-----*/
// Get 1-wire Library here: http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>

//Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <DallasTemperature.h>

/*-----( Declare Constants and Pin Numbers for Temp Sensor )-----*/
#define ONE_WIRE_BUS 2

/*-----( Declare objects )-----*/
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

/*-----( Declare Variables )-----*/
// Assign the addresses of your 1-Wire temp sensors.
// See the tutorial on how to obtain these addresses:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html

DeviceAddress Probe01 = { 0x28, 0x59, 0x62, 0x26, 0x07, 0x00, 0x00, 0x37

 };  // Greenhouse

//define pins
int Cote_power = 8;
int Cote_direction = 7;


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
      if(Serre_Temp > 22)
    {
      //Run Motors for 4 seconds
      digitalWrite(Cote_direction, HIGH); //open the direction relay
      digitalWrite(Cote_power, HIGH); //start the power relay to feed power to the motors
          Serial.print("Ouverture");
          delay(12000); // wait 12 seconds
      digitalWrite(Cote_power, LOW);  //close the power relay to stop the motors
      digitalWrite(Cote_direction, LOW); //close the relay
          Serial.print(" termine, attendre");

    }

   //Close the sides --------------------------
      if(Serre_Temp < 19)
    {
      //Run Motors for 4 seconds
     digitalWrite(Cote_direction, LOW); //No signal to the direction relay
      digitalWrite(Cote_power, HIGH); //open the power relay to start the motors
          Serial.print("Fermeture");
          delay(12000); // wait 12 seconds
      digitalWrite(Cote_power, LOW);  //stop the motors
          Serial.print(" termine, attendre");
    }

          delay(30000); //wait 30 sec then run the program again
          
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
//*********( THE END )***********void setup() {
  // put your setup code here, to run once:

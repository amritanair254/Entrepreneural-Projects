
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   
#include <Wire.h> 
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);    // set the LCD address to 0x27 for a 16 chars and 2 line display
#define ONE_WIRE_BUS 7               //ds18b20 module attach to pin7
OneWire oneWire(ONE_WIRE_BUS);       // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature. 

//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"


void setup() {   

  Serial.begin(9600);                   // For Serial Monitor
  sensors.begin();                      // initialize the bus
  pulseSensor.analogInput(PulseWire);   // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.blinkOnPulse(LED13);      //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   
  lcd.init(); 
  pinMode(6, OUTPUT);                   // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
  if (pulseSensor.begin()) {
     Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
}



void loop() {
sensors.requestTemperatures();
int myBPM = pulseSensor.getBeatsPerMinute();                  // Calls function on our pulseSensor object that returns BPM as an "int". "myBPM" hold this BPM value now. 
float myTemp = (1.8*sensors.getTempCByIndex(0)) + 32.0+10;    // Temperature in F plus an offset
//if (pulseSensor.sawStartOfBeat()) {                         // Constantly test to see if "a beat happened". 
if((myBPM>60) && myTemp>81){

  lcd.backlight();
 
  Serial.println("A HeartBeat Happened ! ");    // If test is "true", print a message "a heartbeat happened".
  Serial.print("BPM: ");                        // Print phrase "BPM: " 
  Serial.println(myBPM);                        // Print the value inside of myBPM. 
  
  lcd.setCursor(0,0);
  lcd.print("HeartRate:");
  lcd.print(myBPM);
  lcd.print("BPM ");
   
  lcd.setCursor(0, 1);
  lcd.print("BodyTemp:");   
  lcd.print(myTemp); //print the temperature on lcd1602
  lcd.print(char(223));  //print the unit" ℉ "
  lcd.print("F");

  if(myTemp>90 && myTemp<100 && myBPM>98){
    lcd.setCursor(0,0);
    lcd.print("You are stressed!");
    lcd.setCursor(0,1);
    lcd.print("Let's Breathe :)");
    lcd.setCursor(0,1);
    lcd.print("Breathe in....");
    digitalWrite(6, HIGH); // sets the digital pin 6 on - VIBRATE
    delay(2000);            // waits for 2 seconds
    lcd.setCursor(0,1);
    lcd.print("Breathe out...");
    digitalWrite(6, LOW);  // sets the digital pin 6 off
    delay(2000);            // waits for 2 seconds
  }
  else if(myTemp>100){
    lcd.setCursor(0,0);
    lcd.print("You are sick!");
    lcd.setCursor(0,1);
    lcd.print("Please consult physician");
  }
  
}
else{
  lcd.noBacklight();            // Turn off backlight to save power
  delay(20);  
}
  delay(20);                    // considered best practice in a simple sketch.

}

  

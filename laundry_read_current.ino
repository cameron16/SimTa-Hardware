#include "EmonLib.h" // Include Emon Library
//#include <SoftwareSerial.h>
EnergyMonitor emon1; // Create an instance


//SoftwareSerial ESPserial(2, 3); // RX | TX
//int state= 0; //start in OFF
int interval = 10000; //10 seconds
int threshold = 1200.0;

int irms_threshold = 1200;
int turning_on_counter = 0;
int turning_off_counter = 0;
int turn_on_counter_threshold = 20;
int turn_off_counter_threshold = 20;
int state =0; 
//State definitions 
//0: OFF and val < threshold
//1 OFF and val > threshold and turning_on_counter < turn_on_counter_threshold
//2: ON and val > threshold
//3: ON and val < threshold and turning_off_counter < turn_off_counter_threshold 

//int send_signal_duration = 10000; //10 seconds
int SIGNAL_PIN = 4;

int red_light = 11;
int blue_light = 10;

void setup()
{
  
  emon1.current(1, 111.1);             // Current: input pin, calibration.
  Serial.begin(9600);     // communication with the host computer

   pinMode(4, OUTPUT);
   pinMode(7, OUTPUT);
   pinMode(LED_BUILTIN, OUTPUT); 
}


void loop() {

   while (state == 0){
      digitalWrite(SIGNAL_PIN, HIGH); //send off signal
      double Irms = emon1.calcIrms(1480);  // Calculate Irms only
      double Irms_val = Irms*230.0;
      Serial.println(Irms*230.0);         // Apparent power
      Serial.println("STATE 0");
      turning_on_counter = 0;
      if (Irms_val >= irms_threshold){
        state = 1;
      }
    }
    while (state == 1){
      digitalWrite(SIGNAL_PIN, HIGH); //send off signal
      double Irms = emon1.calcIrms(1480);  // Calculate Irms only
      double Irms_val = Irms*230.0;
      Serial.println(Irms*230.0);         // Apparent power
      Serial.println("STATE 1");
      if (Irms_val < irms_threshold){
        state =0;
        turning_on_counter = 0;
      }
      //Irms_val >= irms_threshold <- this must be true in next 2 conditions
      else if (turning_on_counter < turn_on_counter_threshold){
        turning_on_counter++;
      }
      else{
        turning_on_counter = 0;
        state = 2;
      } 
    }
    while (state == 2){
        digitalWrite(SIGNAL_PIN, LOW); //send on signal
        double Irms = emon1.calcIrms(1480);  // Calculate Irms only
        double Irms_val = Irms*230.0;
        Serial.println(Irms*230.0);         // Apparent power
        Serial.println("STATE 2");
        turning_off_counter = 0;
        
        if (Irms_val <= irms_threshold){
          state = 3;
        } 
    }
    while (state == 3){
        digitalWrite(SIGNAL_PIN, LOW); //send on signal
        double Irms = emon1.calcIrms(1480);  // Calculate Irms only
        double Irms_val = Irms*230.0;
        Serial.println(Irms*230.0);         // Apparent power
        Serial.println("STATE 3");
        if (Irms_val > irms_threshold){
          state = 2;
          turning_off_counter = 0;
        }
        //Irms_val <= irms_threshold <- this must be true now
        else if (turning_off_counter < turn_off_counter_threshold){
          turning_off_counter++;
        }
        else{
          turning_off_counter = 0;
          state = 0;
//          send_laundry_off_signal();
        } 
    }
}  








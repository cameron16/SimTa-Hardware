#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);


float y_val_curr;
int state = 0; //state = 0 => door close. state =1 ==> door open
float open_margin = 25.0;
float close_margin = 8.0;
float y_avg_val;
float x_avg_val;
float z_avg_val;

int SIGNAL_PIN = 4;



void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void setup(void)
{
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  Serial.begin(9600);
  Serial.println("Magnetometer Test"); Serial.println("");

  /* Enable auto-gain */
  mag.enableAutoRange(true);

  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

  /* Display some basic information on this sensor */
  displaySensorDetails();
  float counter_setup_x = 0.0;
  float counter_setup_y = 0.0;
  float counter_setup_z = 0.0;
  int num_iterations = 5;
  for (int i =0; i<num_iterations; i++){
    sensors_event_t event;
    mag.getEvent(&event);
    counter_setup_x = counter_setup_x + event.magnetic.x;
    counter_setup_y = counter_setup_y + event.magnetic.y;
    counter_setup_z = counter_setup_z + event.magnetic.z;
  }
  x_avg_val = counter_setup_x / num_iterations;
  y_avg_val = counter_setup_y / num_iterations;
  z_avg_val = counter_setup_z / num_iterations;

  
//  float threshold = avg_val + margin;
}
  
  


void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  mag.getEvent(&event);

  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
  y_val_curr = event.magnetic.y;
  /* Note: You can also get the raw (non unified values) for */
  /* the last data sample as follows. The .getEvent call populates */
  /* the raw values used below. */
  // Serial.print("X Raw: "); Serial.print(mag.raw.x); Serial.print("  ");
  // Serial.print("Y Raw: "); Serial.print(mag.raw.y); Serial.print("  ");
  // Serial.print("Z Raw: "); Serial.print(mag.raw.z); Serial.println("");
  
  /* Delay before the next sample */
  
  
  if (state == 0 && abs(y_val_curr - y_avg_val) > open_margin){
    Serial.println("DOOR OPENED"); 
    state = 1; 
  }
  else if (state == 1 && abs(y_val_curr - y_avg_val) < close_margin){
    Serial.println("DOOR CLOSED"); 
    state =0;
   }

   if (state ==0){ //door is closed
        digitalWrite(SIGNAL_PIN, HIGH); //send door closed signal
   }
   else if (state ==1){
        digitalWrite(SIGNAL_PIN, LOW); //send door open signal
    }
  
  Serial.println("STATE #");
  Serial.println(state);
  delay(500);
  


  
}

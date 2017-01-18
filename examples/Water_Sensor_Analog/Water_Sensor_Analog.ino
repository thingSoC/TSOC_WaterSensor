/*macro definition of water sensor and the buzzer*/
#define LED          13
#define WATER_SENSOR A0
#define BUZZER       7
#define PUMP         9

void setup()
{
    pins_init();
}
void loop()
{
  
    if(isExposedToWater()) {
      digitalWrite(LED, HIGH);
      digitalWrite(PUMP, HIGH);
      soundAlarm();
      //runPump();
    } else {
      digitalWrite(LED, LOW);
      digitalWrite(PUMP, LOW);
    }
}

void pins_init()
{
    pinMode(WATER_SENSOR, INPUT);
    pinMode(LED, OUTPUT);
    pinMode(PUMP, OUTPUT);
    pinMode(BUZZER, OUTPUT);
}

/************************************************************************/
/*Function: When the sensor is exposed to the water, the buzzer sounds  */
/*          for 2 seconds.                                              */
/************************************************************************/
void soundAlarm()
{
    for(uint8_t i = 0;i < 20;i ++)
    {
        digitalWrite(BUZZER, HIGH);
        delay(50);
        digitalWrite(BUZZER, LOW);
        delay(50);
    }
}

/************************************************************************/
/*Function: Run the Pump for 20 seconds, stop and wait for second       */
/************************************************************************/
void runPump()
{
        digitalWrite(PUMP, HIGH);
        delay(20000);
        digitalWrite(PUMP, LOW);
        delay(1000);
}

/************************************************************************/
/*Function: Determine whether the sensor is exposed to the water        */
/*Parameter:-void                                                       */
/*Return:   -boolean,if it is exposed to the water,it will return true. */
/************************************************************************/
boolean isExposedToWater()
{
  int sensorValue = 0;        // value read from the water sensor
  sensorValue = analogRead(WATER_SENSOR);
  if(sensorValue < 400)
  return true;
  else return false;  
    
    //if(digitalRead(WATER_SENSOR) == LOW)
    //return true;
    //else return false;
}


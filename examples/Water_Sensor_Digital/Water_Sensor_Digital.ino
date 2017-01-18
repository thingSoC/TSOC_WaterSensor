/*macro definition of water sensor and the buzzer*/
#define LED          13 // 13
#define WATER_SENSOR 7 // A0
#define BUZZER       2  // 7
#define PUMP         8 // 9, 19

#define PUMP_MAX_ON_TIME 120000L // 2 minutes max on time...

int ledState = LOW;
int waterState = LOW;
int buzzState = LOW;
int pumpState = LOW;

unsigned long ledPrevious   = 0;
unsigned long waterPrevious = 0;
unsigned long buzzPrevious  = 0;
unsigned long pumpPrevious  = 0;

unsigned long ledInterval   = 250;
unsigned long waterInterval = 250;
unsigned long buzzInterval  = 60;
unsigned long pumpInterval  = 20000;
unsigned long pumpRestTime  = 0;

int waterSensor         = HIGH;
int waterSensorPrevious = HIGH;

unsigned long waterTotalTime = 0;

void setup()
{
    pins_init();
}
void loop()
{ 
    if(isExposedToWater(waterInterval)) {
      blinkLED(ledInterval);
      runPump(pumpInterval);
      soundAlarm(buzzInterval);
    } else {
      blinkLED(0);
      runPump(0);
      soundAlarm(0);
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
/************************************************************************/
void soundAlarm(unsigned long Interval)
{
  unsigned long currentMillis = millis();
  
  if (Interval == 0) {
    // it's off...
    buzzState = LOW;
  } else {
    if (currentMillis - buzzPrevious >= Interval) {
      // save the last time you buzzed
      buzzPrevious = currentMillis;
      if (buzzState == LOW) {
        buzzState = HIGH;
      } else {
        buzzState = LOW;
      }
    }
  }
  digitalWrite(BUZZER, buzzState);
}

/************************************************************************/
/*Function: Keep track of how long the pump is running continuously     */
/************************************************************************/
void runPump(unsigned long Interval)
{
  unsigned long currentMillis = millis();
  if (Interval == 0) {
    pumpState = LOW;
  } else {
    if (currentMillis - pumpPrevious >= Interval) {
      // save the last time you pumped
      pumpPrevious = currentMillis;
      if (pumpState == LOW) {
        pumpState = HIGH;
       } else {
        pumpState = LOW;
      }
    }    
  }
  if (waterTotalTime > PUMP_MAX_ON_TIME) {
    pumpState = LOW;
    waterTotalTime = 0;
    digitalWrite(PUMP, pumpState);
    ledState = HIGH;
    digitalWrite(LED, ledState);
    // let the pump motor rest for a few minutes, then try again
    // simple way to do that is to block...
    delay(60000);
    ledState = LOW;
    digitalWrite(LED, ledState);
  }
  digitalWrite(PUMP, pumpState);
}

/************************************************************************/
/*Function: blink the LED                                               */
/************************************************************************/
void blinkLED(unsigned long Interval)
{
  unsigned long currentMillis = millis();
  
  if (Interval == 0) {
    // it's off...
    ledState = LOW;
  } else {
    // blink at the interval
    if (currentMillis - ledPrevious >= Interval) {
      // save the last time you blinked the LED
      ledPrevious = currentMillis;
      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
    }
  }
  digitalWrite(LED, ledState);
}

/************************************************************************/
/*Function:  Determine whether the sensor is exposed to the water       */
/*Parameter: Interval to test over                                      */
/*Return:   -boolean,if it is exposed to the water,it will return true. */
/************************************************************************/
boolean isExposedToWater(unsigned long Interval)
{
  unsigned long currentMillis = millis();
  unsigned long deltaMillis = currentMillis - waterPrevious;
  
  if (Interval == 0) {
    // immediate sampling
    waterState = waterSensor = waterSensorPrevious = !digitalRead(WATER_SENSOR);
    waterTotalTime = 0;
  } else {
    // interval sampling
    if (deltaMillis >= Interval) {
      // save the last time you sampled the water sensor
      waterPrevious = currentMillis;
      waterSensorPrevious = waterSensor;
      waterSensor = !digitalRead(WATER_SENSOR);
      if ((waterSensor == HIGH) && (waterSensorPrevious == HIGH)) {
        waterTotalTime += deltaMillis;
        waterState = HIGH;
      } else {
        waterState = LOW;
        waterTotalTime = 0;
      }
    }    
  }
   
  if (waterState)
    return true;
  else 
    return false;
}


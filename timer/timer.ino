#include <Time.h>
#include <TimeAlarms.h>

boolean doneOnce=false;

void setup()
{
  Serial.begin(9600);
  Serial.println("In Setup");
  Alarm.timerOnce(10,OnceOnly); // called once after 10 seconds
  return;
}

void  loop()
{
  Alarm.delay(1000); // this function MUST be called repeatedly for the alarms/timers to work
  if(!doneOnce)
  {
    doneOnce=true;
    Serial.println("This message will only get printed one time");
  }
  Alarm.timerOnce(10,NotSoGood); // alarm setup repeatedly
  return;
}

void OnceOnly()
{
  Serial.println("This timer only triggers once");
  return;
}

void NotSoGood()
{
  Serial.println("Alarm set in loop and so called repeatedly");
  return;
}

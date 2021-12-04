#ifndef CapSense_h
#define CapSense_h

#include <Arduino.h>
#include <CapacitiveSensor.h>

class CapSense
{
  public:
    CapSense(int pin1, int pin2, long tshd);
    bool rose();
    bool fell();
    long read();
    
   private:
    long _val;
    
    long _newTimer;
    long _oldTimer;
    long _timerT;

    long _tshd;
    long _oldR;
    long _oldF;
    CapacitiveSensor* _cs;
};

#endif

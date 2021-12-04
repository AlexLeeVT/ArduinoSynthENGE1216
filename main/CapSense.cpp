#include <CapacitiveSensor.h>

#include "Arduino.h"
#include "CapSense.h"

CapSense::CapSense(int pin1, int pin2, long tshd) {
  _cs = new CapacitiveSensor(pin1,pin2);
  _val = 0;
  _oldR = 0;
  _oldF = 0;
  _tshd = tshd;
  _timerT = 30;
  _oldTimer = 0;
  _newTimer = 0;
}

long CapSense::read() {
  _newTimer = millis();
  _val = _cs->capacitiveSensor(30);
  return _val;
}

//low to high
bool CapSense::rose() {
  bool isRose = false;
  if((_val >= _tshd && _oldR < _tshd) && //check if the value is within the constraints 
          ((_newTimer - _oldTimer) >= _timerT)
    ){
      _oldTimer = _newTimer; //reset debounce timer
      isRose = true;
  }
  _oldR = _val; //reset old pin value
  return isRose;
}

//high -> low
bool CapSense::fell() {
  bool isFell = false;
  if(_val < _tshd && _oldF >= _tshd) {
    isFell = true;
  }
  _oldF = _val;
  return isFell;
}
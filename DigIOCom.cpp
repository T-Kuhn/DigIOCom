/*
  DigOutCom.cpp - Library for intermicrocontroller communication 
  using only digital inputs and outputs
  Created by Tobias Kuhn. Sapporo, December 19, 2015.
  Released into the public domain.
*/

#include "Arduino.h"
#include "DigIOCom.h"

// - - - - - - - - - - - - - - - - - - -
// - - - - DigIOCom CONSTRUCTOR  - - - -
// - - - - - - - - - - - - - - - - - - -
DigIOCom::DigIOCom(bool reciever, int pin1, int pin2, int period)
{
    _period = period;
    _thresholdTime = millis() + _period / 2;
    _clockCounter = 1;
    _pin1 = pin1;   // signal pin
    _pin2 = pin2;   // clock pin
    _rec = reciever;  // is the object created here a reciever or a transmitter?
    _charSignal = 0x05;
    if(_rec == false){
        pinMode(_pin1, OUTPUT);
        pinMode(_pin2, OUTPUT);
    }else{
        pinMode(_pin1, INPUT);
        pinMode(_pin2, INPUT);
    }
}

// - - - - - - - - - - - - - - - - - - -
// - - - - - DigIOCom UPDATE - - - - - -
// - - - - - - - - - - - - - - - - - - -
void DigIOCom::_update()
{
    long tmp = millis();
    if (_thresholdTime < tmp){
        // The operations in here are only executed 1 time every half a period
        _thresholdTime = tmp + _period / 2;
        _clockCounter++;
        _invertClock();
        _updateSignal();    // update signal variable here
        _inputCounter = 0;
        digitalWrite(_pin1, _signal);
        digitalWrite(_pin2, _clock);
    }
}

// - - - - - - - - - - - - - - - - - - -
// - - -  DigIOCom INVERT CLOCK  - - - -
// - - - - - - - - - - - - - - - - - - -
void DigIOCom::_invertClock(){
    if(_clockCounter < 18){
        if(_clock == 1){
            _clock = 0;
        }else{
            _clock = 1;
        }
    }
}

// - - - - - - - - - - - - - - - - - - -
// - - -  DigIOCom UPDATE SIGNAL - - - -
// - - - - - - - - - - - - - - - - - - -
void DigIOCom::_updateSignal(){
    if(_clockCounter % 2 == 0){
        _signal = _charSignal & 1U;
        _charSignal >>= 1;   //shift the thing 1 to the right
    }
}

// - - - - - - - - - - - - - - - - - - -
// - - - - DigIOCom SEND BYTE  - - - - -
// - - - - - - - - - - - - - - - - - - -
void DigIOCom::sendByte(unsigned char data){
    _charSignal = data;
    _clockCounter = 1;
    while(_clockCounter <= 18){
       _update(); 
    }
}

// - - - - - - - - - - - - - - - - - - -
// - - - - - DigIOCom LISTEN - - - - - -
// - - - - - - - - - - - - - - - - - - -
unsigned char DigIOCom::listen(){
    unsigned char tmp = 0x00;
    //long startTimer = millis();
    bool flag = false;
    unsigned int clockIn = LOW;
    _inputCounter = 0;
    unsigned char mask = 0x01;
    while(_inputCounter < 8){
        clockIn = digitalRead(_pin2);        
        if(clockIn == LOW && flag == true){
            // this code is executed once every falling edge
            flag = false;
            _inputCounter++; 
            if (digitalRead(_pin1) == HIGH){
                tmp = tmp | mask; // add the new bit (true) 
                mask <<= 1;      // shift the mask 1 to the left
            }else{
                mask <<= 1;      // shift the mask 1 to the left           
            }
        }else if(clockIn == HIGH && flag == false){
            flag = true;
        }        
    }
    return tmp;
}




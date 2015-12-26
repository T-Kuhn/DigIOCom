/*
  DigOutCom.h - Library for intermicrocontroller communication 
  using only digital inputs and outputs
  Created by Tobias Kuhn. Sapporo, December 19, 2015.
  Released into the public domain.
*/

#ifndef DigIOCom_h
#define DigIOCom_h

#include "Arduino.h"
// - - - - - - - - - - - - - - - - - - -
// - - - - - DigIOCom CLASS  - - - - - -
// - - - - - - - - - - - - - - - - - - -
class DigIOCom
{
    public:
        DigIOCom(bool reciever, int pin1, int pin2, int period);
        void sendByte(unsigned char data);
        unsigned char listen();
    private:
        int _pin1;
        int _pin2;
        bool _rec; 
        long _period;
        int _clock;
        unsigned char _charSignal;
        int _signal;
        int _clockCounter;
        int _inputCounter;
        long _thresholdTime;
        void _update();
        void _invertClock();
        void _updateSignal();
};

#endif














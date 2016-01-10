/*
  DigOutCom_H8.h - Library for intermicrocontroller communication 
  using only digital inputs and outputs.
  Created by Tobias Kuhn. Sapporo, January 10. 2016.
  Released into the public domain.
*/

// - - - - - - - - - - - - - - - - - - -
// - - - -  SEND BYTE  - - - - -
// - - - - - - - - - - - - - - - - - - -
void sendByte(unsigned char data, unsigned long period){

    unsigned long _timeCounter;
    unsigned long _period;
    unsigned long _thresholdClockCount;
    unsigned char _charSignal;
    int _clock;
    int _signal;
    int _clockCounter;
 
    _clock = 0;
    _signal = 0;
    _timeCounter = 0;
    _period = period;
    _thresholdClockCount = _period / 2;

    _charSignal = data;
    _clockCounter = 1;

    while(_clockCounter <= 18){
        unsigned long tmp = _timeCounter++;
        if (_thresholdClockCount < tmp){
            // The operations in here are only executed 1 time every half a period
            _thresholdClockCount = tmp + _period / 2;
            _clockCounter++;
            if(_clockCounter < 18){
                if(_clock == 1){
                    _clock = 0;
                }else{
                    _clock = 1;
                }
            }
            if(_clockCounter % 2 == 0){
                _signal = _charSignal & 1U;
                _charSignal >>= 1;   //shift the thing 1 to the right
            }

        
            digitalWrite(12, _signal);
            digitalWrite(13, _clock);

            //output0(_signal);
            //output1(_clock);
        }
    }
}




/*
  DigOutCom.h - Library for intermicrocontroller communication 
  using only digital inputs and outputs
  Created by Tobias Kuhn. Sapporo, December 19, 2015.
  Released into the public domain.
*/

// - - - - - - - - - - - - - - - - - - -
// - - - - - DigIOCom CLASS  - - - - - -
// - - - - - - - - - - - - - - - - - - -
    void DigIOCom(int reciever, int pin1, int pin2, int period);
    void sendByte(unsigned char data);
    unsigned char listen();
	
    unsigned long _getTimeCount();
    unsigned long _timeCounter;
    int _pin1;
    int _pin2;
    int _rec; 
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


// - - - - - - - - - - - - - - - - - - -
// - - - - DigIOCom CONSTRUCTOR  - - - -
// - - - - - - - - - - - - - - - - - - -
void DigIOCom(int reciever, int pin1, int pin2, int period)
{
    _timeCounter = 0;
    _period = period;
    _thresholdTime = _period / 2;
    _clockCounter = 1;
    _pin1 = pin1;   // signal pin
    _pin2 = pin2;   // clock pin
    _rec = reciever;  // is the object created here a reciever or a transmitter?
    _charSignal = 0x05;
    //if(_rec == false){
        //pinMode(_pin1, OUTPUT);
        //pinMode(_pin2, OUTPUT);
    //}else{
        //pinMode(_pin1, INPUT);
        //pinMode(_pin2, INPUT);
    //}
}

// - - - - - - - - - - - - - - - - - - -
// - - - - - DigIOCom UPDATE - - - - - -
// - - - - - - - - - - - - - - - - - - -
void _update()
{
    unsigned long tmp = _getTimeCount();
    if (_thresholdTime < tmp){
        // The operations in here are only executed 1 time every half a period
        _thresholdTime = tmp + _period / 2;
        _clockCounter++;
        _invertClock();
        _updateSignal();    // update signal variable here
        _inputCounter = 0;
        //digitalWrite(_pin1, _signal);
        //digitalWrite(_pin2, _clock);
        output0(_signal);
        output1(_clock);
    }
}

// - - - - - - - - - - - - - - - - - - -
// - -  DigIOCom GET TIME COUNT  - - - -
// - - - - - - - - - - - - - - - - - - -
unsigned long _getTimeCount(){
    return _timeCounter++;
}


// - - - - - - - - - - - - - - - - - - -
// - - -  DigIOCom INVERT CLOCK  - - - -
// - - - - - - - - - - - - - - - - - - -
void _invertClock(){
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
void _updateSignal(){
    if(_clockCounter % 2 == 0){
        _signal = _charSignal & 1U;
        _charSignal >>= 1;   //shift the thing 1 to the right
    }
}

// - - - - - - - - - - - - - - - - - - -
// - - - - DigIOCom SEND BYTE  - - - - -
// - - - - - - - - - - - - - - - - - - -
void sendByte(unsigned char data){
    _charSignal = data;
    _clockCounter = 1;
    while(_clockCounter <= 18){
       _update(); 
    }
}













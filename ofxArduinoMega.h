#include "ofArduino.h"

// board settings
#define ARD_TOTAL_DIGITAL_PINS	54 // total number of pins currently supported
#define ARD_TOTAL_ANALOG_PINS	16
#define ARD_TOTAL_PORTS   6 // total number of ports for the board

class ofxArduinoMega : public ofArduino
{
public:
    void initPins();
protected:
    void sendDigital(int pin, int value, bool force = false);
};

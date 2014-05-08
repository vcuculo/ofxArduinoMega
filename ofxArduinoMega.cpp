#include "ofxArduinoMega.h"
#include "ofUtils.h"

// initialize pins once we get the Firmata version back from the Arduino board
// the version is sent automatically by the Arduino board on startup
void ofxArduinoMega::initPins()
{
    int firstAnalogPin;

    if (_initialized) return;   // already initialized

    // support Firmata 2.3/Arduino 1.0 with backwards compatibility
    // to previous protocol versions
    if (_firmwareVersionSum >= FIRMWARE2_3)
    {
        _totalDigitalPins = 20;
        firstAnalogPin = 14;
    }
    else
    {
        _totalDigitalPins = ARD_TOTAL_DIGITAL_PINS;
        firstAnalogPin = 16;
    }

    // ports
    for(int i=0; i<ARD_TOTAL_PORTS; ++i)
    {
        _digitalPortValue[i]=0;
        _digitalPortReporting[i] = ARD_OFF;
    }

    // digital pins
    for(int i=0; i<ARD_TOTAL_DIGITAL_PINS; ++i)
    {
        _digitalPinValue[i] = -1;
        _digitalPinMode[i] = ARD_OUTPUT;
        _digitalPinReporting[i] = ARD_OFF;
    }

    // analog in pins
    for (int i=firstAnalogPin; i<_totalDigitalPins; ++i)
    {
        _analogPinReporting[i-firstAnalogPin] = ARD_OFF;
        // analog pins used as digital
        _digitalPinMode[i]=ARD_ANALOG;
        _digitalPinValue[i] = -1;
    }

    for (int i=0; i<_totalDigitalPins; ++i)
    {
        _servoValue[i] = -1;
    }

    _initialized = true;
}

void ofxArduinoMega::sendDigital(int pin, int value, bool force)
{
    if((_digitalPinMode[pin]==ARD_INPUT || _digitalPinMode[pin]==ARD_OUTPUT) && (_digitalPinValue[pin]!=value || force))
    {

        _digitalPinValue[pin] = value;

        int port=0;
        int bit=0;
        int port1Offset;
        int port2Offset;

        // support Firmata 2.3/Arduino 1.0 with backwards compatibility
        // to previous protocol versions
        if (_firmwareVersionSum >= FIRMWARE2_3)
        {
            port1Offset = 16;
            port2Offset = 20;
        }
        else
        {
            port1Offset = 14;
            port2Offset = 22;
        }


        if(pin > 1 && pin <8)
        {
            port=0;
            bit = pin;
        }
        else if(pin <16)
        {
            port = 1;
            bit = pin-8;
        }
        else if(pin <24)
        {
            port = 2;
            bit = pin-16;
        }
        else if(pin <32)
        {
            port = 3;
            bit = pin-24;
        }
        else if(pin <40)
        {
            port = 4;
            bit = pin-32;
        }
        else if(pin <48)
        {
            port = 5;
            bit = pin-40;
        }
        else if(pin <54)
        {
            port = 6;
            bit = pin-48;
        }

        // set the bit
        if(value==1)
            _digitalPortValue[port] |= (1 << bit);

        // clear the bit
        if(value==0)
            _digitalPortValue[port] &= ~(1 << bit);

        sendByte(FIRMATA_DIGITAL_MESSAGE+port);
        sendValueAsTwo7bitBytes(_digitalPortValue[port]);

    }
}

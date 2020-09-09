#ifndef __BUTTON_FKT__
#define __BUTTON_FKT__

#include <arduino.h>

class buttonFkt{
  private:
    bool hold1, hold2, hold3, click, doubleclick, BttnAct, BttnOld;
    int timeClick, timeHold1, timeHold2, timeHold3;
    uint8_t pin;
    unsigned long oldTimeTon, oldTimeTof;

  public:
    //contructor
    buttonFkt(int timeClick, int timeHold1, int timeHold2, int timeHold3); 

    //methods
    void refresh(bool in);
    
    //getter
    bool getHold1();
    bool getHold2();
    bool getHold3();
    bool getClick();
    bool getDoubleclick();
    bool getBttn();
    void reset();
};

#endif
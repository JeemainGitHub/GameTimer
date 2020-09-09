#include "buttonFkt.h"

//constuctor
//pin = PinNumber, other are times for click / hold button
buttonFkt::buttonFkt(int timeClick = 100, int timeHold1 = 500, int timeHold2 = 1000, int timeHold3 = 1500){
  this->timeClick = timeClick;
  this->timeHold1 = timeHold1;
  this->timeHold2 = timeHold2;
  this->timeHold3 = timeHold3;
}

//refresh button status
void buttonFkt::refresh(bool in){
  BttnAct= in;

  if (BttnAct & !BttnOld){
    oldTimeTon = millis();
  }
  else if ( !BttnAct & BttnOld){
    hold3 = ((millis() - oldTimeTon) > 1500);
    hold2 = ((millis() - oldTimeTon) > 1000) & !hold3;
    hold1 = ((millis() - oldTimeTon) > 500) & !hold3 & !hold2;
    click = ((millis() - oldTimeTon) > 100) & !hold3 & !hold2 & !hold1;
  }
  else{
    hold3 = false;
    hold2 = false;
    hold1 = false;
    click = false;
  }

  BttnOld = BttnAct;
}

//button pressed for hold time 1
bool buttonFkt::getHold1(){
  return this->hold1;
}

//button pressed for hold time 2
bool buttonFkt::getHold2(){
  return this->hold2;
}

//button pressed for hold time 3
bool buttonFkt::getHold3(){
  return this->hold3;
}

//button clicked
bool buttonFkt::getClick(){
  return this->click;
}

//button doubleclicked
bool buttonFkt::getDoubleclick(){
  return this->doubleclick;
}

//actual button status
bool buttonFkt::getBttn(){
 return this->BttnAct; 
}

//reset button status
void buttonFkt::reset(){
  this->click = false; 
  this->doubleclick = false; 
  this->hold1 = false; 
  this->hold2 = false; 
  this->hold3 = false; 
}
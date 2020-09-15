// use the cRGB struct hsv method
#define USE_HSV
#include <Arduino.h>
#include <WS2812.h>
#include <buttonFkt.h>
#include <EEPROM.h>

#define LEDCount 16
#define outputPin PB1
#define outputTone PB0
#define outputLED PB3
#define inputPin PB4

//Objects
WS2812 LED(LEDCount); 
cRGB value;
buttonFkt button1(100, 500, 1000, 1500);

//Functions
void Rainbow(int update=50, int steps=1);
void Full(byte r, byte g, byte b);
void Half(byte r, byte g, byte b);
void Toggle(byte r, byte g, byte b, int t, int l);
void Clock1(byte r, byte g, byte b, int t);
void Clock2(byte r1, byte g1, byte b1, byte r2, byte g2, byte b2, unsigned long time, byte ledNo);
void Clock3(byte r, byte g, byte b, unsigned long val, byte ledNo);
void Acls();
void Black();
void Menue(byte mode, byte no);

//Variables
static int mode = 0;
static byte setupBrightness;  //brightness value
static bool setupTone;  //tone on or off
static int setupTimerVal; //timer value
static unsigned long oldMillis;
static unsigned long pausedMillis;
static byte tmpbyte;
unsigned long tmpdword;

//SETUP
void setup() {
  LED.setOutput(outputPin);
  pinMode(inputPin, INPUT);

  //get Parameters from EEPROM
  setupBrightness = EEPROM.read(0);
  setupTone       = EEPROM.read(1);
  setupTimerVal   = EEPROM.read(2);
  setupTimerVal   |= (word)EEPROM.read(3)<<8;
  if (setupTimerVal == 0) setupTimerVal = 60;
}

//LOOP
void loop() {
  button1.refresh(digitalRead(inputPin));

  switch (mode)
  {
  case 0:  //main menue
    Menue(mode, 4);
    if (button1.getClick())
      mode++;
    break;
  case 1:  //main menue select 1
    Menue(mode, 4);
    if (button1.getClick())
      mode++;
    if (button1.getHold1())
      mode = 10;
    break;
  case 2:  //main menue select 2
    Menue(mode, 4);
    if (button1.getClick())
      mode++;
    if (button1.getHold1()){
      button1.reset();
      mode = 20;
    }
    break;
  case 3: //main menue select 3
    Menue(mode, 4);
    if (button1.getClick())
      mode++;
    if (button1.getHold1()){
      button1.reset();
      mode = 30;
    }
    break;
  case 4:  //main menue select 4
    Menue(mode, 4);
    if (button1.getClick())
      mode = 1;
    if (button1.getHold1())
      mode = 40;
    break;
  case 10:  //acls running
    Acls();
    if (button1.getHold1())
      mode = 1;
    break;
  case 20:  //timer Stop
    oldMillis = millis() + (unsigned long)setupTimerVal * 1000;
    Toggle(20,0,0,2000,1);
    if (button1.getClick())
      mode = 21;
    if (button1.getHold1())
      mode = 2;
    if (button1.getHold3())
      {
        tmpbyte = 1;
        mode = 24;
      }
    break;
  case 21:  //timer run
    tmpdword = (oldMillis - millis()) * 10ul / (setupTimerVal);
    Clock3(0,100,0,tmpdword, LEDCount);
    if (button1.getClick()){
      mode = 22;
      pausedMillis = oldMillis - millis();
    }
    if ((oldMillis <= millis()) ){
      Full(255,0,0);
      tone(outputTone,1000,500);
      mode = 23;
    }
    else if (button1.getHold1() ){
      Full(255,0,0);
      mode = 23;
    }
    break;
  case 22:  //timer pause
    oldMillis = pausedMillis + millis() ;
    if (button1.getClick())
      mode = 21;
    if (button1.getHold1()){
      Full(255,0,0);
      mode = 23;
    }
    break;
  case 23:  //timer alarm
    delay(1000);
    mode = 20;
    break;
  case 24:  //timer setup seconds
     if (button1.getClick()){
       tmpbyte = tmpbyte + 1;
       if (tmpbyte > 11){
         tmpbyte = 0;
       }
     }
     tmpdword = (unsigned long)tmpbyte * 625ul;
     Clock3(0,25,25,tmpdword,LEDCount);
     if (button1.getHold1()){
       setupTimerVal = (int)tmpbyte * 5;
       tmpbyte = 0;
       mode = 25;
     }
    break;
  case 25:  //timer setup minutes
     if (button1.getClick()){
       tmpbyte = tmpbyte + 1;
       if (tmpbyte >5)
         tmpbyte = 0;
     }
     tmpdword = (unsigned long)tmpbyte * 625ul;
     Clock3(0,0,50,tmpdword,LEDCount);
     if (button1.getHold1()){
       setupTimerVal += ((int)tmpbyte * 60);
       tmpbyte = 0;
       mode = 26;
     }
     break;
  case 26:  //timer setup save
    EEPROM.write(2, lowByte(setupTimerVal));
    delay(50);
    EEPROM.write(3, highByte(setupTimerVal));
    delay(50);
    mode = 20; 
    break;
  case 30:  //light scene 1
    Toggle(setupBrightness,0,0,255,1);
    if (button1.getClick())
      mode++;
    if (button1.getHold1())
      mode = 3;
    break;
  case 31:  //light scene 2
    Toggle(setupBrightness,setupBrightness,0,255,1);
    if (button1.getClick())
      mode++;
    if (button1.getHold1())
      mode = 3;
    break;
  case 32:  //light scene 3
    Toggle(0,setupBrightness,0,255,1);
    if (button1.getClick())
      mode++;
    if (button1.getHold1())
      mode = 3;
    break;
  case 33:  //light scene 4
    Toggle(0,setupBrightness,setupBrightness,255,1);
    if (button1.getClick())
      mode++;
    if (button1.getHold1())
      mode = 3;
    break;
  case 34:  //light scene 5
    Toggle(0,0,setupBrightness,255,1);
    if (button1.getClick())
      mode++;
    if (button1.getHold1())
      mode = 3;
    break;
  case 35:  //light scene 6
    Toggle(setupBrightness,0,setupBrightness,255,1);
    if (button1.getClick())
      mode++;
    if (button1.getHold1())
      mode = 3;
    break;
  case 36:  //light scene 7
    Full(setupBrightness,0,0);
    if (button1.getClick())
      mode++;
    if (button1.getHold1())
      mode = 3;
    break;
  case 37:  //light scene 8
    Full(0,setupBrightness,0);
    if (button1.getClick())
      mode++;
    if (button1.getHold1())
      mode = 3;
    break;
  case 38:  //light scene 9
    Full(0,0,setupBrightness);
    if (button1.getClick())
      mode++;
    if (button1.getHold1())
      mode = 3;
    break;
  case 39:  //light scene 10
    Rainbow();
    if (button1.getClick())
      mode = 30;
    if (button1.getHold1())
      mode = 3;
    break;
  case 40:  //setup brightness "10%"
    setupBrightness = 10;
    Clock3(map(setupBrightness,0,100,0,255),map(setupBrightness,0,100,0,255),0,(unsigned long)setupBrightness * 100,LEDCount);
    if (button1.getClick())
      mode = 41;
    if (button1.getHold1())
      mode = 49;
    break;
  case 41:  //setup brightness "25%"
    setupBrightness = 25;
    Clock3(map(setupBrightness,0,100,0,255),map(setupBrightness,0,100,0,255),0,(unsigned long)setupBrightness * 100,LEDCount);
    if (button1.getClick())
      mode = 42;
    if (button1.getHold1())
      mode = 49;
    break;
  case 42:  //setup brightness "50%"
    setupBrightness = 50;
    Clock3(map(setupBrightness,0,100,0,255),map(setupBrightness,0,100,0,255),0,(unsigned long)setupBrightness * 100,LEDCount);
    if (button1.getClick())
      mode = 43;
    if (button1.getHold1())
      mode = 49;
    break;
  case 43:  //setup brightness "75%"
    setupBrightness = 75;
    Clock3(map(setupBrightness,0,100,0,255),map(setupBrightness,0,100,0,255),0,(unsigned long)setupBrightness * 100,LEDCount);
    if (button1.getClick())
      mode = 44;
    if (button1.getHold1())
      mode = 49;
    break;
  case 44:  //setup brightness "100%"
    setupBrightness = 100;
    Clock3(map(setupBrightness,0,100,0,255),map(setupBrightness,0,100,0,255),0,(unsigned long)setupBrightness * 100,LEDCount);
    if (button1.getClick())
      mode = 40;
    if (button1.getHold1())
      mode = 49;
    break;
  case 49:  //setup brightness "save"
    EEPROM.write(0,setupBrightness);
    delay(50);
    mode = 4;
    break;
  case 99:
    Black();
    break;
  }
}

//Turn all LED's off
void Black(){
  bool tmp = false;
  value.SetHSV(0,0,0);

  //check, if led's are black
  for(int i = 0; i < LEDCount; i++)
  {
    value = LED.get_crgb_at(i);
    tmp = tmp or value.r or value.g or value.b;
  }

  //actualize only, when led not black before
  if (tmp){
    value.SetHSV(0,0,0);
    for(int i = 0; i < LEDCount; i++)
    {
      LED.set_crgb_at(i, value);
    }
    LED.sync();
  }
}

//Show Rainbow colors in color cycle
//t=update time
void Rainbow(int update, int steps)
{
  static unsigned long oldTime;
  static int h = 1;
  value.SetHSV(h, 255, 255);
  
  if (oldTime < (millis() - update))
  {
    oldTime = millis();
    h += steps;  
    if(h > 360)
    {
      h %= 360;
    }

    for(int i = 0; i < LEDCount; i++)
    {
      LED.set_crgb_at(i, value);
    }
    // Sends the data to the LEDs
    LED.sync();
  }
}

//Change color of all LED's
void Full(byte r, byte g, byte b){
  bool tmp;
  cRGB tmpColor;
  value.r = r;
  value.g = g;
  value.b = b;

  //check, if led's are black
  for(int i = 0; i < LEDCount; i++)
  {
    tmpColor = LED.get_crgb_at(i);
    tmp = tmp or (value.r != tmpColor.r) or (value.b != tmpColor.b) or (value.b != tmpColor.b);
  }

  //Update, if color is different
  if (tmp){
    for(int i = 0; i < LEDCount; i++)
    {
        LED.set_crgb_at(i, value);
    }
    // Sends the data to the LEDs
    LED.sync();
  }
}

//Change color of every 2nd LED's
void Half(byte r, byte g, byte b){
  Black();
  cRGB tmpColor;
  bool tmp = false;
  value.r = r;
  value.g = g;
  value.b = b;
 
  //check, if led's are black
  for(int i = 0; i < LEDCount; i++)
  {
    tmpColor = LED.get_crgb_at(i);
    tmp = tmp or (value.r != tmpColor.r) or (value.b != tmpColor.b) or (value.b != tmpColor.b);
  }

  if (tmp){
    for(int i = 0; i < LEDCount; i+=2)
    {
        LED.set_crgb_at(i, value);
    }
    // Sends the data to the LEDs
    LED.sync();
  }
}

//Change color of every 2nd LED's and toggle
//r=red, g=green, b=blue, t=toggle time, l=free lamps
void Toggle(byte r, byte g, byte b, int t, int l){
  static bool toggle;
  static unsigned long oldTime;
  int i;
  value.SetHSV(0,0,0);

   if (oldTime < (millis() - t)){
    oldTime = millis();
    Black();

    value.r = r;
    value.g = g;
    value.b = b;
    toggle ? i = 0 : i = 1;
    while( i < LEDCount)
    {
      LED.set_crgb_at(i, value);
      i+=l+1;
    }
    // Sends the data to the LEDs
    LED.sync();
    toggle = !toggle;  
  }
}

//Turns on/off LED's one after another
//r = red, g = green, b = blue, t = delay
void Clock1(byte r, byte g, byte b, int t){
  static bool upDown = false;
  static unsigned long oldTime;
  static byte i = 0;
  static byte red, green, blue;
  static bool first;

  if (oldTime < (millis() - t)){
    oldTime = millis();
    if (!first){
      first = true;
      red = r;
      green = g;
      blue = b;
    }

    if (!upDown){
      value.r = red;
      value.g = green;
      value.b = blue;
      LED.set_crgb_at(i, value);
    }
    else{
      value.r = red;
      value.g = green;
      value.b = blue;
      LED.set_crgb_at(i, value);
    }
    i++;
    if (i >= LEDCount){
      byte temp; 
      upDown = !upDown;
      temp = red;
      red = green;
      green = blue;
      blue = temp;
      i = 0;
    }

    LED.sync();
  }
}

//Display time on LED-Ring
//cursor 1 for sek: r1 = red, g1 = green, b1 = blue
//cursor 2 for cycles: r2 = red, g2 = green, b2 = blue
//time = time [ms] to display, ledNo = No of leds
void Clock2(byte r1, byte g1, byte b1, byte r2, byte g2, byte b2, unsigned long time, byte ledNo){
  cRGB tmpcolor;
  byte sek, cycles;
  static unsigned long oldTime = 0;

  cycles = (time / 1000UL) / (ledNo+1);
  sek    = (time / 1000UL) % (ledNo+1);

  //Do in case of time change
  if (oldTime != time){
    for (int i=0;i<ledNo;i++){
      //set pixel for cursor 1 and 2
      if ( ((cycles-(byte)1 ) >= i) & ((sek-(byte)1 ) == i) ){
        tmpcolor.r = (r1 + r2);
        tmpcolor.g = (g1 + g2);
        tmpcolor.b = (b1 + b2);
        LED.set_crgb_at(i, tmpcolor);
      }
      //set pixel for cursor 2
      else if ((cycles-(byte)1 ) >= i){
        tmpcolor.r = r2;
        tmpcolor.g = g2;
        tmpcolor.b = b2;
        LED.set_crgb_at(i, tmpcolor);
      }
      //set pixel for cursor 1
      else if ( (sek-(byte)1 ) >= i){
        tmpcolor.r = r1;
        tmpcolor.g = g1;
        tmpcolor.b = b1;
        LED.set_crgb_at(i, tmpcolor);
      }
      else{
        tmpcolor.r = 0;
        tmpcolor.g = 0;
        tmpcolor.b = 0;
        LED.set_crgb_at(i, tmpcolor);
      }
    }
    //Synchronise with neopixel
    LED.sync();
    oldTime = time;
  }
}

//Display value from 0 to 100 % on LED-Ring
//r=red, g=green, b=blue, val=value to display, ledNo = pixels of Led-Ring
void Clock3(byte r, byte g, byte b, unsigned long val, byte ledNo){
  static unsigned long tmpvalueOld = 0;
  unsigned long zwerg;
  int tmpint;
  zwerg = val * (unsigned long)ledNo;
  tmpint = (int)(zwerg/10000ul);
  

  if (tmpvalueOld != val){
    for(int i = 0; i < ledNo; i++)
    {
      if (i == tmpint){
        value.r = byte((zwerg%10000ul) * (unsigned long)r /10000);
        value.g = byte((zwerg%10000ul) * (unsigned long)g /10000);
        value.b = byte((zwerg%10000ul) * (unsigned long)b /10000);
        LED.set_crgb_at(i, value);
      }
      else if (i <= tmpint){
        value.r = r;
        value.g = g;
        value.b = b;
        LED.set_crgb_at(i, value);
      }
      else
      {
        value.r = 0;
        value.g = 0;
        value.b = 0;
        LED.set_crgb_at(i, value);
      }
    }
    LED.sync();
  }
  tmpvalueOld = val;
}

//menue
void Menue(byte mode, byte no){
  //1st mode
  if (mode == 1)
    value.r = 128;
  else
    value.r = 1;

  value.g = 0;
  value.b = 0; 

  for(int i = 0; i < (LEDCount / 4); i++)
  {
    LED.set_crgb_at(i, value);
  }

  //2nd mode
  if (mode == 2)
    value.g = 64;
  else
    value.g = 1;
  value.r = 0;
  value.b = 0; 

  for(int i = (LEDCount / 4); i < (LEDCount / 2); i++)
  {
    LED.set_crgb_at(i, value);
  }

  //3rd mode
  if ( (mode == 3) & (no > 2))
    value.b = 64;
  else
    value.b = 1;
  value.r = 0;
  value.g = 0; 

  for(int i = (LEDCount / 2); i < (LEDCount / 4 * 3); i++)
  {
    LED.set_crgb_at(i, value);
  }

  //4th mode
  if ( (mode == 4) & (no > 3)){
    value.r = 64;
    value.g = 64;
  }
  else{
    value.r = 1;
    value.g = 1;
  }
  value.b = 0;

  for(int i = (LEDCount / 4 * 3); i < LEDCount; i++)
  {
    LED.set_crgb_at(i, value);
  }
  LED.sync();
}


//ACLS 
//120 BPM Hearthmassage = red
//2 min-1 breath = blue
void Acls(){
  static byte syncOld;
  byte sync;

  if ( (millis() % 30000 )> 25000){
    value.r = 0;
    value.g = 0;
    value.b = 255;

    for (int i=0;i<LEDCount;i++){
      LED.set_crgb_at(i,value);
    }
    sync = 1;
  }
  else if ( (millis() % 500 )> 400){
    value.r = 255;
    value.g = 0;
    value.b = 0;

    for (int i=0;i<LEDCount;i++){
      LED.set_crgb_at(i,value);
    }
    sync = 2;
  }
  else
  {
    value.r = 0;
    value.g = 0;
    value.b = 0;

    for (int i=0;i<LEDCount;i++){
      LED.set_crgb_at(i,value);
    }
    sync = 3;
  }

  if (sync !=syncOld){
    LED.sync();
  }
  syncOld = sync;
}
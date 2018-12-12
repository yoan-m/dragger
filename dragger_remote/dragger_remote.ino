#include <RF24.h>
#include "ssd1306.h"
#include "ssd1306_fonts.h"

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

int button_alarme_0 = 4;
int button_alarme_1 = 5;
int button_alarme_2 = 6;
int valPot = -1;
int oldValPot = 0;
int potPin = 2;
int led = 7;
void setup()
{
 // Serial.begin(9600);
  /* Do not init Wire library for Attiny controllers */

  ssd1306_128x64_i2c_init();
  ssd1306_fillScreen(0x00);
  pinMode(button_alarme_0, INPUT_PULLUP);
  pinMode(button_alarme_1, INPUT_PULLUP);
  pinMode(button_alarme_2, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  ssd1306_128x64_i2c_init();
  ssd1306_fillScreen(0x00);
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  ssd1306_printFixedN (0, 10, "SDIS45", STYLE_BOLD, 1);
  ssd1306_printFixedN (20, 40, "Dragger remote", STYLE_NORMAL, 0);
  ssd1306_printFixedN (50, 60, "Yoan MURCIANO", STYLE_ITALIC, 0);
  delay(5000);
  ssd1306_setFixedFont(comic_sans_font24x32_123);
  ssd1306_clearScreen();
}

void loop()
{

  int buttonStateAlarme0 = digitalRead(button_alarme_0);
  int buttonStateAlarme1 = digitalRead(button_alarme_1);
  int buttonStateAlarme2 = digitalRead(button_alarme_2);
  valPot = int((analogRead(potPin) / 1023.0) * 100.0); // read the value from the sensor
  //button is pressed, message should be sent, turn the green LED on
  if (valPot==-1 || (valPot != oldValPot && valPot % 2 == 0)) {
    oldValPot = valPot;

    // Send a message
    uint8_t data[1];
    data[0] = valPot;
    char text[10];
    itoa(valPot, text, 10);
   
    //Serial.println(text);
    radio.write(&text, sizeof(text));
//    ssd1306_fillScreen(0x00);
    char myConcatenation[5];
    char myStr[] = "%";
    if(valPot<10)
    sprintf(myConcatenation,"  %i%s",valPot,myStr);
    else if(valPot<100) 
    sprintf(myConcatenation," %i%s",valPot,myStr);
    else
    sprintf(myConcatenation,"%i%s",valPot,myStr);
    
    ssd1306_printFixedN (12, 20, myConcatenation, STYLE_NORMAL, 0);

    delay(10);
  }
  if (!buttonStateAlarme0) {
    const char text[10] = "a0";
   // Serial.println(text);
    radio.write(&text, sizeof(text));
  }
  if (!buttonStateAlarme1) {
    const char text[10] = "a1";
   // Serial.println(text);
    radio.write(&text, sizeof(text));
  }
  if (!buttonStateAlarme2) {
    const char text[10] = "a2";
    //Serial.println(text);
    radio.write(&text, sizeof(text));
  }
}

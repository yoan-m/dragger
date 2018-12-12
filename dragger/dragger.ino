#include <RF24.h>

#include "ssd1306.h"
#include "ssd1306_fonts.h"



const long interval = 400;
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

int button_acq = 5;
int valPot = 0;
int oldValPot = 0;
int led = 3;
int led2 = 6;
int moteur = 2;
int buzzer = 4;
int alarme = 0;
bool sender = false;
long previousMillis = millis();
bool ledState = LOW;
void setup()
{
  Serial.begin(9600);
 
  ssd1306_128x64_i2c_init();
  ssd1306_fillScreen(0x00);
  pinMode(button_acq, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(moteur, OUTPUT);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
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
  if (radio.available()) {
    char text[10] = "";
    radio.read(&text, sizeof(text));
    if (strcmp(text, "a1") == 0) {
      if (alarme != 1) {
        Serial.println("    alarme 1");
        alarme = 1;
      }
    } else if (strcmp(text, "a2") == 0) {
      if (alarme != 2) {
        Serial.println("    alarme 2");
        alarme = 2;
      }
    } else if (strcmp(text, "a0") == 0) {
      if (alarme != 0) {
        Serial.println("    alarme 0");
        alarme = 0;
        ledState = LOW;
        noTone(buzzer);
        digitalWrite(led, ledState);
        digitalWrite(led2, ledState);
        digitalWrite(moteur, ledState);
      }
    } else {
      Serial.println(text);
      valPot = atoi(text);
    }
  }
  int ac = digitalRead(button_acq);

  //  Serial.println(alarme);
  if (alarme != 0 && ac == LOW && alarme != 2) {
    alarme = 0;
    Serial.println("alarme ack");

    ledState = LOW;
    noTone(buzzer);
    digitalWrite(led, ledState);
    digitalWrite(led2, ledState);
    digitalWrite(moteur, ledState);
  }

  if (alarme) {

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;

      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW) {
        ledState = HIGH;
        tone(buzzer, 4978);
      } else {
        ledState = LOW;
        noTone(buzzer);
      }

      // set the LED with the ledState of the variable:
      digitalWrite(led, ledState);
      digitalWrite(led2, ledState);
      digitalWrite(moteur, ledState);
    }
  }

  if (valPot != oldValPot) {
    Serial.println(valPot);
    
    char myConcatenation[5];
    char myStr[] = "%";
    if(valPot<10)
    sprintf(myConcatenation,"  %i%s",valPot,myStr);
    else if(valPot<100) 
    sprintf(myConcatenation," %i%s",valPot,myStr);
    else
    sprintf(myConcatenation,"%i%s",valPot,myStr);
    
    ssd1306_printFixedN (12, 20, myConcatenation, STYLE_NORMAL, 0);
                        // wait for a second
    oldValPot = valPot;
    delay(10);
  }
}

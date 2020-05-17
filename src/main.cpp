#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
int RECV_PIN = 7; // the pin where you connect the output pin of IR sensor
IRrecv irrecv(RECV_PIN);
decode_results results;

int valA;
int valB;
int valC;

char button;

byte menuCount = 1;
byte dir = 0;

void staticMenu()
{
  display.setTextSize(2);
  display.setTextColor(WHITE);

  display.setCursor(10, 0);
  display.println("Disinfect");
  //---------------------------------
  display.setTextSize(1);
  display.setCursor(10, 20);
  display.println("Sens:");
  display.setCursor(60, 20);
  display.println(valA);

  display.setCursor(10, 30);
  display.println("Time:");
  display.setCursor(60, 30);
  display.println(valB);

  display.setCursor(10, 40);
  display.println("Value C:");
  display.setCursor(60, 40);
  display.println(valC);

  display.setCursor(2, (menuCount * 10) + 10);
  display.println(">");

  display.display();
}

void menuCheck()
{
  //down button routine
  if (button == 'd' && menuCount < 3)
  {
    menuCount++;
    button = ' ';
  }
  if (button == 'd' && menuCount >= 3)
  {
    menuCount = 1;
    button = ' ';
  }
  //up button routine
  if (button == 'u' && menuCount > 1)
  {
    menuCount--;
    button = ' ';
  }
  if (button == 'u' && menuCount <= 1)
  {
    menuCount = 3;
    button = ' ';
  }

  if (button == 'o')
  {
  }
}

void irReceive()
{
  if (irrecv.decode(&results)) // Returns 0 if no data ready, 1 if data ready.
  {
    int value = results.value; // Results of decoding are stored in result.value
    switch (value)
    {
    case 6375:
      button = 'u';
      break;
    case 19125:
      button = 'd';
      break;
    case 4335:
      button = 'l';
      break;
    case 23205:
      button = 'r';
      break;
    case 14535:
      button = 'o';
      break;
    case -23971:
      button = '1';
      break;
    case 25245:
      button = '2';
      break;
    case -7651:
      button = '3';
      break;
    case 8925:
      button = '4';
      break;
    case 765:
      button = '5';
      break;
    case -15811:
      button = '6';
      break;
    case -8161:
      button = '7';
      break;
    case -22441:
      button = '8';
      break;
    case -28561:
      button = '9';
      break;
    case -26521:
      button = '0';
      break;
    default:
      break;
    }
    irrecv.resume();
    Serial.println(value);
  }
}
void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  delay(2000); // Pause for 2 seconds
  irrecv.enableIRIn();
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop()
{
  staticMenu();
  irReceive();
  menuCheck();

  display.clearDisplay();
  delay(50);

  delay(500);
}

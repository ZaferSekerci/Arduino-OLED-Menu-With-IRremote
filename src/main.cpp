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
#define relayPin 8
#define pirPin 9
IRrecv irrecv(RECV_PIN);
decode_results results;

int valA = 2;
int valB = 1;
boolean valC = false;

char button;

const int MaxChars = 5;
char strValue[MaxChars + 1]; // String for 3 digits + null char
int index = 0;
int accumulVal = 0;
int runMode = 1;
byte menuCount = 1;
byte dir = 0;

int relayState = HIGH;
unsigned long previousMillis = 0;
unsigned long OffTime;   // milliseconds of off-time
unsigned long sprayTime; // milliseconds on time

void staticMenu()
{

  display.setTextSize(2);
  display.setTextColor(WHITE);

  display.setCursor(10, 0);
  display.println("OZGUR");
  //---------------------------------
  if (runMode == 1)
    display.setTextColor(BLACK, WHITE);
  else
    display.setTextColor(WHITE);

  display.setTextSize(1);
  display.setCursor(10, 20);
  display.println("Sens: ");
  display.setCursor(60, 20);
  display.print(valA);
  display.println(" sn");

  if (runMode == 2)
    display.setTextColor(BLACK, WHITE);
  else
    display.setTextColor(WHITE);

  display.setCursor(10, 30);
  display.println("Time: ");
  display.setCursor(60, 30);
  display.print(valB);
  display.println(" dk");

  if (runMode == 3)
    display.setTextColor(BLACK, WHITE);
  else
    display.setTextColor(WHITE);

  display.setCursor(10, 40);
  display.println("Surekli: ");
  display.setCursor(60, 40);
  if (valC)
  {
    display.println("Aktif");
  }
  else
  {
    display.println("Pasif");
  }

  display.setCursor(2, (menuCount * 10) + 10);
  display.println(">");

  display.display();
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
  }
}

void resetVal()
{
  for (unsigned int i = 0; i < sizeof(strValue); i++)
  {
    strValue[i] = ' ';
  }
  index = 0;
}

void menuCheck()
{
  //down button routine
  if (button == 'd' && menuCount < 3)
  {
    menuCount++;
    button = ' ';
    resetVal();
  }
  if (button == 'd' && menuCount >= 3)
  {
    menuCount = 1;
    button = ' ';
    resetVal();
  }
  //up button routine
  if (button == 'u' && menuCount > 1)
  {
    menuCount--;
    button = ' ';
    resetVal();
  }
  if (button == 'u' && menuCount <= 1)
  {
    menuCount = 3;
    button = ' ';
    resetVal();
  }
  /*
   * Accumulation
   */
  if (isDigit(button))
  {
    strValue[index] = button; // add the ASCII character to the string
    if (index < MaxChars - 2)
    {
      index++;
    }
    else
    {
      index = 0;
    }
    button = ' ';
    //accumulVal = atoi(strValue);
    switch (menuCount)
    {
    case 1:
      valA = atoi(strValue);
      break;
    case 2:
      valB = atoi(strValue);
      break;
    default:
      break;
    }
  }
  if (button == 'o')
  {
    index = 0; // reset index to receive other data
    switch (menuCount)
    {
    case 1:
      sprayTime = valA * 1000;
      runMode = 1;
      break;
    case 2:
      OffTime = valB * 60000;
      runMode = 2;
      break;
    case 3:
      runMode = 3;
      valC = !valC;
      break;
    default:
      break;
    }
    button = ' ';
  }
}

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  //delay(2000); // Pause for 2 seconds
  irrecv.enableIRIn();
  pinMode(relayPin, OUTPUT);
  pinMode(pirPin, INPUT);
  digitalWrite(relayPin, relayState);
}

void loop()
{
  unsigned long currentMillis = millis();
  staticMenu();
  irReceive();
  menuCheck();
  switch (runMode)
  {
  case 1:
    if (digitalRead(pirPin))
    {
      digitalWrite(relayPin, LOW);
      delay(sprayTime);
    }
    else
    {
      digitalWrite(relayPin, HIGH);
    }
    break;
  case 2:
    if ((relayState == LOW) && (currentMillis - previousMillis >= sprayTime))
    {
      relayState = HIGH;                  // Turn it off
      previousMillis = currentMillis;     // Remember the time
      digitalWrite(relayPin, relayState); // Update the actual relay
    }
    else if ((relayState == HIGH) && (currentMillis - previousMillis >= OffTime))
    {
      relayState = LOW;                   // turn it on
      previousMillis = currentMillis;     // Remember the time
      digitalWrite(relayPin, relayState); // Update the actual relay
    }

    break;
  case 3:
    if (valC)
      digitalWrite(relayPin, LOW);
    else
      digitalWrite(relayPin, HIGH);
    break;

  default:
    break;
  }

  display.clearDisplay();
  //delay(50);
}

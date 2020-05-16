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
  if (irrecv.decode(&results)) // Returns 0 if no data ready, 1 if data ready.
  {
    int value = results.value; // Results of decoding are stored in result.value
    switch (value)
    {
    case -23971:
      digitalWrite(8, HIGH);
      break;
    case 25245:
      digitalWrite(8, LOW);
      break;

    default:
      break;
    }

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.clearDisplay();
    display.setCursor(10, 0);
    display.println(value);
    display.display();
    irrecv.resume();
  }
}
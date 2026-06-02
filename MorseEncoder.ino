#include "BluetoothSerial.h"
#include <NimBLEDevice.h>

BluetoothSerial serialBT;
const int ledPin = 14;
const int buzzer = 25;
const int button = 33;
int mode = 1;
const int timeUnit = 250;
char symbol;
int flash;
struct MorseMapping
{
  const char* code;
  char symbol;
};

MorseMapping morseTable[] =
{
  {".-", 'A'},   {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'},
  {".", 'E'},    {"..-.", 'F'}, {"--.", 'G'},  {"....", 'H'},
  {"..", 'I'},   {".---", 'J'}, {"-.-", 'K'},  {".-..", 'L'},
  {"--", 'M'},   {"-.", 'N'},   {"---", 'O'},  {".--.", 'P'},
  {"--.-", 'Q'}, {".-.", 'R'},  {"...", 'S'},  {"-", 'T'},
  {"..-", 'U'},  {"...-", 'V'}, {".--", 'W'},  {"-..-", 'X'},
  {"-.--", 'Y'}, {"--..", 'Z'},
  {"-----", '0'}, {".----", '1'}, {"..---", '2'}, {"...--", '3'},
  {"....-", '4'}, {".....", '5'}, {"-....", '6'}, {"--...", '7'},
  {"---..", '8'}, {"----.", '9'}
};

void setup()
{
  serialBT.begin("Esp32");
  
  pinMode(ledPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT);
}

void loop()
{
  int buttonState = digitalRead(button);

  if (buttonState == HIGH)
  {
    mode += 1;
    if (mode > 3)mode = 1;
    if (mode == 1)
    {
      digitalWrite(ledPin, HIGH);
      delay(200);
      digitalWrite(ledPin, LOW);
    }
    if (mode == 2)
    {
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(buzzer, LOW);
    }
    if (mode == 3)
    {
      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzer, LOW);
    }
  }


  if (serialBT.available())
  {
    symbol = serialBT.read();
  }
  
  if(symbol == ' ')
  {
    delay(timeUnit * 4);
  }

  symbol = toupper(symbol);

  for(int i = 0; i <= 35; i++)
  {
    if(symbol == morseTable[i].symbol)
    {
      for(int k = 0; morseTable[i].code[k] != '\0'; k++)
      {
        if(morseTable[i].code[k] == '.')flash = timeUnit;
        if(morseTable[i].code[k] == '-')flash = timeUnit * 3;
        if (mode == 1)
        {
          digitalWrite(ledPin, HIGH);
          delay(flash);
          digitalWrite(ledPin, LOW);
          delay(timeUnit);
        }
        if (mode == 2)
        {
          digitalWrite(buzzer, HIGH);
          delay(flash);
          digitalWrite(buzzer, LOW);
          delay(timeUnit);
        }
        if (mode == 3)
        {
          digitalWrite(ledPin, HIGH);
          digitalWrite(buzzer, HIGH);
          delay(flash);
          digitalWrite(ledPin, LOW);
          digitalWrite(buzzer, LOW);
          delay(timeUnit);
        }
      }
      delay(timeUnit * 3);
    }
  }
  symbol = '\0';
}

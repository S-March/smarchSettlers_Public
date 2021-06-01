/******************
 * smarchSettlers *
 * by Sam March   *
 *****************/
 
#include <FastLED.h>
#include "settlersBoard.h"

#define NUM_LEDS ((NUMBER_OF_HEXES)*NUMBER_OF_LEDS_PER_HEX) 
#define LED_PIN 18
#define BUTTON_PIN 23

CRGB leds[NUM_LEDS];

int currentBoardHexOrder[NUMBER_OF_HEXES];
int currentBoardTokenOrder[NUMBER_OF_HEXES];
int hexesThatProduced[MAX_NUMBER_OF_SAME_NUMBER_TOKENS];
int diceRoll;

int oldTime;
int newTime;

bool buttonDebouncing;

struct buttonStruct 
{
  const uint8_t PIN;
  bool buttonPressed;
};

struct animationConfig
{
  bool isAnimating;
  uint8_t animationToShow;
  uint8_t animationStage;
  int animationDelay;
  int lastAnimationTime;
};

animationConfig animationHandlerParameters = {false, 0, 0, 0, 0};

buttonStruct rollButton = {BUTTON_PIN, false};

void IRAM_ATTR rollButtonISR() {
  rollButton.buttonPressed = true;
}

void initializeBoard(void);
void setBoardToArray(int *hexOrderArray);
void showRolledCommodities(int *hexOrderArray, int *hexesThatProducedArray);
void showNumberOnBoard(int numberToShow);
void animationHandler(animationConfig *animationParameters, int currentMillis);

void setup() { 
	Serial.begin(115200);
  Serial.println(NUM_LEDS);

  buttonDebouncing = false;
  
  randomSeed(analogRead(0));

  pinMode(rollButton.PIN, INPUT);
  attachInterrupt(rollButton.PIN, rollButtonISR, FALLING);
  
  initializeBoard();
  
	LEDS.addLeds<WS2812,LED_PIN,GRB>(leds,NUM_LEDS);
	LEDS.setBrightness(250);

  setBoardToArray(currentBoardHexOrder);
}

void loop() { 
  newTime = millis();
  animationHandler(&animationHandlerParameters, newTime);
  if (rollButton.buttonPressed)
  {
      detachInterrupt(rollButton.PIN); 
      diceRoll = rollDice();
      checkForCommodity(diceRoll, currentBoardTokenOrder, currentBoardHexOrder, hexesThatProduced);
      printArray(hexesThatProduced, MAX_NUMBER_OF_SAME_NUMBER_TOKENS, MAX_NUMBER_OF_SAME_NUMBER_TOKENS);

      animationHandlerParameters.isAnimating = true;
      animationHandlerParameters.animationToShow = showNumber;
      
      rollButton.buttonPressed = false;
      buttonDebouncing = true;
      oldTime = newTime;
  }
  if(buttonDebouncing&&!(animationHandlerParameters.isAnimating))
  {    
    buttonDebouncing = false;
    attachInterrupt(rollButton.PIN, rollButtonISR, FALLING);  
  }
}

void initializeBoard(void)
{
  memcpy(currentBoardHexOrder, boardHexOrder, sizeof(boardHexOrder));
  memcpy(currentBoardTokenOrder, numberTokensOnBoard, sizeof(numberTokensOnBoard));
  for(int i = 0; i<NUMBER_OF_TIMES_TO_RANDOMIZE_BOARD; i++)
  {
    randomizeArray(currentBoardHexOrder, NUMBER_OF_HEXES);
    randomizeArray(currentBoardTokenOrder, NUMBER_OF_HEXES);
  }
  setDesert(currentBoardHexOrder, currentBoardTokenOrder);
  printBoard(currentBoardHexOrder);
  printArray(currentBoardTokenOrder, NUMBER_OF_HEXES, 4);
  Serial.println();
  Serial.println();
}

void setBoardToArray(int *hexOrderArray)
{
  int ledBrightness = BASE_BRIGHTNESS;
  for(int i = 0; i<NUMBER_OF_HEXES; i++)
  {
    for(int j = 0; j<NUMBER_OF_LEDS_PER_HEX; j++)
    {
      switch(hexOrderArray[i]) 
      {
        case brickType:
          leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(15, 200, ledBrightness);
        break;
        case desertType:
          leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, 0);
        break;
        case oreType:
          leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(128, 50, ledBrightness);
        break;
        case sheepType:
          leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(78, 125, ledBrightness);
        break;
        case woodType:
          leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(112, 255, ledBrightness);
        break;
        case wheatType:
          leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(40, 255, ledBrightness);
        break;
        default:
          Serial.println("Error in board setup");
        break;
      } 
    }
  }
  FastLED.show();
}

void showRolledCommodities(int *hexOrderArray, int *hexesThatProducedArray)
{
  int ledBrightness;
  if((hexesThatProducedArray[0]==-1)&&(hexesThatProducedArray[1]==-1))
  {
    for(int i = 0; i<NUMBER_OF_HEXES; i++)
    {
      for(int j = 0; j<NUMBER_OF_LEDS_PER_HEX; j++)
      {
        ledBrightness = 0;
        leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, ledBrightness);
      } 
    }
  }
  else
  {  
    for(int i = 0; i<NUMBER_OF_HEXES; i++)
    {
      for(int j = 0; j<NUMBER_OF_LEDS_PER_HEX; j++)
      {
        if((i==hexesThatProducedArray[0])||(i==hexesThatProducedArray[1]))
        {
          ledBrightness = 250;
        }
        else
        {
          ledBrightness = BASE_BRIGHTNESS/4;
        }
        switch(hexOrderArray[i]) 
        {
          case brickType:
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(15, 200, ledBrightness);
          break;
          case desertType:
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, 0);
          break;
          case oreType:
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(128, 50, ledBrightness);
          break;
          case sheepType:
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(78, 125, ledBrightness);
          break;
          case woodType:
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(112, 255, ledBrightness);
          break;
          case wheatType:
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(40, 255, ledBrightness);
          break;
          default:
            Serial.println("Error");
          break;
        }
      }
    }  
  }
  FastLED.show();
}

void showNumberOnBoard(int numberToShow)
{
  int ledBrightness = 100;
  switch(numberToShow)
  {
    case 2:
      for(int i = 0; i<NUMBER_OF_HEXES; i++)
      {
        for(int j = 0; j<NUMBER_OF_LEDS_PER_HEX; j++)
        {
          if(boardRoll_2[i]==1)
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, ledBrightness);
          }
          else
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, 0);            
          }
        }
      }
    break;
    case 3:
      for(int i = 0; i<NUMBER_OF_HEXES; i++)
      {
        for(int j = 0; j<NUMBER_OF_LEDS_PER_HEX; j++)
        {
          if(boardRoll_3[i]==1)
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, ledBrightness);
          }
          else
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, 0);            
          }
        }
      }
    break;
    case 4:
      for(int i = 0; i<NUMBER_OF_HEXES; i++)
      {
        for(int j = 0; j<NUMBER_OF_LEDS_PER_HEX; j++)
        {
          if(boardRoll_4[i]==1)
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, ledBrightness);
          }
          else
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, 0);            
          }
        }
      }
    break;
    case 5:
      for(int i = 0; i<NUMBER_OF_HEXES; i++)
      {
        for(int j = 0; j<NUMBER_OF_LEDS_PER_HEX; j++)
        {
          if(boardRoll_5[i]==1)
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, ledBrightness);
          }
          else
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, 0);            
          }
        }
      }
    break;
    case 6:
      for(int i = 0; i<NUMBER_OF_HEXES; i++)
      {
        for(int j = 0; j<NUMBER_OF_LEDS_PER_HEX; j++)
        {
          if(boardRoll_6[i]==1)
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, ledBrightness);
          }
          else
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, 0);            
          }
        }
      }
    break;
    case 7:
      for(int i = 0; i<NUMBER_OF_HEXES; i++)
      {
        for(int j = 0; j<NUMBER_OF_LEDS_PER_HEX; j++)
        {
          if(boardRoll_7[i]==1)
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, ledBrightness);
          }
          else
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, 0);            
          }
        }
      }
    break;
    case 8:
      for(int i = 0; i<NUMBER_OF_HEXES; i++)
      {
        for(int j = 0; j<NUMBER_OF_LEDS_PER_HEX; j++)
        {
          if(boardRoll_8[i]==1)
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, ledBrightness);
          }
          else
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, 0);            
          }
        }
      }
    break;
    case 9:
      for(int i = 0; i<NUMBER_OF_HEXES; i++)
      {
        for(int j = 0; j<NUMBER_OF_LEDS_PER_HEX; j++)
        {
          if(boardRoll_9[i]==1)
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, ledBrightness);
          }
          else
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, 0);            
          }
        }
      }
    break;
    case 10:
      for(int i = 0; i<NUMBER_OF_HEXES; i++)
      {
        for(int j = 0; j<NUMBER_OF_LEDS_PER_HEX; j++)
        {
          if(boardRoll_10[i]==1)
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, ledBrightness);
          }
          else
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, 0);            
          }
        }
      }
    break;
    case 11:
      for(int i = 0; i<NUMBER_OF_HEXES; i++)
      {
        for(int j = 0; j<NUMBER_OF_LEDS_PER_HEX; j++)
        {
          if(boardRoll_11[i]==1)
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, ledBrightness);
          }
          else
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, 0);            
          }
        }
      }
    break;
    case 12:
      for(int i = 0; i<NUMBER_OF_HEXES; i++)
      {
        for(int j = 0; j<NUMBER_OF_LEDS_PER_HEX; j++)
        {
          if(boardRoll_12[i]==1)
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 200, ledBrightness);
          }
          else
          {
            leds[(i*NUMBER_OF_LEDS_PER_HEX)+j] = CHSV(0, 0, 0);            
          }
        }
      }
    break;
    default:
    break;
  }
  FastLED.show();
}

void animationHandler(animationConfig *animationParameters, int currentMillis)
{
  if(animationParameters->isAnimating)
  {
    if((currentMillis-animationParameters->lastAnimationTime)>animationParameters->animationDelay)
    {
      switch(animationParameters->animationToShow)
      {
        case showBoard:
          Serial.println("Showing full board");
          setBoardToArray(currentBoardHexOrder);
          animationParameters->isAnimating = false;
        break;
        case showNumber:
          Serial.println("Showing number rolled");
          showNumberOnBoard(diceRoll);
          animationParameters->animationToShow = showCommodity;
          animationParameters->animationDelay = 1500;
        break;
        case showCommodity:
          Serial.println("Showing commodities");
          showRolledCommodities(currentBoardHexOrder, hexesThatProduced);
          animationParameters->animationToShow = showBoard;
          animationParameters->animationDelay = 3000;
        break;
        default:
        break;
      }
      animationParameters->lastAnimationTime = currentMillis;
    }
  }
}

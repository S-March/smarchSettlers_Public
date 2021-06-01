#include <Arduino.h>
#include "settlersBoard.h"

void randomizeArray(int *arrayToRandomize, int arrayLenth)
{  
  int oldValueToSwap;
  int newValueToSwap;
  int randomNumber;
  for(int i = arrayLenth-1; i>=0; i--)
  {
    randomNumber = random(arrayLenth);
    oldValueToSwap = arrayToRandomize[i];
    newValueToSwap = arrayToRandomize[randomNumber];
    arrayToRandomize[i] = newValueToSwap;
    arrayToRandomize[randomNumber] = oldValueToSwap;    
  }
}

void randomizeBoard(int *boardArray)
{
  int oldValueToSwap;
  int newValueToSwap;
  int randomNumber;
  for(int i = NUMBER_OF_HEXES-1; i>=0; i--)
  {
    randomNumber = random(NUMBER_OF_HEXES);
    oldValueToSwap = boardArray[i];
    newValueToSwap = boardArray[randomNumber];
    boardArray[i] = newValueToSwap;
    boardArray[randomNumber] = oldValueToSwap;    
  }
}

void setDesert(int *boardArray, int *numberTokenArray)
{
  int valueToSwap;
  int desertHexNumber;
  int desertTokenNumber;
  for(int i = 0; i<NUMBER_OF_HEXES; i++)
  {
    if(boardArray[i] == desertType)
    {
      desertHexNumber = i;
    }
    if(numberTokenArray[i] == 0)
    {
      desertTokenNumber = i;
    }
  }
  valueToSwap = numberTokenArray[desertHexNumber];
  numberTokenArray[desertHexNumber] = 0;
  numberTokenArray[desertTokenNumber] = valueToSwap;
}

void printArray(int *arrayToPrint, int arrayLength, int columns)
{
  for(int i = 0; i<arrayLength; i++)
  {
    if((i%columns==0)&&(i!=0))
    {
      Serial.println();
    }
    Serial.print(arrayToPrint[i]);
    Serial.print("\t");
  }
  Serial.println();
}

void printBoard(int *boardArray)
{
  for(int i = 0; i<NUMBER_OF_HEXES; i++)
  {
    if((i%4==0)&&(i!=0))
    {
      Serial.println();
    }
    switch(boardArray[i]) 
    {
      case brickType:
        Serial.print("brick\t");
      break;
      case desertType:
        Serial.print("desert\t");
      break;
      case oreType:
        Serial.print("ore\t");
      break;
      case sheepType:
        Serial.print("sheep\t");
      break;
      case woodType:
        Serial.print("wood\t");
      break;
      case wheatType:
        Serial.print("wheat\t");
      break;
      default:
        Serial.println("Error");
      break;
    }
  }
  Serial.println();
}

int rollDice(void)
{
  int randomNumber = random(POTENTIAL_ROLE_COMBINATIONS);
  return potentialRoles[randomNumber];
}

void checkForCommodity(int rollValue, int *numberTokenArray, int *boardOrderArray, int *productionHexes)
{
  Serial.print("Rolled a ");
  Serial.print(rollValue);
  Serial.print("\t");

  int numberOfHexesProducing = 0;

  productionHexes[0] = -1;
  productionHexes[1] = -1;
  
  if(rollValue == 7)
  {
    Serial.print("***ROBBER!***");
  }
  else
  {
    for(int i = 0; i<NUMBER_OF_HEXES; i++)
    {
      if(numberTokenArray[i]==rollValue)
      {
        productionHexes[numberOfHexesProducing] = i;
        numberOfHexesProducing++;
        switch(boardOrderArray[i]) 
        {
          case brickType:
            Serial.print("brick\t");
          break;
          case desertType:
            Serial.print("desert\t");
          break;
          case oreType:
            Serial.print("ore\t");
          break;
          case sheepType:
            Serial.print("sheep\t");
          break;
          case woodType:
            Serial.print("wood\t");
          break;
          case wheatType:
            Serial.print("wheat\t");
          break;
          default:
            Serial.println("Error");
          break;
        }
      }
    }
  }
  Serial.println();
}

void printNumber(int rollNumber)
{

}

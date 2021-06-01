
/**********
* DEFINES *
**********/

#define NUMBER_OF_HEXES 19
#define NUMBER_OF_LEDS_PER_HEX 3
#define POTENTIAL_ROLE_COMBINATIONS 36
#define NUMBER_OF_TIMES_TO_RANDOMIZE_BOARD 5
#define MAX_NUMBER_OF_SAME_NUMBER_TOKENS 2
#define BASE_BRIGHTNESS 150
#define DEBOUNCE_TIME 750

/********
* ENUMS *
********/

enum commodityType {
  brickType = 0,
  desertType = 1,
  oreType = 2,
  sheepType = 3,
  woodType = 4,
  wheatType = 5
};

enum animationType {
  showBoard = 0,
  showNumber = 1,
  showCommodity = 2
};

/*********
* ARRAYS *
*********/

static int potentialRoles[POTENTIAL_ROLE_COMBINATIONS] = {
  2,
  3,3,
  4,4,4,
  5,5,5,5,
  6,6,6,6,6,
  7,7,7,7,7,7,
  8,8,8,8,8,
  9,9,9,9,
  10,10,10,
  11,11,
  12
};

static int numberTokensOnBoard[NUMBER_OF_HEXES] = {
  0,      // 0 is Desert
  2,
  3,3,
  4,4,
  5,5,
  6,6,
  8,8,
  9,9,
  10,10,
  11,11,
  12
};

static int boardHexOrder[NUMBER_OF_HEXES] = {
  0,0,0,    // 3 Bricks
  1,        // 1 Desert
  2,2,2,    // 3 Ore
  3,3,3,3,  // 4 Sheep
  4,4,4,4,  // 4 Wood
  5,5,5,5   // 4 Wheat
};

static int boardRoll_2[NUMBER_OF_HEXES] = {
  1,1,0,0,
  0,1,1,1,
  0,0,0,1,
  0,1,0,0,
  1,0,1
};

static int boardRoll_3[NUMBER_OF_HEXES] = {
  1,1,0,0,
  0,1,1,1,
  0,0,0,1,
  0,1,1,0,
  0,0,1
};

static int boardRoll_4[NUMBER_OF_HEXES] = {
  0,1,0,0,
  0,1,0,0,
  0,0,0,1,
  0,1,1,0,
  0,1,1
};

static int boardRoll_5[NUMBER_OF_HEXES] = {
  1,1,0,0,
  0,1,1,1,
  0,0,0,1,
  0,0,1,0,
  0,1,1
};

static int boardRoll_6[NUMBER_OF_HEXES] = {
  1,1,0,0,
  0,1,1,1,
  0,0,0,1,
  0,0,1,0,
  1,1,1
};

static int boardRoll_7[NUMBER_OF_HEXES] = {
  1,1,0,0,
  0,1,0,0,
  0,0,0,1,
  0,1,1,0,
  0,0,0
};

static int boardRoll_8[NUMBER_OF_HEXES] = {
  1,1,0,0,
  0,1,1,1,
  0,0,0,1,
  0,1,1,0,
  1,1,1
};

static int boardRoll_9[NUMBER_OF_HEXES] = {
  1,1,0,0,
  0,1,0,0,
  0,0,0,1,
  0,1,1,0,
  0,1,1
};

static int boardRoll_10[NUMBER_OF_HEXES] = {
  0,1,1,1,
  1,1,0,0,
  1,1,1,0,
  1,0,0,1,
  0,0,1
};

static int boardRoll_11[NUMBER_OF_HEXES] = {
  0,1,0,0,
  0,1,0,1,
  0,0,0,1,
  0,1,1,0,
  1,1,0
};

static int boardRoll_12[NUMBER_OF_HEXES] = {
  1,1,0,0,
  0,1,1,1,
  0,0,0,1,
  0,1,0,0,
  1,0,1
};

/************
* FUNCTIONS *
*************/

void randomizeArray(int *arrayToRandomize, int arrayLenth);
void randomizeBoard(int *boardArray);
void setDesert(int *boardArray, int *numberTokenArray);
void printArray(int *arrayToPrint, int arrayLength, int columns);
void printBoard(int *boardArray);
int rollDice(void);
void checkForCommodity(int rollValue, int *numberTokenArray, int *boardOrderArray, int *productionHexes);

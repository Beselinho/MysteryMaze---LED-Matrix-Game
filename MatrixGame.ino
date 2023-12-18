#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>

const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte pinSW = 2;
const int pinX = A0;
const int pinY = A1;
const int actionButtonPin = 3;

int xValue = 0;
int yValue = 0;
bool joyMoved = false;

//minumum values at which the move from the joystick counts
const int minNeutralThreshold = 350;
const int maxNeutralThreshold = 650;
const int minValueThreshold = 150;
const int maxValueThreshold = 950;


const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;

const int matrixSize = 8;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
byte matrixBrightness = 2;

//directions
int directionJoyMoved = -1;
const int up = 0;
const int down = 1;
const int left = 2;
const int right = 3;


int playerPositionRow = -1;
int playerPositionCol = -1;
int bombRow = -1;
int bombCol = -1;

unsigned long currentPlayerMillis = 0;
unsigned long previousPlayerMillis = 0;
const int intervalPlayerBlink = 400;

unsigned long currentBombMillis = 0;
unsigned long previousBombMillis = 0;
const int intervalBombBlink = 100;

unsigned long previousBombExplodeMillis = 0;
const int explodeInterval = 3000;

unsigned long trackCooldown = 0;
unsigned long lastTrackCooldown = 0;
unsigned long momentBombExplode = 0;
const int putBombCooldown = 3500;
const int explosionDissapear = 500;


bool wasActionPressed = 0;
byte isButtonReleasedState = HIGH;

bool bombHasExploded = false;
bool playerAlive = true;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

unsigned long long gameStartTimer;
bool gameStartTimerWasStarted;
unsigned long prevGameStartTimer = 0;
int greetingsMessageShowTime = 3000;


const byte firstRow = 0;
const byte firstCol = 0;

const byte lcdLength = 16;
const byte lcdWidth = 2;

byte gameState;
const byte introState = 0;
const byte startGameState = 1;
const byte settingsGameState = 2;
const byte aboutGameState = 3;
const byte lcdBrightnessState = 4;
const byte matrixBrightnessState = 5;
const byte playGameState = 6;
const byte endGameState = 7;
int lcdIndex = 0;

int delayPress = 1000;
unsigned long actionPressedInMenu = 0;
unsigned long timeBrightChanged = 0;

byte playerLife = 1;
int inGameTime = 0;

unsigned long prevInGameTimer = 0;
unsigned long actionPressedAfterEnd = 0;
unsigned long actionPressedForBack = 0;
const int oneSecond = 1000;
const int halfOneSecond = 500;

bool ok = 1;
bool ok2 = 1;
bool ok3 = 0;




byte matrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};


byte badEndScreen[matrixSize][matrixSize] = {
  { 0, 0, 1, 1, 1, 1, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 1, 0 },
  { 1, 0, 1, 0, 0, 1, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 1, 1, 1, 1, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 1, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 1, 1, 1, 1, 0, 0 }
};

byte defaultScreen[matrixSize][matrixSize] = {
  { 1, 1, 0, 1, 1, 1, 1, 1 },
  { 1, 0, 0, 1, 0, 0, 0, 1 },
  { 1, 0, 1, 1, 0, 1, 1, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 1, 0, 1, 0, 0, 1 },
  { 1, 0, 1, 1, 1, 0, 1, 1 },
  { 1, 0, 0, 1, 0, 0, 1, 1 },
  { 1, 0, 1, 1, 0, 1, 1, 1 }
};

byte settingsScreen[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 1, 0, 1 },
  { 0, 0, 0, 0, 1, 1, 1, 1 },
  { 0, 0, 0, 1, 1, 1, 0, 0 },
  { 0, 0, 1, 1, 1, 0, 0, 0 },
  { 1, 1, 1, 1, 0, 0, 0, 0 },
  { 1, 0, 1, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 0, 0, 0 }
};

byte aboutScreen[matrixSize][matrixSize] = {
  { 0, 0, 1, 1, 1, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 0, 0 },
  { 0, 1, 1, 0, 1, 1, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 0, 0 },
  { 0, 0, 0, 1, 1, 1, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 }
};



void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);

  pinMode(pinSW, INPUT_PULLUP);
  pinMode(actionButtonPin, INPUT_PULLUP);

  lcd.begin(lcdLength, lcdWidth);
  gameState = introState;

  randomSeed(analogRead(0));
  playerPositionRow = firstRow;
  playerPositionCol = firstCol;
  setupRandomTerrain();
  Serial.begin(9600);
}

void loop() {

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  moveInMainMenu();
  moveInSettingsMenu();

  Serial.println(gameState);
  switch (gameState) {
    case introState:
      introFunction();
      break;
    case startGameState:
      startGameFunction();
      break;
    case settingsGameState:
      settingsGameFunction();
      break;
    case aboutGameState:
      aboutGameFunction();
      break;
    case playGameState:
      if (playerAlive)
        workingGame();
      break;
    case lcdBrightnessState:
      lcdBrightnessFunction();
      break;
    case matrixBrightnessState:
      matrixBrightnessFunction();
      break;
    case endGameState:
      endGameFunction();
      break;
  }
  if (gameState == introState || gameState == startGameState)
    printMatrix(defaultScreen);

  if (gameState == playGameState) {
    printMatrix(matrix);
  }

  if (gameState == settingsGameState || gameState == lcdBrightnessState || gameState == matrixBrightnessState) {
    printMatrix(settingsScreen);
  }

  if (gameState == endGameState)
    printMatrix(badEndScreen);

  if (gameState == aboutGameState)
    printMatrix(aboutScreen);

  gameStartTimer = millis();
}

void workingGame() {
  playerMove();
  makePlayerBlink();
  readActionButton();
  makeBombBlink();

  lcd.setCursor(lcdIndex, lcdIndex);
  lcd.write("Lives:");
  lcd.setCursor(lcdIndex + 6, lcdIndex);
  lcd.print(playerLife);
  lcd.setCursor(lcdIndex, lcdIndex + 1);
  lcd.write("Time:");
  lcd.setCursor(lcdIndex + 5, lcdIndex + 1);
  lcd.print(inGameTime);
  if (millis() - prevInGameTimer >= oneSecond) {
    inGameTime++;
    prevInGameTimer = millis();
  }
}

void startGameFunction() {
  lcd.setCursor(lcdIndex + 3, lcdIndex);
  lcd.write("Start Game");
  lcd.setCursor(lcdIndex + 2, lcdIndex + 1);
  lcd.write("Press Button");
  readActionButton2();
  if (wasActionPressed && millis() - actionPressedAfterEnd > oneSecond) {
    actionPressedInMenu = millis();
    lcd.clear();
    gameState = playGameState;
    wasActionPressed = false;
  }
}

void introFunction() {
  lcd.setCursor(lcdIndex + 3, lcdIndex);
  lcd.write("Welcome to");
  lcd.setCursor(lcdIndex + 2, lcdIndex + 1);
  lcd.write("Mystery Maze");
  if (gameStartTimer - prevGameStartTimer >= greetingsMessageShowTime) {
    gameState = startGameState;
    lcd.clear();
  }
}

void endGameFunction() {
  if (ok == 1) {
    lcd.clear();
    ok = 0;
  }
  if (!playerAlive)
    lcd.setCursor(lcdIndex + 6, lcdIndex);
  lcd.write("GAME");
  lcd.setCursor(lcdIndex + 6, lcdIndex + 1);
  lcd.write("OVER");
  printMatrix(badEndScreen);
  readActionButton2();
  if (wasActionPressed) {
    //actionPressedInMenu = millis();
    lcd.clear();
    gameState = startGameState;
    playerAlive = true;
    wasActionPressed = false;
    actionPressedAfterEnd = millis();
    /*for (int i = firstRow; i < matrixSize; i++)
      for (int j = firstCol; j < matrixSize; j++)
        matrix[i][j] = LOW;*/

    setupRandomTerrain();
  }
}

void settingsGameFunction() {
  lcd.setCursor(lcdIndex + 4, lcdIndex);
  lcd.write("Settings");
  readActionButton2();
  if (wasActionPressed && millis() - actionPressedForBack >= oneSecond) {
    actionPressedInMenu = millis();
    actionPressedForBack = millis();
    lcd.clear();
    gameState = lcdBrightnessState;
    wasActionPressed = false;
  }
}

void lcdBrightnessFunction() {
  lcd.setCursor(lcdIndex + 1, lcdIndex);
  lcd.write("LCD Brightness");
  lcd.setCursor(lcdIndex, lcdIndex + 1);
  lcd.write("Bright Level: ");
  lcd.print(lcdBrightnessState);
  readActionButton2();
  if (wasActionPressed && millis() - actionPressedForBack > oneSecond) {
    actionPressedForBack = millis();
    lcd.clear();
    gameState = settingsGameState;
    wasActionPressed = false;
  }
}

void matrixBrightnessFunction() {
  lcd.setCursor(lcdIndex, lcdIndex);
  lcd.write("Game Brightness");
  lcd.setCursor(lcdIndex, lcdIndex + 1);
  lcd.write("Bright Level: ");
  lcd.print(matrixBrightness);
  turnUpLowerNumbers();
  lc.setIntensity(0, matrixBrightness);
  readActionButton2();
  if (wasActionPressed && millis() - actionPressedForBack > oneSecond) {
    actionPressedForBack = millis();
    lcd.clear();
    gameState = settingsGameState;
    wasActionPressed = false;
  }
}

void aboutGameFunction() {
  lcd.setCursor(lcdIndex, lcdIndex);
  lcd.write("Game:MysteryMaze");
  lcd.setCursor(lcdIndex, lcdIndex + 1);
  lcd.write("Author: github.com/Beselinho");
}

void makePlayerBlink() {
  currentPlayerMillis = millis();
  if (currentPlayerMillis - previousPlayerMillis >= intervalPlayerBlink) {
    previousPlayerMillis = currentPlayerMillis;
    matrix[playerPositionRow][playerPositionCol] = !matrix[playerPositionRow][playerPositionCol];
    lc.setLed(0, playerPositionRow, playerPositionCol, matrix[playerPositionRow][playerPositionCol]);
  }
}

void makeBombBlink() {
  //readActionButton();
  if (bombRow != -1 && bombCol != -1) {
    currentBombMillis = millis();
    if (currentBombMillis - previousBombMillis >= intervalBombBlink) {
      previousBombMillis = currentBombMillis;
      matrix[bombRow][bombCol] = !matrix[bombRow][bombCol];
      lc.setLed(0, bombRow, bombCol, matrix[bombRow][bombCol]);
    }

    //bomb explosion radius is 3x3 so the moment when the bomb explodes it flashes the spots affected
    if (currentBombMillis - previousBombExplodeMillis >= explodeInterval) {
      previousBombExplodeMillis = currentBombMillis;
      for (int i = bombRow - 1; i <= bombRow + 1; i++)
        for (int j = bombCol - 1; j <= bombCol + 1; j++) {
          matrix[i][j] = HIGH;
          lc.setLed(0, i, j, matrix[i][j]);
        }
      momentBombExplode = millis();
      bombHasExploded = true;
    }
    if (currentBombMillis - momentBombExplode >= explosionDissapear && bombHasExploded == true) {
      momentBombExplode = currentBombMillis;
      for (int i = bombRow - 1; i <= bombRow + 1; i++)
        for (int j = bombCol - 1; j <= bombCol + 1; j++) {
          matrix[i][j] = LOW;
          lc.setLed(0, i, j, matrix[i][j]);  //if player is inside the bomb radius he dies, and bad ending screen appears, game over
          if (i == playerPositionRow && j == playerPositionCol) {
            playerAlive = false;
            gameState = endGameState;
          }
        }
      bombRow = -1;
      bombCol = -1;
      bombHasExploded = false;
    }
  }
}

void moveInMainMenu() {
  if (gameState == startGameState || gameState == settingsGameState || gameState == aboutGameState) {
    if (xValue < minValueThreshold && joyMoved == false) {
      directionJoyMoved = up;
      if (gameState - 1 >= startGameState) {
        gameState -= 1;
        lcd.clear();
      }
      joyMoved = true;
    }
    if (xValue > maxValueThreshold && joyMoved == false) {
      directionJoyMoved = down;
      if (gameState + 1 <= aboutGameState) {
        gameState += 1;
        lcd.clear();
      }
      joyMoved = true;
    }
    if (yValue > minNeutralThreshold && yValue < maxNeutralThreshold && xValue > minNeutralThreshold && xValue < maxNeutralThreshold)
      joyMoved = false;
  }
}


void moveInSettingsMenu() {
  if (gameState == lcdBrightnessState || gameState == matrixBrightnessState) {
    if (xValue < minValueThreshold && joyMoved == false) {
      directionJoyMoved = up;
      if (gameState - 1 >= lcdBrightnessState) {
        gameState -= 1;
        lcd.clear();
      }
      joyMoved = true;
    }
    if (xValue > maxValueThreshold && joyMoved == false) {
      directionJoyMoved = down;
      if (gameState + 1 <= matrixBrightnessState) {
        gameState += 1;
        lcd.clear();
      }
      joyMoved = true;
    }
    if (yValue > minNeutralThreshold && yValue < maxNeutralThreshold && xValue > minNeutralThreshold && xValue < maxNeutralThreshold)
      joyMoved = false;
  }
}

void turnUpLowerNumbers() {
  if (yValue > maxValueThreshold && joyMoved == false) {
    directionJoyMoved = left;
    matrixBrightness -= 1;
    joyMoved = true;
  }
  if (yValue <= minValueThreshold && joyMoved == false) {
    directionJoyMoved = right;
    matrixBrightness += 1;
    joyMoved = true;
  }
  if (yValue > minNeutralThreshold && yValue < maxNeutralThreshold && xValue > minNeutralThreshold && xValue < maxNeutralThreshold)
    joyMoved = false;
}


void playerMove() {
  if (xValue < minValueThreshold && joyMoved == false) {  //verify in which direction the joystick moved
    directionJoyMoved = up;
    if (playerPositionRow - 1 >= firstRow && matrix[playerPositionRow - 1][playerPositionCol] != HIGH) {
      matrix[playerPositionRow][playerPositionCol] = LOW;
      playerPositionRow -= 1;
    }
    joyMoved = true;
  }
  if (xValue > maxValueThreshold && joyMoved == false) {
    directionJoyMoved = down;
    if (playerPositionRow + 1 <= matrixSize - 1 && matrix[playerPositionRow + 1][playerPositionCol] != HIGH) {
      matrix[playerPositionRow][playerPositionCol] = LOW;
      playerPositionRow += 1;
    }
    joyMoved = true;
  }
  if (yValue > maxValueThreshold && joyMoved == false) {
    directionJoyMoved = left;
    if (playerPositionCol - 1 >= firstCol && matrix[playerPositionRow][playerPositionCol - 1] != HIGH) {
      matrix[playerPositionRow][playerPositionCol] = LOW;
      playerPositionCol -= 1;
    }
    joyMoved = true;
  }
  if (yValue <= minValueThreshold && joyMoved == false) {
    directionJoyMoved = right;
    if (playerPositionCol + 1 <= matrixSize - 1 && matrix[playerPositionRow][playerPositionCol + 1] != HIGH) {
      matrix[playerPositionRow][playerPositionCol] = LOW;
      playerPositionCol += 1;
    }
    joyMoved = true;
  }
  if (yValue > minNeutralThreshold && yValue < maxNeutralThreshold && xValue > minNeutralThreshold && xValue < maxNeutralThreshold)
    joyMoved = false;
}


void readActionButton2() {
  if (digitalRead(actionButtonPin) != isButtonReleasedState) {
    lastDebounceTime = millis();
    wasActionPressed = true;
  } else if (millis() - lastDebounceTime > debounceDelay && wasActionPressed) {
    wasActionPressed = false;
  }
}

void readActionButton() {
  if (digitalRead(actionButtonPin) != isButtonReleasedState) {
    lastDebounceTime = millis();
    wasActionPressed = true;
  } else if (millis() - lastDebounceTime > debounceDelay && wasActionPressed) {
    wasActionPressed = false;
  }

  if (wasActionPressed && millis() - actionPressedInMenu >= delayPress) {
    trackCooldown = millis();
    bombRow = playerPositionRow;
    bombCol = playerPositionCol;
    previousBombExplodeMillis = millis();
    wasActionPressed = false;
  }
}

// made a function to generate random terrain with being covered with walls(50-75%)
void setupRandomTerrain() {
  int minPercentage = 50;
  int maxPercentage = 75;

  int minHighs = matrixSize * matrixSize * minPercentage / 100;
  int maxHighs = matrixSize * matrixSize * maxPercentage / 100;

  int noOfHighs = random(minHighs, maxHighs + 1);
  for (int index = 0; index < noOfHighs; ++index) {
    int randRow = random(matrixSize);
    int randCol = random(matrixSize);
    if (randRow - playerPositionRow > 1 || randCol - playerPositionCol > 1)
      matrix[randRow][randCol] = HIGH;
  }
}

void printMatrix(byte testMatrix[matrixSize][matrixSize]) {
  for (int row = firstRow; row < matrixSize; row++) {
    for (int col = firstCol; col < matrixSize; col++) {
      lc.setLed(0, row, col, testMatrix[row][col]);
    }
  }
}

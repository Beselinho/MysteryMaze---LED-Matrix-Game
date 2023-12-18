#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>

const byte rs = 11;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
const byte lcdBrightness = 9;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const char *howToPlayLines[] = {
  "How to play     ",
  "Mystery Maze    ",
  "You need to     ",
  "remember and    ",
  "follow the      ",
  "path until end  ",
  "Every misstep   ",
  "kills you :)    ",
  "Btw, in menu    ",
  "the action      ",
  "button does     ",
  "everything.     ",
  "Press it when   ",
  "in doubt        "
};

const byte pinSW = 2;
const int pinX = A0;
const int pinY = A1;
const byte actionButtonPin = 2;

int xValue = 0;
int yValue = 0;
bool joyMoved = false;

//minumum values at which the move from the joystick counts
const int minNeutralThreshold = 350;
const int maxNeutralThreshold = 650;
const byte minValueThreshold = 150;
const int maxValueThreshold = 950;


const byte dinPin = 12;
const byte clockPin = 10;
const byte loadPin = A2;

const byte matrixSize = 8;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
byte matrixBrightness = 2;

//directions
byte directionJoyMoved = -1;
const byte up = 0;
const byte down = 1;
const byte left = 2;
const byte right = 3;


byte playerPositionRow = -1;
byte playerPositionCol = -1;

unsigned long currentPlayerMillis = 0;
unsigned long previousPlayerMillis = 0;
const int intervalPlayerBlink = 300;

const int intervalBeaconsBlinks = 1000;

unsigned long lastScrollTime = 0;
unsigned long lastDisplayTime = 0;

bool mapTurnedOn = false;

bool wasActionPressed = 0;
byte isButtonReleasedState = HIGH;

bool playerAlive = true;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

unsigned long long gameStartTimer;
unsigned long playGameStateStart;
bool gameStartTimerWasStarted;
unsigned long prevGameStartTimer = 0;
int greetingsMessageShowTime = 500;


const byte firstRow = 0;
const byte firstCol = 0;

const byte lcdLength = 16;
const byte lcdWidth = 2;

byte gameState;
const byte introState = 0;
const byte startGameState = 1;
const byte leaderboardGameState = 2;
const byte settingsGameState = 3;
const byte enterHowToPlayGameState = 4;
const byte aboutGameState = 5;
const byte chooseDifficultyGameState = 6;
const byte lcdBrightnessState = 7;
const byte matrixBrightnessState = 8;
const byte soundOnOffGameState = 9;
const byte playGameState = 10;
const byte endGameState = 11;
const byte creditsGameState = 12;
const byte howToPlayGameState = 13;
const byte chooseNameGameState = 14;
const byte showLeaderboardGameState = 15;

byte currentLine = 0;
byte lcdIndex = 0;

bool gameDifficulty = 0;

int delayPress = 1000;
unsigned long actionPressedInMenu = 0;
unsigned long timeBrightChanged = 0;


int inGameTime = 0;

byte startPoint = 3;
byte startPointRow = -1;
byte startPointCol = -1;

byte endPoint = 4;
byte endPointRow = -1;
byte endPointCol = -1;

byte beacon = 2;
byte firstBeaconRow = -1;
byte firstBeaconCol = -1;

byte secondBeaconRow = -1;
byte secondBeaconCol = -1;

byte firstLetterStart = 65;
byte secondLetterStart = 65;
byte thirdLetterStart = 65;

const byte AInAscii = 65;
const byte ZInAscii = 90;


unsigned long prevInGameTimer = 0;
unsigned long actionPressedAfterEnd = 0;
unsigned long actionPressedForBack = 0;
const int oneSecond = 1000;
const int halfOneSecond = 500;
const int waitTime = 1500;
unsigned long lastWriteBlinkTime = 0;
const int writingInterval = 650;
unsigned long lastTimeFinishBlink = 0;
const byte intervalFinishBlink = 100;

bool clearLine = true;
bool isFirstTimeDone = false;
bool isSoundOn = true;
bool firstTimeInSound = true;

bool reachedEndGame = 1;


bool firstTimeInAbout = true;
bool gameStarted = false;

bool firstTimeGoodSound = true;
bool firstTimeBadSound = true;

byte buzzerPin = 3;

byte positionInName = 12;

bool ok2 = 1;
bool ok3 = 1;


//duta
const byte eepromMatrixBrightnessIndex = 0;
const byte eepromlcdBrightnessIndex = 7;
const byte eepromSoundSettingsIndex = 8;

const byte lcdBrightnessOffset = 4;

const byte lcdBrightnessValues[] = { 125, 150, 175, 200, 225, 250, 255 };
byte vectorValuesIndex = 0;

//Settings for difficulty DEFAULT : FOR NORMAL
unsigned long lastMapShowTime = 0;
int intervalMapShows = 10000;
int intervalMapShowsFirstTime = 2500;
int intervalMapStaysShown = 2500;
unsigned long lastTimeBeaconsBlinks = 0;
byte playerLife = 3;

//Buzzer sounds
const int moveSoundInGame = 500;
const byte moveSoundInGameDuration = 100;
const int moveSoundInMenu = 300;
const byte moveSoundInMenuDuration = 100;
const int mapAppearsSound = 1000;
const byte mapAppearsSoundDuration = 150;
const byte mapDissapearsSound = 200;
const byte mapDissapearsSoundDuration = 100;
const int goodEndGameSound = 1500;
const byte goodEndGameSoundDuration = 150;
const byte badEndGameSound = 250;
const byte badEndGameSoundDuration = 250;
const byte loseLifeSound = 100;
const byte loseLifeSoundDuration = 70;


const byte firstLeaderboardIndex = 1;
const byte lastLeaderboardIndex = 6;
const byte minutesIndex1 = 2;
const byte minutesIndex2 = 3;
const byte secondsIndex1 = 5;
const byte secondsIndex2 = 6;
const byte nameIndex1 = 8;
const byte nameIndex2 = 9;
const byte nameIndex3 = 10;
const byte nameIndex4 = 11;




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

byte map1[matrixSize][matrixSize] = {
  { 3, 1, 0, 0, 0, 1, 1, 1 },
  { 0, 1, 1, 0, 0, 1, 0, 1 },
  { 0, 0, 1, 0, 0, 1, 0, 1 },
  { 1, 1, 1, 0, 1, 1, 2, 1 },
  { 1, 0, 0, 0, 1, 0, 1, 1 },
  { 1, 1, 1, 2, 1, 0, 1, 0 },
  { 0, 0, 1, 1, 1, 0, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 4, 0 }
};

byte map2[matrixSize][matrixSize] = {
  { 4, 0, 1, 1, 0, 0, 3, 0 },
  { 0, 0, 0, 1, 0, 0, 1, 0 },
  { 0, 0, 1, 1, 0, 0, 1, 1 },
  { 0, 1, 1, 0, 0, 0, 0, 1 },
  { 0, 1, 0, 0, 1, 1, 1, 1 },
  { 0, 1, 0, 1, 1, 0, 0, 0 },
  { 0, 1, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte map3[matrixSize][matrixSize] = {
  { 0, 0, 0, 1, 1, 1, 0, 0 },
  { 0, 0, 0, 1, 0, 1, 2, 0 },
  { 0, 0, 0, 1, 0, 1, 1, 1 },
  { 0, 1, 1, 1, 0, 0, 0, 1 },
  { 1, 1, 0, 2, 1, 1, 1, 1 },
  { 1, 0, 0, 0, 1, 0, 0, 0 },
  { 1, 0, 0, 0, 1, 1, 0, 0 },
  { 3, 0, 0, 0, 0, 1, 1, 4 }
};


byte badEndScreen[matrixSize][matrixSize] = {
  { 0, 0, 1, 1, 1, 1, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 1, 0 },
  { 1, 0, 1, 0, 0, 1, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 1, 1, 1, 1, 0, 1 },
  { 1, 1, 0, 0, 0, 0, 1, 1 },
  { 0, 1, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 1, 1, 1, 1, 0, 0 }
};

byte goodEndScreen[matrixSize][matrixSize] = {
  { 0, 0, 1, 1, 1, 1, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 1, 0 },
  { 1, 0, 1, 0, 0, 1, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 1, 0, 0, 0, 0, 1, 1 },
  { 1, 0, 1, 1, 1, 1, 0, 1 },
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
  { 1, 1, 1, 1, 0, 1, 1, 1 }
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

byte howToPlayScreen[matrixSize][matrixSize] = {
  { 1, 1, 1, 0, 0, 1, 0, 0 },
  { 1, 0, 1, 0, 1, 1, 1, 0 },
  { 1, 1, 1, 1, 1, 1, 1, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 1, 0, 0, 1, 1, 0 },
  { 0, 1, 0, 0, 1, 0, 0, 1 },
  { 1, 0, 1, 0, 1, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 1, 1, 0 }
};

byte leaderboardScreen[matrixSize][matrixSize] = {
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 0, 1, 1, 1, 1, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 1, 1, 1, 1, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 }
};

byte next[] = {
  B11000,
  B01100,
  B00110,
  B00011,
  B00011,
  B00110,
  B01100,
  B11000
};

const byte leaderboardSize = 4;
const byte leaderboardPositions[leaderboardSize][6] = {
  {9, 10, 11, 12, 13, 14},
  {15, 16, 17, 18, 19, 20},
  {21, 22, 23, 24, 25, 26},
};


void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);

  pinMode(pinSW, INPUT_PULLUP);
  pinMode(lcdBrightness, OUTPUT);
  pinMode(actionButtonPin, INPUT_PULLUP);

  lcd.begin(lcdLength, lcdWidth);

  //randomSeed(analogRead(0));
  //setupRandomTerrain();
  getBeaconStartAndEnd();
  playerPositionRow = startPointRow;
  playerPositionCol = startPointCol;

  gameState = introState;
  analogWrite(lcdBrightness, lcdBrightnessValues[vectorValuesIndex]);
  Serial.begin(9600);
}

void loop() {

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);


  getBeaconStartAndEnd();
  moveInMainMenu();
  moveInSettingsMenu();

  //Serial.println(gameState);
  Serial.println(isSoundOn);
  switch (gameState) {
    case introState:
      introFunction();
      break;
    case startGameState:
      startGameFunction();
      break;
    case chooseNameGameState:
      chooseNameGameFunction();
      break;
    case leaderboardGameState:
      leaderboardGameFunction();
      break;
    case settingsGameState:
      settingsGameFunction();
      break;
    case enterHowToPlayGameState:
      enterHowToPlayGameFunction();
      break;
    case aboutGameState:
      aboutGameFunction();
      break;
    case howToPlayGameState:
      howToPlayFunction();
      break;
    case creditsGameState:
      creditsGameFunction();
      break;
    case playGameState:
      playGameStateStart = millis();
      if (!gameStarted)
        adjustSettings();
      if (playerAlive)
        workingGame();
      break;
    case lcdBrightnessState:
      lcdBrightnessFunction();
      break;
    case matrixBrightnessState:
      matrixBrightnessFunction();
      break;
    case chooseDifficultyGameState:
      chooseDifficultyGameFunction();
      break;
    case soundOnOffGameState:
      soundOnOffGameFunction();
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

  if (gameState == enterHowToPlayGameState)
    printMatrix(howToPlayScreen);

  if (gameState == aboutGameState || gameState == chooseNameGameState)
    printMatrix(aboutScreen);

  if (gameState == leaderboardGameState) {
    printMatrix(leaderboardScreen);
  }

  gameStartTimer = millis();
}

void workingGame() {
  playerMove();
  makePlayerBlink();
  //readActionButton();
  showMapFirstTime();
  showMap();
  gameStarted = true;
  if (!mapTurnedOn)
    beaconBlink();
  //makeBombBlink();


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

void showMapFirstTime() {
  if (!isFirstTimeDone && playGameStateStart >= waitTime) {
    if (millis() - lastMapShowTime >= intervalMapShowsFirstTime && !mapTurnedOn) {
      lastMapShowTime = millis();
      for (int row = 0; row < matrixSize; row++)
        for (int col = 0; col < matrixSize; col++)
          if (map1[row][col] == HIGH)
            matrix[row][col] = map1[row][col];
      mapTurnedOn = true;
      Serial.println("first primu");
    }

    if (millis() - lastMapShowTime >= intervalMapStaysShown && mapTurnedOn) {
      lastMapShowTime = millis();
      for (int row = 0; row < matrixSize; row++)
        for (int col = 0; col < matrixSize; col++)
          if (map1[row][col] == HIGH)
            matrix[row][col] = !map1[row][col];
      mapTurnedOn = false;
      Serial.println("first doilea");
      isFirstTimeDone = true;
    }
  }
}

void adjustSettings() {
  if (gameDifficulty == true) {
    playerLife = 1;
    intervalMapShows = 15000;
    intervalMapShowsFirstTime = 1500;
    intervalMapStaysShown = 1500;
  }
  if (isSoundOn == false)
    buzzerPin = 14;
}

void startGameFunction() {
  lcd.setCursor(lcdIndex + 3, lcdIndex);
  lcd.write("Start Game");
  if (millis() - lastWriteBlinkTime >= writingInterval && !clearLine) {
    lastWriteBlinkTime = millis();
    lcd.setCursor(lcdIndex, lcdIndex + 1);
    lcd.write("               ");
    clearLine = true;
  }
  if (millis() - lastWriteBlinkTime >= writingInterval && clearLine) {
    lastWriteBlinkTime = millis();
    lcd.setCursor(lcdIndex + 2, lcdIndex + 1);
    lcd.write("Press Button");
    clearLine = false;
  }
  readActionButton2();
  if (wasActionPressed && millis() - actionPressedAfterEnd > oneSecond) {
    actionPressedInMenu = millis();
    lcd.clear();
    gameState = chooseNameGameState;
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
  if (reachedEndGame == 1) {
    lcd.clear();
    reachedEndGame = 0;
  }

  if (!playerAlive) {
    lcd.setCursor(lcdIndex + 4, lcdIndex);
    lcd.write("GAME OVER");
    lcd.setCursor(lcdIndex, lcdIndex + 1);
    lcd.write("Press to Restart");
    printMatrix(badEndScreen);
    if (firstTimeBadSound) {
      makeItSound(badEndGameSound, badEndGameSoundDuration);
      firstTimeBadSound = false;
    }
  }

  if (playerPositionRow == endPointRow && playerPositionCol == endPointCol) {
    lcd.setCursor(lcdIndex, lcdIndex);
    lcd.write("YOU WON!CONGRATZ");
    lcd.setCursor(lcdIndex, lcdIndex + 1);
    lcd.write("Your Time:");
    lcd.print(inGameTime);
    //lcd.createchar(0, next);
    lcd.setCursor(lcdLength - 1, lcdIndex + 1);
    //lcd.write(next);
    //lcd.setCursor(lcdLength - 3, lcdIndex + 1);
    lcd.write("x");
    printMatrix(goodEndScreen);
    if (firstTimeGoodSound) {
      makeItSound(goodEndGameSound, goodEndGameSoundDuration);
      firstTimeGoodSound = false;
    }
  }



  readActionButton2();
  if (wasActionPressed) {
    //actionPressedInMenu = millis();
    lcd.clear();
    gameState = startGameState;
    playerAlive = true;
    playerLife = 3;
    wasActionPressed = false;
    actionPressedAfterEnd = millis();
    reachedEndGame = true;
    gameStarted = false;
    inGameTime = 0;
    firstTimeBadSound = true;
    firstTimeGoodSound = false;
  }
}

void settingsGameFunction() {
  lcd.setCursor(lcdIndex + 4, lcdIndex);
  lcd.write("Settings");
  if (millis() - lastWriteBlinkTime >= writingInterval && !clearLine) {
    lastWriteBlinkTime = millis();
    lcd.setCursor(lcdIndex, lcdIndex + 1);
    lcd.write("               ");
    clearLine = true;
  }
  if (millis() - lastWriteBlinkTime >= writingInterval && clearLine) {
    lastWriteBlinkTime = millis();
    lcd.setCursor(lcdIndex + 2, lcdIndex + 1);
    lcd.write("Press Button");
    clearLine = false;
  }

  readActionButton2();
  if (wasActionPressed && millis() - actionPressedForBack >= oneSecond) {
    actionPressedInMenu = millis();
    actionPressedForBack = millis();
    lcd.clear();
    gameState = chooseDifficultyGameState;
    wasActionPressed = false;
  }
}

void chooseNameGameFunction() {
  lcd.setCursor(lcdIndex, lcdIndex);
  lcd.write("  Choose  ");
  lcd.setCursor(lcdIndex, lcdIndex + 1);
  lcd.write(" Your Name ");
  lcd.setCursor(lcdIndex + 12, lcdIndex);
  lcd.write(firstLetterStart);
  lcd.write(secondLetterStart);
  lcd.write(thirdLetterStart);
  moveInChooseName();
  readActionButton2();
  if (wasActionPressed && millis() - actionPressedInMenu > oneSecond) {
    actionPressedInMenu = millis();
    lcd.clear();
    gameState = playGameState;
    wasActionPressed = false;
  }
}

void moveInChooseName() {
  if (positionInName == 12) {
    lcd.setCursor(positionInName, lcdIndex + 1);
    lcd.write("^");
  }
  if (yValue > maxValueThreshold && joyMoved == false && positionInName - 1 >= 12) {
    directionJoyMoved = left;
    positionInName -= 1;
    if (positionInName == 12 || positionInName == 13) {
      lcd.setCursor(positionInName + 1, lcdIndex + 1);
      lcd.write(" ");
      makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
    }
    lcd.setCursor(positionInName, lcdIndex + 1);
    lcd.write("^");
    lcd.setCursor(positionInName, lcdIndex);
    joyMoved = true;
  }
  if (yValue <= minValueThreshold && joyMoved == false && positionInName + 1 <= 14) {
    directionJoyMoved = right;
    positionInName += 1;
    if (positionInName == 14 || positionInName == 13) {
      lcd.setCursor(positionInName - 1, lcdIndex + 1);
      lcd.write(" ");
    }
    lcd.setCursor(positionInName, lcdIndex + 1);
    lcd.write("^");
    lcd.setCursor(positionInName, lcdIndex);
    joyMoved = true;
    makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
  }
  if (xValue < minValueThreshold && joyMoved == false) {
    directionJoyMoved = up;
    switch (positionInName) {
      case 12:
        if (firstLetterStart - 1 >= AInAscii) {
          firstLetterStart -= 1;
          makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
        } else {
          firstLetterStart = ZInAscii;
          makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
        }
        lcd.write(firstLetterStart);
        joyMoved = true;
        break;
      case 13:
        if (secondLetterStart - 1 >= AInAscii) {
          secondLetterStart -= 1;
          makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
        } else {
          secondLetterStart = ZInAscii;
          makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
        }
        lcd.write(secondLetterStart);
        joyMoved = true;
        break;
      case 14:
        if (thirdLetterStart - 1 >= AInAscii) {
          thirdLetterStart -= 1;
          makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
        } else {
          thirdLetterStart = ZInAscii;
          makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
        }
        lcd.write(thirdLetterStart);
        joyMoved = true;

        break;
    }
  }
  if (xValue > maxValueThreshold && joyMoved == false) {
    directionJoyMoved = down;
    switch (positionInName) {
      case 12:
        if (firstLetterStart + 1 <= ZInAscii) {
          firstLetterStart += 1;
          makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
        } else {
          firstLetterStart = AInAscii;
          makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
        }
        lcd.write(firstLetterStart);
        joyMoved = true;
        break;
      case 13:
        if (secondLetterStart + 1 <= ZInAscii) {
          secondLetterStart += 1;
          makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
        } else {
          secondLetterStart = AInAscii;
          makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
        }
        lcd.write(secondLetterStart);
        joyMoved = true;
        break;
      case 14:
        if (thirdLetterStart + 1 <= ZInAscii) {
          thirdLetterStart += 1;
          makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
        } else {
          thirdLetterStart = AInAscii;
          makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
        }
        lcd.write(thirdLetterStart);
        joyMoved = true;
        break;
    }
  }
  if (yValue > minNeutralThreshold && yValue < maxNeutralThreshold && xValue > minNeutralThreshold && xValue < maxNeutralThreshold)
    joyMoved = false;
}

void leaderboardGameFunction() {
  lcd.setCursor(lcdIndex + 2, lcdIndex);
  lcd.write("Leaderboard");
  if (millis() - lastWriteBlinkTime >= writingInterval && !clearLine) {
    lastWriteBlinkTime = millis();
    lcd.setCursor(lcdIndex, lcdIndex + 1);
    lcd.write("               ");
    clearLine = true;
  }
  if (millis() - lastWriteBlinkTime >= writingInterval && clearLine) {
    lastWriteBlinkTime = millis();
    lcd.setCursor(lcdIndex + 2, lcdIndex + 1);
    lcd.write("Press Button");
    clearLine = false;
  }

  readActionButton2();
  if (wasActionPressed && millis() - actionPressedForBack >= oneSecond) {
    actionPressedInMenu = millis();
    actionPressedForBack = millis();
    lcd.clear();
    gameState = showLeaderboardGameState;
    wasActionPressed = false;
  }
}

void showLeaderboardGameFunction() {
  lcd.setCursor(lcdIndex, lcdIndex);
  lcd.write("No.  Name   Score");
  lcd.setCursor(lcdIndex, lcdIndex + 1);
  lcd.write("1.              ");

}

void checkNewHighScore() {
  if(playerAlive){
      
  }
}

void chooseDifficultyGameFunction() {
  lcd.setCursor(lcdIndex, lcdIndex);
  lcd.write("Game Difficulty");
  lcd.setCursor(lcdIndex, lcdIndex + 1);
  lcd.write("Current: ");
  lcd.setCursor(lcdIndex + 9, lcdIndex + 1);
  if (gameDifficulty)
    lcd.write("Hard");
  else
    lcd.write("Normal");

  if (yValue > maxValueThreshold && joyMoved == false && gameDifficulty == true) {
    directionJoyMoved = left;
    gameDifficulty = false;
    lcd.setCursor(lcdIndex + 9, lcdIndex + 1);
    lcd.write("Normal");
    makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
    joyMoved = true;
  }
  if (yValue <= minValueThreshold && joyMoved == false && gameDifficulty == false) {
    directionJoyMoved = right;
    gameDifficulty = true;
    lcd.setCursor(lcdIndex + 9, lcdIndex + 1);
    lcd.write("Hard  ");
    makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
    joyMoved = true;
  }
  if (yValue > minNeutralThreshold && yValue < maxNeutralThreshold && xValue > minNeutralThreshold && xValue < maxNeutralThreshold)
    joyMoved = false;

  readActionButton2();
  if (wasActionPressed && millis() - actionPressedForBack > oneSecond) {
    actionPressedForBack = millis();
    lcd.clear();
    gameState = settingsGameState;
    wasActionPressed = false;
  }
}

void lcdBrightnessFunction() {
  lcd.setCursor(lcdIndex + 1, lcdIndex);
  lcd.write("LCD Brightness");
  lcd.setCursor(lcdIndex, lcdIndex + 1);
  lcd.write("Bright Level: ");
  lcd.print(vectorValuesIndex + 1);
  turnUpLowerLCDBright();
  analogWrite(lcdBrightness, lcdBrightnessValues[vectorValuesIndex]);
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
  turnUpLowerMatrixBright();
  lc.setIntensity(0, matrixBrightness);
  readActionButton2();
  firstTimeInSound = true;
  if (wasActionPressed && millis() - actionPressedForBack > oneSecond) {
    actionPressedForBack = millis();
    lcd.clear();
    gameState = settingsGameState;
    wasActionPressed = false;
  }
}



void soundOnOffGameFunction() {
  lcd.setCursor(lcdIndex, lcdIndex);
  lcd.write("Sound Settings");
  lcd.setCursor(lcdIndex, lcdIndex + 1);
  lcd.write("Sound: ");
  lcd.setCursor(lcdIndex + 8, lcdIndex + 1);
  lcd.write("ON /");
  lcd.setCursor(lcdIndex + 13, lcdIndex + 1);
  lcd.write("OFF");
  if (firstTimeInSound && isSoundOn) {
    lcd.setCursor(lcdIndex + 7, lcdIndex + 1);
    lcd.write(">");
    firstTimeInSound = false;
  } else if (firstTimeInSound && !isSoundOn) {
    lcd.setCursor(lcdIndex + 12, lcdIndex + 1);
    lcd.write(">");
    firstTimeInSound = false;
  }

  if (yValue > maxValueThreshold && joyMoved == false) {
    directionJoyMoved = left;
    isSoundOn = true;
    lcd.setCursor(lcdIndex + 12, lcdIndex + 1);
    lcd.write(" ");
    lcd.setCursor(lcdIndex + 7, lcdIndex + 1);
    lcd.write(">");
    makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
    joyMoved = true;
  }
  if (yValue < minValueThreshold && joyMoved == false) {
    directionJoyMoved = right;
    makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
    isSoundOn = false;
    lcd.setCursor(lcdIndex + 7, lcdIndex + 1);
    lcd.write(" ");
    lcd.setCursor(lcdIndex + 12, lcdIndex + 1);
    lcd.write(">");
    joyMoved = true;
  }
  if (yValue > minNeutralThreshold && yValue < maxNeutralThreshold && xValue > minNeutralThreshold && xValue < maxNeutralThreshold)
    joyMoved = false;

  Serial.println(isSoundOn);

  readActionButton2();
  if (wasActionPressed && millis() - actionPressedForBack > oneSecond) {
    actionPressedForBack = millis();
    lcd.clear();
    gameState = settingsGameState;
    wasActionPressed = false;
    firstTimeInSound = true;
  }
}

void aboutGameFunction() {
  lcd.setCursor(lcdIndex + 6, lcdIndex);
  lcd.write("About");
  if (millis() - lastWriteBlinkTime >= writingInterval && !clearLine) {
    lastWriteBlinkTime = millis();
    lcd.setCursor(lcdIndex, lcdIndex + 1);
    lcd.write("               ");
    clearLine = true;
  }
  if (millis() - lastWriteBlinkTime >= writingInterval && clearLine) {
    lastWriteBlinkTime = millis();
    lcd.setCursor(lcdIndex + 2, lcdIndex + 1);
    lcd.write("Press Button");
    clearLine = false;
  }

  readActionButton2();
  if (wasActionPressed && millis() - actionPressedInMenu >= oneSecond) {
    actionPressedInMenu = millis();
    actionPressedForBack = millis();
    lcd.clear();
    gameState = creditsGameState;
    wasActionPressed = false;
  }
}

void creditsGameFunction() {
  if (firstTimeInAbout) {
    lcd.setCursor(lcdIndex, lcdIndex);
    lcd.write("Game:MysteryMaze");
    lcd.setCursor(lcdIndex, lcdIndex + 1);
    lcd.write("Author: github.com/Beselinho");
    firstTimeInAbout = false;
  }

  if (millis() - lastScrollTime >= 350) {
    lastScrollTime = millis();
    lcd.scrollDisplayLeft();
  }
  if (wasActionPressed && millis() - actionPressedForBack > oneSecond) {
    actionPressedForBack = millis();
    lcd.clear();
    gameState = aboutGameState;
    firstTimeInAbout = false;
    wasActionPressed = false;
  }
}

void enterHowToPlayGameFunction() {
  lcd.setCursor(lcdIndex, lcdIndex);
  lcd.write("  How to Play  ");
  readActionButton2();

  if (millis() - lastWriteBlinkTime >= writingInterval && !clearLine) {
    lastWriteBlinkTime = millis();
    lcd.setCursor(lcdIndex, lcdIndex + 1);
    lcd.write("               ");
    clearLine = true;
  }
  if (millis() - lastWriteBlinkTime >= writingInterval && clearLine) {
    lastWriteBlinkTime = millis();
    lcd.setCursor(lcdIndex + 2, lcdIndex + 1);
    lcd.write("Press Button");
    clearLine = false;
  }

  if (wasActionPressed && millis() - actionPressedForBack >= oneSecond) {
    actionPressedInMenu = millis();
    actionPressedForBack = millis();
    lcd.clear();
    gameState = howToPlayGameState;
    wasActionPressed = false;
  }
}

void howToPlayFunction() {
  lcd.setCursor(lcdIndex, lcdIndex);
  lcd.print(howToPlayLines[currentLine]);
  lcd.setCursor(lcdIndex, lcdIndex + 1);
  lcd.print(howToPlayLines[currentLine + 1]);
  if (xValue < minValueThreshold && joyMoved == false && currentLine - 1 >= 0) {
    directionJoyMoved = up;
    currentLine -= 1;
    lcd.setCursor(lcdIndex, lcdIndex);
    lcd.print(howToPlayLines[currentLine]);
    lcd.setCursor(lcdIndex, lcdIndex + 1);
    lcd.print(howToPlayLines[currentLine + 1]);
    makeItSound(moveSoundInMenu, moveSoundInMenuDuration);

    joyMoved = true;
  }
  if (xValue > maxValueThreshold && joyMoved == false && currentLine + 1 <= 12) {
    directionJoyMoved = down;
    currentLine += 1;
    lcd.setCursor(lcdIndex, lcdIndex);
    lcd.print(howToPlayLines[currentLine]);
    lcd.setCursor(lcdIndex, lcdIndex + 1);
    lcd.print(howToPlayLines[currentLine + 1]);
    makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
    joyMoved = true;
  }
  if (currentLine > 0) {
    lcd.setCursor(lcdIndex + 15, lcdIndex);
    lcd.write("<");
  }
  if (currentLine < 12) {
    lcd.setCursor(lcdIndex + 15, lcdIndex + 1);
    lcd.write(">");
  }
  if (yValue > minNeutralThreshold && yValue < maxNeutralThreshold && xValue > minNeutralThreshold && xValue < maxNeutralThreshold)
    joyMoved = false;

  readActionButton2();
  if (wasActionPressed && millis() - actionPressedForBack > oneSecond) {
    actionPressedForBack = millis();
    lcd.clear();
    gameState = enterHowToPlayGameState;
    wasActionPressed = false;
  }
}
void makePlayerBlink() {
  currentPlayerMillis = millis();
  if (currentPlayerMillis - previousPlayerMillis >= intervalPlayerBlink) {
    previousPlayerMillis = currentPlayerMillis;
    matrix[playerPositionRow][playerPositionCol] = !matrix[playerPositionRow][playerPositionCol];

    //lc.setLed(0, playerPositionRow, playerPositionCol, matrix[playerPositionRow][playerPositionCol]);
  }
}

// MAP FUNCTIONS
void showMap() {
  if (millis() - lastMapShowTime >= intervalMapShows && !mapTurnedOn) {
    lastMapShowTime = millis();
    for (int row = 0; row < matrixSize; row++)
      for (int col = 0; col < matrixSize; col++)
        if (map1[row][col] == HIGH)
          matrix[row][col] = map1[row][col];
    Serial.println("primu");
    makeItSound(mapAppearsSound, mapAppearsSoundDuration);
    mapTurnedOn = true;
  }

  if (millis() - lastMapShowTime >= intervalMapStaysShown && mapTurnedOn) {
    lastMapShowTime = millis();
    for (int row = 0; row < matrixSize; row++)
      for (int col = 0; col < matrixSize; col++)
        if (map1[row][col] == HIGH)
          matrix[row][col] = LOW;
    Serial.println("doilea");
    makeItSound(mapDissapearsSound, mapDissapearsSoundDuration);
    mapTurnedOn = false;
  }
}

void beaconBlink() {
  if (millis() - lastTimeBeaconsBlinks >= intervalBeaconsBlinks) {
    lastTimeBeaconsBlinks = millis();
    // Assuming you have valid indices for firstBeaconRow, firstBeaconCol, secondBeaconRow, secondBeaconCol

    // Toggle the values for the specified points
    matrix[firstBeaconRow][firstBeaconCol] = !matrix[firstBeaconRow][firstBeaconCol];
    matrix[secondBeaconRow][secondBeaconCol] = !matrix[secondBeaconRow][secondBeaconCol];

    // Toggle the values for neighbors (up, down, left, right) of the first point
    if (firstBeaconRow - 1 >= 0 && map1[firstBeaconRow - 1][firstBeaconCol] == HIGH)
      matrix[firstBeaconRow - 1][firstBeaconCol] = HIGH;  // Up

    if (firstBeaconRow + 1 < matrixSize && map1[firstBeaconRow + 1][firstBeaconCol] == HIGH)
      matrix[firstBeaconRow + 1][firstBeaconCol] = HIGH;  // Down

    if (firstBeaconCol - 1 >= 0 && map1[firstBeaconRow][firstBeaconCol - 1] == HIGH)
      matrix[firstBeaconRow][firstBeaconCol - 1] = HIGH;  // Left

    if (firstBeaconCol + 1 < matrixSize && map1[firstBeaconRow][firstBeaconCol + 1] == HIGH)
      matrix[firstBeaconRow][firstBeaconCol + 1] = HIGH;  // Right

    // Toggle the values for neighbors (up, down, left, right) of the second point
    if (secondBeaconRow - 1 >= 0 && map1[secondBeaconRow - 1][secondBeaconCol] == HIGH)
      matrix[secondBeaconRow - 1][secondBeaconCol] = HIGH;
    if (secondBeaconRow + 1 < matrixSize && map1[secondBeaconRow + 1][secondBeaconCol] == HIGH)
      matrix[secondBeaconRow + 1][secondBeaconCol] = HIGH;  // Down

    if (secondBeaconCol - 1 >= 0 && map1[secondBeaconRow][secondBeaconCol - 1] == HIGH)
      matrix[secondBeaconRow][secondBeaconCol - 1] = HIGH;  // Left

    if (secondBeaconCol + 1 < matrixSize && map1[secondBeaconRow][secondBeaconCol + 1] == HIGH)
      matrix[secondBeaconRow][secondBeaconCol + 1] = HIGH;  // Right

    //lc.setLed(0, firstBeaconRow, firstBeaconCol, matrix[firstBeaconRow][firstBeaconCol]);
    //lc.setLed(0, secondBeaconRow, secondBeaconCol, matrix[secondBeaconRow][secondBeaconCol]);
  }

   if(millis() - lastTimeFinishBlink >= intervalFinishBlink){
      lastTimeFinishBlink = millis();
      matrix[endPointRow][endPointCol] = !matrix[endPointRow][endPointCol]; 
    }
}

void getBeaconStartAndEnd() {
  for (int row = 0; row < matrixSize; row++)
    for (int col = 0; col < matrixSize; col++) {
      if (map1[row][col] == startPoint) {
        startPointRow = row;
        startPointCol = col;
        map1[row][col] = LOW;
      }
      if (map1[row][col] == endPoint) {
        endPointRow = row;
        endPointCol = col;
        map1[row][col] = HIGH;
      }
      if (map1[row][col] == beacon) {
        secondBeaconRow = firstBeaconRow;
        secondBeaconCol = firstBeaconCol;
        firstBeaconRow = row;
        firstBeaconCol = col;
        Serial.println(firstBeaconRow);
        Serial.println(firstBeaconCol);
        map1[row][col] = LOW;
      }
    }
}


void moveInMainMenu() {
  if (gameState == startGameState || gameState == settingsGameState || gameState == aboutGameState || gameState == leaderboardGameState || gameState == enterHowToPlayGameState) {
    if (xValue < minValueThreshold && joyMoved == false) {
      directionJoyMoved = up;
      if (gameState - 1 >= startGameState) {
        gameState -= 1;
        lcd.clear();
        makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
      }
      joyMoved = true;
    }
    if (xValue > maxValueThreshold && joyMoved == false) {
      directionJoyMoved = down;
      if (gameState + 1 <= aboutGameState) {
        gameState += 1;
        lcd.clear();
        makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
      }
      joyMoved = true;
    }
    if (yValue > minNeutralThreshold && yValue < maxNeutralThreshold && xValue > minNeutralThreshold && xValue < maxNeutralThreshold)
      joyMoved = false;
  }
}

void moveInSettingsMenu() {
  if (gameState == lcdBrightnessState || gameState == matrixBrightnessState || gameState == chooseDifficultyGameState || gameState == soundOnOffGameState) {
    if (xValue < minValueThreshold && joyMoved == false) {
      directionJoyMoved = up;
      if (gameState - 1 >= chooseDifficultyGameState) {
        gameState -= 1;
        lcd.clear();
        makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
      }
      joyMoved = true;
    }
    if (xValue > maxValueThreshold && joyMoved == false) {
      directionJoyMoved = down;
      if (gameState + 1 <= soundOnOffGameState) {
        gameState += 1;
        lcd.clear();
        makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
      }
      joyMoved = true;
    }
    if (yValue > minNeutralThreshold && yValue < maxNeutralThreshold && xValue > minNeutralThreshold && xValue < maxNeutralThreshold)
      joyMoved = false;
  }
}

void turnUpLowerMatrixBright() {
  if (yValue > maxValueThreshold && joyMoved == false && matrixBrightness - 1 >= 1) {
    directionJoyMoved = left;
    matrixBrightness -= 1;
    joyMoved = true;
    makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
  }
  if (yValue <= minValueThreshold && joyMoved == false && matrixBrightness + 1 <= 5) {
    directionJoyMoved = right;
    matrixBrightness += 1;
    joyMoved = true;
    makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
  }
  if (yValue > minNeutralThreshold && yValue < maxNeutralThreshold && xValue > minNeutralThreshold && xValue < maxNeutralThreshold)
    joyMoved = false;
}

void turnUpLowerLCDBright() {
  if (yValue > maxValueThreshold && joyMoved == false && vectorValuesIndex - 1 >= 0) {
    directionJoyMoved = left;
    vectorValuesIndex -= 1;
    joyMoved = true;
    makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
  }
  if (yValue <= minValueThreshold && joyMoved == false && vectorValuesIndex + 1 <= 6) {
    directionJoyMoved = right;
    vectorValuesIndex += 1;
    joyMoved = true;
    makeItSound(moveSoundInMenu, moveSoundInMenuDuration);
  }
  if (yValue > minNeutralThreshold && yValue < maxNeutralThreshold && xValue > minNeutralThreshold && xValue < maxNeutralThreshold)
    joyMoved = false;
}


void playerMove() {
  if (xValue < minValueThreshold && joyMoved == false) {  //verify in which direction the joystick moved
    directionJoyMoved = up;
    if (playerPositionRow - 1 >= firstRow) {
      if (map1[playerPositionRow - 1][playerPositionCol] == HIGH) {
        matrix[playerPositionRow][playerPositionCol] = LOW;
        playerPositionRow -= 1;
        makeItSound(moveSoundInGame, moveSoundInGameDuration);
      } else {
        matrix[playerPositionRow][playerPositionCol] = LOW;
        playerPositionRow = startPointRow;
        playerPositionCol = startPointCol;
        playerLife -= 1;
        makeItSound(loseLifeSound, loseLifeSoundDuration);
      }
    }
    joyMoved = true;
  }
  if (xValue > maxValueThreshold && joyMoved == false) {
    directionJoyMoved = down;
    if (playerPositionRow + 1 <= matrixSize - 1) {
      if (map1[playerPositionRow + 1][playerPositionCol] == HIGH) {
        matrix[playerPositionRow][playerPositionCol] = LOW;
        playerPositionRow += 1;
        makeItSound(moveSoundInGame, moveSoundInGameDuration);
      } else {
        matrix[playerPositionRow][playerPositionCol] = LOW;
        playerPositionRow = startPointRow;
        playerPositionCol = startPointCol;
        playerLife -= 1;
        makeItSound(loseLifeSound, loseLifeSoundDuration);
      }
    }
    joyMoved = true;
  }
  if (yValue > maxValueThreshold && joyMoved == false) {
    directionJoyMoved = left;
    if (playerPositionCol - 1 >= firstCol) {
      if (map1[playerPositionRow][playerPositionCol - 1] == HIGH) {
        matrix[playerPositionRow][playerPositionCol] = LOW;
        playerPositionCol -= 1;
        makeItSound(moveSoundInGame, moveSoundInGameDuration);
      } else {
        matrix[playerPositionRow][playerPositionCol] = LOW;
        playerPositionRow = startPointRow;
        playerPositionCol = startPointCol;
        playerLife -= 1;
        makeItSound(loseLifeSound, loseLifeSoundDuration);
      }
    }
    joyMoved = true;
  }
  if (yValue <= minValueThreshold && joyMoved == false) {
    directionJoyMoved = right;
    if (playerPositionCol + 1 <= matrixSize - 1) {
      if (map1[playerPositionRow][playerPositionCol + 1] == HIGH) {
        matrix[playerPositionRow][playerPositionCol] = LOW;
        playerPositionCol += 1;
        makeItSound(moveSoundInGame, moveSoundInGameDuration);
      } else {
        matrix[playerPositionRow][playerPositionCol] = LOW;
        playerPositionRow = startPointRow;
        playerPositionCol = startPointCol;
        playerLife -= 1;
        makeItSound(loseLifeSound, loseLifeSoundDuration);
      }
    }
    joyMoved = true;
  }
  if (yValue > minNeutralThreshold && yValue < maxNeutralThreshold && xValue > minNeutralThreshold && xValue < maxNeutralThreshold)
    joyMoved = false;

  if (playerLife == 0) {
    playerAlive = false;
    gameState = endGameState;
  }
  if (playerPositionRow == endPointRow && playerPositionCol == endPointCol) {
    gameState = endGameState;
  }
}


void readActionButton2() {
  if (digitalRead(actionButtonPin) != isButtonReleasedState) {
    lastDebounceTime = millis();
    wasActionPressed = true;
  } else if (millis() - lastDebounceTime > debounceDelay && wasActionPressed) {
    wasActionPressed = false;
  }
}

void makeItSound(int soundType, int soundDuration) {
  if (isSoundOn)
   tone(buzzerPin, soundType, soundDuration);
}


void printMatrix(byte testMatrix[matrixSize][matrixSize]) {
  for (int row = firstRow; row < matrixSize; row++) {
    for (int col = firstCol; col < matrixSize; col++) {
      lc.setLed(0, row, col, testMatrix[row][col]);
    }
  }
}

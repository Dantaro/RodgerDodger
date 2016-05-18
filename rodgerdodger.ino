
#include "Arduboy.h"
#include "Art.h"
#include "Constants.h"
#include "Sprite.h"

//CONSTANTS
// Make an instance of arduboy used for many functions
Arduboy arduboy;

//Hero-related variables
Sprite *hero;

//Enemy-related variables
Sprite *enemies[30]; //Max of 30  enemies on screen
bool death;

//Scoring
int score;
bool victory;
int frameCount;

//Initial Game State
bool splashScreenCheck = true;
bool menuScreenCheck = false;

void setup() {
  //initiate arduboy instance
  arduboy.begin();

  arduboy.setFrameRate(60);
  
  randomSeed(analogRead(0));
  hero = new Sprite();
  for (int i = 0 ; i < arr_length(enemies) ; i++) {
    Sprite *enemy = new Sprite();
    enemies[i] = enemy;
  }

  setInitialGameState();
  
}

void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;
    
  arduboy.clear();

  if (splashScreenCheck) {
    splashScreen();
  } else if (menuScreenCheck) {
      menuScreen();
  } else {
    if (!victory && !death) {
      int enemiesLength = arr_length(enemies);
      operateHero();
      operateEnemies(enemiesLength);
      calculateCollisions(enemiesLength);
      calculateAndDrawScore();
    } else if (victory) {
      victoryState();
    } else if (death) {
      deathState();
    }
  }
  
  arduboy.display();
}



//Action methods

void setInitialGameState() {

  hero->setX(CENTER_X - (CHAR_WIDTH / 2));
  hero->setY(Y_MAX);
  hero->setCharacterSprite(01);
  hero->setVisible(true);

  for (int i = 0 ; i < arr_length(enemies) ; i++) {
    Sprite *enemy = enemies[i];
    int x = random(X_MAX);
    int y = 0;
    enemy->setX(x);
    enemy->setY(y);
    enemy->setCharacterSprite(15);
    enemy->setVisible(false);
    enemies[i] = enemy;
  }
  
  death = false;
  score = 0;
  victory = false;
  frameCount = 0;
}

void operateHero() {
  //Hero
      if(arduboy.pressed(RIGHT_BUTTON) && (hero->getX() < X_MAX)) {
        hero->setX(hero->getX() + 1);
      }
      if(arduboy.pressed(LEFT_BUTTON) && (hero->getX() > 0)) {
        hero->setX(hero->getX() - 1);
      }
      if(arduboy.pressed(UP_BUTTON) && (hero->getY() > 0)) {
        hero->setY(hero->getY() - 1);
      }
      if(arduboy.pressed(DOWN_BUTTON) && (hero->getY() < Y_MAX)) {
        hero->setY(hero->getY() + 1);
      }
      //Position cursor to draw Hero
      arduboy.setCursor(hero->getX(), hero->getY());
      //Draw Hero
      arduboy.print(hero->getCharacterSprite());
}

void operateEnemies(int enemiesLength) {
  //Move Enemies
  bool hasMadeVisibleThisCycle = false;
  for (int i = 0 ; i < arr_length(enemies) ; i++) {
    Sprite *enemy = enemies[i];
    if (enemy->isVisible()) {
      int y = enemy->getY();
      y++;
      if (y >= Y_MAX) {
        enemy->setY(-1); //Set just before the top of the screen
        enemy->setX(random(X_MAX)); //Set a random X coordinate
        enemy->setVisible(false);
      } else {
        enemy->setY(y);
        //Position cursor to draw enemy
        arduboy.setCursor(enemy->getX(), enemy->getY());
        //Draw Enemy
        arduboy.print(enemy->getCharacterSprite());
      }
    }
    
    if (random(4) == 1 && !hasMadeVisibleThisCycle) {
      hasMadeVisibleThisCycle = true;
      enemy->setVisible(true);
    }
    //Return to array
    enemies[i] = enemy;
  }
}

void calculateCollisions(int enemiesLength) {
  //Calculate collision
  for (int i = 0 ; i < enemiesLength ; i++) {
    Sprite *enemy = enemies[i];
    
    //Left Corner
    bool hitXLeftMin = false;
    bool hitXLeftMax = false;
    if (enemy->getX() <= hero->getX()) {
      hitXLeftMin = true;
    }
    if (enemy->getX() + CHAR_WIDTH >= hero->getX()) {
      hitXLeftMax = true;
    }
    
    //Left Corner
    bool hitXRightMin = false;
    bool hitXRightMax = false;
    if (enemy->getX() <= hero->getX() + CHAR_WIDTH) {
      hitXRightMin = true;
    }
    if (enemy->getX() + CHAR_WIDTH >= hero->getX() + CHAR_WIDTH) {
      hitXRightMax = true;
    }
    
    //Top Side
    bool hitYTopMin = false;
    bool hitYTopMax = false;
    if (enemy->getY() <= hero->getY()) {
      hitYTopMin = true;
    }
    if (enemy->getY() + CHAR_HEIGHT >= hero->getY()) {
      hitYTopMax = true;
    }
    
    //Bottom Side
    bool hitYBottomMin = false;
    bool hitYBottomMax = false;
    if (enemy->getY() <= hero->getY() + CHAR_HEIGHT) {
      hitYBottomMin = true;
    }
    if (enemy->getY() + CHAR_HEIGHT >= hero->getY() + CHAR_HEIGHT) {
      hitYBottomMax = true;
    }

    //Calculate if hit
    if ((hitXRightMin && hitXRightMax) && ((hitYTopMin && hitYTopMax) || (hitYBottomMin && hitYBottomMax))) {
      death = true;
      break;
    } else if ((hitXLeftMin && hitXLeftMax) && ((hitYTopMin && hitYTopMax) || (hitYBottomMin && hitYBottomMax))) {
      death = true;
      break;
    } else {
      death = false;
    }
  }
}

void calculateAndDrawScore() {
  //Calculate and Draw Score
  frameCount++;
  if (frameCount % 10 == 0) {
    score++;
  }
  if (score >= VICTORY_SCORE) {
    victory = true;
  }
  arduboy.setCursor(0,0);
  arduboy.print(score);
}

void victoryState() {
  char winText[] = "You win!";
  arduboy.setCursor((CENTER_X - (sizeof(winText) * CHAR_WIDTH / 2)), CENTER_Y);
  arduboy.print(winText);
  arduboy.setCursor(0, Y_MAX);
  arduboy.print(playText);
  arduboy.setCursor((CENTER_X - (sizeof(finalScoreText) * CHAR_WIDTH / 2)), CENTER_Y - (CHAR_HEIGHT * 2) - 6);
  arduboy.print(finalScoreText);
  arduboy.setCursor((CENTER_X - (CHAR_WIDTH * 2)), CENTER_Y - (CHAR_HEIGHT * 2) - 3);
  arduboy.print(score);
  if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
    victory = false;
    setInitialGameState();
  }
}

void deathState() {
  char loseText[] = "You lose!";
  arduboy.setCursor((CENTER_X - (sizeof(loseText) * CHAR_WIDTH / 2)), CENTER_Y);
  arduboy.print(loseText);
  arduboy.setCursor(0, Y_MAX);
  arduboy.print(playText);
  arduboy.setCursor((CENTER_X - (sizeof(finalScoreText) * CHAR_WIDTH / 2)), CENTER_Y - (CHAR_HEIGHT * 2) - 6);
  arduboy.print(finalScoreText);
  arduboy.setCursor((CENTER_X - (CHAR_WIDTH * 2)), CENTER_Y - (CHAR_HEIGHT) - 3);
  arduboy.print(score);
  if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
    death = false;
    setInitialGameState();
  }
}

void splashScreen() {
  arduboy.drawBitmap(0, 0, logo, 128, 40, WHITE);
  arduboy.setCursor(0, Y_MAX);
  arduboy.print("Press any button!");
  if (arduboy.pressed(A_BUTTON) 
  || arduboy.pressed(B_BUTTON) 
  || arduboy.pressed(UP_BUTTON)
  || arduboy.pressed(DOWN_BUTTON)
  || arduboy.pressed(RIGHT_BUTTON)
  || arduboy.pressed(LEFT_BUTTON)){
    splashScreenCheck = false;
    menuScreenCheck = true;
    delay(250);
  }
}

void menuScreen() {
  int textY = 10;
  arduboy.setCursor(0,textY);
  arduboy.print("Avoid the bullets!");
  textY = textY + CHAR_HEIGHT;
  arduboy.setCursor(0, textY);
  arduboy.print("Reach 250 points!");
  textY = textY + CHAR_HEIGHT;
  arduboy.setCursor(0, textY);
  arduboy.print("Become the one true");
  textY = textY + CHAR_HEIGHT + 3;
  arduboy.setCursor(20, textY);
  arduboy.print("RODGER DODGER");
  arduboy.setCursor(0, Y_MAX);
  arduboy.print(playText);
  if (arduboy.pressed(A_BUTTON)) {
    splashScreenCheck = false;
    menuScreenCheck = false;
  }
}


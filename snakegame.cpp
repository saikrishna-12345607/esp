#include<Arduino.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 1

#define DATA_PIN 23
#define CS_PIN   5
#define CLK_PIN  18

#define MATRIX_SIZE 8
#define MAX_SNAKE_LEN 64

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
void resetGame();
void readJoystick();
void moveSnake();
void newFood();
void drawGame();

#define JOY_X 32
#define JOY_Y 33
#define RESET_BTN 25

int snakeX[MAX_SNAKE_LEN];
int snakeY[MAX_SNAKE_LEN];
int snakeLen;

int foodX, foodY;

int dir;  

unsigned long lastMove = 0;

void setup() {
  Serial.begin(912600);
  mx.begin();
  mx.clear();

  pinMode(RESET_BTN, INPUT_PULLUP);

  randomSeed(analogRead(0));

  resetGame();
}

void loop() {

  if (digitalRead(RESET_BTN) == LOW) {
    resetGame();
    delay(300); 
  }

  readJoystick();

  if (millis() - lastMove > 300) {
    moveSnake();
    lastMove = millis();
  }
}
void resetGame() {
  snakeLen = 3;

  for (int i = 0; i < snakeLen; i++) {
    snakeX[i] = 4 - i;
    snakeY[i] = 4;
  }

  dir = 1; 
  newFood();
  mx.clear();
}

void readJoystick() {
  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);

  if (x < 1000) dir = 3;       
  else if (x > 3000) dir = 1; 
  else if (y < 1000) dir = 0;  
  else if (y > 3000) dir = 2;  
}

void moveSnake() {

  for (int i = snakeLen - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  if (dir == 0) snakeY[0]--;
  if (dir == 1) snakeX[0]++;
  if (dir == 2) snakeY[0]++;
  if (dir == 3) snakeX[0]--;

  snakeX[0] = (snakeX[0] + MATRIX_SIZE) % MATRIX_SIZE;
  snakeY[0] = (snakeY[0] + MATRIX_SIZE) % MATRIX_SIZE;
  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    if (snakeLen < MAX_SNAKE_LEN) {
      snakeLen++;
    }
    newFood();
  }

  drawGame();
}
void newFood() {
  foodX = random(0, MATRIX_SIZE);
  foodY = random(0, MATRIX_SIZE);
}
void drawGame() {
  mx.clear();

  mx.setPoint(foodY, foodX, true);
  for (int i = 0; i < snakeLen; i++) {
    mx.setPoint(snakeY[i], snakeX[i], true);
  }
}

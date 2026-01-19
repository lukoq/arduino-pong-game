#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

const int playerLeftButtonUp = 2;
const int playerLeftButtonDown = 3;

const int playerRightButtonUp = 4;
const int playerRightButtondDown = 5;

unsigned long playersRefresh = 0;
const long playersRefreshInterval = 100;

unsigned long ballRefresh = 0;
const long ballRefreshInterval = 300;

int racketSize = 3;
struct Player {
    int x;
    int y;

    int buttonUp;
    int buttonDown;
};

struct Ball {
    int x;
    int y;

    int vel_x;
    int vel_y;
};

Player leftPlayer{0, 3, 2, 3};
Player rightPlayer{11, 3, 4, 5};
Ball ball{6, 3, 1, 1};

void handleInput(Player &player) {
  if (digitalRead(player.buttonUp) == LOW) {
    player.y--;
  }
  if (digitalRead(player.buttonDown) == LOW) {
    player.y++;
  }
  
  player.y = constrain(player.y, 0, 7 - racketSize + 1);
}
void updateBall(){
  ball.x += ball.vel_x;
  ball.y += ball.vel_y;


  if (ball.y <= 0 || ball.y >= 7) {
    ball.vel_y *= -1; 
  }
  if (ball.x == leftPlayer.x + 1) {
    if (ball.y >= leftPlayer.y && ball.y < leftPlayer.y + racketSize) {
      ball.vel_x = 1; 
    }
  }
  
  if (ball.x == rightPlayer.x - 1) {
    if (ball.y >= rightPlayer.y && ball.y < rightPlayer.y + racketSize) {
      ball.vel_x = -1; 
    }
  }

  if (ball.x < 0 || ball.x > 11) {
    ball.x = 5; ball.y = 3; 
    ball.vel_x *= -1;         
  }
}

void setup() {
  Serial.begin(9600);
  matrix.begin();

  pinMode(leftPlayer.buttonUp, INPUT_PULLUP);
  pinMode(leftPlayer.buttonDown, INPUT_PULLUP);
  pinMode(rightPlayer.buttonUp, INPUT_PULLUP);
  pinMode(rightPlayer.buttonDown, INPUT_PULLUP);


}

void loop() {

  unsigned long curTime = millis();

  if(curTime - playersRefresh >= playersRefreshInterval) {
      playersRefresh = curTime; 

      handleInput(leftPlayer);
      handleInput(rightPlayer);

      matrix.beginDraw();
      matrix.clear();


      matrix.stroke(0xFFFFFFFF);
      matrix.line(leftPlayer.x, leftPlayer.y, leftPlayer.x, leftPlayer.y + racketSize - 1 );
      matrix.line(rightPlayer.x, rightPlayer.y, rightPlayer.x, rightPlayer.y + racketSize - 1 );
      matrix.point(ball.x, ball.y);

      matrix.endDraw();
  }
   if(curTime - ballRefresh >= ballRefreshInterval) {
      ballRefresh = curTime; 

      updateBall();
   }

}
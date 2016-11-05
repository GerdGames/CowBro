#include <Arduboy.h>
#include "bitmaps.h"

Arduboy ardu;
ArduboyAudio arduAudio;
ArduboyTunes arduTunes;

int x, y, bulletX, bulletY, targetX, targetY, bonusX, bonusY, level;
int frame = 0;
int score = 0;
int timeLeft = 10;
String sound;
boolean soundStatus = true;
boolean soundCooldown = false;
boolean targetActive;
boolean front = true;
boolean bulletActive = false;
boolean bulletCooldown = false;
boolean game = false;
boolean pause = false;
boolean pauseCooldown = false;
boolean timeOver = false;
boolean bonusActive = false;
boolean levelUp = false;
boolean finish;


void setup() 
{
  // put your setup code here, to run once:
   ardu.begin();
   arduAudio.begin();
   ardu.setFrameRate(60);
}

void loop() 
{
   //to see if a button can be pressed again
   if((ardu.notPressed(A_BUTTON) && ardu.notPressed(B_BUTTON)))
   {
      pauseCooldown = false;
      bulletCooldown = false;
   }
   //to run the menu
   if(!game && !timeOver)
   {
      //initialize game data
      x = 50;
      y = 26; 
      frame = 0;
      score = 0;
      timeLeft = 10;
      level = 1;
      front = true;
      bulletActive = false;
      game = false;
      pause = false;
      //Display game title and menu options
      ardu.clear();
      ardu.drawBitmap(96, 8, titleFace, 32, 32, WHITE);
      ardu.setCursor(16, 16);
      ardu.setTextSize(2);
      ardu.println("CowBro");
      ardu.setCursor(0, 48);
      ardu.setTextSize(1);
      ardu.println("Press A or B to start");
      ardu.println("Sound: " + sound);
      ardu.display();
      //to change sound status
      if(((ardu.pressed(RIGHT_BUTTON))||(ardu.pressed(LEFT_BUTTON))) && (!soundCooldown))
      {
        soundStatus = !soundStatus;
        soundCooldown = true;
      }
      //to see if sound can be changed again
      if((ardu.notPressed(RIGHT_BUTTON) && ardu.notPressed(LEFT_BUTTON)))
      {
        soundCooldown = false;
      }
      //to return the state of sound
      if(soundStatus)
      {
        arduAudio.on();
        sound = "on";
      }
      else
      {
        arduAudio.off();
        sound = "off";
      }
      //to see if the game can be started
      if(((ardu.pressed(A_BUTTON))||(ardu.pressed(B_BUTTON))) && (!bulletCooldown))
      {
        game = true;
        bulletCooldown = true;
      }
      
   }
   //to run the game over screen
   if(timeOver && !game)
   {
      ardu.clear();
      ardu.setCursor(32, 24);
      ardu.println("Game Over");
      ardu.setCursor(0, 40);
      ardu.print("Your score was: ");
      ardu.println(score);
      ardu.print("A or B to menu");
      ardu.display();
      //get input to finish game over screen
      if(((ardu.pressed(A_BUTTON))||(ardu.pressed(B_BUTTON))) && (!bulletCooldown))
      {
        timeOver = false;
        bulletCooldown = true;
      }
   }
   //to run the game itself
   if(game && ardu.nextFrame())
   {
      ardu.clear();
      //to see if the game is paused and act accordingly
      if(pause)
      {
         ardu.setCursor(0,8);
         ardu.print("Paused A+B to resume");
      }
      //to get inputs for pausing the game
      if((ardu.pressed(A_BUTTON) && ardu.pressed(B_BUTTON)) && !pauseCooldown)
      { 
         pauseCooldown = true;
         pause = !pause;
      }
      if(!pause)
      {
        //to level up difficulty
        if((score % 100 == 0) && (score != 0))
        {
          if(!levelUp)
          {
            level++;
            levelUp = true;
          }
        }
        //to set bonus parameters
         if(!bonusActive && ardu.everyXFrames(240))
         {
            bonusActive = true;
            ardu.initRandomSeed();
            bonusX = random(1, 119);
            bonusY = 0;
         }
        //to create time bonuses
         if(bonusActive)
         {
          ardu.drawBitmap(bonusX, bonusY, timeBonus, 8, 8, WHITE);
          bonusY ++;
         }
         //to perform hit detection on time bonuses
         if((x + 16 >= bonusX)&&(x <= bonusX + 8)&&(y + 16 >= bonusY)&&(y <= bonusY + 8) && bonusActive == true)
         {
            timeLeft += 5;
            bonusActive = false;
            arduTunes.tone(500, 100);
         }
         if(bonusY > 64)
         {
          bonusActive = false;
         }
        //to set target parameters
         if(!targetActive)
         {
            targetActive = true;
            ardu.initRandomSeed();
            targetX = random(1, 119);
            targetY = random(9, 55);
         }
        //to create targets
         if(targetActive)
         {
            if (targetX < 64)
               ardu.drawBitmap(targetX, targetY, targetLeft, 8, 8, WHITE);
            else
               ardu.drawBitmap(targetX, targetY, targetRight, 8, 8, WHITE);
         }
        //to perform hit detection on targets
         if((bulletX >= targetX)&&(bulletX <= targetX + 8)&&(bulletY >= targetY)&&(bulletY <= targetY + 8))
         {
            bulletActive = false;
            targetActive = false;
            levelUp = false;
            arduTunes.tone(300, 100);
            score++;
         }
      
        //to get movement input, to move Cowbro and change frames on his animation
         if((ardu.pressed(RIGHT_BUTTON))&&(x < 112))
         {
            x += 1;
            frame += 1;
         }
         if((ardu.pressed(LEFT_BUTTON))&&(x > 0))
         {
            x -= 1;
            frame += 1;
         }
         if((ardu.pressed(UP_BUTTON))&&(y > 8))
         {
            y -= 1;
            frame += 1;
         }
         if((ardu.pressed(DOWN_BUTTON))&&(y < 48))
         {
            y += 1;
            frame += 1;
         }
      
        //to get shooting input and start shooting a bullet
         if((ardu.pressed(B_BUTTON)) && (!bulletActive) && (!bulletCooldown))
         {
            bulletActive = true;
            front = true;
            bulletX = x + 12;
            bulletY = y + 9;
            arduTunes.tone(100, 100);
         }
         if((ardu.pressed(A_BUTTON)) && (!bulletActive) && (!bulletCooldown))
         {
            bulletActive = true;
            front = false;
            bulletX = x + 2;
            bulletY = y + 9;
            arduTunes.tone(100, 100);
         }
        //to create and move the bullet
         if(bulletActive)
         {
            bulletCooldown = true;
            if(front)
            {
               ardu.drawLine(bulletX, bulletY, bulletX + 4, bulletY, WHITE);
               bulletX += 4;
            }
            else
            {
               ardu.drawLine(bulletX + 4, bulletY, bulletX, bulletY, WHITE);
               bulletX -= 4;
            }
         }
         if(bulletX > 128 || bulletX < -4)
         {
            bulletActive = false;
         }
      
        //to animate the cowboy sprite
         if (front)
         {
            if(frame > 4)
            {
               ardu.drawBitmap(x, y, cowBro, 16, 16, WHITE);
            }
            else
            {
               ardu.drawBitmap(x, y, cowBroStep, 16, 16, WHITE);
            }
         }
         else
         {
            if(frame > 4)
            {
               ardu.drawBitmap(x, y, cowBroFlip, 16, 16, WHITE);
            }
            else
            {
               ardu.drawBitmap(x, y, cowBroStepFlip, 16, 16, WHITE);
            }
         }
         if(frame > 8)
         {
            frame = 0;
         }
        //manage time
         if(!timeOver)
         {
            if(ardu.everyXFrames(60))
            {
               timeLeft--;
            }
            if(timeLeft == 0)
            {
               timeOver = true;
               game = false;
            }
         }
      }
      //draw the UI
      ardu.setCursor(0, 0);
      ardu.print("Score ");
      ardu.print(score);
      ardu.print(" Time: ");
      ardu.print(timeLeft);
      ardu.display();
   }
}



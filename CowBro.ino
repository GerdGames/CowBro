#include <Arduboy.h>
#include "bitmaps.h"

Arduboy ardu;
ArduboyTunes arduTune;

int x = 50;
int y = 26;
int bulletX, bulletY;
int targetX, targetY;
int frame = 0;
int score = 0;
String sound = "off";
boolean targetActive;
boolean front = true;
boolean bulletActive = false;
boolean game = false;
boolean pause = false;
boolean unpauseCooldown = false;


void setup() 
{
  // put your setup code here, to run once:
  ardu.begin();
  ardu.setFrameRate(60);
}

void loop() 
{
  //to run the menu
  if(!game)
  {
    //Display game title and menu options
    ardu.clear();
    ardu.setCursor(0, 0);
    ardu.println("CowBro");
    ardu.setCursor(0, 48);
    ardu.println("Press A or B to start");
    ardu.println("Sound: " + sound);
    ardu.display();

    if((ardu.pressed(A_BUTTON))||(ardu.pressed(B_BUTTON)))
    {
      game = true;
    }
  }
  if(pause)
  {
    ardu.setCursor(0,8);
    ardu.print("Paused A+B to resume");
    ardu.display();
    if(ardu.pressed(A_BUTTON) && ardu.pressed(B_BUTTON))
    {
      pause = false;
      unpauseCooldown = true;
    }
    
  }
  //to run the game itself
    if(game && !pause && ardu.nextFrame())
    {
      ardu.clear();

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
      //to perform hit detection
      if((bulletX >= targetX)&&(bulletX <= targetX + 8)&&(bulletY >= targetY)&&(bulletY <= targetY + 8))
      {
        bulletActive = false;
        targetActive = false;
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
      if((ardu.pressed(B_BUTTON)) && (!bulletActive))
      {
        bulletActive = true;
        front = true;
        bulletX = x + 12;
        bulletY = y + 9;
      }
       if((ardu.pressed(A_BUTTON)) && (!bulletActive))
      {
        bulletActive = true;
        front = false;
        bulletX = x + 2;
        bulletY = y + 9;
      }
      //to get inputs for pausing the game
      if(ardu.pressed(A_BUTTON) && ardu.pressed(B_BUTTON) && !unpauseCooldown)
      {
        pause = true;
      }
      //to see if the game can be paused again
      if(unpauseCooldown)
      {
        int count = 0;
        while(count < 60)
        {
          count++;
        }
        unpauseCooldown = false;
      }
      //to create and move the bullet
      if(bulletActive)
      {
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
      ardu.setCursor(0, 0);
      ardu.print("Score ");
      ardu.print(score);
      ardu.display();
    }
}


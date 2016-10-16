#include <Arduboy.h>
#include "bitmaps.h"

Arduboy ardu;
ArduboyTunes arduTune;

int x = 50;
int y = 26;
int frame = 0;
boolean bulletActive = false;

void setup() 
{
  // put your setup code here, to run once:
  ardu.begin();
  ardu.setFrameRate(30);
}

void loop() 
{
  // put your main code here, to run repeatedly:
    if(ardu.nextFrame())
    {
      ardu.clear();
      //to get movement input, to move Cowbro and change frames on his animation
      if((ardu.pressed(RIGHT_BUTTON))&&(x < 112))
      {
        x += 2;
        frame += 1;
      }
      if((ardu.pressed(LEFT_BUTTON))&&(x > 0))
      {
        x -= 2;
        frame += 1;
      }
      if((ardu.pressed(UP_BUTTON))&&(y > 0))
      {
        y -= 2;
        frame += 1;
      }
      if((ardu.pressed(DOWN_BUTTON))&&(y < 48))
      {
        y += 2;
        frame += 1;
      }
      //to get shooting input and start shooting a bullet
      if((ardu.pressed(B_BUTTON)) && (!bulletActive))
      {
        bulletActive = true;
      }
      if(bulletActive)
      {
        bullet(x + 12, y + 8);
      }
      if(frame > 2)
      {
        ardu.drawBitmap(x, y, cowBro, 16, 16, WHITE);
      }
      else
      {
        ardu.drawBitmap(x, y, cowBroStep, 16, 16, WHITE);
      }
      if(frame >= 4)
      {
        frame = 0;
      }
      ardu.display();
    }
}
void bullet(int bulletX, int bulletY)
{
  for(int count = 0; count < 64; count += 2)
  {
    ardu.drawBitmap(bulletX + count, bulletY, bulletSprite, bulletX + 8 + count, bulletY + 8, WHITE);
  }
  bulletActive = false;
}


//
//  VirtualPeacefulKing.cpp
//  CS1666_SpaceForce
//
//  Created by XuZhisheng on 11/5/18.
//  Copyright © 2018 XuZhisheng. All rights reserved.
//

#include <stdio.h>
#include <cmath>
#include "VirtualPeacefulKing.h"

//Public methods


VirtualPeacefulKing::VirtualPeacefulKing(int initialHealth, SDL_Texture* sheet, int attack,int skillCD):hitPoints(initialHealth),kingSheet(sheet),attackPower(attack),phys(0,0,300.0,3600.0),xCoord(1280/10),yCoord(720/2),cd(skillCD)
{
    kingRect = {0, 0, 144, 144};
    kingCam = {1280/2, 720/2, 144, 144};
    
}

//Causing damage to the boss
void VirtualPeacefulKing::LostHealth(int damage)
{
    minusHealth(damage);
}

//set the current position of the king
void VirtualPeacefulKing::setPosition(double x, double y)
{
    xCoord = x;
    yCoord = y;
    
    checkBoundary();
    
    kingCam.x = (int) xCoord;
    kingCam.y = (int) yCoord;
}

//set the current velocity of the king
void VirtualPeacefulKing::setVelocity(double x, double y)
{
    phys.setxVelocity(x);
    phys.setyVelocity(y);
}

//Check the boundary so that our king will not move outside the screen
void VirtualPeacefulKing::checkBoundary()
{
    if (xCoord < 0)
        xCoord = 0;
    if (xCoord + kingCam.w > SCREEN_WIDTH)
        xCoord = SCREEN_WIDTH - kingCam.w;
    if (yCoord < 0)
        yCoord = 0;
    if (yCoord + kingCam.h > SCREEN_HEIGHT)
        yCoord = SCREEN_HEIGHT - kingCam.h;
}

//Move the king
void VirtualPeacefulKing::move(double deltax, double deltay, double step)
{
    phys.ChangeVelocity(deltax, deltay, step);
    
    xCoord += (phys.getxVelocity() * step);
    yCoord += (phys.getyVelocity() * step);
    
    //Our king will not move pass the screen
    checkBoundary();
    
    kingCam.x = (int) xCoord;
    kingCam.y = (int) yCoord;
}

//Animate the king
void VirtualPeacefulKing::animate(int frames)
{
    kingRect.x = ((frames / 10) % 6) * kingRect.w;
}

//Some accessors
double VirtualPeacefulKing::getXPosition()
{
    return xCoord;
}

double VirtualPeacefulKing::getYPosition()
{
    return yCoord;
}

SDL_Rect VirtualPeacefulKing::getRect()
{
    return kingRect;
}

SDL_Texture* VirtualPeacefulKing::getSheet()
{
    return kingSheet;
}

SDL_Rect VirtualPeacefulKing::getCamera()
{
    kingCam.x = (int) xCoord;
    kingCam.y = (int) yCoord;
    return kingCam;
}



//private methods
void VirtualPeacefulKing::minusHealth(int amount)
{
    hitPoints -= amount;
}

void VirtualPeacefulKing::plusHealth(int amount)
{
    hitPoints += amount;
}

bool VirtualPeacefulKing::hasCollision(Player *p)
{
    SDL_Rect pRect = p -> getPlayerCam();
    
    SDL_Rect result;
    
    //if (SDL_IntersectRect(<#const SDL_Rect *A#>, <#const SDL_Rect *B#>, <#SDL_Rect *result#>))
    
    return false;
}





#pragma once

#ifndef ENEMY_H
#define ENEMY_H
#include "Player.h"
#include "INC_SDL.h"
#include "physics.h"
#include "Player.h"
#include "attack.h"
#include <cmath>
#include "OpenGLRenderer.hpp"
#define MAX_SPEED 50

class Enemy
{
	public:

		//Constructor: takes health, character sheet, and attack value and sets all member vars
		Enemy(OpenGLRenderer *gRenderer, int startingHealth, char* characterImages, int attac, attack* player, char _type);

		//Subract hit points from the enemy
		void LostHealth(int damage);

		//Add hit points to the enemy
		void GainedHealth(int heal);

		//Return the enemy's current health points
		int GetHealth();

		//Return the enemy's current attack
		int GetAttack();

		void IncEnemySpeed(int addedSpeed);

		void DecEnemySpeed(int lostSpeed);

		int GetSpeed();

		//Set the position of the enemy on screen
		void setPosition(double x, double y);

		//Sets the current velocity of the enemy
		void setVelocity(double x, double y);

		//Methods that can be called from model class
		void move(double xdvel, double ydvel, double tstep);

		// Animate jet propulsion
		void animate(int frames);

		//Check for collision with the player
		void checkPlayerCollision(class Player* p, double tstep);

		//Return the current x velocity
		double getxVel();

		//Return the current x velocity
		double getyVel();

		//Get the enemy camera rectangle
		SDL_Rect getEnemyCam();

		//Get the current rectangle from the sprite sheet
		SDL_Rect getEnemyRect();

		//Get the enemy sprite sheet
		SDL_Texture* getEnemySheet();

		//Get a pointer to the enemy cam
		SDL_Rect* getEnemyCamLoc();

		void ChangeMaxVelocity(double Speed);

		char getType();

		//counts how many times an enemy has been hit
		void checkAttacked();

	private:

		/* Member variables:
		 * health, attack
		 * currently a character sheet but can be updated to
		 * OpenGL later
		 */

		double ACCEL = 3600.0;
		double SPEED_LIMIT = 300.0;
		const int SCREEN_WIDTH = 1280;
		const int SCREEN_HEIGHT = 720;
		int hitPoints;
		int speed;
		double attackPower;

		//Determine the type of enemy
		char type;

		attack* plyBlast;

		//Not perm obviously but here as a reminder to store enemy texture here
		char* enemySheet;
		Physics phys;

		SDL_Rect enemyCam;
		SDL_Rect enemyRect;

		OpenGLRenderer *openGL;
		RenderObject *render;

		double xCoord;
		double yCoord;

		void DecrementHealth(int decAmount);

		void IncrementHealth(int incAmount);

		void CheckBoundaries();

		void IncrementSpeed(int addedSpeed);

		void DecrementSpeed(int lostSpeed);

		bool hasCollision(Player* p);
};
#endif

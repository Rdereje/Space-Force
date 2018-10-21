
#ifndef PLAYER_H
#define PLAYER_H
#include "INC_SDL.h"
#include "physics.hpp"
#define MAX_SPEED 50


class Player
{
	public:
		
		//Constructor: takes health, character sheet, and attack value and sets all member vars
		Player(int startingHealth, SDL_Texture* characterImages, int attack);


		//Set the position of the player on screen
		void setPosition(int x, int y);

			
		//Methods that can be called from model class
		void move(double xdvel, double ydvel, double tstep);

		
		// Animate jet propulsion
		void animate(int frames);

		
		//Return the current x velocity
		double getxVel();

		
		//Get the player camera rectangle
		SDL_Rect getPlayerCam();

		//Get the current rectangle from the sprite sheet
		SDL_Rect getPlayerRect();
	
		
		//Get the player sprite sheet
		SDL_Texture* getPlayerSheet();

		
		//Subract hit points from the player
		void LostHealth(int damage);


		//Add hit points to the player
		void GainedHealth(int heal);


		//send in double modifiers to increase attack or defense by percentages
		void GainedPowerup(double extraAttack, double extraDefense);


		//Reset the attack and defence modifiers to normal
		void PowerupEnd();


		//Return the player's current health points
		int GetHealth();


		//Return the player's current attack
		int GetAttack();


	private:

		/* Member variables:
		 * health, attack, and modifiers for powerups
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
		double attackModifier;
		double defenseModifier;
		//Not perm obviously but here as a reminder to store player texture here
		SDL_Texture* playerSheet;
		Physics phys;

		SDL_Rect playerCam;
		SDL_Rect playerRect;
		
		double xCoord;
		double yCoord;
		
		void CheckBoundaries();
		
		//Private method to decrease player health
		void DecrementHealth(int decAmount);

		//Private method to increase player health
		void IncrementHealth(int incAmount);
};

#endif
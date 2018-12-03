// Includes
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "INC_SDL.h"
#include "Magnetar.h"
#include "AlcoholCloud.h"
#include "Player.h"
#include "Enemy.h"
#include "attack.h"
#include "blackhole.h"
#include "Menu.h"
#include "ClientInterface.h"
#include <cstdlib>
#include "HyperStar.h"
#include "music.h"
#include "Shield.h"
#include "VirtualPeacefulKing.h"

#include "OpenGLRenderer.hpp"

// Used for file walk (somewhat crudely)
#include <stdio.h>
#include <dirent.h>

// Constants for resolution
// Now declared globally in OpenGLRenderer.hpp
//~ const int SCREEN_WIDTH = 1280;
//~ const int SCREEN_HEIGHT = 720;

// Constants for level
const int LEVEL_LEN = 5120;
const int TILE_SIZE = 100;

// Constant for acceleration
//const double ACCEL = 3600.0;

// Parent folder for credit images
// Not const due to contrivance (can pass immediately if not const)
char CREDITS_FOLDER[] = "resources/Credit_Image/";

//The context that is used to render OpenGL to the SDL Window
SDL_GLContext gContext;

// Function declarations
bool init();
SDL_Texture* loadImage(std::string fname);
int close();

// Globals
SDL_Window* gWindow = nullptr;
OpenGLRenderer* gRenderer = nullptr;
SDL_Texture* gBackground;
SDL_Texture* gAttack;
SDL_Texture* gBlackhole;
SDL_Texture* gPlayerSheet;
SDL_Texture* gHealthbar;
std::vector<SDL_Texture*> gTex;
ClientInterface* client;

music mus;

bool init()
{
	// Flag what subsystems to initialize
	// For now, just video
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Set texture filtering to linear
	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}


	gWindow = SDL_CreateWindow(
		"Space Force",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
	);
	if (gWindow == nullptr)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	// Initialize PNG loading via SDL_image extension library
	int imgFlags = IMG_INIT_PNG;
	int retFlags = IMG_Init(imgFlags);
	if(retFlags != imgFlags)
	{
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}
	client = new ClientInterface();

	return true;
}

int close()
{
	mus.close();

	//~ SDL_DestroyRenderer(gRenderer);
    SDL_GL_DeleteContext(gContext);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();

	return 0;
}

// CREDITS
int playCredits(OpenGLRenderer *openGL)
{
	// Your compiler may complain because you are not supposed to do this
	// Credits to play
	std::string creditNames[] = {
		"resources/Credit_Image/carolyn_cole.png",
		"resources/Credit_Image/Credit_AnthonyMartrano.png",
		"resources/Credit_Image/DylanUmble.png",
		"resources/Credit_Image/KevinW_credit.png",
		"resources/Credit_Image/luke_malchow_bergenthal_1_3_FINAL_last_edge_lord.png",
		"resources/Credit_Image/RuthDereje.png",
		"resources/Credit_Image/ryan-kuhn.png",
		"resources/Credit_Image/ShreeSampath.png",
		"resources/Credit_Image/Zane_Credits.png",
		"resources/Credit_Image/zhishengXu.png"
	};

	int index = 0;
	bool creditsContinue = true;
	SDL_Event e;

	Uint32 startTime = SDL_GetTicks();
	double timeDelta = 0;

	std::cout << creditNames[index].c_str() << std::endl;
	RenderObject *currentCreditImage = new RenderObject(
		0, 0, 1, openGL->allBufferAttributes[creditNames[index].c_str()]
	);
	openGL->AppendRenderObject(currentCreditImage);

	openGL->Display();

	// Magic number length
	while (index < 10 && creditsContinue)
	{
		while(SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				creditsContinue = false;
			}
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					creditsContinue = false;
				}
				if (e.key.keysym.sym == SDLK_q)
				{
					creditsContinue = false;
				}
			}
		}

		// Wait 3 seconds
		if (timeDelta >= 3000)
		{
			index++;
			// Magic number length
			if (index > 9)
			{
				break;
			}

			const char *currentCreditName = creditNames[index].c_str();

			startTime = SDL_GetTicks();
			timeDelta = 0;

			// Kill image
			openGL->RemoveRenderObject(currentCreditImage->index);

			// Get new
			std::cout << currentCreditName << std::endl;
			currentCreditImage = new RenderObject(
				0, 0, 1, openGL->allBufferAttributes[currentCreditName]
			);
			openGL->AppendRenderObject(currentCreditImage);

			openGL->Display();
		}
		else
		{
			timeDelta = SDL_GetTicks() - startTime;
		}
	}

	if (currentCreditImage) {
		// Kill image
		openGL->RemoveRenderObject(currentCreditImage->index);
	}

	//close();
    return 0;
}

// Test main
int main(int argc, char* argv[]) {
	bool Multiplayer = false;

	if (!init())
	{
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}

	// Music
	mus = music();
	mus.init();
	mus.playMusic();

	// OpenGL init
	OpenGLRenderer openGL = OpenGLRenderer(gWindow);

	// Menu
	Menu menu = Menu(&openGL);
	int selection = menu.runMenu();
	//std::cout << selection << std::endl;
	while (selection == 2)
	{
		playCredits(&openGL);
		selection = menu.runMenu();
	}
	if (selection == 0)
	{
		close();
		return 0;
	}
	if (selection == 1 || selection == 3)
	{
		if(selection == 3){
			Multiplayer = true;
		}
		cout << "SELECTION " << selection << endl;
	}

	// Now that we are ready to start the game, clean the openGLRenderer
	openGL.TabulaRasa();

	// FPS params
	Uint32 fpsLasttime = SDL_GetTicks();
	Uint32 fpsCurtime = 0;
	Uint32 moveLasttime = SDL_GetTicks();
	double timestep = 0;

	// Rough sketch of a RenderObject?
	RenderObject *background1 = new RenderObject(
		0, 0, -1, openGL.allBufferAttributes["resources/imgs/space_2_background.png"]
	);

	RenderObject *background2 = new RenderObject(
		SCREEN_WIDTH, 0, -1, openGL.allBufferAttributes["resources/imgs/space_2_background.png"]
	);

	// Crude use of render queue, but background requires only a few lines of code anyway
	openGL.AppendRenderObject(background1);
	openGL.AppendRenderObject(background2);

	bool loop = 1;
	while (loop)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				loop = false;
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					loop = false;
					break;
				case SDLK_q:
					loop = false;
					break;
				default:
					break;
				}
			}
		}

		// Increment timestep
		timestep = (SDL_GetTicks() - moveLasttime) / 1000.0;

		// Move the background
		background1->ChangeCoordinates(background1->x - 1, background1->y, background1->z);
		background2->ChangeCoordinates(background2->x - 1, background2->y, background2->z);

		// Reset the background as necessary
		if (background1->x < -SCREEN_WIDTH)
		{
			background1->ChangeCoordinates(0, background1->y, background1->z);
			background2->ChangeCoordinates(SCREEN_WIDTH, background2->y, background2->z);
		}

		// Displays stuff
		openGL.Display();
	}

	openGL.Close();
	close();
	return -1;
}

// Test main
//~ int main(int argc, char* argv[]) {
	//~ bool Multiplayer = false;

	//~ if (!init())
	//~ {
		//~ std::cout <<  "Failed to initialize!" << std::endl;
		//~ close();
		//~ return 1;
	//~ }

	//~ // Music
	//~ mus = music();
	//~ mus.init();
	//~ mus.playMusic();

	//~ // OpenGL init
	//~ OpenGLRenderer openGL = OpenGLRenderer(gWindow);

	//~ // Menu
	//~ Menu menu = Menu(&openGL);
	//~ int selection = menu.runMenu();
	//~ //std::cout << selection << std::endl;
	//~ while (selection == 2)
	//~ {
		//~ playCredits(&openGL);
		//~ selection = menu.runMenu();
	//~ }
	//~ if (selection == 0)
	//~ {
		//~ close();
		//~ return 0;
	//~ }
	//~ if (selection == 1 || selection == 3)
	//~ {
		//~ if(selection == 3){
			//~ Multiplayer = true;
		//~ }
		//~ cout << "SELECTION " << selection << endl;
	//~ }

	//~ // Now that we are ready to start the game, clean the openGLRenderer
	//~ openGL.TabulaRasa();

	//~ // FPS params
	//~ Uint32 fpsLasttime = SDL_GetTicks();
	//~ Uint32 fpsCurtime = 0;
	//~ Uint32 moveLasttime = SDL_GetTicks();
	//~ double timestep = 0;

	//~ SDL_Rect attackRect = {0, 0, 60, 10};
	//~ SDL_Rect attackRect2 = {0, 0, 60, 10};
	//~ //SDL_Rect attackCam = {SCREEN_WIDTH+80, SCREEN_HEIGHT/2+51/2, 80, 20};
	//~ SDL_Rect blackholeRect = {0, 0, 300, 300};
	//~ SDL_Rect blackholeCam = {SCREEN_WIDTH,SCREEN_HEIGHT/2, 300, 300};
	//~ Player ply(10, loadImage("resources/imgs/starman.png"), 1, gRenderer);
	//~ Player ply2(10, loadImage("resources/imgs/starman_blue.png"), 1, gRenderer);


	//~ Enemy emy(&ply, loadImage("resources/imgs/faxanaduitis.png"), loadImage("resources/imgs/Faxanaduitis_Death.png"), 1, &ply.hit, 'f', &timestep);

    //~ //Our king appears!!!!!
    //~ VirtualPeacefulKing king(100, loadImage("resources/imgs/King.png"),2,4);

    //~ king.setPosition(1100, 0);
    //~ king.setVelocity(0, 50);


	//~ SDL_Rect healthRect = {0, 0, 177, 33};
	//~ SDL_Rect healthCam = {30, 30, 177, 33};

	//~ HyperStar stars(loadImage("resources/imgs/star4.png"),&ply);
	//~ blackhole enemyBlackhole(loadImage("resources/imgs/blackhole.png"), &ply);
	//~ Magnetar mag(&ply, loadImage("resources/imgs/Magnetars.png"));

	//~ // Removed for demo
	//~ AlcoholCloud ac(&ply, &emy, loadImage("resources/imgs/Alcohol_Cloud.png"), loadImage("resources/imgs/Alcohol_Cloud_Flare_Up.png"), &ply.hit);
	//~ double ACCEL = ply.GetMove();

	//~ //the beginning/default image and attack box
	//~ ply.hit.setAttack(gAttack,&attackRect);
	//~ ply2.hit.setAttack(gAttack, &attackRect2);
	//~ SDL_Event e;
	//~ bool gameOn = true;
	//~ bool up = true;
	//~ bool credits = true;

    //~ double emyDelta = 1;
    //~ double kingDelta = 1;

    //~ int connected = 0;

	//~ bool loop = 1;
	//~ while (loop)
	//~ {
		//~ SDL_Event event;
		//~ while (SDL_PollEvent(&event))
		//~ {
			//~ if (event.type == SDL_QUIT)
				//~ loop = false;
			//~ if (event.type == SDL_KEYDOWN)
			//~ {
				//~ switch (event.key.keysym.sym)
				//~ {
				//~ case SDLK_ESCAPE:
					//~ loop = false;
					//~ break;
				//~ case SDLK_q:
					//~ loop = false;
					//~ break;
				//~ default:
					//~ break;
				//~ }
			//~ }
		//~ }

		//~ // Increment timestep
		//~ timestep = (SDL_GetTicks() - moveLasttime) / 1000.0;

		//~ xDeltav = 0.0;
		//~ yDeltav = 0.0;
		//~ x2DeltaV = 0.0;
		//~ y2DeltaV = 0.0;

		//~ // WASD movement
		//~ const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		//~ if (keyState[SDL_SCANCODE_A])
			//~ xDeltav -= (abs(ACCEL) * timestep);
		//~ if (keyState[SDL_SCANCODE_D])
			//~ xDeltav += (abs(ACCEL) * timestep);
		//~ if (keyState[SDL_SCANCODE_W])
			//~ yDeltav -= (ACCEL * timestep);
		//~ if (keyState[SDL_SCANCODE_S])
			//~ yDeltav += (ACCEL * timestep);

        //~ //boundary check for king
        //~ if (king.getCamera().y + king.getCamera().h == SCREEN_HEIGHT)
        //~ {
            //~ kingDelta = -1;
            //~ king.setVelocity(0, -10);
        //~ }
        //~ if (king.getCamera().y == 0)
        //~ {
            //~ kingDelta = 1;
            //~ king.setVelocity(0, 10);
        //~ }

		//~ SDL_Rect pRect = ply.getPlayerRect();
		//~ SDL_Rect pCam = ply.getPlayerCam();
		//~ SDL_Rect pRect2 = ply2.getPlayerRect();
		//~ SDL_Rect pCam2 = ply.getPlayerCam();
		//~ SDL_Rect transfer;
        //~ SDL_Rect kRect = king.getRect();
        //~ SDL_Rect kCam = king.getCamera();

		//~ // Displays stuff
		//~ openGL.Display();

		//~ SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		//~ SDL_RenderClear(gRenderer);

		//~ rem = scrollOffset % SCREEN_WIDTH;
		//~ bgRect.x = -rem;


		//~ SDL_RenderCopy(gRenderer, gBackground, nullptr, &bgRect);
		//~ bgRect.x += SCREEN_WIDTH;
		//~ SDL_RenderCopy(gRenderer, gBackground, nullptr, &bgRect);

		//~ frames += 1 % 1000000000;

		//~ ply.animate(frames);
		//~ if(connected){
			//~ ply2.animate(frames);
		//~ }

        //~ king.animate(frames);


		//~ // Since game levels progress from L to R, no need for sprite to flip
		//~ // Code for flipping remains here if theres a change of plan

		//~ // Flip if facing other direction
		//~ /*if (ply.getxVel() > 0 && flip == SDL_FLIP_HORIZONTAL)
			//~ flip = SDL_FLIP_NONE;
		//~ else if (ply.getxVel() < 0 && flip == SDL_FLIP_NONE)
			//~ flip = SDL_FLIP_HORIZONTAL;*/

		//~ pRect = ply.getPlayerRect();
		//~ pCam = ply.getPlayerCam();

		//~ pRect2 = ply2.getPlayerRect();

        //~ Uint32 currTime = SDL_GetTicks();

        //~ if(currTime>=6000)
		//~ {
            //~ //std::cout << currTime % 3000 << std::endl;
			//~ if((currTime % 10000 <= 50 && !mag.Seen()) ||mag.Seen())
			//~ {

				//~ mag.Render();
			//~ }
			//~ if(currTime%3000<=20)
			//~ {
				//~ stars.addStar();
			//~ }
			//~ if(currTime%5000<=20)
			//~ {
				//~ protect.NewItem();
			//~ }
		//~ }
        //~ if(currTime >= 5000)
        //~ {
            //~ //int bFrames;
            //~ if((currTime % 5000 < 50 && !enemyBlackhole.seen()) || enemyBlackhole.seen())
            //~ {
                //~ //SDL_RenderCopy(gRenderer, gBlackhole, &blackholeRect, &blackholeCam);
                //~ //bFrames = 0;
                //~ //blackhole vacuum(gRenderer,gBlackhole,&blackholeRect,blackholeCam);
				//~ enemyBlackhole.showBlackhole(xDeltav, yDeltav, timestep);
            //~ }
         //~ /*    else
            //~ {
                //~ bFrames++;

                //~ if (bFrames / 12 > 5)
                    //~ bFrames = 0;

                //~ blackholeRect.x = (bFrames / 12) * 300;
                //~ blackholeCam.x = blackholeCam.x - 1;
                //~ SDL_RenderCopy(gRenderer, gBlackhole, &blackholeRect, &blackholeCam);

                //~ if(blackholeCam.x < SCREEN_WIDTH && blackholeCam.x > 0)
                //~ {
                    //~ if(blackholeCam.x + 150 > pCam.x)
                    //~ {
                        //~ if(blackholeCam.y + 150 > pCam.y)
                        //~ {
                            //~ yDeltav = yDeltav + 20;
                        //~ }
                        //~ if(blackholeCam.y + 150 < pCam.y)
                        //~ {
                            //~ yDeltav = yDeltav - 20;
                        //~ }
                        //~ xDeltav = xDeltav + 20;
                    //~ }
                    //~ else if(blackholeCam.x + 150 < pCam.x)
                    //~ {
                        //~ if(blackholeCam.y + 150 > pCam.y)
                        //~ {
                            //~ yDeltav = yDeltav + 20;
                        //~ }
                        //~ if(blackholeCam.y + 150 < pCam.y)
                        //~ {
                            //~ yDeltav = yDeltav - 20;
                        //~ }
                        //~ xDeltav = xDeltav - 20;
                    //~ }
                //~ }
            //~ }

            //~ if(blackholeCam.x == -300)
            //~ {
                //~ blackholeCam = {SCREEN_WIDTH,rand() % (SCREEN_HEIGHT-300), 300, 300};
                //~ bFrames = 0;
            //~ } */
        //~ }

		//~ ply.move(xDeltav, yDeltav, timestep);

		//~ if (emy.Exists())
		//~ {
			//~ bool collision = ply.checkEnemyCollision(&emy, timestep);

			//~ emy.Render();

			//~ if (collision && emy.GetHealth() > 0)
			//~ {
				//~ //ply.LostHealth(1);
				//~ if (healthRect.x == 1770)
				//~ {
					//~ return playCredits();
				//~ }
				//~ else
				//~ {
					//~ healthRect.x += 177;
				//~ }
			//~ }
		//~ }
		//~ else
		//~ {
			//~ if (emy.getNextSpawn() == 0)
			//~ {
				//~ emy.setNextSpawn((rand() % 5001) + 5000 + currTime);
			//~ }

			//~ if (currTime >= emy.getNextSpawn())
			//~ {
				//~ emy.Spawn();
			//~ }
		//~ }

		//~ if(healthRect.x >= 1598)//will now play credits when health is gone
		//~ {
			//~ return playCredits();
		//~ }

        //~ king.move(0, kingDelta, timestep);

		//~ /*
		//~ collision = emy.checkPlayerCollision(&ply, timestep);
		//~ if (collision)
		//~ {
			//~ //Minor enemies should be destroyed in event of collision
		//~ }
		//~ */
		//~ pCam = ply.getPlayerCam();

		//~ if(connected){
			//~ transfer = client->Communicate(pCam);
		//~ }
		//~ pCam2.x = transfer.x;
		//~ pCam2.y = transfer.y;

        //~ kCam = king.getCamera();

        //~ //attack button

		//~ if(keyState[SDL_SCANCODE_SPACE] && up == true)
		//~ {
			//~ up = false;
			//~ ply.hit.addAttack(pCam.x + 240,pCam.y + 51/2);

			//~ //play fire sound effect
			//~ mus.fireSound();
		//~ }
		//~ //lets the attack move across the screen
		//~ ply.hit.renderAttack(timestep);
		//~ SDL_RenderCopyEx(gRenderer, ply.getPlayerSheet(), &pRect, &pCam, 0.0, nullptr, flip);

		//~ protect.Render();
        //~ SDL_RenderCopyEx(gRenderer, king.getSheet(), &kRect, &kCam, 0.0, nullptr, flip);


		// removed for demo
		//~ if (ac.getDelay() == 0)
		//~ {
		 	//~ ac.setDelay((rand() % 3000) + 5000);
		//~ }

		//~ if (currTime >= ac.getDelay())
		//~ {
		 	//~ if (!ac.Seen())
		 	//~ {
		 		//~ ac.setYPosition(rand() % 421);
		 	//~ }

		 	//~ ac.Render();
		//~ }

		//~ stars.Render(timestep);

		//~ if(Multiplayer){
			//~ SDL_RenderCopyEx(gRenderer, ply2.getPlayerSheet(), &pRect2, &pCam2, 0.0, nullptr, flip);
		//~ }

		//~ SDL_RenderCopy(gRenderer, gHealthbar, &healthRect, &healthCam);

		//~ SDL_RenderPresent(gRenderer);

	//~ }
	//~ if (credits)
	//~ {
		//~ playCredits();
	//~ }

	//~ openGL.Close();
	//~ close();
	//~ return -1;
//~ }

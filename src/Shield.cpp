#include "Shield.h"
Shield::Shield(SDL_Texture* imItem, SDL_Texture* power, SDL_Texture* textIm, Player* main): item(imItem), protect(power), text(textIm), ply(main)
{
	gRenderer = ply->getRend();
	plyCam = ply ->getPlayerCamLoc();
	int size = 50;
	itemLoc = {1280,720,size,size};
	itemIm = {0,0,100,100};
	size = 300;
	protectLoc = {-size,-size,size,size};;
	protectIm = {0,0,300,300};;
	itemScreen = false;
	screen = false;
	hits = 0;
	addStrength = 2;
	ply -> shieldLocation(&protectLoc, &hits);
	ply->hasShield(false);
	textH = 128;
	textW = 837;
	textBox = {0,0, textW, textH};
	textScreen = {1280/2 - 700/2,720-100,700,100};
	endMessage = 3;
	startMessage = 5;
}
void Shield::Render()
{
	
	if(itemScreen)
	{
		if(!screen)
		{
			int range = 721 - itemLoc.h;
			itemLoc.y = rand()%range;
			screen = true;
		}
		SDL_RenderCopy(gRenderer, item, &itemIm, &itemLoc);
		bool intersect = SDL_HasIntersection(&itemLoc,plyCam);
		itemLoc.x = itemLoc.x - 1;
		if(itemLoc.x < -itemLoc.w || intersect)
		{
			itemScreen = false;
			screen = false;
			itemLoc.x = 1280;
			if(intersect)
			{
				startMessage = SDL_GetTicks();
				endMessage = startMessage+2000;
				ply->hasShield(true);
				hits = hits + addStrength;
			}
			
		}
		
	}
	if(hits <= 0)
	{
		hits = 0;
		ply->hasShield(false);
	}
	if(hits > 0)
	{
		RenderPower();
	}
	if(startMessage <= endMessage)
	{
		startMessage = SDL_GetTicks();
		Text();
	}
		
}
void Shield::RenderPower()
{
	protectLoc.x = plyCam->x - 30;
	protectLoc.y = plyCam->y - 125;
	SDL_RenderCopy(gRenderer, protect, &protectIm, &protectLoc);
	
}
void Shield::Damage(int hitsTaken)
{
	hits = hits - hitsTaken;
	if(hits <= 0)
	{
		hits = 0;
		ply->hasShield(false);
	}
}
void Shield::NewItem()
{
	itemScreen = true;
}
void Shield::Text()
{
	SDL_RenderCopy(gRenderer, text, &textBox, &textScreen);
}
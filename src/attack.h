#pragma once

#ifndef ATTACK_H
#define ATTACK_H
#include<stdio.h>
#include<stdlib.h>
#include "INC_SDL.h"
#include "OpenGLRenderer.hpp"

// Now uses a vector to monitor attacks

struct Node
{
	SDL_Rect attackCam;
	RenderObject *render;
	struct Node *next;
};
// What does this do?
struct interList
{
	int type;
	SDL_Rect* rect;
	SDL_Rect* circle;
	SDL_Rect* triangle;
};

class attack
{
	//~ static constexpr char attackTexture[] = "resources/imgs/attack.png";

	public:
		attack(OpenGLRenderer* Renderer);
		void addAttack(int x, int y);
		//~ void addAttack(SDL_Rect* attackBox);
		void renderAttack(double timestep);
		void setAttack(SDL_Rect* attac);
		int hitIntersect(SDL_Rect* rect);
	private:
		struct Node* head;
		struct Node* end;
		struct Node* curr;
		SDL_Rect cam;
		SDL_Rect* attackBox;
		OpenGLRenderer* openGL;
		SDL_Texture* gAttack;

		const char* attackTexture;

		//~ std::vector<RenderObject*> attackVector;
};
#endif

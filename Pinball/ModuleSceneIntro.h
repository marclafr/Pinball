#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void Animations();

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	uint score;
	uint record_score;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* background;
	SDL_Texture* elements;
	SDL_Texture* buttons_texture;
	SDL_Texture* shiny_points;
	SDL_Texture* shiny_twister_point;
	SDL_Texture* shiny_weels;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	//Animations
	//Animation* button_animation = nullptr;
	Animation button;
	Animation shiny_weels_animation;
};

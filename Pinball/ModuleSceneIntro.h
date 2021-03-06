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
	bool shiny_little_ball;
	iPoint pos;
	uint anim_time;
	bool shiny_pink_ball;
	bool twister_b;
	bool twister_end;
	bool yell_lev;
	uint yell_lev_time;



public:
	uint score;
	uint record_score;

	PhysBody* sensor;
	bool sensed;


	SDL_Texture* background;
	SDL_Texture* elements;
	SDL_Texture* buttons_texture;
	SDL_Texture* shiny_points;
	SDL_Texture* shiny_twister_point;
	SDL_Texture* shiny_weels;
	SDL_Texture* end_screen;
	SDL_Texture* yellow_lever;
	SDL_Texture* hammer_texture;

	uint pink_blue_fx;
	uint levers_fx;
	bool lever_down_l;
	bool lever_down_r;
	uint twister_fx;
	uint rest_elem;
	uint yell_lev_fx;
	uint lose_fx;
	uint button_click_fx;
	uint initial_spring;
	p2Point<int> ray;
	bool ray_on;

	//Animations
	Animation button_up;
	Animation button_down;
	Animation shiny_weels_animation;
	Animation shiny_point_animation;
	Animation twister;
	Animation yellow_lever_animation;
	Animation yellow_lever_animation1;
	Animation hammer_down;
	Animation hammer_up;

};

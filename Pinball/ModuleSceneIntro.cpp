#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	Animations();

	score = 0;
	record_score = 0;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	background = App->textures->Load("pinball/background.png");
	elements = App->textures->Load("pinball/pinball_elements.png");
	hammer_texture = App->textures->Load("pinball/hammers.png");
	buttons_texture = App->textures->Load("pinball/button.png");
	shiny_points = App->textures->Load("pinball/shiny_points.png");
	shiny_twister_point = App->textures->Load("pinball/shiny_twister_point.png");
	shiny_weels = App->textures->Load("pinball/shiny_weels.png");
	end_screen = App->textures->Load("pinball/Game_over.png");
	yellow_lever = App->textures->Load("pinball/yellow_lever.png");
	pink_blue_fx = App->audio->LoadFx("sounds/pink_blue_sound.wav");
	levers_fx = App->audio->LoadFx("sounds/Levers_sound.wav");
	twister_fx = App->audio->LoadFx("sounds/Twister_sound.wav");
	rest_elem = App->audio->LoadFx("sounds/rest_fxs.wav");
	yell_lev_fx = App->audio->LoadFx("sounds/yellow_lever_fx.wav");
	lose_fx = App->audio->LoadFx("sounds/Lose_fx.wav");
	button_click_fx = App->audio->LoadFx("sounds/button_click_fx.wav");
	initial_spring = App->audio->LoadFx("sounds/initial_spring.wav");
	anim_time = GetTickCount();
	yell_lev_time = GetTickCount();

	lever_down_l = true;
	lever_down_r = true;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->physics->force_makers.getFirst()->data->body->ApplyForceToCenter(b2Vec2(0, -100), true);
		App->physics->force_makers.getFirst()->next->next->data->body->ApplyForceToCenter(b2Vec2(0, -100), true);
		if (lever_down_l == true)
		{
			App->audio->PlayFx(levers_fx);
			lever_down_l = false;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		lever_down_l = true;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->physics->force_makers.getFirst()->next->data->body->ApplyForceToCenter(b2Vec2(0, -100), true);
		App->physics->force_makers.getLast()->data->body->ApplyForceToCenter(b2Vec2(0, -100), true);
		if (lever_down_r == true)
		{
			App->audio->PlayFx(levers_fx);
			lever_down_r = false;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		lever_down_r = true;

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c =	App->physics->map_bodies.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(background, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	c = App->physics->levers.getFirst();
	//Top left lever
	if (c != NULL)
	{
		SDL_Rect r;
		r.w = 43;
		r.h = 33;
		r.x = 95;
		r.y = 0;
		App->renderer->Blit(elements, + 25,  + 183, &r, 1.0f, c->data->GetRotation(), 8, 8);
		c = c->next;
	}
	//top_right_lever 
	if (c != NULL)
	{
		SDL_Rect r;
		r.w = 48;
		r.h = 26;
		r.x = 0;
		r.y = 0;
		App->renderer->Blit(elements, +275, +280, &r, 1.0f, c->data->GetRotation(), 37, 10);
		c = c->next;
	}
	//bot_left_lever 
	if (c != NULL)
	{
		SDL_Rect r;
		r.w = 48;
		r.h = 18;
		r.x = 138;
		r.y = 0;
		App->renderer->Blit(elements, 133, 501, &r, 1.0f, c->data->GetRotation(), 8, 8);
		c = c->next;
	}
	//bot_left_lever 
	if (c != NULL)
	{
		SDL_Rect r;
		r.w = 48;
		r.h = 19;
		r.x = 48;
		r.y = 0;
		App->renderer->Blit(elements, +199, +501, &r, 1.0f, c->data->GetRotation(), 37, 10);
		c = c->next;
	}
	//ball texture
	if (App->physics->ball != NULL)
	{
		int x_ball, y_ball;
		App->physics->ball->GetPosition(x_ball, y_ball);
		SDL_Rect r;
		r.w = 12;
		r.h = 12;
		r.x = 57;
		r.y = 27;
		App->renderer->Blit(elements, x_ball, y_ball, &r, 1.0f);
	}

	c = App->physics->red_wheels.getFirst();
	if (c != NULL)
	{
		SDL_Rect r;
		r.w = 38;
		r.h = 38;
		r.x = 154;
		r.y = 18;
		App->renderer->Blit(elements, +50, +278, &r, 1.0f, c->data->GetRotation());
		c = c->next;
	}
	if (c != NULL)
	{
		SDL_Rect r;
		r.w = 38;
		r.h = 38;
		r.x = 154;
		r.y = 18;
		App->renderer->Blit(elements, +231, +349, &r, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	if (App->physics->button_up_sensed == true)
	{
		App->renderer->Blit(buttons_texture, 235, 126, &(button_up.GetCurrentFrame()));
	}
	if (App->physics->button_pressed_sensed == true)
	{
		App->renderer->Blit(buttons_texture, 235, 126, &(button_down.GetCurrentFrame()));
		button_up.Reset();
		App->physics->button_pressed_sensed = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->physics->game_end == true)
	{
		App->physics->game_end = false;
	}

	if (App->physics->game_end == true)
	{
		App->renderer->Blit(end_screen, 0, 167, NULL, 1.0f, 0);
	}

	if (shiny_little_ball == true)
	{
		App->renderer->Blit(shiny_points, pos.x, pos.y, &(shiny_point_animation.GetCurrentFrame()));
		if (GetTickCount() - anim_time > 150)
		{
			anim_time = GetTickCount();
			shiny_little_ball = false;
		}
	}

	if (shiny_pink_ball == true)
	{
		App->renderer->Blit(shiny_weels, pos.x, pos.y, &(shiny_weels_animation.GetCurrentFrame()));
		if (shiny_weels_animation.Finished() == true)
		{
			shiny_pink_ball = false;
		}
	}
	
	if (twister_b == true)
	{
		App->renderer->Blit(shiny_twister_point, 116, 236, &(twister.GetCurrentFrame()));
		if (twister_end == true)
		{
			twister_b = false;
			twister_end = false;
		}
	}
	if (App->physics->hammer_down == true)
	{
		if (hammer_down.Finished() == true)
		{
			SDL_Rect hammer_r;
			hammer_r.w = 18;
			hammer_r.h = 15;
			hammer_r.x = 192;
			hammer_r.y = 0;
			App->renderer->Blit(elements, 358, 385, &hammer_r);
		}
		else 
		{
			App->renderer->Blit(hammer_texture, 358, 330, &(hammer_down.GetCurrentFrame()));
			App->physics->hammer_down = false;
		}
	}

	else if (App->physics->hammer_up == true)
	{
		App->renderer->Blit(hammer_texture, 358, 330, &(hammer_up.GetCurrentFrame()));
		App->physics->hammer_up = false;
	}

	else
	{
		App->renderer->Blit(hammer_texture, 358, 330);
	}

	if (yell_lev == true)
	{
		App->renderer->Blit(yellow_lever, 181, 60, &(yellow_lever_animation1.GetCurrentFrame()));
		
		if (GetTickCount() - yell_lev_time > 250)
		{
			yell_lev_time = GetTickCount();
			yell_lev = false;
		}
	}
	else
	{
		App->renderer->Blit(yellow_lever, 181, 60, &(yellow_lever_animation.GetCurrentFrame()));
	}
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{}

void ModuleSceneIntro::Animations()
{
	shiny_weels_animation.PushBack({ 22, 0, 23, 24 });
	shiny_weels_animation.PushBack({ 46, 0, 23, 24 });
	shiny_weels_animation.PushBack({ 71, 0, 23, 24 });
	shiny_weels_animation.PushBack({ 95, 0, 23, 24 });

	shiny_weels_animation.loop = true;
	shiny_weels_animation.speed = 0.5f;

	shiny_point_animation.PushBack({ 7, 0, 8, 8 });
	shiny_point_animation.PushBack({ 15, 0, 8, 8 });
	shiny_point_animation.PushBack({ 24, 0, 8, 8 });
	shiny_point_animation.loop = true;
	shiny_point_animation.speed = 0.3;

	twister.PushBack({ 0, 0, 34, 34 });
	twister.PushBack({ 34, 0, 34, 34 });
	twister.PushBack({ 68, 0, 34, 34 });
	twister.PushBack({ 102, 0, 34, 34 });
	twister.PushBack({ 136, 0, 34, 34 });
	twister.PushBack({ 170, 0, 34, 34 });
	twister.PushBack({ 204, 0, 34, 34 });
	twister.PushBack({ 238, 0, 34, 34 });
	twister.loop = true;
	twister.speed = 0.8f;

	shiny_weels_animation.PushBack({ 22, 0, 23, 24 });
	shiny_weels_animation.PushBack({ 46, 0, 23, 24 });
	shiny_weels_animation.PushBack({ 71, 0, 23, 24 });
	shiny_weels_animation.PushBack({ 95, 0, 23, 24 });
	shiny_weels_animation.loop = false;
	shiny_weels_animation.speed = 0.5f;

	yellow_lever_animation.PushBack({ 0, 0, 57, 19 });
	yellow_lever_animation.loop = false;
	yellow_lever_animation.speed = 0.4f;

	hammer_down.PushBack({ 0, 0, 18, 69 });
	hammer_down.PushBack({ 18, 0, 18, 69 });
	hammer_down.PushBack({ 36, 0, 18, 69 });
	hammer_down.PushBack({ 54, 0, 18, 69 });
	hammer_down.PushBack({ 72, 0, 18, 69 });
	hammer_down.PushBack({ 90, 0, 18, 69 });
	hammer_down.PushBack({ 108, 0, 18, 69 });
	hammer_down.PushBack({ 126, 0, 18, 69 });
	hammer_down.PushBack({ 145, 0, 18, 69 });
	hammer_down.loop = true;
	hammer_down.speed = 0.4f;
	

	hammer_up.PushBack({ 145, 0, 18, 69 });
	hammer_up.PushBack({ 126, 0, 18, 69 });
	hammer_up.PushBack({ 108, 0, 18, 69 });
	hammer_up.PushBack({ 90, 0, 18, 69 });
	hammer_up.PushBack({ 72, 0, 18, 69 });
	hammer_up.PushBack({ 54, 0, 18, 69 });
	hammer_up.PushBack({ 36, 0, 18, 69 });
	hammer_up.PushBack({ 18, 0, 18, 69 });
	hammer_up.PushBack({ 0, 0, 18, 69 });
	hammer_up.loop = true;
	hammer_up.speed = 5.0f;

	yellow_lever_animation1.PushBack({ 0, 0, 57, 19 });
	yellow_lever_animation1.PushBack({ 56, 0, 57, 19 });
	yellow_lever_animation1.PushBack({ 116, 0, 57, 19 });
	yellow_lever_animation1.PushBack({ 175, 0, 57, 19 });
	yellow_lever_animation1.PushBack({ 175, 0, 57, 19 });
	yellow_lever_animation1.PushBack({ 116, 0, 57, 19 });
	yellow_lever_animation1.PushBack({ 56, 0, 57, 19 });
	yellow_lever_animation1.PushBack({ 0, 0, 57, 19 });
	yellow_lever_animation1.loop = true;
	yellow_lever_animation1.speed = 0.4f;

	button_up.PushBack({ 0, 0, 51, 77 });
	button_up.PushBack({ 51, 0, 51, 77 });
	button_up.PushBack({ 111, 0, 51, 77 });
	button_up.PushBack({ 180, 0, 51, 77 });
	button_up.PushBack({ 251, 0, 51, 77 });
	button_up.PushBack({ 319, 0, 51, 77 });
	button_up.PushBack({ 393, 0, 51, 77 });
	button_up.PushBack({ 457, 0, 51, 77 });
	button_up.PushBack({ 524, 0, 51, 77 });
	button_up.PushBack({ 586, 0, 51, 77 });
	button_up.PushBack({ 654, 0, 51, 77 });
	button_up.PushBack({ 720, 0, 51, 77 });
	button_up.PushBack({ 788, 0, 51, 77 });
	button_up.loop = false;
	button_up.speed = 0.4f;

	App->scene_intro->button_down.PushBack({ 788, 0, 51, 77 });
	App->scene_intro->button_down.PushBack({ 720, 0, 51, 77 });
	App->scene_intro->button_down.PushBack({ 654, 0, 51, 77 });
	App->scene_intro->button_down.PushBack({ 586, 0, 51, 77 });
	App->scene_intro->button_down.PushBack({ 524, 0, 51, 77 });
	App->scene_intro->button_down.PushBack({ 457, 0, 51, 77 });
	App->scene_intro->button_down.PushBack({ 393, 0, 51, 77 });
	App->scene_intro->button_down.PushBack({ 319, 0, 51, 77 });
	App->scene_intro->button_down.PushBack({ 251, 0, 51, 77 });
	App->scene_intro->button_down.PushBack({ 180, 0, 51, 77 });
	App->scene_intro->button_down.PushBack({ 111, 0, 51, 77 });
	App->scene_intro->button_down.PushBack({ 51, 0, 51, 77 });
	App->scene_intro->button_down.PushBack({ 0, 0, 51, 77 });
	App->scene_intro->button_down.loop = false;
	App->scene_intro->button_down.speed = 0.5f;
}
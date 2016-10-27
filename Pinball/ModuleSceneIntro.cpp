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
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
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

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	background = App->textures->Load("pinball/background.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
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

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		//ray_on = !ray_on;
		//ray.x = App->input->GetMouseX();
		//ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		//boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64, 0));
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = App->physics->map_bodies.getFirst();

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

	App->renderer->Blit(buttons_texture, 235, 126, &(button.GetCurrentFrame()));
	

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
		App->renderer->Blit(hammer_texture, 358, 330, &(hammer_down.GetCurrentFrame()));
		App->physics->hammer_down = false;
	}

	if (App->physics->hammer_up == true)
	{
		App->renderer->Blit(hammer_texture, 358, 330, &(hammer_up.GetCurrentFrame()));
		App->physics->hammer_up = false;
	}
	App->renderer->Blit(buttons_texture, 235, 126, &(button.GetCurrentFrame()));
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

	SDL_Rect hammer_r;
	hammer_r.w = 28;
	hammer_r.h = 69;
	hammer_r.x = 210;
	hammer_r.y = 0;
	App->renderer->Blit(elements, 352, 390, &hammer_r);
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;


	if (bodyA)
	{
		//if(bodyB == lose_sensor)
		App->audio->PlayFx(bonus_fx);
	}
}

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

	App->scene_intro->shiny_weels_animation.PushBack({ 22, 0, 23, 24 });
	App->scene_intro->shiny_weels_animation.PushBack({ 46, 0, 23, 24 });
	App->scene_intro->shiny_weels_animation.PushBack({ 71, 0, 23, 24 });
	App->scene_intro->shiny_weels_animation.PushBack({ 95, 0, 23, 24 });
	App->scene_intro->shiny_weels_animation.loop = false;
	App->scene_intro->shiny_weels_animation.speed = 0.5f;

	App->scene_intro->yellow_lever_animation.PushBack({ 0, 0, 57, 19 });
	App->scene_intro->yellow_lever_animation.loop = false;
	App->scene_intro->yellow_lever_animation.speed = 0.4f;

	App->scene_intro->hammer_down.PushBack({ 0, 0, 18, 69 });
	App->scene_intro->hammer_down.PushBack({ 18, 0, 18, 69 });
	App->scene_intro->hammer_down.PushBack({ 36, 0, 18, 69 });
	App->scene_intro->hammer_down.PushBack({ 54, 0, 18, 69 });
	App->scene_intro->hammer_down.PushBack({ 72, 0, 18, 69 });
	App->scene_intro->hammer_down.PushBack({ 90, 0, 18, 69 });
	App->scene_intro->hammer_down.PushBack({ 108, 0, 18, 69 });
	App->scene_intro->hammer_down.PushBack({ 126, 0, 18, 69 });
	App->scene_intro->hammer_down.PushBack({ 145, 0, 18, 69 });
	App->scene_intro->hammer_down.loop = false;
	App->scene_intro->hammer_down.speed = 0.4f;
	

	App->scene_intro->hammer_up.PushBack({ 145, 0, 18, 69 });
	App->scene_intro->hammer_up.PushBack({ 126, 0, 18, 69 });
	App->scene_intro->hammer_up.PushBack({ 108, 0, 18, 69 });
	App->scene_intro->hammer_up.PushBack({ 90, 0, 18, 69 });
	App->scene_intro->hammer_up.PushBack({ 72, 0, 18, 69 });
	App->scene_intro->hammer_up.PushBack({ 54, 0, 18, 69 });
	App->scene_intro->hammer_up.PushBack({ 36, 0, 18, 69 });
	App->scene_intro->hammer_up.PushBack({ 18, 0, 18, 69 });
	App->scene_intro->hammer_up.PushBack({ 0, 0, 18, 69 });
	App->scene_intro->hammer_up.loop = false;
	App->scene_intro->hammer_up.speed = 5.0f;

	//Button up
	App->audio->PlayFx(App->scene_intro->yell_lev_fx);
	App->scene_intro->yellow_lever_animation1.PushBack({ 0, 0, 57, 19 });
	App->scene_intro->yellow_lever_animation1.PushBack({ 56, 0, 57, 19 });
	App->scene_intro->yellow_lever_animation1.PushBack({ 116, 0, 57, 19 });
	App->scene_intro->yellow_lever_animation1.PushBack({ 175, 0, 57, 19 });
	App->scene_intro->yellow_lever_animation1.PushBack({ 175, 0, 57, 19 });
	App->scene_intro->yellow_lever_animation1.PushBack({ 116, 0, 57, 19 });
	App->scene_intro->yellow_lever_animation1.PushBack({ 56, 0, 57, 19 });
	App->scene_intro->yellow_lever_animation1.PushBack({ 0, 0, 57, 19 });
	App->scene_intro->yellow_lever_animation1.loop = true;
	App->scene_intro->yellow_lever_animation1.speed = 0.4f;
}
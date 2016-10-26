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

	score = 0;
	record_score = 0;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	background = App->textures->Load("pinball/background.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	elements = App->textures->Load("pinball/pinball_elements.png");

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
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->physics->force_makers.getFirst()->next->data->body->ApplyForceToCenter(b2Vec2(0, -100), true);
		App->physics->force_makers.getLast()->data->body->ApplyForceToCenter(b2Vec2(0, -100), true);
	}

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
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
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

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
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

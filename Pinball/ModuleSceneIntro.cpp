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

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	background = App->textures->Load("pinball/background.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	//sensor_start = App->physics->CreateRectangleSensor(365, 325, 15, 20);

	//initial ball
	ball = App->physics->CreateCircle(365, 325, 6);

	// Pivot 0, 0
	int pinball_contorn[102] = {
		142, 594,
		114, 580,
		68, 557,
		42, 529,
		36, 497,
		49, 470,
		52, 434,
		46, 386,
		37, 347,
		30, 348,
		28, 370,
		21, 378,
		11, 377,
		7, 271,
		20, 246,
		39, 230,
		68, 215,
		50, 190,
		22, 148,
		8, 116,
		11, 90,
		36, 56,
		64, 32,
		98, 13,
		132, 4,
		179, 2,
		225, 2,
		279, 20,
		324, 50,
		357, 88,
		378, 134,
		378, 325,
		365, 332,
		354, 324,
		354, 199,
		336, 242,
		315, 276,
		280, 302,
		304, 333,
		332, 373,
		346, 416,
		350, 462,
		345, 511,
		318, 552,
		276, 578,
		239, 597,
		388, 597,
		386, 0,
		2, -1,
		3, 602,
		142, 602
	};

	int top_collider[16] = {
		170, 29,
		209, 28,
		208, 20,
		182, 18,
		159, 18,
		132, 21,
		132, 29,
		167, 29
	};

	int top_left_collider[18] = {
		46, 127,
		52, 137,
		57, 145,
		52, 150,
		46, 144,
		40, 136,
		35, 122,
		41, 119,
		44, 124
	};

	int top_right_collider[36] = {
		253, 108,
		245, 99,
		243, 88,
		244, 78,
		237, 76,
		237, 89,
		239, 97,
		241, 105,
		245, 111,
		253, 116,
		259, 122,
		268, 124,
		277, 126,
		283, 125,
		279, 118,
		272, 118,
		264, 115,
		257, 112
	};

	int mid_right_collider[32] = {
		265, 261,
		257, 251,
		247, 243,
		233, 236,
		222, 233,
		217, 237,
		224, 239,
		232, 244,
		241, 249,
		248, 254,
		254, 261,
		259, 267,
		262, 273,
		266, 278,
		269, 274,
		267, 264
	};

	int mid_left_collider[72] = {
		136, 300,
		127, 291,
		118, 282,
		107, 272,
		94, 263,
		82, 257,
		73, 257,
		62, 258,
		53, 262,
		46, 270,
		39, 277,
		36, 283,
		35, 289,
		41, 283,
		45, 279,
		50, 274,
		53, 270,
		59, 266,
		65, 263,
		71, 263,
		79, 263,
		86, 266,
		93, 270,
		99, 275,
		108, 282,
		113, 289,
		120, 295,
		126, 301,
		132, 307,
		137, 313,
		141, 318,
		147, 324,
		150, 322,
		150, 319,
		144, 311,
		139, 304
	};

	int bot_left_collider[12] = {
		122, 429,
		110, 410,
		108, 439,
		119, 448,
		135, 448,
		124, 432
	};


	int bot_right_collider[10] = {
		245, 449,
		257, 450,
		270, 440,
		272, 410,
		245, 448
	};

	int middle_wall[18] = {
		163, 358,
		164, 364,
		166, 369,
		164, 375,
		159, 375,
		157, 365,
		157, 356,
		160, 354,
		163, 356
	};

	int top_left_lever[28] = {
		30, 201,
		41, 206,
		54, 212,
		61, 216,
		66, 217,
		69, 216,
		68, 213,
		56, 200,
		45, 189,
		37, 183,
		30, 184,
		25, 189,
		25, 196,
		29, 200
	};

	int bot_left_lever[26] = {
		157, 519,
		142, 521,
		134, 520,
		130, 515,
		131, 507,
		137, 502,
		147, 503,
		163, 506,
		179, 509,
		180, 512,
		179, 515,
		172, 516,
		158, 519
	};

	int bot_right_lever[26] = {
		214, 518,
		234, 521,
		245, 520,
		249, 514,
		246, 507,
		242, 503,
		233, 503,
		212, 507,
		204, 508,
		199, 509,
		198, 512,
		199, 515,
		213, 517
	};

	int top_right_lever[24] = {
		293, 304,
		315, 300,
		321, 297,
		322, 290,
		320, 284,
		314, 281,
		308, 282,
		291, 292,
		275, 301,
		274, 304,
		277, 306,
		291, 304
	};

	//Map contorn
	map_bodies.add(App->physics->CreateChain(0, 0, pinball_contorn, 102));
	//Map walls
	map_bodies.add(App->physics->CreateChain(0, 0, top_collider, 16));
	map_bodies.add(App->physics->CreateChain(0, 0, top_left_collider, 18));
	map_bodies.add(App->physics->CreateChain(0, 0, top_right_collider, 36));
	map_bodies.add(App->physics->CreateChain(0, 0, mid_right_collider, 32));
	map_bodies.add(App->physics->CreateChain(0, 0, mid_left_collider, 72));
	map_bodies.add(App->physics->CreateChain(0, 0, bot_left_collider, 12));
	map_bodies.add(App->physics->CreateChain(0, 0, bot_right_collider, 10));
	map_bodies.add(App->physics->CreateChain(0, 0, middle_wall, 18));
	//Levers
	map_bodies.add(App->physics->CreateChain(0, 0, top_left_lever, 28));
	map_bodies.add(App->physics->CreateChain(0, 0, top_right_lever, 24));
	map_bodies.add(App->physics->CreateChain(0, 0, bot_left_lever, 26));
	map_bodies.add(App->physics->CreateChain(0, 0, bot_right_lever, 26));


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
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
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

	c = map_bodies.getFirst();

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

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}

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
	lose_sensor = App->physics->CreateRectangleSensor(190, 590, 115, 50);

	//initial ball
	ball = App->physics->CreateCircle(365, 325, 6);

	// Pivot 0, 0
	//Contorn
	/*int pinball_contorn[306] = {
		141, 599,
		141, 595,
		131, 593,
		122, 590,
		109, 587,
		98, 582,
		86, 575,
		68, 563,
		55, 547,
		48, 535,
		39, 518,
		38, 503,
		37, 492,
		39, 480,
		43, 465,
		45, 451,
		47, 435,
		48, 417,
		47, 398,
		44, 380,
		42, 366,
		39, 359,
		35, 346,
		31, 346,
		29, 351,
		29, 357,
		29, 364,
		26, 371,
		21, 375,
		15, 376,
		10, 376,
		8, 370,
		8, 357,
		8, 343,
		8, 326,
		8, 303,
		8, 282,
		8, 270,
		10, 262,
		15, 255,
		20, 248,
		25, 242,
		31, 236,
		39, 230,
		46, 225,
		51, 222,
		58, 217,
		49, 213,
		31, 206,
		20, 196,
		24, 182,
		32, 179,
		39, 181,
		33, 172,
		25, 160,
		18, 147,
		12, 131,
		8, 116,
		8, 103,
		15, 83,
		25, 66,
		35, 55,
		49, 43,
		62, 33,
		77, 24,
		92, 16,
		111, 10,
		133, 6,
		157, 4,
		171, 3,
		185, 3,
		202, 3,
		218, 4,
		231, 5,
		252, 11,
		271, 17,
		285, 23,
		297, 30,
		310, 38,
		320, 47,
		334, 59,
		345, 71,
		354, 83,
		361, 94,
		368, 107,
		373, 122,
		375, 130,
		376, 136,
		377, 145,
		377, 164,
		377, 189,
		377, 208,
		377, 227,
		377, 248,
		377, 258,
		377, 275,
		377, 325,
		371, 329,
		371, 463,
		362, 463,
		362, 329,
		355, 325,
		355, 315,
		355, 262,
		355, 204,
		352, 200,
		350, 206,
		348, 213,
		344, 223,
		339, 236,
		333, 248,
		326, 259,
		323, 265,
		318, 270,
		314, 274,
		310, 278,
		320, 276,
		326, 281,
		329, 289,
		326, 301,
		317, 304,
		306, 307,
		294, 308,
		285, 309,
		289, 313,
		296, 321,
		302, 331,
		309, 341,
		317, 353,
		324, 363,
		329, 373,
		337, 388,
		342, 405,
		346, 418,
		348, 436,
		350, 456,
		349, 471,
		348, 487,
		345, 505,
		343, 520,
		336, 535,
		321, 552,
		299, 568,
		280, 579,
		259, 590,
		242, 597,
		242, 600,
		593, 599,
		592, 2,
		4, 2,
		5, 600,
		131, 600,
		140, 600
	};*/
	//Walls
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

	int mid_left_collider[82] = {
		38, 288,
		40, 285,
		44, 279,
		48, 274,
		53, 269,
		59, 264,
		65, 262,
		73, 261,
		80, 262,
		88, 265,
		95, 269,
		102, 274,
		108, 280,
		115, 286,
		123, 295,
		132, 305,
		138, 312,
		143, 319,
		147, 324,
		150, 323,
		150, 319,
		146, 314,
		140, 306,
		134, 298,
		127, 290,
		120, 283,
		112, 276,
		103, 268,
		96, 263,
		88, 259,
		80, 257,
		71, 257,
		62, 258,
		55, 262,
		48, 267,
		42, 274,
		37, 282,
		35, 286,
		35, 289,
		37, 290,
		39, 287
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
	//Levers
	int yellow_lever[34] = {
		189, 76,
		201, 75,
		214, 73,
		219, 72,
		223, 74,
		229, 74,
		233, 70,
		233, 63,
		227, 59,
		221, 60,
		215, 65,
		205, 66,
		192, 69,
		182, 72,
		180, 72,
		180, 76,
		187, 76
	};

	//Walls close to weels
	int wall_red_weel[46] = {
		247, 335,
		258, 335,
		267, 340,
		276, 348,
		283, 359,
		285, 370,
		287, 376,
		290, 378,
		292, 374,
		289, 359,
		283, 348,
		273, 338,
		263, 332,
		256, 331,
		245, 331,
		235, 335,
		225, 340,
		219, 344,
		218, 347,
		220, 348,
		227, 344,
		236, 340,
		246, 335
	};

	int right_lever_curve[44] = {
		311, 417,
		313, 424,
		310, 428,
		306, 445,
		301, 462,
		288, 479,
		272, 494,
		261, 505,
		250, 513,
		251, 508,
		248, 503,
		242, 501,
		254, 496,
		267, 488,
		277, 481,
		286, 473,
		294, 463,
		299, 454,
		302, 443,
		303, 431,
		305, 421,
		308, 417
	};

	int left_lever_curve[48] = {
		102, 489,
		114, 500,
		125, 510,
		129, 514,
		129, 511,
		130, 505,
		135, 501,
		137, 500,
		117, 491,
		100, 480,
		87, 465,
		80, 452,
		76, 436,
		74, 421,
		72, 416,
		67, 417,
		66, 422,
		68, 426,
		71, 427,
		71, 436,
		75, 451,
		81, 466,
		91, 479,
		100, 488
	};

	int top_right_right[54] = {
		342, 171,
		344, 162,
		346, 150,
		345, 135,
		342, 121,
		334, 103,
		324, 87,
		313, 73,
		303, 62,
		297, 56,
		294, 57,
		294, 61,
		303, 71,
		313, 83,
		321, 93,
		329, 105,
		335, 118,
		339, 130,
		340, 140,
		340, 151,
		338, 162,
		335, 173,
		334, 179,
		335, 182,
		339, 179,
		340, 177,
		341, 174
	};

	int end_wall_left[40] = {
		77, 539,
		88, 548,
		102, 557,
		118, 562,
		136, 565,
		148, 564,
		150, 560,
		148, 552,
		134, 545,
		125, 540,
		112, 532,
		99, 519,
		88, 507,
		78, 494,
		73, 488,
		68, 489,
		63, 500,
		62, 509,
		64, 523,
		75, 537
	};

	int end_wall_right[50] = {
		317, 493,
		320, 506,
		317, 521,
		311, 532,
		299, 543,
		284, 553,
		267, 560,
		252, 563,
		241, 564,
		234, 563,
		231, 557,
		233, 551,
		238, 547,
		245, 545,
		252, 542,
		263, 538,
		274, 531,
		282, 522,
		288, 514,
		295, 505,
		301, 498,
		304, 492,
		307, 489,
		312, 487,
		316, 490
	};


	//Map contorn
	//map_bodies.add(App->physics->CreateChain(0, 0, pinball_contorn, 306));
	//Map walls
	map_bodies.add(App->physics->CreateChain(0, 0, top_collider, 16));
	map_bodies.add(App->physics->CreateChain(0, 0, top_left_collider, 18));
	map_bodies.add(App->physics->CreateChain(0, 0, top_right_collider, 36));
	map_bodies.add(App->physics->CreateChain(0, 0, mid_right_collider, 32));
	map_bodies.add(App->physics->CreateChain(0, 0, mid_left_collider, 82));
	map_bodies.add(App->physics->CreateChain(0, 0, bot_left_collider, 12));
	map_bodies.add(App->physics->CreateChain(0, 0, bot_right_collider, 10));
	map_bodies.add(App->physics->CreateChain(0, 0, middle_wall, 18));
	map_bodies.add(App->physics->CreateChain(0, 0, wall_red_weel, 46));
	map_bodies.add(App->physics->CreateChain(0, 0, right_lever_curve, 44));
	map_bodies.add(App->physics->CreateChain(0, 0, left_lever_curve, 48));
	map_bodies.add(App->physics->CreateChain(0, 0, top_right_right, 54));
	map_bodies.add(App->physics->CreateChain(0, 0, end_wall_left, 40));
	map_bodies.add(App->physics->CreateChain(0, 0, end_wall_right, 50));
	//Levers

	map_bodies.add(App->physics->CreateChain(0, 0, yellow_lever, 34));

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
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{

	}

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

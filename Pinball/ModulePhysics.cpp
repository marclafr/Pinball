#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"
#include "ModuleSceneIntro.h"
#include "ModuleAudio.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);
	
	b2BodyDef bd;
	ground = world->CreateBody(&bd);
	
	lives = 4;
	game_end = false;

	CreateMap();
	CreateSensors();
	CreateLevers();
	CreateScrewers(); //Circles to rotate levers
	CreateLeverForceMakers();

	CreateRevoutionJoints();

	start_time = GetTickCount();
	bonus_time = GetTickCount() + 30000;
	rot_time = GetTickCount() + 4000;

	return true;
}

void ModulePhysics::CreateMap()
{
	//initial ball
	ball = CreateCircle(365, 325, 6);
	ball->body->IsBullet();
	ball->listener = this;

	//Contorn
	int pinball_contorn[306] = {
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
	};

	//Walls
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

	int bot_left_collider[16] = {
		110, 408,
		108, 438,
		121, 449,
		137, 448,
		139, 443,
		124, 441,
		115, 434,
		114, 407
	};

	int bot_right_collider[16] = {
		243, 449,
		261, 450,
		273, 441,
		271, 406,
		265, 409,
		268, 434,
		260, 443,
		242, 443
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

	int left_lever_curve[68] = {
		74, 422,
		74, 429,
		75, 436,
		77, 442,
		80, 450,
		83, 456,
		87, 463,
		93, 470,
		101, 478,
		108, 484,
		117, 489,
		123, 493,
		129, 497,
		134, 499,
		133, 502,
		130, 504,
		128, 507,
		128, 512,
		130, 515,
		124, 509,
		116, 502,
		105, 492,
		93, 480,
		85, 471,
		79, 460,
		75, 450,
		72, 441,
		71, 432,
		71, 427,
		68, 425,
		66, 420,
		68, 417,
		72, 417,
		74, 420
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
	map_bodies.add(App->physics->CreateChain(0, 0, pinball_contorn, 306, 0));
	//Map walls
	map_bodies.add(CreateChain(0, 0, top_right_collider, 36, 0));
	map_bodies.add(CreateChain(0, 0, mid_right_collider, 32, 0));
	map_bodies.add(CreateChain(0, 0, mid_left_collider, 82, 0));
	map_bodies.add(CreateChain(0, 0, bot_left_collider, 16, 0));
	map_bodies.add(CreateChain(0, 0, bot_right_collider, 16, 0));

	//Elements that give points

	//Mid right pink weel
	elements_100_p.add(CreateStaticCircle(224, 283, 12, 1));
	//Mid right pink weel
	elements_100_p.add(CreateStaticCircle(310, 160, 12, 1));
	//Down blue weel
	elements_100_p.add(CreateStaticCircle(289, 96, 11, 1));
	//Top blue weel
	elements_100_p.add(CreateStaticCircle(270, 78, 11, 1));

	//top walls
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
	elements_100_p.add(CreateChain(0, 0, top_collider, 16, 0.7f));
	elements_100_p.add(CreateChain(0, 0, top_left_collider, 18, 0.7f));

	//bot restitution walls
	int res_left[8] = {
		111, 410,
		111, 413,
		134, 445,
		139, 446
	};
	elements_100_p.add(CreateStaticRestPolygon(0, 0, res_left, 8, 1.5f));
	int res_right[8] = {
		243, 446,
		249, 446,
		270, 415,
		268, 408
	};
	elements_100_p.add(CreateStaticRestPolygon(0, 0, res_right, 8, 1.5f));


	map_bodies.add(CreateChain(0, 0, middle_wall, 18, 0));
	map_bodies.add(CreateChain(0, 0, wall_red_weel, 46, 0));
	map_bodies.add(CreateChain(0, 0, right_lever_curve, 44, 0));
	map_bodies.add(CreateChain(0, 0, left_lever_curve, 68, 0));
	map_bodies.add(CreateChain(0, 0, top_right_right, 54, 0));
	map_bodies.add(CreateChain(0, 0, end_wall_left, 40, 0));
	map_bodies.add(CreateChain(0, 0, end_wall_right, 50, 0));
	
	//Yellow lever
	CreateStaticCircle(234, 71, 4, 0);

	//Bot left red weel
	red_wheels.add(CreateCircle(69, 297, 18));
	//Bot right red weel
	red_wheels.add(CreateCircle(250, 368, 18));

	screwers.add(CreateStaticCircle(69, 297, 2, 0));
	screwers.add(CreateStaticCircle(250, 368, 2, 0));

	rot_circle = CreateCircleSensor(133, 252, 7);

	p2List_item<PhysBody*>* item = elements_100_p.getFirst();

	while (item != NULL)
	{
		item->data->listener = this;
		item = item->next;
	}
}

void ModulePhysics::CreateSensors()
{
	start_sensor = CreateRectangleSensor(365, 335, 7, 23);
	lose_sensor = CreateRectangleSensor(190, 630, 300, 50);

	l_impulse_sensor = CreateRectangleSensor(18, 366, 26, 26);

	int yellow_lever[16] = {
		189, 76,
		201, 75,
		214, 73,
		219, 72,
		223, 74,
		229, 74,
		233, 70,
		233, 63,
	};
	button_up_sensor = CreatePolygonSensor(0, 0, yellow_lever, 16);

	//top
	shiny_circles.add(CreateCircleSensor(360, 138, 4));
	shiny_circles.add(CreateCircleSensor(350, 103, 4));
	shiny_circles.add(CreateCircleSensor(330, 77, 4));
	shiny_circles.add(CreateCircleSensor(305, 54, 4));

	//bot left
	shiny_circles.add(CreateCircleSensor(58, 275, 4));
	shiny_circles.add(CreateCircleSensor(80, 273, 4));
	shiny_circles.add(CreateCircleSensor(96, 284, 4));
	shiny_circles.add(CreateCircleSensor(111, 301, 4));

	//bot right
	shiny_circles.add(CreateCircleSensor(223, 359, 4));
	shiny_circles.add(CreateCircleSensor(241, 348, 4));
	shiny_circles.add(CreateCircleSensor(264, 350, 4));
	shiny_circles.add(CreateCircleSensor(275, 369, 4));

}

void ModulePhysics::CreateLevers()
{
	int top_left_lever[12] = {
		31, 184,
		41, 185,
		69, 214,
		66, 218,
		28, 200,
		25, 191
	};
	int top_right_lever[12] = {
		311, 281,
		274, 302,
		276, 305,
		315, 299,
		322, 294,
		321, 284
	}; 
	int bot_left_lever[16] = {
		139, 521,
		179, 515,
		180, 512,
		178, 509,
		140, 502,
		132, 506,
		132, 516,
		137, 520
	};
	int bot_right_lever[12] = {
		240, 502,
		199, 509,
		198, 515,
		240, 522,
		248, 517,
		249, 508
	};

	levers.add(CreatePolygon(0, 0, top_left_lever, 12));
	levers.add(CreatePolygon(0, 0, top_right_lever, 12));
	levers.add(CreatePolygon(0, 0, bot_left_lever, 16));
	levers.add(CreatePolygon(0, 0, bot_right_lever, 12));
}

void ModulePhysics::CreateScrewers()
{
	screwers.add(CreateStaticCircle(34, 193, 1, 0));
	screwers.add(CreateStaticCircle(312, 290, 1, 0));
	screwers.add(CreateStaticCircle(139, 510, 1, 0));
	screwers.add(CreateStaticCircle(239, 510, 1, 0));
}

void ModulePhysics::CreateLeverForceMakers()
{
	force_makers.add(CreateCircle(62, 212, 1));
	force_makers.add(CreateCircle(281, 302, 1));
	force_makers.add(CreateCircle(177, 512, 1));
	force_makers.add(CreateCircle(205, 512, 1));
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureB()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	if (lose_sensed == true)
	{
		ball->body->GetWorld()->DestroyBody(ball->body);
		ball = CreateCircle(365, 325, 6);
		ball->body->IsBullet();
		ball->listener = this;
		lose_sensed = false;
		if (App->scene_intro->record_score < App->scene_intro->score)
		{
			App->scene_intro->record_score = App->scene_intro->score;
		}
		bonus_time += 30000; //ends the bonus points as soon as the new game starts
		lives--;
		if (lives == 0)
		{
			App->scene_intro->score = 0;
			lives = 4;
			game_end = true;
		}
	}

	if (button_pressed_sensed == true)
	{
		button->body->GetWorld()->DestroyBody(button->body);
		button = nullptr;
		button_pressed->body->GetWorld()->DestroyBody(button_pressed->body);
		button_pressed = nullptr;
		bonus_time = GetTickCount();
		App->scene_intro->score += 500 * bonus_score;
		button_pressed_sensed = false;
		button_up_sensed = false;
	}

	if (rot_ball == true)
	{
		if (joint_created == false)
		{
			joint_created = CreateTemporaryJoint();
		}
		else
		{
			App->scene_intro->score += 1;
		}
		if (GetTickCount() - rot_time > 3000)
		{
			DeleteTemporaryJoint();
			joint_created = false;
			rot_ball = false;
			int x, y;
			x = rand() % 100 - 50;
			y = rand() % 100 - 50;
			b2Vec2 force(x, y);
			ball->body->ApplyForceToCenter(force, true);
			App->scene_intro->twister_end = true;
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModulePhysics::Update()
{
	//IMPULSING THE BALL
	static float force = 10;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (hammer_down == false)
		{
			hammer_down = true;
		}
		if (start_sensed == true)
		{
			if (force < 100)
			{
				force += 10;
			}
			
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		
		ball->body->ApplyForceToCenter(b2Vec2(0, -force), true);
		force = 0;
		start_sensed = false;
		hammer_up = true;
	}

	if (button_up_sensed == true)
	{
		//TODO: button animation
		if (button == nullptr)
		{
			int button_points[16] = {
				240, 149,
				281, 147,
				282, 159,
				276, 159,
				275, 153,
				247, 154,
				247, 160,
				240, 160
			};
			button = CreateStaticRestPolygon(0, 0, button_points, 16, 0);

			int button_press_points[8] = {
				253, 165,
				249, 159,
				272, 157,
				270, 164
			};
			button_pressed = CreatePolygonSensor(0, 0, button_press_points, 8);
		}
	}

	if (l_impulse_sensed == true)
	{

		if (GetTickCount() - start_time > 2000)
		{
			ball->body->ApplyForceToCenter(b2Vec2(0, -100), true);
			l_impulse_sensed = false;
		}
	}


	if (GetTickCount() - bonus_time < 30000)
	{
		bonus_score = 2;
	}
	else
	{
		bonus_score = 1;
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 2.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateStaticCircle(int x, int y, int radius, float res)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	
	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = res;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType type)
{
	b2BodyDef body;
	if (type == b2_dynamicBody)
	{
		body.type = b2_dynamicBody;
	}
	else if (type == b2_staticBody)
	{
		body.type = b2_staticBody;
	}
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 25.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreatePolygonSensor(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape shape;
	b2Vec2* vert = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		vert[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		vert[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.Set(vert, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody * ModulePhysics::CreateCircleSensor(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, float res)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.restitution = res;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody* ModulePhysics::CreatePolygon(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape shape;
	b2Vec2* vert = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		vert[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		vert[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.Set(vert, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	//fixture.restitution = res;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody* ModulePhysics::CreateStaticRestPolygon(int x, int y, int* points, int size, float res)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape shape;
	b2Vec2* vert = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		vert[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		vert[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.Set(vert, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.restitution = res;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

void ModulePhysics::CreateRevoutionJoints()
{
	p2List_item<PhysBody*>* l = levers.getFirst();
	p2List_item<PhysBody*>* s = screwers.getFirst();
	p2List_item<PhysBody*>* f = force_makers.getFirst();
	p2List_item<PhysBody*>* r_w = red_wheels.getFirst();

	b2RevoluteJointDef red_w_l_def;
	red_w_l_def.bodyA = r_w->data->body;
	red_w_l_def.bodyB = s->data->body;
	red_w_l_def.collideConnected = false;
	red_w_l_def.localAnchorA.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	red_w_l_def.localAnchorB.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	red_w_l_def.referenceAngle = 0;
	red_w_l_def.enableLimit = false;
	red_w_l_def.enableMotor = true;
	red_w_l_def.maxMotorTorque = 1;
	red_w_l_def.motorSpeed = 90 * DEGTORAD;
	b2RevoluteJoint* rev_joint_red_w_l = (b2RevoluteJoint*)world->CreateJoint(&red_w_l_def);
	
	s = s->next;
	r_w = r_w->next;

	b2RevoluteJointDef red_w_r_def;
	red_w_r_def.bodyA = r_w->data->body;
	red_w_r_def.bodyB = s->data->body;
	red_w_r_def.collideConnected = false;
	red_w_r_def.localAnchorA.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	red_w_r_def.localAnchorB.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	red_w_r_def.enableLimit = false;
	red_w_r_def.enableMotor = true;
	red_w_r_def.maxMotorTorque = 1;
	red_w_r_def.motorSpeed = -90 * DEGTORAD;
	b2RevoluteJoint* rev_joint_red_w_r = (b2RevoluteJoint*)world->CreateJoint(&red_w_r_def);

	s = s->next;

	//stick top left
	b2RevoluteJointDef t_l_joint_def;
	t_l_joint_def.bodyA = l->data->body;
	t_l_joint_def.bodyB = s->data->body;
	t_l_joint_def.collideConnected = false;
	t_l_joint_def.localAnchorA.Set(PIXEL_TO_METERS(34), PIXEL_TO_METERS(193));
	t_l_joint_def.localAnchorB.Set(0, 0);
	t_l_joint_def.enableLimit = true;
	t_l_joint_def.lowerAngle = 0 * DEGTORAD;
	t_l_joint_def.upperAngle = 45 * DEGTORAD;
	b2RevoluteJoint* rev_joint_t_l = (b2RevoluteJoint*)world->CreateJoint(&t_l_joint_def);

	//force
	b2RevoluteJointDef t_l_f_joint_def;
	t_l_f_joint_def.bodyA = l->data->body;
	t_l_f_joint_def.bodyB = f->data->body;
	t_l_f_joint_def.collideConnected = false;
	t_l_f_joint_def.localAnchorA.Set(PIXEL_TO_METERS(62), PIXEL_TO_METERS(212));
	t_l_f_joint_def.localAnchorB.Set(0, 0);
	b2RevoluteJoint* rev_joint_t_l_f = (b2RevoluteJoint*)world->CreateJoint(&t_l_f_joint_def);

	l = l->next;
	s = s->next;
	f = f->next;

	//stick top right
	b2RevoluteJointDef t_r_joint_def;
	t_r_joint_def.bodyA = l->data->body;
	t_r_joint_def.bodyB = s->data->body;
	t_r_joint_def.collideConnected = false;
	t_r_joint_def.localAnchorA.Set(PIXEL_TO_METERS(312), PIXEL_TO_METERS(290));
	t_r_joint_def.localAnchorB.Set(0, 0);
	t_r_joint_def.enableLimit = true;
	t_r_joint_def.lowerAngle = -25 * DEGTORAD;
	t_r_joint_def.upperAngle = 20 * DEGTORAD;
	b2RevoluteJoint* rev_joint_t_r = (b2RevoluteJoint*)world->CreateJoint(&t_r_joint_def);

	//force
	b2RevoluteJointDef t_r_f_joint_def;
	t_r_f_joint_def.bodyA = l->data->body;
	t_r_f_joint_def.bodyB = f->data->body;
	t_r_f_joint_def.collideConnected = false;
	t_r_f_joint_def.localAnchorA.Set(PIXEL_TO_METERS(281), PIXEL_TO_METERS(302));
	t_r_f_joint_def.localAnchorB.Set(0, 0);
	b2RevoluteJoint* rev_joint_t_r_f = (b2RevoluteJoint*)world->CreateJoint(&t_r_f_joint_def);

	l = l->next;
	s = s->next;
	f = f->next;

	//stick bot left
	b2RevoluteJointDef b_l_joint_def;
	b_l_joint_def.bodyA = l->data->body;
	b_l_joint_def.bodyB = s->data->body;
	b_l_joint_def.collideConnected = false;
	b_l_joint_def.localAnchorA.Set(PIXEL_TO_METERS(139), PIXEL_TO_METERS(510));
	b_l_joint_def.localAnchorB.Set(0, 0);
	b_l_joint_def.enableLimit = true;
	b_l_joint_def.lowerAngle = 0 * DEGTORAD;
	b_l_joint_def.upperAngle = 45 * DEGTORAD;
	b2RevoluteJoint* rev_joint_b_l = (b2RevoluteJoint*)world->CreateJoint(&b_l_joint_def);

	//force
	b2RevoluteJointDef b_l_f_joint_def;
	b_l_f_joint_def.bodyA = l->data->body;
	b_l_f_joint_def.bodyB = f->data->body;
	b_l_f_joint_def.collideConnected = false;
	b_l_f_joint_def.localAnchorA.Set(PIXEL_TO_METERS(177), PIXEL_TO_METERS(512));
	b_l_f_joint_def.localAnchorB.Set(0, 0);
	b2RevoluteJoint* rev_joint_b_l_f = (b2RevoluteJoint*)world->CreateJoint(&b_l_f_joint_def);

	l = l->next;
	s = s->next;
	f = f->next;

	//stick bot right
	b2RevoluteJointDef b_r_joint_def;
	b_r_joint_def.bodyA = l->data->body;
	b_r_joint_def.bodyB = s->data->body;
	b_r_joint_def.collideConnected = false;
	b_r_joint_def.localAnchorA.Set(PIXEL_TO_METERS(239), PIXEL_TO_METERS(510));
	b_r_joint_def.localAnchorB.Set(0, 0);
	b_r_joint_def.enableLimit = true;
	b_r_joint_def.lowerAngle = -45 * DEGTORAD;
	b_r_joint_def.upperAngle = 0 * DEGTORAD;
	b2RevoluteJoint* rev_joint_b_r = (b2RevoluteJoint*)world->CreateJoint(&b_r_joint_def);

	//force
	b2RevoluteJointDef b_r_f_joint_def;
	b_r_f_joint_def.bodyA = l->data->body;
	b_r_f_joint_def.bodyB = f->data->body;
	b_r_f_joint_def.collideConnected = false;
	b_r_f_joint_def.localAnchorA.Set(PIXEL_TO_METERS(205), PIXEL_TO_METERS(512));
	b_r_f_joint_def.localAnchorB.Set(0, 0);
	b2RevoluteJoint* rev_joint_b_r_f = (b2RevoluteJoint*)world->CreateJoint(&b_r_f_joint_def);
}

bool ModulePhysics::CreateTemporaryJoint()
{
	b2RevoluteJointDef temp_joint_def;
	temp_joint_def.bodyA = ball->body;
	temp_joint_def.bodyB = rot_circle->body;
	temp_joint_def.collideConnected = false;
	temp_joint_def.localAnchorA.Set(0, 0);
	temp_joint_def.localAnchorB.Set(0, 0);
	temp_rev_joint = (b2RevoluteJoint*)world->CreateJoint(&temp_joint_def);
	return true;
}

void ModulePhysics::DeleteTemporaryJoint()
{
	world->DestroyJoint(temp_rev_joint);
	temp_rev_joint = nullptr;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		bool obj_ins_mouse = false;
		b2Body* b_mouse = b;
		b2Vec2 mouse_pos;
		mouse_pos.x = PIXEL_TO_METERS(App->input->GetMouseX());
		mouse_pos.y = PIXEL_TO_METERS(App->input->GetMouseY());

		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();
				App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->GetVertex(0));
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
			}
			break;
			}

			if (obj_ins_mouse == false)
			{
				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
				{
					mouse_pos.x = PIXEL_TO_METERS(App->input->GetMouseX());
					mouse_pos.y = PIXEL_TO_METERS(App->input->GetMouseY());
					if (f->TestPoint(mouse_pos) && f->GetType() == b2Shape::e_circle)
					{
						obj_ins_mouse = true;
						break;
					}
				}
			}
		}
		b2MouseJointDef def;
		if (obj_ins_mouse == true)
		{
			
			def.bodyA = ground;
			def.bodyB = b_mouse;
			def.target = mouse_pos;
			def.dampingRatio = 0.5f;
			def.frequencyHz = 2.0f;
			def.maxForce = 100.0f * b_mouse->GetMass();			

			mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
		}		

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint)
		{
			mouse_joint->SetTarget(mouse_pos);
			App->renderer->DrawLine(METERS_TO_PIXELS(mouse_pos.x), METERS_TO_PIXELS(mouse_pos.y), METERS_TO_PIXELS(mouse_joint->GetAnchorB().x), METERS_TO_PIXELS(mouse_joint->GetAnchorB().y), 255, 100, 100);
		}

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint)
		{
			obj_ins_mouse = false;
			world->DestroyJoint(mouse_joint);
			mouse_joint = nullptr;
		}
	}
	
	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}


void ModulePhysics::OnCollision(PhysBody * bodyA, PhysBody * bodyB)
{
	int x, y;

	//App->audio->PlayFx(bonus_fx);

	if (bodyA)
	{
		//Lose
		if (bodyB == lose_sensor)
		{
			//App->audio->PlayFx(bonus_fx);
			lose_sensed = true;
		}

		//Ball is in start position
		else if (bodyB == start_sensor)
		{
			start_sensed = true;
		}

		//Elements that gave 100 points
		else if (elements_100_p.find(bodyB) != -1)
		{
			//TODO Add Sound
			App->scene_intro->score += 100 * bonus_score;
			int i = elements_100_p.find(bodyB);
			int x, y;
			bodyB->GetPosition(x, y);
			if (i == 0 || i == 1)
			{
				//pink (blue doesn't have anim)
				App->scene_intro->shiny_pink_ball = true;
				App->scene_intro->pos.x = x;
				App->scene_intro->pos.y = y;
			}
			App->audio->PlayFx(App->scene_intro->pink_blue_fx);
		}

		//Lever that pulls the button for double points up
		else if (bodyB == button_up_sensor && button_up_sensed == true)
		{
			App->scene_intro->yellow_lever_animation.PushBack({ 0, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.PushBack({ 56, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.PushBack({ 116, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.PushBack({ 175, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.loop = false;
			App->scene_intro->yellow_lever_animation.speed = 0.4f;

			App->scene_intro->yellow_lever_animation.PushBack({ 175, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.PushBack({ 116, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.PushBack({ 56, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.PushBack({ 0, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.loop = false;
			App->scene_intro->yellow_lever_animation.speed = 0.4f;
		}

		else if (bodyB == button_up_sensor && button_up_sensed == false)
		{
			button_up_sensed = true;

			App->scene_intro->yellow_lever_animation.PushBack({ 0, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.PushBack({ 56, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.PushBack({ 116, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.PushBack({ 175, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.loop = false;
			App->scene_intro->yellow_lever_animation.speed = 0.4f;

			App->scene_intro->button.PushBack({ 0, 0, 51, 77 });
			App->scene_intro->button.PushBack({ 51, 0, 51, 77 });
			App->scene_intro->button.PushBack({ 111, 0, 51, 77 });
			App->scene_intro->button.PushBack({ 180, 0, 51, 77 });
			App->scene_intro->button.PushBack({ 251, 0, 51, 77 });
			App->scene_intro->button.PushBack({ 319, 0, 51, 77 });
			App->scene_intro->button.PushBack({ 393, 0, 51, 77 });
			App->scene_intro->button.PushBack({ 457, 0, 51, 77 });
			App->scene_intro->button.PushBack({ 524, 0, 51, 77 });
			App->scene_intro->button.PushBack({ 586, 0, 51, 77 });
			App->scene_intro->button.PushBack({ 654, 0, 51, 77 });
			App->scene_intro->button.PushBack({ 720, 0, 51, 77 });
			App->scene_intro->button.PushBack({ 788, 0, 51, 77 });
			App->scene_intro->button.loop = false;
			App->scene_intro->button.speed = 0.4f;

			App->scene_intro->yellow_lever_animation.PushBack({ 175, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.PushBack({ 116, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.PushBack({ 56, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.PushBack({ 0, 0, 57, 19 });
			App->scene_intro->yellow_lever_animation.loop = false;
			App->scene_intro->yellow_lever_animation.speed = 0.4f;
		}
		

		//Left side of the map impulser
		else if (bodyB == l_impulse_sensor)
		{
			l_impulse_sensed = true;
			start_time = GetTickCount();
		}

		//Shiny circles around the map
		else if (shiny_circles.find(bodyB) != -1)
		{
			int x, y;
			bodyB->GetPosition(x, y);
			App->scene_intro->score += 25 * bonus_score;
			App->scene_intro->shiny_little_ball = true;
			App->scene_intro->anim_time = GetTickCount();
			App->scene_intro->pos.x = x;
			App->scene_intro->pos.y = y;
		}

		//Button pressed
		if (button_pressed != nullptr)
		{
			if (bodyB == button_pressed)
			{
				button_pressed_sensed = true;
				App->scene_intro->button.PushBack({ 788, 0, 51, 77 });
				App->scene_intro->button.PushBack({ 720, 0, 51, 77 });
				App->scene_intro->button.PushBack({ 654, 0, 51, 77 });
				App->scene_intro->button.PushBack({ 586, 0, 51, 77 });
				App->scene_intro->button.PushBack({ 524, 0, 51, 77 });
				App->scene_intro->button.PushBack({ 457, 0, 51, 77 });
				App->scene_intro->button.PushBack({ 393, 0, 51, 77 });
				App->scene_intro->button.PushBack({ 319, 0, 51, 77 });
				App->scene_intro->button.PushBack({ 251, 0, 51, 77 });
				App->scene_intro->button.PushBack({ 180, 0, 51, 77 });
				App->scene_intro->button.PushBack({ 111, 0, 51, 77 });
				App->scene_intro->button.PushBack({ 51, 0, 51, 77 });
				App->scene_intro->button.PushBack({ 0, 0, 51, 77 });
				App->scene_intro->button.loop = false;
				App->scene_intro->button.speed = 0.5f;
			}
		}

		//Rotation circle
		if (bodyB == rot_circle && rot_ball == false && GetTickCount() - rot_time > 4000)
		{
			rot_time = GetTickCount();
			rot_ball = true;
			App->scene_intro->twister_b = true;
		}

	}
}
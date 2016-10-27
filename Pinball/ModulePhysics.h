#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateStaticCircle(int x, int y, int radius, float res);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType type);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreatePolygonSensor(int x, int y, int* points, int size);
	PhysBody* CreateCircleSensor(int x, int y, int radius);
	PhysBody* CreateChain(int x, int y, int* points, int size, float res);
	PhysBody* CreatePolygon(int x, int y, int* points, int size);
	PhysBody* CreateStaticRestPolygon(int x, int y, int* points, int size, float res);

	void CreateRevoutionJoints();

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	PhysBody* ball;
	p2List<PhysBody*> map_bodies;

	p2List<PhysBody*> levers;
	PhysBody* start_sensor;
	bool start_sensed;
	PhysBody* lose_sensor;
	bool lose_sensed;
	PhysBody* button_up_sensor;
	bool button_up_sensed;
	PhysBody* l_impulse_sensor;
	bool l_impulse_sensed;
	p2List<PhysBody*> shiny_circles;
	p2List<bool> shiny_circles_sensed;
	bool hammer_down = false;
	bool hammer_up = false;

	p2List<PhysBody*> force_makers;
	p2List<PhysBody*> red_wheels;
	bool game_end;

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;

	void CreateMap();
	void CreateSensors();
	void CreateLevers();
	void CreateScrewers();
	void CreateLeverForceMakers();

	p2List<PhysBody*> elements_100_p;
	p2List<PhysBody*> restit_bodies;
	p2List<PhysBody*> screwers;

	PhysBody* button;
	PhysBody* button_pressed;
	bool button_pressed_sensed;
	
	PhysBody* rot_circle;
	uint rot_time;
	bool rot_ball;
	b2RevoluteJoint* temp_rev_joint;
	bool joint_created;
	bool CreateTemporaryJoint();
	void DeleteTemporaryJoint();
	
	uint start_time;

	uint bonus_time;
	uint bonus_score;

	uint lives;
};
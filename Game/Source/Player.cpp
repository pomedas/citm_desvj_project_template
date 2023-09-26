#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: TODO 2: Initialize Player parameters
	position = iPoint(0, 0);

	return true;
}

bool Player::Start() {

	texture = app->tex->Load("Assets/Textures/player1.png");
	return true;
}

bool Player::Update(float dt)
{
	//L03: TODO 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		position.y -= (int) (speed * dt);

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		position.y += (int)(speed * dt);

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		position.x -= (int)(speed * dt);

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		position.x += (int)(speed * dt);

	app->render->DrawTexture(texture,position.x,position.y);

	return true;
}

bool Player::CleanUp()
{

	return true;
}
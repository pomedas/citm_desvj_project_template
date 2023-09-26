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

	return true;
}

bool Player::Start() {

	return true;
}

bool Player::Update()
{
	//L03: TODO 4: render the player texture and modify the position of the player using WSAD keys and render the texture

	return true;
}

bool Player::CleanUp()
{

	return true;
}
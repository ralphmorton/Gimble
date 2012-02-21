
#include "Game/GimbleEngine.h"

#include "PhysicsTestApp.h"

int main(int argc, char *argv)
{
	Gimble::Game::Engine engine;
	engine.run("gimble.cfg", new PhysicsTestApp());
}
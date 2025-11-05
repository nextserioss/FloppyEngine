#pragma once

class Composite;
class GameEngine;

class Component
{
public:
	virtual void Add(GameEngine* gameEngine, Composite* composite) = 0;
	virtual void Update(GameEngine* gameEngine) = 0;
	virtual void CleanUp(GameEngine* gameEngine) = 0;
	virtual void Draw(GameEngine* gameEngine) = 0;
};
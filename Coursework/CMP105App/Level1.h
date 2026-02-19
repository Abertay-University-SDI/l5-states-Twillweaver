#pragma once

#include "Framework/BaseLevel.h"
#include "Framework/GameObject.h"
#include "Framework/TileMap.h"
#include "Player.h"
#include "Framework/Collision.h"

class Level1 : public BaseLevel {
public:
	Level1(sf::RenderWindow& window, Input& input, GameState& gameState);
	~Level1() override = default;

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	// Default functions for rendering to the screen.

	// Default variables for level class.
	TileMap m_tileMap;

	Player m_player;

	// Interactable objects
	GameObject m_flag;
	GameObject m_switch;

	// Tile texture used for interactables
	sf::Texture m_tileTexture;
};
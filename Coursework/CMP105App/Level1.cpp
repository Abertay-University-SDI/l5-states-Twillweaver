#include "Level1.h"
#include <set>

Level1::Level1(sf::RenderWindow& hwnd, Input& in, GameState& gs) :
	BaseLevel(hwnd, in, gs)
{
	const int tile_size = 18;
	const int num_cols = 20;
	const int num_rows = 9;
	const int sheet_spacing = 1;

	std::vector<GameObject> tileSet;

	// Tiles that are solid/collidable
	const std::set<int> solidTiles = { 1, 3, 20, 21, 22, 23, 81, 83, 104, 142 };

	// Helper lambda to create a tile
	auto makeTile = [&](int index) {
		GameObject t;
		t.setSize({ tile_size * 5.f, tile_size * 5.f });
		t.setCollisionBox({ {0,0}, t.getSize() });

		int row = index / num_cols;
		int col = index % num_cols;

		t.setTextureRect({
			{ (tile_size + sheet_spacing) * col, (tile_size + sheet_spacing) * row },
			{ tile_size, tile_size }
			});

		t.setCollider(solidTiles.count(index) > 0);
		return t;
		};

	// Build tileset for the level
	for (int i = 0; i < num_cols * num_rows; i++)
	{
		tileSet.push_back(makeTile(i));
	}

	// Add a blank tile for “empty” spaces
	GameObject blankTile;
	blankTile.setSize({ tile_size * 5.f, tile_size * 5.f });
	blankTile.setCollisionBox({ {0,0}, blankTile.getSize() });
	blankTile.setTextureRect({ {0,0}, {-tile_size, -tile_size} });
	blankTile.setCollider(false);
	const int blankIndex = static_cast<int>(tileSet.size());
	tileSet.push_back(blankTile);

	// Define tile placement in the level
	std::vector<int> tileMapLocations{
		blankIndex, blankIndex, 20, blankIndex, 66, blankIndex, blankIndex, blankIndex, blankIndex, 112,
		blankIndex, 21, 104, 22, 22, 23, blankIndex, blankIndex, blankIndex, 131,
		1, 142, 142, 142, 142, 142, 3, blankIndex, 81, 83
	};

	sf::Vector2u mapSize = { 10, 3 };

	// Set up the tile map
	m_tileMap.setPosition({ 0.f, 200.f });
	m_tileMap.loadTexture("gfx/tilemap.png");
	m_tileMap.setTileMap(tileMapLocations, mapSize);
	m_tileMap.setTileSet(tileSet);
	m_tileMap.buildLevel();

	// Link input to player
	m_player.setInput(&m_input);
}


// OLD CODE:

//Level1::Level1(sf::RenderWindow& hwnd, Input& in, GameState& gs) :
//	BaseLevel(hwnd, in, gs)
//{
//	int tile_size = 18;
//	int num_cols = 20;
//	int num_rows = 9;
//	int sheet_spacing = 1;
//	GameObject tile;
//	tile.setSize(sf::Vector2f( tile_size*5, tile_size*5 ));
//	tile.setCollisionBox({ { 0,0 },tile.getSize() });
//	std::vector<GameObject> tileSet;

//	const std::set<int> solidTiles = {
//	1, 3,
//	20, 21, 22, 23,
//	81, 83,
//	104,
//	142
//	};

//	for (int i = 0; i < num_cols * num_rows; i++)
//	{
//		int row = i / num_cols;
//		int col = i % num_cols;

//		tile.setTextureRect({
//			{(tile_size + sheet_spacing) * col,
//			 (tile_size + sheet_spacing) * row},
//			{tile_size, tile_size}
//			});

//		tile.setCollider(solidTiles.count(i));

//		tileSet.push_back(tile);
//	}

	// Add Blank
//	tile.setTextureRect({ {0, 0}, {-tile_size, -tile_size} });
//	int b = tileSet.size();
//	tile.setCollider(false);
//	tileSet.push_back(tile);

//	std::vector<int> tileMapLocations{
//		b,	b,	20,	b,	66,	b,	b,	b,	b,	112,
//		b,	21, 104,22,	22,23,	b,	b,	b,	131,
//		1,	142,142,142,142,142,3,	b,	81,	83
//	};
//	sf::Vector2u mapSize = { 10, 3 };

//	m_tileMap.setPosition({ 0,200 });
//	m_tileMap.loadTexture("gfx/tilemap.png");
//	m_tileMap.setTileMap(tileMapLocations, mapSize);
//	m_tileMap.setTileSet(tileSet);
//	m_tileMap.buildLevel();

//	m_player.setInput(&m_input);
//}

// handle user input
void Level1::handleInput(float dt)
{
	if (m_input.isLeftMousePressed())
	{
		std::cout << "left mouse pressed" << std::endl;
	}

	m_player.handleInput(dt);
}

// Update game objects
void Level1::update(float dt)
{
	m_player.update(dt);

// Grab tiles as a vector
	std::vector<GameObject>& tiles = *m_tileMap.getLevel();

	for (auto& tile : tiles)
	{
 // Only check if the tile is a collider
		if (tile.isCollider())
		{
			sf::FloatRect playerCollider = m_player.getCollisionBox();
			sf::FloatRect tileBounds = tile.getCollisionBox();
// Check if their collision boxes are overlapping
			auto overlap = playerCollider.findIntersection(tileBounds);
			if (overlap)
			{
				m_player.collisionResponse(tile);
			}
		}
	}
	
}

// Render Level1
void Level1::render()
{
	beginDraw();
	m_tileMap.render(m_window);
	m_window.draw(m_player);
	endDraw();
}


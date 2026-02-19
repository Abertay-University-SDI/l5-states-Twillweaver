#include "Player.h"
#include <iostream>

Player::Player()
{
	if (!m_texture.loadFromFile("gfx/dino1.png"))
		std::cerr << "No dino texture. sad";

	setTexture(&m_texture);
	setSize({ 36,36 });
	setCollisionBox({ {6,6}, { 24,25 } });
	setPosition({ 50,0 });
	m_isOnGround = false;
	setTextureRect({ { 0,0 }, { 24,24 } });
}

void Player::handleInput(float dt)
{
    m_velocity.x = 0;   // Reset horizontal velocity every frame

    if (m_input->isKeyDown(sf::Keyboard::Scancode::A))
        m_velocity.x = -SPEED;

    if (m_input->isKeyDown(sf::Keyboard::Scancode::D))
        m_velocity.x = SPEED;

    if (m_input->isKeyDown(sf::Keyboard::Scancode::R))
    {
        setPosition({ 50,0 });
        m_velocity = { 0,0 };
    }
}


void Player::update(float dt)
{
    m_isOnGround = false; // Reset ground state

    // Set acceleration to a constant value, do not use +=
    m_acceleration.y = GRAVITY;

    // Velocity = Acceleration * Time
    m_velocity.y += m_acceleration.y * dt;

    // Move the sprite
    move(m_velocity * dt);
}

void Player::collisionResponse(GameObject& collider)
{
    // 1. Define the player's collision box
    sf::FloatRect playerCollider = getCollisionBox();

    // 2. Define the tile's (wall) collision box
    sf::FloatRect wallBounds = collider.getCollisionBox();

    // 3. Check for the intersection
    auto overlap = playerCollider.findIntersection(wallBounds);

    // 4. If there is no overlap, then leave
    if (!overlap) return;

    // If we are moving downward, handle the floor
    if (m_velocity.y > 0)
    {
        // Stop the downward velocity
        if (m_velocity.y > 0) // falling
        {
            m_velocity.y = 0;

            float newY = wallBounds.position.y
                - m_collisionBox.size.y
                - m_collisionBox.position.y;

            setPosition({ getPosition().x, newY });

            m_isOnGround = true;
        }
    }
}

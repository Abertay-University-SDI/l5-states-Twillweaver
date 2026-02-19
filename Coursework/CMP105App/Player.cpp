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

    else if (m_input->isKeyDown(sf::Keyboard::Scancode::D))
        m_velocity.x = SPEED;

    // Jump (only if grounded)
    if ((m_input->isKeyDown(sf::Keyboard::Scancode::W) ||
        m_input->isKeyDown(sf::Keyboard::Scancode::Space))
        && m_isOnGround)
    {
        m_velocity.y = -JUMP_FORCE;   // negative = upward
        m_isOnGround = false;
    }

    if (m_input->isKeyDown(sf::Keyboard::Scancode::R))
    {
        setPosition({ 50,0 });
        m_velocity = { 0,0 };
        m_isOnGround = false;
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
    sf::FloatRect playerCollider = getCollisionBox();
    sf::FloatRect tileBounds = collider.getCollisionBox();

    auto overlap = playerCollider.findIntersection(tileBounds);
    if (!overlap) return;

    // If overlap is wider than tall = vertical collision
    if (overlap->size.x > overlap->size.y)
    {
        // -------- TOP COLLISION (falling) --------
        if (m_velocity.y > 0)
        {
            float newY = tileBounds.position.y
                - m_collisionBox.size.y
                - m_collisionBox.position.y;

            setPosition({ getPosition().x, newY });

            m_velocity.y = 0;
            m_isOnGround = true;
        }
    }
    else
    {
        // -------- SIDE COLLISION --------

        // If player is to left of tile
        if (playerCollider.position.x < tileBounds.position.x)
        {
            setPosition({
                tileBounds.position.x
                - m_collisionBox.size.x
                - m_collisionBox.position.x,
                getPosition().y
                });
        }
        else
        {
            // Player is to right of tile
            setPosition({
                tileBounds.position.x
                + tileBounds.size.x
                - m_collisionBox.position.x,
                getPosition().y
                });
        }

        // Reverse x velocity with restitution
        m_velocity.x *= -COEFF_RESTITUTION;
    }
}


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
    m_oldPosition = getPosition();

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

    // ---------------- FLOOR COLLISION ----------------
    float playerBottomLastFrame = m_oldPosition.y + m_collisionBox.position.y + m_collisionBox.size.y;
    float tileTop = tileBounds.position.y;

    sf::Vector2f playerLeftLastFrame = m_oldPosition + m_collisionBox.position;
    sf::Vector2f playerRightLastFrame = playerLeftLastFrame + sf::Vector2f(m_collisionBox.size.x, m_collisionBox.size.y);


    if (playerBottomLastFrame <= tileTop && m_velocity.y >= 0.f)
    {
        // Player was above tile last frame -> floor
        float newY = tileTop - m_collisionBox.position.y - m_collisionBox.size.y;
        setPosition({ getPosition().x, newY });

        m_velocity.y = 0.f;
        m_isOnGround = true;
    }
    else
    {
        // ---------------- SIDE COLLISION ----------------
        float overlapX = overlap->size.x;

        // Calculate edges
        float playerLeftLast = m_oldPosition.x + m_collisionBox.position.x;
        float playerRightLast = m_oldPosition.x + m_collisionBox.position.x + m_collisionBox.size.x;
        float tileLeft = tileBounds.position.x;
        float tileRight = tileBounds.position.x + tileBounds.size.x;

        // Did we come from left?
        if (playerRightLast <= tileLeft)
        {
            // Player hit tile from left
            float newX = tileLeft - m_collisionBox.position.x - m_collisionBox.size.x;
            setPosition({ newX, getPosition().y });
        }
        else if (playerLeftLast >= tileRight)
        {
            // Player hit tile from right
            float newX = tileRight - m_collisionBox.position.x;
            setPosition({ newX, getPosition().y });
        }

        // Reverse X velocity
        m_velocity.x *= -COEFF_RESTITUTION;
    }
}

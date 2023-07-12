#include "include/SFML/Graphics.hpp"
#include "include/box2d/box2d.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
 
// Pixels per meter. Box2D uses metric units, so we need to define a conversion
#define PPM 30.0F
// SFML uses degrees for angles while Box2D uses radians
#define DEG_PER_RAD 57.2957795F

// Box2D world for physics simulation, gravity = 9 m/s^2
b2World world(b2Vec2(0, -9));

// A structure with all we need to render a box
struct Box
{
    float width;
    float height;
    sf::Color color;
    b2Body* body;
};

// A structure with all we need to render a circle
struct Circle
{
    float radius;
    sf::Color color;
    b2Body* body;
};

Box createBox(float x, float y, float width, float height, float density, float friction, sf::Color color)
{
    // Body definition
    b2BodyDef boxBodyDef;
    boxBodyDef.position.Set(x / PPM, y / PPM);
    boxBodyDef.type = b2_dynamicBody;

    // Shape definition
    b2PolygonShape boxShape;
    boxShape.SetAsBox(width / 2 / PPM, height / 2 / PPM);

    // Fixture definition
    b2FixtureDef fixtureDef;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.shape = &boxShape;

    // Now we have a body for our Box object
    b2Body* boxBody = world.CreateBody(&boxBodyDef);
    // Lastly, assign the fixture
    boxBody->CreateFixture(&fixtureDef);

    return Box{ width, height, color, boxBody };
}

Circle createCircle(float x, float y, float radius, float density, float friction, sf::Color color)
{
    // Body definition
    b2BodyDef circleBodyDef;
    circleBodyDef.position.Set(x / PPM, y / PPM);
    circleBodyDef.type = b2_dynamicBody;

    // Shape definition
    b2CircleShape circleShape;
    circleShape.m_radius = radius / PPM;

    // Fixture definition
    b2FixtureDef fixtureDef;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.shape = &circleShape;

    // Now we have a body for our Circle object
    b2Body* circleBody = world.CreateBody(&circleBodyDef);
    // Lastly, assign the fixture
    circleBody->CreateFixture(&fixtureDef);

    return Circle{ radius, color, circleBody };
}

Box createGround(float x, float y, float width, float height, float angle, sf::Color color)
{
    // Static body definition
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(x / PPM, y / PPM);
    groundBodyDef.angle = angle * b2_pi / 180.0f;

    // Shape definition
    b2PolygonShape groundBox;
    groundBox.SetAsBox(width / 2 / PPM, height / 2 / PPM);

    // Now we have a body for our Box object
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    // For a static body, we don't need a custom fixture definition, this will do:
    groundBody->CreateFixture(&groundBox, 0.0f);

    return Box{ width, height, color, groundBody };
}

void render(sf::RenderWindow& w, std::vector<Box>& boxes, std::vector<Circle>& circles)
{
    w.clear();
    for (const auto& box : boxes)
    {
        sf::RectangleShape rect;

        // For the correct Y coordinate of our drawable rect, we must subtract from WINDOW_HEIGHT
        // because SFML uses an OpenGL coordinate system where X is right, Y is down
        // while Box2D uses a traditional coordinate system where X is right, Y is up
        rect.setPosition(box.body->GetPosition().x * PPM, WINDOW_HEIGHT - (box.body->GetPosition().y * PPM));

        // We also need to set our drawable's origin to its center
        // because in SFML, "position" refers to the upper left corner
        // while in Box2D, "position" refers to the body's center
        rect.setOrigin(box.width / 2, box.height / 2);

        rect.setSize(sf::Vector2f(box.width, box.height));

        // For the rect to be rotated in the correct direction, we have to multiply by -1
        rect.setRotation(-1 * box.body->GetAngle() * DEG_PER_RAD);

        rect.setFillColor(box.color);
        w.draw(rect);
    }

    for (const auto& circle : circles)
    {
        sf::CircleShape shape;

        // For the correct Y coordinate of our drawable circle, we must subtract from WINDOW_HEIGHT
        // because SFML uses an OpenGL coordinate system where X is right, Y is down
        // while Box2D uses a traditional coordinate system where X is right, Y is up
        shape.setPosition(circle.body->GetPosition().x * PPM, WINDOW_HEIGHT - (circle.body->GetPosition().y * PPM));

        shape.setOrigin(circle.radius, circle.radius);
        shape.setRadius(circle.radius);
        shape.setFillColor(circle.color);
        w.draw(shape);
    }

    w.display();
}

int main()
{
    // Setup SFML window
    sf::RenderWindow w(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML + Box2D");
    w.setFramerateLimit(60);

    // Containers to hold all the boxes and circles we create
    std::vector<Box> boxes;
    std::vector<Circle> circles;

    // Generate ground
    float slopeAngle = 30.0f; // Angle of the slope in degrees
    float slopeWidth = 500.0f; // Width of the slope
    float slopeHeight = 100.0f; // Height of the slope
    float slopeX = 350.0f; // X position of the slope
    float slopeY = 200.0f; // Y position of the slope

    boxes.push_back(createGround(slopeX, slopeY, slopeWidth, slopeHeight, slopeAngle, sf::Color::Green));
  

    // Create a ball
    auto&& circle = createCircle(500, 500, 12, 1.f, 0.7f, sf::Color::White);
    circles.push_back(circle);

    /** GAME LOOP **/
    while (w.isOpen())
    {
        // Update the world, standard arguments
        world.Step(1 / 60.f, 6, 3);
        // Render everything
        render(w, boxes, circles);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
{
    // left key is pressed: move our character
       auto&& circle = createCircle(500, 500, 12, 1.f, 0.7f, sf::Color::White);
    circles.push_back(circle);

}
    }

    return 0;
}

#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

int main()
{
    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Text Example");

    // Create a font (you'll need to provide a font file)
    sf::Font font;
    if (!font.loadFromFile("style/Roboto-Medium.ttf"))
    {
        // Handle font loading error
        return 1;
    }

    // Create the text object
    sf::Text text;
    text.setFont(font);
    text.setString("Your Text Here"); // Replace with your desired text
    text.setCharacterSize(36); // Set the character size
    text.setFillColor(sf::Color::White); // Set the text color
    text.setStyle(sf::Text::Bold); // Set the text style (optional)

    // Position the text in the center of the screen
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.width / 2, textBounds.height / 2);
    text.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // Draw the text
        window.draw(text);

        window.display();
    }

    return 0;
}

#include <SFML/Graphics.hpp>
#include <string>

const sf::Keyboard::Key controls[4] = {
    sf::Keyboard::A,
    sf::Keyboard::Z,
    sf::Keyboard::Up,
    sf::Keyboard::Down
};

// Parameters
const sf::Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;
const float time_step = 0.017f;

const float initial_velocity_x = 100.f;
const float initial_velocity_y = 60.f;
const float velocity_multiplier = 1.1f;

// Objects
sf::CircleShape ball;
sf::RectangleShape paddles[2];

// Ball
sf::Vector2f ball_velocity;
bool is_player_serving = true;

// Score
int player1_score = 0;
int player2_score = 0;

// Text
sf::Font font;
sf::Text text;


void reset() {

    // Reset paddle positions
    paddles[0].setPosition(
        sf::Vector2f(
            paddleOffsetWall + paddleSize.x / 2.f,
            gameHeight / 2.f
        )
    );

    paddles[1].setPosition(
        sf::Vector2f(
            gameWidth - paddleOffsetWall - paddleSize.x / 2.f,
            gameHeight / 2.f
        )
    );

    // Reset ball position
    ball.setPosition(
        sf::Vector2f(
            gameWidth / 2.f,
            gameHeight / 2.f
        )
    );

    // Set ball velocity based on serving player
    ball_velocity = sf::Vector2f(
        is_player_serving
            ? initial_velocity_x
            : -initial_velocity_x,
        initial_velocity_y
    );

    // Update score text
    text.setString(
        std::to_string(player1_score) +
        "       " +
        std::to_string(player2_score)
    );

    // Centre score text
    text.setPosition(
        sf::Vector2f(
            (gameWidth * 0.5f) -
            (text.getLocalBounds().width * 0.5f),
            0.f
        )
    );
}


void init() {

    // Set size and origin of paddles
    for (sf::RectangleShape &p : paddles) {
        p.setSize(paddleSize);
        p.setOrigin(paddleSize / 2.f);
    }

    // Set size and origin of ball
    ball.setRadius(ballRadius);
    ball.setOrigin(
        sf::Vector2f(
            ballRadius,
            ballRadius
        )
    );

    // Load font
    font.loadFromFile(
        "res/fonts/RobotoMono-Regular.ttf"
    );

    text.setFont(font);
    text.setCharacterSize(24);

    // Reset game
    reset();
}


void update(float dt) {

    // -------------------------
    // Player 1 movement
    // -------------------------

    float direction = 0.0f;

    if (sf::Keyboard::isKeyPressed(controls[0])) {
        direction--;
    }

    if (sf::Keyboard::isKeyPressed(controls[1])) {
        direction++;
    }

    paddles[0].move(
        sf::Vector2f(
            0.f,
            direction * paddleSpeed * dt
        )
    );


    // -------------------------
    // Player 2 movement
    // -------------------------

    direction = 0.0f;

    if (sf::Keyboard::isKeyPressed(controls[2])) {
        direction--;
    }

    if (sf::Keyboard::isKeyPressed(controls[3])) {
        direction++;
    }

    paddles[1].move(
        sf::Vector2f(
            0.f,
            direction * paddleSpeed * dt
        )
    );


    // -------------------------
    // Keep paddles on screen
    // -------------------------

    for (sf::RectangleShape &p : paddles) {

        float y = p.getPosition().y;

        if (y - paddleSize.y / 2.f < 0.f) {

            p.setPosition(
                sf::Vector2f(
                    p.getPosition().x,
                    paddleSize.y / 2.f
                )
            );
        }

        if (y + paddleSize.y / 2.f > gameHeight) {

            p.setPosition(
                sf::Vector2f(
                    p.getPosition().x,
                    gameHeight - paddleSize.y / 2.f
                )
            );
        }
    }


    // -------------------------
    // Move ball
    // -------------------------

    ball.move(ball_velocity * dt);


    // -------------------------
    // Ball collision
    // -------------------------

    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;


    // Bottom wall
    if (by > gameHeight) {

        ball_velocity.x *= velocity_multiplier;
        ball_velocity.y *= -velocity_multiplier;

        ball.move(
            sf::Vector2f(
                0.f,
                -10.f
            )
        );
    }


    // Top wall
    else if (by < 0) {

        ball_velocity.x *= velocity_multiplier;
        ball_velocity.y *= -velocity_multiplier;

        ball.move(
            sf::Vector2f(
                0.f,
                10.f
            )
        );
    }


    // Right score wall
    else if (bx > gameWidth) {

        player1_score++;

        is_player_serving = true;

        reset();
    }


    // Left score wall
    else if (bx < 0) {

        player2_score++;

        is_player_serving = false;

        reset();
    }


    // -------------------------
    // Left paddle collision
    // -------------------------

    else if (
        bx - ballRadius <
            paddles[0].getPosition().x +
            paddleSize.x / 2.f &&

        bx >
            paddles[0].getPosition().x &&

        by >
            paddles[0].getPosition().y -
            paddleSize.y / 2.f &&

        by <
            paddles[0].getPosition().y +
            paddleSize.y / 2.f
    ) {

        ball_velocity.x *= -velocity_multiplier;

        ball_velocity.y *= velocity_multiplier;

        ball.setPosition(
            sf::Vector2f(
                paddles[0].getPosition().x +
                paddleSize.x / 2.f +
                ballRadius,
                ball.getPosition().y
            )
        );
    }


    // -------------------------
    // Right paddle collision
    // -------------------------

    else if (
        bx + ballRadius >
            paddles[1].getPosition().x -
            paddleSize.x / 2.f &&

        bx <
            paddles[1].getPosition().x &&

        by >
            paddles[1].getPosition().y -
            paddleSize.y / 2.f &&

        by <
            paddles[1].getPosition().y +
            paddleSize.y / 2.f
    ) {

        ball_velocity.x *= -velocity_multiplier;

        ball_velocity.y *= velocity_multiplier;

        ball.setPosition(
            sf::Vector2f(
                paddles[1].getPosition().x -
                paddleSize.x / 2.f -
                ballRadius,
                ball.getPosition().y
            )
        );
    }
}


void render(sf::RenderWindow &window) {

    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);
    window.draw(text);
}


int main() {

    sf::RenderWindow window(
        sf::VideoMode(
            gameWidth,
            gameHeight
        ),
        "PONG"
    );

    window.setVerticalSyncEnabled(true);

    init();

    sf::Clock clock;

    while (window.isOpen()) {

        float dt = clock.restart().asSeconds();

        sf::Event event;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        update(dt);

        window.clear();

        render(window);

        window.display();
    }

    return 0;
}
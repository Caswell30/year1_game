#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

Vector2f ballVelocity;
bool isPlayer1Serving = false;
const float initialVelocityX = 100.f;
const float initialVelocityY = 60.f;
const float velocityMultiplier = 1.1f;

const Keyboard::Key controls[4] = {
    Keyboard::A,   // Player1 UP
    Keyboard::Z,   // Player1 Down
    Keyboard::Up,  // Player2 UP
    Keyboard::Down // Player2 Down
};

const Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;

CircleShape ball;
RectangleShape paddles[2];

void Load() {
    for (auto& p : paddles) {
        p.setSize(paddleSize);
        p.setOrigin(paddleSize / 2.f);
    }

    ball.setRadius(ballRadius);
    ball.setOrigin(ballRadius, ballRadius);

    paddles[0].setPosition(Vector2f(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f));
    paddles[1].setPosition(Vector2f(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f));

    ball.setPosition(Vector2f(gameWidth / 2.f, gameHeight / 2.f));
    ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };
}

void Reset() {
    Load(); // Reuse the Load function to reset the game state
    ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY }; // Reset ball velocity
}

void Update(RenderWindow& window) {
    // Reset clock, recalculate deltatime
    static Clock clock;
    float dt = clock.restart().asSeconds();
    // check and consume events
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    // Quit Via ESC Key
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    // handle paddle movement
    float direction = 0.0f;
    if (Keyboard::isKeyPressed(controls[0])) {
        direction--;
    }
    if (Keyboard::isKeyPressed(controls[1])) {
        direction++;
    }
    paddles[0].move(Vector2f(0.f, direction * paddleSpeed * dt));
    ball.move(ballVelocity * dt);

    // check ball collision
    const float bx = ball.getPosition().x;

    const float by = ball.getPosition().y;

    if (by > gameHeight) { //bottom wall
        // bottom wall
        ballVelocity.x *= velocityMultiplier;
        ballVelocity.y *= -velocityMultiplier;
        ball.move(Vector2f(0.f, -10.f));
        Reset();
    }
    else if (bx > gameWidth) {
        // right wall
        Reset();
    }
    else if (bx < 0) {
        // left wall
        Reset();
    }
    else if (
        // Ball is inline or behind the left paddle AND
        bx < paddleSize.x + paddleOffsetWall &&
        // Ball is below the top edge of the left paddle AND
        by > paddles[0].getPosition().y - (paddleSize.y * 0.5) &&
        // Ball is above the bottom edge of the left paddle
        by < paddles[0].getPosition().y + (paddleSize.y * 0.5))
    {
        // Bounce off left paddle
        ballVelocity.x = -ballVelocity.x * velocityMultiplier; // Reverse X direction
        ball.move(Vector2f(10.f, 0.f)); // Prevent sticking to paddle
    }
    else if (
        // Ball is inline or behind the right paddle AND
        bx > gameWidth - paddleOffsetWall - paddleSize.x &&
        // Ball is below the top edge of the right paddle AND
        by > paddles[1].getPosition().y - (paddleSize.y * 0.5) &&
        // Ball is above the bottom edge of the right paddle
        by < paddles[1].getPosition().y + (paddleSize.y * 0.5))
    {
        // Bounce off right paddle
        ballVelocity.x = -ballVelocity.x * velocityMultiplier; // Reverse X direction
        ball.move(Vector2f(-10.f, 0.f)); // Prevent sticking to paddle
    }


}

void Render(RenderWindow& window) {
    // Draw Everything
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);
}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
    Load();
    while (window.isOpen()) {
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}
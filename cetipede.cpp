#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;
//Muneeba Zaheer
//23i-0581
//CS_B

// game constants
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid
const int x = 0;
const int y = 1;
const int exists = 2;
const int lastPlayableRow = gameRows - 5; // Last 5 game rows
const int maxHealth = 2; //mushroom max health

int gameGrid[gameRows][gameColumns] = {}; // Initializing GameGrid.

//game functions
void drawPlayer(RenderWindow& window, float player[], Sprite& playerSprite); //to draw the player sprite
void drawBullet(RenderWindow& window, float bullet[], Sprite& bulletSprite); //it draws the bullet
void fire(float bullet[], float player[]); //to fire the bullet
void moveBullet(float bullet[], Clock& bulletClock); //after firing it movies the bullet up
void drawMushroom(RenderWindow& window, Texture& MushroomTexture); //draws the mushrooms
void showMushroom(int num); //calculates the total number of mushrooms
bool collision(float bullet[], int& score); //detects bullet and mushroom collisions
void drawCentipede(RenderWindow& window, float centipede[][3], int centipedeLength, Sprite& centipedeSprite, Sprite& centipedeHeadSprite); //draws the centipede
void moveCentipede(float centipede[][3], int centipedeLength, float& centipedeDirection, int& lastCentipedeRow); //moves the centipede 
bool isMushroomPresent(float centipede[][3], int centipedeLength, int gameGrid[][gameColumns]); //checks the presence of mushroom 
void handleMushroomCollision(float centipede[][3], int centipedeLength, float& centipedeDirection); //checks the collsion of the mushroom with centipede
void handleCentipedeBounds(float centipede[][3], int centipedeLength, float& centipedeDirection, int& lastCentipedeRow); //keeps the centipede in player regio
bool checkPlayerCollision(float player[], float centipede[][3], int centipedeLength); //checks the collision of centipede wth the player
bool collisionCentipede(RenderWindow& window, float bullet[], float centipede[][3], int centipedeLength, int& score); //checks the collsion of centipede with the bullet

int main() {
    // Window defining
    RenderWindow window(VideoMode(resolutionX, resolutionY), "Centipede", Style::Close | Style::Titlebar);
    window.setSize(Vector2u(640, 640));
    window.setPosition(Vector2i(100, 0));

    // Background defining
    Texture backgroundTexture;
    Sprite backgroundSprite;
    backgroundTexture.loadFromFile("Textures/background.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(Color(255, 255, 255, 255 * 0.4));

    // Initializing Background Music.
    Music bgMusic;
    bgMusic.openFromFile("Music/field_of_hopes.ogg");
    bgMusic.play();
    bgMusic.setVolume(45);

    // Firing sound initialization
    SoundBuffer firingBuffer;
    Sound firingSound;
    firingBuffer.loadFromFile("Effects/fire1.wav");
    firingSound.setBuffer(firingBuffer);
    firingSound.setVolume(100);

    // Mushroom finished sound initialization
    SoundBuffer mushroomBuffer;
    Sound mushroomSound;
    mushroomBuffer.loadFromFile("Effects/kill.wav");
    mushroomSound.setBuffer(mushroomBuffer);
    mushroomSound.setVolume(100);

    // Player death sound initialization
    SoundBuffer deathBuffer;
    Sound deathSound;
    deathBuffer.loadFromFile("Effects/death.wav");
    deathSound.setBuffer(deathBuffer);
    deathSound.setVolume(100);

    // Initializing player
    float player[3] = {};
    player[x] = (gameColumns / 2) * boxPixelsX;
    player[y] = (gameRows - 5) * boxPixelsY + boxPixelsY / 2;
    Texture playerTexture;
    Sprite playerSprite;
    playerTexture.loadFromFile("Textures/player.png");
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Initializing Bullet and Bullet Sprites.
    float bullet[3] = {};
    bullet[x] = player[x];
    bullet[y] = player[y] - boxPixelsY;
    bullet[exists] = true;
    Clock bulletClock;
    Texture bulletTexture;
    Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Initializing mushrooms.
    srand(time(0));
    int number = rand() % 10 + 21;
    Texture MushroomTexture;
    MushroomTexture.loadFromFile("Textures/mushroom.png");
    showMushroom(number);

    // Initializing Centipede.
    Texture centipedeTexture;
    Sprite centipedeSprite;
    centipedeTexture.loadFromFile("Textures/c_body_left_walk.png");
    centipedeSprite.setTexture(centipedeTexture);
    centipedeSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Initializing Centipede.
    const int centipedeLength = 12; // Body segments + Head
    float centipede[centipedeLength][3]; //centipede storing array
    srand(time(0));

    // Initialize centipede head at a random position
    centipede[0][x] = rand() % (gameRows - 5) * boxPixelsX; // Random X position within playable area
    centipede[0][y] = rand() % (gameColumns - 1) * boxPixelsY; // Random Y position within playable area
    centipede[0][exists] = true;
    
    for (int i = 1; i < centipedeLength; ++i) {
        centipede[i][x] = centipede[i - 1][x] + boxPixelsX; // Align body segments horizontally with the head
        centipede[i][y] = centipede[0][y];
        centipede[i][exists] = true;
    }

    // Initializing the centipede's head.
    Texture centipedeHeadTexture;
    Sprite centipedeHeadSprite;
    centipedeHeadTexture.loadFromFile("Textures/c_head_left_walk.png");
    centipedeHeadSprite.setTexture(centipedeHeadTexture);
    centipedeHeadSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Initializing score font:
    Font scoreFont;
    scoreFont.loadFromFile("Textures/OMEGLE.ttf");
    Text scoreText;
    scoreText.setFont(scoreFont);
    scoreText.setString("Score : ");
    scoreText.setCharacterSize(34);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(770, 15);
    scoreText.setStyle(Text::Bold);

    // Initializing gameOver font:
    Font ExitFont;
    ExitFont.loadFromFile("Textures/OMEGLE.ttf");
    Text ExitText;
    ExitText.setFont(ExitFont);
    ExitText.setString("!!!GAME OVER!!!");
    ExitText.setCharacterSize(110);
    ExitText.setFillColor(Color::Cyan);
    ExitText.setPosition(130, 400);
    ExitText.setStyle(Text::Bold);

    // Scoring
    int score = 0; // Score of killing mushrooms.
    float movementSpeed = 0.5f; // Speed of the player
    bool lost = false; // To consider the behavior of the game when lost
    gameGrid[4][6] = 2; // To match the mushroom

    float centipedeDirection = -1.0f; // Start by moving left
    int lastCentipedeRow = 0; // To bound the centipede
    bool hasFiredSound = false; // To play the firing sound once
    bool collisionOccurredThisIteration = false; // To calculate the score of the centipede

    // Main loop
    while (window.isOpen()) {
        // Check for key presses and limit movement within the last 5 game rows
        if (Keyboard::isKeyPressed(Keyboard::W) && player[y] > (gameColumns - 5) * boxPixelsY) {
            player[y] -= movementSpeed; // Move up
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && player[y] + boxPixelsY < resolutionY && player[y] < (gameColumns - 1) * boxPixelsY) {
            player[y] += movementSpeed; // Move down
        }

        if (Keyboard::isKeyPressed(Keyboard::A) && player[x] > 0) {
            player[x] -= movementSpeed; // Move left
        }

        if (Keyboard::isKeyPressed(Keyboard::D) && player[x] + boxPixelsX < resolutionX) {
            player[x] += movementSpeed; // Move right
        }

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        if (lost) //if player collides then the game is over
            continue;

        window.clear(); 
        window.draw(backgroundSprite); //drawing the background
        drawPlayer(window, player, playerSprite); 
        fire(bullet, player);

        if (bullet[exists] == true) {
            if (!hasFiredSound) {
                firingSound.play(); //firing sound produced
                hasFiredSound = true;
            }

            moveBullet(bullet, bulletClock);
            drawBullet(window, bullet, bulletSprite);

            if (collisionCentipede(window, bullet, centipede, centipedeLength, score)) {
                bullet[exists] = false; //if collison with encounted then the bullet is set to false
            }

            if (collision(bullet, score)) {
                mushroomSound.play(); //mushroom hit sound
                bullet[exists] = false; //if collision is detected then bullet is turned off
            }
        }

        if (!bullet[exists]) {
            hasFiredSound = false; //to manage the sound of bullet it resets it
        }

        drawMushroom(window, MushroomTexture);
        moveCentipede(centipede, centipedeLength, centipedeDirection, lastCentipedeRow);
        handleMushroomCollision(centipede, centipedeLength, centipedeDirection);

		//if the player collides with the centipede
        if (checkPlayerCollision(player, centipede, centipedeLength)) {
            window.draw(ExitText);
            lost = true;
            deathSound.play();
        }

        drawCentipede(window, centipede, centipedeLength, centipedeSprite, centipedeHeadSprite);
        
        //score display on the screen
        stringstream Fscore;
        Fscore << "Score: " << score;
        scoreText.setString(Fscore.str());
        window.draw(scoreText);

        window.display();
    }

    return 0;
}

// Function definitions

void drawPlayer(RenderWindow& window, float player[], Sprite& playerSprite) {
    playerSprite.setPosition(player[x], player[y]); //drawing the player sprite
    window.draw(playerSprite);
}

void fire(float bullet[], float player[]) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K) && bullet[exists] == false) { //shooting bullet on pressing K
        bullet[x] = player[x]; 
        bullet[y] = player[y] - boxPixelsY; //the bullet position initializaed to be the position of the player
        bullet[exists] = true;
    }
}

void moveBullet(float bullet[], Clock& bulletClock) {
    if (bulletClock.getElapsedTime().asMilliseconds() < 20) //for scmooth movement of the bullet
        return;

    bulletClock.restart();
    bullet[y] -= 20;

    if (bullet[y] < -32)
        bullet[exists] = false;
}

void drawBullet(RenderWindow& window, float bullet[], Sprite& bulletSprite) {
    bulletSprite.setPosition(bullet[x], bullet[y]); //bullet sprite drawing
    window.draw(bulletSprite);
}

void drawMushroom(RenderWindow& window, Texture& MushroomTexture) {
    // Initializing the damage mushroom sprite
    Texture DamageMushroomTexture;
    DamageMushroomTexture.loadFromFile("Textures/damagemushroom.png");

    for (int i = 0; i < gameRows; i++) {
        for (int j = 0; j < gameColumns; j++) {
            if (gameGrid[i][j] == 2 || gameGrid[i][j] == 1) { //game grid containg the mushrooms
                Sprite MushroomSprite;
                MushroomSprite.setTexture(MushroomTexture);
                Sprite DamageMushroomSprite; //damage mushrooms
                DamageMushroomSprite.setTexture(DamageMushroomTexture);

                if (gameGrid[i][j] == 2) {
                    MushroomSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
                } else if (gameGrid[i][j] == 1) {
                    DamageMushroomSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
                }

                if (gameGrid[i][j] == 2) {
                    MushroomSprite.setPosition(i * boxPixelsX, j * boxPixelsX);
                    window.draw(MushroomSprite);
                } else if (gameGrid[i][j] == 1) {
                    DamageMushroomSprite.setPosition(i * boxPixelsX, j * boxPixelsX);
                    window.draw(DamageMushroomSprite);
                }
            }
        }
    }
}

void showMushroom(int num) {
    int r; // Define random row
    int c; // Define random col
    srand(time(0));
    for (int i = 0; i < num; i++) {
        r = rand() % gameRows; 
        c = rand() % (gameColumns - 5);

        if (gameGrid[r][c] == 0) { //random positions of the mushrooms
            gameGrid[r][c] = 2;
        }
    }
}

bool collision(float bullet[], int& score) {
    int grid[2] = {static_cast<int>(bullet[x] / 31), static_cast<int>(bullet[y] / 32)};

    if (gameGrid[grid[x]][grid[y]] == 2 || gameGrid[grid[x]][grid[y]] == 1) {
        gameGrid[grid[x]][grid[y]]--;
        if (gameGrid[grid[x]][grid[y]] == 0) {
            score++;
        }
        return true; // Collision with mushroom
    }

    return false; // No collision with mushroom
}

void drawCentipede(RenderWindow& window, float centipede[][3], int centipedeLength, Sprite& centipedeSprite, Sprite& centipedeHeadSprite) {
    // Draw centipede head
    centipedeHeadSprite.setPosition(centipede[0][x], centipede[0][y]);
    window.draw(centipedeHeadSprite);

    // Draw centipede body segments
    for (int i = 1; i < centipedeLength; ++i) {
        if (centipede[i][exists]) {
            centipedeSprite.setPosition(centipede[i][x], centipede[i][y]);
            window.draw(centipedeSprite);
        }
    }
}

void moveCentipede(float centipede[][3], int centipedeLength, float& centipedeDirection, int& lastCentipedeRow) {
    const float speed = 0.1f; //centipede speed control
    static Clock clock;

    if (clock.getElapsedTime().asSeconds() >= (0.5f * speed)) { 
        clock.restart();

        // Updates the body segments to follow the head
        for (int i = centipedeLength - 1; i > 0; --i) {
            centipede[i][x] = centipede[i - 1][x];
            centipede[i][y] = centipede[i - 1][y];
        }

        // Moves the head based on the current direction
        centipede[0][x] += centipedeDirection * boxPixelsX;

        // Checks if the head has reached the screen edges
        if (centipede[0][x] < 0 || centipede[0][x] + boxPixelsX > resolutionX) {
            centipedeDirection *= -1.0f; // Change direction
            centipede[0][y] += boxPixelsY; // Move down one row
        }

        // Calls the new function to handle bounds
        handleCentipedeBounds(centipede, centipedeLength, centipedeDirection, lastCentipedeRow);

        // Updates the lastCentipedeRow variable
        lastCentipedeRow = centipede[0][y] / boxPixelsY;
    }
}

bool isMushroomPresent(float centipede[][3], int centipedeLength, int gameGrid[][gameColumns]) {
    // Checks if there is a mushroom in the centipede's path
    int headGridX = static_cast<int>(centipede[0][x] / boxPixelsX);
    int headGridY = static_cast<int>(centipede[0][y] / boxPixelsY);

    // Checks for a mushroom at the head position
    if (headGridX >= 0 && headGridX < gameRows && headGridY >= 0 && headGridY < gameColumns) {
        if (gameGrid[headGridX][headGridY] == 2) {
            gameGrid[headGridX][headGridY] = 0;
            return true;
        }
    }

    // Checks for a mushroom in the path of the body segments
    for (int i = 1; i < centipedeLength; ++i) {
        int bodyGridX = static_cast<int>(centipede[i][x] / boxPixelsX);
        int bodyGridY = static_cast<int>(centipede[i][y] / boxPixelsY);

        // Checks if body segment is within valid range
        if (bodyGridX >= 0 && bodyGridX < gameRows && bodyGridY >= 0 && bodyGridY < gameColumns) {
            if (gameGrid[bodyGridX][bodyGridY] == 2) {
                return true;
            }
        }
    }

    return false;
}

void handleMushroomCollision(float centipede[][3], int centipedeLength, float& centipedeDirection) {
    // Checks if there is a mushroom in the centipede's path
    if (isMushroomPresent(centipede, centipedeLength, gameGrid)) {
        // Moves down one row if within bounds
        if (centipede[0][y] + boxPixelsY < resolutionY) {
            centipede[0][y] += boxPixelsY;
        }

        // Changes direction
        centipedeDirection *= -1.0f;
    }
}

void handleCentipedeBounds(float centipede[][3], int centipedeLength, float& centipedeDirection, int& lastCentipedeRow) {
    // Checks if the centipede reached the bottom of the screen
    if (centipede[0][y] + boxPixelsY > resolutionY) {
        // Bounces back within the last 5 rows of the player area
        centipedeDirection *= -1.0f; // Change direction
        lastCentipedeRow = (resolutionY - boxPixelsY * 5) / boxPixelsY;

        // Moves to the last 5 rows of the player area
        centipede[0][y] = lastCentipedeRow * boxPixelsY;

        // Adjusts the position to wrap around the screen horizontally
        if (centipede[0][x] < 0) {
            centipede[0][x] = resolutionX - boxPixelsX;
        } else if (centipede[0][x] + boxPixelsX > resolutionX) {
            centipede[0][x] = 0;
        }
    }
}

bool checkPlayerCollision(float player[], float centipede[][3], int centipedeLength) {
    // Checks for collision with each segment of the centipede
    for (int i = 0; i < centipedeLength; ++i) {
        if (centipede[i][exists]) {
            // Calculate boundaries for player and centipede segment
            float playerLeft = player[x];
            float playerRight = player[x] + boxPixelsX;
            float playerTop = player[y];
            float playerBottom = player[y] + boxPixelsY;

            float centipedeLeft = centipede[i][x];
            float centipedeRight = centipede[i][x] + boxPixelsX;
            float centipedeTop = centipede[i][y];
            float centipedeBottom = centipede[i][y] + boxPixelsY;

            // Check for overlap in both X and Y dimensions
            if (playerRight > centipedeLeft && playerLeft < centipedeRight &&
                playerBottom > centipedeTop && playerTop < centipedeBottom) {
                // Collision detected
                return true;
            }
        }
    }
    return false;
}
bool collisionCentipede(sf::RenderWindow& window, float bullet[], float centipede[][3], int centipedeLength, int& score) {
    for (int i = 0; i < centipedeLength; ++i) {
        // Check if bullet coordinates overlap with the segment's position
        if (bullet[x] > centipede[i][x] && bullet[x] < centipede[i][x] + boxPixelsX &&
            bullet[y] > centipede[i][y] && bullet[y] < centipede[i][y] + boxPixelsY) {

            // Deactivate the hit segment
            centipede[i][exists] = false;

            if (centipede[i][exists] == false) {
                score += 10; // adding 10 to the score for hitting
                return true;
            }
        }

        // Check if bullet coordinates overlap with the head's position (assuming head is at index 0)
        if (i == 0 && bullet[x] > centipede[i][x] && bullet[x] < centipede[i][x] + boxPixelsX &&
            bullet[y] > centipede[i][y] && bullet[y] < centipede[i][y] + boxPixelsY) {

            // Deactivate the head segment
            centipede[i][exists] = false;

            if (!centipede[i][exists] == false) {
                score += 20; // adding 20 to the score for hitting the head
                // Remove the head segment from the array (shift the remaining segments)
                for (int j = i; j < centipedeLength - 1; ++j) {
                    for (int k = 0; k < 3; ++k) {
                        centipede[j][k] = centipede[j + 1][k];
                    }
                }
                return true;
            }
        }
    }

    return false;
}


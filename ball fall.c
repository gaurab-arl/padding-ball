#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

// Screen dimensions
const int screenWidth = 640;
const int screenHeight = 480;

// Paddle dimensions
const int paddleWidth = 100;
const int paddleHeight = 20;
const int paddleSpeed = 50;

// Ball dimensions
const int ballRadius = 10;
int ballSpeed = 5;

// Function prototypes
void drawPaddle(int x);
void drawBall(int x, int y, bool isHovered);
void displayScore(int score, int level);
void showMainMenu();
void drawBorder();

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int paddleX = screenWidth / 2 - paddleWidth / 2;
    int ballX = rand() % (screenWidth - 2 * ballRadius) + ballRadius;
    int ballY = ballRadius;
    int ballDirectionX = 1; // 1 for right, -1 for left
    int score = 0;
    int level = 1;
    int consecutiveHits = 0;
    char ch;
    bool isHovered = false;

    // Seed random number generator
    srand(time(0));

    // Display the main menu
    showMainMenu();

    // Wait for a key press to start the game
    getch();

    while (true) {
        // Clear screen
        cleardevice();

        // Draw the border
        drawBorder();

        // Draw paddle
        drawPaddle(paddleX);

        // Draw ball with hover effect
        drawBall(ballX, ballY, isHovered);

        // Display score and level
        displayScore(score, level);

        // Move ball
        ballY += ballSpeed;
        ballX += ballDirectionX * ballSpeed;

        // Ball and paddle collision
        if (ballY + ballRadius >= screenHeight - paddleHeight &&
            ballX >= paddleX && ballX <= paddleX + paddleWidth) {
            ballY = ballRadius;
            ballX = rand() % (screenWidth - 2 * ballRadius) + ballRadius;
            score += 1 + consecutiveHits; // Award bonus points for consecutive hits
            consecutiveHits++;
            isHovered = true; // Activate hover effect

            if (score % 10 == 0) {
                level++;
                ballSpeed += 2; // Increase speed each level
            }
        } else if (ballY - ballRadius > screenHeight) {
            // Ball missed
            outtextxy(screenWidth / 2 - 50, screenHeight / 2, "Game Over!");
            getch();
            break;
        } else {
            isHovered = false; // Deactivate hover effect if not hitting the paddle
        }

        // Ball hits the left or right wall
        if (ballX - ballRadius <= 0 || ballX + ballRadius >= screenWidth) {
            ballDirectionX *= -1;
        }

        // Paddle controls
        if (kbhit()) {
            ch = getch();
            if (ch == 75) { // Left arrow key
                paddleX -= paddleSpeed;
                if (paddleX < 0) paddleX = 0;
            }
            if (ch == 77) { // Right arrow key
                paddleX += paddleSpeed;
                if (paddleX > screenWidth - paddleWidth) paddleX = screenWidth - paddleWidth;
            }
        }

        // Delay for smooth motion
        delay(30);
    }

    closegraph();
    return 0;
}

// Draw the paddle
void drawPaddle(int x)
{
    setfillstyle(SOLID_FILL, WHITE);
    bar(x, screenHeight - paddleHeight, x + paddleWidth, screenHeight);
}

// Draw the ball with hover effect
void drawBall(int x, int y, bool isHovered)
{
    setfillstyle(SOLID_FILL, isHovered ? YELLOW : RED); // Change color on hover
    fillellipse(x, y, ballRadius, ballRadius);
}

// Display the score and level
void displayScore(int score, int level)
{
    settextstyle(BOLD_FONT, HORIZ_DIR, 3); // Set larger font size
    char scoreText[30];
    sprintf(scoreText, "Score: %d  Level: %d", score, level);
    outtextxy(20, 20, scoreText);
}

// Show the main menu
void showMainMenu()
{
    cleardevice();
    settextstyle(BOLD_FONT, HORIZ_DIR, 4);
    outtextxy(screenWidth / 2 - 160, screenHeight / 2 - 60, "Welcome to Paddle Game");
    outtextxy(screenWidth / 2 - 160, screenHeight / 2, "Press any key to start");
}

// Draw the border
void drawBorder()
{
    setcolor(WHITE);
    rectangle(0, 0, screenWidth - 1, screenHeight - 1);
}


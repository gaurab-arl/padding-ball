#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

// Screen dimensions
const int screenWidth = 640;
const int screenHeight = 480;

// Paddle dimensions
const int paddleWidth = 100;
const int paddleHeight = 10;
const int paddleSpeed = 20;

// Ball dimensions
const int ballRadius = 10;
int ballSpeed = 5;

// Function prototypes
void drawPaddle(int x);
void drawBall(int x, int y);
void displayScore(int score, int level);

int main()
{
    int gr = DETECT, gm;
    initgraph(&gr, &gm, "");

    int paddleX = screenWidth / 2 - paddleWidth / 2;
    int ballX = rand() % (screenWidth - 2 * ballRadius) + ballRadius;
    int ballY = ballRadius;
    int ballDirectionX = 1; // 1 for right, -1 for left
    int score = 0;
    int level = 1;
    int consecutiveHits = 0;
    char ch;

    // Seed random number generator
    srand(time(0));

    while (true) {
        // Clear screen
        cleardevice();

        // Draw paddle
        drawPaddle(paddleX);

        // Draw ball
        drawBall(ballX, ballY);

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

            if (score % 10 == 0) {
                level++;
                ballSpeed += 2; // Increase speed each level
            }
        } else if (ballY - ballRadius > screenHeight) {
            // Ball missed
            outtextxy(screenWidth / 2 - 50, screenHeight / 2, "Game Over!");
            getch();
            break;
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

// Draw the ball
void drawBall(int x, int y)
{
    setfillstyle(SOLID_FILL, RED);
    fillellipse(x, y, ballRadius, ballRadius);
}

// Display the score and level
void displayScore(int score, int level)
{
    char scoreText[30];
    sprintf(scoreText, "Score: %d  Level: %d", score, level);
    outtextxy(10, 10, scoreText);
}

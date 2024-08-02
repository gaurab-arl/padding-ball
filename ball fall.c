#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>  // For Beep function

// Screen dimensions
const int screenWidth = 640;
const int screenHeight = 480;

// Paddle dimensions
const int paddleWidth = 100;
const int paddleHeight = 10;
const int paddleSpeed = 50;

// Ball dimensions
const int ballRadius = 10;
int ballSpeed = 5;

// Function prototypes
void drawPaddle(int x);
void drawBall(int x, int y, bool isHovered);
void displayScore(int score, int level, int highScore);
void showMainMenu(int highScore);
void drawBorder();
void saveHighScore(int highScore);
int loadHighScore();
void playHitSound(); // Function prototype for sound effect

int main()
{
    int gr = DETECT, gm;
    initgraph(&gr, &gm, (char*)""); // Use nullptr instead of "" for path

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

    // Load the highest score from file
    int highScore = loadHighScore();

    // Display the main menu
    showMainMenu(highScore);

    // Wait for a key press to start the game
    ch = getch();
    if (ch == 'q' || ch == 'Q') {
        closegraph();
        return 0; // Quit the game if 'q' or 'Q' is pressed
    }

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
        displayScore(score, level, highScore);

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

            // Play the hit sound effect
            playHitSound();

            if (score % 5 == 0) {
                level++;
                ballSpeed += 2; // Increase speed each level
            }
        } else if (ballY - ballRadius > screenHeight) {
            // Ball missed
            outtextxy(screenWidth / 2 - 50, screenHeight / 2, "Game Over!");
            if (score > highScore) {
                highScore = score; // Update highest score
                saveHighScore(highScore); // Save the new high score to file
            }
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
            if (ch == 'q' || ch == 'Q') { // Quit game if 'q' or 'Q' is pressed
                if (score > highScore) {
                    highScore = score; // Update highest score
                    saveHighScore(highScore); // Save the new high score to file
                }
                break;
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
    setfillstyle(SOLID_FILL, YELLOW); // Set the paddle color to yellow
    bar(x, screenHeight - paddleHeight, x + paddleWidth, screenHeight);
}

// Draw the ball with hover effect
void drawBall(int x, int y, bool isHovered)
{
    setfillstyle(SOLID_FILL, isHovered ? YELLOW : RED); // Change color on hover
    fillellipse(x, y, ballRadius, ballRadius);
}

// Display the score and level
void displayScore(int score, int level, int highScore)
{
    settextstyle(BOLD_FONT, HORIZ_DIR, 3); // Set larger font size
    char scoreText[50];
    sprintf(scoreText, "Score: %d  Level: %d  High Score: %d", score, level, highScore);
    outtextxy(20, 20, scoreText);
}

// Show the main menu
void showMainMenu(int highScore)
{
    cleardevice();
    settextstyle(BOLD_FONT, HORIZ_DIR, 4);
    setcolor(13);
    outtextxy(screenWidth / 2 - 160, screenHeight / 2 - 60, "Welcome to Pong Game");
    outtextxy(screenWidth / 2 - 160, screenHeight / 2, "Press any key to start");
    outtextxy(screenWidth / 2 - 160, screenHeight / 2 + 60, "Press 'Q' to quit");
    
    char highScoreText[30];
    sprintf(highScoreText, "High Score: %d", highScore);
    outtextxy(screenWidth / 2 - 160, screenHeight / 2 + 120, highScoreText);
}

// Draw the border
void drawBorder()
{
    setcolor(WHITE);
    rectangle(0, 0, screenWidth - 1, screenHeight - 1);
}

// Save the high score to a file
void saveHighScore(int highScore)
{
    FILE* file = fopen("highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", highScore);
        fclose(file);
    }
}

// Load the high score from a file
int loadHighScore()
{
    int highScore = 0;
    FILE* file = fopen("highscore.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }
    return highScore;
}

// Play a sound when the ball hits the paddle
void playHitSound()
{
    Beep(1000, 100); // Play a sound at 1000 Hz for 100 milliseconds
}

#include "graphics.h"
#include "circle.h"
#include "rect.h"
#include "Button.h"
#include "bitUser.h"
#include <iostream>
#include <memory>
#include <vector>
#include <unistd.h>
#include <thread>
#include <time.h>
using namespace std;
using namespace chrono;
using namespace this_thread;

Button defaultButton({1/255.0, 110/255.0, 214/255.0}, {827, 460}, 200, 100, "DEFAULT");
Button galaxyButton({119/255.0, 11/255.0, 224/255.0}, {827, 570}, 200, 100, "GALAXY");
Button marioButton({201/255.0, 20/255.0, 20/255.0}, {827, 680}, 200, 100, "MARIO MODE");
const int SIDE_LENGTH = 20;
GLdouble width, height;
int player1Score, player2Score, finalScore = 7; // Set Score limit
float ballPosX, ballPosY;
bool player1scored, player2scored;
bool goal = false;
int wd;
vector<Circle> galaxy;
bitUser mario;
// Create enum to distinguish between three different screens
enum Screen {
    Login = 0,
    Start = 1,
    Play = 2,
    Stop = 3,
};
enum Background {
    Default = 0,
    Galaxy = 1,
    SuperMario = 2
};
// Initialize screen variable to start screen
Screen screen = Login;
Background background = Default;
const color skyBlue(77/255.0, 213/255.0, 240/255.0);
const color grassGreen(26/255.0, 176/255.0, 56/255.0);
const color white(1, 1, 1);
const color brickRed(201/255.0, 20/255.0, 20/255.0);
const color darkBlue(1/255.0, 110/255.0, 214/255.0);
const color purple(119/255.0, 11/255.0, 224/255.0);
const color black(0, 0, 0);
const color magenta(1, 0, 1);
const color orange(1, 163/255.0, 22/255.0);
const color cyan (0, 1, 1);
const int RADIUS = 25;
time_t start, end;

// Initialize users and game ball using constructors
Rect user1;
Rect user2;
Circle gameBall(1, 1, 1, 0, 1 - (rand() % 20 / 100.0), 1 - (rand() % 20 / 100.0), 1 - (rand() % 20 / 100.0),
                0, 700, 540, RADIUS);

void initSnow() {
    for (int i = 0; i < 150; ++i) {
        galaxy.push_back(Circle(1, 1, (rand() % 2) + 0.7, 1, rand() % int(width), -(rand() % int(height)), rand() % 2 + 1));
    }
}


void initGameBall() {
    if (rand() % 2) {
        gameBall.setVelocity(rand() % 8 + 7, rand() % 8 + 7);
        while (gameBall.getYVelocity() == 0) { // Loop to ensure that ball is not stuck on a horizontal path
            gameBall.setVelocity(rand() % 8 + 7, rand() % 8 + 7);
        }
    }
    else {
        gameBall.setVelocity(-(rand() % 8 + 7), -(rand() % 8 + 7));
        while (gameBall.getYVelocity() == 0) {
            gameBall.setVelocity(-(rand() % 8 + 7), -(rand() % 8 + 7));
        }
    }
}

void initUsers() {
    // Initialize User1 to be a rectangle of width = (width of screen * 0.016)
    // and height = (height of screen * 0.136)
    // centered in the top left corner of the graphics window
    user1.setColor(white);
    user1.setSize(width * 0.008,height * 0.070);
    user1.setCenter(width * 0.85, height/2);
    user1.setXVelocity(-0.63);
    // Initialize User2
    user2.setColor(white);
    user2.setSize(width * 0.008,height * 0.070);
    user2.setCenter(width * 0.02, height/2);
    user2.setXVelocity(0.63);
}

void init() {
    width = 1920;
    height = 1080;
    srand(time(0));
    initSnow();
    initGameBall();
    initUsers();
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0, 0, 0, 1.0f);
}

void drawLoginScreen() {
    glColor3f(1, 1, 1);
    string label = "Welcome to PONG";
    glRasterPos2i(745 , 360 ); // Center label
    for (const char &letter : label) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
    }
    string label2 = "Choose the background you'd like to play on: ";
    glRasterPos2i(645 , 380 ); // Center label
    for (const char &letter : label2) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
    }
    defaultButton.draw();
    galaxyButton.draw();
    marioButton.draw();
}

// Draws screen that prompts user to press enter
void drawStartScreen() {
    glColor3f(1, 1, 1);
    string label = "Press ENTER to start";
    glRasterPos2i(750 , 540 ); // Center label
    for (const char &letter : label) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
    }
}

void drawStopScreen() {
    glColor3f(1, 1, 1);
    string label = "GAME OVER";
    glRasterPos2i(750 , 450 ); // Center label
    for (const char &letter : label) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
    }
    if (player1Score > player2Score) {
        string label2 = "Player 1 Won!";
        glRasterPos2i(780 , 470 ); // Center label
        for (const char &letter : label2) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
        }
    }
    else if (player2Score > player1Score) {
        string label2 = "Player 2 Won!";
        glRasterPos2i(780 , 470 ); // Center label
        for (const char &letter : label2) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
        }
    }

}

// Function used to draw each players' score on top of the screen
void drawPlayerScores() {
    glColor3f(1, 1, 1);
    string user2Score = "P2 Score: ";
    glRasterPos2i(10 ,50 ); // Center label
    for (const char &letter : user2Score) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
    }
    // Draw Updated Score (number) for user 2
    string p2score = to_string(player2Score);
    glRasterPos2f(85, 50);
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, p2score.c_str()[0]);

    glColor3f(1, 1, 1);
    string user1Score = "P1 Score: ";
    glRasterPos2i(1560 ,50 ); // Center label
    for (const char &letter : user1Score) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
    }
    // Draw Updated Score (number) for user q
    string p1score = to_string(player1Score);
    glRasterPos2f(1635, 50);
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, p1score.c_str()[0]);

    // Draw

}

void drawMiddleBorder() {
    for(int i = -1; i < 22; i++)
    {
        glColor3f(1.0,1.0,1.0);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);  /*  dashed  */
        glBegin(GL_LINES);
        glVertex2f(830, 50.0 + ((GLfloat) i * 50.0));
        glVertex2f(830, 50.0 + ((GLfloat)(i+1) * 50.0));
    }
    glDisable(GL_LINE_STIPPLE);
    glEnd();
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height); // DO NOT CHANGE THIS LINE (unless you are running Catalina on Mac)

    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE

    /*
     * Draw here
     */
    if (screen == Login) {
        drawLoginScreen();
    }

    else if (screen == Start) {
        // call the start screen draw method
        drawStartScreen();
    }

    else if (screen == Play) {
        start = time(0);
        if (background == Galaxy) {
            // Draw galaxy effect
            for (Circle &stars : galaxy) {
                stars.draw();
            }
        }
        else if (background == SuperMario) {
            mario.setStartingX(0);
            mario.setStartingY(0);
            mario.draw();
        }
        drawPlayerScores();
        drawMiddleBorder();
        if (!goal) {
            user1.draw();
            user2.draw();
            gameBall.drawBall();
        }
    }
    else if (screen == Stop) {
        // Draw game-over screen
        drawStopScreen();
    }
    glFlush();  // Render now
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }
    if (screen == Start) {
        // press enter to start
        if (key == 13) {
            screen = Play;
        }
    }
    if (screen == Play) {
        if (key == 'w' || key == 'W') {
            if (user2.getTopY() > height * 0.03) {
                user2.move(0, -25, 0);
            }
        }
        if (key == 's' || key == 'S') {
            if (user2.getBottomY() < height * 0.98) {
                user2.move(0, 25, 0);
            }
        }
    }

    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    //Can only happen in the second screen
    if (screen == Play) {
        switch(key) {
            case GLUT_KEY_DOWN:
                if (user1.getBottomY() < height * 0.98) {
                    user1.move(0,25,0);
                }

                break;
            case GLUT_KEY_UP:
                if (user1.getTopY() > height * 0.03) {
                    user1.move(0, -25, 0);
                }
                break;
        }
        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) {
    if (screen == Login) {
        // If the left button is down and the cursor is overlapping with the Button, call the pressDown method. Otherwise, call the release method.
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && defaultButton.isOverlapping(x, y)) {
            defaultButton.pressDown();
        }
        else {
            defaultButton.release();
        }
        // If the left button is up and the cursor is overlapping with the Button, call spawnConfetti.
        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && defaultButton.isOverlapping(x, y)) {
            background = Default;
            screen = Start;
        }
        // Press down function for Galaxy Button
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && galaxyButton.isOverlapping(x, y)) {
            galaxyButton.pressDown();
        }
        else {
            galaxyButton.release();
        }
        // If the left button is up and the cursor is overlapping with the Button, call spawnConfetti.
        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && galaxyButton.isOverlapping(x, y)) {
            background = Galaxy;
            screen = Start;
        }
        // Press down function for Mario Button
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && marioButton.isOverlapping(x, y)) {
            marioButton.pressDown();
        }
        else {
            marioButton.release();
        }
        // If the left button is up and the cursor is overlapping with the Button, call spawnConfetti.
        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && marioButton.isOverlapping(x, y)) {
            background = SuperMario;
            screen = Start;
        }
    }
    glutPostRedisplay();
}

void gameBallTimer(int dummy) {
    if (screen == Play) {
        gameBall.move(gameBall.getXVelocity(), gameBall.getYVelocity());
        if (gameBall.getLeftX() < user2.getRightX()) {
            // Ball crosses over user 2's x coordinate (left user)
            // Check if the ball collided with the user
            if ((gameBall.getBottomY() >= user2.getTopY() && gameBall.getTopY() <= user2.getTopY()) ||
                gameBall.getBottomY() >= user2.getTopY() && gameBall.getBottomY() <= user2.getBottomY() ||
                gameBall.getTopY() <= user2.getBottomY() && gameBall.getBottomY() >= user2.getBottomY()) {
                gameBall.bounceX();
            }
                // Otherwise it's a goal for user1
            else {
                if (gameBall.getLeftX() < user2.getLeftX()) {
                    player1Score++;
                    goal = true;
                    // Check if score to win is reached
                    if (player1Score == finalScore || player2Score == finalScore) {
                        screen = Stop;
                    }
                }
            }
        }
        if (gameBall.getRightX() > user1.getLeftX()) {
            // Ball crosses over user 1's x coordinate (right user)
            // Check if the ball collided with the user
            if ((gameBall.getBottomY() >= user1.getTopY() && gameBall.getTopY() <= user1.getTopY()) ||
                (gameBall.getBottomY() >= user1.getTopY() && gameBall.getBottomY() <= user1.getBottomY()) ||
                (gameBall.getTopY() <= user1.getBottomY() && gameBall.getBottomY() >= user1.getBottomY())) {
                gameBall.bounceX();
            }
                // Otherwise it's a goal for user2
            else {
                if (gameBall.getRightX() > user1.getRightX()) {
                    player2Score++;
                    goal = true;
                    // Check if score to win is reached
                    if (player1Score == finalScore || player2Score == finalScore) {
                        screen = Stop;
                    }
                }
            }
        }

        if (gameBall.getCenterY() < gameBall.getRadius()) {
            gameBall.bounceY();
            gameBall.setCenterY(gameBall.getRadius());
        } else if (gameBall.getCenterY() > (height - gameBall.getRadius())) {
            gameBall.bounceY();
            gameBall.setCenterY(height - gameBall.getRadius());
        }

        if (goal) {
            goal = false;
            // Reset the balls and users positions
            gameBall.setCenter(830, 540);
            initGameBall();
            initUsers();
            // Create a 1 second delay between rounds
            sleep(1);
            start = time(0);
        }
        gameBall.setBorderColor((rand() % 60 / 100.0), (rand() % 10 / 100.0), (rand() % 10 / 100.0), 0.5);
        //gameBall.setVelocity(gameBall.getXVelocity() + 1/10, gameBall.getYVelocity() + 1/10);
    }
        glutPostRedisplay();
        glutTimerFunc(20, gameBallTimer, dummy);

}


void galaxyTimer(int dummy) {
    if (background == Galaxy) {
        int counter = 0;
        for (Circle &stars : galaxy) {
            counter++;
            stars.moveY(stars.getRadius()/3);
            stars.setColor(1, 1, stars.getBlue() - 1.0/1000, 1);
            if (stars.getBlue() <= 0.699) {
                stars.setColor(0, 0, 0, 1);
            }
            // Nested if-statements used to increase randomness and decrease the probability
            // in order to "slow-down" the stars blinking
            if (rand() % 47) {
                if (counter % 7) {
                    stars.setColor(1, 1, (rand() % 2) + 0.7, 1);
                }
                //glutTimerFunc(0, blinkerTimer, 0);
            }

            if (stars.getTopY() > height) {
                stars.setCenter(rand() % int(width), stars.getRadius());
                //flake.setCenter(flake.getRadius(), rand() % int(width));
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(30, galaxyTimer, dummy);
}

void userTimer(int dummy) {
    if (screen == Play) {
        user1.moveX(user1.getXVelocity());
        user2.moveX(user2.getXVelocity());
        if (user2.getCenterX() >= 650 && user1.getCenterX() <= 1010) { // resets the user position once they are too close to center border.
            initUsers();
        }
    }
    glutPostRedisplay();
    glutTimerFunc(30, userTimer, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(500, 500); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Pong" /* title */ );
    glutFullScreen();
    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    //glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, galaxyTimer, 0);
    glutTimerFunc(0, gameBallTimer, 0);
    glutTimerFunc(120000, userTimer, 0); // After 2 minutes of gameplay the users begin to move towards the center.
//    glutTimerFunc(0, galaxyTimer, 0);
//    glutTimerFunc(0, gameBallTimer, 0);
    //glutTimerFunc(0, buildingTimer, 0);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}

/*******************************************************
   Author:    Bochao Zhang
   ID:        B00748967
   Project:   Assigment 1 of CSCI3161
   Date:      2020-10-02
********************************************************/
#include <freeglut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>//Used in srand

#define PI 3.14159265
#define DEG_TO_RAD PI/180
#define RATIO 1.33333//Default width/height ratio in this application

//All function used in this application
void initializeGL(void);
void reshape(int w, int h);
void myIdle(void);
void sparkleIdle(void);
void displayMenu(void);
void displayShape(void);
void displayMorph(void);
void displayMain(void);
void displaySparkle(void);
void sparksOne(void);
void sparksTwo(void);
void sparksThree(void);
void sparksFour(void);
void sparksFive(void);
void sparksSix(void);
void sparksSeven(void);
void sparksEight(void);
void sparksNine(void);
void sparksTen(void);
void mouse(int button, int state, int x, int y);

//Current window width & height
GLfloat curWidth, curHeight;

//interp for linear vertex move; theta for circling vertex move
GLfloat interp = 0.0;
GLfloat theta = 0.0;

//Following 10 'traq's are used for 10 sparks generated in the sparks function. Every 'traq' is implemented in corrspoding IDLE function
GLfloat traq1 = 0.0;
GLfloat traq2 = 0.0;
GLfloat traq3 = 0.0;
GLfloat traq4 = 0.0;
GLfloat traq5 = 0.0;
GLfloat traq6 = 0.0;
GLfloat traq7 = 0.0;
GLfloat traq8 = 0.0;
GLfloat traq9 = 0.0;
GLfloat traq10 = 0.0;

//id is a sign for enabling 'sparkle', ida for enabling 'aprks'
int id = 0;
int ida = 0;

//This function is used for window resize
void reshape(int w, int h) {
	//Update current width & height timely
	curWidth = w;
	curHeight = h;

	//Target width & height used in the new resized window 
	GLfloat width, height;

	//This prevent program crash when the height is resized to 0
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Resize the content inside the window, depends on the current ratio of width/height
	if (w >= h) {
		width = 480 * w / h;
		height = 480;
	}
	else {
		width = 640;
		height = 640 * h / w;
	}

	//Redisplay the content according to the current window size
	glOrtho(0, width, 0, height, 0, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Initializer of all
void initializeGL(void)
{
	//Set background to black color
	glClearColor(0, 0, 0, 0.0);

	//Set default drawing color to black
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(4.0);

	//Enable alpha channel
	glEnable(GL_BLEND);

	//Enable Smooth line drawing
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Default content display size
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

//This Idle controlls the movement of name initial to bolt
void myIdle(void) {
	interp += 0.01;

	//When reaches 1, stop morphing
	if (interp <= 1.0) {
		glutPostRedisplay();
	}
}

//Following IDLEs are used for sparks generated in 'sparks' function, there are ten kinds of sparks that will be generated randomly
//Structure are the same in 'myIdle', but without ending detection, since sparks need to be respawn
void oneIdle(void) {
	traq1 += 0.01;
	if (traq1 <= 1.0) {
		glutPostRedisplay();
	}
}
void twoIdle(void) {
	traq2 += 0.01;
	if (traq2 <= 1.0) {
		glutPostRedisplay();
	}
}
void threeIdle(void) {
	traq3 += 0.01;
	if (traq3 <= 1.0) {
		glutPostRedisplay();
	}
}
void fourIdle(void) {
	traq4 += 0.01;
	if (traq4 <= 1.0) {
		glutPostRedisplay();
	}
}
void fiveIdle(void) {
	traq5 += 0.01;
	if (traq5 <= 1.0) {
		glutPostRedisplay();
	}
}
void sixIdle(void) {
	traq6 += 0.01;
	if (traq6 <= 1.0) {
		glutPostRedisplay();
	}
}
void sevenIdle(void) {
	traq7 += 0.01;
	if (traq7 <= 1.0) {
		glutPostRedisplay();
	}
}
void eightIdle(void) {
	traq8 += 0.01;
	if (traq8 <= 1.0) {
		glutPostRedisplay();
	}
}
void nineIdle(void) {
	traq9 += 0.01;
	if (traq9 <= 1.0) {
		glutPostRedisplay();
	}
}
void tenIdle(void) {
	traq10 += 0.01;
	if (traq10 <= 1.0) {
		glutPostRedisplay();
	}
}

//This is the IDLE controls the sparkle movement
void sparkleIdle(void) {
	//Uses degrees for circular movement
	theta += 0.4;
	if (theta >= 360.0) {
		theta -= 360.0;
	}
	glutPostRedisplay();
}

//Main function
int main(int argc, char** argv) {
	glutInit(&argc, argv);

	//Display mode
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	//Initial window position and size
	glutInitWindowPosition(100, 150);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Assignment 1");

	//For random number generator
	srand(time(0));

	initializeGL();
	glutDisplayFunc(displayMain);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);

	glutMainLoop();
	return 0;
}

//Mouse movement detection
void mouse(int button, int state, int x, int y) {

	//when the width is higher, apply these detections
	if (curHeight*RATIO <= curWidth) {
		if (button == GLUT_RIGHT_BUTTON) {
			ida = 0;
			id = 0;
			glutDisplayFunc(displayMain);
		}
		if (button == GLUT_LEFT_BUTTON && x > (curHeight*RATIO)*(500.f / 640.f) && x < (curHeight*RATIO)*(620.f / 640.f) && y>curHeight*(415.f / 480.f) && y < curHeight*(455.f / 480.f)) {
			ida = 0;
			id = 0;
			interp = 0;
			glutDisplayFunc(displayMorph);
		}
		if (button == GLUT_LEFT_BUTTON && x > (curHeight*RATIO)*(60.f / 640.f) && x < (curHeight*RATIO)*(180.f / 640.f) && y>curHeight*(415.f / 480.f) && y < curHeight*(455.f / 480.f)) {
			ida = 1;
			id = 0;
			glutDisplayFunc(displaySparkle);
		}
		//if 'sparkle' is clicked, enable the button 'sparks' ready to be clicked, same as in the 'else' part
		if (button == GLUT_LEFT_BUTTON && x > (curHeight*RATIO)*(280.f / 640.f) && x < (curHeight*RATIO)*(400.f / 640.f) && y>curHeight*(415.f / 480.f) && y < curHeight*(455.f / 480.f) && ida == 1) {
			id = 1;
			glutDisplayFunc(displaySparkle);
		}
	}
	//else, apply these detections
	else {
		//gap stands for the distance between the acutual content and the top frame
		//add stands for the real ratio needs to be added on the gap to get the actual postion of the buttons
		GLfloat gap = curHeight - curWidth / RATIO;
		GLfloat add = curWidth / RATIO;
		if (button == GLUT_RIGHT_BUTTON) {
			ida = 0;
			id = 0;
			glutDisplayFunc(displayMain);
		}
		if (button == GLUT_LEFT_BUTTON && x > (curWidth)*(500.f / 640.f) && x < (curWidth)*(620.f / 640.f) && y>gap + add * (415.f / 480.f) && y < gap + add * (455.f / 480.f)) {
			ida = 0;
			id = 0;
			interp = 0;
			glutDisplayFunc(displayMorph);
		}
		if (button == GLUT_LEFT_BUTTON && x > (curWidth)*(60.f / 640.f) && x < (curWidth)*(180.f / 640.f) && y>gap + add * (415.f / 480.f) && y < gap + add * (455.f / 480.f)) {
			ida = 1;
			id = 0;
			glutDisplayFunc(displaySparkle);
		}
		if (button == GLUT_LEFT_BUTTON && x > (curWidth)*(280.f / 640.f) && x < (curWidth)*(400.f / 640.f) && y>gap + add * (415.f / 480.f) && y < gap + add * (455.f / 480.f) && ida == 1) {
			id = 1;
			glutDisplayFunc(displaySparkle);
		}
	}
}

//This is the Main menu display at the window bottom
void displayMenu(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//A grey large square for the menu back ground
	glBegin(GL_POLYGON);
	glColor3f(0.6, 0.6, 0.6);
	glVertex2i(0, 0);
	glVertex2i(640, 0);
	glVertex2i(640, 80);
	glVertex2i(0, 80);
	glEnd();

	//shadow of button 'sparkle'
	glBegin(GL_POLYGON);
	glColor3f(0.4, 0.4, 0.4);
	glVertex2i(60, 20);
	glVertex2i(180, 20);
	glVertex2i(180, 60);
	glVertex2i(60, 60);
	glEnd();

	//shadow of button 'sparks'
	glBegin(GL_POLYGON);
	glColor3f(0.4, 0.4, 0.4);
	glVertex2i(280, 20);
	glVertex2i(400, 20);
	glVertex2i(400, 60);
	glVertex2i(280, 60);
	glEnd();

	//shadow of button 'morph'
	glBegin(GL_POLYGON);
	glColor3f(0.4, 0.4, 0.4);
	glVertex2i(500, 20);
	glVertex2i(620, 20);
	glVertex2i(620, 60);
	glVertex2i(500, 60);
	glEnd();

	//actual button 'sparkle'
	glBegin(GL_POLYGON);
	glColor3f(0.8, 0.8, 0.8);
	glVertex2i(56, 24);
	glVertex2i(176, 24);
	glVertex2i(176, 64);
	glVertex2i(56, 64);
	glEnd();

	//actual button 'sparks'
	glBegin(GL_POLYGON);
	glColor3f(0.8, 0.8, 0.8);
	glVertex2i(276, 24);
	glVertex2i(396, 24);
	glVertex2i(396, 64);
	glVertex2i(276, 64);
	glEnd();

	//actual button 'morph'
	glBegin(GL_POLYGON);
	glColor3f(0.8, 0.8, 0.8);
	glVertex2i(496, 24);
	glVertex2i(616, 24);
	glVertex2i(616, 64);
	glVertex2i(496, 64);
	glEnd();

	//Text of 'sparkle' & 'sparks' & 'morph'
	glColor3f(0.4, 0.4, 0.4);
	glRasterPos2i(80, 35);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, "Sparkle");

	glColor3f(0.7, 0.7, 0.7);
	glRasterPos2i(300, 35);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, "Sparks");

	glColor3f(0.4, 0.4, 0.4);
	glRasterPos2i(520, 35);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, "Morph");
}

//This is the function that display my name initial letter 'Z'
void displayShape(void) {
	//Following three polygons forms the letter
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex2i(150, 400);
	glColor3f(0, 1, 0);
	glVertex2i(500, 400);
	glColor3f(0, 0, 1);
	glVertex2i(500, 350);
	glColor3f(0, 0, 1);
	glVertex2i(400, 350);
	glColor3f(1, 0, 0);
	glVertex2i(150, 350);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1);
	glVertex2i(500, 350);
	glColor3f(1, 0, 0);
	glVertex2i(375, 250);
	glColor3f(1, 0, 0);
	glVertex2i(250, 150);
	glColor3f(1, 0, 0);
	glVertex2i(150, 150);
	glColor3f(0, 0, 1);
	glVertex2i(275, 250);
	glColor3f(0, 0, 1);
	glVertex2i(400, 350);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex2i(150, 150);
	glColor3f(1, 0, 0);
	glVertex2i(250, 150);
	glColor3f(0, 0, 1);
	glVertex2i(500, 150);
	glColor3f(0, 0, 1);
	glVertex2i(500, 100);
	glColor3f(0, 1, 0);
	glVertex2i(150, 100);
	glEnd();
}

//The main and default display for this application, right-click anywhere anytime to go back to this status
void displayMain(void) {
	displayMenu();
	displayShape();

	//display all content
	glutSwapBuffers();
}

//This is the animation morhing the letter to a bolt
void displayMorph(void) {
	//Move the postion of every point to the target postion to form the shape bolt
	displayMenu();
	myIdle();
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex2i(150 + (interp) * 225, 400);
	glColor3f(0, 1, 0);
	glVertex2i(500 - interp * 70, 400 - interp * 85);
	glColor3f(0, 0, 1);
	glVertex2i(500 - interp * 100, 350 - interp * 40);
	glColor3f(0, 0, 1);
	glVertex2i(400 - (interp) * 100, 350 - (interp) * 50);
	glColor3f(1, 0, 0);
	glVertex2i(150 + (interp) * 100, 350 + (interp) * 40);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1);
	glVertex2i(500 - interp * 100, 350 - interp * 40);
	glColor3f(1, 0, 0);
	glVertex2i(375 + interp * 50, 250);
	glColor3f(1, 0, 0);
	glVertex2i(250 + interp * 145, 150 + interp * 95);
	glColor3f(1, 0, 0);
	glVertex2i(150 + interp * 160, 150 + interp * 85);
	glColor3f(0, 0, 1);
	glVertex2i(275, 250 + interp * 45);
	glColor3f(0, 0, 1);
	glVertex2i(400 - (interp) * 100, 350 - (interp) * 50);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex2i(150 + interp * 160, 150 + interp * 85);
	glColor3f(1, 0, 0);
	glVertex2i(250 + interp * 145, 150 + interp * 95);
	glColor3f(0, 0, 1);
	glVertex2i(500 - interp * 65, 150 - interp * 50);
	glColor3f(0, 0, 1);
	glVertex2i(500 - interp * 65, 100);
	glColor3f(0, 1, 0);
	glVertex2i(150 + interp * 135, 100 + interp * 132);
	glEnd();

	//dispay new button to states that the button is on
	glBegin(GL_POLYGON);
	glColor3f(0.7, 0.7, 0.7);
	glVertex2i(496, 24);
	glVertex2i(616, 24);
	glVertex2i(616, 64);
	glVertex2i(496, 64);
	glEnd();

	glColor3f(0.4, 0.4, 0.4);
	glRasterPos2i(520, 35);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, "Morph");

	glutSwapBuffers();
}

//Function to display sparkle
void displaySparkle(void) {
	displayMenu();
	displayShape();
	sparkleIdle();

	//New button replaces the original one to display the button "sparkle" is on and 'sparks' is ready to be clicked
	glBegin(GL_POLYGON);
	glColor3f(0.7, 0.7, 0.7);
	glVertex2i(56, 24);
	glVertex2i(176, 24);
	glVertex2i(176, 64);
	glVertex2i(56, 64);
	glEnd();

	glColor3f(0.4, 0.4, 0.4);
	glRasterPos2i(80, 35);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, "Sparkle");

	glColor3f(0.4, 0.4, 0.4);
	glRasterPos2i(300, 35);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, "Sparks");

	//The sparkle is formed with 4 line strips, and it moves in a circle, beside that it will twinkle in the same time
	glBegin(GL_LINE_STRIP);
	glColor4f(1, 1, 1, 0);
	glVertex2i(360 + 100 * cos(DEG_TO_RAD*theta), 250 + 100 * sin(DEG_TO_RAD*theta));
	glColor4f(1, 1, 1, 1);
	glVertex2i(370 + 100 * cos(DEG_TO_RAD*theta) + cos(theta) * 5, 250 + 100 * sin(DEG_TO_RAD*theta));
	glVertex2i(380 + 100 * cos(DEG_TO_RAD*theta) - cos(theta) * 5, 250 + 100 * sin(DEG_TO_RAD*theta));
	glColor4f(1, 1, 1, 0);
	glVertex2i(390 + 100 * cos(DEG_TO_RAD*theta), 250 + 100 * sin(DEG_TO_RAD*theta));
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor4f(1, 1, 1, 0);
	glVertex2i(375 + 100 * cos(DEG_TO_RAD*theta), 235 + 100 * sin(DEG_TO_RAD*theta));
	glColor4f(1, 1, 1, 1);
	glVertex2i(375 + 100 * cos(DEG_TO_RAD*theta), 245 + 100 * sin(DEG_TO_RAD*theta) + cos(theta) * 5);
	glVertex2i(375 + 100 * cos(DEG_TO_RAD*theta), 255 + 100 * sin(DEG_TO_RAD*theta) - cos(theta) * 5);
	glColor4f(1, 1, 1, 0);
	glVertex2i(375 + 100 * cos(DEG_TO_RAD*theta), 265 + 100 * sin(DEG_TO_RAD*theta));
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor4f(1, 1, 1, 0);
	glVertex2i(365 + 100 * cos(DEG_TO_RAD*theta), 240 + 100 * sin(DEG_TO_RAD*theta));
	glColor4f(1, 1, 1, 1);
	glVertex2i(370 + 100 * cos(DEG_TO_RAD*theta) + cos(theta) * 3, 245 + 100 * sin(DEG_TO_RAD*theta) + cos(theta) * 3);
	glVertex2i(380 + 100 * cos(DEG_TO_RAD*theta) - cos(theta) * 3, 255 + 100 * sin(DEG_TO_RAD*theta) - cos(theta) * 3);
	glColor4f(1, 1, 1, 0);
	glVertex2i(385 + 100 * cos(DEG_TO_RAD*theta), 260 + 100 * sin(DEG_TO_RAD*theta));
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor4f(1, 1, 1, 0);
	glVertex2i(385 + 100 * cos(DEG_TO_RAD*theta), 240 + 100 * sin(DEG_TO_RAD*theta));
	glColor4f(1, 1, 1, 1);
	glVertex2i(380 + 100 * cos(DEG_TO_RAD*theta) - cos(theta) * 3, 245 + 100 * sin(DEG_TO_RAD*theta) + cos(theta) * 3);
	glVertex2i(370 + 100 * cos(DEG_TO_RAD*theta) + cos(theta) * 3, 255 + 100 * sin(DEG_TO_RAD*theta) - cos(theta) * 3);
	glColor4f(1, 1, 1, 0);
	glVertex2i(365 + 100 * cos(DEG_TO_RAD*theta), 260 + 100 * sin(DEG_TO_RAD*theta));
	glEnd();

	//This is the part where 'sparks' is enabled
	if (id == 1) {

		glBegin(GL_POLYGON);
		glColor3f(0.7, 0.7, 0.7);
		glVertex2i(276, 24);
		glVertex2i(396, 24);
		glVertex2i(396, 64);
		glVertex2i(276, 64);
		glEnd();

		glColor3f(0.4, 0.4, 0.4);
		glRasterPos2i(300, 35);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, "Sparks");

		//generate a randome number to display the corresponding sparks pattern
		int num = (rand() % (100 - 0 + 1)) + 0;

		//if a spark is choosen, starts its IDLE and animation
		//After the animation is done, reset the IDLE for the next time when it is choosen
		if (num == 0 && traq1 == 0) {
			traq1 += 0.01;
		}
		if (traq1 != 0) {
			sparksOne();
		}
		if (traq1 > 1) {
			traq1 = 0;
		}

		if (num == 1 && traq2 == 0) {
			traq2 += 0.01;
		}
		if (traq2 != 0) {
			sparksTwo();
		}
		if (traq2 > 1) {
			traq2 = 0;
		}

		if (num == 2 && traq3 == 0) {
			traq3 += 0.01;
		}
		if (traq3 != 0) {
			sparksThree();
		}
		if (traq3 > 1) {
			traq3 = 0;
		}

		if (num == 3 && traq4 == 0) {
			traq4 += 0.01;
		}
		if (traq4 != 0) {
			sparksFour();
		}
		if (traq4 > 1) {
			traq4 = 0;
		}

		if (num == 4 && traq5 == 0) {
			traq5 += 0.01;
		}
		if (traq5 != 0) {
			sparksFive();
		}
		if (traq5 > 1) {
			traq5 = 0;
		}

		if (num == 5 && traq6 == 0) {
			traq6 += 0.01;
		}
		if (traq6 != 0) {
			sparksSix();
		}
		if (traq6 > 1) {
			traq6 = 0;
		}

		if (num == 6 && traq7 == 0) {
			traq7 += 0.01;
		}
		if (traq7 != 0) {
			sparksSeven();
		}
		if (traq7 > 1) {
			traq7 = 0;
		}

		if (num == 7 && traq8 == 0) {
			traq8 += 0.01;
		}
		if (traq8 != 0) {
			sparksEight();
		}
		if (traq8 > 1) {
			traq8 = 0;
		}

		if (num == 8 && traq9 == 0) {
			traq9 += 0.01;
		}
		if (traq9 != 0) {
			sparksNine();
		}
		if (traq9 > 1) {
			traq9 = 0;
		}

		if (num == 9 && traq10 == 0) {
			traq10 += 0.01;
		}
		if (traq10 != 0) {
			sparksTen();
		}
		if (traq10 > 1) {
			traq10 = 0;
		}
	}

	glutSwapBuffers();
}

//Animation for every sparks
//Following ten functions share the same structure, but the color, speed, movement angle is different for every spark
void sparksOne(void) {
	//get current sparkle postion
	//Same for the rest of sparks
	int temx = 375 + 100 * cos(DEG_TO_RAD*theta);
	int temy = 250 + 100 * sin(DEG_TO_RAD*theta);

	oneIdle();

	glBegin(GL_LINE_STRIP);
	glColor4f(0, 1, 1, 1);
	glVertex2i(temx + traq1 * 400, temy + traq1 * 400);
	glColor4f(1, 1, 1, 0);
	glVertex2i(temx + traq1 * 400 - 100, temy + traq1 * 400 - 100);
	glEnd();

}

void sparksTwo(void) {
	int temx = 375 + 100 * cos(DEG_TO_RAD*theta);
	int temy = 250 + 100 * sin(DEG_TO_RAD*theta);

	twoIdle();

	glBegin(GL_LINE_STRIP);
	glColor4f(0, 1, 0, 1);
	glVertex2i(temx - traq2 * 400, temy + traq2 * 400);
	glColor4f(1, 1, 1, 0);
	glVertex2i(temx - traq2 * 400 + 100, temy + traq2 * 400 - 100);
	glEnd();

}

void sparksThree(void) {
	int temx = 375 + 100 * cos(DEG_TO_RAD*theta);
	int temy = 250 + 100 * sin(DEG_TO_RAD*theta);

	threeIdle();

	glBegin(GL_LINE_STRIP);
	glColor4f(1, 0, 0, 1);
	glVertex2i(temx + traq3 * 400, temy + traq3 * 200);
	glColor4f(1, 1, 1, 0);
	glVertex2i(temx + traq3 * 400 - 100, temy + traq3 * 200 - 50);
	glEnd();

}

void sparksFour(void) {
	int temx = 375 + 100 * cos(DEG_TO_RAD*theta);
	int temy = 250 + 100 * sin(DEG_TO_RAD*theta);

	fourIdle();

	glBegin(GL_LINE_STRIP);
	glColor4f(0, 0, 1, 1);
	glVertex2i(temx + traq4 * 400, temy + traq4 * 800);
	glColor4f(1, 1, 1, 0);
	glVertex2i(temx + traq4 * 400 - 50, temy + traq4 * 800 - 100);
	glEnd();

}

void sparksFive(void) {
	int temx = 375 + 100 * cos(DEG_TO_RAD*theta);
	int temy = 250 + 100 * sin(DEG_TO_RAD*theta);

	fiveIdle();

	glBegin(GL_LINE_STRIP);
	glColor4f(1, 0, 1, 1);
	glVertex2i(temx - traq5 * 400, temy + traq5 * 800);
	glColor4f(1, 1, 1, 0);
	glVertex2i(temx - traq5 * 400 + 50, temy + traq5 * 800 - 100);
	glEnd();

}

void sparksSix(void) {
	int temx = 375 + 100 * cos(DEG_TO_RAD*theta);
	int temy = 250 + 100 * sin(DEG_TO_RAD*theta);

	sixIdle();

	glBegin(GL_LINE_STRIP);
	glColor4f(1, 1, 1, 1);
	glVertex2i(temx + traq6 * 400, temy + traq6);
	glColor4f(1, 1, 1, 0);
	glVertex2i(temx + traq6 * 400 - 100, temy + traq6);
	glEnd();

}

void sparksSeven(void) {
	int temx = 375 + 100 * cos(DEG_TO_RAD*theta);
	int temy = 250 + 100 * sin(DEG_TO_RAD*theta);

	sevenIdle();

	glBegin(GL_LINE_STRIP);
	glColor4f(0.4, 0.5, 0.8, 1);
	glVertex2i(temx - traq7 * 400, temy - traq7 * 200);
	glColor4f(1, 1, 1, 0);
	glVertex2i(temx - traq7 * 400 + 100, temy - traq7 * 200 + 50);
	glEnd();

}

void sparksEight(void) {
	int temx = 375 + 100 * cos(DEG_TO_RAD*theta);
	int temy = 250 + 100 * sin(DEG_TO_RAD*theta);

	eightIdle();

	glBegin(GL_LINE_STRIP);
	glColor4f(1, 0.5, 0.5, 1);
	glVertex2i(temx, temy - traq8 * 400);
	glColor4f(1, 1, 1, 0);
	glVertex2i(temx, temy - traq8 * 400 + 100);
	glEnd();

}

void sparksNine(void) {
	int temx = 375 + 100 * cos(DEG_TO_RAD*theta);
	int temy = 250 + 100 * sin(DEG_TO_RAD*theta);

	nineIdle();

	glBegin(GL_LINE_STRIP);
	glColor4f(0.3, 0.5, 1, 1);
	glVertex2i(temx + traq9 * 400, temy - traq9 * 400);
	glColor4f(1, 1, 1, 0);
	glVertex2i(temx + traq9 * 400 - 100, temy - traq9 * 400 + 100);
	glEnd();

}

void sparksTen(void) {
	int temx = 375 + 100 * cos(DEG_TO_RAD*theta);
	int temy = 250 + 100 * sin(DEG_TO_RAD*theta);

	tenIdle();

	glBegin(GL_LINE_STRIP);
	glColor4f(0.4, 0.2, 1, 1);
	glVertex2i(temx - traq10 * 400, temy - traq10 * 400);
	glColor4f(1, 1, 1, 0);
	glVertex2i(temx - traq10 * 400 + 100, temy - traq10 * 400 + 100);
	glEnd();

}
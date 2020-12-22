/*******************************************************
   Author:    Bochao Zhang
   ID:        B00748967
   Project:   Final Project of CSCI3161
   Date:      2020-11-28
   Note:      Using 'CRT_SECURE_NO_DPRECATE' to enable
			  some basic functions
********************************************************/

#define _CRT_SECURE_NO_WARNINGS

//Library sets
#include <freeglut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> 
#include <string.h>

//All Method Used
void initializeGL(void);
void reshape(int w, int h);
void mainIdle();
void usrInput(unsigned char key, int xmouse, int ymouse);
void keySpecial(int key, int x, int y);
void mouseMove(int x, int y);
void meshReader();
GLuint textureLoader(const char * filename, int w, int h);
void display();
void grid();
void sence();
void mountain();
void mGenerator();
void cessna();
void propeller(); 
void games();
void record();

//Basic variables
GLfloat theta = 0.0;
GLfloat alpha = 0.0;
int grid_length = 20;
int grid_rnum = 100;
float scale = 5.0;

//All movement coordinates and control
int width;
int height;
int posx;
int posy;
float speed = 0.5;
float senst = 0.01;
float movx;
float movy;
float movz;
float angle;

//All static colors
static GLfloat blue[] = {0,0,1,1};
static GLfloat black[] = { 0,0,0,1 };
static GLfloat purple[] = { 1,0,1,1 };
static GLfloat yellow[] = { 1,1,0,1 };
static GLfloat white[] = { 1,1,1,1 };
static GLfloat grey[] = { 0.5,0.5,0.5,1 };
static GLfloat mint[] = { 0.3,0.1,0.8,1 };
static GLfloat green[] = { 0.3,0.6,0.3,1 };

//Switches
bool id_polymode = true;
int id_fullwd = 0;
bool id_mode = true;
bool id_fog = false;
bool id_mt = false;
bool id_txt = false;
bool id_game = false;

//Mesh for athe plane
float mesh_verts[6763][3];
float mesh_norms[6763][3];
int mesh_faces[33][500][20];
int mesh_pars[2][500][20];

//Mesh of mountains
float mountain_vets[65][65][3]; 
float mountain_vets1[65][65][3];
float mountain_vets2[65][65][3];

//Gaming Mechanics
int game_mac[10][3];
GLUquadric* gbj[10];
int nuang[10];
int score=0;

//Main idle function for al animation
void mainIdle() {
	theta += 20;
	if (theta >= 360.0) {
		theta -= 360.0;
	}

	float ratiox = (posx - (width / 2.0)) / (width / 2.0);
	float ratioy = (posy - (height / 2.0)) / (height / 2.0);

	alpha += ratiox*senst;
	if (alpha >=360.0) {
		alpha = 0;
	}
	else if (alpha<=-360) {
		alpha = 0;
	}

	movx+=speed*cos(alpha);
	movy+=speed*-(sin(alpha));
	movz += (speed/2)*ratioy;
	angle += ratiox*0.57;

	glutPostRedisplay(); 
}

//Initializer for this program
void initializeGL(void)
{
	//Set background to black color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0.0);

	//Set default drawing color to black
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(4.0);

	GLfloat diffuse[] = {1,1,1,1};
	GLfloat ambient[] = { 1,1,1,1 };
	GLfloat specular[] = { 1,1,1,1 };

	//Enable alpha channel
	//glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);

	glLightfv(GL_FRONT, GL_AMBIENT, ambient);
	glLightfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glLightfv(GL_FRONT, GL_SPECULAR, specular);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();
}

//Camera setings
void reshape(int w, int h) {
	width = w;
	height = h;

	//View port
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Camera perspective
	gluPerspective(50.0, w / (GLfloat)h, 3.0, 5000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Initial camera postion and angle
	gluLookAt(40, 0, 7, 0, 0, 7, 0, 0, 1);
}

void usrInput(unsigned char key, int xmouse, int ymouse) {
	//Toogle Line Render
	if (key == 'w') {
		id_polymode = !id_polymode;
	}
	//Toggle Full Screen
	if (key == 'f') {
		id_fullwd += 1;
	}
	//Quit
	if (key == 'q') {
		exit(1);
	}
	//Toggle sence
	if (key == 's') {
		id_mode = !id_mode;
	}
	//Toggle Fog
	if (key == 'b') {
		id_fog = !id_fog;
	}
	//Toggle Moutain
	if (key == 'm') {
		id_mt = !id_mt;
	}
	//Toggle Mountain Texture
	if (key =='t') {
		id_txt = !id_txt;
	}
	//Start Game
	if (key == 'g') {
		id_game = !id_game;
	}
}

//Adjust moving speed
void keySpecial(int key, int x, int y) {
	if (key == GLUT_KEY_PAGE_UP) {
		speed += 0.1;
		if (speed>=5) {
			speed = 5;
		}
	}
	if (key == GLUT_KEY_PAGE_DOWN) {
		speed -= 0.1;
		if (speed<=0.5) {
			speed = 0.5;
		}
	}
}

//Mouse movement
void mouseMove(int x, int y){
	posx = x;
	posy = y;
}

//Read plane mesh
void meshReader() {
	//Star to read file
	char c[500];
	FILE *file;
	char *tk;

	//Counter
	int counter_verts = 0;
	int counter_norms = 0;

	int counter_objs = 0;
	int counter_faces = 0;
	int counter_pots = 0;

	file = fopen("cessna.txt", "r");
	if (file == NULL) {
		fprintf(stdout, "Error");
		exit(1);
	}
	//Following3 if statement is vertex, normals and faces
	while (fgets(c, sizeof(c), file) != NULL) {
		tk = strtok(c, " ");
		if (strcmp(tk,"v") == 0) {
			tk = strtok(NULL, " ");
			mesh_verts[counter_verts][0] = atof(tk);
			tk = strtok(NULL, " ");
			mesh_verts[counter_verts][1] = atof(tk);
			tk = strtok(NULL, " ");
			mesh_verts[counter_verts][2] = atof(tk);
			counter_verts++;
		}
		else if (strcmp(tk, "n") == 0) {
			tk = strtok(NULL, " ");
			mesh_norms[counter_norms][0] = atof(tk);
			tk = strtok(NULL, " ");
			mesh_norms[counter_norms][1] = atof(tk);
			tk = strtok(NULL, " ");
			mesh_norms[counter_norms][2] = atof(tk);
			counter_norms++;
		}
		else if (strcmp(tk, "g") == 0 || strcmp(tk, "f") == 0) {
			if (strcmp(tk, "g") == 0) {
				counter_objs++;
				counter_faces = 0;
				continue;
			}
			else{
				counter_pots = 0;
				tk = strtok(NULL, " ");
				while (tk != NULL) {
					mesh_faces[counter_objs - 1][counter_faces][counter_pots] = atoi(tk);
					counter_pots++;
					tk = strtok(NULL, " ");
				}
				counter_faces++;
			}
		}
	}

	//Testing the Reader
	/*for (int i = 0; i < 33;i++) {
		for (int j = 0; j < 500;j++) {
			for (int k = 0; k < 20;k++) {
				fprintf(stdout,"%d ",mesh_faces[23][j][k]);
			}
			fprintf(stdout, "\n");
		}
		fprintf(stdout, "\n");
	}*/
	/*for (int i = 0; i < 6763;i++) {
		for (int j = 0; j < 3;j++) {
			if (mesh_verts[i][j]<0) {
				fprintf(stdout, "%f ", mesh_verts[i][j]);
			}
		}
		fprintf(stdout, "\n");
	}*/
	/*for (int i = 0; i < 6763;i++) {
		for (int j = 0; j < 3;j++) {
			fprintf(stdout, "%f ", mesh_norms[i][j]);
		}
		fprintf(stdout, "\n");
	}*/

	fclose(file);

	//Read propller mesh
	file = fopen("propeller.txt","r");
	if (file == NULL) {
		fprintf(stdout, "Error");
		exit(1);
	}

	counter_objs = 0;
	counter_faces = 0;
	counter_pots = 0;

	//Same way as above
	while (fgets(c, sizeof(c), file) != NULL) {
		tk = strtok(c, " ");
	    if (strcmp(tk, "g") == 0 || strcmp(tk, "f") == 0) {
		    if (strcmp(tk, "g") == 0) {
			    counter_objs++;
			    counter_faces = 0;
			    continue;
		    }
		    else {
			    counter_pots = 0;
			    tk = strtok(NULL, " ");
			    while (tk != NULL) {
				    mesh_pars[counter_objs - 1][counter_faces][counter_pots] = atoi(tk);
				    counter_pots++;
				    tk = strtok(NULL, " ");
			    }
			    counter_faces++;
		    }
	    }
	}

	fclose(file);
}

//Texture Reader
GLuint textureLoader(const char * filename,int w,int h) {
	//Create empty texture
	GLuint texture;
	int txt_width, txt_height;

	//Read file
	FILE * file;
	file = fopen(filename, "r");

	if (file == NULL) {
	    fprintf(stdout, "Error");
	    exit(1);
    }
	txt_width = w;
	txt_height = h;
	int size = 3 * txt_width*txt_height;

	//Scan the file
	unsigned char* data = (unsigned char*)malloc(size);
	fread(data, size, 1, file);
	fclose(file);
	
	//Switch red and blue data, they are reversed in bmp
	for (int i = 0; i < size; i+=3){
		char bd, rd;
		bd = data[i];
		rd = data[i + 2];

		data[i] = rd;
		data[i + 2] = bd;
	}

	//Build up the texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, txt_width, txt_height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
	return texture;
}

//Main Display Function
void display() {
	//Clear Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (id_polymode == true) { 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (id_polymode == false) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	//Change between window and full screen
	if (id_fullwd == 1) {
		glutFullScreen();
		id_fullwd += 1;
	}
	else if (id_fullwd == 3) {
		glutPositionWindow(100,150);
		glutReshapeWindow(1000,700);
		id_fullwd = 0;
	}

	//Cemera mocment ratio (Perspectively)
	float ratiox = (posx - (width / 2.0)) / (width / 2.0);
	float ratioy = (posy - (height / 2.0)) / (height / 2.0);

	//Moving Sence
	glPushMatrix();
	glRotatef(angle, 0, 0, 1);
	glPushMatrix();
	glTranslatef(movx, movy, movz);
	glDisable(GL_LIGHTING);
	if(id_game) games();
	if(id_mode) grid();
	else sence();
	if(id_mt) mountain();
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPopMatrix();

	//Construct Plane Mesh
	glPushMatrix();
	glRotatef(ratiox*-20,1,0,0);
	glRotatef(ratioy*-3, 0, 1, 0);
	glPushMatrix();
	glTranslatef(30,0,5);
	glRotatef(90,1, 0, 0);

	//Place lightning
	glEnable(GL_LIGHTING);
	GLfloat position[] = { -5,6,4,0.25 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	cessna();
	propeller();
	record();

	glDisable(GL_LIGHTING);
	glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
}

//Display gird function
void grid() {
	//Set up basic setting like line width
	glLineWidth(1);
	glPushMatrix();
	glColor3f(1, 1, 1);
	GLUquadric* qobj1 = gluNewQuadric();
	glTranslatef(0, 0, 0);
	gluSphere(qobj1, 0.2, 10, 10);
	glPopMatrix();

	//Build up the grid
	int stp = grid_rnum / 2;
	for (int i = -(grid_rnum/2)*grid_length; i < (grid_rnum/2)*grid_length; i+=grid_length) {
		for (int j = -(grid_rnum / 2)*grid_length; j < (grid_rnum / 2)*grid_length; j += grid_length) {
			glPushMatrix();
			glColor3f(0.4, 0.3, 0.8);
			glBegin(GL_POLYGON);
			glVertex3f(i, j, 0);
			glVertex3f(i+grid_length, j, 0);
			glVertex3f(i+grid_length, j+grid_length, 0);
			glVertex3f(i, j+grid_length, 0);
			glEnd();
			glPopMatrix();
		}
	}

	//Do not render x,y,z axis signs
	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0.05);
	glVertex3f(grid_length*1.2, 0, 0.05);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0.05);
	glVertex3f(0, grid_length*1.2, 0.05);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, grid_length*1.2);
	glEnd();

	glLineWidth(1);
}

//Construct the sence
void sence() {
	//Reader Sea texture
	GLuint texture;
	texture = textureLoader("sea02.bmp",200,150);

	//Togglt fog and the distance settings
	if (id_fog) glEnable(GL_FOG);
	else glDisable(GL_FOG);
	GLfloat fogColor[4] = { 1,0.6,0.6,1 };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.0003);

	//Apply the texture to the sea (Disk)
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glColor3f(1, 1, 1);
	GLUquadric* dobj = gluNewQuadric();
	gluQuadricTexture(dobj, texture);
	gluDisk(dobj, 0, 2000, 30,40);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_FOG);

	//Apply texture to Sky
	texture = textureLoader("sky08.bmp", 300, 128);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(0,0,-50);
	glColor3f(1, 1, 1);
	GLUquadric* cobj = gluNewQuadric();
	gluQuadricTexture(cobj, texture);
	gluCylinder(cobj, 1950, 1950, 2000, 40,50);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//Mountain
void mountain() {
	//Read mountain texture
	glEnable(GL_LIGHTING);
	GLuint texture;
	texture = textureLoader("mount03.bmp", 200, 172);

	glTranslatef(-500, 200, -20);
	
	//Mountan #1
	glPushMatrix(); 
	GLfloat position[] = { -5,6,4,0.20 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	//Giving material and lighting and so on
	//Same as the rest two mountain
	glColor3f(1, 1, 1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey);

	glTranslatef(0, -320, 10);

	if(id_txt) glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64;j++) {
			glBegin(GL_POLYGON);
			glColor3f(0,1,0);
			for (int k = 0; k < 4; k++) {
				if (id_txt == false) {
					float tprtl=mountain_vets[i][j][2] / mountain_vets[32][32][2];
					GLfloat curcol[] = { tprtl,1,tprtl,1 };
					glMaterialfv(GL_FRONT, GL_DIFFUSE, curcol);
				}
				glTexCoord2f(mountain_vets[i][j][0] /(65*6), mountain_vets[i][j][1] / (65 * 6));
				glVertex3f(mountain_vets[i][j][0], mountain_vets[i][j][1], mountain_vets[i][j][2]);

				glTexCoord2f(mountain_vets[i][j+1][0] / (65 * 6), mountain_vets[i][j+1][1] / (65 * 6));
				glVertex3f(mountain_vets[i][j+1][0], mountain_vets[i][j+1][1], mountain_vets[i][j+1][2]);

				glTexCoord2f(mountain_vets[i+1][j+1][0] / (65 * 6), mountain_vets[i+1][j+1][1] / (65 * 6));
				glVertex3f(mountain_vets[i+1][j+1][0], mountain_vets[i+1][j+1][1], mountain_vets[i+1][j+1][2]);

				glTexCoord2f(mountain_vets[i+1][j][0] / (65 * 6), mountain_vets[i+1][j][1] / (65 * 6));
				glVertex3f(mountain_vets[i+1][j][0], mountain_vets[i+1][j][1], mountain_vets[i+1][j][2]);
			}
			glEnd();
		}
	}
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	//Mountan #2
	glEnable(GL_LIGHTING);
	glTranslatef(0, 590,-20);

	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, position);


	glColor3f(1, 1, 1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey);

	glTranslatef(0, -320, 10);

	if (id_txt) glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			if (id_txt == false) {
				float tprtl = mountain_vets[i][j][2] / mountain_vets[32][32][2];
				GLfloat curcol[] = { tprtl,1,tprtl,1 };
				glMaterialfv(GL_FRONT, GL_DIFFUSE, curcol);
			}
			glBegin(GL_POLYGON);
			glColor3f(0, 1, 0);
			for (int k = 0; k < 4; k++) {
				glTexCoord2f(mountain_vets1[i][j][0] / (65 * 6), mountain_vets1[i][j][1] / (65 * 6));
				glVertex3f(mountain_vets1[i][j][0], mountain_vets1[i][j][1], mountain_vets1[i][j][2]);

				glTexCoord2f(mountain_vets1[i][j + 1][0] / (65 * 6), mountain_vets1[i][j + 1][1] / (65 * 6));
				glVertex3f(mountain_vets1[i][j + 1][0], mountain_vets1[i][j + 1][1], mountain_vets1[i][j + 1][2]);

				glTexCoord2f(mountain_vets1[i + 1][j + 1][0] / (65 * 6), mountain_vets1[i + 1][j + 1][1] / (65 * 6));
				glVertex3f(mountain_vets1[i + 1][j + 1][0], mountain_vets1[i + 1][j + 1][1], mountain_vets1[i + 1][j + 1][2]);

				glTexCoord2f(mountain_vets1[i + 1][j][0] / (65 * 6), mountain_vets1[i + 1][j][1] / (65 * 6));
				glVertex3f(mountain_vets1[i + 1][j][0], mountain_vets1[i + 1][j][1], mountain_vets1[i + 1][j][2]);
			}
			glEnd();
		}
	}
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);


	glEnable(GL_LIGHTING);
	glTranslatef(600, 0, 10);

	//Mountan #3
	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, position);


	glColor3f(1, 1, 1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey);

	glTranslatef(0, -320, 10);

	if (id_txt) glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			if (id_txt == false) {
				float tprtl = mountain_vets[i][j][2] / mountain_vets[32][32][2];
				GLfloat curcol[] = { tprtl,1,tprtl,1 };
				glMaterialfv(GL_FRONT, GL_DIFFUSE, curcol);
			}
			glBegin(GL_POLYGON);
			glColor3f(0, 1, 0);
			for (int k = 0; k < 4; k++) {
				glTexCoord2f(mountain_vets2[i][j][0] / (65 * 6), mountain_vets2[i][j][1] / (65 * 6));
				glVertex3f(mountain_vets2[i][j][0], mountain_vets2[i][j][1], mountain_vets2[i][j][2]);

				glTexCoord2f(mountain_vets2[i][j + 1][0] / (65 * 6), mountain_vets2[i][j + 1][1] / (65 * 6));
				glVertex3f(mountain_vets2[i][j + 1][0], mountain_vets2[i][j + 1][1], mountain_vets2[i][j + 1][2]);

				glTexCoord2f(mountain_vets2[i + 1][j + 1][0] / (65 * 6), mountain_vets2[i + 1][j + 1][1] / (65 * 6));
				glVertex3f(mountain_vets2[i + 1][j + 1][0], mountain_vets2[i + 1][j + 1][1], mountain_vets2[i + 1][j + 1][2]);

				glTexCoord2f(mountain_vets2[i + 1][j][0] / (65 * 6), mountain_vets2[i + 1][j][1] / (65 * 6));
				glVertex3f(mountain_vets2[i + 1][j][0], mountain_vets2[i + 1][j][1], mountain_vets2[i + 1][j][2]);
			}
			glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPopMatrix();
}

//Plane Mesh
void cessna() {
	glColor3f(1, 1, 1);

	//Giving material to the mesh
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 80.0f);
    glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
	for (int i = 0; i < 33; i++) {
		if (i == 0) glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
		if (i == 4) glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
		if (i == 6) glMaterialfv(GL_FRONT, GL_DIFFUSE, purple);
		if (i == 7) glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
		if (i == 8) glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
		if (i == 14) glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
		if (i == 26) glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
		//Read the mesh from the array givien by the mesh reader
		for (int j = 0; j < 500; j++) {
			glBegin(GL_POLYGON);
			for (int k = 0; k < 20; k++) {
				if (mesh_faces[i][j][k]!=0) {
					glNormal3f(mesh_norms[mesh_faces[i][j][k] - 1][0], mesh_norms[mesh_faces[i][j][k] - 1][1], mesh_norms[mesh_faces[i][j][k] - 1][2]);
					glVertex3f(mesh_verts[mesh_faces[i][j][k]-1][0]*scale, mesh_verts[mesh_faces[i][j][k]-1][1]*scale, mesh_verts[mesh_faces[i][j][k]-1][2]*scale);
				}
			}
			glEnd();
		}
	}
}

//Two propellers
void propeller() {
	//Build up mesh
	glPushMatrix();
	glTranslatef(0, -0.15*scale, 0.35*scale);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 500; j++) {
			glPushMatrix();
			//Rotation animation same as the following one
			glRotatef(theta, 1, 0, 0);
			glTranslatef(0, 0.15*scale, -0.35*scale);
			glBegin(GL_POLYGON);
			for (int k = 0; k < 20; k++) {
				if (mesh_pars[i][j][k] != 0) {
					glNormal3f(mesh_norms[mesh_pars[i][j][k] - 1][0], mesh_norms[mesh_pars[i][j][k] - 1][1], mesh_norms[mesh_pars[i][j][k] - 1][2]);
					glVertex3f(mesh_verts[mesh_pars[i][j][k] - 1][0] * scale, mesh_verts[mesh_pars[i][j][k] - 1][1] * scale, mesh_verts[mesh_pars[i][j][k] - 1][2] * scale);
				}
			}
			glEnd();
			glPopMatrix();
		}
	}
	glPopMatrix();

	//Propeller #2
	glPushMatrix();
	glTranslatef(0, -0.15*scale, -0.35*scale);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 500; j++) {
			glPushMatrix();
			glRotatef(theta, 1, 0, 0);
			glTranslatef(0, 0.15*scale, 0.35*scale);
			glBegin(GL_POLYGON);
			for (int k = 0; k < 20; k++) {
				if (mesh_pars[i][j][k] != 0) {
					glNormal3f(mesh_norms[mesh_pars[i][j][k] - 1][0], mesh_norms[mesh_pars[i][j][k] - 1][1], mesh_norms[mesh_pars[i][j][k] - 1][2]);
					glVertex3f(mesh_verts[mesh_pars[i][j][k] - 1][0] * scale, mesh_verts[mesh_pars[i][j][k] - 1][1] * scale, -(mesh_verts[mesh_pars[i][j][k] - 1][2]) * scale);
				}
			}
			glEnd();
			glPopMatrix();
		}
	}
	glPopMatrix();
}

//Randomness Generator
void mGenerator() {
	int msc = 6;
	//Generates Vertexs for mountain #1
	for (int i = 0; i < 65; i++) {
		for (int j = 0; j < 65;j++) {
			mountain_vets[i][j][0] = -(32.0) * msc + i * msc;
			mountain_vets[i][j][1] = -(32.0) * msc + j * msc;
			mountain_vets[i][j][2] = 0;
		}
	}

	float ape = 0.5;
	float hm = 1;
	float temp = 0;
	for (int k = 1; k < 32; k++) {
		ape = ape * 13 / 12;
		temp = hm;
		for (int i = k; i <= 64 - k; i++) {
			hm += ape * ((rand() % 10001) / 10000.0);
			mountain_vets[k][i][2]=hm ;
			hm = temp;

			hm += ape * ((rand() % 10001) / 10000.0);
			mountain_vets[64-k][i][2] = hm;
			hm = temp;

			hm += ape * ((rand() % 10001) / 10000.0);
			mountain_vets[i][64 - k][2] = hm;
			hm = temp;

			hm += ape * ((rand() % 10001) / 10000.0);
			mountain_vets[i][k][2] = hm ;
			hm = temp;
		}
		hm = hm + ape;
	}
	mountain_vets[32][32][2]=hm;

	//Generates Vertexs for mountain #2
	msc = 4;
	for (int i = 0; i < 65; i++) {
		for (int j = 0; j < 65; j++) {
			mountain_vets1[i][j][0] = -(32.0) * msc + i * msc;
			mountain_vets1[i][j][1] = -(32.0) * msc + j * msc;
			mountain_vets1[i][j][2] = 0;
		}
	}

	ape = 5;
	hm = 1;
	temp = 0;
	for (int k = 1; k < 32; k++) {
		ape = ape * 11 / 12;
		temp = hm;
		for (int i = k; i <= 64 - k; i++) {
			hm += ape * ((rand() % 10001) / 10000.0);
			mountain_vets1[k][i][2] = hm;
			hm = temp;

			hm += ape * ((rand() % 10001) / 10000.0);
			mountain_vets1[64 - k][i][2] = hm;
			hm = temp;

			hm += ape * ((rand() % 10001) / 10000.0);
			mountain_vets1[i][64 - k][2] = hm;
			hm = temp;

			hm += ape * ((rand() % 10001) / 10000.0);
			mountain_vets1[i][k][2] = hm;
			hm = temp;
		}
		hm = hm + ape;
	}
	mountain_vets1[32][32][2] = hm;

	//Generates Vertexs for mountain #3
	msc = 12;
	for (int i = 0; i < 65; i++) {
		for (int j = 0; j < 65; j++) {
			mountain_vets2[i][j][0] = -(32.0) * msc + i * msc;
			mountain_vets2[i][j][1] = -(32.0) * msc + j * msc;
			mountain_vets2[i][j][2] = 0;
		}
	}

	ape = 0.5;
	hm = 1;
	temp = 0;
	for (int k = 1; k < 32; k++) {
		ape = ape * 13 / 12;
		temp = hm;
		for (int i = k; i <= 64 - k; i++) {
			hm += ape * ((rand() % 10001) / 10000.0);
			mountain_vets2[k][i][2] = hm;
			hm = temp;

			hm += ape * ((rand() % 10001) / 10000.0);
			mountain_vets2[64 - k][i][2] = hm;
			hm = temp;

			hm += ape * ((rand() % 10001) / 10000.0);
			mountain_vets2[i][64 - k][2] = hm;
			hm = temp;

			hm += ape * ((rand() % 10001) / 10000.0);
			mountain_vets2[i][k][2] = hm;
			hm = temp;
		}
		hm = hm + ape;
	}
	mountain_vets2[32][32][2] = hm;

	//Gnerator the mesh of all the flying circles
	for (int i = 0; i < 10; i++) {
		game_mac[i][0] = -50- rand() % 20;
		game_mac[i][1] = 500-rand()%1000;
		game_mac[i][2] = 500 - rand() % 1000;
		nuang[i] = rand() % 360;
	}
}

//Gaming mechniques
void games() {
	//Create several disks for player
	for (int i = 0; i < 10; i++){
		glPushMatrix();
		glRotatef(90,0,1,0);
		glPushMatrix();
		glTranslatef(game_mac[i][0], game_mac[i][1], game_mac[i][2]);
		float w = (rand() + 1.0) / (RAND_MAX + 2.0);
		glRotatef(nuang[i], 1, 0, 0);
		glColor3f(0.7, 0, 0);
		gbj[i] = gluNewQuadric();
		gluDisk(gbj[i], 20, 30, 20, 5);
		glPopMatrix();
		glPopMatrix();
	}

	//Collision detection, and giving scores
	for (int i = 0; i < 10; i++) {
		float dist1 = sqrt((movy + game_mac[i][1])*(movy + game_mac[i][1]));
		float dist2 = sqrt((movx + game_mac[i][2])*(movx + game_mac[i][2]));
		float dist3 = sqrt((movz - game_mac[i][0])*(movz - game_mac[i][0]));
		if ( dist1<=10 && dist2<=10 && dist3<=10) {
			score += 1;
		}
	}
}

//Display player's score
void record() {
	//Render the score as text
	glDisable(GL_LIGHTING);
	glColor3f(1,1,1);
	glRasterPos3f(0,0,2);
	char* disr[20];
	sprintf(disr,"Score: %d",score);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,disr);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	//Display mode
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	//Initial window position and size
	glutInitWindowPosition(100, 150);
	glutInitWindowSize(1000, 700);
	glutCreateWindow("Project");

	//For random number generator
	srand(time(NULL));
	initializeGL();
	glutReshapeFunc(reshape);

	//Mesh reader
	meshReader();

	//Start randomness
	mGenerator();

	//Display all
	glutDisplayFunc(display);

	//Trigger user inputs
	glutKeyboardFunc(usrInput);
	glutPassiveMotionFunc(mouseMove);
	glutSpecialFunc(keySpecial);

	//Start all animation
	glutIdleFunc(mainIdle);

	//A guide to the program
	fprintf(stdout, "Scene Controls:\n");
	fprintf(stdout, "-------------------------\n");
	fprintf(stdout, "\nW: Switch Polygon Mode\n");
	fprintf(stdout, "F: Toggle Full Screen\n");
	fprintf(stdout, "Q: Quit\n");
	fprintf(stdout, "S: Switch Sence\n");
	fprintf(stdout, "B: Toggle Fog\n");
	fprintf(stdout, "M: Display Moutains\n");
	fprintf(stdout, "T: Display Moutain Texture\n");
	fprintf(stdout, "G: Start Game\n");
	fprintf(stdout, "\nCamera Controls:\n");
	fprintf(stdout, "-------------------------\n");
	fprintf(stdout, "\nMouse\tUp: Up");
	fprintf(stdout, "\nMouse\tDown: Down");
	fprintf(stdout, "\nMouse\tLeft: Left");
	fprintf(stdout, "\nMouse\tRight: Right\n");
	fprintf(stdout, "Up\tPage: Speed Up\n");
	fprintf(stdout, "Down\tPage: Speed Down\n");

	glutMainLoop();

	return 0;
}
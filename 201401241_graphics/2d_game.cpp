#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>
#include <ctime>
#include <string>
#include "SOIL/SOIL.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#endif

using namespace std;

#include <math.h>

// class Vector3D
void print(int x, int y,int z, char *string)
{
//set the position of the text in the window using the x and y coordinates
glRasterPos2f(x,y);
//get the length of the string to display

//loop to display character by character
for (int i = 0; string[i]!=0; i++)
{
glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);
}
};	
class Vector3D {
public:
	float x;								
	float y;				
	float z;									

	Vector3D() {									
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3D(float x, float y, float z)	{		
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3D& operator= (Vector3D v) {			
		x= v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	Vector3D operator+ (Vector3D v)	{			
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}

	Vector3D operator- (Vector3D v)	{			
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}

	Vector3D operator* (float value) {			
		return Vector3D(x * value, y * value, z * value);
	}

	Vector3D operator/ (float value) {			
		return Vector3D(x / value, y / value, z / value);
	}

	Vector3D& operator+= (Vector3D v) {			
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3D& operator-= (Vector3D v) {			
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3D& operator*= (float value) {		
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	Vector3D& operator/= (float value) {		
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}


	Vector3D operator- () {						
		return Vector3D(-x, -y, -z);
	}

	float length() {								
		return sqrtf(x*x + y*y + z*z);
	};			   		

};

			
GLuint eyes_open, wheel1, wheel2, troll;

void texParams1() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

GLuint loadImage1(string fileName) {
	GLuint tex_ID;

	tex_ID = SOIL_load_OGL_texture(
		fileName.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO
		| SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_MULTIPLY_ALPHA
		| SOIL_FLAG_COMPRESS_TO_DXT
		| SOIL_FLAG_DDS_LOAD_DIRECT
		| SOIL_FLAG_INVERT_Y
		);

	if( tex_ID > 0 ) {
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, tex_ID );

		return tex_ID;
	}
	else {
		cout << "Bad image" << endl;
		return 0;
	}
}

class Bird {
public:
	float r;				
	Vector3D pos;			
	Vector3D vel;			
		
	Bird(float x=0, float y=0, float z=0, float r=1) {
		this->r = r;
		this->pos.x = x;
		this->pos.y = y;
		this->pos.z = z;
		eyes_open = loadImage1(string("pictures/eyes_open.png"));
		
	}

	void draw() {
		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0.0f);
		glScalef(0.1*r, 0.1*r, 0.0f);

		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0.5f, 0.5f, 0.5f);
		for(int i=0;i<360;i++) {
			glVertex2f(10*cos(i*(3.14/180)),10*sin(i*(3.14/180)));
		}
		glEnd();

		
		glPushMatrix();
		glTranslatef(5.0f, 1.0f, 0.0f);
		glRotatef(10, 0.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0.94f, 0.9f, 0.7f);
		for(int i=0;i<360;i++) {
			glVertex2f(5*cos(i*(3.14/180)),7*sin(i*(3.14/180)));
		}
		glEnd();
		glPopMatrix();

		
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslatef(6.0f, 4.0f, 0.0f);
		if(vel.x == 0 || (pos.x < -15)) glBindTexture(GL_TEXTURE_2D, eyes_open);
		else glBindTexture(GL_TEXTURE_2D, eyes_open);
		texParams1();
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-3.0f, -3.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(3.0f, -3.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(3.0f, 3.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-3.0f, 3.0f, 0.0f);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		
		glPushMatrix();
		glRotatef(15, 0.0f, 0.0f, 1.0f);
		glTranslatef(10.0f, 0.0f, 0.0f);
		glColor3f(1.0f, 0.7f, 0.0f);
		glBegin(GL_TRIANGLES);
		glVertex2f(0.0f, 2.0f);
		glVertex2f(3.0f, 0.0f);
		glVertex2f(0.0f, -2.0f);
		glEnd();
		glPopMatrix();
		glPopMatrix();
	}
	
	void applyVel(Vector3D impulse) {
		this->vel += impulse;					
	}


};

class Target {
public:
	Vector3D pos;			
	float r;
	bool visible;
	Target(float x=0, float y=0, float z=0, float r=2) {
		this->pos.x = x;
		this->pos.y = y;
		this->pos.z = z;
		this->r = r;
		troll = loadImage1(string("pictures/pig1.png"));
		visible = true;
	}

	void draw() {
		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0.0f);
		glScalef(0.1*r, 0.1*r, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, troll);
		glBegin(GL_TRIANGLE_FAN);
		for(int i=0;i<360;i++) {
			glTexCoord2f(0.5+0.54*cos(i*(3.14/180)),0.5+0.54*sin(i*(3.14/180)));
			glVertex2f(10*cos(i*(3.14/180)),10*sin(i*(3.14/180)));
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
};

class Block {
public:
	Vector3D pos;			
	Vector3D vel;			
	float side;
	bool gravity;
	Block(float x=0, float y=0, float z=0, float s=4) {
		this->pos.x = x;
		this->pos.y = y;
		this->pos.z = z;
		this->side = s;
		bool gravity = false;
	}

	void draw() {
		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0.0);
		glColor3f(0.3f, 0.3f, 0.3f);
		glRectf(-2.0f, -2.0f, 2.0f, 2.0f);
		glPopMatrix();
	}
};

class Cannon {
public:
	float size, angle;
	Vector3D pos;			
	Vector3D vel;			
	Cannon(float x=0, float y=0, float z=0, float s = 2, float ang=-90) {
		this->angle = ang;
		this->size = s;
		this->pos.x = x;
		this->pos.y = y;
		this->pos.z = z;
		
	}

	void draw() {
		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0.0f);
		glScalef(0.2*size, 0.2*size, 0.0f);
		
		

		
		glPushMatrix();
		glRotatef(angle, 0.0f, 0.0f, 1.0f);
		glColor3f(0.4f, 0.4f, 0.4f);
		
		glBegin(GL_TRIANGLE_FAN);
		for(int i=0;i<360;i++) {
			glVertex2f(5*cos(i*(3.14/180)),5*sin(i*(3.14/180)));
		}
		glEnd();

		glPushMatrix();
		glTranslatef(0.0f, -5.0 - 0.2*5.0, 0.0f);
		glScalef(0.3f, 0.3f, 0.0f);
		glBegin(GL_TRIANGLE_FAN);
		for(int i=0;i<360;i++) {
			glVertex2f(5*cos(i*(3.14/180)),5*sin(i*(3.14/180)));
		}
		glEnd();
		glPopMatrix();

		
		glPushMatrix();
		glTranslatef(0,10,0);
		glBegin(GL_QUADS);
		glVertex2f(-5.0f, -10.0f);
		glVertex2f(5.0f, -10.0f);
		glVertex2f(5.0f, 10.0f);
		glVertex2f(-5.0f, 10.0f);
		glEnd();
		glPopMatrix();
		glPopMatrix();

		
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslatef(2.0f, -3.0f, -5.0f);
		glBindTexture(GL_TEXTURE_2D, wheel2);
		texParams1();
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-6.0f, -6.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(6.0f, -6.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(6.0f, 7.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-6.0f, 6.0f, 0.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		
		glPopMatrix();
	}

	void rotate(float ang) {
		this->angle += ang;
		if (angle < -105) angle = -105;
		if (angle > 0) angle = 0;
		draw();
	}
};


GLuint	texture[3];
float _angle, ang = 0;
Bird** birds;
Block** blocks;
Cannon* cannon;
Target* targ;
int no_box;
int turn = 0;
int minDist = 1 + 1; //r + side/2
float power=2;
int level=1;
int score;
Vector3D initPos = Vector3D(-33, -13, 1);
GLuint loadImage(string fileName);
void drawLevel1() {
	turn = 0;
	no_box = 2;
	birds = new Bird*[3];
	birds[2] = new Bird(-36,-15.5,1,1);
	birds[1] = new Bird(-33,-13,1.0,1);
	birds[0] = new Bird(-33, -15.5, 1,1);
	
	cannon = new Cannon(-30,-13,0,2);

	blocks = new Block*[10];
	blocks[0] = new Block(20,-14.5);
	blocks[1] = new Block(23,-10.5);

	targ = new Target(15,-15,0,1.5);
}

void drawLevel2() {
	turn = 0;
	no_box = 2;
	birds = new Bird*[3];
	birds[2] = new Bird(-36,-15.5,1,1);
	birds[1] = new Bird(-33,-13,1.0,1);
	birds[0] = new Bird(-33, -15.5, 1,1);
	
	cannon = new Cannon(-30,-13,0,2);

	blocks = new Block*[10];
	blocks[0] = new Block(10,-14.5);
	blocks[1] = new Block(20,-14.5);
	targ = new Target(15,-15,0,1.5);
}

void drawLevel3() {
	turn = 0;
	no_box = 3;
	birds = new Bird*[3];
	birds[2] = new Bird(-36,-15.5,1,1);
	birds[1] = new Bird(-33,-13,1.0,1);
	birds[0] = new Bird(-33, -15.5, 1,1);

	cannon = new Cannon(-30,-13,0,2);

	blocks = new Block*[10];
	blocks[0] = new Block(10,-14.5);
	blocks[1] = new Block(20,-14.5);
	blocks[2] = new Block(20,-10.5);
	targ = new Target(15,-15,0,1.5);

}

void drawLevel4() {
	turn = 0;
	no_box = 4;
	birds = new Bird*[3];
	birds[2] = new Bird(-36,-15.5,1,1);
	birds[1] = new Bird(-33,-13,1.0,1);
	birds[0] = new Bird(-33, -15.5, 1,1);
	
	cannon = new Cannon(-30,-13,0,2);

	blocks = new Block*[10];
	blocks[0] = new Block(10,-14.5);
	blocks[1] = new Block(20,-14.5);
	blocks[2] = new Block(20,-10.5);
	blocks[3] = new Block(10,-10.5);

	targ = new Target(15,-15,0,1.5);

}

void drawLevel5() {
	turn = 0;
	no_box = 5;
	birds = new Bird*[3];
	birds[2] = new Bird(-36,-15.5,1,1);
	birds[1] = new Bird(-33,-13,1.0,1);
	birds[0] = new Bird(-33, -15.5, 1,1);
	
	cannon = new Cannon(-30,-13,0,2);

	blocks = new Block*[10];
	blocks[0] = new Block(10,-14.5);
	blocks[1] = new Block(20,-14.5);
	blocks[2] = new Block(20,-10.5);
	blocks[3] = new Block(10,-10.5);
	blocks[4] = new Block(10,-6.5);

	targ = new Target(15,-15,0,1.5);
}

void texParams() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}
void LoadScenery() {
	glEnable(GL_TEXTURE_2D);
	
	glPushMatrix();
	glTranslatef(0.0f,13.0f,-150.0f);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	texParams();

    
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-200.0f, -50.0f, 0.0f);
	glTexCoord2f(3.0f, 0.0f);
	glVertex3f(200.0f, -50.0f, 0.0f);
	glTexCoord2f(3.0f, 1.0f);
	glVertex3f(200.0f, 50.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-200.0f, 50.0f, 0.0f);
	glEnd();
	glPopMatrix();

	
	glPushMatrix();
	glTranslatef(0.0f,-23.0f,-100.0f);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	texParams();

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-150.0f, -10.0f, 0.0f);
	glTexCoord2f(3.0f, 0.0f);
	glVertex3f(150.0f, -10.0f, 0.0f);
	glTexCoord2f(3.0f, 1.0f);
	glVertex3f(150.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-150.0f, 10.0f, 0.0f);
	glEnd();
	glPopMatrix();

	
	glPushMatrix();
	glTranslatef(0.0f,-19.0f,-50.0f);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	texParams();

	glBegin(GL_QUADS);				
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-50.0f, -2.5f, 0.0f);
	glTexCoord2f(3.0f, 0.0f);
	glVertex3f(50.0f, -2.5f, 0.0f);
	glTexCoord2f(3.0f, 1.0f);
	glVertex3f(50.0f, 2.5f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-50.0f, 2.5f, 0.0f);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	LoadScenery();

	glPushMatrix();
	
	glTranslatef(0.0f,0.0f,-50.0f);

	glPushMatrix();
	glTranslatef(-35,0,0);
	
	glScalef(10, 10, 0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glRectf(-0.5, -0.05, 0.5, 0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35,0,0);
	
	glScalef(10, 10, 0);
	glColor3f(0.8f, 0.5f, 0.3f);
	glRectf(-0.5, -0.05, -0.5 + power -1.5, 0.05);
	glPopMatrix();

	
	for(int i=0 ; i<3; i++)
		birds[i]->draw();

  	cannon->draw();

	for(int i=0 ; i<no_box ; i++) {
		if (blocks[i]->gravity == true) {
			blocks[i]->pos.y -= blocks[i]->side;
			blocks[i]->gravity = false;
		}
		blocks[i]->draw();
	}
	if(targ->visible == true)
		targ->draw();

	glPopMatrix();
	glutSwapBuffers();
}


GLuint loadImage(string fileName) {
	GLuint tex_ID;

	tex_ID = SOIL_load_OGL_texture(
		fileName.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO
		| SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_MULTIPLY_ALPHA
		| SOIL_FLAG_COMPRESS_TO_DXT
		| SOIL_FLAG_DDS_LOAD_DIRECT
		| SOIL_FLAG_INVERT_Y
		);

	if( tex_ID > 0 ) {
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, tex_ID );

		return tex_ID;
	}
	else {
		cout << "Bad image" << endl;
		return 0;
	}
}


void keyboard1(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		cout << "Score is : " << score << endl;
		exit(0);
		break;
	case ' ':
		birds[turn]->applyVel(Vector3D(power*cos((cannon->angle + 90)*(3.14/180)),power*sin((cannon->angle + 90)*(3.14/180)),0));
		//cout << 
		break;
	case 'r': 
		drawLevel1();
		
		break;
	case 'a':
			cannon->rotate(1);
		break;
	case 'b':
			cannon->rotate(-1);
		break;
	case 'f':
				if (power +0.1 > 2.5)
				power = 2.5;
			else
				power += 0.1;
		break;
	case 's':
				if (power-0.1 < 1.5)
				power = 1.5;
			else
				power -= 0.1;
		break;
	}

}

void keyboard2(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		if (power-0.1 < 1.5)
			power = 1.5;
		else
			power -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		if (power +0.1 > 2.5)
			power = 2.5;
		else
			power += 0.1;
		break;
	case GLUT_KEY_UP:
		cannon->rotate(1);
		break;
	case GLUT_KEY_DOWN:
		cannon->rotate(-1);
		break;
	}
}


void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			cannon->rotate(-1);
		}
		else if (button == GLUT_RIGHT_BUTTON){
			cannon->rotate(1);
		}
		else if (button == 3) {
			if (power +0.1 > 2.5)
				power = 2.5;
			else
				power += 0.1;
		}
		else if (button == 4) {
			if (power-0.1 < 1.5)
				power = 1.5;
			else
				power -= 0.1;
		}
	}
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void collision() {
	
	Vector3D x = birds[turn]->pos - targ->pos;
	float dist = x.length();
	if (dist < birds[turn]->r + targ->r) {
		score += 10;
		targ->visible = false;
	}

	for(int i=0 ; i<no_box ; i++) {
		
		float x, y,r, x1, x2, y1, y2;
		x = birds[turn]->pos.x;
		y = birds[turn]->pos.y;
		r = birds[turn]->r;
		x1 = blocks[i]->pos.x - blocks[i]->side/2;
		x2 = blocks[i]->pos.x + blocks[i]->side/2;
		y1 = blocks[i]->pos.y - blocks[i]->side/2;
		y2 = blocks[i]->pos.y + blocks[i]->side/2;
		
		if (x > x1 - birds[turn]->vel.x -r && x < x1 + birds[turn]->vel.x ) {
			if (y >= y1 && y <= y2) {
				birds[turn]->pos.x = x1 - r;
				blocks[i]->vel.x = 0.5 * birds[turn]->vel.x;
				birds[turn]->vel.x *= -0.5;
			}
		}
		
		if (x > x2 - birds[turn]->vel.x && x < x2 + r + birds[turn]->vel.x) {
			if (y >= y1 && y <= y2) {
				birds[turn]->pos.x = x2 + r;
				blocks[i]->vel.x = 0.5 * birds[turn]->vel.x;
				birds[turn]->vel.x *= -0.5;
			}
		}

		if (x >= x1 && x <= x2) {
			if (y > y1 -r - birds[turn]->vel.y && y < y1 + birds[turn]->vel.y) {
				birds[turn]->pos.y = y1 - r;
				
				birds[turn]->vel.y *= -0.5;
			}
		}

		if (x >= x1 && x <= x2) {
			if (y > y2 - birds[turn]->vel.y -10 && y < y2 + r + birds[turn]->vel.y) {
				birds[turn]->pos.y = y2 + r;
				
				birds[turn]->vel.y *= -0.5;
			}
		}
		blocks[i]->gravity = true;
		for (int j=0 ; j< no_box ; j++) {
			if(j==i)continue;
			
			if (fabs(blocks[i]->pos.x - blocks[j]->pos.x) < blocks[i]->side/2 + blocks[j]->side/2 && blocks[i]->pos.y - blocks[j]->pos.y <= 1e-5) {
				if(blocks[i]->vel.x != 0) {
					blocks[j]->vel.x = blocks[i]->vel.x;
					blocks[i]->vel.x = 0;
				}
				else {
					blocks[i]->vel.x = blocks[j]->vel.x;
					blocks[j]->vel.x = 0;
				}
			}
			
			
			
			if (blocks[i]->pos.y < -10.5) {
				blocks[i]->gravity = false;
				continue;
			}
			else {
				if (fabs(blocks[j]->pos.y - (blocks[i]->pos.y -4))<=1e-5 && fabs(blocks[i]->pos.x - blocks[j]->pos.x) <=4.001) {
					blocks[i]->gravity = false;
					continue;
				}
			}
		}
	}
}

void update(int value) {

	if (targ->visible == false) {
			level++;
			if (level == 2) {
				drawLevel2();
			}
			else if(level==3) {
				drawLevel3();
			}
			else if(level==4) {
				drawLevel4();
			}
			else if(level==5) {
				drawLevel5();
			}
			else {
				level = 1;
				drawLevel1();
			}
		}

	if (birds[turn]->vel.x == 0 && birds[turn]->vel.y == 0 && birds[turn]->pos.x != initPos.x && birds[turn]->pos.y != initPos.y) {
		turn++;
		if (turn > 2) {
		
			if (targ->visible == true) {
				 
				if (level == 1)
					drawLevel1();
				else if(level==2) {
				drawLevel2();
			}
			else if(level==3) {
				drawLevel3();
			}
			else if(level==4) {
				drawLevel4();
			}
				else
					drawLevel5();
			}
		}
		else {
			birds[turn]->pos = initPos;
		}
	}

	if (fabs(birds[turn]->vel.x) < 0.005) birds[turn]->vel.x = 0;
	if (fabs(birds[turn]->vel.y) < 0.005) birds[turn]->vel.y = 0;

	
	 
	if(!(birds[turn]->vel.x == 0 && birds[turn]->vel.x == 0))
		birds[turn]->vel.y -= 0.09;
	else
		birds[turn]->vel.y = 0;

	birds[turn]->pos.x += birds[turn]->vel.x;
	birds[turn]->pos.y += birds[turn]->vel.y;

	if (birds[turn]->pos.y < -15.5 ) {			
		birds[turn]->pos.y = -15.5;
		birds[turn]->vel.y *= -0.5;
		birds[turn]->vel.x *= 0.8;
	}
	
	collision();

	for(int i=0 ; i<no_box ; i++) {
	
		if (fabs(blocks[i]->vel.x) < 0.005) blocks[i]->vel.x = 0;
		if (fabs(blocks[i]->vel.y) < 0.005) blocks[i]->vel.y = 0;

		blocks[i]->pos.x += blocks[i]->vel.x;
		blocks[i]->pos.y += blocks[i]->vel.y;

		blocks[i]->vel.x *= 0.8;
		blocks[i]->vel.y *= 0.8;
	}

	birds[turn]->pos += birds[turn]->vel;
	glutTimerFunc(25, update, 0);
}

int main(int argc, char **argv) {


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	int windowWidth =w*2/3, windowHeight=h*2/3;
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((w-windowWidth)/2, (h-windowHeight)/2);

	glutCreateWindow("My Angry Birds!!");
	initRendering();


	
	texture[0] = loadImage(string("backgrounds/ab1.png"));
	texture[1] = loadImage(string("backgrounds/midground2.png"));
	texture[2] = loadImage(string("backgrounds/ground2.png"));

	drawLevel1();

	glutDisplayFunc(drawScene);
	print(0,0,0,"Score");

	glutIdleFunc(drawScene);
	glutKeyboardFunc(keyboard1);
	glutSpecialFunc(keyboard2);
	glutMouseFunc (mouse);
	glutReshapeFunc(handleResize);
	glutTimerFunc(10, update, 0);

	glutMainLoop();
	return 0;
}



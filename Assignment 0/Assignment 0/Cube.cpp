#include "Cube.h"

Cube::Cube(float width, float length, float height)
{
		mWidth = width;
		mLength = length;
		mHeight = height;

		init();
}

void Cube::init()
{
	mPosX = 0;
	mPosY = 0;
	mPosZ = 0;
}

void Cube::update()
{

}
	
void Cube::draw()
{
	float w = mWidth/2;
	float l = mLength/2;
	float h = mHeight/2;

	glLoadIdentity();
	glPushMatrix();
	glTranslatef(mPosX, mPosY, mPosZ);
	glRotatef(45.0f, 90.0f, 90.0f, 0.0f);
	glTranslatef(-mPosX, -mPosY, -mPosZ);
	glBegin(GL_QUADS);
		//Top face (y = 1.0f)    // Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(mPosX+w, mPosY+h, mPosZ-l);
		glVertex3f(mPosX-w, mPosY+h, mPosZ-l);
		glVertex3f(mPosX-w, mPosY+h, mPosZ+l);
		glVertex3f(mPosX+w, mPosY+h, mPosZ+l);
		//Bottom face (y = -1.0f)  // Orange
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(mPosX+w, mPosY-h, mPosZ+l);
		glVertex3f(mPosX-w, mPosY-h, mPosZ+l);
		glVertex3f(mPosX-w, mPosY-h, mPosZ-l);
		glVertex3f(mPosX+w, mPosY-h, mPosZ-l);
		// Front face  (z = 1.0f)
		glColor3f(1.0f, 0.0f, 0.0f);     // Red
		glVertex3f(mPosX+w, mPosY+h, mPosZ+l);
		glVertex3f(mPosX-w, mPosY+h, mPosZ+l);
		glVertex3f(mPosX-w, mPosY-h, mPosZ+l);
		glVertex3f(mPosX+w, mPosY-h, mPosZ+l);
		// Back face (z = -1.0f)
		glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
		glVertex3f(mPosX+w, mPosY-h, mPosZ-l);
		glVertex3f(mPosX-w, mPosY-h, mPosZ-l);
		glVertex3f(mPosX-w, mPosY+h, mPosZ-l);
		glVertex3f(mPosX+w, mPosY+h, mPosZ-l);
		// Left face (x = -1.0f)
		glColor3f(0.0f, 0.0f, 1.0f);     // Blue
		glVertex3f(mPosX-w, mPosY+h, mPosZ+l);
		glVertex3f(mPosX-w, mPosY+h, mPosZ-l);
		glVertex3f(mPosX-w, mPosY-h, mPosZ-l);
		glVertex3f(mPosX-w, mPosY-h, mPosZ+l);
		// Right face (x = 1.0f)
		glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
		glVertex3f(mPosX+w, mPosY+h, mPosZ-l);
		glVertex3f(mPosX+w, mPosY+h, mPosZ+l);
		glVertex3f(mPosX+w, mPosY-h, mPosZ+l);
		glVertex3f(mPosX+w, mPosY-h, mPosZ-l);
	glEnd();
	glPopMatrix();
}

/*glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -3.0f);
		glRotatef((float) glfwGetTime() * 50.f, 0.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
			//Top face (y = 1.0f)    // Green
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(size, size, -size);
			glVertex3f(-size, size, -size);
			glVertex3f(-size, size, size);
			glVertex3f(size, size, size);
			//Bottom face (y = -1.0f)  // Orange
			glColor3f(size, 0.5f, 0.0f);
			glVertex3f(size, -size, size);
			glVertex3f(-size, -size, size);
			glVertex3f(-size, -size, -size);
			glVertex3f(size, -size, -size);
			// Front face  (z = 1.0f)
			glColor3f(1.0f, 0.0f, 0.0f);     // Red
			glVertex3f( size,  size, size);
			glVertex3f(-size,  size, size);
			glVertex3f(-size, -size, size);
			glVertex3f( size, -size, size);
			// Back face (z = -1.0f)
			glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
			glVertex3f( size, -size, -size);
			glVertex3f(-size, -size, -size);
			glVertex3f(-size,  size, -size);
			glVertex3f( size,  size, -size);
			// Left face (x = -1.0f)
			glColor3f(0.0f, 0.0f, 1.0f);     // Blue
			glVertex3f(-size,  size,  size);
			glVertex3f(-size,  size, -size);
			glVertex3f(-size, -size, -size);
			glVertex3f(-size, -size,  size);
			// Right face (x = 1.0f)
			glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
			glVertex3f(size,  size, -size);
			glVertex3f(size,  size,  size);
			glVertex3f(size, -size,  size);
			glVertex3f(size, -size, -size);
		glEnd();*/
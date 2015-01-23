#include "Cube.h"

Cube::Cube(float width, float length, float height)
{
	mWidth = width;
	mLength = length;
	mHeight = height;

	//init();
	mPos = Vector3(0,0,0);
	mRotations = vector<Vector3>();
	mRotationAngles = vector<float>();
}

/*void Cube::init()
{
	//mPos = Vector3(0,0,0);
}*/

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
	applyRotations();
	glBegin(GL_QUADS);
		//Top face (y = 1.0f)    // Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(mPos.x+w, mPos.y+h, mPos.z-l);
		glVertex3f(mPos.x-w, mPos.y+h, mPos.z-l);
		glVertex3f(mPos.x-w, mPos.y+h, mPos.z+l);
		glVertex3f(mPos.x+w, mPos.y+h, mPos.z+l);
		//Bottom face (y = -1.0f)  // Orange
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(mPos.x+w, mPos.y-h, mPos.z+l);
		glVertex3f(mPos.x-w, mPos.y-h, mPos.z+l);
		glVertex3f(mPos.x-w, mPos.y-h, mPos.z-l);
		glVertex3f(mPos.x+w, mPos.y-h, mPos.z-l);
		// Front face  (z = 1.0f)
		glColor3f(1.0f, 0.0f, 0.0f);     // Red
		glVertex3f(mPos.x+w, mPos.y+h, mPos.z+l);
		glVertex3f(mPos.x-w, mPos.y+h, mPos.z+l);
		glVertex3f(mPos.x-w, mPos.y-h, mPos.z+l);
		glVertex3f(mPos.x+w, mPos.y-h, mPos.z+l);
		// Back face (z = -1.0f)
		glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
		glVertex3f(mPos.x+w, mPos.y-h, mPos.z-l);
		glVertex3f(mPos.x-w, mPos.y-h, mPos.z-l);
		glVertex3f(mPos.x-w, mPos.y+h, mPos.z-l);
		glVertex3f(mPos.x+w, mPos.y+h, mPos.z-l);
		// Left face (x = -1.0f)
		glColor3f(0.0f, 0.0f, 1.0f);     // Blue
		glVertex3f(mPos.x-w, mPos.y+h, mPos.z+l);
		glVertex3f(mPos.x-w, mPos.y+h, mPos.z-l);
		glVertex3f(mPos.x-w, mPos.y-h, mPos.z-l);
		glVertex3f(mPos.x-w, mPos.y-h, mPos.z+l);
		// Right face (x = 1.0f)
		glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
		glVertex3f(mPos.x+w, mPos.y+h, mPos.z-l);
		glVertex3f(mPos.x+w, mPos.y+h, mPos.z+l);
		glVertex3f(mPos.x+w, mPos.y-h, mPos.z+l);
		glVertex3f(mPos.x+w, mPos.y-h, mPos.z-l);
	glEnd();
	glPopMatrix();
}
#include "scene.h"

#include <glut.h>

#define _USE_MATH_DEFINES
#include<math.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct
{
    float x, y, z;
} translate;

GLfloat DiffuseLight[] = {1.f, 1.f, 1.f};
GLfloat AmbientLight[] = {0.3f, 0.3f, 0.3f};
GLfloat LightPosition[] = {0.f, 1.f, 2.f, 0.f};
//GLfloat LightPosition2[] = {30.f, 20.f, 0.f, 1.f};

const float orgX = 0.f, orgY = 0.f, orgZ = 0.f;

/*keyFrame keys[] = {	{1.f,			0.0f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f},
						{360.f/90.f,	90.f,	0.f,	0.f,	1.0f,	0.f,	15.f,	0.f,	0.f,	0.f,	0.f,	0.f},
						{1.f,			0.0f,	0.0f,	0.f,	1.0f,	0.f,	15.f,	0.f,	0.f,	0.f,	0.f,	0.f},
						{360.f/90.f,	-90.f,	0.f,	0.f,	1.0f,	0.f,	15.f,	0.f,	0.f,	0.f,	0.f,	0.f},
						{0.f,			0.0f,	0.0f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f},
};*/

/*keyFrame camera[] = {	{2.f,	0.f,		0.f, 0.f, 0.f,	0.f, 0.f, 0.f,		-5.f/2.f,	0.f, 0.f, 1.f},
						{2.f,	0.f,		0.f, 0.f, 0.f,	0.f, 0.f, 0.f,		0.f,		0.f, 0.f, 0.f},
						{4.f,	0.f,		0.f, 0.f, 0.f,	0.f, 0.f, 0.f,		5.f/2.f,	0.f, 0.f, 1.f},
						{1.f,	90.f/1.f, 	0.f, 1.f, 0.5f,	0.f, 0.f, -5.f,		0.f,		0.f, 0.f, 0.f},
						{3.f,	0.f,		0.f, 0.f, 0.f,	0.f, 0.f, 0.f,		-1.f,		1.f, 0.f, 0.f},
						{1.f,	45.f,		0.f, 0.f, 1.f,	-3.f, 0.f, -5.f,	0.f,		0.f, 0.f, 0.f}	}*/

const int cards = 52;
const int roundEdgeDivisions = 5;

float angle = 0.0;

void card(float w, float h, float e, float r, int wRes, int hRes);
void cardWire(double w, double h, double e, double r, int wRes, int hRes, float wRad, float hRad, GLuint texture, GLuint textureBack);

#include<stdio.h>

void scene(GLuint* texture, GLuint texture2, animation anim, double timeElapsed)
{
    static double time;
    time+= timeElapsed;

    glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
    //glLightfv (GL_LIGHT2, GL_DIFFUSE, DiffuseLight);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, AmbientLight);
	glLightfv (GL_LIGHT0, GL_POSITION, LightPosition);
	//glLightfv (GL_LIGHT2, GL_POSITION, LightPosition2);

    for(size_t i = 0; i < anim.count-1; ++i)
	{
		if(anim.sh[i].index >= anim.sh[i].size)
			continue;
		keyFrame* cKey = (keyFrame*)(((intptr_t)anim.key) + anim.sh[i].offset - sizeof(mainHeader) - anim.count*sizeof(subHeader));
		//printf("%p\n", (((intptr_t)key) + sh[i].offset - sizeof(mainHeader) - count*sizeof(subHeader)) - (intptr_t)key);
		if(time > cKey[anim.sh[i].index].timeSec && anim.sh[i].index < anim.sh[i].size)
		{
			anim.sh[i].index++;
			anim.sh[i].angleX = round(anim.sh[i].angleX);
			anim.sh[i].angleY = round(anim.sh[i].angleY);
			anim.sh[i].angleZ = round(anim.sh[i].angleZ);
		}

		anim.sh[i].angleX+=cKey[anim.sh[i].index].angleX*timeElapsed;
		anim.sh[i].angleY+=cKey[anim.sh[i].index].angleY*timeElapsed;
		anim.sh[i].angleZ+=cKey[anim.sh[i].index].angleZ*timeElapsed;
		anim.sh[i].transX+=cKey[anim.sh[i].index].transX*timeElapsed;
		anim.sh[i].transY+=cKey[anim.sh[i].index].transY*timeElapsed;
		anim.sh[i].transZ+=cKey[anim.sh[i].index].transZ*timeElapsed;

		if(anim.sh[i].angleX >= 360)
			anim.sh[i].angleX -= 360;
		else if(anim.sh[i].angleX < 0)
			anim.sh[i].angleX += 360;

		if(anim.sh[i].angleY >= 360)
			anim.sh[i].angleY -= 360;
		else if(anim.sh[i].angleY < 0)
			anim.sh[i].angleY += 360;

		if(anim.sh[i].angleZ >= 360)
			anim.sh[i].angleZ -= 360;
		else if(anim.sh[i].angleZ < 0)
			anim.sh[i].angleZ += 360;

		/*if(i == 0)
			printf("%d[%d] %f %f %f %f %f %f %f %f %f %f %f\n", i, anim.sh[i].index, cKey[anim.sh[i].index].angleX, cKey[anim.sh[i].index].angleY, cKey[anim.sh[i].index].angleZ,
					anim.sh[i].angleX, anim.sh[i].angleY, anim.sh[i].angleZ, anim.sh[i].transX, anim.sh[i].transY, anim.sh[i].transZ);*/
	}
	//printf("continue(y/n)\n");
	//getchar();
	//printf("debug\n");
	int i  = 0;
    for(int i = 0; i < cards; ++i)
    {
    	keyFrame* cKey = (keyFrame*)(((intptr_t)anim.key) + anim.sh[i].offset - sizeof(mainHeader) - anim.count*sizeof(subHeader));
    	//printf("get cKey v\n");
    	glPushMatrix();
    	//glTranslatef(0.f, 15.f, 0.f);
    	glTranslatef(anim.sh[i].transX-cKey[MIN(anim.sh[i].index, anim.sh[i].size-1)].transAngX,
					anim.sh[i].transY-cKey[MIN(anim.sh[i].index, anim.sh[i].size-1)].transAngY,
					anim.sh[i].transZ-cKey[MIN(anim.sh[i].index, anim.sh[i].size-1)].transAngZ + 0.023f*i);
		glRotatef(anim.sh[i].angleX, 1.f, 0.f, 0.f);
		glRotatef(anim.sh[i].angleY, 0.f, 1.f, 0.f);
		glRotatef(anim.sh[i].angleZ, 0.f, 0.f, 1.f);
		glTranslatef(cKey[MIN(anim.sh[i].index, anim.sh[i].size-1)].transAngX, cKey[MIN(anim.sh[i].index, anim.sh[i].size-1)].transAngY, cKey[MIN(anim.sh[i].index, anim.sh[i].size-1)].transAngZ);
		//glRotatef(sh[i].translateAng, cKey[sh[i].index].transAngX, cKey[sh[i].index].transAngY, cKey[sh[i].index].transAngZ);
		//printf("rotate v\n");
        //printf("translate v\n");
        //glColor3f(i%3 == 0 ? 1.0f : 0.0f, i%3 == 1 ? 1.0f : 0.0f, i%3 == 2 ? 1.0f : 0.0f);
        cardWire(6.4f, 9.4f, 0.023f, 0.3f, 10, 10, 0.f, 60.f*M_PI/180.f, texture[i], texture2);
        glPopMatrix();
    }
    //printf("debug\n");
    //glPopMatrix();
}

void cardWire(double w, double h, double e, double r, int wRes, int hRes, float wRad, float hRad, GLuint texture, GLuint textureBack)
{
	glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    ///front
    //curva carta esquerda (5 divisões)
    for(unsigned int i = 0; i < roundEdgeDivisions; ++i)
    {
    	double rad1 = i*(90.f/roundEdgeDivisions*M_PI/180.f);
    	double rad2 = (i+1)*(90.f/roundEdgeDivisions*M_PI/180.f);

    	//esquerda
    	glTexCoord2d(1.f-r*(1-cos(rad1))/w, r*(1-sin(rad1))/h);
    	glVertex3f(-w/2+r*(1-cos(rad1)), -h/2+r*(1-sin(rad1)), 0.f);
    	glTexCoord2d(1.f-r*(1-cos(rad1))/w, (h-r*(1-sin(rad1)))/h);
		glVertex3f(-w/2+r*(1-cos(rad1)), h/2-r*(1-sin(rad1)), 0.f);
		glTexCoord2d(1.f-r*(1-cos(rad2))/w, (h-r*(1-sin(rad2)))/h);
		glVertex3f(-w/2+r*(1-cos(rad2)), h/2-r*(1-sin(rad2)), 0.f);
		glTexCoord2d(1.f-r*(1-cos(rad2))/w, r*(1-sin(rad2))/h);
		glVertex3f(-w/2+r*(1-cos(rad2)), -h/2+r*(1-sin(rad2)), 0.f);

		//direita
		glTexCoord2d(1.f-(w-r*(1-cos(rad2)))/w, r*(1-sin(rad2))/h);
		glVertex3f(w/2-r*(1-cos(rad2)), -h/2+r*(1-sin(rad2)), 0.f);
		glTexCoord2d(1.f-(w-r*(1-cos(rad2)))/w, (h-r*(1-sin(rad2)))/h);
		glVertex3f(w/2-r*(1-cos(rad2)), h/2-r*(1-sin(rad2)), 0.f);
		glTexCoord2d(1.f-(w-r*(1-cos(rad1)))/w, (h-r*(1-sin(rad1)))/h);
		glVertex3f(w/2-r*(1-cos(rad1)), h/2-r*(1-sin(rad1)), 0.f);
		glTexCoord2d(1.f-(w-r*(1-cos(rad1)))/w, r*(1-sin(rad1))/h);
		glVertex3f(w/2-r*(1-cos(rad1)), -h/2+r*(1-sin(rad1)), 0.f);
    }

    glTexCoord2d(1.f-r/w, 0.f);
    glVertex3f(-w/2+r, -h/2, 0.f);
    glTexCoord2d(1.f-r/w, 1.f);
    glVertex3f(-w/2+r, h/2, 0.f);
    glTexCoord2d(r/w, 1.f);
    glVertex3f(w/2-r, h/2, 0.f);
    glTexCoord2d(r/w, 0.f);
    glVertex3f(w/2-r, -h/2, 0.f);
    glEnd();

    //back
    glBindTexture(GL_TEXTURE_2D, textureBack);
    glBegin(GL_QUADS);
	for(unsigned int i = 0; i < roundEdgeDivisions; ++i)
    {
    	double rad1 = i*(90.f/roundEdgeDivisions*M_PI/180.f);
    	double rad2 = (i+1)*(90.f/roundEdgeDivisions*M_PI/180.f);

    	//esquerda
    	glTexCoord2d(r*(1-cos(rad1))/w, r*(1-sin(rad1))/h);
    	glVertex3f(-w/2+r*(1-cos(rad1)), -h/2+r*(1-sin(rad1)), e);	//- -
    	glTexCoord2d(r*(1-cos(rad2))/w, r*(1-sin(rad2))/h);
		glVertex3f(-w/2+r*(1-cos(rad2)), -h/2+r*(1-sin(rad2)), e);	//+ -
		glTexCoord2d(r*(1-cos(rad2))/w, (h-r*(1-sin(rad2)))/h);
		glVertex3f(-w/2+r*(1-cos(rad2)), h/2-r*(1-sin(rad2)), e);	//+ +
		glTexCoord2d(r*(1-cos(rad1))/w, (h-r*(1-sin(rad1)))/h);
		glVertex3f(-w/2+r*(1-cos(rad1)), h/2-r*(1-sin(rad1)), e);	//- +


		//direita
		glTexCoord2d((w-r*(1-cos(rad2)))/w, r*(1-sin(rad2))/h);
		glVertex3f(w/2-r*(1-cos(rad2)), -h/2+r*(1-sin(rad2)), e);
		glTexCoord2d((w-r*(1-cos(rad1)))/w, r*(1-sin(rad1))/h);
		glVertex3f(w/2-r*(1-cos(rad1)), -h/2+r*(1-sin(rad1)), e);
		glTexCoord2d((w-r*(1-cos(rad1)))/w, (h-r*(1-sin(rad1)))/h);
		glVertex3f(w/2-r*(1-cos(rad1)), h/2-r*(1-sin(rad1)), e);
		glTexCoord2d((w-r*(1-cos(rad2)))/w, (h-r*(1-sin(rad2)))/h);
		glVertex3f(w/2-r*(1-cos(rad2)), h/2-r*(1-sin(rad2)), e);
    }

    glTexCoord2d(r/w, 0.f);
    glVertex3f(-w/2+r, -h/2, e);
    glTexCoord2d(1.f-r/w, 0.f);
    glVertex3f(w/2-r, -h/2, e);
    glTexCoord2d(1.f-r/w, 1.f);
    glVertex3f(w/2-r, h/2, e);
    glTexCoord2d(r/w, 1.f);
    glVertex3f(-w/2+r, h/2, e);
    glEnd();

    //right
    glBegin(GL_QUADS);
    for(unsigned int i = 0; i < roundEdgeDivisions; ++i)
    {
    	double rad1 = i*(90.f/roundEdgeDivisions*M_PI/180.f);
    	double rad2 = (i+1)*(90.f/roundEdgeDivisions*M_PI/180.f);

    	//cima
    	glVertex3f(w/2-r*(1-cos(rad1)), h/2-r*(1-sin(rad1)), 0.f);		//- -
		glVertex3f(w/2-r*(1-cos(rad2)), h/2-r*(1-sin(rad2)), 0.f);		//+ -
		glVertex3f(w/2-r*(1-cos(rad2)), h/2-r*(1-sin(rad2)), e);	//+ +
		glVertex3f(w/2-r*(1-cos(rad1)), h/2-r*(1-sin(rad1)), e);	//- +


		//baixo
		glVertex3f(w/2-r*(1-cos(rad2)), -h/2+r*(1-sin(rad2)), 0.f);
		glVertex3f(w/2-r*(1-cos(rad1)), -h/2+r*(1-sin(rad1)), 0.f);
		glVertex3f(w/2-r*(1-cos(rad1)), -h/2+r*(1-sin(rad1)), e);
		glVertex3f(w/2-r*(1-cos(rad2)), -h/2+r*(1-sin(rad2)), e);
    }

    glVertex3f(w/2, -h/2+r, 0.f);	//- -
    glVertex3f(w/2, h/2-r, 0.f);	//+ -
    glVertex3f(w/2, h/2-r, e);		//+ +
    glVertex3f(w/2, -h/2+r, e);		//- +

    //left
    for(unsigned int i = 0; i < roundEdgeDivisions; ++i)
    {
    	double rad1 = i*(90.f/roundEdgeDivisions*M_PI/180.f);
    	double rad2 = (i+1)*(90.f/roundEdgeDivisions*M_PI/180.f);

    	//cima
    	glVertex3f(-w/2+r*(1-cos(rad1)), h/2-r*(1-sin(rad1)), 0.f);		//- -
    	glVertex3f(-w/2+r*(1-cos(rad1)), h/2-r*(1-sin(rad1)), e);	//- +
		glVertex3f(-w/2+r*(1-cos(rad2)), h/2-r*(1-sin(rad2)), e);	//+ +
		glVertex3f(-w/2+r*(1-cos(rad2)), h/2-r*(1-sin(rad2)), 0.f);		//+ -


		//baixo
		glVertex3f(-w/2+r*(1-cos(rad2)), -h/2+r*(1-sin(rad2)), 0.f);
		glVertex3f(-w/2+r*(1-cos(rad2)), -h/2+r*(1-sin(rad2)), e);
		glVertex3f(-w/2+r*(1-cos(rad1)), -h/2+r*(1-sin(rad1)), e);
		glVertex3f(-w/2+r*(1-cos(rad1)), -h/2+r*(1-sin(rad1)), 0.f);
    }

    glVertex3f(-w/2, -h/2+r, 0.f);	//- -
    glVertex3f(-w/2, -h/2+r, e);		//- +
    glVertex3f(-w/2, h/2-r, e);		//+ +
    glVertex3f(-w/2, h/2-r, 0.f);	//+ -

    //top
    glVertex3f(-w/2+r, h/2, 0.f);
    glVertex3f(-w/2+r, h/2, e);
    glVertex3f(w/2-r, h/2, e);
    glVertex3f(w/2-r, h/2, 0.f);

    //bottom
    glVertex3f(-w/2+r, -h/2, 0.f);
    glVertex3f(w/2-r, -h/2, 0.f);
    glVertex3f(w/2-r, -h/2, e);
    glVertex3f(-w/2+r, -h/2, e);
    glEnd();
}

/*void cardWire(float w, float h, float e, float r, int wRes, int hRes, float wRad, float hRad)
{
    float wStep = w/wRes;
    float hStep = h/hRes;
    float hRadStep = hRad/hRes;

    float ref = sin(hRad);

    float wVert = -w/2;
    for(int i = 0; i < wRes; wVert += wStep, ++i)
    {

        float hVert = -h/2;
        float hRadCur = 0.0f;
        for(int j = 0; j < hRes; hVert += hStep, hRadCur += hRadStep, ++j)
        {
            //front
            glBegin(GL_QUADS);
                glVertex3f(wVert, hVert, 0.0f);
                glVertex3f(wVert+wStep, hVert, 0.0f);
                glVertex3f(wVert+wStep, hVert+hStep, 0.0f);
                glVertex3f(wVert, hVert+hStep, 0.0f);
            glEnd();

            //back
            glBegin(GL_QUADS);
                glVertex3f(wVert, hVert, e);
                glVertex3f(wVert+wStep, hVert, e);
                glVertex3f(wVert+wStep, hVert+hStep, e);
                glVertex3f(wVert, hVert+hStep, e);
            glEnd();

            //right
            glBegin(GL_QUADS);
                glVertex3f(w/2, hVert, 0.0f);
                glVertex3f(w/2, hVert, e);
                glVertex3f(w/2, hVert+hStep, e);
                glVertex3f(w/2, hVert+hStep, 0.0f);
            glEnd();

            //left
            glBegin(GL_QUADS);
                glVertex3f(-w/2, hVert, 0.0f);
                glVertex3f(-w/2, hVert, e);
                glVertex3f(-w/2, hVert+hStep, e);
                glVertex3f(-w/2, hVert+hStep, 0.0f);
            glEnd();
        }

        //top
        glBegin(GL_QUADS);
            glVertex3f(wVert, h/2, 0.0f);
            glVertex3f(wVert, h/2, e);
            glVertex3f(wVert+wStep, h/2, e);
            glVertex3f(wVert+wStep, h/2, 0.0f);
        glEnd();

        //bottom
        glBegin(GL_QUADS);
            glVertex3f(wVert, -h/2, 0.0f);
            glVertex3f(wVert, -h/2, e);
            glVertex3f(wVert+wStep, -h/2, e);
            glVertex3f(wVert+wStep, -h/2, 0.0f);
        glEnd();
    }

}

void card(float w, float h, float e, float r, int wRes, int hRes)
{


	glBegin(GL_QUADS);
        //front
		glVertex3f(w/2, h/2, 0.0f);
		glVertex3f(-w/2, h/2, 0.0f);
		glVertex3f(-w/2, -h/2, 0.0f);
		glVertex3f(w/2, -h/2, 0.0f);

		//right
		glVertex3f(w/2, h/2, 0.0f);
		glVertex3f(w/2, h/2, e);
		glVertex3f(w/2, -h/2, e);
		glVertex3f(w/2, -h/2, 0.0f);

		//top
		glVertex3f(w/2, h/2, 0.0f);
		glVertex3f(w/2, h/2, e);
		glVertex3f(-w/2, h/2, e);
		glVertex3f(-w/2, h/2, 0.0f);

		//left
		glVertex3f(-w/2, h/2, 0.0f);
		glVertex3f(-w/2, h/2, e);
		glVertex3f(-w/2, -h/2, e);
		glVertex3f(-w/2, -h/2, 0.0f);

		//bottom
		glVertex3f(w/2, -h/2, 0.0f);
		glVertex3f(w/2, -h/2, e);
		glVertex3f(-w/2, -h/2, e);
		glVertex3f(-w/2, -h/2, 0.0f);

		//back
		glVertex3f(w/2, h/2, e);
		glVertex3f(-w/2, h/2, e);
		glVertex3f(-w/2, -h/2, e);
		glVertex3f(w/2, -h/2, e);
	glEnd();
}*/

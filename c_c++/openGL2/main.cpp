#include <glut.h>

#define _USE_MATH_DEFINES
#include<math.h>

#include<stdio.h>

#include "highResTimer.h"
#include "texture.h"
#include "animationLoader.h"

GLuint texture[52], texture2;
animation anim;

#include "scene.h"

const float maxFPS = 24.f;
double timeEl = 0.f;
const float speed = 50.f;

HIGH_RES_TIMER_FREQ timerFreq;
TIMER oldTime;
const int window_width = 640, window_height = 480;
unsigned char keyStates[512] = {0};
struct {
	float position[3];
	float lookingAt[3];
	float upDirection[3];
	float frontDirection[3];
	float rightDirection[3];
	float pitch, yaw, roll;
} camera;

void initCamera(void);
void cameraAnim(double timeElapsed);
//void keyOperations(void);
void drawSquare(void);

//callback functions
void display(void);
void idle(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void specialUp(int key, int x, int y);

int main(int argc, char* argv[])
{
	initCamera();
	camera.position[2] = -20.f;
	camera.frontDirection[2] = 1.f;

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Olá Mundo OpenGL");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutKeyboardUpFunc(keyUp);
	glutSpecialUpFunc(specialUp);

	void* cloves, *diamonds, *hearts, *spades;
	RECT dimensions1, dimensions2, dimensions3, dimensions4, clip;
	if(getBitmapFromBMP(&cloves, "Cloves.bmp", &dimensions1) != 0)
	{
		printf("error!! %x", GetLastError());
		return 1;
	}
	if(getBitmapFromBMP(&diamonds, "Diamonds.bmp", &dimensions2) != 0)
	{
		printf("error!! %x", GetLastError());
		return 2;
	}
	if(getBitmapFromBMP(&hearts, "Hearts.bmp", &dimensions3) != 0)
	{
		printf("error!! %x", GetLastError());
		return 3;
	}
	if(getBitmapFromBMP(&spades, "Spades.bmp", &dimensions4) != 0)
	{
		printf("error!! %x", GetLastError());
		return 4;
	}

	clip.left = 0;
	clip.top = 0;
	clip.bottom = 1480-82;
	clip.right = 1069-72;

	if(LoadTexture(texture, cloves, dimensions1, clip, 13, 1069, 1480, 7) != 0)
	{
		printf("error!! %x", GetLastError());
		return 5;
	}
	if(LoadTexture(&texture[13], diamonds, dimensions2, clip, 13, 1069, 1480, 7) != 0)
	{
		printf("error!! %x", GetLastError());
		return 6;
	}
	clip.left = 0;
	clip.top = 0;
	clip.bottom = 1272-71;
	clip.right = 920-63+100;

	if(LoadTexture(&texture[26], hearts, dimensions3, clip, 13, 920, 1272, 7) != 0)
	{
		printf("error!! %x", GetLastError());
		return 7;
	}
	if(LoadTexture(&texture[39], spades, dimensions4, clip, 13, 920, 1272, 7) != 0)
	{
		printf("error!! %x", GetLastError());
		return 8;
	}

	clip.left = 6420;
	clip.top = 1479;
	clip.bottom = 1392+1479;
	clip.right = 999+6420;

	if(LoadTexture(&texture2, cloves, dimensions1, clip, 1, 0, 0, 7) != 0)
	{
		printf("error!! %x", GetLastError());
		return 9;
	}
	free(cloves);
	free(hearts);
	free(spades);
	free(diamonds);

	loadAnim(&anim.key, &anim.sh, &anim.count, "anim.ah", 0.f);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);

	getTimerFrequency(&timerFreq, &oldTime);
	glutMainLoop();
	return 0;
}

void display(void)
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	cameraAnim(timeEl);
	gluLookAt(camera.position[0] + anim.sh[52].transX, camera.position[1] + anim.sh[52].transY, camera.position[2] + anim.sh[52].transZ,
			camera.lookingAt[0], camera.lookingAt[1], camera.lookingAt[2],
			camera.upDirection[0], camera.upDirection[1], camera.upDirection[2]);

	scene(texture, texture2, anim, timeEl);

	glutSwapBuffers();
}

void cameraAnim(double timeElapsed)
{
	static double time;
	size_t i = 52;

	/*if(anim.sh[i].index == anim.sh[i].size)
		return;*/

	keyFrame* cKey = (keyFrame*)(((intptr_t)anim.key) + anim.sh[i].offset - sizeof(mainHeader) - anim.count*sizeof(subHeader));

	if(anim.sh[i].index < anim.sh[i].size)
	{
		time+= timeElapsed;

		if(time > cKey[anim.sh[i].index].timeSec && anim.sh[i].index < anim.sh[i].size)
		{
			anim.sh[i].index++;
			anim.sh[i].angleX = round(anim.sh[i].angleX);
			anim.sh[i].angleY = round(anim.sh[i].angleY);
			anim.sh[i].angleZ = round(anim.sh[i].angleZ);
		}

		anim.sh[i].transX+=cKey[anim.sh[i].index].transX*timeElapsed;
		anim.sh[i].transY+=cKey[anim.sh[i].index].transY*timeElapsed;
		anim.sh[i].transZ+=cKey[anim.sh[i].index].transZ*timeElapsed;
		anim.sh[i].angleX+=cKey[anim.sh[i].index].angleX*timeElapsed;
		anim.sh[i].angleY+=cKey[anim.sh[i].index].angleY*timeElapsed;
		anim.sh[i].angleZ+=cKey[anim.sh[i].index].angleZ*timeElapsed;
	}

	/*if(anim.sh[i].index)
	{*/
		camera.lookingAt[0] = anim.sh[51].transX + cKey[anim.sh[i].index].transAngX + anim.sh[i].angleX;
		camera.lookingAt[1] = anim.sh[51].transY + cKey[anim.sh[i].index].transAngY + anim.sh[i].angleY;
		camera.lookingAt[2] = anim.sh[51].transZ + cKey[anim.sh[i].index].transAngZ + anim.sh[i].angleZ;
	//}

	printf("%d %f %f %f %f %f %f = %f %f %f\n",anim.sh[i].index, anim.sh[i].transX, anim.sh[i].transY, anim.sh[i].transZ,
			anim.sh[51].transAngleX, anim.sh[51].transAngleY, anim.sh[51].transAngleZ,
			camera.lookingAt[0], camera.lookingAt[1], camera.lookingAt[2]);
}

void cameraCalc(double time)
{
    /*if (keyStates[' '] == true)
	{
		camera.position[0] += camera.upDirection[0] * speed * time / 1E9;
		camera.position[1] += camera.upDirection[1] * speed * time / 1E9;
		camera.position[2] += camera.upDirection[2] * speed * time / 1E9;
	}
	if (keyStates['c'] == true)
	{
		camera.position[0] -= camera.upDirection[0] * speed * time / 1E9;
		camera.position[1] -= camera.upDirection[1] * speed * time / 1E9;
		camera.position[2] -= camera.upDirection[2] * speed * time / 1E9;
	}
	if (keyStates[GLUT_KEY_LEFT + 255] == true)
	{
		camera.position[0] += camera.rightDirection[0] * speed * time / 1E9;
		camera.position[1] += camera.rightDirection[1] * speed * time / 1E9;
		camera.position[2] += camera.rightDirection[2] * speed * time / 1E9;
	}
	if (keyStates[GLUT_KEY_RIGHT + 255] == true)
	{
		camera.position[0] -= camera.rightDirection[0] * speed * time / 1E9;
		camera.position[1] -= camera.rightDirection[1] * speed * time / 1E9;
		camera.position[2] -= camera.rightDirection[2] * speed * time / 1E9;
	}
	if (keyStates[GLUT_KEY_UP + 255] == true)
	{
		camera.position[0] += camera.frontDirection[0] * speed * time / 1E9;
		camera.position[1] += camera.frontDirection[1] * speed * time / 1E9;
		camera.position[2] += camera.frontDirection[2] * speed * time / 1E9;
	}
	if (keyStates[GLUT_KEY_DOWN + 255] == true)
	{
		camera.position[0] -= camera.frontDirection[0] * speed * time / 1E9;
		camera.position[1] -= camera.frontDirection[1] * speed * time / 1E9;
		camera.position[2] -= camera.frontDirection[2] * speed * time / 1E9;
	}

	if (keyStates['w'] == true)
	{
		camera.pitch += 60.f * time / 1E9;
		if (camera.pitch > 360.f)
			camera.pitch -= 360.f;
	}
	if (keyStates['s'] == true)
	{
		camera.pitch -= 60.f * time / 1E9;
		if (camera.pitch <= 0.f)
			camera.pitch += 360.f;
	}
	if (keyStates['a'] == true)
	{
		camera.yaw -= 60.f* time / 1E9;
		if (camera.yaw < 0.f)
			camera.yaw += 360.f;
	}
	if (keyStates['d'] == true)
	{
		camera.yaw += 60.f* time / 1E9;
		if (camera.yaw >= 360.f)
			camera.yaw -= 360.f;
	}

	if (keyStates['d'] || keyStates['a'] || keyStates['w'] || keyStates['s'])
	{
		camera.frontDirection[0] = cos(camera.yaw / 180.f * M_PI) * cos(camera.pitch / 180.f * M_PI);
		camera.frontDirection[1] = sin(camera.pitch / 180.f * M_PI);
		camera.frontDirection[2] = sin(camera.yaw / 180.f * M_PI) * cos(camera.pitch / 180.f * M_PI);
		camera.rightDirection[0] = camera.frontDirection[1] * camera.upDirection[2] - camera.frontDirection[2] * camera.upDirection[1];
		camera.rightDirection[1] = camera.frontDirection[2] * camera.upDirection[0] - camera.frontDirection[0] * camera.upDirection[2];
		camera.rightDirection[2] = camera.frontDirection[0] * camera.upDirection[1] - camera.frontDirection[1] * camera.upDirection[0];
		double lenght = camera.frontDirection[0] * camera.frontDirection[0] + camera.frontDirection[1] * camera.frontDirection[1] + camera.frontDirection[2] * camera.frontDirection[2];
		camera.frontDirection[0] /= lenght;
		camera.frontDirection[1] /= lenght;
		camera.frontDirection[2] /= lenght;
	}*/
}

void idle(void)
{
	static double totalSeconds;

	double time = getTimeNs(timerFreq, &oldTime);

	cameraCalc(time);

	totalSeconds += time;
	if (totalSeconds / 1E9 >= 1/ maxFPS)
	{
		glutPostRedisplay();
		timeEl = (totalSeconds /= 1E9);
		totalSeconds -= 1/ maxFPS;
	}
}

void reshape(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.f);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	keyStates[key] = true;
}

void special(int key, int x, int y)
{
	keyStates[key + 255] = true;
}

void keyUp(unsigned char key, int x, int y)
{
	keyStates[key] = false;
}

void specialUp(int key, int x, int y)
{
	keyStates[key + 255] = false;
}

void initCamera(void)
{
	memset(&camera, 0, sizeof(camera));
	camera.frontDirection[2] = -1.f;
	camera.upDirection[1] = 1.f;
	camera.rightDirection[0] = 1.f;
	camera.yaw = 90.f;
}

#include "main.h"
#include "OpenGL_Header.h"
#include "UI_warpper.h"
#include "Mesh.h"
int g_screen_width = DEFAULT_SCREEN_WIDTH;
int g_screen_height = DEFAULT_SCREEN_HEIGHT;
//---------------Mouse----------------//
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMidDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance;
//----------------UI------------------//
AntTweakBarWrapper* g_config_bar;
//------------Render State------------//
bool g_show_wireframe = false;
float position[]={5,5,5};
float light_ambient[]={1,1,0,1};
//------------Mesh--------------------//
Mesh g_mesh;
char *model1Path[3]={("../../data/models/surgical/asa3.obj"),("../../data/models/surgical/attachment2.obj"),("../../data/models/surgical/fescia3.obj")}; //rectumnew
void TurnOnLight()
{
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0,GL_POSITION,position);
	glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
	glEnable(GL_LIGHT0);
}
void init_opengl()
{
	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glShadeModel(GL_SMOOTH);
	g_config_bar = new AntTweakBarWrapper();
    g_config_bar->ChangeTwBarWindowSize(g_screen_width, g_screen_height);
	g_config_bar->Reset();
	g_mesh.LoadModel(model1Path[0],NULL,0);
	TurnOnLight();
}
void applyTransform()
{
	glTranslatef(0,0,cameraDistance-5);
	glRotatef(cameraAngleX, 1, 0, 0);   // pitch
	glRotatef(cameraAngleY, 0, 1, 0);   // heading
}
void releaseTransform()
{
	glPopMatrix();
}
void Perspective()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,float(g_screen_width) / float(g_screen_height),0.1,20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void resize(int w,int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,float(g_screen_width) / float(g_screen_height),0.1,20);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glClearColor(0,0,0,0);
	//Perspective();
	applyTransform();
	//glTranslatef(0,0,-5);
	if(g_show_wireframe)
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glutSolidTeapot(1);
	g_mesh.RenderFP();
	releaseTransform();

	g_config_bar->Draw();
	glutSwapBuffers();
}
void idle()
{
	glutPostRedisplay();
}
void mouseMotionCB(int x, int y)
{
	if (!TwEventMouseMotionGLUT(x, y))
	{
		cameraAngleY += (x - mouseX);
		cameraAngleX += (y - mouseY);
		mouseX = x;
		mouseY = y;
	}
}
void mouseCB(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;

    if (!TwEventMouseButtonGLUT(button, state, x, y))
	{
		switch(state)
		{
		case GLUT_DOWN:
			if (glutGetModifiers() == GLUT_LEFT_BUTTON)
			{
				mouseLeftDown = true;
			}
			break;
		case GLUT_UP:
			if (glutGetModifiers() == GLUT_LEFT_BUTTON)
			{
				mouseLeftDown = false;
			}
			break;
		}
	}
}
void KeyProcess(unsigned char key, int x, int y)
{
	if (!TwEventKeyboardGLUT(key, x, y))
	{

	}
}
int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(g_screen_width, g_screen_height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("framework");
	//glutFullScreen();
	init_opengl();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(resize);
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);
	glutKeyboardFunc(KeyProcess);
	glutMainLoop();

	return 0;
}
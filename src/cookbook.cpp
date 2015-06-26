#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "Graphics\shader.h"
#include "Graphics\TexturedPlane.h"
#include "Math\Vector3.h"
#include "Math\Matrix4.h"
#include "../Dependencies/glm/glm/glm.hpp"
#include "../Dependencies/glm/glm/gtc/matrix_transform.hpp"
#include "../Dependencies/glm/glm/gtc/type_ptr.hpp"
#include <assert.h>
#include "Math\TransformUtils.h"
#include "Graphics\FreeCamera.h"

#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);

const int WIDTH = 800;
const int HEIGHT = 600;

const int NUM_X = 40; //total quads on X axis
const int NUM_Z = 40; //total quads on Z axis

const float SIZE_X = 4; //size of plane in world space
const float SIZE_Z = 4;
const float HALF_SIZE_X = SIZE_X/2.0f;
const float HALF_SIZE_Z = SIZE_Z/2.0f;

//virtual key codes
const int VK_W = 0x57;
const int VK_S = 0x53;
const int VK_A = 0x41;
const int VK_D = 0x44;
const int VK_Q = 0x51;
const int VK_Z = 0x5a;

int windowHandler;

//ripple displacement speed
const float SPEED = 2;

//free camera instance
Graphics::CFreeCamera cam;

//floor checker texture ID
GLuint checkerTextureID;


//checkered plane object
Graphics::CTexturedPlane* checker_plane;

float mouseX=0, mouseY=0; //filtered mouse values

//delta time
float dt = 0;

//shader reference
Shader shader;

//vertex array and vertex buffer object IDs
GLuint vaoID;
GLuint vboVerticesID;
GLuint vboIndicesID;

//out vertex struct for interleaved attributes
struct Vertex {
	Engine::Vector3 position;
	Engine::Vector3 color;
};

//triangle vertices and indices
Engine::Vector3 vertices[(NUM_X+1)*(NUM_Z+1)];
const int TOTAL_INDICES = NUM_X*NUM_Z*2*3;
GLushort indices[TOTAL_INDICES];

glm::mat4  P2 = glm::mat4(1);
Engine::Matrix4 P;
//Matrix4 MV;

//camera transformation variables
int state = 0, oldX=0, oldY=0;
float rX=0, rY=0, fov = 45;

//current time
float time = 0;
//timing related variables
float last_time=0, current_time =0;

//mosue click handler
void OnMouseDown(int button, int s, int x, int y)
{
	if (s == GLUT_DOWN)
	{
		oldX = x;
		oldY = y;
	}
	std::cout << "onmousedown" << std::endl;

	if(button == GLUT_MIDDLE_BUTTON)
		state = 0;
	else
		state = 1;
}

//mosue move handler
void OnMouseMove(int x, int y)
{
	std::cout << "onmousemove" << std::endl;

	if (state == 0) {
		fov += (y - oldY)/5.0f;
		cam.SetupProjection(fov, cam.GetAspectRatio());
	} else {
		rY += (y - oldY)/5.0f;
		rX += (oldX-x)/5.0f;
		mouseX = rX;
		mouseY = rY;
		cam.Rotate(mouseX,mouseY, 0);
	}
	oldX = x;
	oldY = y;

	glutPostRedisplay();

}

void OnKeyDown( unsigned char key, int i, int j )
{
	/*switch(key) {
		case ' ':
			useFiltering = !useFiltering;
		break;
	}*/
	glutPostRedisplay();
}

void initGlut(int argc, char ** argv)
{
	glutInit(&argc, argv);	
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);
	glutInitWindowSize(WIDTH, HEIGHT);

	windowHandler = glutCreateWindow("Graphic Engine 0.001");
}

void initGlew()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err){
		std::cerr<<"Error: "<<glewGetErrorString(err)<<std::endl;
	} else {
		if (GLEW_VERSION_3_3)
		{
			std::cout<<"Driver supports OpenGL 3.3\nDetails:"<<std::endl;
		}
	}
	std::cout<<"\tUsing glew "<<glewGetString(GLEW_VERSION)<<std::endl;
	std::cout<<"\tVendor: "<<glGetString (GL_VENDOR)<<std::endl;
	std::cout<<"\tRenderer: "<<glGetString (GL_RENDERER)<<std::endl;
	std::cout<<"\tVersion: "<<glGetString (GL_VERSION)<<std::endl;
	std::cout<<"\tGLSL:"<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;
}

void OnInit()
{
	GL_CHECK_ERRORS
	//generate the checker texture
	GLubyte data[128][128]={0};
	for(int j=0;j<128;j++) {
		for(int i=0;i<128;i++) {
			data[i][j]=(i<=64 && j<=64 || i>64 && j>64 )?255:0;
		}
	}
	//generate texture object
	glGenTextures(1, &checkerTextureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, checkerTextureID);
	//set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GL_CHECK_ERRORS

	//set maximum aniostropy setting
	GLfloat largest_supported_anisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_supported_anisotropy);

	//set mipmap base and max level
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

	//allocate texture object
	glTexImage2D(GL_TEXTURE_2D,0,GL_RED, 128, 128, 0, GL_RED, GL_UNSIGNED_BYTE, data);

	//generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	GL_CHECK_ERRORS

	//create a textured plane object
	checker_plane = new Graphics::CTexturedPlane();

	GL_CHECK_ERRORS

	//setup camera
	//setup the camera position and look direction
	Vector3 p = Vector3(5, 5, 5);
	cam.SetPosition(p);
	Vector3 look(p);
	look.normalize();

	//rotate the camera for proper orientation
	float yaw = glm::degrees(float(atan2(look.z, look.x)+Math::M_PI));
	float pitch = glm::degrees(asin(look.y));
	rX = yaw;
	rY = pitch;
	cam.Rotate(rX,rY,0);

	std::cout << "initialization completed" << std::endl;

}

void OnShutDown()
{
	delete checker_plane;
	glDeleteTextures(1, &checkerTextureID);

	std::cout << "shutdown completed" << std::endl;
}

void OnResize(int newWidth, int newHeight)
{
	glViewport (0, 0, (GLsizei) newWidth, (GLsizei) newHeight);
	cam.SetupProjection(45, (GLfloat)newWidth/newHeight);
}

//idle event callback
void OnIdle() {

	//handle the WSAD, QZ key events to move the camera around
	if( GetAsyncKeyState(VK_W) & 0x8000) {
		cam.Walk(dt);
	}

	if( GetAsyncKeyState(VK_S) & 0x8000) {
		cam.Walk(-dt);
	}

	if( GetAsyncKeyState(VK_A) & 0x8000) {
		cam.Strafe(-dt);
	}

	if( GetAsyncKeyState(VK_D) & 0x8000) {
		cam.Strafe(dt);
	}

	if( GetAsyncKeyState(VK_Q) & 0x8000) {
		cam.Lift(dt);
	}

	if( GetAsyncKeyState(VK_Z) & 0x8000) {
		cam.Lift(-dt);
	}

	if( GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
		glutDestroyWindow(windowHandler);
	}

	;
	Vector3 t = cam.getTranslation(); 
	if(t.dotProduct(t)>Math::EPSILON2) {
		cam.setTranslation(t*0.95f);
	}

	glutPostRedisplay();
}

void OnRender()
{
	//timing related calcualtion
	last_time = current_time;
	current_time = glutGet(GLUT_ELAPSED_TIME)/1000.0f;
	dt = current_time-last_time;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera transformation
	/*glm::mat4 MV	= cam.GetViewMatrix();
	glm::mat4 P     = cam.GetProjectionMatrix();
    glm::mat4 MVP	= P*MV;*/

	Engine::Matrix4 MV2;
	MV2 = cam.GetViewMatrix();
	P = cam.GetProjectionMatrix();

	Engine::Matrix4 MVP = MV2 * P;

	checker_plane->Render(MVP.m);

	glutSwapBuffers();
}


int main(int argc, char ** argv)
{
	//init OpenGL context
	initGlut(argc, argv);
	initGlew();

	OnInit();
	glutCloseFunc(OnShutDown);
	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnResize);
	glutMouseFunc(OnMouseDown);
	glutMotionFunc(OnMouseMove);
	glutKeyboardFunc(OnKeyDown);
	glutIdleFunc(OnIdle);
	glutMainLoop();
	return 0;
}





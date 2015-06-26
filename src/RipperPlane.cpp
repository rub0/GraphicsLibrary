/*#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "Graphics\shader.h"
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

//ripple displacement speed
const float SPEED = 2;

//free camera instance
Graphics::CFreeCamera cam;

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
float rX=25, rY=-40, dist = -7;

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

	if (state == 0)
		dist *= (1 + (y - oldY)/60.0f);
	else
	{
		rY += (x - oldX)/5.0f;
		rX += (y - oldY)/5.0f;
	}
	oldX = x;
	oldY = y;

	glutPostRedisplay();

}

void OnKeyDown( unsigned char k, int i, int j )
{

}

void initGlut(int argc, char ** argv)
{
	glutInit(&argc, argv);	
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);
	glutInitWindowSize(WIDTH, HEIGHT);

	glutCreateWindow("First try");
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
	//set the polygon mode to render lines
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	shader.loadFromFile( GL_FRAGMENT_SHADER,"resources/shaders/default.frag");
	shader.loadFromFile( GL_VERTEX_SHADER,"resources/shaders/default.vs");
	shader.createAndLinkProgram();
	shader.use();
		shader.addAttribute("vVertex");
		shader.addUniform("MVP");
		shader.addUniform("time");
	shader.unUse();
	GL_CHECK_ERRORS

	//setup plane geometry
	//setup plane vertices
	int count = 0;
	int i=0, j=0;
	for( j=0;j<=NUM_Z;j++) {
		for( i=0;i<=NUM_X;i++) {
			vertices[count++] = Engine::Vector3( ((float(i)/(NUM_X-1)) *2-1)* HALF_SIZE_X, 0, ((float(j)/(NUM_Z-1))*2-1)*HALF_SIZE_Z);
		}
	}

	//fill plane indices array
	GLushort* id=&indices[0];
	for (i = 0; i < NUM_Z; i++) {
		for (j = 0; j < NUM_X; j++) {
			int i0 = i * (NUM_X+1) + j;
			int i1 = i0 + 1;
			int i2 = i0 + (NUM_X+1);
			int i3 = i2 + 1;
			if ((j+i)%2) {
				*id++ = i0; *id++ = i2; *id++ = i1;
				*id++ = i1; *id++ = i2; *id++ = i3;
			} else {
				*id++ = i0; *id++ = i2; *id++ = i3;
				*id++ = i0; *id++ = i3; *id++ = i1;
			}
		}
	}

	GL_CHECK_ERRORS

	//setup triangle vao and vbo stuff
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboVerticesID);
	glGenBuffers(1, &vboIndicesID);
	GLsizei stride = sizeof(Vertex);

	glBindVertexArray(vaoID);

		glBindBuffer (GL_ARRAY_BUFFER, vboVerticesID);
		//pass triangle verteices to buffer object
		glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
		//enable vertex attribute array for position
		glEnableVertexAttribArray(shader["vVertex"]);
		glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE,0,0);
		GL_CHECK_ERRORS
		//pass indices to element array buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
	std::cout << "initialization completed" << std::endl;

}

void OnShutDown()
{
	shader.deleteProgram();
	glDeleteBuffers(1, &vboVerticesID);
	glDeleteBuffers(1, &vboIndicesID);
	glDeleteVertexArrays(1, &vaoID);

	std::cout << "shutdown completed" << std::endl;
}

void OnResize(int newWidth, int newHeight)
{
	glViewport (0, 0, (GLsizei) newWidth, (GLsizei) newHeight);
	P2 = glm::perspective(45.0f, (GLfloat)newWidth/newHeight, 1.f, 1000.f);
	P = Math::perspectiveMatrix(45.0f, (GLfloat)newWidth/newHeight, 1.f, 1000.f);
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

	Vector3 t = cam.GetTranslation(); 
	if(t.dotProduct(t)>Math::EPSILON2) {
		cam.SetTranslation(t*0.95f);
	}

	glutPostRedisplay();
}

void OnRender()
{
	//timing related calcualtion
	last_time = current_time;
	current_time = glutGet(GLUT_ELAPSED_TIME)/1000.0f;
	dt = current_time-last_time;
	time = glutGet(GLUT_ELAPSED_TIME)/1000.0f * SPEED;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set teh camera viewing transformation
	glm::mat4 T		= glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, dist));
	glm::mat4 Rx	= glm::rotate(T,  rX, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 MV	= glm::rotate(Rx, rY, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 MVP1	= P2*MV;

	Engine::Matrix4 MV2;
	MV2 = Math::createTransMatrix(MV2, Engine::Vector3(0.0f, 0.0f, dist));
	Engine::Matrix3 aux = MV2.getRotationMatrix();
	aux.rotate(Engine::Vector3(1.0f, 0.0f, 0.0f), rX);
	aux.rotate(Engine::Vector3(0.0f, 1.0f, 0.0f), rY);
	MV2.setRotationMatrix(aux);

	shader.use();

	Engine::Matrix4 MVP = MV2 * P;

	glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, MVP.m);
	//glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(MVP1));
	glUniform1f(shader("time"), time);
			//draw the mesh triangles
			glDrawElements(GL_TRIANGLES, TOTAL_INDICES, GL_UNSIGNED_SHORT, 0);
	shader.unUse();

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




*/
#include "TexturedPlane.h"
#include "../Math/Vector3.h"
using namespace Engine;
namespace Graphics
{
	CTexturedPlane::CTexturedPlane(const int w, const int d)
	{
		width = w;
		depth = d;

		//setup shader
		shader.loadFromFile(GL_VERTEX_SHADER, "resources/shaders/checker_shader.vert");
		shader.loadFromFile(GL_FRAGMENT_SHADER, "resources/shaders/checker_shader.frag");
		shader.createAndLinkProgram();
		shader.use();	
			shader.addAttribute("vVertex");
			shader.addUniform("MVP"); 
			shader.addUniform("textureMap"); 
			glUniform1i(shader("textureMap"), 0);
		shader.unUse();
 
		Init();
	}


	CTexturedPlane::~CTexturedPlane(void)
	{
	}

	int CTexturedPlane::GetTotalVertices() {
		return 4;
	}

	int CTexturedPlane::GetTotalIndices() {
		return 6;
	}

	GLenum CTexturedPlane::GetPrimitiveType() {
		return GL_TRIANGLES;
	}

	void CTexturedPlane::FillVertexBuffer(GLfloat* pBuffer) {
		Vector3* vertices = (Vector3*)(pBuffer);
	
		int width_2 = width/2;
		int depth_2 = depth/2;
	 
		vertices[0] = Vector3( -width_2, 0,-depth_2);
		vertices[1] = Vector3( width_2,0, -depth_2);

		vertices[2] = Vector3( width_2,0,depth_2);
		vertices[3] = Vector3( -width_2,0,depth_2); 
	}

	void CTexturedPlane::FillIndexBuffer(GLuint* pBuffer) {
	
		//fill indices array
		GLuint* id=pBuffer; 
		*id++ = 0; 
		*id++ = 1; 
		*id++ = 2;
		*id++ = 0;
		*id++ = 2;
		*id++ = 3;
	}
}
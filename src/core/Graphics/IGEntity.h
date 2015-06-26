#pragma once
#include "Shader.h"
namespace Graphics
{
	class IGEntity
	{
	public:
		IGEntity(void);
		virtual ~IGEntity(void);
		void Render(const float* MVP);
	
		virtual int GetTotalVertices()=0;
		virtual int GetTotalIndices()=0;
		virtual GLenum GetPrimitiveType() =0;

		virtual void FillVertexBuffer(GLfloat* pBuffer)=0;
		virtual void FillIndexBuffer(GLuint* pBuffer)=0;
	
		virtual void SetCustomUniforms(){}

		void Init();
		void Destroy();

	protected:
		GLuint vaoID;
		GLuint vboVerticesID;
		GLuint vboIndicesID;
	
		Shader shader;

		GLenum primType;
		int totalVertices, totalIndices;
	};
}


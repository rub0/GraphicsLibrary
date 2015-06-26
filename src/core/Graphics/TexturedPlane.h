#pragma once
#include "IGEntity.h"
namespace Graphics
{
	class CTexturedPlane : public IGEntity
	{
	public:
		CTexturedPlane(const int width=1000, const int depth=1000);
		virtual ~CTexturedPlane(void);
		int GetTotalVertices();
		int GetTotalIndices(); 
		GLenum GetPrimitiveType();

		void FillVertexBuffer( GLfloat* pBuffer);
		void FillIndexBuffer( GLuint* pBuffer);
	 
	private:
		int width, depth;
	};
}


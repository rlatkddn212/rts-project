#include "Precompiled.h"
#include "SkinnedVertexArray.h"
#include <GL/glew.h>

SkinnedVertexArray::SkinnedVertexArray(const float * verts, unsigned int numVerts, const unsigned int * indices, unsigned int numIndices)
{
	mNumIndices = numIndices;
	mNumVerts = numVerts;
	// Create vertex array
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	unsigned vertexSize = 16 * sizeof(float);

	// Create vertex buffer
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVerts * vertexSize, verts, GL_STATIC_DRAW);

	// Create index buffer
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// Position is 3 floats
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
	// Normal is 3 floats
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
		reinterpret_cast<void*>(sizeof(float) * 3));
	// Skinning indices (keep as ints)
	glEnableVertexAttribArray(2);
	glVertexAttribIPointer(2, 4, GL_UNSIGNED_INT, vertexSize,
		reinterpret_cast<void*>(sizeof(float) * 6));
	// Skinning weights (convert to floats)
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, vertexSize,
		reinterpret_cast<void*>(sizeof(float) * 10));
	// Texture coordinates
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, vertexSize,
		reinterpret_cast<void*>(sizeof(float) * 14));
}

SkinnedVertexArray::~SkinnedVertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void SkinnedVertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}

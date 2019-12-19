#pragma once
class SkinnedVertexArray
{
public:
	SkinnedVertexArray(const float* verts, unsigned int numVerts,
		const unsigned int* indices, unsigned int numIndices);
	~SkinnedVertexArray();

	void SetActive();
	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }
private:
	// How many vertices in the vertex buffer?
	unsigned int mNumVerts;
	// How many indices in the index buffer
	unsigned int mNumIndices;
	// OpenGL ID of the vertex buffer
	unsigned int mVertexBuffer;
	// OpenGL ID of the index buffer
	unsigned int mIndexBuffer;
	// OpenGL ID of the vertex array object
	unsigned int mVertexArray;
};
#pragma once
class VertexArrayAlpha
{
public:
											VertexArrayAlpha(const float* verts, unsigned int numVerts,
												const unsigned int* indices, unsigned int numIndices);
										   ~VertexArrayAlpha();

	void									SetActive();
	unsigned int							GetNumIndices() const { return mNumIndices; }
	unsigned int							GetNumVerts() const { return mNumVerts; }

private:
	unsigned int							mNumVerts;
	unsigned int							mNumIndices;
	unsigned int							mVertexBuffer;
	unsigned int							mIndexBuffer;
	unsigned int							mVertexArray;
};
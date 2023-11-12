#pragma once
#include <GL/glew.h>
#include "Renderer.h"



class IndexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_count;
public:
	IndexBuffer(const unsigned int* data, unsigned int size);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;
	inline unsigned int GetCount() const;

};

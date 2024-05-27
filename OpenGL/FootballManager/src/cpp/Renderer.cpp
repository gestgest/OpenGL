#include <iostream>
#include "../header/Renderer.h"
#include "../header/DebugRenderer.h"


void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

//bind
void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {

    //shader.setUniform4f("u_color", 0.0f, g, 1.0f, 1.0f);

    va.bind();
    ib.bind();

    //사각형이면 count = 6.
    GLCHECK(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

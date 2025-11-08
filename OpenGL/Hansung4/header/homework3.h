#pragma once
#ifndef HOMEWORK3
#define HOMEWORK3
#include <glad/glad.h>
#include <../Hansung4/header/shader.h>


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


class Object
{
protected:
    unsigned int vao;
    unsigned int vbo;

    glm::vec3 scale;
    glm::vec3 position;

public:
    Object()
    {
        scale = glm::vec3(1.0f, 1.0f, 1.0f);
        position = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    ~Object()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    void drawObject(Shader& shader, Camera& camera, glm::vec3 lightColor, glm::vec3 lightPos, glm::vec3 color)
    {
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // draw the sphere object
        // light properties
        shader.setVec3("lightColor", lightColor);
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", camera.Position);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        shader.setMat4("model", model);

        shader.setVec3("objectColor", color);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6); //삼각형
    }

    unsigned int& getVAO()
    {
        return vao;
    }
    unsigned int& getVBO()
    {
        return vbo;
    }

};


class Ground : public Object
{
public:
    Ground()
    {
        //position = glm::vec3(0.0f, -0.5f, 0.0f);
        
        float groundVertices[] = {
            // positions            // normals 
             12.0f, -1.0f,  12.0f,   0.0f, 1.0f, 0.0f,
            -12.0f, -1.0f,  12.0f,   0.0f, 1.0f, 0.0f,
            -12.0f, -1.0f, -12.0f,   0.0f, 1.0f, 0.0f,

             12.0f, -1.0f,  12.0f,   0.0f, 1.0f, 0.0f,
            -12.0f, -1.0f, -12.0f,   0.0f, 1.0f, 0.0f,
             12.0f, -1.0f, -12.0f,   0.0f, 1.0f, 0.0f
        };

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

        // 1. 위치(Position) 속성 (layout (location = 0))
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // 2. 법선(Normal) 속성 (layout (location = 1))
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void drawObject(Shader& shader, Camera& camera, glm::vec3 lightColor, glm::vec3 lightPos, glm::vec3 color)
    {
        Object::drawObject(shader, camera, lightColor, lightPos, color);
        glDrawArrays(GL_TRIANGLES, 0, 6); //삼각형
    }
    
};


class Ball : public Object
{
    glm::vec3 velocity;
    int nSphereVert;
    int nSphereAttr;
public:
    Ball()
    {
        velocity = glm::vec3(0.0f, -0.1f, 0.0f);
        position = glm::vec3(0.0f, -1.0f, 0.0f);

        float* sphereVerts = NULL;

        init_sphere(&sphereVerts);
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, nSphereVert * nSphereAttr * sizeof(float), sphereVerts, GL_STATIC_DRAW); //static draw를 하는게 맞나?

        // 1. 위치(Position) 속성 (layout (location = 0))
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // 2. 법선(Normal) 속성 (layout (location = 1))
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        free(sphereVerts);
    }


    // position, normal, tex_coords.
    // 매개변수 방정식으로 원 그리는 함수 => 
    void init_sphere(float** vertices)
    {
        //nAttr : 8
        // sphere: set up vertex data and configure vertex attributes
        float pi = acosf(-1.0f);	// pi = 3.14152...
        float pi2 = 2.0f * pi;
        int nu = 40, nv = 20;
        const double du = pi2 / nu;
        const double dv = pi / nv;

        //19 * 40 * 6
        nSphereVert = (nv - 1) * nu * 6;		// two triangles
        nSphereAttr = 6;
        *vertices = (float*)malloc(sizeof(float) * (nSphereVert) * (nSphereAttr));

        float u, v;
        int k = 0;

        v = 0.0f;
        u = 0.0f;
        for (v = (-0.5f) * pi + dv; v < 0.5f * pi - dv; v += dv)
        {
            for (u = 0.0f; u < pi2; u += du)
            {
                // p(u,v)
                (*vertices)[k++] = cosf(v) * cosf(u); 	(*vertices)[k++] = cosf(v) * sinf(u);	(*vertices)[k++] = sinf(v); 	// position (x,y,z)
                (*vertices)[k++] = cosf(v) * cosf(u);	(*vertices)[k++] = cosf(v) * sinf(u);	(*vertices)[k++] = sinf(v);		// normal (x,y z)
                // 이렇게 해서 8개의 속성 => nAttr


                // p(u+du,v)
                (*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du);	(*vertices)[k++] = sinf(v); 	// position
                (*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du);	(*vertices)[k++] = sinf(v);		// normal

                // p(u,v+dv)
                (*vertices)[k++] = cosf(v + dv) * cosf(u);	(*vertices)[k++] = cosf(v + dv) * sinf(u);	(*vertices)[k++] = sinf(v + dv);	// position
                (*vertices)[k++] = cosf(v + dv) * cosf(u);	(*vertices)[k++] = cosf(v + dv) * sinf(u);	(*vertices)[k++] = sinf(v + dv);	// normal

                // p(u+du,v)
                (*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du);	(*vertices)[k++] = sinf(v); 	// position
                (*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du);	(*vertices)[k++] = sinf(v);		// normal

                // p(u+du,v+dv)
                (*vertices)[k++] = cosf(v + dv) * cosf(u + du);	(*vertices)[k++] = cosf(v + dv) * sinf(u + du);	(*vertices)[k++] = sinf(v + dv); 	// position
                (*vertices)[k++] = cosf(v + dv) * cosf(u + du);	(*vertices)[k++] = cosf(v + dv) * sinf(u + du);	(*vertices)[k++] = sinf(v + dv);	// normal

                // p(u,v+dv)
                (*vertices)[k++] = cosf(v + dv) * cosf(u);	(*vertices)[k++] = cosf(v + dv) * sinf(u);	(*vertices)[k++] = sinf(v + dv);	// position
                (*vertices)[k++] = cosf(v + dv) * cosf(u);	(*vertices)[k++] = cosf(v + dv) * sinf(u);	(*vertices)[k++] = sinf(v + dv);	// normal
            }
        }
        // triangles around north pole and south pole
        for (u = 0.0f; u < pi2; u += du)
        {
            // triangles around north pole
            // p(u,pi/2-dv)
            v = 0.5f * pi - dv;
            (*vertices)[k++] = cosf(v) * cosf(u); 	(*vertices)[k++] = cosf(v) * sinf(u);	(*vertices)[k++] = sinf(v); 	// position
            (*vertices)[k++] = cosf(v) * cosf(u);	(*vertices)[k++] = cosf(v) * sinf(u);	(*vertices)[k++] = sinf(v);		// normal

            // p(u+du,pi/2-dv)
            v = 0.5f * pi - dv;
            (*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du); (*vertices)[k++] = sinf(v); 	// position
            (*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du); (*vertices)[k++] = sinf(v);		// normal

            // p(u,pi/2) = (0, 1. 0)  ~ north pole
            v = 0.5f * pi;
            (*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du);	(*vertices)[k++] = sinf(v); 	 // position
            (*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du);	(*vertices)[k++] = sinf(v);		 // normal

            // triangles around south pole
            // p(u,-pi/2) = (0, -1, 0)  ~ south pole
            v = (-0.5f) * pi;
            (*vertices)[k++] = cosf(v) * cosf(u); 	(*vertices)[k++] = cosf(v) * sinf(u);	(*vertices)[k++] = sinf(v); 		// position
            (*vertices)[k++] = cosf(v) * cosf(u);	(*vertices)[k++] = cosf(v) * sinf(u);	(*vertices)[k++] = sinf(v);			// normal

            // p(u+du,-pi/2+dv)
            v = (-0.5f) * pi + dv;
            (*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du); (*vertices)[k++] = sinf(v);	// position
            (*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du); (*vertices)[k++] = sinf(v);	// normal

            // p(u,-pi/2+dv)
            (*vertices)[k++] = cosf(v) * cosf(u);	(*vertices)[k++] = cosf(v) * sinf(u); (*vertices)[k++] = sinf(v);	// position
            (*vertices)[k++] = cosf(v) * cosf(u);	(*vertices)[k++] = cosf(v) * sinf(u); (*vertices)[k++] = sinf(v);	// normal
        }
    }

    void drawObject(Shader& shader, Camera& camera, glm::vec3 lightColor, glm::vec3 lightPos, glm::vec3 color, float deltaTime)
    {
        //position += velocity * deltaTime;
        std::cout << position.y << '\n';
        Object::drawObject(shader, camera, lightColor, lightPos, color);

        glDrawArrays(GL_TRIANGLES, 0, nSphereVert); //삼각형
    }
};
#endif
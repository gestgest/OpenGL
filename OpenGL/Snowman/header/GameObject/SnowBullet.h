#ifndef SNOWBULLET
#define SNOWBULLET
#include <../Snowman/header/GameObject/GameObject.h>

class SnowBullet : public GameObject{
public:

    //todo 물체 닿으면 그냥 사라지는 기능도 구현해야함. isCollider 오버라이딩
    SnowBullet()
    {
        initSnowBullet();
    }
    SnowBullet(Shader & shader, glm::vec3 color) : GameObject(shader, color)
    {
        initSnowBullet();
    }
    void initSnowBullet()
    {
        scale = glm::vec3(1.0f, 1.0f, 1.0f);
        position = glm::vec3(0.0f, 10.0f, 0.0f);
        velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        isStatic = false;

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

    void drawGameObject(Camera& camera, glm::vec3 lightColor, glm::vec3 lightPos)
    {
        if (!isActive)
        {
            return;
        }

        float pi = acosf(-1.0f);
        shader->use();
        //fs
        // light properties
        shader->setVec3("objectColor", color);

        //scale = glm::vec3(50.0f, 50.0f, 50.0f);

        GameObject::drawMiniGameObject(camera, lightColor, lightPos, color, glm::vec3(0, -0.5f, 0.0f));
        glDrawArrays(GL_TRIANGLES, 0, nSphereVert); //삼각형 형태로 그려라

    }

    //override
    void addRepulsion(float deltaTime)
    {
        this->isActive = false;
    }
};

#endif
#pragma once

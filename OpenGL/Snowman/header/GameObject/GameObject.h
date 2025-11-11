#ifndef GAMEOBJECT
#define GAMEOBJECT
#include <../Snowman/header/shader.h>
#include <../Snowman/header/camera.h>
#include <glad/glad.h>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

#define COR  -0.33f; //반발계수
#define GRAVITY_ACCELERATION -9.81f

void debugMat(glm::mat4 m)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << m[i][j] << ' ';
        }
        std::cout <<  '\n';

    }
    
}

bool isInBoundary(float a, float b, float size_a, float size_b)
{
    //a 10 ~ 20, b 5 ~ 10아래 => 충돌 판정에 안 된경우
    if (
        a - size_a > b - size_b &&
        a - size_a > b + size_b
        )
    {
        return false;
    }
    else if //a 5 ~ 10 > b 12 ~ 20
        (
            a - size_a < b - size_b &&
            a - size_a < b + size_b
            )
    {
        return false;
    }
    return true;

}

class GameObject {
protected:
    unsigned int vao;
    unsigned int vbo;
    Shader* shader;

    glm::vec3 scale;
    glm::vec3 position;
    glm::vec3 velocity;

    float movement_speed;

    bool isStatic;

public:
    GameObject()
    {
        scale = glm::vec3(1.0f, 1.0f, 1.0f);
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        movement_speed = 0.0f;

        isStatic = true;
    }
    GameObject(Shader& shader)
    {
        setShader(shader);
    }
    ~GameObject()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
    void setShader(Shader& shader)
    {
        this->shader = &shader;
    }

    void drawGameObject(Camera& camera, glm::vec3 lightColor, glm::vec3 lightPos, glm::vec3 color, glm::vec3 addPos)
    {
        //fs 셰이더 속성은 drawObject위에

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //카메라 사이즈
        //glm::mat4 view = glm::lookAt(camera.Position, glm::vec3(0.0f, 0.0f, 0.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::mat4 view = camera.GetViewMatrix();


        //m v p 벡터3
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);
        //debugMat(view);

        shader->setVec3("lightColor", lightColor);
        shader->setVec3("lightPos", lightPos);
        shader->setVec3("viewPos", camera.Position);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position + addPos);
        shader->setMat4("model", model);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6); //삼각형
    }

    void move(glm::vec3 vec, float deltaTime)
    {
        vec = glm::normalize(vec);
        float velocity = movement_speed * deltaTime;
            position += vec * velocity;
    }

    unsigned int& getVAO()
    {
        return vao;
    }
    unsigned int& getVBO()
    {
        return vbo;
    }
    glm::vec3 getPosition()
    {
        return position;
    }


    bool isCollisionEnter(GameObject& object)
    {
        // ==이어도 0 ~ 10, 0 ~ 10, 0 ~ 10 즉, 3개가 같아야함 ==> 하나라도 다르면 커트
        if (!isInBoundary(this->position.x, object.position.x, this->scale.x, object.scale.x))
        {
            return false;
        }
        if (!isInBoundary(this->position.y, object.position.y, this->scale.y, object.scale.y))
        {
            return false;
        }
        if (!isInBoundary(this->position.z, object.position.z, this->scale.z, object.scale.z))
        {
            return false;
        }
        return true;
    }

    //반발력 추가
    void addRepulsion(float deltaTime)
    {
        if (isStatic)
        {
            return;
        }
        velocity *= COR; //반발계수
    }

    void applyPhysics(float deltaTime)
    {
        if (isStatic)
        {
            return;
        }
        //a만큼 속력을 추가
        position += velocity * deltaTime;
        velocity += glm::vec3(0, GRAVITY_ACCELERATION, 0) * deltaTime;
    }
};

#endif
#pragma once

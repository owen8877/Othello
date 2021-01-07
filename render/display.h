#ifndef DISPLAY_H
#define DISPLAY_H

//#include "../core/element.h"
#include "../base.h"
#include "../core/game.h"
#include "shader.h"
#include <boost/format.hpp>

using namespace boost;

const double step = 2.0 / BOARD_SIZE;
const double hstep = 1.0 / BOARD_SIZE;
const int CIRCLE_MAX = 30;

extern double theta, fai;
extern int screenSize, screenWidth, screenHeight;

// void initOpenGLOptions();
// void drawCircle(double, double, double, Status);
// void drawHint(double, double, double);
// void drawBoard();
// void drawPiece();
// void drawStone();
// void drawBackGround();
// void display();
// void reshape(int, int);
void displayThread(const bool *);

extern void mouseKey(int, int, int, int);

extern void mouseMotion(int, int);

struct DirectionalLight {
    bool enabled = true;
    glm::vec3 direction{};
    glm::vec3 ambient{};
    glm::vec3 diffuse{};
    glm::vec3 specular{};

    void apply(Shader &shader, int index) const {
        shader.setBool((format("dirLights[%1%].enabled") % index).str(), enabled);
        shader.setVec3((format("dirLights[%1%].direction") % index).str(), direction);
        shader.setVec3((format("dirLights[%1%].ambient") % index).str(), ambient);
        shader.setVec3((format("dirLights[%1%].diffuse") % index).str(), diffuse);
        shader.setVec3((format("dirLights[%1%].specular") % index).str(), specular);
    }
};

struct PointLight {
    bool enabled = true;
    glm::vec3 position{};
    glm::vec3 ambient{};
    glm::vec3 diffuse{};
    glm::vec3 specular{};
    float constant{};
    float linear{};
    float quadratic{};

    void apply(Shader &shader, int index) const {
        shader.setBool((format("pointLights[%1%].enabled") % index).str(), enabled);
        shader.setVec3((format("pointLights[%1%].position") % index).str(), position);
        shader.setVec3((format("pointLights[%1%].ambient") % index).str(), ambient);
        shader.setVec3((format("pointLights[%1%].diffuse") % index).str(), diffuse);
        shader.setVec3((format("pointLights[%1%].specular") % index).str(), specular);
        shader.setFloat((format("pointLights[%1%].constant") % index).str(), constant);
        shader.setFloat((format("pointLights[%1%].linear") % index).str(), linear);
        shader.setFloat((format("pointLights[%1%].quadratic") % index).str(), quadratic);
    }
};

struct SpotLight {
    bool enabled = true;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;

    void apply(Shader &shader, int index) const {
        shader.setBool((format("spotLights[%1%].enabled") % index).str(), enabled);
        shader.setVec3((format("spotLights[%1%].position") % index).str(), position);
        shader.setVec3((format("spotLights[%1%].direction") % index).str(), direction);
        shader.setVec3((format("spotLights[%1%].ambient") % index).str(), ambient);
        shader.setVec3((format("spotLights[%1%].diffuse") % index).str(), diffuse);
        shader.setVec3((format("spotLights[%1%].specular") % index).str(), specular);
        shader.setFloat((format("spotLights[%1%].constant") % index).str(), constant);
        shader.setFloat((format("spotLights[%1%].linear") % index).str(), linear);
        shader.setFloat((format("spotLights[%1%].quadratic") % index).str(), quadratic);
        shader.setFloat((format("spotLights[%1%].cutOff") % index).str(), cutOff);
        shader.setFloat((format("spotLights[%1%].outerCutOff") % index).str(), outerCutOff);
    }
};

#endif

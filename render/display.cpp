#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "display.h"
#include "camera.h"
#include "shader.h"
#include "../control/model.h"
#include "../core/player.h"
#include "../core/ai.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/range/adaptor/indexed.hpp>

int screenSize = 80 * BOARD_SIZE, screenWidth = 80 * BOARD_SIZE, screenHeight = 80 * BOARD_SIZE;
double floatingx = 0.0, floatingy = 0.0;
float backGroundColor = 0.304;
float fogDensity = 0.00f;
bool isFocus = false;
GLfloat fogColorPause[] = {backGroundColor, backGroundColor, backGroundColor, 1.0f};
float fogColorFocus[] = {0.1f, 0.15f, 0.2f, 0.0f};
char s_Save[] = "Save";
char s_Load[] = "Load";
char s_Save_and_Quit[] = "Save and Quit";
char s_Quit_without_saving[] = "Quit without Saving";

extern double xcenter, ycenter, zcenter;
extern vector<Stone> stones;

extern void refreshModel(bool lifting);

extern int updateRenderStatus(int status);

extern void handleMouseButton(GLFWwindow *window, int button, int action, int mods);

extern void handleMouseMotion(int x, int y);

// debugging camera
Camera camera(glm::vec3{0.0f, -3.0f, 10.0f}, glm::vec3{0.0f, 0.0f, 1.0f});
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;
bool clipping = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lights
int directionalLightCap;
int pointLightCap;
int spotLightCap;
vector<DirectionalLight> directionalLights;
vector<PointLight> pointLights;
vector<SpotLight> spotLights;


//======================================================================================================================
// Input handlers

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (Game::getGameStatus() == Lifting || clipping) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.ProcessKeyboard(DOWN, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
            camera.ProcessKeyboard(LIFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_L) && Game::getGameStatus() == Playing) {
        Game::liftTheTable();
        camera.Zoom = ZOOM_MAX;
    }

    isFocus = false;
    if (Settings::btShift) {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) {
            isFocus = true;
        }
    }
    if (Settings::btCtrl) {
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) {
            isFocus = true;
        }
    }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    // glfw: whenever the mouse moves, this callback is called
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (Game::getGameStatus() == Lifting) {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }

    handleMouseMotion(xpos, ypos);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    camera.ProcessMouseScroll(yoffset);
}

void reshapeCore(int width, int height) {
    screenSize = min(width, height);
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
}

void reshapeCallback(GLFWwindow *window, int width, int height) {
#ifdef __EMSCRIPTEN__
    puts("Resized!");
#else

#endif
    reshapeCore(width, height);
}

//======================================================================================================================
// Init

void initLights() {
    glm::vec3 ambient = {0.25f, 0.25f, 0.25f};
    glm::vec3 diffuse = {0.7f, 0.7f, 0.7f};
    glm::vec3 specular = {0.4f, 0.4f, 0.4f};
    directionalLights = {
            {
                    .direction {0.0f, 0.0f, -1.0f},
                    .ambient = ambient,
                    .diffuse = diffuse,
                    .specular = specular,
            },
            {.enabled = false},
            {.enabled = false},
            {.enabled = false},
    };
    directionalLightCap = 1;

    glm::vec3 e1 = {1.0, 0.0, 0.0};
    glm::vec3 e2 = {0.0, 1.0, 0.0};
    glm::vec3 e3 = {0.0, 0.0, 1.0};
    float radius = 6.0f;
    pointLights = {
            {
                    .position {radius, radius * sqrt(3), radius + TABLE_HEIGHT},
                    .ambient = ambient,
                    .diffuse = diffuse,
                    .specular = specular,
                    .constant = 1.0f,
                    .linear = 0.09f,
                    .quadratic = 0.032,
            },
            {
                    .position {radius, -radius * sqrt(3), radius + TABLE_HEIGHT},
                    .ambient = ambient,
                    .diffuse = diffuse,
                    .specular = specular,
                    .constant = 1.0f,
                    .linear = 0.09f,
                    .quadratic = 0.032,
            },
            {
                    .position {-radius * 2, 0, radius + TABLE_HEIGHT},
                    .ambient = ambient,
                    .diffuse = diffuse,
                    .specular = specular,
                    .constant = 1.0f,
                    .linear = 0.09f,
                    .quadratic = 0.032,
            },
            {.enabled = false},
    };
    pointLightCap = 3;

    spotLights = {
            {
                    // position determined by camera!
                    .ambient = ambient,
                    .diffuse = diffuse / 3.0f,
                    .specular = specular / 3.0f,
                    .constant = 1.0f,
                    .linear = 0.09f,
                    .quadratic = 0.032,
                    .cutOff = glm::cos(glm::radians(12.0f)),
                    .outerCutOff = glm::cos(glm::radians(15.0f)),
            },
            {.enabled = false},
            {.enabled = false},
            {.enabled = false},
    };
    spotLightCap = 1;
}

GLFWwindow *initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_DOUBLEBUFFER, true);
    glfwWindowHint(GLFW_REFRESH_RATE, 60);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window;
    window = glfwCreateWindow(screenWidth, screenHeight, "Othello", nullptr, nullptr);
    if (window == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, handleMouseButton);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetWindowSizeCallback(window, reshapeCallback);

#ifdef __EMSCRIPTEN__
    emscripten_request_fullscreen("#canvas", true);
#else
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif

    return window;
}

void initOpenGLOptions() {
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

//======================================================================================================================
// Update fog and lights

void updateFog() {
//     if (Game::getGameStatus() == Pause) {
//         glEnable(GL_FOG);
//         glFogi(GL_FOG_MODE, GL_EXP2);
//         glFogf(GL_FOG_DENSITY, 0.15f);
//         glFogfv(GL_FOG_COLOR, fogColorPause);
//     }
//     else if (Game::getGameStatus() == Lifting) {
//         glEnable(GL_FOG);
//         glFogi(GL_FOG_MODE, GL_EXP);
//         glFogf(GL_FOG_DENSITY, fogDensity);
//         glFogfv(GL_FOG_COLOR, fogColorFocus);
//     }
//     else glDisable(GL_FOG);
}

void updateLights(Shader &shader) {
    if (Settings::fancyLights) {
        for (int i = 0; i < spotLightCap; ++i) {
            spotLights[i].enabled = true;
        }
        for (int i = 0; i < pointLightCap; ++i) {
            pointLights[i].enabled = true;
        }
    } else {
        for (int i = 0; i < spotLightCap; ++i) {
            spotLights[i].enabled = false;
        }
        for (int i = 0; i < pointLightCap; ++i) {
            pointLights[i].enabled = false;
        }
    }

    shader.use();
    for (auto const &directionalLight : directionalLights | boost::adaptors::indexed(0)) {
        directionalLight.value().apply(shader, directionalLight.index());
    }
    for (auto const &pointLight : pointLights | boost::adaptors::indexed(0)) {
        pointLight.value().apply(shader, pointLight.index());
    }
    for (auto const &spotLight: spotLights | boost::adaptors::indexed(0)) {
        spotLight.value().apply(shader, spotLight.index());
    }
}

//======================================================================================================================
// Render structures

struct Vertex_NT {
    float position[3];
    float normal[3];
    float texture[2];
};

struct Vertex_C {
    float position[3];
    float color[3];
};

struct RenderResource {
    GLuint vao{};
    GLuint vbo{};
    GLenum mode;
    int drawCount{};
    GLuint diffuseMap = -1;
    GLuint specularMap = -1;

    RenderResource() = delete;

    RenderResource(const vector<Vertex_NT> &vertices, GLenum mode, GLuint diffuseMap, GLuint specularMap)
            : mode{mode}, diffuseMap{diffuseMap}, specularMap{specularMap} {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
        drawCount = vertices.size();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    RenderResource(const vector<Vertex_C> &vertices, GLenum mode)
            : mode{mode} {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
        drawCount = vertices.size();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void draw() const {
        if (diffuseMap >= 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
        }
        if (specularMap >= 0) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap);
        }
        glBindVertexArray(vao);
        glDrawArrays(mode, 0, drawCount);
    }

    void free() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
};

vector<Vertex_NT> getCubeVertices() {
    return {
            {{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f,  -1.0f}, {0.0f, 0.0f}},
            {{0.5f,  -0.5f, -0.5f}, {0.0f,  0.0f,  -1.0f}, {1.0f, 0.0f}},
            {{0.5f,  0.5f,  -0.5f}, {0.0f,  0.0f,  -1.0f}, {1.0f, 1.0f}},
            {{0.5f,  0.5f,  -0.5f}, {0.0f,  0.0f,  -1.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f,  -0.5f}, {0.0f,  0.0f,  -1.0f}, {0.0f, 1.0f}},
            {{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f,  -1.0f}, {0.0f, 0.0f}},

            {{-0.5f, -0.5f, 0.5f},  {0.0f,  0.0f,  1.0f},  {0.0f, 0.0f}},
            {{0.5f,  -0.5f, 0.5f},  {0.0f,  0.0f,  1.0f},  {1.0f, 0.0f}},
            {{0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f},  {1.0f, 1.0f}},
            {{0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f},  {1.0f, 1.0f}},
            {{-0.5f, 0.5f,  0.5f},  {0.0f,  0.0f,  1.0f},  {0.0f, 1.0f}},
            {{-0.5f, -0.5f, 0.5f},  {0.0f,  0.0f,  1.0f},  {0.0f, 0.0f}},

            {{-0.5f, 0.5f,  0.5f},  {-1.0f, 0.0f,  0.0f},  {0.0f, 0.0f}},
            {{-0.5f, 0.5f,  -0.5f}, {-1.0f, 0.0f,  0.0f},  {1.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f,  0.0f},  {1.0f, 1.0f}},
            {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f,  0.0f},  {1.0f, 1.0f}},
            {{-0.5f, -0.5f, 0.5f},  {-1.0f, 0.0f,  0.0f},  {0.0f, 1.0f}},
            {{-0.5f, 0.5f,  0.5f},  {-1.0f, 0.0f,  0.0f},  {0.0f, 0.0f}},

            {{0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}},
            {{0.5f,  0.5f,  -0.5f}, {1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},
            {{0.5f,  -0.5f, -0.5f}, {1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}},
            {{0.5f,  -0.5f, -0.5f}, {1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}},
            {{0.5f,  -0.5f, 0.5f},  {1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},
            {{0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}},

            {{-0.5f, -0.5f, -0.5f}, {0.0f,  -1.0f, 0.0f},  {0.0f, 0.0f}},
            {{0.5f,  -0.5f, -0.5f}, {0.0f,  -1.0f, 0.0f},  {1.0f, 0.0f}},
            {{0.5f,  -0.5f, 0.5f},  {0.0f,  -1.0f, 0.0f},  {1.0f, 1.0f}},
            {{0.5f,  -0.5f, 0.5f},  {0.0f,  -1.0f, 0.0f},  {1.0f, 1.0f}},
            {{-0.5f, -0.5f, 0.5f},  {0.0f,  -1.0f, 0.0f},  {0.0f, 1.0f}},
            {{-0.5f, -0.5f, -0.5f}, {0.0f,  -1.0f, 0.0f},  {0.0f, 0.0f}},

            {{-0.5f, 0.5f,  -0.5f}, {0.0f,  1.0f,  0.0f},  {0.0f, 0.0f}},
            {{0.5f,  0.5f,  -0.5f}, {0.0f,  1.0f,  0.0f},  {1.0f, 0.0f}},
            {{0.5f,  0.5f,  0.5f},  {0.0f,  1.0f,  0.0f},  {1.0f, 1.0f}},
            {{0.5f,  0.5f,  0.5f},  {0.0f,  1.0f,  0.0f},  {1.0f, 1.0f}},
            {{-0.5f, 0.5f,  0.5f},  {0.0f,  1.0f,  0.0f},  {0.0f, 1.0f}},
            {{-0.5f, 0.5f,  -0.5f}, {0.0f,  1.0f,  0.0f},  {0.0f, 0.0f}},
    };
}

vector<Vertex_NT> getFloorVertices() {
    return {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f,  0.0f}},
            {{0.5f,  -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {30.0f, 0.0f}},
            {{0.5f,  0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}, {30.0f, 30.0f}},
            {{0.5f,  0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}, {30.0f, 30.0f}},
            {{-0.5f, 0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f,  30.0f}},
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f,  0.0f}},
    };
}

vector<Vertex_NT> getCylinderVertices() {
    const int sectorNumber = CIRCLE_MAX;
    const float theta = M_PI * 2 / sectorNumber;
    const float inner = 0.8f;

    vector<Vertex_NT> vertices;
    vertices.reserve(6 * sectorNumber);

    // Black part is up
    for (int i = 0; i < sectorNumber; ++i) {
        float angle_s = theta * i;
        float angle_e = angle_s + theta;

        float c = cos(angle_s);
        float cp = cos(angle_e);
        float s = sin(angle_s);
        float sp = sin(angle_e);

        float tx = (1 + c) / 4;
        float tpx = (1 + cp) / 4;
        float tix = (1 + c * inner) / 4;
        float tipx = (1 + cp * inner) / 4;
        float ty = (1 + s) / 2;
        float tpy = (1 + sp) / 2;
        float tiy = (1 + s * inner) / 2;
        float tipy = (1 + sp * inner) / 2;

        vertices.push_back({{c,  s, 0},
                            {c,  s, 0},
                            {tx, ty}});
        vertices.push_back({{cp,  sp, 0},
                            {cp,  sp, 0},
                            {tpx, tpy}});
        vertices.push_back({{cp,   sp, 1},
                            {cp,   sp, 0},
                            {tipx, tipy}});
        vertices.push_back({{cp,   sp, 1},
                            {cp,   sp, 0},
                            {tipx, tipy}});
        vertices.push_back({{c,   s, 1},
                            {c,   s, 0},
                            {tix, tiy}});
        vertices.push_back({{c,  s, 0},
                            {c,  s, 0},
                            {tx, ty}});

        vertices.push_back({{c,   s, 1},
                            {0,   0, 1},
                            {tix, tiy}});
        vertices.push_back({{cp,   sp, 1},
                            {0,    0,  1},
                            {tipx, tipy}});

        vertices.push_back({{0,     0, 1},
                            {0,     0, 1},
                            {0.25f, 0.5f}});
    }

    // White part is down
    for (int i = 0; i < sectorNumber; ++i) {
        float angle_s = theta * i;
        float angle_e = angle_s + theta;

        float c = cos(angle_s);
        float cp = cos(angle_e);
        float s = sin(angle_s);
        float sp = sin(angle_e);

        float tx = (3 - c) / 4;
        float tpx = (3 - cp) / 4;
        float tix = (3 - c * inner) / 4;
        float tipx = (3 - cp * inner) / 4;

        float ty = (1 + s) / 2;
        float tpy = (1 + sp) / 2;
        float tiy = (1 + s * inner) / 2;
        float tipy = (1 + sp * inner) / 2;

        vertices.push_back({{c,   s, -1},
                            {c,   s, 0},
                            {tix, tiy}});
        vertices.push_back({{cp,   sp, -1},
                            {cp,   sp, 0},
                            {tipx, tipy}});
        vertices.push_back({{cp,  sp, 0},
                            {cp,  sp, 0},
                            {tpx, tpy}});
        vertices.push_back({{cp,  sp, 0},
                            {cp,  sp, 0},
                            {tpx, tpy}});
        vertices.push_back({{c,  s, 0},
                            {c,  s, 0},
                            {tx, ty}});
        vertices.push_back({{c,   s, -1},
                            {c,   s, 0},
                            {tix, tiy}});

        vertices.push_back({{cp,   sp, -1},
                            {0,    0,  -1},
                            {tipx, tipy}});
        vertices.push_back({{c,   s, -1},
                            {0,   0, -1},
                            {tix, tiy}});
        vertices.push_back({{0,     0, -1},
                            {0,     0, -1},
                            {0.75f, 0.5f}});
    }

    return
            vertices;
}

vector<Vertex_C> getAxisFrames() {
    return {
            {{0.0f,    0.0f,    0.0f},    {0.0f, 0.0f, 1.0f}},
            {{0.0f,    0.0f,    1000.0f}, {0.0f, 0.0f, 1.0f}},
            {{0.0f,    0.0f,    0.0f},    {0.0f, 1.0f, 0.0f}},
            {{0.0f,    1000.0f, 0.0f},    {0.0f, 1.0f, 0.0f}},
            {{0.0f,    0.0f,    0.0f},    {1.0f, 0.0f, 0.0f}},
            {{1000.0f, 0.0f,    0.0f},    {1.0f, 0.0f, 0.0f}},
    };
}

vector<Vertex_NT> getUniformSquareVertices(const glm::vec2 tex) {
    return {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {tex[0], tex[1]}},
            {{0.5f,  -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {tex[0], tex[1]}},
            {{0.5f,  0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}, {tex[0], tex[1]}},
            {{0.5f,  0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}, {tex[0], tex[1]}},
            {{-0.5f, 0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}, {tex[0], tex[1]}},
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {tex[0], tex[1]}},
    };
}

unsigned int loadTexture(char const *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        switch (nrComponents) {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
            default:
                cout << "Unexpected number of components " << nrComponents << "!" << endl;
                stbi_image_free(data);
                return -1;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

//======================================================================================================================
// Display thread

std::function<void()> loop;

void main_loop() { loop(); }

//#define __EMSCRIPTEN__

void displayThread(const bool *gameEnds) {

    // Init
    GLFWwindow *window = initGLFW();
    initOpenGLOptions();
    initLights();
    reshapeCore(screenWidth, screenHeight);

    int render_status = 0;
    refreshModel(false);

#ifdef __EMSCRIPTEN__
    printf("Init finished.\n");
#else

#endif

    // Load shaders, render objects ...
    Shader simpleShader("render/simple.vert", "render/simple.frag");
    Shader lightShader("render/simple.vert", "render/light.frag");
    Shader withColorShader("render/with_color.vert", "render/with_color.frag");

#ifdef __EMSCRIPTEN__
    printf("Shaders loaded.\n");
#else

#endif

    // Set-up shaders and textures
    lightShader.use();
    lightShader.setFloat("material.shininess", 32.0f);

    int boxDtex, boxStex, ceramicTileDtex, blankTex, paletteTex, stoneDtex, tableDtex;
    {
        auto wrapper = [](auto name, int glt) {
            int id = loadTexture(filesystem::path((boost::format("resources/textures/%1%") % name).str()).c_str());
            glActiveTexture(glt);
            glBindTexture(GL_TEXTURE_2D, id);
            return id;
        };
        boxDtex = wrapper("container.png", GL_TEXTURE0);
        boxStex = wrapper("container-specular.png", GL_TEXTURE1);
        ceramicTileDtex = wrapper("ceramic-tile.jpg", GL_TEXTURE2);
        blankTex = wrapper("black.png", GL_TEXTURE3);
        paletteTex = wrapper("palette.png", GL_TEXTURE4);
        stoneDtex = wrapper("simple-stone.png", GL_TEXTURE5);
        tableDtex = wrapper("table.png", GL_TEXTURE6);
    }

    RenderResource axisFrames = RenderResource(getAxisFrames(), GL_LINES);
    RenderResource blackValidHint = RenderResource(getUniformSquareVertices({0.9f, 0.1f}), GL_TRIANGLES, paletteTex,
                                                   paletteTex);
    RenderResource whiteValidHint = RenderResource(getUniformSquareVertices({0.1f, 0.3f}), GL_TRIANGLES, paletteTex,
                                                   paletteTex);
    RenderResource cursor = RenderResource(getUniformSquareVertices({0.3f, 0.3f}), GL_TRIANGLES, paletteTex,
                                           paletteTex);
    RenderResource cube = RenderResource(getCubeVertices(), GL_TRIANGLES, boxDtex, boxStex);
    RenderResource table = RenderResource(getCubeVertices(), GL_TRIANGLES, tableDtex, blankTex);
    RenderResource stone = RenderResource(getCylinderVertices(), GL_TRIANGLES, stoneDtex, boxStex);
    RenderResource light = RenderResource(getCubeVertices(), GL_TRIANGLES, -1, -1);
    RenderResource floor = RenderResource(getFloorVertices(), GL_TRIANGLES, ceramicTileDtex, blankTex);

#ifdef __EMSCRIPTEN__
    printf("Textures loaded.\n");
#else

#endif

#ifdef __EMSCRIPTEN__
    // Init game
    Player *player0, *player1;
    auto p0 = PLAYER_HUMAN, p1 = PLAYER_AI;
    if (p0 == PLAYER_HUMAN) player0 = new Player(PLAYER_HUMAN, BLACK_SIDE);
    else player0 = new AI(BLACK_SIDE);

    if (p1 == PLAYER_HUMAN) player1 = new Player(PLAYER_HUMAN, WHITE_SIDE);
    else player1 = new AI(WHITE_SIDE);

    Player *player[2] = {player0, player1};
    Game::gameStart(player[0]->whoami(), player[1]->whoami());

    bool endGameNotice = false;
#else

#endif

    // Main loop
    loop = [&] {
        // Per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        processInput(window);

#ifdef __EMSCRIPTEN__
        // Update game
        if (Game::canContinue()) {
//            player[Game::playerIsWho()]->print();
            if (Game::canPlayerPlay(Game::getSideFlag())) {
                Piece p = (player[Game::playerIsWho()])->getPiece();
                if (!Game::setPiece(p)) {
                    if (Game::getSideFlag() == BLACK_SIDE) {
                        Game::getBoard().blackRecord.response.push_back({p.getX(), p.getY()});
                        Game::getBoard().whiteRecord.request.push_back({p.getX(), p.getY()});
                    } else {
                        Game::getBoard().whiteRecord.response.push_back({p.getX(), p.getY()});
                        Game::getBoard().blackRecord.request.push_back({p.getX(), p.getY()});
                    }
                    Game::switchSide();
                }
            } else {
                printf("You have no place to toss your piece!\n");
                Game::jump();
                Game::switchSide();
            }
        } else {
            if (!endGameNotice) {
                if (Game::getBoard().getBlackcount() > Game::getBoard().getWhitecount()) printf("Black Wins!!!\n");
                else if (Game::getBoard().getBlackcount() < Game::getBoard().getWhitecount()) printf("White Wins!!!\n");
                else printf("Tie!!!\n");
                endGameNotice = true;
            }
        }
#else

#endif

        // Bind camera to spotLight0
        spotLights[0].position = camera.Position;
        spotLights[0].direction = camera.Front;

        float lr_ratio = (float) screenWidth / (float) screenSize * camera.Zoom;
        float bt_ratio = (float) screenHeight / (float) screenSize * camera.Zoom;
        glm::mat4 projMat = glm::frustum(-lr_ratio, lr_ratio, -bt_ratio, bt_ratio, 1.0f, 1000.0f);

        glm::mat4 viewMat = camera.GetViewMatrix();

        lightShader.use();
        lightShader.setVec3("viewPos", camera.Position);
        lightShader.setMat4("projection", projMat);
        lightShader.setMat4("view", viewMat);

        simpleShader.use();
        simpleShader.setMat4("projection", projMat);
        simpleShader.setMat4("view", viewMat);

        withColorShader.use();
        withColorShader.setMat4("projection", projMat);
        withColorShader.setMat4("view", viewMat);

        // Update scene
        render_status = updateRenderStatus(render_status);

        // Render
        glClearColor(backGroundColor, backGroundColor, backGroundColor, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        updateFog(); // TODO
        updateLights(lightShader);

        // Objects needed to be lightened
        lightShader.use();
        lightShader.setInt("material.diffuse", 0);
        lightShader.setInt("material.specular", 1);

        // Table
        {
            lightShader.use();
            auto tableModelMat = glm::mat4(1.0f);
            tableModelMat = glm::translate(tableModelMat, glm::vec3{0.0, 0.0, TABLE_HEIGHT});
            tableModelMat = glm::scale(tableModelMat, glm::vec3{TABLE_SIZE, TABLE_SIZE, TABLE_THICKNESS});
            lightShader.setMat4("model", tableModelMat);
            table.draw();
        }

        // Cursor
        if (Game::getGameStatus() == Playing) {
            double alignx = int(floatingx * BOARD_SIZE + BOARD_SIZE) / (BOARD_SIZE + 0.0) - 1.0;
            double aligny = int(floatingy * BOARD_SIZE + BOARD_SIZE) / (BOARD_SIZE + 0.0) - 1.0;
            double tiny = 1.0 / BOARD_SIZE;

//            printf("alignx: %.2f, aligny: %.2f.\n", alignx, aligny);
            if (alignx >= -0.5 && aligny >= -0.5 && alignx <= 0.5 - 1.0 / BOARD_SIZE &&
                aligny <= 0.5 - 1.0 / BOARD_SIZE) {
                lightShader.use();
                auto cursorModelMat = glm::mat4(1.0f);
                cursorModelMat = glm::translate(cursorModelMat, glm::vec3{0.0, 0.0, TABLE_HEIGHT});
                cursorModelMat = glm::scale(cursorModelMat, glm::vec3{TABLE_SIZE, TABLE_SIZE, TABLE_THICKNESS});
                cursorModelMat = glm::translate(cursorModelMat, glm::vec3{0.0, 0.0, 0.5});
                cursorModelMat = glm::scale(cursorModelMat,
                                            glm::vec3{static_cast<float>(BOARD_SIZE * STONE_INTERVAL / TABLE_SIZE)});
                cursorModelMat = glm::translate(cursorModelMat,
                                                glm::vec3{alignx + tiny / 2, aligny + tiny / 2, 0.01});
                cursorModelMat = glm::scale(cursorModelMat, glm::vec3{static_cast<float>(tiny)});
                lightShader.setMat4("model", cursorModelMat);
                cursor.draw();
            }
        }

        // Stone
        if (Game::getGameStatus() != Idle) {
            for (auto s : stones) {
                if ((s.getColor() == BlackValid) || (s.getColor() == WhiteValid) || (s.getColor() == Valid)) {
                    if ((Game::getGameStatus() == Playing) && (Settings::pieceAssistance)) {
                        if (getValidTag(Game::getSideFlag()) & s.getColor()) {
                            auto hintModelMat = glm::mat4(1.0f);
                            hintModelMat = glm::translate(hintModelMat, {s.getX(), s.getY(), s.getZ()});
                            hintModelMat = glm::translate(hintModelMat, {0.0, 0.0, 0.0001});
                            hintModelMat = glm::scale(hintModelMat, {STONE_INTERVAL, STONE_INTERVAL, 1});
                            lightShader.use();
                            lightShader.setMat4("model", hintModelMat);

                            if (Game::getSideFlag() == BLACK_SIDE) {
                                blackValidHint.draw();
                            }
                            if (Game::getSideFlag() == WHITE_SIDE) {
                                whiteValidHint.draw();
                            }
                        }
                    }
                } else {
                    auto stoneModelMat = glm::mat4(1.0f);
                    stoneModelMat = glm::translate(stoneModelMat, glm::vec3{s.getX(), s.getY(), s.getZ()});
                    stoneModelMat = glm::rotate(stoneModelMat, glm::radians(
                            atan2f(s.getAxisy(), s.getAxisx() / M_PI) * 180.0f - 90.0f), {0.0f, 0.0f, 1.0f});

                    if ((s.getAxisy() != 0.0) && (s.getAxisx() != 0.0)) {
                        float degree = (atan2f(s.getAxisz(),
                                               sqrtf(s.getAxisx() *
                                                     s.getAxisx() +
                                                     s.getAxisy() *
                                                     s.getAxisy()))) *
                                       180.0f / M_PI;
                        stoneModelMat = glm::rotate(stoneModelMat,
                                                    glm::radians(degree),
                                                    glm::vec3{s.getAxisy(), -s.getAxisx(), 0.0});
                    }
                    stoneModelMat = glm::rotate(stoneModelMat, glm::radians((float) s.getAngle()),
                                                glm::vec3{0.0f, 1.0f, 0.0f});
                    stoneModelMat = glm::translate(stoneModelMat, glm::vec3{0.0, 0.0, STONE_HEIGHT});
                    if (s.getColor() == White) {
                        stoneModelMat = glm::rotate(stoneModelMat, glm::radians(180.0f), glm::vec3{0.0, 1.0, 0.0});
                    }
                    stoneModelMat = glm::scale(stoneModelMat, glm::vec3{STONE_RADIUS, STONE_RADIUS, STONE_HEIGHT});

                    lightShader.use();
                    lightShader.setMat4("model", stoneModelMat);
                    stone.draw();
                }
            }
        }

        // Floor
        {
            auto floorModelMat = glm::mat4(1.0f);
            floorModelMat = glm::scale(floorModelMat, glm::vec3(60.0f));
            lightShader.use();
            lightShader.setMat4("model", floorModelMat);
            floor.draw();
        }

        // Objects that has intrinsic colors
        simpleShader.use();
        for (auto const &pointLight : pointLights) {
            if (!pointLight.enabled) {
                continue;
            }
            auto lightModelMat = glm::mat4(1.0f);
            lightModelMat = glm::translate(lightModelMat, pointLight.position);
            lightModelMat = glm::scale(lightModelMat, glm::vec3(0.3f));
            simpleShader.setMat4("model", lightModelMat);
            simpleShader.setVec4("uniformColor", pointLight.diffuse[0], pointLight.diffuse[1], pointLight.diffuse[2],
                                 1.0f);
            light.draw();
        }
        for (auto const &spotLight : spotLights) {
            if (!spotLight.enabled) {
                continue;
            }
            auto lightModelMat = glm::mat4(1.0f);
            lightModelMat = glm::translate(lightModelMat, spotLight.position);
            lightModelMat = glm::scale(lightModelMat, glm::vec3(0.3f));
            simpleShader.setMat4("model", lightModelMat);
            simpleShader.setVec4("uniformColor", spotLight.diffuse[0], spotLight.diffuse[1], spotLight.diffuse[2],
                                 1.0f);
            light.draw();
        }

        withColorShader.use();
        if (Settings::showAxis) {
            withColorShader.setMat4("model", glm::identity<glm::mat4>());
            axisFrames.draw();
        }

        glfwSwapBuffers(window);
    };

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 60, true);
#else
    while (!glfwWindowShouldClose(window) && !*gameEnds) {
        main_loop();
    }
#endif

    // Clean up
    cube.free();
    axisFrames.free();
    light.free();

#ifdef __EMSCRIPTEN__
    delete player0;
    delete player1;
    Game::quitNormal();
#else

#endif

    glfwTerminate();
}

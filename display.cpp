#include "display.h"
#include "element.h"
#include "base.h"
#include "game.h"
#include "model.h"
#include "shader/shader.h"
#include <cmath>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <unordered_map>
#include "camera.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

int screenSize = 80 * BOARD_SIZE, screenWidth = 80 * BOARD_SIZE, screenHeight = 80 * BOARD_SIZE;
double zoom = DEFAULT_ZOOM;
double theta = -95.5, fai = 20.0;
double floatingx = 0.0, floatingy = 0.0;
float backGroundColor = 0.104;
float fogDensity = 0.00f;
bool isFocus = false;
GLfloat fogColorPause[] = {backGroundColor, backGroundColor, backGroundColor, 1.0f};
GLfloat fogColorFocus[] = {0.1f, 0.15f, 0.2f, 0.0f};
char s_Save[] = "Save";
char s_Load[] = "Load";
char s_Save_and_Quit[] = "Save and Quit";
char s_Quit_without_saving[] = "Quit without Saving";

extern double xcenter, ycenter, zcenter;
extern vector<Stone> stones;


extern void refreshModel(bool lifting);

extern int updateRenderStatus(int status);

extern void keyboardCallback(unsigned char key, int _x, int _y);

extern void keyboardUpCallback(unsigned char key, int _x, int _y);

extern void skeyboardCallback(int key, int _x, int _y);

extern void skeyboardUpCallback(int key, int _x, int _y);

// debugging camera
Camera camera;
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

inline void setVertexColor(double x, double y) {
//    double p = pow(((pow(x, 8) + pow(y, 8)) / 2.0), 0.125) / FLOOR_SHADE_SIZE;
//    double temp = (p < 2) ? (-(p-2)*(p-2) + 1) : 1.0;
//    glColor3d(temp, temp, temp);
}





// New static variables

// lights
int directionalLightCap;
int pointLightCap;
int spotLightCap;
vector<DirectionalLight> directionalLights;
vector<PointLight> pointLights;
vector<SpotLight> spotLights;

// matrices
glm::mat4 projMat;


//======================================================================================================================
// Declarations

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void reshape_callback(GLFWwindow *window, int width, int height);

//======================================================================================================================
// Init

void getTextureMap(vector<unsigned int> &mapId);

void initLights() {
    glm::vec3 ambient = {0.25f, 0.25f, 0.25f};
    glm::vec3 diffuse = {0.8f, 0.8f, 0.8f};
    glm::vec3 specular = {0.7f, 0.7f, 0.7f};
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

    pointLights = {
            {.enabled = false},
            {.enabled = false},
            {.enabled = false},
            {.enabled = false},
    };
    pointLightCap = 0;

    glm::vec3 e1 = {1.0, 0.0, 0.0};
    glm::vec3 e2 = {0.0, 1.0, 0.0};
    glm::vec3 e3 = {0.0, 0.0, 1.0};
    spotLights = {
            {
                    .position {3, 3 * sqrt(3), 3},
                    .direction {-3, -3 * sqrt(3), -3 + 1},
                    .ambient = ambient * e1,
                    .diffuse = diffuse * e1,
                    .specular = specular * e1,
                    .constant = 1.0f,
                    .linear = 0.09f,
                    .quadratic = 0.032,
                    .cutOff = glm::cos(glm::radians(60.0f)),
                    .outerCutOff = glm::cos(glm::radians(90.0f)),
            },
            {
                    .position {3, -3 * sqrt(3), 3},
                    .direction {-3, 3 * sqrt(3), -3 + 1},
                    .ambient = ambient * e2,
                    .diffuse = diffuse * e2,
                    .specular = specular * e2,
                    .constant = 1.0f,
                    .linear = 0.09f,
                    .quadratic = 0.032,
                    .cutOff = glm::cos(glm::radians(60.0f)),
                    .outerCutOff = glm::cos(glm::radians(90.0f)),
            },
            {
                    .position {-6, 0, 3},
                    .direction {6, 0, -3 + 1},
                    .ambient = ambient * e3,
                    .diffuse = diffuse * e3,
                    .specular = specular * e3,
                    .constant = 1.0f,
                    .linear = 0.09f,
                    .quadratic = 0.032,
                    .cutOff = glm::cos(glm::radians(60.0f)),
                    .outerCutOff = glm::cos(glm::radians(90.0f)),
            },
            {.enabled = false},
    };
    spotLightCap = 3;
}

GLFWwindow *initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_DOUBLEBUFFER, true);
    glfwWindowHint(GLFW_REFRESH_RATE, 60);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeCallback(window, reshape_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
// ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }

    return window;
}

void initOpenGLOptions() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//     glEnable(GL_MULTISAMPLE);
//     glEnable(GL_POINT_SMOOTH);
//     glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
//     glEnable(GL_LINE_SMOOTH);
//     glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//     glEnable(GL_POLYGON_SMOOTH);
//     glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
//
//     glEnable(GL_NORMALIZE);

    glEnable(GL_DEPTH_TEST);
//     glEnable(GL_COLOR_MATERIAL);
//     glShadeModel(GL_SMOOTH);
}

//======================================================================================================================
// Callbacks

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (Game::getGameStatus() == Lifting) {
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
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    camera.ProcessMouseScroll(yoffset);
}

void reshape_callback(GLFWwindow *window, int width, int height) {
    screenSize = min(width, height);
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
    float lr_ratio = (float) screenWidth / (float) screenSize;
    float bt_ratio = (float) screenHeight / (float) screenSize;
    projMat = glm::frustum(-lr_ratio, lr_ratio, -bt_ratio, bt_ratio, 1.0f, 1000.0f);
}

// Render

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

void drawStone() {
//     if (Game::getGameStatus() == Idle) return;
//     for (auto stone : stones) {
//         if ((stone.getColor() == BlackValid) || (stone.getColor() == WhiteValid) || (stone.getColor() == Valid)) {
//             if ((Game::getGameStatus() == Playing) && (Settings::pieceAssistance)) {
//                 if (getValidTag(Game::getSideFlag()) & stone.getColor()) {
//                     glMatrixMode(GL_MODELVIEW);
//                     glPushMatrix();
//                         glTranslated(stone.getX(), stone.getY(), stone.getZ());

//                         glTranslated(0.0, 0.0, 0.0001);
//                         if (Game::getSideFlag() == BLACK_SIDE) glColor3d(0.2, 0.2, 0.2);
//                         if (Game::getSideFlag() == WHITE_SIDE) glColor3d(0.8, 0.8, 0.8);
//                         glBegin(GL_QUADS);
//                             glVertex3d(-STONE_INTERVAL / 2.0, -STONE_INTERVAL / 2.0, 0.0);
//                             glVertex3d(-STONE_INTERVAL / 2.0, STONE_INTERVAL / 2.0, 0.0);
//                             glVertex3d(STONE_INTERVAL / 2.0, STONE_INTERVAL / 2.0, 0.0);
//                             glVertex3d(STONE_INTERVAL / 2.0, -STONE_INTERVAL / 2.0, 0.0);
//                         glEnd();
//                     glPopMatrix();
//                 }
//             }
//             continue;
//         }
//         glMatrixMode(GL_MODELVIEW);
//         glPushMatrix();
//             glTranslated(stone.getX(), stone.getY(), stone.getZ());
//             if (Settings::showBigBall) { glLineWidth(2); glutWireSphere(10.0, 30, 30); }
//             glRotated((atan2(stone.getAxisy(), stone.getAxisx() / M_PI) * 180 - 90), 0.0, 0.0, 1.0);
//             if ((stone.getAxisy()!=0.0) && (stone.getAxisx()!=0.0))
//                 glRotated((atan2(stone.getAxisz(), sqrt(stone.getAxisx()*stone.getAxisx()+stone.getAxisy()*stone.getAxisy()))) * 180 / M_PI, stone.getAxisy(), -stone.getAxisx(), 0.0);
//             glRotated(stone.getAngle(), 0.0, 1.0, 0.0);
//             glTranslated(0.0, 0.0, STONE_HEIGHT / 4);

//             if (stone.getColor() == White) { glScaled(1.0, 1.0, -1.0); glTranslated(0.0, 0.0, -STONE_HEIGHT / 2); }
//             glPushMatrix();
//                 glTranslated(0.0, 0.0, STONE_HEIGHT / 4);
//                 glColor3d(0.0, 0.0, 0.0);
//                 glutSolidCylinder(STONE_RADIUS, STONE_HEIGHT / 2, 20, 1);
//             glPopMatrix();
//             glTranslated(0.0, 0.0, -STONE_HEIGHT / 4);
//             glColor3d(1.0, 1.0, 1.0);
//             glutSolidCylinder(STONE_RADIUS, STONE_HEIGHT / 2, 20, 1);
//         glPopMatrix();
//     }
}

void drawCursor() {
//     if (Game::getGameStatus() != Playing) return;

//     glEnable(GL_LIGHTING);
//     glTranslated(0.0, 0.0, 0.01);

//     double alignx = int(floatingx * BOARD_SIZE + BOARD_SIZE) / (BOARD_SIZE + 0.0) - 1.0;
//     double aligny = int(floatingy * BOARD_SIZE + BOARD_SIZE) / (BOARD_SIZE + 0.0) - 1.0;
//     double tiny = 1.0 / BOARD_SIZE;

//     if (alignx < -0.5) return;
//     if (aligny < -0.5) return;
//     if (alignx > 0.5 - 1.0 / BOARD_SIZE) return;
//     if (aligny > 0.5 - 1.0 / BOARD_SIZE) return;
//     glBegin(GL_QUAD_STRIP);
//         glColor4d(0.0, 1.0, 1.0, 0.7);
//         glVertex3d(alignx, aligny, 0.0);
//         glVertex3d(alignx+tiny, aligny, 0.0);
//         glVertex3d(alignx, aligny+tiny, 0.0);
//         glVertex3d(alignx+tiny, aligny+tiny, 0.0);
//     glEnd();
}

void drawTable() {
//     glMatrixMode(GL_MODELVIEW);
//     if (Settings::showAxis) {
//         glDisable(GL_LIGHTING);
//         glPushMatrix();
//             glColor3d(1.0, 0.0, 0.0);
//             glBegin(GL_LINES);
//                 glVertex3d(0.0, 0.0, 0.0);
//                 glVertex3d(1000.0, 0.0, 0.0);
//             glEnd();
//             glColor3d(0.0, 1.0, 0.0);
//             glBegin(GL_LINES);
//                 glVertex3d(0.0, 0.0, 0.0);
//                 glVertex3d(0.0, 1000.0, 0.0);
//             glEnd();
//             glColor3d(0.0, 0.0, 1.0);
//             glBegin(GL_LINES);
//                 glVertex3d(0.0, 0.0, 0.0);
//                 glVertex3d(0.0, 0.0, 1000.0);
//             glEnd();
//         glPopMatrix();
//     }

//     glEnable(GL_LIGHTING);

//     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1.0);
//     glPushMatrix();
//     glTranslated(0.0, 0.0, TABLE_HEIGHT);
//     glScaled(TABLE_SIZE , TABLE_SIZE , TABLE_THICKNESS);
//     glColor4d(0.6, 0.6, 0.6, 1.0);
//     glutWireCube(1);
//     glColor4d(1.0, 1.0, 1.0, 0.6);
//     glutSolidCube(1);
//     glTranslated(0.0, 0.0, 0.5);
//     double scaleCoeffiency = BOARD_SIZE * STONE_INTERVAL / TABLE_SIZE;
//     glScaled(scaleCoeffiency, scaleCoeffiency, scaleCoeffiency);
//     glLineWidth(3);
//     glBegin(GL_LINES);
//         for (double i = 1.0; i < BOARD_SIZE; ++i) {
//             glVertex3d(i / BOARD_SIZE - 0.5, -0.5, 0.0);
//             glVertex3d(i / BOARD_SIZE - 0.5, 0.5, 0.0);
//         }
//     glEnd();

//     glBegin(GL_LINES);
//         for (double i = 1.0; i < BOARD_SIZE; ++i) {
//             glVertex3d(-0.5, i / BOARD_SIZE - 0.5, 0.0);
//             glVertex3d(0.5, i / BOARD_SIZE - 0.5, 0.0);
//         }
//     glEnd();
//     drawCursor();
//     glPopMatrix();
}

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
    GLuint ibo{};
    GLenum mode;
    int drawCount{};

    RenderResource() = delete;

    RenderResource(const vector<Vertex_NT> &vertices, const vector<GLushort> &indices, GLenum mode) : mode{mode} {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
        drawCount = indices.size();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    RenderResource(const vector<Vertex_C> &vertices, const vector<GLushort> &indices, GLenum mode) : mode{mode} {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
        drawCount = indices.size();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void draw() const {
        glBindVertexArray(vao);
        glDrawElements(mode, drawCount, GL_UNSIGNED_SHORT, nullptr);
    }

    void free() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }
};

RenderResource getCube(float cubeSize_2) {
    const vector<Vertex_NT> vertices = {
            {-0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f},
            {0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f},
            {0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f},
            {0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f},
            {-0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f},
            {-0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f},

            {-0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f},
            {0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f},
            {0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f},
            {0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f},
            {-0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f},
            {-0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f},

            {-0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f},
            {-0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 0.0f},
            {-0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f},
            {-0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f},
            {-0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 1.0f},
            {-0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f},

            {0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f},
            {0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f},
            {0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f},
            {0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f},
            {0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f},
            {0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f},

            {-0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 0.0f},
            {0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 0.0f},
            {0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 1.0f},
            {0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 1.0f},
            {-0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 1.0f},
            {-0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 0.0f},

            {-0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f},
            {0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f},
            {0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f},
            {0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f},
            {-0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f},
            {-0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f},
    };

    vector<GLushort> indices(36);
    for (int i = 0; i < indices.size(); ++i) {
        indices[i] = i;
    }

    return RenderResource(vertices, indices, GL_TRIANGLES); // NOLINT(modernize-return-braced-init-list)
}

RenderResource getFloor(float floorSize) {
    const vector<Vertex_NT> vertices = {
            {-floorSize / 2, -floorSize / 2, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
            {floorSize / 2,  -floorSize / 2, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
            {floorSize / 2,  floorSize / 2,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
            {floorSize / 2,  floorSize / 2,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
            {-floorSize / 2, floorSize / 2,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
            {-floorSize / 2, -floorSize / 2, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
    };

    vector<GLushort> indices(6);
    for (int i = 0; i < indices.size(); ++i) {
        indices[i] = i;
    }

    return RenderResource(vertices, indices, GL_TRIANGLES); // NOLINT(modernize-return-braced-init-list)
}

RenderResource getAxisFrames() {
    const vector<Vertex_C> vertices = {
            {{0.0f,    0.0f,    0.0f},    {0.0f, 0.0f, 1.0f}},
            {{0.0f,    0.0f,    1000.0f}, {0.0f, 0.0f, 1.0f}},
            {{0.0f,    0.0f,    0.0f},    {0.0f, 1.0f, 0.0f}},
            {{0.0f,    1000.0f, 0.0f},    {0.0f, 1.0f, 0.0f}},
            {{0.0f,    0.0f,    0.0f},    {1.0f, 0.0f, 0.0f}},
            {{1000.0f, 0.0f,    0.0f},    {1.0f, 0.0f, 0.0f}},
    };

    const vector<GLushort> indices = {0, 1, 2, 3, 4, 5};

    return RenderResource(vertices, indices, GL_LINES); // NOLINT(modernize-return-braced-init-list)
}

unsigned int loadTexture(char const *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

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

enum class TEXTURE {
    BoxDiffuse, BoxSpecular, CeramicTileDiffuse, Blank, Palette
};

auto getTextureMap() {
    vector<std::tuple<TEXTURE, string>> config = {
            {TEXTURE::BoxDiffuse,         "container.png"},
            {TEXTURE::BoxSpecular,        "container_specular.png"},
            {TEXTURE::CeramicTileDiffuse, "ceramic-tile.jpg"},
            {TEXTURE::Blank,              "black.jpg"},
            {TEXTURE::Palette,            "palette.png"},
    };

    int n = config.size();
    vector<unsigned int> mapId(n);

    for (auto const &t : config | boost::adaptors::indexed(0)) {
        auto filepath = boost::format("resources/textures/%1%") % get<1>(t.value());
        auto id = loadTexture(filesystem::path(filepath.str()).c_str());
        mapId.emplace_back(id);
        glActiveTexture(GL_TEXTURE0 + t.index());
        glBindTexture(GL_TEXTURE_2D, id);
    }

    return mapId;
}

void displayThread(const bool *gameEnds) {
    // glutInit(&argc, argv); dep
    // glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    // glutInitWindowPosition(50, 50); +
    // glutInitWindowSize(screenSize, screenSize); done
    // glutCreateWindow("Othello"); done
    // glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS); done
    // glutDisplayFunc(&display);
    // glutMouseFunc(&mouseKey);
    // glutMotionFunc(&mouseMotion);
    // glutPassiveMotionFunc(&mouseMotion);
    // glutReshapeFunc(&reshape);
    // glutKeyboardFunc(&keyboardCallback);
    // glutSpecialFunc(&skeyboardCallback);
    // glutKeyboardUpFunc(&keyboardUpCallback);
    // glutSpecialUpFunc(&skeyboardUpCallback);
    // initOpenGLOptions();+
    // refreshModel(false);done
    // glutTimerFunc(0, &updateRenderStatus, 0);done
    // glutMainLoop();deprecated

    // Init
    GLFWwindow *window = initGLFW();
    initOpenGLOptions();
    initLights();

    int render_status = 0;
    refreshModel(false);

    // Load shaders, render objects ...
    Shader simpleShader("shader/simple.vert", "shader/simple.frag");
    Shader lightShader("shader/simple.vert", "shader/light.frag");
    Shader withColorShader("shader/with_color.vert", "shader/with_color.frag");

    RenderResource axisFrames = getAxisFrames();
    RenderResource cube = getCube(0.5f);
    RenderResource light = getCube(0.2f);
    RenderResource floor = getFloor(2 * FLOOR_SIZE);

    glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    // Set-up shaders and textures
    lightShader.use();
    lightShader.setFloat("material.shininess", 32.0f);

    auto mapId = getTextureMap();

    // Main loop
    while (!glfwWindowShouldClose(window) && !*gameEnds) {
        // Per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input and set projection+view to shader
        processInput(window);

        {//        if (Game::getGameStatus() == Lifting) {
//            camera.Position = {zoom * cos(theta / 180.0 * M_PI) * sin(fai / 180.0 * M_PI) + xcenter,
//                               zoom * sin(theta / 180.0 * M_PI) * sin(fai / 180.0 * M_PI) + ycenter,
//                               zoom * cos(fai / 180.0 * M_PI) + zcenter};
//            glm::vec3 center = {xcenter, ycenter, zcenter};
//            camera.Front = center - camera.Position;
//        } else {
//            camera.Position = {zoom * cos(theta / 180.0 * M_PI) * sin(fai / 180.0 * M_PI),
//                               zoom * sin(theta / 180.0 * M_PI) * sin(fai / 180.0 * M_PI),
//                               zoom * cos(fai / 180.0 * M_PI)};
//            glm::vec3 center = {0.0, 0.0, TABLE_HEIGHT + TABLE_THICKNESS / 2};
//            camera.Front = center - camera.Position;
//        }
        }

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

        drawTable(); // TODO
        drawStone(); // TODO

        // Objects needed to be lightened
        lightShader.use();

        // Cubes
        {
            lightShader.setInt("material.diffuse", static_cast<int>(TEXTURE::BoxDiffuse));
            lightShader.setInt("material.specular", static_cast<int>(TEXTURE::BoxSpecular));
            for (auto const &cubePosition : cubePositions | boost::adaptors::indexed(0)) {
                int i = cubePosition.index();
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i + currentFrame * 20;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                lightShader.setMat4("model", model);

                cube.draw();
            }
        }

        // Floor
        {
            lightShader.setInt("material.diffuse", static_cast<int>(TEXTURE::CeramicTileDiffuse));
            lightShader.setInt("material.specular", static_cast<int>(TEXTURE::Blank));
            lightShader.setMat4("model", glm::identity<glm::mat4>());
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

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    cube.free();
    axisFrames.free();
    light.free();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

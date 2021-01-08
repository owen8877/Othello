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

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <cmath>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/range/adaptor/indexed.hpp>

int screenSize = 80 * BOARD_SIZE, screenWidth = 80 * BOARD_SIZE, screenHeight = 80 * BOARD_SIZE;
double zoom = DEFAULT_ZOOM;
double theta = -95.5, fai = 20.0;
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

extern void keyboardCallback(unsigned char key, int _x, int _y);

extern void keyboardUpCallback(unsigned char key, int _x, int _y);

extern void skeyboardCallback(int key, int _x, int _y);

extern void skeyboardUpCallback(int key, int _x, int _y);

// debugging camera
Camera camera;
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;
bool clipping = true;

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
    };
    spotLightCap = 4;
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeCallback(window, reshape_callback);

    // tell GLFW to capture our mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
// Callbacks

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
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

void reshape_core(int width, int height) {
    screenSize = min(width, height);
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
    float lr_ratio = (float) screenWidth / (float) screenSize;
    float bt_ratio = (float) screenHeight / (float) screenSize;
    projMat = glm::frustum(-lr_ratio, lr_ratio, -bt_ratio, bt_ratio, 1.0f, 1000.0f);
}

void reshape_callback(GLFWwindow *window, int width, int height) {
    puts("Resized!");
    reshape_core(width, height);
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

vector<Vertex_NT> getSquareVertices() {
    return {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{0.5f,  -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{0.5f,  0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{0.5f,  0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
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

vector<Vertex_C> getBlackValidHint() {
    return {
            {{-0.5f, -0.5f, 0.0f}, {0.8f, 0.8f, 0.8f}},
            {{-0.5f, 0.5f,  0.0f}, {0.8f, 0.8f, 0.8f}},
            {{0.5f,  0.5f,  0.0f}, {0.8f, 0.8f, 0.8f}},
            {{0.5f,  -0.5f, 0.0f}, {0.8f, 0.8f, 0.8f}},
    };
}

vector<Vertex_C> getWhiteValidHint() {
    return {
            {{-0.5f, -0.5f, 0.0f}, {0.2f, 0.2f, 0.2f}},
            {{-0.5f, 0.5f,  0.0f}, {0.2f, 0.2f, 0.2f}},
            {{0.5f,  0.5f,  0.0f}, {0.2f, 0.2f, 0.2f}},
            {{0.5f,  -0.5f, 0.0f}, {0.2f, 0.2f, 0.2f}},
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

std::function<void()> loop;

void main_loop() { loop(); }

void displayThread(const bool *gameEnds) {
    // glutMouseFunc(&mouseKey);
    // glutMotionFunc(&mouseMotion);
    // glutPassiveMotionFunc(&mouseMotion);
    // glutReshapeFunc(&reshape);
    // glutKeyboardFunc(&keyboardCallback);
    // glutSpecialFunc(&skeyboardCallback);
    // glutKeyboardUpFunc(&keyboardUpCallback);
    // glutSpecialUpFunc(&skeyboardUpCallback);
    // glutTimerFunc(0, &updateRenderStatus, 0);done

    // Init
    GLFWwindow *window = initGLFW();
    initOpenGLOptions();
    initLights();
    reshape_core(screenWidth, screenHeight);

    int render_status = 0;
    refreshModel(false);

    printf("Init finished.\n");

    // Load shaders, render objects ...
    Shader simpleShader("render/simple.vert", "render/simple.frag");
    Shader lightShader("render/simple.vert", "render/light.frag");
    Shader withColorShader("render/with_color.vert", "render/with_color.frag");

    printf("Shaders loaded.\n");

    // Set-up shaders and textures
    lightShader.use();
    lightShader.setFloat("material.shininess", 32.0f);

    int boxDiffuseTex, boxSpecularTex, ceramicTileDiffuseTex, blankTex, paletteTex, simpleStoneTex;
    {
        auto wrapper = [](auto name, int glt) {
            int id = loadTexture(filesystem::path((boost::format("resources/textures/%1%") % name).str()).c_str());
            glActiveTexture(glt);
            glBindTexture(GL_TEXTURE_2D, id);
            return id;
        };
        boxDiffuseTex = wrapper("container.png", GL_TEXTURE0);
        boxSpecularTex = wrapper("container-specular.png", GL_TEXTURE1);
        ceramicTileDiffuseTex = wrapper("ceramic-tile.jpg", GL_TEXTURE2);
        blankTex = wrapper("black.png", GL_TEXTURE3);
        paletteTex = wrapper("palette.png", GL_TEXTURE4);
        simpleStoneTex = wrapper("simple-stone.png", GL_TEXTURE5);
    }

    RenderResource axisFrames = RenderResource(getAxisFrames(), GL_LINES);
    RenderResource blackValidHint = RenderResource(getBlackValidHint(), GL_QUADS);
    RenderResource whiteValidHint = RenderResource(getWhiteValidHint(), GL_QUADS);
    RenderResource cube = RenderResource(getCubeVertices(), GL_TRIANGLES, boxDiffuseTex, boxSpecularTex);
    RenderResource stone = RenderResource(getCylinderVertices(), GL_TRIANGLES, simpleStoneTex, boxSpecularTex);
    RenderResource light = RenderResource(getCubeVertices(), GL_TRIANGLES, -1, -1);
    RenderResource floor = RenderResource(getFloorVertices(), GL_TRIANGLES, ceramicTileDiffuseTex, blankTex);

    printf("Textures loaded.\n");

    // Main loop

    loop = [&] {
//        printf("Looping...\n");
        // Per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input and set projection+view to render
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

        // Bind camera to spotLight0
        spotLights[0].position = camera.Position;
        spotLights[0].direction = camera.Front;

        // TODO: Update zoom as well

//        glm::mat4 viewMat = camera.GetViewMatrix();
        glm::mat4 viewMat = glm::lookAt(camera.Position, {0, 0, 0}, {0, 0, 1});

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
        lightShader.setInt("material.diffuse", 0);
        lightShader.setInt("material.specular", 1);

        // Stone
        if (Game::getGameStatus() != Idle) {
            for (auto s : stones) {
                if ((s.getColor() == BlackValid) || (s.getColor() == WhiteValid) ||
                    (s.getColor() == Valid)) {
                    continue;
                    if ((Game::getGameStatus() == Playing) && (Settings::pieceAssistance)) {
                        if (getValidTag(Game::getSideFlag()) & s.getColor()) {
                            auto stoneModelMat = glm::mat4(1.0f);
                            stoneModelMat = glm::translate(stoneModelMat, {s.getX(), s.getY(), s.getZ()});
                            stoneModelMat = glm::translate(stoneModelMat, {0.0, 0.0, 0.0001});
                            stoneModelMat = glm::scale(stoneModelMat, {STONE_INTERVAL, STONE_INTERVAL, 1});
                            withColorShader.use();
                            withColorShader.setMat4("model", stoneModelMat);

                            if (Game::getSideFlag() == BLACK_SIDE) {
                                blackValidHint.draw();
                            }
                            if (Game::getSideFlag() == WHITE_SIDE) {
                                whiteValidHint.draw();
                            }
                        }
                    }
                } else {
//                if (Settings::showBigBall) {
//                    glLineWidth(2);
//                    glutWireSphere(10.0, 30, 30);
//                }
                    auto stoneModelMat = glm::mat4(1.0f);
                    stoneModelMat = glm::translate(stoneModelMat, glm::vec3{s.getX(), s.getY(), s.getZ()});
                    stoneModelMat = glm::rotate(stoneModelMat, glm::degrees(
                            atan2f(s.getAxisy(), s.getAxisx() / M_PI) * 180.0f - 90.0f), {0.0f, 0.0f, 1.0f});

                    if ((s.getAxisy() != 0.0) && (s.getAxisx() != 0.0)) {
                        float degree = (atan2f(s.getAxisz(),
                                               sqrtf(s.getAxisx() *
                                                     s.getAxisx() +
                                                     s.getAxisy() *
                                                     s.getAxisy()))) *
                                       180.0f / M_PI;
                        stoneModelMat = glm::rotate(stoneModelMat,
                                                    glm::degrees(degree),
                                                    glm::vec3{s.getAxisy(), -s.getAxisx(), 0.0});
                    }
                    stoneModelMat = glm::rotate(stoneModelMat, glm::degrees((float) s.getAngle()),
                                                glm::vec3{0.0f, 1.0f, 0.0f});
                    stoneModelMat = glm::translate(stoneModelMat, glm::vec3{0.0, 0.0, STONE_HEIGHT / 4});

                    if (s.getColor() == White) {
                        stoneModelMat = glm::rotate(stoneModelMat, glm::degrees(180.0f), glm::vec3{0.0, 1.0, 0.0});
                        stoneModelMat = glm::translate(stoneModelMat, glm::vec3{0.0, 0.0, -STONE_HEIGHT / 2});
//                        glScaled(1.0, 1.0, -1.0);
//                        glTranslated(0.0, 0.0, -STONE_HEIGHT / 2);
                    }

                    stoneModelMat = glm::scale(stoneModelMat, glm::vec3{STONE_RADIUS, STONE_RADIUS, STONE_HEIGHT});

                    lightShader.use();
                    lightShader.setMat4("model", stoneModelMat);
                    stone.draw();
//                    glPushMatrix();
//                    glTranslated(0.0, 0.0, STONE_HEIGHT / 4);
//                    glColor3d(0.0, 0.0, 0.0);
//                    glutSolidCylinder(STONE_RADIUS, STONE_HEIGHT / 2, 20, 1);
//                    glPopMatrix();
//                    glTranslated(0.0, 0.0, -STONE_HEIGHT / 4);
//                    glColor3d(1.0, 1.0, 1.0);
//                    glutSolidCylinder(STONE_RADIUS, STONE_HEIGHT / 2, 20, 1);
//                    glPopMatrix();
                }
            }


//            auto stoneModelMat = glm::mat4(1.0f);
//            stoneModelMat = glm::translate(stoneModelMat, {0, 0, 2});
//            stoneModelMat = glm::rotate(stoneModelMat, glm::degrees(0.0f), {0.0f, 1.0f, 0.0f});
//            lightShader.setMat4("model", stoneModelMat);
//            stone.draw();
        }

        // Floor
        {
            auto floorModelMat = glm::mat4(1.0f);
            floorModelMat = glm::scale(floorModelMat, glm::vec3(30.0f));
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

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    };

#ifdef EMSCRIPTEN
    emscripten_set_main_loop(main_loop, 10, true);
#else
    while (!glfwWindowShouldClose(window) && !*gameEnds) {
        main_loop();
    }
#endif

    // Clean up
    cube.free();
    axisFrames.free();
    light.free();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

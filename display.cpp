#include "display.h"
#include "element.h"
#include "base.h"
#include "game.h"
#include "model.h"
#include "shader/shader.h"
#include <cmath>
#include <vector>
#include <boost/filesystem.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <boost/format.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include "camera.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

int screenSize = 80 * BOARD_SIZE, screenWidth = 80 * BOARD_SIZE, screenHeight = 80 * BOARD_SIZE;
double zoom = DEFAULT_ZOOM;
double theta = -90.5, fai = 20.0;
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

// debugging camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

extern double xcenter, ycenter, zcenter;
extern vector<Stone> stones;

extern void refreshModel(bool lifting);

extern int updateRenderStatus(int status);

extern void keyboardCallback(unsigned char key, int _x, int _y);

extern void keyboardUpCallback(unsigned char key, int _x, int _y);

extern void skeyboardCallback(int key, int _x, int _y);

extern void skeyboardUpCallback(int key, int _x, int _y);


inline void setVertexColor(double x, double y) {
//    double p = pow(((pow(x, 8) + pow(y, 8)) / 2.0), 0.125) / FLOOR_SHADE_SIZE;
//    double temp = (p < 2) ? (-(p-2)*(p-2) + 1) : 1.0;
//    glColor3d(temp, temp, temp);
}

void initDisplay() {
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

//     GLfloat ambient [] = {0.3f, 0.3f, 0.3f, 1.0f};
//     GLfloat diffuse [] = {0.7f, 0.7f, 0.7f, 1.0f};
//     GLfloat specref[] = {1.0f, 1.0f, 1.0f, 1.0f};
//     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
//     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
//     //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specref);
//     glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 10);
//     glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

//     GLfloat light_0_position [] = {0.0, 0.0, 40.0, 1.0};
//     GLfloat light_1_position [] = {20.0, 20.0, 20.0, 1.0};
//     GLfloat light_2_position [] = {20.0, -20.0, 20.0, 1.0};
//     GLfloat light_3_position [] = {-20.0, 0.0, 20.0, 1.0};
//     GLfloat light_0_dir [] = {0.0, 0.0, -1.0};
//     GLfloat light_1_dir [] = {-20.0, -20.0, -17.0};
//     GLfloat light_2_dir [] = {-20.0, 20.0, -17.0};
//     GLfloat light_3_dir [] = {20.0, 0.0, -17.0};

//     GLfloat color_white [] = {1.0, 1.0, 1.0, 1.0};
//     GLfloat color_black [] = {0.0, 0.0, 0.0, 1.0};
//     GLfloat color_red [] = {1.0, 0.0, 0.0, 1.0};
//     GLfloat color_green [] = {0.0, 1.0, 0.0, 1.0};
//     GLfloat color_blue [] = {0.0, 0.0, 1.0, 1.0};
//     GLfloat ambientLight [] = {0.1f, 0.1f, 0.1f, 1.0f};
//     GLfloat diffuseLight [] = {0.7f, 0.7f, 0.7f, 1.0f};
//     GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
//     GLfloat globalAmbient [] = {0.3, 0.3, 0.3, 1.0};

//     glLightfv(GL_LIGHT1, GL_POSITION, light_1_position);
//     glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_1_dir);
//     glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
//     glLightfv(GL_LIGHT1, GL_DIFFUSE, color_red);
//     glLightfv(GL_LIGHT1, GL_SPECULAR, color_red);
//     glLightfv(GL_LIGHT2, GL_POSITION, light_2_position);
//     glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_2_dir);
//     glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight);
//     glLightfv(GL_LIGHT2, GL_DIFFUSE, color_green);
//     glLightfv(GL_LIGHT2, GL_SPECULAR, color_green);
//     glLightfv(GL_LIGHT3, GL_POSITION, light_3_position);
//     glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light_3_dir);
//     glLightfv(GL_LIGHT3, GL_AMBIENT, ambientLight);
//     glLightfv(GL_LIGHT3, GL_DIFFUSE, color_blue);
//     glLightfv(GL_LIGHT3, GL_SPECULAR, color_blue);
//     glLightfv(GL_LIGHT0, GL_POSITION, light_0_position);
//     glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
//     glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
//     glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
//     glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_0_dir);
//     glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
//     glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.5);
//     glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 90.0);
//     glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0.5);
//     glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 90.0);
//     glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 0.5);
//     glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
//     glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 30);

//     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
}

void Fog() {
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

void Lights() {
//     if (Settings::fancyLights) {
//         glEnable(GL_LIGHT1);
//         glEnable(GL_LIGHT2);
//         glEnable(GL_LIGHT3);
//         glEnable(GL_LIGHT0);
//     }
//     else {
//         glDisable(GL_LIGHT1);
//         glDisable(GL_LIGHT2);
//         glDisable(GL_LIGHT3);
//         glEnable(GL_LIGHT0);
//     }
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

void drawBackGround() {
//     glEnable(GL_LIGHTING);
//     glBegin(GL_QUAD_STRIP);
//         glColor3d(1.0, 1.0, 1.0);
//         glNormal3d(0.0, 0.0, 1.0);
//         glVertex3i(-FLOOR_SIZE, -FLOOR_SIZE, 0);
//         glNormal3d(0.0, 0.0, 1.0);
//         glVertex3i(-FLOOR_CENTER_SIZE, -FLOOR_CENTER_SIZE, 0);
//         glNormal3d(0.0, 0.0, 1.0);
//         glVertex3i(-FLOOR_SIZE, FLOOR_SIZE, 0);
//         glNormal3d(0.0, 0.0, 1.0);
//         glVertex3i(-FLOOR_CENTER_SIZE, FLOOR_CENTER_SIZE, 0);
//         glNormal3d(0.0, 0.0, 1.0);
//         glVertex3i(FLOOR_SIZE, FLOOR_SIZE, 0);
//         glNormal3d(0.0, 0.0, 1.0);
//         glVertex3i(FLOOR_CENTER_SIZE, FLOOR_CENTER_SIZE, 0);
//         glNormal3d(0.0, 0.0, 1.0);
//         glVertex3i(FLOOR_SIZE, -FLOOR_SIZE, 0);
//         glNormal3d(0.0, 0.0, 1.0);
//         glVertex3i(FLOOR_CENTER_SIZE, -FLOOR_CENTER_SIZE, 0);
//         glNormal3d(0.0, 0.0, 1.0);
//         glVertex3i(-FLOOR_SIZE, -FLOOR_SIZE, 0);
//         glNormal3d(0.0, 0.0, 1.0);
//         glVertex3i(-FLOOR_CENTER_SIZE, -FLOOR_CENTER_SIZE, 0);
//     glEnd();

//     glBegin(GL_QUADS);
//         for (double i = -FLOOR_CENTER_SIZE; i < FLOOR_CENTER_SIZE; i += FLOOR_GRID_SIZE) {
//             for (double j = -FLOOR_CENTER_SIZE; j < FLOOR_CENTER_SIZE; j += FLOOR_GRID_SIZE) {
//                 setVertexColor(i, j);
//                 glVertex3d(i, j, 0);
//                 setVertexColor(i, j+FLOOR_GRID_SIZE);
//                 glVertex3d(i, j+FLOOR_GRID_SIZE, 0);
//                 setVertexColor(i+FLOOR_GRID_SIZE, j+FLOOR_GRID_SIZE);
//                 glVertex3d(i+FLOOR_GRID_SIZE, j+FLOOR_GRID_SIZE, 0);
//                 setVertexColor(i+FLOOR_GRID_SIZE, j);
//                 glVertex3d(i+FLOOR_GRID_SIZE, j, 0);
//             }
//         }
//     glEnd();
//     glDisable(GL_LIGHTING);

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

void display() {
    glClearColor(backGroundColor, backGroundColor, backGroundColor, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


//     Fog();

//     glMatrixMode(GL_MODELVIEW);
//     glLoadIdentity();
//     if (Game::getGameStatus() == Lifting) gluLookAt(zoom * cos(theta / 180.0 * M_PI) * sin(fai / 180.0 * M_PI) + xcenter,
//         zoom * sin(theta / 180.0 * M_PI) * sin(fai / 180.0 * M_PI) + ycenter,
//         zoom * cos(fai / 180.0 * M_PI) + zcenter,
//         xcenter, ycenter, zcenter,
//         0.0,0.0,1.0);
//     else gluLookAt(zoom * cos(theta / 180.0 * M_PI) * sin(fai / 180.0 * M_PI),
//         zoom * sin(theta / 180.0 * M_PI) * sin(fai / 180.0 * M_PI),
//         zoom * cos(fai / 180.0 * M_PI),
//         0.0,0.0,TABLE_HEIGHT + TABLE_THICKNESS / 2,
//         0.0,0.0,1.0);
//     Lights();
//     drawBackGround();
//     drawTable();
//     drawStone();
//     if (Game::getGameStatus() == Pause) {
//         glDisable(GL_DEPTH_TEST);
//         glDisable(GL_LIGHTING);
//         glMatrixMode(GL_PROJECTION);
//         glPushMatrix();
//             glLoadIdentity();
//             gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

//             glMatrixMode(GL_MODELVIEW);
//             glPushMatrix();
//                 glLoadIdentity();
//                 glColor4d(1.0, 0.0, 0.0, 0.8);
//                 glRectd(-0.3, 0.3, 0.3, 0.5);
//                 glColor4d(1.0, 1.0, 0.0, 0.8);
//                 glRectd(-0.3, 0.0, 0.3, 0.2);
//                 glColor4d(0.0, 1.0, 0.0, 0.8);
//                 glRectd(-0.3, -0.3, 0.3, -0.1);
//                 glColor4d(0.0, 0.0, 1.0, 0.8);
//                 glRectd(-0.3, -0.6, 0.3, -0.4);

//                 glColor4d(0.0, 0.0, 0.0, 0.9);

//                 glRasterPos2d(-0.07, 0.38);
//                 for (int k = 0; k < 4; ++k)
//                     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s_Save[k]);

//                 glRasterPos2d(-0.07, 0.08);
//                 for (int k = 0; k < 4; ++k)
//                     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s_Load[k]);

//                 glRasterPos2d(-0.19, -0.22);
//                 for (int k = 0; k < 13; ++k)
//                     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s_Save_and_Quit[k]);

//                 glRasterPos2d(-0.25, -0.52);
//                 for (int k = 0; k < 19; ++k)
//                     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s_Quit_without_saving[k]);

//             glPopMatrix();

//         glMatrixMode(GL_PROJECTION);
//         glPopMatrix();
//         glEnable(GL_LIGHTING);
//         glEnable(GL_DEPTH_TEST);
//     }
}

void reshape(int width, int height) {
//     screenSize = (width < height ? width : height);
//     screenWidth = width;
//     screenHeight = height;
//     glViewport(0, 0, width, height);
//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     glFrustum(-(GLfloat)screenWidth / screenSize, (GLfloat)screenWidth / screenSize,
//         -(GLfloat)screenHeight / screenSize, (GLfloat)screenHeight / screenSize,
//         1.0f, 1000.0f);

//     glClear(GL_COLOR_BUFFER_BIT);
//     glutPostRedisplay();
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

RenderResource getAxisFrames() {
    const vector<Vertex_C> vertices = {
            {{0.0f,    0.0f,    0.0f},    {1.0f, 0.0f, 0.0f}},
            {{0.0f,    0.0f,    1000.0f}, {1.0f, 0.0f, 0.0f}},
            {{0.0f,    0.0f,    0.0f},    {0.0f, 1.0f, 0.0f}},
            {{0.0f,    1000.0f, 0.0f},    {0.0f, 1.0f, 0.0f}},
            {{0.0f,    0.0f,    0.0f},    {0.0f, 0.0f, 1.0f}},
            {{1000.0f, 0.0f,    0.0f},    {0.0f, 0.0f, 1.0f}},
    };

    const vector<GLushort> indices = {0, 1, 2, 3, 4, 5};

    return RenderResource(vertices, indices, GL_LINES); // NOLINT(modernize-return-braced-init-list)
}

struct DirectionalLight {
    bool enabled = true;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    void apply(Shader &shader) const {
        shader.setBool("dirLight.enabled", enabled);
        shader.setVec3("dirLight.position", direction);
        shader.setVec3("dirLight.ambient", ambient);
        shader.setVec3("dirLight.diffuse", diffuse);
        shader.setVec3("dirLight.specular", specular);
    }
};

struct PointLight {
    bool enabled = true;
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;

    void apply(Shader &shader, int index) const {
        using namespace boost;
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

    void apply(Shader &shader) const {
        shader.setBool("spotLight.enabled", enabled);
        shader.setVec3("spotLight.position", position);
        shader.setVec3("spotLight.direction", direction);
        shader.setVec3("spotLight.ambient", ambient);
        shader.setVec3("spotLight.diffuse", diffuse);
        shader.setVec3("spotLight.specular", specular);
        shader.setFloat("spotLight.constant", constant);
        shader.setFloat("spotLight.linear", linear);
        shader.setFloat("spotLight.quadratic", quadratic);
        shader.setFloat("spotLight.cutOff", cutOff);
        shader.setFloat("spotLight.outerCutOff", outerCutOff);
    }
};


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
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
    // initDisplay();+
    // refreshModel(false);done
    // glutTimerFunc(0, &updateRenderStatus, 0);done
    // glutMainLoop();deprecated

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window;
    window = glfwCreateWindow(screenWidth, screenHeight, "Othello", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    int render_status = 0;

    initDisplay();
    refreshModel(false);

    Shader simpleShader("shader/simple.vert", "shader/simple.frag");
    Shader lightShader("shader/simple.vert", "shader/light.frag");
    Shader withColorShader("shader/with_color.vert", "shader/with_color.frag");

    // Render objects
    RenderResource axisFrames = getAxisFrames();
    RenderResource cube = getCube(0.5f);
    RenderResource light = getCube(0.2f);

    glm::mat4 modelMat = glm::rotate(glm::mat4(1.0f), (float) M_PI / 4 * 0, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    DirectionalLight directionalLight = {
            .direction {-0.2f, -1.0f, -0.3f},
            .ambient {0.05f, 0.05f, 0.05f},
            .diffuse {0.4f, 0.4f, 0.4f},
            .specular {0.5f, 0.5f, 0.5f},
    };

    vector<PointLight> pointLights = {
            {
                    .position {0.7f, 0.2f, 2.0f},
                    .ambient {0.05f, 0.05f, 0.05f},
                    .diffuse {0.8f, 0.8f, 0.8f},
                    .specular {1.0f, 1.0f, 1.0f},
                    .constant = 1.0f,
                    .linear = 0.09f,
                    .quadratic = 0.032,
            },
            {
                    .position {2.3f, -3.3f, -4.0f},
                    .ambient {0.05f, 0.05f, 0.05f},
                    .diffuse {0.8f, 0.8f, 0.8f},
                    .specular {1.0f, 1.0f, 1.0f},
                    .constant = 1.0f,
                    .linear = 0.09f,
                    .quadratic = 0.032,
            },
            {
                    .position {-4.0f, 2.0f, -12.0f},
                    .ambient {0.05f, 0.05f, 0.05f},
                    .diffuse {0.8f, 0.8f, 0.8f},
                    .specular {1.0f, 1.0f, 1.0f},
                    .constant = 1.0f,
                    .linear = 0.09f,
                    .quadratic = 0.032,
            },
            {
                    .position {0.0f, 0.0f, -3.0f},
                    .ambient {0.05f, 0.05f, 0.05f},
                    .diffuse {0.8f, 0.8f, 0.8f},
                    .specular {1.0f, 1.0f, 1.0f},
                    .constant = 1.0f,
                    .linear = 0.09f,
                    .quadratic = 0.032,
            },
    };

    SpotLight spotLight = {
            .position {0.0f, 0.0f, 0.0f},
            .direction {0.0f, 0.0f, 0.0f},
            .ambient {0.0f, 0.0f, 0.0f},
            .diffuse {1.0f, 1.0f, 1.0f},
            .specular {1.0f, 1.0f, 1.0f},
            .constant = 1.0f,
            .linear = 0.09f,
            .quadratic = 0.032,
            .cutOff = glm::cos(glm::radians(12.5f)),
            .outerCutOff = glm::cos(glm::radians(15.0f)),
    };

    lightShader.use();
    lightShader.setFloat("material.shininess", 32.0f);
    // directional light
    directionalLight.apply(lightShader);
    // point light
    for (auto const &pointLight : pointLights | boost::adaptors::indexed(0)) {
        pointLight.value().apply(lightShader, pointLight.index());
    }
    // spotLight
    spotLight.apply(lightShader);

    unsigned int diffuseMap = loadTexture(boost::filesystem::path("resources/textures/container.png").c_str());
    unsigned int specularMap = loadTexture(
            boost::filesystem::path("resources/textures/container_specular.png").c_str());

    lightShader.use();
    lightShader.setInt("material.diffuse", 0);
    lightShader.setInt("material.specular", 1);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        glm::mat4 viewMat = camera.GetViewMatrix();
        glm::mat4 projMat = glm::perspective(glm::radians(camera.Zoom), (float) screenWidth / (float) screenHeight,
                                             1.0f,1000.f);

        lightShader.use();
        lightShader.setVec3("viewPos", camera.Position);
        spotLight.position = camera.Position;
        spotLight.direction = camera.Front;

        spotLight.apply(lightShader);

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
        display();

        // Render
        lightShader.use();
        modelMat = glm::rotate(modelMat, (float) M_PI / 4 / FPS, glm::vec3(0.0f, 1.0f, 0.0f));
        lightShader.setMat4("model", modelMat);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        for (auto const& cubePosition : cubePositions | boost::adaptors::indexed(0)) {
            int i = cubePosition.index();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i + currentFrame * 20;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightShader.setMat4("model", model);

            cube.draw();
        }

        simpleShader.use();
        for (auto const &pointLight : pointLights) {
            auto lightModelMat = glm::mat4(1.0f);
            lightModelMat = glm::translate(lightModelMat, pointLight.position);
            lightModelMat = glm::scale(lightModelMat, glm::vec3(0.3f));
            simpleShader.setMat4("model", lightModelMat);
            simpleShader.setVec4("uniformColor", 1.0f, 1.0f, 1.0f, 1.0f);
            light.draw();
        }

        withColorShader.use();
        withColorShader.setMat4("model", glm::identity<glm::mat4>());
        axisFrames.draw();

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
#include "display.h"
#include "element.h"
#include "base.h"
#include "game.h"
#include "model.h"
#include "shader/shader.h"
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <GL/freeglut.h>
// #include <GL/glut.h>
// #include <GL/glu.h>

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

typedef struct Vertex_s {
    float position[3];
} Vertex;

typedef struct VertexWithColor_s {
    float position[3];
    float color[3];
} VertexWithColor;

struct RenderResource {
    GLuint vao{};
    GLuint vbo{};
    GLuint ibo{};
    GLenum mode;
    int drawCount{};

    RenderResource() = delete;

    RenderResource(const vector<Vertex> &vertices, const vector<GLushort> &indices, GLenum mode) : mode{mode} {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
        drawCount = indices.size();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//        glBindVertexArray(0);
    }

    RenderResource(const vector<VertexWithColor> &vertices, const vector<GLushort> &indices, GLenum mode) : mode{mode} {
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

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//        glBindVertexArray(0);
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
    const vector<Vertex> vertices = {
            // Front face
            {{-cubeSize_2, -cubeSize_2, cubeSize_2}},
            {{cubeSize_2,  -cubeSize_2, cubeSize_2}},
            {{cubeSize_2,  cubeSize_2,  cubeSize_2}},
            {{-cubeSize_2, cubeSize_2,  cubeSize_2}},
            // Back face
            {{cubeSize_2,  -cubeSize_2, -cubeSize_2}},
            {{-cubeSize_2, -cubeSize_2, -cubeSize_2}},
            {{-cubeSize_2, cubeSize_2,  -cubeSize_2}},
            {{cubeSize_2,  cubeSize_2,  -cubeSize_2}},
            // Left face
            {{-cubeSize_2, -cubeSize_2, -cubeSize_2}},
            {{-cubeSize_2, -cubeSize_2, cubeSize_2}},
            {{-cubeSize_2, cubeSize_2,  cubeSize_2}},
            {{-cubeSize_2, cubeSize_2,  -cubeSize_2}},
            // Right face
            {{cubeSize_2,  -cubeSize_2, cubeSize_2}},
            {{cubeSize_2,  -cubeSize_2, -cubeSize_2}},
            {{cubeSize_2,  cubeSize_2,  -cubeSize_2}},
            {{cubeSize_2,  cubeSize_2,  cubeSize_2}},
            // Top face
            {{cubeSize_2,  cubeSize_2,  -cubeSize_2}},
            {{-cubeSize_2, cubeSize_2,  -cubeSize_2}},
            {{-cubeSize_2, cubeSize_2,  cubeSize_2}},
            {{cubeSize_2,  cubeSize_2,  cubeSize_2}},
            // Bottom face
            {{-cubeSize_2, -cubeSize_2, -cubeSize_2}},
            {{cubeSize_2,  -cubeSize_2, -cubeSize_2}},
            {{cubeSize_2,  -cubeSize_2, cubeSize_2}},
            {{-cubeSize_2, -cubeSize_2, cubeSize_2}},
    };

    // Generate the index array
    const GLsizei vertsPerSide = 4;
    const GLsizei numSides = 6;
    const GLsizei indicesPerSide = 6;
    const GLsizei numIndices = indicesPerSide * numSides;
    vector<GLushort> indices(numIndices);
    GLuint i = 0;
    for (GLushort j = 0; j < numSides; ++j) {
        GLushort sideBaseIdx = j * vertsPerSide;
        indices[i++] = sideBaseIdx + 0;
        indices[i++] = sideBaseIdx + 1;
        indices[i++] = sideBaseIdx + 2;
        indices[i++] = sideBaseIdx + 2;
        indices[i++] = sideBaseIdx + 3;
        indices[i++] = sideBaseIdx + 0;
    }

    return RenderResource(vertices, indices, GL_TRIANGLES); // NOLINT(modernize-return-braced-init-list)
}

RenderResource getAxisFrames() {
    const vector<VertexWithColor> vertices = {
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

SDL_Window *initWindow() {
    // Set-up window
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        throw std::runtime_error("Cannot initialize SDL.");
    }

    // Setup the exit hook
    atexit(SDL_Quit);

    // Request OpenGL ES 2.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create the window
    SDL_Window *window = SDL_CreateWindow("Othello", 50, 50, screenSize, screenSize,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't create the main window.", nullptr);
        throw std::runtime_error("Couldn't create the main window.");
    }

    return window;
}

SDL_Renderer *initRenderer(SDL_Window *window) {
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_Log("Could not create a renderer: %s", SDL_GetError());
        throw std::runtime_error("Could not create a renderer.");
    }
    return renderer;
}

void setSDLAttribute() {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
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

    SDL_Window *window = initWindow();

    SDL_Renderer *renderer = initRenderer(window);

    setSDLAttribute();

    int render_status = 0;

    initDisplay();
    refreshModel(false);

    Shader simpleShader ("shader/simple.vert", "shader/simple.frag");
    simpleShader.use();

    Shader withColorShader ("shader/with_color.vert", "shader/with_color.frag");
    withColorShader.use();

    // Create the 3D cube
    float cubeSize_2 = 1.0f / 2.0f; // Half the cube's size
    RenderResource axisFrames = getAxisFrames();
    RenderResource cube = getCube(cubeSize_2);

    glm::mat4 modelMat = glm::rotate(glm::mat4(1.0f), (float) M_PI / 4 * 0, glm::vec3(0.0f, 1.0f, 0.0f));
    // Set up the camera
    // NOTE: OpenGL cameras look down the negative z-axis
    glm::vec3 cameraPos = glm::vec3(3.0f, 1.5f, 3.0f);
    glm::vec3 cameraFocus = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 viewMat = glm::lookAt(cameraPos, cameraFocus, cameraUp);
    glm::mat4 projMat = glm::perspective(glm::radians(60.0f), (float) screenWidth / (float) screenHeight, 1.0f,
                                         1000.f);

    simpleShader.use();
    simpleShader.setMat4("projection", projMat);
    simpleShader.setMat4("view", viewMat);

    withColorShader.use();
    withColorShader.setMat4("projection", projMat);
    withColorShader.setMat4("view", viewMat);

    // Main loop
    bool quit = false;
    while (!(quit || *gameEnds)) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                // User wants to quit
                quit = true;
            }
        }

        // Update scene
        float timeValue = SDL_GetTicks() / 1000.0f;
        float greenValue = (sin(timeValue) / 10.0f) + 0.5f;

        render_status = updateRenderStatus(render_status);
        display();

        // Render
        simpleShader.use();
        modelMat = glm::rotate(modelMat, (float) M_PI / 4 / FPS, glm::vec3(0.0f, 1.0f, 0.0f));
        simpleShader.setMat4("model", modelMat);
        simpleShader.setVec4("uniformColor", glm::vec4(0.0f, greenValue, 0.0f, 1.0f));
        cube.draw();

        withColorShader.use();
        withColorShader.setMat4("model", glm::identity<glm::mat4>());
        axisFrames.draw();

        SDL_GL_SwapWindow(window);
        SDL_Delay(1000 / FPS);
    }

    // Clean up
    cube.free();
    axisFrames.free();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
#include "model.h"
#include "../core/game.h"
#include "../render/camera.h"

using namespace std;

extern bool isFocus;
extern float fogColorFocus[];
extern float fogDensity;
extern Camera camera;

vector<Stone> stones;
double lightDirAngle = 0;
double xcenter = 0.0, ycenter = 0.0, zcenter = 0.0;
clock_t starttime;
int liftingTableTime = 0;
double emulateTimes = 5.0;

inline double exponent_coeffiency(double z) {
    if (abs(z - STONE_RADIUS) < 0.1) return (1.0 - 100 * (z - STONE_RADIUS) * (z - STONE_RADIUS));
    return 0.0;
}

Stone::Stone(Piece p) {
    color = p.getStatus();
    x = (p.getY() - BOARD_SIZE / 2.0 - 0.5) * STONE_INTERVAL;
    y = (BOARD_SIZE / 2.0 + 0.5 - p.getX()) * STONE_INTERVAL;
    z = TABLE_HEIGHT + TABLE_THICKNESS / 2;
    angle = 0.0;
    axisx = 0.0;
    axisy = 0.0;
    axisz = 1.0;
}

void Stone::fly() {
    z = STONE_RADIUS + TABLE_HEIGHT + TABLE_THICKNESS / 2;
    double rm = RAND_MAX;
    vx = 60.0 * rand() / rm - 30.0;
    vy = 60.0 * rand() / rm - 30.0;
    vz = 50.0 + 50.0 * rand() / rm;
    vangle = 1000.0 * rand() / rm;
    axisx = rand() / rm;
    axisy = rand() / rm;
    axisz = rand() / rm;
}

void Stone::move(double dt) {
    x += vx * dt;
    y += vy * dt;
    z += vz * dt;
    vz -= GRAVITY * dt;
    angle = vangle * dt + angle;
    angle = (int((angle + 90.0) / 180.0) * 180.0) * exponent_coeffiency(z) + angle * (1 - exponent_coeffiency(z));
    if ((abs(x) < TABLE_SIZE / 2) && (abs(y) < TABLE_SIZE / 2)) {
        if (z < STONE_RADIUS + TABLE_HEIGHT + TABLE_THICKNESS / 2) {
            z = 2 * (STONE_RADIUS + TABLE_HEIGHT + TABLE_THICKNESS / 2) - z;
            vz = -vz * RESTITUTION;
            vangle = -vangle * RESTITUTION;
            axisz /= 2.0;
        }
    } else {
        double h = STONE_RADIUS * abs(int((angle + 90.0) / 180.0) - angle / 180.0) + STONE_HEIGHT / 2;
        if (z < h) {
            z = 2 * h - z;
            vz = -vz * RESTITUTION;
            vangle = -vangle * RESTITUTION;
            axisz /= 2.0;
        }
    }

}

void Stone::print() {
    printf("coordinate : (%lf, %lf, %lf)\nvelocity : (%lf, %lf, %lf)\naxis : (%lf, %lf, %lf)\ncolor : %s\n",
           x, y, z, vx, vy, vz, axisx, axisy, axisz, (color == Black) ? "Black" : "White");
}

void refreshModel(bool lifting) {
    Board b = Game::getBoard();
    stones.clear();
    for (int i = 1; i <= BOARD_SIZE; ++i) {
        for (int j = 1; j <= BOARD_SIZE; ++j) {
            if ((b.getPiece(i, j).getStatus() == Black) || (b.getPiece(i, j).getStatus() == White))
                stones.push_back(Stone(b.getPiece(i, j)));
            if (!lifting) {
                if ((b.getPiece(i, j).getStatus() == Valid)
                    || (b.getPiece(i, j).getStatus() == BlackValid)
                    || (b.getPiece(i, j).getStatus() == WhiteValid))
                    stones.push_back(Stone(b.getPiece(i, j)));
            }
        }
    }
}

int updateRenderStatus(int status) {
    switch (status) {
        case 0: // Idle
            camera.resetYawPitch();
            if (Game::getGameStatus() != Playing) {
                status = 0;
            } else {
                status = 1;
            }
            break;
        case 1: // Playing
            // TODO
//            SDL_ShowCursor(SDL_DISABLE);
            refreshModel(false);
            switch (Game::getGameStatus()) {
                case Playing:
                    status = 1;
                    break;
                case Pause:
                    status = 2;
                    break;
                case Lifting:
                    status = 3;
                    break;
                default:
                    status = 0;
            }
            break;
        case 2: // Pausing...
            // TODO
//            SDL_ShowCursor(SDL_ENABLE);
            switch (Game::getGameStatus()) {
                case Playing:
                    status = 1;
                    break;
                case Pause:
                    status = 2;
                    break;
                default:
                    status = 0;
            }
            break;
        case 3: //Preparing to lift
            // TODO
//            SDL_ShowCursor(SDL_ENABLE);
            status = 4;
            refreshModel(true);
            for (auto &stone : stones) {
                stone.fly();
            }
            break;
        case 4: // Lifting
        default:
            if (Game::getGameStatus() == Lifting) {
                status = 4;
            } else {
                status = 0;
            }
            starttime = clock();

            if (isFocus) {
                fogDensity = fogDensity * 0.9 + 0.05 * 0.1;
                emulateTimes = emulateTimes * 0.9 + 5 * 0.1;
            } else {
                fogDensity = fogDensity * 0.9;
                emulateTimes = emulateTimes * 0.9 + 30 * 0.1;
            }

            for (int i = 0; i < emulateTimes; ++i) {
                for (auto &stone : stones) {
                    stone.move(TIME_UNIT);
                }
            }

            double x = 0.0, y = 0.0, z = 0.0;
            for (auto &stone : stones) {
                x += stone.getX();
                y += stone.getY();
                z += stone.getZ();
            }
            xcenter = x / stones.size() * lambda + xcenter * (1 - lambda);
            ycenter = y / stones.size() * lambda + ycenter * (1 - lambda);
            zcenter = z / stones.size() * lambda + zcenter * (1 - lambda);

            //printf("timefunc update complete using %.1lf μs\n", (0.0 + clock() - starttime) / CLOCKS_PER_SEC * 1e6);

            liftingTableTime += emulateTimes;
            if (liftingTableTime >= 250 * FPS) fogDensity = fogDensity * 0.995 + 10 * 0.005;
            if (liftingTableTime >= 300 * FPS) {
                Game::endGame();
                liftingTableTime = 0;
                fogDensity = 0.05f;
            }
            break;
    }
    return status;
}

#define _USE_MATH_DEFINES
#ifndef GAME_H
#define GAME_H
#include <Graphics.h>
#include <Raycaster.h>
#include <tchar.h>
#include <string>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <math.h>
#include <Map.h>
#include <vector>

LRESULT CALLBACK gameProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

class Game
{
    public:

        enum class GAME_STATE {RUNNING, STOPPED, PAUSED};

        Game(HWND,int,int, int, int);
        virtual ~Game();
        void update(bool);
        void drawMap();
        int player_x = 78;
        int player_y = 68;
        float p_angle = PI/2;
        void calcDist(float,bool);
        bool displayMap = true;
        void handleInput(WPARAM, LPARAM);
        Game::GAME_STATE getState();
        Game::GAME_STATE setState(GAME_STATE new_state);

    private:

        Graphics* g2d;
        HWND windowHandle;
        Game::GAME_STATE game_state = GAME_STATE::STOPPED;
        Map* myMap;
        Raycaster* r;

        //Canvas width and height
        int width;
        int height;

        //GDI structs
        HDC m_hdc;
        PAINTSTRUCT ps;
        HDC Memhdc;

        //Keeping track of world co-ordinates
        int mapX;
        int mapY;
        float map_scale = 8;

        //dimension of a map block
        int block_square_dimension = 64;
        float map_size =  block_square_dimension/map_scale;

        //Helper utility
        int num = 0;
        int idxOngle = 0;
        float colWidth = 1;

        void createAngles();
        void calculateConstraints();
};

#endif // GAME_H

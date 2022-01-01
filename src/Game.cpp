#include "Game.h"

LRESULT CALLBACK gameProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{

    switch (msg)                  /* handle the messages */
    {

    case WM_ERASEBKGND:
        return TRUE;
        break;

    default:                   /* for messages that we don't deal with */
        return DefWindowProc (hwnd, msg, w, l);
        break;
    }

}


Game::Game(HWND handle, int width, int height, int x, int y) : width(width), height(height)
{
    TCHAR szClassName[ ] = _T("BIGCHUNGUS");

    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = NULL;
    wincl.lpszClassName = szClassName;
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.lpfnWndProc = gameProc;
    wincl.hbrBackground = NULL;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
    { }

    windowHandle = CreateWindowEx(0, szClassName, _T("Big Chunugs 3: Revenge of the son"),WS_EX_LAYERED | WS_CHILD | WS_VISIBLE, x,y, width, height, handle, NULL, NULL, NULL);

    myMap = new Map(std::string("res/mad.txt"));

    m_hdc = GetDC(windowHandle);

    Memhdc = m_hdc;

    g2d = new Graphics(windowHandle);

    mapX = width - myMap->getCols()* (block_square_dimension/map_scale);

    mapY = 0;

    r = new Raycaster(myMap, block_square_dimension, width, height);

}


Game::~Game()
{

    delete g2d;

    DestroyWindow(windowHandle);

    delete myMap;

    delete r;

}

int curr = 0;

D2D1_COLOR_F getRandom()
{

    D2D1_COLOR_F clr;
    if (curr == 0) clr = D2D1::ColorF(255,0,0);
    if (curr == 1) clr = D2D1::ColorF(0,255,0);
    if (curr == 2) clr = D2D1::ColorF(255,0,255);
    if (curr == 3) clr = D2D1::ColorF(0,064,255);
    if (curr == 4) clr = D2D1::ColorF(128,128,0);

    curr++;

    if (curr == 5)  curr = 0;
    return clr;

}

#define SQRT_MAGIC_F 0x5f3759df
float  sqrt3(const float x)
{
    const float xhalf = 0.5f*x;

    union // get bits for floating value
    {
        float x;
        int i;
    } u;
    u.x = x;
    u.i = SQRT_MAGIC_F - (u.i >> 1);  // gives initial guess y0
    return x*u.x*(1.5f - xhalf*u.x*u.x);// Newton step, repeating increases accuracy
}

void Game::update(bool debug)
{
    if (game_state == GAME_STATE::PAUSED || game_state == GAME_STATE::STOPPED) return;

    float distScreen = r->getDistanceFromScreen();

    g2d->beginDraw();

    g2d->clear();

    auto vectors = r->getIntersections(player_x, player_y);

    g2d->fill(255,255,255);

    int i =0;
    for (auto intersection_vector : vectors)
    {

        for (auto intersection: intersection_vector)
        {

            float dist = sqrt3( (player_x - intersection.first)*(player_x - intersection.first) + (player_y - intersection.second)*(player_y - intersection.second) );
            float sliceH = block_square_dimension / dist * distScreen;

            float proj = distScreen /(dist + 1);
            g2d->fill(0, proj, 0);
            g2d->drawLine(width - i, height/2 - (sliceH/2), width - i, height/2 + (sliceH/2));

        }

        i++;

    }

    drawMap();

    g2d->endDraw();

}

void Game::drawMap()
{

    for (int i = 0; i < myMap->getRows(); i++)
    {

        for (int j = 0; j < myMap->getCols() ; j++)
        {

            if (myMap->getData(j,i) == 'W')
            {

                g2d->fill(0,0,0);

            }
            else
            {
                g2d->fill(0,0,255);
            }

            g2d->drawFillRect(mapX + map_size*(j), mapY + map_size*i, map_size, map_size);
        }

    }

    g2d->fill(255,255,255);

    for (int i = 0; i <= myMap->getRows() - 1; i++)
    {

        g2d->drawLine(mapX, mapY + map_size*(i+1), mapX + map_size * myMap->getCols(), mapY + map_size*(i+1));

    }

    for (int i = 0; i <= myMap->getCols(); i++)
    {

        g2d->drawLine(mapX + map_size * i, mapY, mapX + map_size*i, mapY + map_size * myMap->getCols());

    }

    g2d->fill(255,0,0);
    g2d->drawFillEllipse(mapX + player_x/map_scale, mapY + player_y/map_scale, 2);

    g2d->drawLine(mapX + player_x/map_scale, mapY + player_y/map_scale, mapX + player_x/map_scale + cos(r->getPlayerAngle())*5, mapY + player_y/map_scale + sin(r->getPlayerAngle())*5 );

}

Game::GAME_STATE Game::setState(GAME_STATE new_state)
{
    this->game_state = new_state;
    return this->game_state;
}

Game::GAME_STATE Game::getState()
{
    return this->game_state;
}

float clip(float n, float lower, float upper)
{
    return std::max(lower, std::min(n, upper));
}

void Game::handleInput(WPARAM input, LPARAM args)
{

    int displacement = 5;
    auto p_a = r->getPlayerAngle();
    float cx = player_x;
    float cy = player_y;

    auto state_up = GetAsyncKeyState (VK_UP);
    auto state_down = GetAsyncKeyState (VK_DOWN);
    auto state_left = GetAsyncKeyState (VK_LEFT);
    auto state_right = GetAsyncKeyState (VK_RIGHT);

    if ( (state_up & 0x01) && !(state_left & 0x01) && !(state_right & 0x01))
    {
        player_x += displacement*cos(p_a);
        player_y += displacement*sin(p_a);

    }
    else if ((state_down & 0x01) && !(state_left & 0x01) && !(state_right & 0x01))
    {
        player_x -= displacement*cos(p_a);
        player_y -= displacement*sin(p_a);

    }
    else if ((state_up & 0x01) && (state_left & 0x01))
    {
        r->increasePlayerAngle();
        player_x += displacement*cos(p_a);
        player_y += displacement*sin(p_a);
    }
    else if ((state_up & 0x01) && (state_right & 0x01))
    {
        r->decreasePlayerAngle();
        player_x -= displacement*cos(p_a);
        player_y -= displacement*sin(p_a);
    }
    else if ( !(state_up & 0x01) && (state_left & 0x01) && !(state_right & 0x01))
    {
       r->increasePlayerAngle();
    }
    else if ( !(state_up & 0x01) && !(state_left & 0x01) && (state_right & 0x01))
    {
        r->decreasePlayerAngle();
    }

    int map_x = player_x / block_square_dimension;
    int map_y = player_y / block_square_dimension;

    if (myMap->getData(map_x, map_y) == 'W')
    {
        player_x = cx;
        player_y = cy;
    }

}




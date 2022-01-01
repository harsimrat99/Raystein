#ifndef WORLD_H
#define WORLD_H
#include <Map.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <float.h>

#define PI 3.1415926535897932384626433832795
#define PI_2 1.5707963267948966192313216916398
#define THREE_PI_2 4.712389
#define sz 200
#define HYP sz/2

class Raycaster
{
public:

    Raycaster(Map* m_map, int world_dimension, float w, float h);

    float getPlayerAngle() {
        return this->ANGLES[angle_index];
    }

    float getPlayerAngleIndex() {return this->angle_index;}
    float increasePlayerAngle();
    float decreasePlayerAngle();
    float getDistanceFromScreen() { return this->distanceFromScreen ;}

    std::vector < std::vector < std::pair <float, float> > >& getIntersections(float p_x, float p_y);

    std::vector < std::vector <float> > RAW_ANGLES;
    std::vector < std::vector <float> > COS_OF_RAW_ANGLES;
    std::vector < std::vector <float> > SIN_OF_RAW_ANGLES;
    std::vector < std::vector <float> > TAN_OF_RAW_ANGLES;

protected:

private:

    std::vector <std::pair <float, float>> calculateHorizontalIntersections(float a, float p_x,  float p_y, float index);
    std::vector <std::pair <float, float>> calculateVerticalIntersections  (float a, float p_x,  float p_y, float index);

    int number_angles = 25;
    std::vector <float> ANGLES;
    std::vector < std::vector < std::pair <float, float> > > calculated_positions;

    int m_resolution = 5;
    // field of view is 60 degrees, which when divided by two is 30 degrees. 30 on left side and 30 on right side.
    const float depth = 30 * (M_PI/180);
    // The width of the view plan divided by the
    float divAngle = (2.0f * depth) / m_resolution;
    //Distance from the projection screen to the player
    float distanceFromScreen = (this->width/2) * tan(depth);

    void calculateConstraints();
    void createAngles();


    Map* m_map = nullptr;
    bool is_outside(float, float);

    int size;
    int angle_index;
    float width, height;

    bool ccw(float Ax, float Ay, float Bx, float By, float Cx, float Cy);
    bool intersect(float Ax,float  Ay,float  Bx,float  By,float  Cx ,float Cy, float Dx,float  Dy);
    std::pair<float, float> lineLineIntersection(std::pair<float, float> A, std::pair<float, float> B, std::pair<float, float> C, std::pair<float, float> D);

};

#endif // WORLD_H

#include "Raycaster.h"

Raycaster::Raycaster(Map* map, int world_dimension, float width, float height ) : width(width), height(height)
{
    this->m_map = map;
    this->size = world_dimension;

    calculateConstraints();
    createAngles();

}

float Raycaster::increasePlayerAngle()
{

    angle_index += 1;

    if ( angle_index == (int) ANGLES.size())
    {
        angle_index = 0;
    }

    return ANGLES[angle_index];

}

float Raycaster::decreasePlayerAngle()
{

    angle_index -= 1;

    if ( angle_index < 0 )
    {
        angle_index = (int) ANGLES.size() - 1;
    }

    return ANGLES[angle_index];

}

void Raycaster::createAngles()
{
    float start_angle = (2*PI)  / number_angles;

    for (int i = 0 ; i < number_angles; i++)
    {

        ANGLES.push_back( start_angle * ( i + 1) );

    }

    for (unsigned int i = 0 ; i < ANGLES.size(); i++)
    {

        float player_angle = ANGLES[i];
        std::vector <float> RAW;
        std::vector <float> COS;
        std::vector <float> SIN;
        std::vector <float> TAN;

        for (float current_angle = player_angle - depth ; current_angle < player_angle + depth; current_angle += divAngle)
        {
            RAW.push_back(current_angle);
            COS.push_back(cos(current_angle));
            SIN.push_back(sin(current_angle));
            TAN.push_back(tan(current_angle));

        }

        RAW_ANGLES.push_back(RAW);
        COS_OF_RAW_ANGLES.push_back(COS);
        SIN_OF_RAW_ANGLES.push_back(SIN);
        TAN_OF_RAW_ANGLES.push_back(TAN);

    }

    for ( int i = 0; i < m_resolution; i++)
    {
        std::vector < std::pair <float, float> > a;
        calculated_positions.push_back(a);
    }


    std::cout << RAW_ANGLES[0].size() << ": RA \n";

    angle_index = 0;

}

void Raycaster::calculateConstraints()
{

    m_resolution = width;
    // The width of the view plan divided by the
    divAngle = (2.0f * depth) / m_resolution;
    //Distance from the projection screen to the player
    distanceFromScreen = (this->width/2) / tan(depth);
    std::cout << width << " " << height << " " << (float) this->depth << " " << (float) this->divAngle << " " << (float) this->distanceFromScreen << std::endl;

}

std::vector < std::vector < std::pair <float, float> > >& Raycaster::getIntersections(float player_x, float player_y)
{

    for (unsigned int i = 0 ; i < RAW_ANGLES[angle_index].size(); i++)
    {

        auto angle_c = RAW_ANGLES[angle_index][i];

        auto hcords = calculateHorizontalIntersections( angle_c, player_x, player_y, i );
        auto vcords = calculateVerticalIntersections( angle_c, player_x, player_y, i );

        int last_h = hcords.size() - 1;
        int last_v = vcords.size() - 1;

        float mag_h = abs ( player_x - hcords[ last_h ].first ) + abs ( player_y - hcords[ last_h ].second );
        float mag_v = abs ( player_x - vcords[ last_v ].first ) + abs ( player_y - vcords[ last_v ].second );

         std::vector <std::pair<float,float>> v;

        if ( (mag_v) > (mag_h) )
        {
            v.push_back(hcords[last_h]);
            calculated_positions[i] = v;
        }
        else
        {
            v.push_back(vcords[last_v]);
            calculated_positions[i] = v;
        }

    }

    return this->calculated_positions;

}

std::vector <std::pair <float, float>> Raycaster::calculateHorizontalIntersections(float angle,  float p_x,  float p_y, float id)
{

    float phi = PI/2.0f - angle;

    std::vector < std::pair < float, float > > cords;

    if (angle == 0.0f)
    {

        float x_naught =  ( ( (int) p_x / size) + 1 ) * size;
        float y_naught = p_y;
        float x_ = x_naught, y_ = y_naught;

        cords.push_back ( std::make_pair ( x_, y_ ) );

        for (int i = 0 ; i < 5; i++)
        {

            x_ = x_ + size;
            cords.push_back ( std::make_pair ( x_, y_ ) );

        }

    }

    if (angle > 2*PI)
    {

        angle = angle - 2*PI;

    }

    if (angle < PI)
    {

        float y_naught = ( ( (int) p_y / size ) + 1 ) * size;
        float x_naught = ( y_naught - p_y ) * tan (phi) + p_x;

        float x_ = x_naught;
        float y_ = y_naught;

        cords.push_back ( std::make_pair ( x_, y_ ) );

        int map_x = x_/size;
        int map_y = y_/size;

        char a = this->m_map->getData(map_x,map_y);

        if (a == 'W')
        {
            return cords;
        }

        for (int i = 0 ; i < 10; i++)
        {

            x_ = size*tan(phi) + x_;
            y_ = y_ + size;

            cords.push_back ( std::make_pair ( x_, y_ ) );

            map_x = x_/size;
            map_y = y_/size;

            a = this->m_map->getData(map_x,map_y);

            if (a == 'W')
            {
                break;
            }
            else if (a == 'U')
            {
                //cords.pop_back();
                break;
            }

        }
    }

    else
    {

        phi = (float) THREE_PI_2 - angle;

        int quotient = ( ( p_y / size ) );
        float y_naught = quotient * size - 1;
        float x_naught = - ( p_y  - y_naught  ) * ( 1 / TAN_OF_RAW_ANGLES [angle_index][id] ) + p_x;

        float x_ = x_naught;
        float y_ = y_naught;

        cords.push_back ( std::make_pair ( x_, y_ ) );

        int map_x = x_/size;
        int map_y = y_/size ;

        char a = this->m_map->getData(map_x,map_y);

        if (a == 'W')
        {
            return cords;
        }

        for (int i = 0 ; i < 10; i++)
        {

            x_ = -size* (( 1 / TAN_OF_RAW_ANGLES [angle_index][id] ) ) + x_;
            y_ = y_ - size;

            cords.push_back ( std::make_pair ( x_, y_ ) );

            map_x = x_/size;
            map_y = y_/size ;

            a = this->m_map->getData(map_x,map_y);

            if (a == 'W')
            {
                break;
            }
            else if (a == 'U')
            {
                //cords.pop_back();
                break;
            }

        }

    }

    if (cords.size() == 0)
    {

        cords.push_back({0,0});

    }

    return cords;

}

std::vector <std::pair <float, float>> Raycaster::calculateVerticalIntersections(float angle,  float p_x,  float p_y, float id)
{

    std::vector < std::pair < float, float > > cords;

    if ( angle < (float) PI_2 )
    {

        int quotient = p_x / size;
        float x_naught = ( quotient ) * size + size;
        float y_naught = (( x_naught - p_x ) * TAN_OF_RAW_ANGLES [angle_index][id]) + p_y;

        float x_ = x_naught;
        float y_ = y_naught;

        int map_x = x_/size;
        int map_y = y_/size;

        char a = this->m_map->getData(map_x,map_y);
        cords.push_back(std::make_pair(x_,y_));

        if (a == 'W')
        {
            return cords;
        }


        for (int i = 0 ; i < 10; i++)
        {

            //TAN_OF_RAW_ANGLES [angle_index][i]
            y_ = (size * TAN_OF_RAW_ANGLES [angle_index][id]) + y_;
            x_ = x_ + size;

            cords.push_back(std::make_pair(x_,y_));

            map_x = x_/size;
            map_y = y_/size;

            a = this->m_map->getData(map_x,map_y);

            if (a == 'W')
            {
                break;
            }
            else if (a == 'U')
            {
                //cords.pop_back();
                break;
            }

        }

    }

    else if (angle > THREE_PI_2)
    {

        //float phi = 2*PI - angle;

        int quotient = p_x / size;
        float x_naught = ( quotient * size) + size;
        float y_naught = - ( ( - p_x + x_naught ) * (-TAN_OF_RAW_ANGLES [angle_index][id]) ) + p_y;

        float x_ = x_naught;
        float y_ = y_naught;

        int map_x = x_/size;
        int map_y = y_/size;

        char a = this->m_map->getData(map_x,map_y);
        cords.push_back(std::make_pair(x_,y_));

        if (a == 'W')
        {
            return cords;
        }

        for (int i = 0 ; i < 10; i++)
        {

            y_ = -size*(-TAN_OF_RAW_ANGLES [angle_index][id]) + y_;
            x_ = x_ + size;

            map_x = x_/size;
            map_y = y_/size;

            a = this->m_map->getData(map_x,map_y);
            cords.push_back(std::make_pair(x_,y_));

            if (a == 'W')
            {
                break;
            }
            else if (a == 'U')
            {
                //cords.pop_back();
                break;
            }
        }


    }

    else
    {

        //float phi = PI - angle;

        int quotient = p_x / size;
        float x_naught = ( quotient * size) ;
        float y_naught = ( (  p_x - x_naught ) * (-TAN_OF_RAW_ANGLES [angle_index][id]) ) + p_y;

        float x_ = x_naught;
        float y_ = y_naught;

        int map_x = x_/size - 1;
        int map_y = y_/size;

        char a = this->m_map->getData(map_x,map_y);
        cords.push_back(std::make_pair(x_,y_));

        if (a == 'W')
        {
            return cords;
        }

        for (int i = 0 ; i < 10; i++)
        {

            y_ = size*(-TAN_OF_RAW_ANGLES [angle_index][id]) + y_;
            x_ = x_ - size;

            map_x = x_/size - 1;
            map_y = y_/size;

            a = this->m_map->getData(map_x,map_y);
            cords.push_back(std::make_pair(x_,y_));

            if (a == 'W')
            {
                break;
            }
            else if (a == 'U')
            {
                //cords.pop_back();
                break;
            }
        }

    }

    if (cords.size() == 0)
    {

        cords.push_back({0,0});

    }

    return cords;

}

bool Raycaster::is_outside(float x, float y)
{

    if (x < 0 || x > size*this->m_map->getCols() || y < 0 || y >size*this->m_map->getRows()) return true;
    else return false;

}

bool Raycaster::ccw(float Ax, float Ay, float Bx, float By, float Cx, float Cy)
{
    return (Cy-Ay) * (Bx-Ax) > (By-Ay) * (Cx-Ax);
}

bool Raycaster::intersect(float Ax,float  Ay,float  Bx,float  By,float  Cx,float Cy, float Dx,float  Dy)
{
    return ccw(Ax,Ay, Cx, Cy, Dx,Dy) != ccw(Bx, By, Cx, Cy, Dx, Dy) and ccw(Ax, Ay, Bx, By, Cx, Cy) != ccw(Ax, Ay, Bx, By, Dx, Dy);
}

std::pair<float, float> Raycaster::lineLineIntersection(std::pair<float, float> A, std::pair<float, float> B, std::pair<float, float> C, std::pair<float, float> D)
{

    // Line AB represented as a1x + b1y = c1
    double a1 = B.second - A.second;
    double b1 = A.first - B.first;
    double c1 = a1*(A.first) + b1*(A.second);

    // Line CD represented as a2x + b2y = c2
    double a2 = D.second - C.second;
    double b2 = C.first - D.first;
    double c2 = a2*(C.first)+ b2*(C.second);

    double determinant = a1*b2 - a2*b1;

    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        return std::make_pair(FLT_MAX, FLT_MAX);
    }
    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
        return std::make_pair(x, y);
    }
}

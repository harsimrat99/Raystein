#ifndef MAP_H
#define MAP_H
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <vector>
#include <algorithm>    // std::reverse

#define MAX_LENGTH 10

class Map
{
    public:

        Map(std::string map_file);
        int Initialise();
        int getRows();
        int getCols();
        int getGridLength();
        int getGridWidth();
        inline char getData(int x, int y) {

            if (x < Rows && x >= 0 && y < Cols && y >= 0)
                return Grid[y][x];
            else return 'U';

        }


    private:

        int Rows;
        int Cols;
        std::vector < std::vector <char> > Grid;
        std::string file_location;
};

#endif // MAP_H

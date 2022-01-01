#include "Map.h"

Map::Map(std::string map_file)
{
    file_location = map_file;
    Initialise();
}

int Map::Initialise()
{
    std::fstream file(this->file_location.c_str());
    std::string str;
    std::vector <std::string> strings;

    while (std::getline(file, str))
    {
        // Line contains string of length > 0 then save it in vector
        if(str.size() > 0)
            strings.push_back(str);
    }

    file.close();

    this->Rows = strings.size();
    this->Cols = strings[0].length();

    this->Rows = strings.size();
    this->Cols = strings[0].length();

    Grid.reserve(Rows);

    for (int i = 0 ; i < Rows; i++)
    {
        std::vector <char> cols(Cols, 0);
        Grid.push_back(cols);
    }

    std::reverse(strings.begin(),strings.end());

    for (int i = 0 ; i  < Rows; i++)
    {

        for (int j = 0; j < Cols; j++ )
        {

            Grid[i][j] = (char) strings[i][j];
            //std::cout << this->getData(j,j);

        }

        //std::cout << std::endl;

    }

    return 0;
}

int Map::getRows()
{
    return this->Rows;
}

int Map::getCols()
{
    return this->Cols;
}



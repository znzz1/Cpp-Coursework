#ifndef MYCOLLISIONDETECTOR_H
#define MYCOLLISIONDETECTOR_H

#include "header.h"
#include <vector>

using namespace std;

class MyCollisionDetector
{
public:
    MyCollisionDetector() = delete;

    static int calcCollisionDistance(int mainX, int mainY, vector<vector<int>> pixelMapMain, int minorX, int minorY, vector<vector<int>> pixelMapMinor, int dir)
    {
        int offset = 0;

        // Left = 1, Right = 2, Up = 3, Down = 4;
        if (dir == 1)
        {
            while (checkCollision(mainX + offset, mainY, pixelMapMain, minorX, minorY, pixelMapMinor))
                offset++;
        }
        else if (dir == 2)
        {
            while (checkCollision(mainX - offset, mainY, pixelMapMain, minorX, minorY, pixelMapMinor))
                offset++;
        }
        else if (dir == 3)
        {
            while (checkCollision(mainX, mainY + offset, pixelMapMain, minorX, minorY, pixelMapMinor))
                offset++;
        }
        else if (dir == 4)
        {
            while (checkCollision(mainX, mainY - offset, pixelMapMain, minorX, minorY, pixelMapMinor))
                offset++;
        }

        return offset;
    }

    static bool checkCollision(int mainX, int mainY, vector<vector<int>> pixelMapMain, int minorX, int minorY, vector<vector<int>> pixelMapMinor)
    {
        if (mainX + pixelMapMain[0].size() < minorX)
            return false;
        if (minorX + pixelMapMinor[0].size() < mainX)
            return false;
        if (mainY + pixelMapMain.size() < minorY)
            return false;
        if (minorY + pixelMapMinor.size() < mainY)
            return false;

        // Build the map
        int minX = (mainX < minorX) ? mainX : minorX;
        int minY = (mainY < minorY) ? mainY : minorY;
        int maxX = (mainX + pixelMapMain[0].size() > minorX + pixelMapMinor[0].size() ? mainX + pixelMapMain[0].size() : minorX + pixelMapMinor[0].size()) - minX;
        int maxY = (mainY + pixelMapMain.size() > minorY + pixelMapMinor.size() ? mainY + pixelMapMain.size() : minorY + pixelMapMinor.size()) - minY;
        vector<vector<int>> map(maxY, vector<int>(maxX, 0));

        // Load one object
        int mainBaseX = mainX - minX;
        int mainBaseY = mainY - minY;
        for (int i = 0; i < pixelMapMain.size(); i++)
        {
            for (int j = 0; j < pixelMapMain[0].size(); j++)
            {
                map[i+mainBaseY][j+mainBaseX] = pixelMapMain[i][j];
            }
        }

        // Use another object to detect collision
        int minorBaseX = minorX - minX;
        int minorBaseY = minorY - minY;
        for (int i = 0; i < pixelMapMinor.size(); i++)
            for (int j = 0; j < pixelMapMinor[0].size(); j++)
            {
                if (map[i+minorBaseY][j+minorBaseX] == 1 && pixelMapMinor[i][j] == 1)
                    return true;
            }

        return false;
    }
};

#endif
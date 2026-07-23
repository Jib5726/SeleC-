#ifndef STROKE_H
#define STROKE_H
#include<SFML/Graphics.hpp>
#include<vector>
using namespace std;
using namespace sf;


struct stroke{
    vector<Vector2f> points;
    float size;
    Color color;
};

struct colorpallete{            // preset colors
    vector<Color> colors;
    int select=0;               // current color
    Color getcolor(){
        return colors[select];
    }
};

#endif
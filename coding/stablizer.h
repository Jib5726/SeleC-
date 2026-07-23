#ifndef STABILIZER_H
#define STABILIZER_H

#include<SFML/Graphics.hpp>
#include<deque>
using namespace sf;
using namespace std;

struct stabilizer{
    deque<Vector2f> his;
    int stabl=5; // default stabilizer val;

    Vector2f stable(Vector2f newPoint){
        his.push_back(newPoint);
        if(his.size()>stabl){  // check to make sure we dont overflow ts
            his.pop_front();
        }


        Vector2f sum(0.f,0.f);
        for(const auto& p:his){         // movin average
            sum+=p;
        }
        return sum/float(his.size());
    }

    void clr(){
        his.clear();    // clears shii
    }

};

#endif
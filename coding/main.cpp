#include<SFML/Graphics.hpp>
#include<iostream>
#include<stdlib.h>
#include"stroke.h"
#include"stablizer.h"
using namespace sf;
using namespace std;

void redraw(RenderTexture& canvas, const vector<stroke>& strokes){ // ts redarws evrythn again
    canvas.clear(Color::White);
    for (const auto& s : strokes) {
        for (const auto& point : s.points) {
            CircleShape dot(s.size);
            dot.setFillColor(s.color);
            dot.setPosition(point - Vector2f(s.size, s.size));
            canvas.draw(dot);
        }
    }
    canvas.display();
}

int main(){
    RenderWindow window(VideoMode({800, 600}), "SeleC++");
    window.setFramerateLimit(60);

// OUR TEX FOR BUTTONS

Texture penTex, eraseTex, incTex, decTex, undoTex, redoTex, stabTex;

penTex.loadFromFile("assets/Pen.png");
eraseTex.loadFromFile("assets/Erase.png");
incTex.loadFromFile("assets/Inc.png");
decTex.loadFromFile("assets/Dec.png");
undoTex.loadFromFile("assets/Undo.png");
redoTex.loadFromFile("assets/Redo.png");
stabTex.loadFromFile("assets/Stab.png");


Sprite penBtn(penTex), eraseBtn(eraseTex), incBtn(incTex);  // ts da buttons for the GUI
Sprite decBtn(decTex), undoBtn(undoTex), redoBtn(redoTex);
Sprite stabBtn(stabTex);

penBtn.setPosition({0, 0});
eraseBtn.setPosition({64, 0});     // buttons pos
incBtn.setPosition({128, 0});
decBtn.setPosition({192, 0});
undoBtn.setPosition({256, 0});
redoBtn.setPosition({320, 0});
stabBtn.setPosition({384, 0});


    colorpallete palette;
    palette.colors.push_back(Color::Black);               
    palette.colors.push_back(Color::Red);                    
    palette.colors.push_back(Color::Green);
    palette.colors.push_back(Color(157, 0, 255)); // our peak PURP
    palette.colors.push_back(Color::Blue);
    palette.colors.push_back(Color::Yellow);
    palette.colors.push_back(Color::Cyan);

    Color currentColor = palette.getcolor();  

    RenderTexture canvas({800, 600});
    canvas.clear(Color::White);
    Vector2f lastPos;
    Vector2f lastPosErase;
    float brushSize = 2.f;
    float eraserSize = 10.f;
    bool wasDrawing = false;
    bool wasErasing = false;

    vector<stroke> undo; // STORES ALL STROKES BY DEFAULT TS GONN BE RENDERED/DISPLAYED
    vector<stroke> redo; // ts gonna be stored when undone to be redone. not visible cuz no render :(
    stroke strokes; // ts da said strokes for the brush
    stroke eraserstrokes; // ts da said strokes for eraser


    stabilizer stab; // ts da stabilizer for the brush
    bool usestab= false; // will be false by default



    while (window.isOpen()){
        while (auto event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

if (auto* keyEvent = event->getIf<Event::KeyPressed>()){        // ALL KEY EVENTS


    if(keyEvent->code==Keyboard::Key::S){  // STABILIZER
        usestab = !usestab; // toggles the stabilizer
    }
    if(keyEvent->code==Keyboard::Key::Equal){  // STABILIZER INCREASE    +
        stab.stabl += 1;
    }
    if(keyEvent->code==Keyboard::Key::Hyphen){  // STABILIZER DECREASE     -
        stab.stabl -= 1;
        if(stab.stabl<1){  // makes sure its +ve
            stab.stabl=1;
        }
    }



    if(keyEvent->code==Keyboard::Key::Up){     // BRUSH SIZES
        brushSize += 1.f;
    }
     if(keyEvent->code==Keyboard::Key::Down){
        brushSize -= 1.f;
    }
     if(keyEvent->code==Keyboard::Key::Right){ // ERASER SIZES
        eraserSize += 1.f;
    }
     if(keyEvent->code==Keyboard::Key::Left){
        eraserSize -= 1.f;
    }
                    // UNDO
    if(keyEvent->code==Keyboard::Key::Z && keyEvent->control){
        if(!undo.empty()){ // checks if smthn is actally drawn
            redo.push_back(undo.back()); // adds it to the redo
            undo.pop_back(); // DESTROYS shii i.e undoes the stroke
            redraw(canvas, undo); // redraws the updated canvas 
        }
    }
                      // REDO
    if(keyEvent->code==Keyboard::Key::Y && keyEvent->control){
        if(!redo.empty()){ // checks if smthn is actally drawn
            undo.push_back(redo.back()); // adds it to the undo
            redo.pop_back(); // REVERSES shii i.e redoes the stroke
            redraw(canvas, undo); // redraws the updated canvas 
        }
    }
                        
                 // COLOR SELECTION
       if(keyEvent->code==Keyboard::Key::Num1){  // black
        palette.select = 0;
    }
    if(keyEvent->code==Keyboard::Key::Num2){    // red
        palette.select = 1;
    }
    if(keyEvent->code==Keyboard::Key::Num3){       // green 
        palette.select = 2;
    }
    if(keyEvent->code==Keyboard::Key::Num4){        // PEAKple(purple)
        palette.select = 3;
    }
    if(keyEvent->code==Keyboard::Key::Num5){       // blue
        palette.select = 4;
    }
    if(keyEvent->code==Keyboard::Key::Num6){     // yellow
        palette.select = 5;
    }
    if(keyEvent->code==Keyboard::Key::Num7){    // cyan
        palette.select = 6;
    }

}

 // WHEN MOUSE PRESSED

if (auto* mouseEvent = event->getIf<Event::MouseButtonPressed>()){     // BRUSH
    if(mouseEvent->button == Mouse::Button::Left){
        wasDrawing = true;
        lastPos = Vector2f(Mouse::getPosition(window));
        currentColor = palette.getcolor();

        strokes.points.clear();
        strokes.size = brushSize;
        strokes.color = currentColor;

        CircleShape dot(brushSize);
    dot.setFillColor(currentColor);
    dot.setPosition(lastPos - Vector2f(brushSize, brushSize));
    canvas.draw(dot);
    canvas.display();
    }
    if(mouseEvent->button == Mouse::Button::Right){        // ERASER
        wasErasing = true;
        lastPosErase = Vector2f(Mouse::getPosition(window));
        eraserstrokes.points.clear();
        eraserstrokes.size = eraserSize;
        eraserstrokes.color = Color::White;
    }
}


        // WHEN MOUSE RELEASED
if(auto* mouseEvent = event->getIf<Event::MouseButtonReleased>()){
    if(mouseEvent->button == Mouse::Button::Left && wasDrawing){
        if(!strokes.points.empty()){
            undo.push_back(strokes);
            redo.clear();
        }
        stab.clr(); // clears the stabilizer history when the mouse is released
        wasDrawing = false;
    }
    if(mouseEvent->button == Mouse::Button::Right && wasErasing){
        if(!eraserstrokes.points.empty()){
            undo.push_back(eraserstrokes);
            redo.clear();
        }
        stab.clr();
        wasErasing = false;
    }
}

if(auto* mouseEvent=event->getIf<Event::MouseMoved>()){
    if(wasDrawing){
       Vector2f start = Vector2f(lastPos);
     Vector2i rawPos = Vector2i(mouseEvent->position);
     Vector2f pos = usestab ? stab.stable(Vector2f(rawPos)) : Vector2f(rawPos);


    Vector2f end = pos;
    Vector2f diff = end - start;
    float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
    int steps = max(1, (int) dist);

    strokes.size = brushSize;
    strokes.color = currentColor;

    for (int i = 0; i <= steps; ++i) {
        float t = (float) i / steps;
        Vector2f point = start + diff * t; // LINEAR INTERPOLATION OF SM SHII
        strokes.points.push_back(point); // ts will add to the point vector

        CircleShape dot(brushSize);
        dot.setFillColor(currentColor);
        dot.setPosition(point - Vector2f(brushSize, brushSize));
        canvas.draw(dot);
    }
    
    lastPos = pos;
    canvas.display(); // Update the texture after drawing

  }
if(wasErasing){
    Vector2f start = Vector2f(lastPosErase);
    Vector2i rawPos = Vector2i(mouseEvent->position);
    Vector2f pos = usestab ? stab.stable(Vector2f(rawPos)) : Vector2f(rawPos);
    Vector2f end = pos; 
    Vector2f diff = end - start;
    float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
    int steps = max(1, (int) dist);

        eraserstrokes.size = eraserSize;
        eraserstrokes.color = Color::White;

    for (int i = 0; i <= steps; ++i) {
        float t = (float) i / steps;
        Vector2f point = start + diff * t; // LINEAR INTER

        eraserstrokes.points.push_back(point); // ts will add to the point vector

        CircleShape dot(eraserSize);
        dot.setFillColor(Color::White); // Set the color to white for erasing
        dot.setPosition(point - Vector2f(eraserSize, eraserSize));
        canvas.draw(dot);
    }
    
    lastPosErase = pos;
    canvas.display(); // Update the texture after erasing

}
}
        }
 
window.clear(Color::White);
Sprite sprite(canvas.getTexture());
window.draw(sprite);

window.draw(penBtn);
window.draw(eraseBtn);
window.draw(incBtn);
window.draw(decBtn);
window.draw(undoBtn);
window.draw(redoBtn);
window.draw(stabBtn);

window.display();
    }

    return 0;
}

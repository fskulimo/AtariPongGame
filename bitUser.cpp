//
// Created by Filip Skulimowski on 12/8/22.
//

#include "bitUser.h"
#include <fstream>
#include <iostream>
#include "graphics.h"
#include "shape.h"
const int SIDE_LENGTH = 10;


/********************* Shape Class *********************/

bitUser::bitUser() : Shape(), startingPoint({0, 0}) {
}

bitUser::bitUser(point2D start) : Shape(start), startingPoint(start) {
}

point2D bitUser::getStartingPoint() const {
    return startingPoint;
}

double bitUser::getStartingX() const {
    return startingPoint.x;
}

double bitUser::getStartingY() const {
    return startingPoint.y;
}

double bitUser::getLeftX() const {
    return startingPoint.x;
}

double bitUser::getRightX() const  {
    return startingPoint.x;
}

double bitUser::getTopY() const {
    return startingPoint.y;
}

double bitUser::getBottomY() const {
    return startingPoint.y;
}

void bitUser::setStartingPoint(point2D startingPoint) {
    this->startingPoint = startingPoint;
}

void bitUser::setStartingPoint(double x, double y) {
    startingPoint = {x, y};
}

void bitUser::setStartingX(double x) {
    startingPoint.x = x;
}

void bitUser::setStartingY(double y) {
    startingPoint.y = y;
}

void bitUser::move(double deltaX, double deltaY) {
    startingPoint.x += deltaX;
    startingPoint.y += deltaY;
}

void bitUser::moveX(double deltaX) {
    startingPoint.x += deltaX;
}

void bitUser::moveY(double deltaY) {
    startingPoint.y += deltaY;
}

void bitUser::draw() const {
    double xCoord = getStartingX();
    double yCoord = getStartingY();
    ifstream inFile(fileName);
    inFile >> noskipws;
    char letter;
    bool draw;
    while (inFile >> letter) {
        draw = true;
        switch(letter) {
            case 'r': glColor3f(1, 0, 0); break;
            case 'g': glColor3f(0.00392157, 0.670588, 0); break;
            case 'l': glColor3f(0.772549, 0.960784, 0.321568); break;
            case 'b': glColor3f(0, 0, 1); break;
            case 'y': glColor3f(0.9254902, 0.81960784, 0.70980392); break; // YELLOWISH/BROWN
            case 'm': glColor3f(1, 0, 1); break; // MAGENTA
            case 'c': glColor3f(0, 1, 1); break;
            case 'o': glColor3f(0.9, 0.8, 0.3); break;
            case 'w': glColor3f(0.84313725, 0.40784314, 0.18039216); break; // BROWN
            case 'x': glColor3f(0.35, 0.51176, 0.94705); break; // LIGHT BLUE (SKY)
            case ' ': glColor3f(0, 0, 0); break;
            default: // newline
                draw = false;
                xCoord = getStartingX();
                yCoord += SIDE_LENGTH;
        }
        if (draw) {
            glBegin(GL_QUADS);
            glVertex2i(xCoord, yCoord);
            glVertex2i(xCoord+SIDE_LENGTH, yCoord);
            glVertex2i(xCoord+SIDE_LENGTH, yCoord+SIDE_LENGTH);
            glVertex2i(xCoord, yCoord+SIDE_LENGTH);
            glEnd();
            xCoord += SIDE_LENGTH;
        }
    }
    inFile.close();
}

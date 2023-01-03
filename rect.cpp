#include "graphics.h"
#include "rect.h"
#include <iostream>
using namespace std;

/********************* Dimensions Struct ********************/

dimensions::dimensions() : width(0), height(0) {}

dimensions::dimensions(double w, double h) : width(w), height(h) {}

ostream& operator << (ostream& outs, const dimensions &d) {
    outs << "[" << d.width << ", " << d.height << "]";
    return outs;
}


Rect::Rect() : Shape(), size({0, 0}) {
}

Rect::Rect(dimensions size) : Shape() {
    setSize(size);
}

Rect::Rect(color fill) : Shape(fill), size({0, 0}) {
}

Rect::Rect(point2D center) : Shape(center), size({0, 0}) {
}

Rect::Rect(color fill, point2D center) : Shape(fill, center), size({0, 0}) {
}

Rect::Rect(double red, double green, double blue, double alpha) : Shape(red, green, blue, alpha), size({0, 0}) {
}

Rect::Rect(double x, double y) : Shape(x, y), size({0, 0}) {
}

Rect::Rect(double red, double green, double blue, double alpha, double x, double y) : Shape(red, green, blue, alpha, x, y), size({0, 0}) {
}

Rect::Rect(color fill, double x, double y) : Shape(fill, x, y), size({0, 0}) {
}

Rect::Rect(double red, double green, double blue, double alpha, point2D center) : Shape(red, green, blue, alpha, center), size({0, 0}) {
}

Rect::Rect(color fill, dimensions size) : Shape(fill) {
    setSize(size);
}

Rect::Rect(point2D center, dimensions size) : Shape(center) {
    setSize(size);
}

Rect::Rect(color fill, point2D center, dimensions size) : Shape(fill, center) {
    setSize(size);
}

Rect::Rect(double red, double green, double blue, double alpha, dimensions size) : Shape(red, green, blue, alpha) {
    setSize(size);
}

Rect::Rect(double x, double y, dimensions size) : Shape(x, y) {
    setSize(size);
}

Rect::Rect(double red, double green, double blue, double alpha, double x, double y, dimensions size) : Shape(red, green, blue, alpha, x, y) {
    setSize(size);
}

Rect::Rect(color fill, double x, double y, dimensions size) : Shape(fill, x, y) {
    setSize(size);
}

Rect::Rect(double red, double green, double blue, double alpha, point2D center, dimensions size) : Shape(red, green, blue, alpha, center) {
    setSize(size);
}

dimensions Rect::getSize() const {
    return size;
}

double Rect::getWidth() const {
    return size.width;
}

double Rect::getHeight() const {
    return size.height;
}

double Rect::getLeftX() const {
    return center.x - (size.width / 2.0);
}

double Rect::getRightX() const {
    return center.x + (size.width / 2.0);
}

double Rect::getTopY() const {
    return center.y - (size.height / 2.0);
}

double Rect::getBottomY() const {
    return center.y + (size.height / 2.0);
}

void Rect::setSize(dimensions size) {
    if (size.width >= 0 && size.height >= 0) {
        this->size = size;
    }
}

void Rect::setSize(double width, double height) {
    setSize({width, height});
}

void Rect::setWidth(double width) {
    setSize({width, size.height});
}

void Rect::setHeight(double height) {
    setSize({size.width, height});
}

void Rect::changeSize(double deltaWidth, double deltaHeight) {
    setSize({size.width + deltaWidth, size.height + deltaHeight});
}

void Rect::changeWidth(double delta) {
    setSize({size.width + delta, size.height});
}

void Rect::changeHeight(double delta) {
    setSize({size.width, size.height + delta});
}

//Move
void Rect::move(double deltaX, double deltaY, double deltaZ)
{
    center.x += deltaX;
    center.y += deltaY;
    center.z += deltaZ;
    for (point2D &p : corners)
    {
        p.x += deltaX;
        p.y += deltaY;
        p.z += deltaZ;
    }
}

//Rotate
void Rect::rotate(double theta_x, double theta_y, double theta_z)
{
    double old_x = 0, old_y = 0, old_z = 0;
    double center_x = center.x, center_y = center.y, center_z = center.z;
    // Move to origin
    move(-center_x, -center_y, -center_z);
    // Rotate x
    for (point2D &p : corners) {
        old_y = p.y;
        old_z = p.z;
        p.y = old_y * cos(theta_x) - old_z * sin(theta_x);
        p.z = old_y * sin(theta_x) + old_z * cos(theta_x);
    }
    // Rotate y
    for (point2D &p : corners)
    {
        old_x = p.x;
        old_z = p.z;
        p.x = old_x * cos(theta_y) - old_z * sin(theta_y);
        p.z = old_x * sin(theta_y) + old_z * cos(theta_y);
    }
    //Rotate z
    for (point2D &p : corners)
    {
        old_x = p.x;
        old_y = p.y;
        p.x = old_x * cos(theta_z) - old_y * sin(theta_z);
        p.y = old_x * sin(theta_z) + old_y * cos(theta_z);
    }
    // Move back to position
    move(center_x, center_y, center_z);
}

// TODO: Implement this method
bool Rect::isOverlapping(const Rect &r) const {
    // There are only two cases when rectangles are *not* overlapping:
    // 1. when one is to the left of the other
    if(center.x < r.getCenterX())
    {
        if((center.x + (size.width / 2.0)) < r.getLeftX())
        {
            return false;
        }
    }
    else
    {
        if(r.getRightX() < (center.x - (size.width / 2.0)))
        {
            return false;
        }
    }

    // 2. when one is above the other
    if(center.y < r.getCenterY())
    {
        if((center.y + (size.height / 2.0)) < r.getTopY())
        {
            return false;
        }
    }
    else
    {
        if(r.getBottomY() < (center.y - (size.height / 2.0)))
        {
            return false;
        }
    }

    return true; // placeholder value for compilation
}

// TODO: Implement this method
void Rect::draw() const {
    // Don't forget to set the color to the fill field
    glColor3f(fill.red, fill.green, fill.blue);
    glBegin(GL_QUADS);

    glVertex2i(getLeftX(), getTopY());
    glVertex2i(getRightX(), getTopY());
    glVertex2i(getRightX(), getBottomY());
    glVertex2i(getLeftX(), getBottomY());
    glEnd();

    //glFlush();
}

void Rect::drawBlack() const {
    // Set drawing color to fill color
    glColor3f(0, 0, 0);
    // Draw rectangle as GL_QUADS
    glBegin(GL_QUADS);
    // Draw each corner point
    // glVertex2i takes a 2-D (x, y) coordinate
    glVertex2i(getLeftX(), getTopY());
    glVertex2i(getRightX(), getTopY());
    glVertex2i(getRightX(), getBottomY());
    glVertex2i(getLeftX(), getBottomY());
    // End Quad
    glEnd();
}
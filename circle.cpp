#include "circle.h"
#include "graphics.h"
#include "rect.h"


Circle::Circle() : Shape(), radius(0) {
}

Circle::Circle(double radius) : Shape() {
    setRadius(radius);
}

Circle::Circle(color fill) : Shape(fill), radius(0) {
}

Circle::Circle(point2D center) : Shape(center), radius(0) {
}

Circle::Circle(color fill, point2D center) : Shape(fill, center), radius(0) {
}

Circle::Circle(double red, double green, double blue, double alpha) : Shape(red, green, blue, alpha), radius(0) {
}

Circle::Circle(double x, double y) : Shape(x, y), radius(0) {
}

Circle::Circle(double red, double green, double blue, double alpha, double x, double y) : Shape(red, green, blue, alpha, x, y), radius(0) {
}

Circle::Circle(color fill, double x, double y) : Shape(fill, x, y), radius(0) {
}

Circle::Circle(double red, double green, double blue, double alpha, point2D center) : Shape(red, green, blue, alpha, center), radius(0) {
}

Circle::Circle(color fill, double radius) : Shape(fill) {
    setRadius(radius);
}

Circle::Circle(point2D center, double radius) : Shape(center) {
    setRadius(radius);
}

Circle::Circle(color fill, point2D center, double radius) : Shape(fill, center) {
    setRadius(radius);
}

Circle::Circle(double red, double green, double blue, double alpha, double radius) : Shape(red, green, blue, alpha) {
    setRadius(radius);
}

Circle::Circle(double x, double y, double radius) : Shape(x, y) {
    setRadius(radius);
}

Circle::Circle(double red, double green, double blue, double alpha, double x, double y, double radius) : Shape(red, green, blue, alpha, x, y) {
    setRadius(radius);
}

Circle::Circle(color fill, double x, double y, double radius) : Shape(fill, x, y) {
    setRadius(radius);
}

Circle::Circle(double red, double green, double blue, double alpha, point2D center, double radius) : Shape(red, green, blue, alpha, center) {
    setRadius(radius);
}

Circle::Circle(double redFill, double greenFill, double blueFill, double alphaFill, double redBorder, double greenBorder, double blueBorder, double alphaBorder, double x, double y, double radius) : Shape(redFill, greenFill, blueFill, alphaFill, redBorder, greenBorder, blueBorder, alphaBorder, x, y) {
    setRadius(radius);
}

double Circle::getRadius() const {
    return radius;
}

double Circle::getLeftX() const {
    return center.x - radius;
}

double Circle::getRightX() const  {
    return center.x + radius;
}

double Circle::getTopY() const {
    return center.y - radius;
}

double Circle::getBottomY() const {
    return center.y + radius;
}

void Circle::setRadius(double r) {
    if (r < 0) {
        r = 0;
    }
    radius = r;
}

void Circle::changeRadius(double delta) {
    setRadius(radius + delta);
}

bool Circle::isOverlapping(const Rect &r) const {
    // return true if the distance between the center points is <= the sum of the radii with rectangles width/2
    return (sqrt((r.getWidth()/2 - center.x)*(r.getWidth()/2 - center.x) + (r.getHeight()/2 - center.y)*(r.getHeight()/2 - center.y)) <= r.getWidth()/2 + radius);
}

void Circle::draw() const {
    // Set drawing color to fill color
    glColor3f(fill.red, fill.green, fill.blue);
    // Draw circle as Triangle Fan
    glBegin(GL_TRIANGLE_FAN);
    // Draw center point
    glVertex2i(center.x, center.y);
    // Draw points on edge of circle
    for (double i = 0; i < 2.0*PI+0.05; i += (2.0*PI)/360.0) {
        glVertex2i(center.x + (radius * cos(i)),
                   center.y + (radius * sin(i)));
    }
    // End Triangle Fan
    glEnd();
}

void Circle::drawBall() const {
    // Set drawing color to fill color
    glColor3f(fill.red, fill.green, fill.blue);
    // Draw circle as Triangle Fan
    glBegin(GL_TRIANGLE_FAN);
    // Draw center point
    glVertex2i(center.x, center.y);
    // Set drawing color to border color
    glColor3f(border.red, border.green, border.blue);
    // Draw points on edge of circle
    for (double i = 0; i < 2.0*PI+0.05; i += (2.0*PI)/360.0) {
        glVertex2i(center.x + (radius * cos(i)),
                   center.y + (radius * sin(i)));
    }
    // End Triangle Fan
    glEnd();
}

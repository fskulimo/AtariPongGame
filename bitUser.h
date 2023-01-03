//
// Created by Filip Skulimowski on 12/8/22.
//

#ifndef FINAL_PROJECT_BITUSER_H
#define FINAL_PROJECT_BITUSER_H
#include <string>
#include "shape.h"
using namespace std;

const string fileName = "../scene.txt";


class bitUser : public Shape {
protected:
    point2D startingPoint;

public:
    /* Constructors */
    bitUser();
    explicit bitUser(point2D startingPoint);

    /* Getters */
    point2D getStartingPoint() const;
    double getStartingX() const;
    double getStartingY() const;
    double getLeftX() const override;
    double getRightX() const override;
    double getTopY() const override;
    double getBottomY() const override;

    /* Setters */
    void setStartingPoint(point2D point);
    void setStartingPoint(double, double y);
    void setStartingX(double x);
    void setStartingY(double y);
    virtual void move(double deltaX, double deltaY) override;
    virtual void moveX(double deltaX) override;
    virtual void moveY(double deltaY) override;

    void draw() const override;
};


#endif //FINAL_PROJECT_BITUSER_H

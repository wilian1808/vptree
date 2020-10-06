#ifndef PUNTO_H_INCLUDED
#define PUNTO_H_INCLUDED

class Punto
{
private:
    int x;
    int y;
public:
    Punto(int x, int y);
    int getX();
    int getY();
};

Punto::Punto(int x = 0, int y = 0)
{
    this->x = x;
    this->y = y;
}

int Punto::getX()
{
    return this->x;
}

int Punto::getY()
{
    return this->y;
}


#endif // PUNTO_H_INCLUDED

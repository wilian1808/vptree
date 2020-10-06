#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED

#include "Punto.h"

class Nodo
{
private:
    Nodo *left;
    Nodo *right;
    Punto punto;
    int eje;
    int radio;
public:
    Nodo(Punto punto, Nodo *left, Nodo *right, int eje, int radio);
    Nodo *&getLeft();
    Nodo *&getRight();
    Punto getPunto();
    int getEje();
    int getRadio();
    void setRadio(int radio);
};

Nodo::Nodo(Punto punto, Nodo *left = NULL, Nodo *right = NULL, int eje = 0, int radio = 0)
{
    this->punto = punto;
    this->left = left;
    this->right = right;
    this->eje = eje;
    this->radio = radio;
}

Nodo *&Nodo::getLeft()
{
    return this->left;
}

Nodo *&Nodo::getRight()
{
    return this->right;
}

Punto Nodo::getPunto()
{
    return this->punto;
}

int Nodo::getEje()
{
    return this->eje;
}

int Nodo::getRadio()
{
    return this->radio;
}

void Nodo::setRadio(int radio)
{
    this->radio = radio;
}

#endif // NODO_H_INCLUDED

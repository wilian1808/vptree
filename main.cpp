#include <iostream>
#include <windows.h>
#include <vector>
#include <math.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>


#include <math.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <iostream>

#include "resource.h"
#include "Punto.h"
#include "Nodo.h"

HINSTANCE hInst;

int xInicial;
int xFinal;
int yInicial;
int yFinal;

void SetPixel2(HDC hdc,int x,int y,COLORREF Color)
{
    if(x>xInicial && x<xFinal && y>yInicial && y<yFinal)
        SetPixel(hdc, x, y, Color);
}
void Draw8Points(HDC hdc,int xc,int yc, int a, int b,COLORREF Color)
{
    SetPixel2(hdc, xc+a, yc+b, Color);
    SetPixel2(hdc, xc-a, yc+b, Color);
    SetPixel2(hdc, xc-a, yc-b, Color);
    SetPixel2(hdc, xc+a, yc-b, Color);
    SetPixel2(hdc, xc+b, yc+a, Color);
    SetPixel2(hdc, xc-b, yc+a, Color);
    SetPixel2(hdc, xc-b, yc-a, Color);
    SetPixel2(hdc, xc+b, yc-a, Color);
}
void CircleBresenham(HDC hdc,int xc,int yc, int R,int rrr, COLORREF color)
{
    int x=0,y=R;
    int d=1-R;
    Draw8Points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        std::cout << "CIRCULO DIBUJADO" << std::endl;
        if(d<0)
            d+=2*x+2;
        else
        {
            d+=2*(x-y)+5;
            y-=rrr;
        }
        x+=rrr;
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}

class VPTree
{
    private:
    Nodo *root;
    void addingNodo(Punto punto, Nodo *&root, int eje, int radio);
    void LookingNodo(HDC hdc, Nodo *root, int xInicial, int yInicial, int a);

    void viewingRectangle(HDC hdc, Nodo *root, int xInicial, int yInicial, int xFinal, int yFinal);

    float getDistance(int x1, int y1, int x2, int y2);

public:
    VPTree();
    void addNodo(Punto punto);
    void lookNodo(HDC hdc, int xInicial, int yInicial, int xFinal, int yFinal);
    void viewRectangle(HDC hdc, int xInicial, int yInicial, int xFinal, int yFinal);

    // FUNCION PARA DIBUJAR LOS CIRCULOS
};

VPTree::VPTree()
{
    this->root = NULL;
}


//
float VPTree::getDistance(int x1, int y1, int x2, int y2)
{
    int one = pow((x2 - x1), 2);
    int two = pow((y2 - y1), 2);
    float res = sqrt(one + two);
    return (int)res;
}

void VPTree::addNodo(Punto punto)
{
    this->addingNodo(punto, this->root, 0, 0);
}

void VPTree::addingNodo(Punto punto, Nodo *&root, int eje, int radio)
{
    if (root == NULL)
    {
        root = new Nodo(punto, NULL, NULL, eje, 0);
    } else
    {
        // YA ES EL SEGUNDO NODO
        if (root->getRadio() == 0)
        {
            int x1 = root->getPunto().getX();
            int y1 = root->getPunto().getY();
            int x2 = punto.getX();
            int y2 = punto.getY();

            int distancia = this->getDistance(x1, y1, x2, y2);
            std::cout << x1 << ":" << y1 << " DISTANCIA ACTUAL: " << root->getRadio() << std::endl;
            std::cout << x2 << ":" << y2 << " DISTANCIA NUEVA:  " << distancia << std::endl << std::endl;

            root->setRadio(distancia);
            this->addingNodo(punto, root->getLeft(), 1, 1);

        } else
        {
            // SI EL RADIO NO ES 0 SE COMPARA EL RADIO ACTUAL CON EL RADIO DEL NUEVO
            // NODO PARA SABER A DONDE SE VA
            // COMPARAMOS LA DISTANCIA ACTUAL DEL NODO ACTUAL Y LA DEL NODO ACTUAL CON EL NUEVO
            int x1 = root->getPunto().getX(), y1 = root->getPunto().getY();
            int x2 = punto.getX(), y2 = punto.getY();
            int distancia = this->getDistance(x1, y1, x2, y2);
            std::cout << x1 << ":" << y1 << " DISTANCIA ACTUAL: " << root->getRadio() << std::endl;
            std::cout << x2 << ":" << y2 << " DISTANCIA NUEVA:  " << distancia << std::endl;
            // AHORA COMPARAMOS PARA SABER SI SE INSERTA EN EL NODO IZQUIERDO O DERECHO
            if (distancia <= root->getRadio())
                this->addingNodo(punto, root->getLeft(), 0, 0);
            else
                this->addingNodo(punto, root->getRight(), 0, 0);
        }
    }
}

void VPTree::lookNodo(HDC hdc, int xInicial, int yInicial, int xFinal, int yFinal)
{
    this->LookingNodo(hdc, this->root, (xInicial+xFinal)/2, yInicial+20, xFinal-xInicial);
}

void VPTree::LookingNodo(HDC hdc, Nodo  *root, int xInicial, int yInicial, int a)
{
    if (root != NULL)
    {
        char datos[30];

        if (root->getLeft() != NULL)
        {
            MoveToEx(hdc, xInicial, yInicial, NULL);
            LineTo(hdc, xInicial-a/2, yInicial+50);
        }

        if (root->getRight() != NULL)
        {
            MoveToEx(hdc, xInicial, yInicial, NULL);
            LineTo(hdc, xInicial+a/2, yInicial+50);
        }

        Rectangle(hdc, xInicial-3, yInicial-3, xInicial+28, yInicial+48);
        sprintf(datos, "%i", int(root->getPunto().getX()));
        TextOut(hdc, xInicial, yInicial, datos, strlen(datos));
        sprintf(datos, "%i", int(root->getPunto().getY()));
        TextOut(hdc, xInicial, yInicial+15, datos, strlen(datos));


        // PARA MOSTRAR EL RADIO ACTUAL
        std::stringstream ss;
        ss << root->getRadio();
        char st[2000];
        sprintf(st, "%d", root->getRadio());


        TextOut(hdc, xInicial, yInicial+30, st, 3);

        this->LookingNodo(hdc, root->getLeft(), xInicial-a/2, yInicial+50, a/2);
        this->LookingNodo(hdc, root->getRight(), xInicial+a/2, yInicial+50, a/2);
    }
}


// PARA LOS GRAFICOS DE ESPACIOS DIMENSIONALES
void VPTree::viewRectangle(HDC hdc, int xInicial, int yInicial, int xFinal, int yFinal)
{
    this->viewingRectangle(hdc, this->root, xInicial, yInicial, xFinal, yFinal);
}
// EDITAR PARA CONSEGUIR EL CIRCULO Y TAMBIEN AÑADIR UN RADIO O MEDIA
void VPTree::viewingRectangle(HDC hdc, Nodo *root, int xInicial, int yInicial, int xFinal, int yFinal)
{
    if (root != NULL)
    {
        // OBTENEMOS LOS PUNTOS 'X' Y 'Y' DEL NODO ACTUAL
        int puntoX = root->getPunto().getX();
        int puntoY = root->getPunto().getY();

        // COMPROBAMOS SI EL EJE ES EL 1 OSEA EL EJE X
        if (root->getEje() == 1)
        {
            MoveToEx(hdc, xInicial, puntoY, NULL);

            // LLAMAMOS AL METODO PARA DIBUJAR EL CIRCULO Y TENDREMOS QUE PASARLE
            // PRIMERO EL HDC LUEGO
            //
            // CircleBresenham(hdc,puntoX,puntoY, root->getRadio(), 0, 000);
            //
            // xInicial == pxi
            // yInicial == pyi
            // xFinal == pxf
            // yFinal == pyf
            CircleBresenham(hdc, puntoX, puntoY, root->getRadio(), 1, RGB(root->getRadio(),0,0));

            this->viewingRectangle(hdc, root->getLeft(), xInicial, yInicial, xFinal, puntoY);
            this->viewingRectangle(hdc, root->getRight(), xInicial, puntoY, xFinal, yFinal);
        } else
        {
            MoveToEx(hdc, puntoX, yInicial, NULL);
            CircleBresenham(hdc, puntoX, puntoY, root->getRadio(), 1, RGB(root->getRadio(),0,0));
            this->viewingRectangle(hdc, root->getLeft(), xInicial, yInicial, puntoX, yFinal);
            this->viewingRectangle(hdc, root->getRight(), puntoX, yInicial, xFinal, yFinal);
        }

        Ellipse(hdc, puntoX-5, puntoY-5, puntoX+5, puntoY+5);
    }
}

VPTree tree;

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc;
        hdc = BeginPaint(hwndDlg, &ps);
        // ESTOS ES PARA LOS RECUADROS DEL ARBOL - NODO
        tree.lookNodo(hdc, 380, 20, 720, 400);
        // ESTO ES PARA DIBUJAR EL RECTANGULO DENTRO DEL RECUADRO GRANDE
        Rectangle(hdc, xInicial, yInicial, xFinal, yFinal);
        /*xInicial = 10;
        yInicial = 50;
        xFinal = 210;
        yFinal = 250;*/
        // xInicial == pxi
        // yInicial == pyi
        // xFinal == pxf
        // yFinal == pyf
        tree.viewRectangle(hdc, xInicial, yInicial, xFinal, yFinal);
        EndPaint(hwndDlg, &ps);
    }
    return TRUE;

    case WM_LBUTTONDOWN:
    {
        long xValue = GET_X_LPARAM(lParam);
        long yValue = GET_Y_LPARAM(lParam);
        if (xValue<xFinal && xValue > xInicial && yValue < yFinal && yValue > yInicial)
        tree.addNodo(Punto(xValue, yValue));
        InvalidateRect(hwndDlg, NULL, true);
    }
    return TRUE;

    // DLG_MAIN DIALOG 0, 0, 600, 350
    case WM_INITDIALOG:
    {
        xInicial = 10;
        yInicial = 50;
        xFinal = 210;
        yFinal = 250;
    }
    return TRUE;

    case WM_MOUSEMOVE:
    {
        long xValue = GET_X_LPARAM(lParam);
        long yValue = GET_Y_LPARAM(lParam);
        SetDlgItemInt(hwndDlg, EDITX, (int)xValue, FALSE);
        SetDlgItemInt(hwndDlg, EDITY, (int)yValue, FALSE);
    }
    return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        }
    }
    return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}

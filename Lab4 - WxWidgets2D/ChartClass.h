#ifndef __ChartClass__
#define __ChartClass__

#include <memory>
#include "ConfigClass.h"
class Matrix;

class ChartClass
{

private:
    std::shared_ptr<ConfigClass> cfg;
    int x_step;         // liczba odcinkow z jakich bedzie sie skladal wykres
    double x_min, x_max; // zakres zmiennej x
    double y_min, y_max; // zakres wartosci przyjmowanych przez funkcje

    //FUNKCJE PODSTAWOWE//////////////////////////////////
    double GetFunctionValue(double x); // zwraca wartosci rysowanej funkcji

    //FUNKCJE NA MACIERZACH///////////////////////////////
    Matrix Set_Translate(double Tx, double Ty);
    Matrix Set_Rotation(double alpha);
    Matrix Set_Scale(int w, int h);

    //FUNKCJE RYSUJACE/OBSLUGUJACE///////////////////////
    void line2d(wxDC* DC, Matrix t, double x1, double y1, double x2, double y2, int w, int h);
    void setTransformM(Matrix& transformationM, double x0, double y0, int w, int h);
    void setCenter(double& x0, double& y0, int w, int h);
    void drawAxes(wxDC* DC, Matrix transformationM, int w, int h);
    void drawArrows(wxDC* DC, Matrix transformationM, int w, int h);
    void drawMarkX(wxDC* DC, Matrix transformationM, double v, int w, int h);
    void drawMarkY(wxDC* DC, Matrix transformationM, double v, int w, int h);
    void drawFunction(wxDC* DC, Matrix transformationM, int w, int h, double step);
    void setStep(double& step);

public:
    
    //FUNKCJE PODSTAWOWE//////////////////////////////////
    ChartClass(std::shared_ptr<ConfigClass> c);
    void Set_Range();   // ustala wartosci zmiennych x_min,y_min,x_max,y_max
    double Get_Y_min(); // zwraca y_min
    double Get_Y_max(); // zwraca y_max

    //FUNKCJE RYSUJACE////////////////////////////////////
    void Draw(wxDC* DC, int w, int h);  // rysuje wykres
};

#endif

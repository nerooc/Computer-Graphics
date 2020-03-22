#include <sfml/window.hpp>
#include <sfml/graphics.hpp>
#include <iostream>
#include <string>
#include <iomanip>
#include <math.h>
#define PI 3.14159


float Hue_2_RGB(float v1, float v2, float vH) { //Function Hue_2_RGB
    if (vH < 0) vH += 1;
    if (vH > 1) vH -= 1;
    if ((6 * vH) < 1) return (v1 + (v2 - v1) * 6 * vH);
    if ((2 * vH) < 1) return (v2);
    if ((3 * vH) < 2) return (v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6);
    return (v1);
}


void colorPixels(sf::Uint8* pixels, int X, int x, int y, char r, char g, char b, char alpha) {
    pixels[4 * (y * X + x)] = r;
    pixels[4 * (y * X + x) + 1] = g;
    pixels[4 * (y * X + x) + 2] = b;
    pixels[4 * (y * X + x) + 3] = alpha;
}


void HSVtoRGB(float& R, float& G, float& B, float H, float S, float V) { //algorytm ze strony easyrgb.com
   
    if (S == 0){
        R = V * 255.0;
        G = V * 255.0;
        B = V * 255.0;
    }

    else {

        float var_h, var_i, var_1, var_2, var_3, var_r, var_g, var_b;

        var_h = H * 6.0;
        if (var_h == 6.0) var_h = 0;
        var_i = (int)(var_h);
        var_1 = V * (1.0 - S);
        var_2 = V * (1.0 - S * (var_h - var_i));
        var_3 = V * (1.0 - S * (1.0 - (var_h - var_i)));

        if (var_i == 0) { var_r = V; var_g = var_3; var_b = var_1; }
        else if (var_i == 1) { var_r = var_2; var_g = V; var_b = var_1; }
        else if (var_i == 2) { var_r = var_1; var_g = V; var_b = var_3; }
        else if (var_i == 3) { var_r = var_1; var_g = var_2; var_b = V; }
        else if (var_i == 4) { var_r = var_3; var_g = var_1; var_b = V; }
        else { var_r = V; var_g = var_1; var_b = var_2; }

        R = var_r * 255;
        G = var_g * 255;
        B = var_b * 255;
    }
}


void HSLtoRGB(float& R, float& G, float& B, float H, float S, float L) { //algorytm ze strony easyrgb.com

    if (S == 0){

        R = L * 255;
        G = L * 255;
        B = L * 255;
    }

    else{

        float var_2, var_1;

        if (L < 0.5) var_2 = L * (1 + S);
        else  var_2 = (L + S) - (S * L);

        var_1 = 2 * L - var_2;

        R = 255 * Hue_2_RGB(var_1, var_2, H + (1.0 / 3.0));
        G = 255 * Hue_2_RGB(var_1, var_2, H);
        B = 255 * Hue_2_RGB(var_1, var_2, H - (1.0 / 3.0));
    }
}


void writeText(sf::Text text, sf::RenderWindow& window, float posX, float posY, std::string txt) {
    text.setPosition(posX, posY);
    text.setString(txt);
    window.draw(text);
}


void writeCMY(sf::Text text, sf::RenderWindow& window, int middle) {
    int x = middle * 100 / 255.0;
    std::string initString = "Y = ";
    std::string temp = std::to_string(x);
    std::string final = temp.substr(0, temp.find(".") + 4);
    initString.append(final);
    initString.append("%");
    writeText(text, window, 220, 600, initString);
    writeText(text, window, 40, 350, "CMY");
}


void writeRGB(sf::Text text, sf::RenderWindow& window, int middle) {
    int x = middle;
    std::string initString = "B = ";
    std::string temp = std::to_string(x);
    std::string final = temp.substr(0, temp.find(".") + 4);
    initString.append(final);
    writeText(text, window, 550, 600, initString);
    writeText(text, window, 370, 350, "RGB");
}


void writeHSV(sf::Text text, sf::RenderWindow& window, int middle) {
    float x = middle / 255.0;
    std::string initString = "V = ";
    std::string temp = std::to_string(x);
    std::string final = temp.substr(0, temp.find(".") + 4);
    initString.append(final);
    writeText(text, window, 550, 290, initString);
    writeText(text, window, 370, 40, "HSV");
}


void writeHSL(sf::Text text, sf::RenderWindow& window, int middle) {
    float x = middle / 255.0;
    std::string initString = "L = ";
    std::string temp = std::to_string(x);
    std::string final = temp.substr(0, temp.find(".") + 4);
    initString.append(final);
    writeText(text, window, 220, 290, initString);
    writeText(text, window, 40, 40, "HSL");
}


//funkcja kolorująca koło HSL
void drawCircleHSL(float& X, float& Y, float& Rad, float& Rad2, float& alpha, float& cosAlpha, int& x, int& y, sf::Uint8* HSL, int middle, float& R, float& G, float& B, float& H, float& S, float& L) {

    X = x - Rad;
    Y = Rad - y;

    Rad2 = sqrt(X * X + Y * Y);

    cosAlpha = X / Rad2;
    alpha = acosl(cosAlpha);

    if (y > 100) {
        alpha = (2 * PI) - alpha;
    }

    H = (alpha / (2 * PI));
    S = Rad2 / Rad;
    L = middle / 255.0;

    if (Rad2 <= 100){
        HSLtoRGB(R, G, B, H, S, L);
    }

    if (Rad2 >= Rad){
        colorPixels(HSL, 200, x, y, 0, 0, 0, 0);
    }
    else{
        colorPixels(HSL, 200, x, y, R, G, B, 255);
    }
}


//funkcja kolorująca koło HSV
void drawCircleHSV(float& X, float& Y, float& Rad, float& Rad2, float& alpha, float& cosAlpha, int& x, int& y, sf::Uint8* HSV, int middle, float& R, float& G, float& B, float& H, float& S, float& V) {
    X = x - Rad;
    Y = Rad - y;

    Rad2 = sqrt(X * X + Y * Y);
    cosAlpha = X / Rad2;
    alpha = acosl(cosAlpha);

    if (y > 100) {
        alpha = (2 * PI) - alpha;
    }

    H = alpha / (2 * PI);
    S = Rad2 / 100;
    V = middle / 255.0;

    if (Rad2 < Rad) {
        HSVtoRGB(R, G, B, H, S, V);
    }

    if (Rad2 >= Rad) {
        colorPixels(HSV, 200, x, y, 0, 0, 0, 0); 
    }

    else {
        colorPixels(HSV, 200, x, y, R, G, B, 255);
    }
}


//funkcja kolorująca koło RGB
void drawCircleRGB(float& X, float& Y, float& Rad, float& Rad2, float& alpha, float& cosAlpha, int& x, int& y, sf::Uint8* RGB, int middle) {
    X = x - Rad;
    Y = Rad - y;
    
    Rad2 = sqrt(X * X + Y * Y);
    cosAlpha = X / Rad2;
    alpha = acosl(cosAlpha);

    if (y > 100) {
        alpha = (2 * PI) - alpha;
    }

    if (Rad2 >= Rad) {
        colorPixels(RGB, 200, x, y, 0, 0, 0, 0);
    }

    else {
        colorPixels(RGB, 200, x, y, 255 * Rad2 / Rad, 255 * alpha / (2 * PI), middle, 255);
    }
}


//funkcja kolorująca koło CMY
void drawCircleCMY(float& X, float& Y, float& Rad, float& Rad2, float& alpha, float& cosAlpha, int& x, int& y, sf::Uint8* CMY, int middle) {
    X = x - Rad;
    Y = Rad - y;
    
    Rad2 = sqrt(X * X + Y * Y);

    cosAlpha = X / Rad2;
    alpha = acosl(cosAlpha);

    if (y > 100) {
        alpha = (2 * PI) - alpha;
    }

    if (Rad2 >= Rad) {
        colorPixels(CMY, 200, x, y, 0, 0, 0, 0);
    }

    else {
        colorPixels(CMY, 200, x, y, 255 - 255 * Rad2 / Rad, 255 - 255 * alpha / (2 * PI), 255 - middle, 255);
    }
}


void setHSL(sf::Texture* txt, int middle) {

    //zmienne 
    float H, S, L;
    float R, G, B;

    //szerokość koła
    int circle_x = 200;
    //wysokość koła
    int circle_y = 200;

    //promień koła
    float Rad = circle_x / 2;

    //kąt alfa, cosinus kąta alfa, promień zależny od odległości od środka koła, wartości X i Y w układzie współrzędnych od środka koła
    float alpha, cosAlpha, Rad2, X, Y;

    //nowa tablica pikseli
    sf::Uint8* HSL = new sf::Uint8[circle_x * circle_y * 4];

    //wypełniamy tablicę w pętli i rysujemy z niej koło
    for (int x = 0; x < 2 * Rad; x++) {

        for (int y = 0; y < 2 * Rad; y++) {

            drawCircleHSL(X, Y, Rad, Rad2, alpha, cosAlpha, x, y, HSL, middle, R, G, B, H, S, L);

        }
    }

    //aktualizujemy teksturę podaną jako argument
    txt->update(HSL);
}
 

void setHSV(sf::Texture* txt, int middle){
    //zmienne 
    float H, S, V;
    float R, G, B;

    //szerokość koła
    int circle_x = 200;
    //wysokość koła
    int circle_y = 200;

    //promień koła
    float Rad = circle_x/2;

    //kąt alfa, cosinus kąta alfa, promień zależny od odległości od środka koła, wartości X i Y w układzie współrzędnych od środka koła
    float alpha, cosAlpha, Rad2, X, Y;

    //nowa tablica pikseli
    sf::Uint8* HSV = new sf::Uint8[circle_x * circle_y * 4];

    //wypełniamy tablicę w pętli i rysujemy z niej koło
    for (int x = 0; x < circle_x; x++) {

        for (int y = 0; y < circle_y; y++) {

            drawCircleHSV(X, Y, Rad, Rad2, alpha, cosAlpha, x, y, HSV, middle, R, G, B, H, S, V);
            
        }
    }

    //aktualizujemy teksturę podaną jako argument
    txt -> update(HSV);
}


void setRGB(sf::Texture* txt, int middle){

    //szerokość koła
    int circle_x = 200;
    //wysokość koła
    int circle_y = 200;

    //promień koła
    float Rad = circle_x/2;

    //kąt alfa, cosinus kąta alfa, promień zależny od odległości od środka koła, wartości X i Y w układzie współrzędnych od środka koła
    float alpha, cosAlpha, Rad2, X, Y;

    //nowa tablica pikseli
    sf::Uint8* RGB = new sf::Uint8[circle_x * circle_y * 4];

    //wypełniamy tablicę w pętli i rysujemy z niej koło
    for (int x = 0; x < circle_x; ++x) {

        for (int y = 0; y < circle_y; ++y){

            drawCircleRGB(X, Y, Rad, Rad2, alpha, cosAlpha, x, y, RGB, middle);

        }
    }

    //aktualizujemy teksturę podaną jako argument
    txt->update(RGB);
}

void setCMY(sf::Texture* txt, int middle){
    //szerokość koła
    int circle_x = 200;
    //wysokość koła
    int circle_y = 200;

    //promień koła
    float Rad = circle_x / 2;

    //kąt alfa, cosinus kąta alfa, promień zależny od odległości od środka koła, wartości X i Y w układzie współrzędnych od środka koła
    float alpha, cosAlpha, Rad2, X, Y;

    //nowa tablica pikseli
    sf::Uint8* CMY = new sf::Uint8[circle_x * circle_y * 4];

    //wypełniamy tablicę w pętli i rysujemy z niej koło
    for (int x = 0; x < circle_x; ++x) {

        for (int y = 0; y < circle_y; ++y) {

            drawCircleCMY(X, Y, Rad, Rad2, alpha, cosAlpha, x, y, CMY, middle);

        }
    }

    //aktualizujemy teksturę podaną jako argument
    txt->update(CMY);
}


    int main(){

        //definiujemy okno, jego rozmiary i tytuł
        sf::RenderWindow window(sf::VideoMode(730, 650), "GFK Lab 02 - Tomasz Gajda", sf::Style::Titlebar | sf::Style::Close);
       
        //uzywana czcionka pobierana z pliku
        sf::Font montserrat; 
        montserrat.loadFromFile("Montserrat-Regular.ttf");
        
        //czas i licznik FPS
        sf::Clock clock;
        sf::Time time = sf::Time::Zero;
        unsigned int FPS = 0, frame_counter = 0;
        
        //zmienna wskazująca środek palety szarości
        int middle = 127;
        
        //inicjacja tekstu używanego w programie
        sf::Text text;
        text.setFont(montserrat);
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::Black);

        //inicjacja palety szarości, z której będziemy wybierać wartości
        sf::Uint8* valuePicker;
        valuePicker = new sf::Uint8[50 * 255 * 4];

        for (int x = 0; x < 50; x++) {
            for (int y = 0; y < 255; y++) {
                colorPixels(valuePicker, 50, x, y, y, y, y, 255);
            }
        }

        //inicjacja suwaka, który będzie się znajdował w palecie szarości
        sf::RectangleShape slider;
        slider.setSize(sf::Vector2f(50, 3));
        slider.setFillColor(sf::Color::Black);

        //tekstura trzymająca suwak
        sf::Texture texture;
        texture.create(50, 255);
        texture.update(valuePicker);

        //sprite trzymający teksturę suwaka
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setPosition(670, 50);

        //ramka okalająca paletę szarości
        sf::RectangleShape border;
        border.setSize(sf::Vector2f(50, 255));
        border.setFillColor(sf::Color::Transparent);
        border.setOutlineColor(sf::Color::Black);
        border.setOutlineThickness(1.0f);
        border.setPosition(670, 50);

        //tekstura zawierająca koło HSV
        sf::Texture* HSVtxt = new sf::Texture;
        HSVtxt->create(200, 200);

        //tekstura zawierająca koło RGB
        sf::Texture* RGBtxt = new sf::Texture;
        RGBtxt->create(200, 200);

        //tekstura zawierająca koło CMY
        sf::Texture* CMYtxt = new sf::Texture;
        CMYtxt->create(200, 200);

        //tekstura zawierająca koło HSL
        sf::Texture* HSLtxt = new sf::Texture;
        HSLtxt->create(200, 200);

        //sprite zawierający koło HSV
        sf::Sprite HSVspr;
        HSVspr.setPosition(400, 70);

        //sprite zawierający koło RGB
        sf::Sprite RGBspr;
        RGBspr.setPosition(400, 380);

        //sprite zawierający koło CMY
        sf::Sprite CMYspr;
        CMYspr.setPosition(70, 380);

        //sprite zawierający koło HSL
        sf::Sprite HSLspr;
        HSLspr.setPosition(70, 70);
        
        sf::Event event;
        clock.restart().asMilliseconds();
        
        //pętla działania okna
        while (window.isOpen()){   
            window.clear(sf::Color::White);

            //pozycja slidera znajduje się w pętli window by była update'owana w zaleznie od eventu
            slider.setPosition(670, 50 + middle); 
            
            while (window.pollEvent(event)){
                //wyłączanie programu przez X
                if (event.type == sf::Event::Closed){ 
                    window.close();
                }   
                
                //zmiana wartości i pozycji wskaźnika na palecie szarości w zależności od miejsca kliknięcia w granicach
                if (event.type == sf::Event::MouseButtonReleased){
                    if ((event.mouseButton.x >= 670 && event.mouseButton.y >= 50) && (event.mouseButton.x <= 720 && event.mouseButton.y <= 305)){
                        middle =  event.mouseButton.y - 50;                     
                    }
                }

                //zmiana wartości i pozycji wskaźnika na palecie szarości poprzez gładkie przesuwanie 
                if ((sf::Mouse::isButtonPressed(sf::Mouse::Left)) || (sf::Mouse::isButtonPressed(sf::Mouse::Right))){
                    if (event.type == sf::Event::MouseMoved && (event.mouseMove.x >= 670 && event.mouseMove.y >= 50) && (event.mouseMove.x <= 720 && event.mouseMove.y <= 305)){
                        middle = event.mouseMove.y - 50;                       
                    }
                }
            }

            //tu wyrysować wszystko na ekran
            window.draw(sprite); //rysuje paletę
            window.draw(border); //rysuje ramkę
            window.draw(slider); //rysuje suwak

            //tu wypisać na ekran wartość FPS
            if (clock.getElapsedTime().asSeconds() >= 1.0f){
                FPS = (unsigned int)((float)frame_counter / clock.getElapsedTime().asSeconds()); // +5000; :DD
                clock.restart();
                frame_counter = 0;
            }

            //zamieniam FPS na string i dopisuję " FPS"
            frame_counter++;
            std::string temp = std::to_string(FPS);
            temp.append(" FPS");

            //wypisuje akutalne FPS
            writeText(text, window, 660, 320, temp);

            //wypisuje teksty i poszczególne wartości kół
            writeRGB(text, window, middle);
            writeHSV(text, window, middle);
            writeHSL(text, window, middle);
            writeCMY(text, window, middle);

            //funkcje kolorujace tekstury
            setHSV(HSVtxt, middle); 
            setRGB(RGBtxt, middle); 
            setCMY(CMYtxt, middle);
            setHSL(HSLtxt, middle);

            //wstawiamy tekstury na sprite'y
            HSVspr.setTexture(*HSVtxt);
            RGBspr.setTexture(*RGBtxt);
            CMYspr.setTexture(*CMYtxt);
            HSLspr.setTexture(*HSLtxt);

            //rysujemy sprite'y na ekranie
            window.draw(HSVspr);
            window.draw(RGBspr);
            window.draw(CMYspr);
            window.draw(HSLspr);
           
            window.display();
        }
    }
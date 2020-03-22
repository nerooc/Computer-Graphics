// Calosc mozna dowolnie edytowac wedle uznania. 

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "font.h"


//FUNKCJE

void setCurMode(sf::Text* currentMode, sf::Font& font) {
    currentMode->setFont(font);
    currentMode->setCharacterSize(12);
    currentMode->setFillColor(sf::Color::White);
}

void curMode(sf::RenderWindow& window, sf::Text* currentMode, char& keyPressed) {
    currentMode->setPosition(720, 615);
    currentMode->setString(keyPressed);
    window.draw(*currentMode);
}

void mouseClick(float& initX, float& initY, sf::RenderWindow& window, bool& flag) {
    initX = sf::Mouse::getPosition(window).x;
    initY = sf::Mouse::getPosition(window).y;
    if (sf::Mouse::getPosition(window).y < 566) {
        flag = true;
    }
}

void mouseMoved(float& initX, float& initY, sf::RenderWindow& window, bool& flag, int& maxX, int& maxY) {
    if (flag == true) {

        double mX = sf::Mouse::getPosition(window).x;
        double mY = sf::Mouse::getPosition(window).y;

        if (mX < 0 || mY < 0 || mX > maxX || mY > maxY){ //ograniczenie przeciwko wychodzeniu poza granice programu
        
            if (mX < 0)
                mX = 0;
            else if (mX > maxX)
                mX = maxX;

            if (mY < 0)
                mY = 0;
            else if (mY > maxY)
                mY = maxY;

            sf::Mouse::setPosition(sf::Vector2i(mX, mY), window);
        }
    }
}

void setFilledRectangle(sf::RectangleShape& rectangle, float& initX, float& initY, sf::Color& color, sf::Color& fillColor, sf::RenderWindow& window) {
    rectangle.setSize(sf::Vector2f(static_cast<float> (sf::Mouse::getPosition(window).x - initX), static_cast<float> (sf::Mouse::getPosition(window).y - initY)));
    rectangle.setFillColor(fillColor);
    rectangle.setOutlineColor(color);
    rectangle.setOutlineThickness(1.0f);
    rectangle.setPosition(initX, initY);
}


void setRectangle(sf::RectangleShape& rectangle, float& initX, float& initY, sf::Color& color, sf::Color& fillColor, sf::RenderWindow& window) {
    rectangle.setSize(sf::Vector2f(static_cast<float> (sf::Mouse::getPosition(window).x - initX), static_cast<float> (sf::Mouse::getPosition(window).y - initY)));
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(color);
    rectangle.setOutlineThickness(1.0f);
    rectangle.setPosition(initX, initY);
}


void setLine(sf::VertexArray& line, float& initX, float& initY, sf::Color& color, sf::Color& fillColor, sf::RenderWindow& window) {
    line[0].position = sf::Vector2f(initX, initY);
    line[0].color = color;
    line[1].position = sf::Vector2f((sf::Mouse::getPosition(window).x), (sf::Mouse::getPosition(window).y));
    line[1].color = fillColor;
}

void setCircle(sf::CircleShape& circle, float& initX, float& initY, sf::Color& color, sf::Color& fillColor, sf::RenderWindow& window){
    circle.setRadius(sqrt(static_cast<float> (pow(sf::Mouse::getPosition(window).x - initX, 2) + pow(sf::Mouse::getPosition(window).y - initY, 2))) / 2);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(color);
    circle.setOutlineThickness(1.0f);
    circle.setOrigin(sf::Vector2f(0, circle.getRadius()));
    circle.setPosition(sf::Vector2f(initX, (initY - circle.getRadius())));
    circle.move(0, circle.getRadius());
    circle.setRotation(0);
    circle.rotate((atan((sf::Mouse::getPosition(window).y - initY) / (sf::Mouse::getPosition(window).x - initX)) * 180 / 3.14));
    circle.setScale(1, 1);
    if (sf::Mouse::getPosition(window).x - initX < 0){
        circle.setScale(-1, 1);
    }
}


class Menu : public sf::Drawable
{
private:
    sf::Font font;
    sf::Text* text;
    sf::RectangleShape* rectangle;

    sf::Texture* colors_texture;
    sf::Sprite* colors_sprite;
    sf::Uint8* colors_pixels;
    const unsigned int colors_size_x = 765;
    const unsigned int colors_size_y = 60;
    inline void draw_to_color_pixels(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
    {
        colors_pixels[4 * (y * colors_size_x + x) + 0] = r;
        colors_pixels[4 * (y * colors_size_x + x) + 1] = g;
        colors_pixels[4 * (y * colors_size_x + x) + 2] = b;
        colors_pixels[4 * (y * colors_size_x + x) + 3] = 255;
    }

public:
    Menu()
    {
        font.loadFromMemory(font_data, font_data_size);
        text = new sf::Text;
        text->setFont(font);
        text->setCharacterSize(12);
        text->setFillColor(sf::Color::White);

        rectangle = new sf::RectangleShape(sf::Vector2f(796, 536));
        rectangle->setFillColor(sf::Color::Transparent);
        rectangle->setOutlineColor(sf::Color::White);
        rectangle->setOutlineThickness(1.0f);
        rectangle->setPosition(2, 62);

        unsigned int x, y;
        colors_pixels = new sf::Uint8[colors_size_x * colors_size_y * 4];
        for (x = 0; x < 255; x++)
            for (y = 0; y < 30; y++)
            {
                draw_to_color_pixels(x, y, x, 255, 0);
                draw_to_color_pixels(x + 255, y, 255, 255 - x, 0);
                draw_to_color_pixels(x + 510, y, 255, 0, x);
                draw_to_color_pixels(254 - x, y + 30, 0, 255, 255 - x);
                draw_to_color_pixels(509 - x, y + 30, 0, x, 255);
                draw_to_color_pixels(764 - x, y + 30, 255 - x, 0, 255);
            }

        colors_texture = new sf::Texture();
        colors_texture->create(colors_size_x, colors_size_y);
        colors_texture->update(colors_pixels);

        colors_sprite = new sf::Sprite();
        colors_sprite->setTexture(*colors_texture);
        colors_sprite->setPosition(1, 1);
    }

    void outtextxy(sf::RenderTarget& target, float x, float y, const wchar_t* str) const
    {
        text->setPosition(x, y);
        text->setString(str);
        target.draw(*text);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        outtextxy(target, 5, 600, L"f - wybór koloru rysowania");
        outtextxy(target, 5, 615, L"b - wybór koloru wypełniania");
        outtextxy(target, 5, 630, L"l - rysowanie linii");

        outtextxy(target, 200, 600, L"r - rysowanie prostokąta");
        outtextxy(target, 200, 615, L"a - rysowanie wypełnionego prostokąta");
        outtextxy(target, 200, 630, L"c - rysowanie okręgu");

        outtextxy(target, 470, 600, L"w - zapis do pliku");
        outtextxy(target, 470, 615, L"o - odczyt z pliku");
        outtextxy(target, 470, 630, L"esc - wyjście");

        outtextxy(target, 650, 615, L"Aktualne:");

        target.draw(*rectangle);
        target.draw(*colors_sprite);
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 650), "GFK Lab 01", sf::Style::Titlebar | sf::Style::Close);
    sf::Event event;
    Menu menu;

    bool flag = false; //flaga rysowania
    char keyPressed = ' '; //mówi o tym jaki tryb jest włączony
 
    sf::Texture texture;  //tekstura
    texture.create(800, 650);

    sf::Texture screenshot; //tekstura zapisująca aktualny obraz
    screenshot.create(800, 650);
    screenshot.update(window);

    sf::Sprite sprite;
    sprite.setTexture(texture);
 
    sf::Font font;//czcionka do tekstu o trybie

    sf::Text* currentMode; //tekst mówiący jaki tryb jest włączony

    font.loadFromMemory(font_data, font_data_size);
    currentMode = new sf::Text;
    setCurMode(currentMode, font);

    sf::Color color = sf::Color::Black; //wybrany kolor podstawowy
    sf::Color fillColor = sf::Color::Black; //wybrany kolor wypełnienia


    int maxX = 800; //zmienne ograniczające wyjazd myszką w trakcie rysowania
    int maxY = 595;

    
    sf::VertexArray line(sf::LinesStrip, 2); // kształty możliwe do narysowania
    sf::RectangleShape rectangle;
    sf::CircleShape circle;


    sf::RectangleShape colorChoice, fillColorChoice; //kwadraciki zapisujące wybrane kolory

    colorChoice.setPosition(sf::Vector2f(767, 1)); //ustawiam pozycję i wielkość pojemników na kolory
    colorChoice.setSize(sf::Vector2f(32, 29));  //zostawiam pixel przerwy by było widać granicę

    fillColorChoice.setPosition(sf::Vector2f(767, 31));
    fillColorChoice.setSize(sf::Vector2f(32, 29));



    float initX = 0;//deklaracja zmiennych używanych później jako początkowe wartości pozycji myszki
    float initY = 0;


    while (window.isOpen())
    {

        window.clear(sf::Color::Black);

        colorChoice.setFillColor(color); //wstawiam kolory do pojemników
        fillColorChoice.setFillColor(fillColor);

        while (window.pollEvent(event)) //obsługa zdarzeń
        {
            if (event.type == sf::Event::Closed) window.close(); //wyłączanie przez X
            
            //klawiatura
            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::F) {
                    keyPressed = 'f';
                }

                if (event.key.code == sf::Keyboard::B) {
                    keyPressed = 'b';
                }

                if (event.key.code == sf::Keyboard::L) {
                    keyPressed = 'l';
                }

                if (event.key.code == sf::Keyboard::R) {
                    keyPressed = 'r';
                }

                if (event.key.code == sf::Keyboard::A) {
                    keyPressed = 'a';
                }

                if (event.key.code == sf::Keyboard::C) {
                    keyPressed = 'c';
                }

                if (event.key.code == sf::Keyboard::W) { //zapis do pliku
                    keyPressed = 'w';
                    texture.copyToImage().saveToFile("obrazek.png");
                }

                if (event.key.code == sf::Keyboard::O) { //wczytanie z pliku
                    keyPressed = 'o';
                    texture.loadFromFile("obrazek.png");
                }

                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }

            
            //myszka
            if (event.type == sf::Event::MouseButtonPressed) { // przyciśnięcie przycisku
                mouseClick(initX, initY, window, flag);
            }

            if (event.type == sf::Event::MouseMoved) { //ruszanie myszką (w celu ograniczenia pola manewru w trakcie rysowania)
                mouseMoved(initX, initY, window, flag, maxX, maxY);
            }
            
            if (event.type == sf::Event::MouseButtonReleased) {
                int x = event.mouseButton.x;
                flag = false;

                texture.update(screenshot.copyToImage());

                if (keyPressed == 'f'){//wybiór koloru głównego
                    if (event.mouseButton.y <= 30){
                        if (x <= 255){
                            color = sf::Color(x, 255, 0, 255);
                        }
                        if (x > 255 && x <= 510){
                            color = sf::Color(255, 510 - x, 0, 255);
                        }
                        if (x > 510 && x <= 765)
                        {
                            color = sf::Color(255, 0, x - 510, 255);
                        }

                        keyPressed = ' ';
                        break;
                    }

                    if (event.mouseButton.y <= 60 && event.mouseButton.y > 30){
                        if (x <= 255){
                            color = sf::Color(0, 255, x, 255);
                        }
                        if (x > 255 && x <= 510){
                            color = sf::Color(0, 510 - x, 255, 255);
                        }
                        if (x > 510 && x <= 765){
                            color = sf::Color(x - 510, 0, 255, 255);
                        }

                        keyPressed = ' ';
                        break;
                    }
                }

                if (keyPressed == 'b') { //wybiór koloru wypełnienia
                    if (event.mouseButton.y <= 30){
                        if (x <= 255){
                            fillColor = sf::Color(x, 255, 0, 255);
                        }
                        if (x > 255 && x <= 510){
                            fillColor = sf::Color(255, 510 - x, 0, 255);
                        }
                        if (x > 510 && x <= 765){
                            fillColor = sf::Color(255, 0, x - 510, 255);
                        }

                        keyPressed = ' ';
                        break;
                    }

                    if (event.mouseButton.y <= 60 && event.mouseButton.y > 30){
                        if (x <= 255){
                            fillColor = sf::Color(0, 255, x, 255);
                        }
                        if (x > 255 && x <= 510){
                            fillColor = sf::Color(0, 510 - x, 255, 255);
                        }
                        if (x > 510 && x <= 765){
                            fillColor = sf::Color(x - 510, 0, 255, 255);
                        }

                        keyPressed = ' ';
                        break;
                    }
                }

            }
        }

        window.draw(sprite);

        //RYSOWANIE

        if (flag){

                switch (keyPressed){

                case 'l':
                    setLine(line, initX, initY, color, fillColor, window); //funkcja rysujaca linię
                    window.draw(line);
                    break;

                case 'r':
                    setRectangle(rectangle, initX, initY, color, fillColor, window); //funkcja rysujaca prostokat bez wypelnienia
                    window.draw(rectangle);
                    break;
                    
                case 'a':
                    setFilledRectangle(rectangle, initX, initY, color, fillColor, window); //funkcja rysujaca wypleniony prostokat
                    window.draw(rectangle); 
                    break;
                
                case 'c':
                    setCircle(circle, initX, initY, color, fillColor, window); //funkcja rysujaca kolo
                    window.draw(circle); 
                    break;
                }
        }
        

        screenshot.update(window);

        window.draw(colorChoice);//rysuje kwadraty pokazujace wybrane kolory
        window.draw(fillColorChoice);
        
        window.draw(menu);//rysuje menu

        curMode(window, currentMode, keyPressed);//wypisuje w jakim trybie aktualnie jesteśmy
        window.display();

        //Draw END
    }
    return 0;
}
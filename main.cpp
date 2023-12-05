#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;
using namespace sf;

const int WIDTH = 700;
const int HEIGHT = 500;


float Fx(float x, float y) {
    return 0;
}

float Fy(float x, float y) {
    return 0;
}

class Balls {
    struct Ball {
        float x, y, vx, vy;
    };
    const int r = 10;
    const int m = 4 * pow(r, 3);
    vector<Ball> bls;
public:
    Balls() : bls({}) {}

    void move() {
        for (auto& b : bls) {
            b.x += b.vx;
            b.y += b.vy;
            b.vx += Fx(b.x, b.y);
            b.vy += Fy(b.x, b.y);
        }
    }

    int getsize() {
        return bls.size();
    }

    void newball(float x0, float y0, float vx0, float vy0) {
        Ball new_ball{ x0, y0, vx0, vy0 };
        bls.push_back(new_ball);
    }
};

int main()
{
    RenderWindow window(VideoMode(WIDTH, HEIGHT), L"Project_2023", Style::Default);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    int time_of_pressing_mb = 0;
    float mouse_button_x0, mouse_button_y0;
    Balls balls;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            else {
                if (event.type == Event::MouseButtonPressed) {
                    if (time_of_pressing_mb == 0) {
                        mouse_button_x0 = event.mouseButton.x;
                        mouse_button_y0 = event.mouseButton.y;
                    }
                    time_of_pressing_mb += 1;
                }
                if (event.type == Event::MouseButtonReleased) {
                    float vx0 = (mouse_button_x0 - event.mouseButton.x) * 0.1;
                    float vy0 = (mouse_button_y0 - event.mouseButton.y) * 0.1;
                    balls.newball(mouse_button_x0, mouse_button_y0, vx0, vy0);
                    time_of_pressing_mb = 0;
                }
            }
        }
        balls.move();
        /*for (auto b : balls.bls) {
            CircleShape b;
            window.draw(b);
        }*/
        window.clear(Color::White);
        window.display();
    }

    return 0;
}
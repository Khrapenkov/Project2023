#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace sf;

const int WIDTH = 1200;
const int HEIGHT = 700;

class Centers {
    struct Center {
        int x, y;
        int r;
    };
    float G = 0.00005;
    float mu = 0.4;
    vector<Center> ctrs;

public:
    Centers() : ctrs({}) {}

    int getnomber() {
        return ctrs.size();
    }

    void newcenter(float x0, float y0, int r) {
        Center new_center{ x0, y0, r };
        ctrs.push_back(new_center);
    }

    vector<CircleShape> draw() {
        vector<CircleShape> circles = {};
        for (auto c : ctrs) {
            CircleShape circle(c.r, 25);
            circle.setFillColor(Color::Black);
            circle.setOrigin(Vector2f(-c.x + c.r, -c.y + c.r));
            circles.push_back(circle);
        }
        return circles;
    }

    float Fx(float x, float y, int r) {
        float fx = 0;
        int m = mu * pow(r, 3);
        for (auto c : ctrs) {
            float r2 = pow(x - c.x, 2) + pow(y - c.y, 2);
            int mi = mu * pow(c.r, 3);
            fx += G * m * mi * (c.x - x) / pow(r2, 3 / 2);
        }
        return fx;
    }

    float Fy(float x, float y, int r) {
        float fy = 0;
        int m = mu * pow(r, 3);
        for (auto c : ctrs) {
            float r2 = pow(x - c.x, 2) + pow(y - c.y, 2);
            int mi = mu * pow(c.r, 3);
            fy += G * m * mi * (c.y - y) / pow(r2, 3 / 2);
        }
        return fy;
    }

    bool ball_in_center(float x, float y) {
        bool ball_is_in_center = false;
        for (auto c : ctrs) {
            if (c.x == x or c.y == y)
                ball_is_in_center = true;
        }
        return ball_is_in_center;
    }

    void bad_center() {
        if (!ctrs.empty())
            ctrs.erase(ctrs.begin());
    }
};

class Balls : Centers {
    struct Ball {
        float x, y, vx, vy;
    };
    int r = 8;
    int m = 4 * pow(r, 3);
    vector<Ball> bls;
    
public:
    Balls() : bls({}) {}

    void move(Centers centers) {
        for (auto& b : bls) {
            b.x += b.vx;
            b.y += b.vy;
            b.vx += centers.Fx(b.x, b.y, r);
            b.vy += centers.Fy(b.x, b.y, r);
        }
    }

    int getnomber() {
        return bls.size();
    }

    void newball(float x0, float y0, float vx0, float vy0) {
        Ball new_ball{ x0, y0, vx0, vy0 };
        bls.push_back(new_ball);
    }

    vector<CircleShape> draw() {
        vector<CircleShape> circles = {};
        for (auto b : bls) {
            CircleShape circle(r, 25);
            circle.setFillColor(Color::Red);
            circle.setOrigin(Vector2f(-b.x + r, -b.y + r));
            circles.push_back(circle);
        }
        return circles;
    }

    void ball_is_out(Centers centers) {
        for (auto iter = bls.begin(); iter != bls.end();) {
            if ((*iter).x > WIDTH * 4 || (*iter).x < -3 * WIDTH || (*iter).y > HEIGHT * 4 || (*iter).y < -3 * HEIGHT)
                iter = bls.erase(iter);
            if (ball_in_center((*iter).x, (*iter).y))
                iter = bls.erase(iter);
            else
                ++iter;
        }
    }

    void bad_ball() {
        if (!bls.empty())
            bls.erase(bls.begin());
    }
};

VertexArray growing_line(int x, int y, int mouse_x, int mouse_y) {
    VertexArray line(LinesStrip, 2);
    line[0].position = Vector2f(x, y);
    line[0].color = Color::Black;
    line[1].position = Vector2f(mouse_x, mouse_y);
    line[1].color = Color::Red;
    return line;
}

CircleShape growing_spot(float x, float y, int r) {
    CircleShape spot(r, 100);
    spot.setFillColor(Color::Black);
    spot.setOrigin(Vector2f(-x + r, -y + r));
    return spot;
}

int main()
{
    RenderWindow window(VideoMode(WIDTH, HEIGHT), L"Project_2023", Style::Default);
    window.setVerticalSyncEnabled(true);
    // window.setKeyRepeatEnabled(false);
    int time_of_pressing_mb_r = 0;
    int mouse_button_l_x0 = 0, mouse_button_l_y0 = 0, mouse_button_r_x0 = 0, mouse_button_r_y0 = 0;
    bool mouse_left_button_is_pressed = false;
    bool mouse_right_button_is_pressed = false;
    Balls balls;
    Centers centers;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            else {
                if (event.type == Event::MouseButtonPressed) {
                    if (event.mouseButton.button == Mouse::Left) {
                        if (mouse_left_button_is_pressed == false) {
                            mouse_button_l_x0 = event.mouseButton.x;
                            mouse_button_l_y0 = event.mouseButton.y;
                        }
                        mouse_left_button_is_pressed = true;
                    }
                    else if (event.mouseButton.button == Mouse::Right) {
                        if (mouse_right_button_is_pressed == false) {
                            mouse_button_r_x0 = event.mouseButton.x;
                            mouse_button_r_y0 = event.mouseButton.y;
                        }
                        mouse_right_button_is_pressed = true;
                        time_of_pressing_mb_r += 1;
                    }
                }
                else if (event.type == Event::MouseButtonReleased) {
                    if (event.mouseButton.button == Mouse::Left) {
                        float vx0 = (mouse_button_l_x0 - event.mouseButton.x) * 0.1;
                        float vy0 = (mouse_button_l_y0 - event.mouseButton.y) * 0.1;
                        balls.newball(mouse_button_l_x0, mouse_button_l_y0, vx0, vy0);
                        mouse_left_button_is_pressed = false;
                    }
                    else if (event.mouseButton.button == Mouse::Right) {
                        centers.newcenter(mouse_button_r_x0, mouse_button_r_y0, time_of_pressing_mb_r);
                        mouse_right_button_is_pressed = false;
                        time_of_pressing_mb_r = 0;
                    }
                }
                else if (event.type == Event::KeyPressed) {
                    if (event.key.code = Keyboard::B)
                        balls.bad_ball();
                    else if (event.key.code = Keyboard::C)
                        centers.bad_center();
                }
            }
        }
        if (mouse_right_button_is_pressed)
            time_of_pressing_mb_r += 1;
        balls.move(centers);
        balls.ball_is_out(centers);
        window.clear(Color::White);

        if (mouse_left_button_is_pressed)
            window.draw(growing_line(mouse_button_l_x0, mouse_button_l_y0,
                Mouse::getPosition(window).x, Mouse::getPosition(window).y));
        if (mouse_right_button_is_pressed)
            window.draw(growing_spot(mouse_button_r_x0, mouse_button_r_y0, time_of_pressing_mb_r));
        for (auto center : centers.draw())
            window.draw(center);
        for (auto ball : balls.draw())
            window.draw(ball);
        window.display();
    }

    return 0;
}


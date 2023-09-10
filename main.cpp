#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <array>
#include <list>
#include <random>

class Block : public sf::RectangleShape
{
public:
    enum values
    {
        empty,
        snake,
        fruit
    };
    static void set_size(int s) { size = s; }

    Block()
    {
        setSize(sf::Vector2f(size, size));
        set_value(empty);
    }
    void set_pos(int x, int y)
    {
        this->x = x;
        this->y = y;
        setPosition(sf::Vector2f(x * size, y * size));
    }
    void set_value(values value)
    {
        this->value = value;
        switch(value)
        {
        case empty:
            setFillColor(sf::Color::Black);
            break;
        case snake:
            setFillColor(sf::Color::Green);
            break;
        case fruit:
            setFillColor(sf::Color::Red);
        }
    }
    values Value() { return value; }
    int X() { return x; }
    int Y() { return y; }

private:
    inline static int size = 10;
    int x, y;
    values value = empty;
};

int main()
{
    const int X = 20, Y = 20;
    int width = 1000, height = 1000;
    if(height < width)
        Block::set_size(height / Y);
    else
        Block::set_size(width / X);

    std::array<std::array<Block, X>, Y> map;
    for(int y = 0; y < Y; y++)
        for(int x = 0; x < X; x++)
            map[y][x].set_pos(x, y);

    map[1][1].set_value(Block::values::fruit);
    std::list<Block *> cells;
    for(int i = 7; i > 3; i--)
        cells.push_back(&map[Y / 2][i]);
    for(auto &cell : cells)
        cell->set_value(Block::values::snake);

    sf::Keyboard::Key direction = sf::Keyboard::Right;

    sf::RenderWindow window(sf::VideoMode(width, height), "window");
    sf::Clock clock;
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::Resized)
            {
                width = window.getSize().x;
                height = window.getSize().y;
            }
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction != sf::Keyboard::Left)
            direction = sf::Keyboard::Right;
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction != sf::Keyboard::Right)
            direction = sf::Keyboard::Left;
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction != sf::Keyboard::Down)
            direction = sf::Keyboard::Up;
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction != sf::Keyboard::Up)
            direction = sf::Keyboard::Down;

        if (clock.getElapsedTime() > sf::milliseconds(250))
        {
            Block *front = cells.front(), *next;
            switch(direction)
            {
            case sf::Keyboard::Right:
                next = &map[front->Y()][front->X() + 1];
                if(front->X() == X - 1 || next->Value() == Block::values::snake)
                    return 100;
                cells.push_front(next);
                break;
            case sf::Keyboard::Left:
                next = &map[front->Y()][front->X() - 1];
                if(front->X() == 0 || next->Value() == Block::values::snake)
                    return 100;
                cells.push_front(next);
                break;
            case sf::Keyboard::Up:
                next = &map[front->Y() - 1][front->X()];
                if(front->Y() == 0 || next->Value() == Block::values::snake)
                    return 100;
                cells.push_front(next);
                break;
            case sf::Keyboard::Down:
                next = &map[front->Y() + 1][front->X()];
                if(front->Y() == Y - 1 || next->Value() == Block::values::snake)
                    return 100;
                cells.push_front(next);
                break;
            }
            if(next->Value() == Block::values::fruit)
            {
                std::vector<Block *> free;
                for(auto &y : map)
                    for(auto &block : y)
                        if(block.Value() == Block::values::empty)
                            free.push_back(&block);
                free[rand() % free.size()]->set_value(Block::values::fruit);
            }
            else
            {
                cells.back()->set_value(Block::values::empty);
                cells.pop_back();
            }
            cells.front()->set_value(Block::values::snake);

            clock.restart();
        }
        
        for(auto &y : map)
            for(auto &block : y)
                window.draw(block);
        window.display();
    }

    return 0;
}
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

using namespace std;

const float CELL_SIZE = 24.f;   // розмір однієї клітини (в пікселях)
const float DELAY = 0.2f;       // інтервал між ігровими тіками (в секундах)

class Cells {
private:
    int Width;
    int Height;
    bool** CellsMatrix;

    // якщо клітина жива, повертається одиниця; інакше - нуль

    int GetCellStateForCount(int i, int j) const {
        return GetCell(i, j) ? 1 : 0;
    }
public:
    Cells(int width, int height) {
        Width = width;
        Height = height;

        CellsMatrix = new bool* [Height];

        for (int i = 0; i < Height; i++) {
            CellsMatrix[i] = new bool[Width];
            for (int j = 0; j < Width; j++) {
                CellsMatrix[i][j] = false;
            }
        }
    }

    Cells(const Cells& original) {
        Width = original.Width;
        Height = original.Height;

        CellsMatrix = new bool* [Height];

        for (int i = 0; i < Height; i++) {
            CellsMatrix[i] = new bool[Width];
            for (int j = 0; j < Width; j++) {
                CellsMatrix[i][j] = original.CellsMatrix[i][j];
            }
        }
    }

    // оператор присвоєння, що потім дозволяє реалізувати одночасне оновлення всіх клітин в матриці

    Cells& operator=(const Cells& original) {
        for (int i = 0; i < Height; i++) {
            for (int j = 0; j < Width; j++) {
                CellsMatrix[i][j] = original.CellsMatrix[i][j];
            }
        }

        return *this;
    }

    // отримати стан клітини з перевіркою діапазону

    bool GetCell(int i, int j) const {
        if (i >= 0 && i < Height && j >= 0 && j < Width) {
            return CellsMatrix[i][j];
        }

        return false;
    }

    // змінити стан клітини з перевіркою діапазону

    void SetCell(int i, int j, bool value) {
        if (i >= 0 && i < Height && j >= 0 && j < Width) {
            CellsMatrix[i][j] = value;
        }
    }

    int GetNumberOfNeighbors(int i, int j) {
        return
            GetCellStateForCount(i - 1, j) + GetCellStateForCount(i + 1, j)
            + GetCellStateForCount(i, j - 1) + GetCellStateForCount(i, j + 1)
            + GetCellStateForCount(i - 1, j - 1) + GetCellStateForCount(i - 1, j + 1)
            + GetCellStateForCount(i + 1, j - 1) + GetCellStateForCount(i + 1, j + 1);
    }

    ~Cells() {
        for (int i = 0; i < Height; i++) {
            delete[] CellsMatrix[i];
            CellsMatrix[i] = nullptr;
        }

        delete[] CellsMatrix;
        CellsMatrix = nullptr;
    }
};

int main()
{
    int width = 0;
    int height = 0;

    cout << "Enter window width (number of cells) = ";
    cin >> width;

    cout << "Enter window height (number of cells) = ";
    cin >> height;

    Cells cells(width, height);

    cout << "Enter cell coordinates (like \"2,3|6,4|12,11\"\n";

    string input;
    cin >> input;
    istringstream stream(input);
    string token;

    while (getline(stream, token, '|')) {
        try {
            int x = stoi(token.substr(0, token.find_first_of(','))) - 1;
            int y = stoi(token.substr(token.find_first_of(',') + 1)) - 1;

            cells.SetCell(y, x, true);
        }
        catch (...) {
            cout << "Invalid input!\n";
            return 0;
        }
    }

    sf::RenderWindow window(
        sf::VideoMode(
            {
                static_cast<unsigned int>(CELL_SIZE * width),
                static_cast<unsigned int>(CELL_SIZE * height)
            }
        ),
        "Game of life",
        sf::Style::Titlebar | sf::Style::Close
    );
    sf::Clock clock;

    sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(225, 225, 225));

        // рендер поточного стану клітин

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (cells.GetCell(i, j)) {
                    cellShape.setOrigin({ 0.f, 0.f });
                    cellShape.setPosition({ j * CELL_SIZE, i * CELL_SIZE });
                    cellShape.setFillColor(sf::Color::Black);

                    window.draw(cellShape);
                }
            }
        }

        // оновлення стану клітин

        if (clock.getElapsedTime().asSeconds() >= DELAY) {
            Cells tempCells(cells);

            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    int numOfNeighbors = cells.GetNumberOfNeighbors(i, j);

                    if (cells.GetCell(i, j) && (numOfNeighbors < 2 || numOfNeighbors > 3)) {
                        tempCells.SetCell(i, j, false);
                    }
                    else if (!cells.GetCell(i, j) && numOfNeighbors == 3) {
                        tempCells.SetCell(i, j, true);
                    }
                }
            }

            cells = tempCells;

            clock.restart();
        }

        window.display();
    }

    return 0;
}

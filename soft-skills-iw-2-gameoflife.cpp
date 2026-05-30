#include <iostream>
using namespace std;

class Cells {
private:
    int Width;
    int Height;
    bool** CellsMatrix;

    // якщо клітина жива, повертається одиниця; інакше - нуль

    int GetCellStateForCount(int i, int j) const {
        bool state = GetCell(i, j);
        return state ? 1 : 0;
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
    std::cout << "Hello World!\n";
}

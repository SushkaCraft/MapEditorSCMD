#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <windows.h> // Для использования функций Windows API

using namespace std;

const int HEIGHT = 32;
const int WIDTH = 64;
vector<vector<char>> map(HEIGHT, vector<char>(WIDTH, ' '));
int cursorX = 1, cursorY = 1;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // Получаем хэндл стандартного вывода

void resetCursorPosition() {
    COORD topLeft = {0, 0};
    SetConsoleCursorPosition(console, topLeft);
}

void displayMap() {
    resetCursorPosition(); // Обновляем консоль без мигания
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (i == cursorY && j == cursorX) cout << 'A';
            else cout << map[i][j];
        }
        cout << endl;
    }
}

void initializeMap() {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (i == 0 || i == HEIGHT - 1) map[i][j] = '-';
            else if (j == 0 || j == WIDTH - 1) map[i][j] = '|';
        }
    }
}

void moveCursor(int dx, int dy) {
    int newX = cursorX + dx;
    int newY = cursorY + dy;
    if (newX > 0 && newX < WIDTH - 1 && newY > 0 && newY < HEIGHT - 1) {
        cursorX = newX;
        cursorY = newY;
    }
}

void placeItem(char item) {
    map[cursorY][cursorX] = item;
}

void cleanUpMapFile(const string& filename) {
    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf(); // Считываем файл в буфер
    file.close();

    ofstream outFile(filename);
    string line;
    while (getline(buffer, line)) {
        if (line.find('-') == string::npos && line.find('|') == string::npos) {
            outFile << line << endl;
        }
    }
    outFile.close();
}

void saveMap(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                if (map[i][j] != ' ') {
                    file << "map[" << i << "][" << j << "] = '" << map[i][j] << "';\n";
                }
            }
        }
        file.close();
        cleanUpMapFile(filename); // Очищаем файл от нежелательных строк
        cout << "Map saved and cleaned up in " << filename << endl;
    } else {
        cout << "Unable to open file for writing." << endl;
    }
}

int main() {
	SetConsoleTitle("Shooter - Control Mission Deployment - Map Editor");
	system("chcp 65001");
	system("cls");
    initializeMap();
    displayMap();

    char input;
    do {
        input = _getch();
        switch (input) {
            case 'w': moveCursor(0, -1); break;
            case 's': moveCursor(0, 1); break;
            case 'a': moveCursor(-1, 0); break;
            case 'd': moveCursor(1, 0); break;
            case '1': placeItem('#'); break; // Стена
            case '2': placeItem('+'); break; // Аптечка
            case '3': placeItem('%'); break; // Ловушка
            case '4': placeItem(' '); break; // Пустое поле
        }
        displayMap();
    } while (input != 'x');

    saveMap("map.txt");

    return 0;
}

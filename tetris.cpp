#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>

using namespace std;

int dropSpeed = 1000, maxx = 0;
const int WIDTH = 15;
const int HEIGHT = 20;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorPos = {0, 0};

// Tetromino class with rotation states
struct Tetromino {
    vector<vector<vector<int>>> rotations;
    string emoji;
    int rotationState;

    Tetromino(vector<vector<vector<int>>> r, string e) : rotations(r), emoji(e), rotationState(0) {}

    void rotate() {
        rotationState = (rotationState + 1) % rotations.size();
    }

    vector<vector<int>> getShape() {
        return rotations[rotationState];
    }
};

// Tetromino Definitions
vector<Tetromino> tetrominoes = {
    {{{{1}, {1}, {1}, {1}}, {{1, 1, 1, 1}}}, "🟦"},
    {{{{1, 1}, {1, 1}}}, "🟨"},
    {{{{1, 1, 1}, {0, 1, 0}}, {{0, 1}, {1, 1}, {0, 1}}, {{0, 1, 0}, {1, 1, 1}}, {{1, 0}, {1, 1}, {1, 0}}}, "🟪"},
    {{{{0, 1, 1}, {1, 1, 0}}, {{1, 0}, {1, 1}, {0, 1}}}, "🟩"},
    {{{{1, 1, 0}, {0, 1, 1}}, {{0, 1}, {1, 1}, {1, 0}}}, "🟥"},
    {{{{0, 1}, {0, 1}, {1, 1}}, {{1, 0, 0}, {1, 1, 1}}, {{1, 1}, {1, 0}, {1, 0}}, {{1, 1, 1}, {0, 0, 1}}}, "🟫"},
    {{{{1, 0}, {1, 0}, {1, 1}}, {{1, 1, 1}, {1, 0, 0}}, {{1, 1}, {0, 1}, {0, 1}}, {{0, 0, 1}, {1, 1, 1}}}, "🟧"}
};

vector<vector<string>> board(HEIGHT, vector<string>(WIDTH, "⬜"));
Tetromino currentPiece = tetrominoes[0];
int pieceX = WIDTH / 2 - 1;
int pieceY = 0;
int score = 0;

void moveCursor(int x, int y) {
    cursorPos.X = x;
    cursorPos.Y = y;
    SetConsoleCursorPosition(hConsole, cursorPos);
}

void displayBoard() {
    moveCursor(0, 3);
    ostringstream output;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == "⬜" && pieceY <= i && i < pieceY + currentPiece.getShape().size() &&
                pieceX <= j && j < pieceX + currentPiece.getShape()[0].size() &&
                currentPiece.getShape()[i - pieceY][j - pieceX] == 1) {
                output << currentPiece.emoji;
            } else {
                output << board[i][j];
            }
        }
        output << "\n";
    }
    output << "Score: " << score << endl;
    cout << output.str();
}

bool isValidMove(Tetromino &piece, int x, int y) {
    vector<vector<int>> shape = piece.getShape();
    for (int i = 0; i < shape.size(); i++) {
        for (int j = 0; j < shape[i].size(); j++) {
            if (shape[i][j] == 1) {
                int newX = x + j, newY = y + i;
                if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || board[newY][newX] != "⬜") {
                    return false;
                }
            }
        }
    }
    return true;
}

void placePiece() {
    vector<vector<int>> shape = currentPiece.getShape();
    for (int i = 0; i < shape.size(); i++) {
        for (int j = 0; j < shape[i].size(); j++) {
            if (shape[i][j] == 1) {
                board[pieceY + i][pieceX + j] = currentPiece.emoji;
            }
        }
    }
}

void clearLines() {
    for (int i = HEIGHT - 1; i >= 0; i--) {
        if (count(board[i].begin(), board[i].end(), "⬜") == 0) {
            board.erase(board.begin() + i);
            board.insert(board.begin(), vector<string>(WIDTH, "⬜"));
            score += 100;
            dropSpeed = max(100, dropSpeed - 100);
        }
    }
}

void spawnPiece() {
    currentPiece = tetrominoes[rand() % 7];
    pieceX = WIDTH / 2 - 1;
    pieceY = 0;
    if (!isValidMove(currentPiece, pieceX, pieceY)) {
        maxx = max(maxx, score);
        cout << "\nGame Over! Final Score: " << score << endl;
        cout << "Maximum Score: " << maxx << endl;
        cout << "Press ENTER to play again or ESC to exit..." << endl;
        while (true) {
            if (_kbhit()) {
                char key = _getch();
                if (key == 13) {
                    score = 0;
                    dropSpeed = 1000;
                    board.assign(HEIGHT, vector<string>(WIDTH, "⬜"));
                    spawnPiece();
                    system("cls");
                    return;
                }
                if (key == 27) {
                    dropSpeed = 1000;
                    maxx = 0;
                    cout << R"(
                        ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗
                       ██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔══ ██╗██║   ██║██╔════╝██╔══██╗
                       ██║  ███╗███████║██╔████╔██║█████╗      ██║   ██ ██║   ██║█████╗  ██████╔╝
                       ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║██║   ██║██╔══╝  ██╔══██╗
                       ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗     ██████╔╝╚ ████╔╝ ███████╗██║  ██║  
                        ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚═════╝  ╚═════╝ ╚══════╝╚═╝  ╚═╝   
                         )" << std::endl;
                    exit(0);
                }
            }
        }
    }
}


void handleInput() {
    if (_kbhit()) {
        char key = _getch();
        int newX = pieceX, newY = pieceY;
        Tetromino newPiece = currentPiece;

        if (key == 75 && isValidMove(currentPiece, pieceX - 1, pieceY)) newX--;
        if (key == 77 && isValidMove(currentPiece, pieceX + 1, pieceY)) newX++;
        if (key == 80 && isValidMove(currentPiece, pieceX, pieceY + 1)) newY++;
        if (key == 72) {
            newPiece.rotate();
            if (isValidMove(newPiece, pieceX, pieceY)) currentPiece = newPiece;
        }
        if (key == 32) while (isValidMove(currentPiece, pieceX, newY + 1)) newY++;

        pieceX = newX;
        pieceY = newY;
    }
}

void gameLoop() {
    DWORD lastFall = GetTickCount();
    const int inputFrameDelay = 10;  // Check input every 10ms

    while (true) {
        handleInput();

        DWORD now = GetTickCount();
        if (now - lastFall >= (DWORD)dropSpeed) {
            if (isValidMove(currentPiece, pieceX, pieceY + 1)) {
                pieceY++;
            } else {
                placePiece();
                clearLines();
                spawnPiece();
            }
            lastFall = now;
        }

        displayBoard();
        Sleep(inputFrameDelay);  // Smooth input
    }
}

int main() {
    srand(time(0));
    SetConsoleOutputCP(CP_UTF8);

    
    // Hide console cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    spawnPiece();
    gameLoop();
    return 0;
}

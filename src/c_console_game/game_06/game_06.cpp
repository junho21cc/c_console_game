#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <windows.h>

#define CHAR_185    '+'
#define CHAR_186    '|'
#define CHAR_187    '+'
#define CHAR_188    '+'
#define CHAR_176    '@'
#define CHAR_200    '+'
#define CHAR_201    '+'
#define CHAR_204    '+'
#define CHAR_205    '-'


#define gotoXY(X,Y) SetConsoleCursorPosition(console, {short(Y), short(X)})

DWORD cNumRead, newConsoleMode, fdwSaveOldMode;
HANDLE hStdin, console;
INPUT_RECORD irInBuf;
COORD oldConsoleBufferSize;

CONSOLE_SCREEN_BUFFER_INFO csbi;
int fX, fY;

void ConsoleBufferSize(short int width, short int height) {
    GetConsoleScreenBufferInfo(console, &csbi);

    if (width > csbi.srWindow.Right) {
        (height > csbi.srWindow.Bottom) ?
            SetConsoleScreenBufferSize(console, { SHORT(width), SHORT(height) }) :
            SetConsoleScreenBufferSize(console, { SHORT(width), SHORT(csbi.srWindow.Bottom + 1) });
    }
    else if (height > csbi.srWindow.Bottom)
        SetConsoleScreenBufferSize(console, { SHORT(csbi.srWindow.Right + 1), SHORT(height) });
    else
        SetConsoleScreenBufferSize(console, { SHORT(csbi.srWindow.Right + 1), SHORT(csbi.srWindow.Bottom + 1) });
}

void ShowConsoleCursor(bool showFlag) {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(console, &cursorInfo);
}

void PrintTextDelay(int delay, int nChar, const char* s) {
    for (int i(0); i < nChar; ++i) {
        Sleep(delay);
        putchar(s[i]);
    }
}

class Board {
    struct Cell { int value = 0, shown = false, flaged = false; } **mat;
    int height, width, bombs, opened, cX, cY;
    bool init = false;
public:
    Board(int h, int w, int m) : height(h), width(w), bombs(m), opened(0) {
        if (height <= 0) height = 1;
        if (width < 8) width = 8;
        if (bombs < 0) bombs = 0;
        if (bombs >= height * width) bombs = height * width - 1;

        /// Set console buffer size for optimization
        ConsoleBufferSize(width * 3 + 5, height + 10);

        /// initialize board matrix
        mat = new Cell * [height];
        for (int i(0); i < height; ++i) {
            mat[i] = new Cell[width];
            for (int j(0); j < width; ++j)
                mat[i][j].value = 0;
        }
    }
    ~Board() {
        SetConsoleScreenBufferSize(console, { SHORT(width * 3 + 5), SHORT(height + 12) });
        SetConsoleMode(hStdin, fdwSaveOldMode);
        for (int i(0); i < height; ++i) delete[] mat[i];
        delete[] mat;
    }
    bool Init(int posX, int posY) {
        init = true;

        /// set bomb locations
        srand(time(NULL));

        if (bombs == 0) {
            for (posX = 0; posX < height; ++posX)
                for (posY = 0; posY < width; ++posY) {
                    mat[posX][posY].value = 0;
                    mat[posX][posY].shown = true;
                }
            opened = height * width;
            PrintScore();
            Win();
            return true;
        }
        else if (bombs == height * width - 1) {
            for (int i(-1); i < 2; ++i)
                for (int j(-1); j < 2; ++j)
                    if ((i != 0 || j != 0) && posX + i >= 0 && posX + i < height && posY + j >= 0 && posY + j < width)
                        mat[posX][posY].value++;
            mat[posX][posY].shown = true;
            opened = 1;
            PrintScore();
            Win();
            return true;
        }
        else if (bombs == height * width - 2) {
            for (int j(0); j < height; ++j)
                for (int k(0); k < width; ++k)
                    if (j != posX || k != posY)
                        mat[j][k].value = -1;
            mat[posX][posY].value = 0;
            /// Generate 1 random point, not equal to (posX,posY), in valid range
            int x(rand() % height), y(rand() % width);
            while (posX == x && posY == y) {
                x = rand() % height;
                y = rand() % width;
            }
            mat[x][y].value = 0;
        }
        else {
            int x, y;
            for (int i(0); i < bombs; ++i) {
                x = rand() % height;
                if (height == 1) {
                    if (posY < 2)
                        y = rand() % (width - posY) + posY;
                    else if (posY > width - 3)
                        y = rand() % (posY - 1);
                    else
                        y = (rand() % 2) ? (rand() % (posY - 1)) : (posY + 2 + rand() % (width - posY - 2));
                }
                else if (x > posX - 2 && x < posX + 2) {
                    if (posY < 2)
                        y = posY + 2 + rand() % (width - posY - 2);
                    else if (posY > width - 3)
                        y = rand() % (posY - 1);
                    else
                        y = (rand() % 2 == 0) ? (rand() % (posY - 1)) : (posY + 2 + rand() % (width - posY - 2));
                }
                else
                    y = rand() % width;

                (mat[x][y].value == 0) ? --mat[x][y].value : --i;
            }
        }

        /// set values of non bomb cells
        for (int i(0); i < height; ++i) {
            for (int j(0); j < width; ++j) {
                if (mat[i][j].value == -1) continue;
                for (int x(-1); x < 2; ++x) {
                    if (i + x < 0 || i + x >= height) continue;
                    for (int y(-1); y < 2; ++y)
                        if (j + y >= 0 && j + y < width && mat[i + x][j + y].value == -1)
                            ++mat[i][j].value;
                }
            }
        }
        return false;
    }
    char Open(int posX, int posY) {
        /// Translate posX and posY from screen to matrix coordinates
        posX -= 3 + cX;
        posY = (posY - 1 - cY) / 3;
        if (posX < 0 || posX >= height || posY < 0 || posY >= width) return 'C';
        /// On first press initialize mine locations and cell values
        if (!init && Init(posX, posY))
            return 'W';

        if (mat[posX][posY].flaged || mat[posX][posY].shown) return 'C';
        mat[posX][posY].shown = true;
        ++opened;
        if (mat[posX][posY].value == 0) OpenEmpty(posX, posY);
        PrintDiff(posX, posY);

        /// On mine hit show all bombs
        if (mat[posX][posY].value == -1) {
            for (int i(0); i < height; ++i)
                for (int j(0); j < width; ++j)
                    if (mat[i][j].value == -1) {
                        mat[i][j].shown = true;
                        if (i == posX && j == posY) {
                            gotoXY(posX + 3 + cX, (posY + 1) * 3 - 2 + cY);
                            SetConsoleTextAttribute(console, (0 << 4) + 12);
                            puts("[*]");
                            SetConsoleTextAttribute(console, (0 << 4) + 7);
                        }
                        else PrintDiff(i, j);
                    }
            gotoXY(5 + height, 0);
            fX = posX;
            fY = posY;
            return 'L';
        }
        PrintScore();

        /// On win flag all bombs
        if (height * width - bombs == opened) {
            Win();
            return 'W';
        }
        return 'C';
    }
    void Win() {
        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j) {
                if (!mat[i][j].shown && !mat[i][j].flaged) {
                    mat[i][j].flaged = true;
                    PrintDiff(i, j);
                }
                else PrintDiff(i, j);
            }
    }
    void OpenEmpty(int x, int y) {
        /// Open all cells around empty cell and move to next empty cell
        for (int i(-1); i < 2; ++i) {
            if (x + i < 0 || x + i >= height) continue;
            for (int j(-1); j < 2; ++j) {
                if (y + j < 0 || y + j >= width) continue;
                if ((i != 0 || j != 0) && !mat[x + i][y + j].shown && mat[x + i][y + j].value == 0)
                    OpenEmpty(x + i, y + j);
                if (!mat[x + i][y + j].shown) ++opened;
                mat[x + i][y + j].shown = true;
                PrintDiff(x + i, y + j);
            }
        }
    }
    void Flag(int posX, int posY) {
        posX -= 3 + cX;
        posY = (posY - 1 - cY) / 3;
        if (posX < 0 || posX >= height || posY < 0 || posY >= width) return;
        if (!mat[posX][posY].shown)
            mat[posX][posY].flaged = !mat[posX][posY].flaged;
        PrintDiff(posX, posY);
    }
    void Print() {
        system("cls");

        GetConsoleScreenBufferInfo(console, &csbi);
        cX = csbi.srWindow.Bottom / 2 - (height + 4) / 2;
        cY = csbi.srWindow.Right / 2 - (width * 3 + 2) / 2;

        char* game(new char[3 * width + 3]);
        game[3 * width + 2] = '\0';
        int n(0);

        /// Top of frame
        game[n++] = char(CHAR_201);
        for (int i(0); i < width * 3; ++i)
            game[n++] = char(CHAR_205);
        game[n++] = char(CHAR_187);
        gotoXY(cX, cY);
        puts(game);
        n = 0;

        /// Score
        game[n++] = char(CHAR_186);
        for (int i(0); i < 8; ++i)
            game[n++] = " Opened:"[i];
        for (int i(0); i < width * 3 - 8; ++i)
            game[n++] = ' ';
        game[n++] = char(CHAR_186);
        gotoXY(cX + 1, cY);
        puts(game);
        PrintScore();
        n = 0;

        /// Border
        game[n++] = char(CHAR_204);
        for (int i(0); i < width * 3; i++)
            game[n++] = char(CHAR_205);
        game[n++] = char(CHAR_185);
        gotoXY(cX + 2, cY);
        puts(game);
        n = 0;

        /// Game
        for (int i(0); i < height; ++i) {
            gotoXY(cX + 3 + i, cY);
            putchar(CHAR_186);
            for (int j(0); j < width; ++j) {
                if (i == fX && j == fY && mat[i][j].shown && mat[i][j].value == -1) {
                    SetConsoleTextAttribute(console, (0 << 4) + 12);
                    printf("[*]");
                    SetConsoleTextAttribute(console, (0 << 4) + 7);
                    continue;
                }
                putchar('[');

                if (mat[i][j].shown) {
                    if (mat[i][j].value == -1) {
                        SetConsoleTextAttribute(console, (0 << 4) + 12);
                        putchar('*');
                        SetConsoleTextAttribute(console, (0 << 4) + 7);
                    }
                    else if (mat[i][j].value == 0) putchar(' ');
                    else putchar(mat[i][j].value + 48);
                }
                else if (mat[i][j].flaged) {
                    SetConsoleTextAttribute(console, (0 << 4) + 14);
                    putchar('#');
                    SetConsoleTextAttribute(console, (0 << 4) + 7);
                }
                else putchar(CHAR_176);

                putchar(']');
            }
            putchar(CHAR_186);
        }

        /// Bottom of frame
        game[n++] = char(CHAR_200);
        for (int i(0); i < width * 3; i++)
            game[n++] = char(CHAR_205);
        game[n++] = char(CHAR_188);
        gotoXY(cX + height + 3, cY);
        puts(game);

        delete[] game;
    }
    void PrintDiff(int posX, int posY) {
        gotoXY(posX + 3 + cX, 3 * posY + 2 + cY);
        if (mat[posX][posY].shown) {
            if (mat[posX][posY].value == 0) putchar(' ');
            else if (mat[posX][posY].value == -1) {
                SetConsoleTextAttribute(console, (0 << 4) + 12);
                putchar('*');
                SetConsoleTextAttribute(console, (0 << 4) + 7);
            }
            else putchar(mat[posX][posY].value + 48);
        }
        else {
            if (mat[posX][posY].flaged) {
                SetConsoleTextAttribute(console, (0 << 4) + 14);
                putchar('#');
                SetConsoleTextAttribute(console, (0 << 4) + 7);
            }
            else putchar(CHAR_176);
        }
    }
    void PrintScore() {
        gotoXY(cX + 1, cY + 10);
        printf("%d/%d", opened, height * width - bombs);
    }
};

void PrintMenu(int x, int y, int n) {
    const char* menuStr[] = { "              Height  Width  Bombs   ",
                             "   Easy       9       9      10      ",
                             "   Medium     16      16     40      ",
                             "   Hard       16      30     99      ",
                             "   Custom                            ",
                             "   Quit                              " };



    /// Top of frame
    gotoXY(x, y);
    putchar(CHAR_201);
    for (int i(0); i < 37; ++i) putchar(CHAR_205);
    putchar(CHAR_187);

    /// Menu
    for (int i(0); i < 6; ++i) {
        gotoXY(x + i + 1, y);
        printf("%c%s%c\n", CHAR_186, menuStr[i], CHAR_186);
    }

    /// Bottom of frame
    gotoXY(x + 7, y);
    putchar(CHAR_200);
    for (int i(0); i < 37; ++i) putchar(CHAR_205);
    putchar(CHAR_188);

    gotoXY(x + n + 2, y + 2);
    putchar('>');
}

int Menu() {
    int x, y, oldx(-1), oldy(-1), n(0);

    SetConsoleMode(hStdin, newConsoleMode);

    INPUT_RECORD irInBuf;
    for (;;) {
        GetConsoleScreenBufferInfo(console, &csbi);
        x = csbi.srWindow.Bottom / 2 - 3;
        y = csbi.srWindow.Right / 2 - 19;
        if (x != oldx || y != oldy) {
            system("cls");
            PrintMenu(oldx = x, oldy = y, n);
            SetConsoleMode(hStdin, newConsoleMode);
        }

        ConsoleBufferSize(0, 0);

        GetNumberOfConsoleInputEvents(hStdin, &cNumRead);
        if (cNumRead != 0) {
            ReadConsoleInput(hStdin, &irInBuf, 1, &cNumRead);
            if (cNumRead != 0) {
                if ((irInBuf.Event.MouseEvent.dwEventFlags == 0 || irInBuf.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK) &&
                    irInBuf.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED &&
                    irInBuf.Event.MouseEvent.dwMousePosition.Y == n + oldx + 2) {
                    return n;
                }
                else if (irInBuf.Event.MouseEvent.dwEventFlags == MOUSE_MOVED &&
                    irInBuf.Event.MouseEvent.dwMousePosition.Y > oldx + 1 &&
                    irInBuf.Event.MouseEvent.dwMousePosition.Y < oldx + 7 &&
                    irInBuf.Event.MouseEvent.dwMousePosition.Y != n + oldx + 2) {
                    gotoXY(n + oldx + 2, oldy + 2);
                    putchar(' ');
                    n = irInBuf.Event.MouseEvent.dwMousePosition.Y - oldx - 2;
                    gotoXY(n + oldx + 2, oldy + 2);
                    putchar('>');
                }
            }
        }
    }
    return n;
}

void PrintEndMessage(int windowHeight, int windowWidth, int h, bool won, bool delay) {
    if (windowHeight < (csbi.srWindow.Bottom + h) / 2 + 8)
        ConsoleBufferSize(windowWidth, (csbi.srWindow.Bottom + h) / 2 + 8);

    windowHeight = (csbi.srWindow.Bottom % 2 == 0) ? 1 : 0;

    gotoXY((csbi.srWindow.Bottom + h) / 2 + 3 + windowHeight, csbi.srWindow.Right / 2 - 4);
    if (delay) PrintTextDelay(25, won ? 9 : 10, won ? "You Win!\n" : "You Lose!\n");
    else puts(won ? "You Win!\n" : "You Lose!\n");

    gotoXY((csbi.srWindow.Bottom + h) / 2 + 5 + windowHeight, csbi.srWindow.Right / 2 - 12);
    if (delay) PrintTextDelay(12, 25, "Press any key to continue\n\n");
    else puts("Press any key to continue\n");
    puts(" ");
}

int main() {
    SetConsoleTitleA("Bomber");
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    console = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleBufferSize(0, 0);
    SetConsoleTextAttribute(console, (0 << 4) + 7);
    GetConsoleMode(hStdin, &fdwSaveOldMode);
    newConsoleMode = fdwSaveOldMode | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    newConsoleMode &= ~ENABLE_QUICK_EDIT_MODE & ~ENABLE_INSERT_MODE;
    ShowConsoleCursor(false);

    int windowHeight(csbi.srWindow.Bottom), windowWidth(csbi.srWindow.Right), x, y;

    for (;;) {
        int h, w, b;
        Board* game(nullptr);

        switch (Menu()) {
        case 0: game = new Board(h = 9, w = 9, 10); break;
        case 1: game = new Board(h = 16, w = 16, 40); break;
        case 2: game = new Board(h = 16, w = 30, 99); break;
        case 3:
            windowHeight = csbi.srWindow.Bottom;
            windowWidth = csbi.srWindow.Right;
            gotoXY(windowHeight / 2 + 2, windowWidth / 2 - 15);
            PrintTextDelay(30, 6, "Enter:");
            gotoXY(windowHeight / 2 + 2, windowWidth / 2 - 4);
            ShowConsoleCursor(true);
            scanf("%d", &h);
            gotoXY(windowHeight / 2 + 2, windowWidth / 2 + 4);
            scanf("%d", &w);
            gotoXY(windowHeight / 2 + 2, windowWidth / 2 + 11);
            scanf("%d", &b);
            ShowConsoleCursor(false);
            game = new Board(h, w, b);
            break;
        default:
            if (game != nullptr)
                delete game;
            gotoXY(windowHeight - 3, 0);
            return 0;
        }
        game->Print();

        SetConsoleMode(hStdin, newConsoleMode);
        GetConsoleScreenBufferInfo(console, &csbi);
        windowHeight = csbi.srWindow.Bottom;
        windowWidth = csbi.srWindow.Right;

        bool finished(false), won(false);
        while (!finished) {
            ConsoleBufferSize(0, 0);
            GetConsoleScreenBufferInfo(console, &csbi);
            x = csbi.srWindow.Bottom;
            y = csbi.srWindow.Right;
            if (x != windowHeight || y != windowWidth) {
                windowHeight = x;
                windowWidth = y;
                game->Print();
                SetConsoleMode(hStdin, newConsoleMode);
            }

            GetNumberOfConsoleInputEvents(hStdin, &cNumRead);
            if (cNumRead != 0) {
                ReadConsoleInput(hStdin, &irInBuf, 1, &cNumRead);
                if (cNumRead != 0) {
                    if (irInBuf.Event.MouseEvent.dwEventFlags == 0 || irInBuf.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK) {
                        if (irInBuf.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                            switch (game->Open(irInBuf.Event.MouseEvent.dwMousePosition.Y, irInBuf.Event.MouseEvent.dwMousePosition.X)) {
                            case 'W':
                                won = true;
                            case 'L':
                                finished = true;
                                break;
                            }
                        }
                        else if (irInBuf.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
                            game->Flag(irInBuf.Event.MouseEvent.dwMousePosition.Y, irInBuf.Event.MouseEvent.dwMousePosition.X);
                        }
                    }
                }
            }
        }
        PrintEndMessage(windowHeight, windowWidth, h, won, true);
        GetConsoleScreenBufferInfo(console, &csbi);
        windowHeight = csbi.srWindow.Bottom;
        windowWidth = csbi.srWindow.Right;

        FlushConsoleInputBuffer(hStdin);
        for (;;) {
            ConsoleBufferSize(0, 0);
            GetConsoleScreenBufferInfo(console, &csbi);
            x = csbi.srWindow.Bottom;
            y = csbi.srWindow.Right;
            if (x != windowHeight || y != windowWidth) {
                windowHeight = x;
                windowWidth = y;
                game->Print();
                PrintEndMessage(windowHeight, windowWidth, h, won, false);
                SetConsoleMode(hStdin, newConsoleMode);
            }

            GetNumberOfConsoleInputEvents(hStdin, &cNumRead);
            if (cNumRead != 0) {
                ReadConsoleInput(hStdin, &irInBuf, 1, &cNumRead);
                if (cNumRead != 0) {
                    if (irInBuf.EventType == KEY_EVENT) break;
                    if (irInBuf.EventType == MOUSE_EVENT) {
                        if (irInBuf.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED ||
                            irInBuf.Event.MouseEvent.dwButtonState == FROM_LEFT_2ND_BUTTON_PRESSED ||
                            irInBuf.Event.MouseEvent.dwButtonState == FROM_LEFT_3RD_BUTTON_PRESSED ||
                            irInBuf.Event.MouseEvent.dwButtonState == FROM_LEFT_4TH_BUTTON_PRESSED ||
                            irInBuf.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED ||
                            irInBuf.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
                            break;
                    }
                }
            }
        }
        delete game;
        game = nullptr;
    }
    ShowConsoleCursor(true);
    SetConsoleMode(hStdin, fdwSaveOldMode);
    return 0;
}

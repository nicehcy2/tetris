#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>

#pragma warning(disable:4996)

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define LEFT_COLLISION 1
#define RIGHT_COLLISION 2
#define AIR 0
#define BLOCK 1
#define SAVEBLOCK 2
#define MAP 3
#define BOTTOM 4
#define LINE 5

using namespace std;

void gotoxy(int x, int y);
void setCursor();

const int straight_tetromino[4][4][4] =
{ { {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0} },
  { {0,0,0,0},
    {1,1,1,1},
    {0,0,0,0},
    {0,0,0,0} },
  { {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0} },
  { {0,0,0,0},
    {0,0,0,0},
    {1,1,1,1},
    {0,0,0,0} }
};

const int square_tetromino[4][4][4] =
{ { {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0} },
  { {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0} },
  { {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0} },
  { {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0} }
};

const int Z_tetromino[4][4][4] =
{ { {0,1,1,0},
    {0,0,1,1},
    {0,0,0,0},
    {0,0,0,0} },
  { {0,0,0,1},
    {0,0,1,1},
    {0,0,1,0},
    {0,0,0,0} },
  { {0,1,1,0},
    {0,0,1,1},
    {0,0,0,0},
    {0,0,0,0} },
  { {0,0,0,1},
    {0,0,1,1},
    {0,0,1,0},
    {0,0,0,0} }
};

const int S_tetromino[4][4][4] =
{ { {0,0,1,1},
    {0,1,1,0},
    {0,0,0,0},
    {0,0,0,0} },
  { {0,0,1,0},
    {0,0,1,1},
    {0,0,0,1},
    {0,0,0,0} },
  { {0,0,1,1},
    {0,1,1,0},
    {0,0,0,0},
    {0,0,0,0} },
  { {0,0,1,0},
    {0,0,1,1},
    {0,0,0,1},
    {0,0,0,0} }
};

const int L_tetromino[4][4][4] =
{ { {0,0,1,0},
    {0,0,1,0},
    {0,0,1,1},
    {0,0,0,0} },
  { {0,0,0,0},
    {0,1,1,1},
    {0,1,0,0},
    {0,0,0,0} },
  { {0,1,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,0,0} },
  { {0,0,0,1},
    {0,1,1,1},
    {0,0,0,0},
    {0,0,0,0} }
};

const int J_tetromino[4][4][4] =
{ { {0,0,1,0},
    {0,0,1,0},
    {0,1,1,0},
    {0,0,0,0} },
  { {0,1,0,0},
    {0,1,1,1},
    {0,0,0,0},
    {0,0,0,0} },
  { {0,0,1,1},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,0,0} },
  { {0,0,0,0},
    {0,1,1,1},
    {0,0,0,1},
    {0,0,0,0} }
};

const int T_tetromino[4][4][4] =
{ { {0,1,1,1},
    {0,0,1,0},
    {0,0,0,0},
    {0,0,0,0} },
  { {0,0,0,1},
    {0,0,1,1},
    {0,0,0,1},
    {0,0,0,0} },
  { {0,0,0,0},
    {0,0,1,0},
    {0,1,1,1},
    {0,0,0,0} },
  { {0,1,0,0},
    {0,1,1,0},
    {0,1,0,0},
    {0,0,0,0} }
};

int Y_MAP = 38, X_MAP = 18;

class Block {
public:
    Block() {
        rotCnt = 0, x = 0, y = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    block[i][j][k] = AIR;
                }
            }
        }
    }

    void setX(int x) {
        this->x = x;
    }
    void setY(int y) {
        this->y = y;
    }
    int getX() {
        return x;
    }
    int getY() {
        return y;
    }
    int getBlock(int rotationCount, int y, int x) {
        return block[rotationCount][y][x];
    }
    int getRotate() {
        return rotCnt;
    }
    void down() {
        y++;
    }
    void left() {
        x--;
    }
    void right() {
        x++;
    }
    void rotate() {
        rotCnt = (rotCnt + 1) % 4;
    }
    void setRotationCount(int r) {
        this->rotCnt = r;
    }
    void setShape(int r, int y, int x, int value) {
        this->block[r][y][x] = value;
    }

protected:
    int rotCnt, x, y;
    bool block[4][4][4];
};

class STRAIGT : public Block {
public:
    STRAIGT() {
        rotCnt = 0, x = X_MAP - 1 - 4, y = 1;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    block[i][j][k] = straight_tetromino[i][j][k];
                }
            }
        }
    }
};

class SQUARE : public Block {
public:
    SQUARE() {
        rotCnt = 0, x = X_MAP - 1 - 4, y = 1;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    block[i][j][k] = square_tetromino[i][j][k];
                }
            }
        }
    }
};

class ZBLOCK : public Block {
public:
    ZBLOCK() {
        rotCnt = 0, x = X_MAP - 1 - 4, y = 1;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    block[i][j][k] = Z_tetromino[i][j][k];
                }
            }
        }
    }
};

class SBLOCK : public Block {
public:
    SBLOCK() {
        rotCnt = 0, x = X_MAP - 1 - 4, y = 1;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    block[i][j][k] = S_tetromino[i][j][k];
                }
            }
        }
    }
};

class JBLOCK : public Block {
public:
    JBLOCK() {
        rotCnt = 0, x = X_MAP - 1 - 4, y = 1;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    block[i][j][k] = J_tetromino[i][j][k];
                }
            }
        }
    }
};

class LBLOCK : public Block {
public:
    LBLOCK() {
        rotCnt = 0, x = X_MAP - 1 - 4, y = 1;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    block[i][j][k] = L_tetromino[i][j][k];
                }
            }
        }
    }
};

class TBLOCK : public Block {
public:
    TBLOCK() {
        rotCnt = 0, x = X_MAP - 1 - 4, y = 1;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    block[i][j][k] = T_tetromino[i][j][k];
                }
            }
        }
    }
};

class backup {
public:
    /*블럭 백업*/
    static void updateBlock(Block* source, Block& backupBlock) {
        backupBlock.setX(source->getX()); // 블럭의 x좌표 백업
        backupBlock.setY(source->getY()); // 블럭의 y좌표 백업
        backupBlock.setRotationCount(source->getRotate()); // 블럭의 회전 상태 변수 백업
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    backupBlock.setShape(i, j, k, source->getBlock(i, j, k)); // 블럭의 모양 백업
                }
            }
        }
    }
    /*table 백업*/
    static void updateTable(vector<vector<int> >& source, vector<vector<int> >& backupTable) {
        backupTable.resize(source.size(), vector<int>(source.size())); // 기존 table의 크기만큼 2차원 벡터 크기 할당
        copy(source.begin(), source.end(), backupTable.begin()); // 기존 table vector을 backupTable vector에 백업
    }
};

class GameTable {
public:
    GameTable() {
        vector<int> Xtable;
        for (int i = 0; i < Y_MAP; i++) {
            for (int j = 0; j < X_MAP; j++) {
                Xtable.push_back(AIR);
            }
            blockTable.push_back(Xtable);
        }

        // set Map
        for (int x = 0; x < X_MAP; x++) {
            blockTable[0][x] = MAP;
        }
        for (int x = 1; x < X_MAP - 1; x++) {
            blockTable[5][x] = LINE;
        }
        for (int y = 1; y < Y_MAP - 1; y++) {
            blockTable[y][0] = MAP;
            blockTable[y][X_MAP - 1] = MAP;
        }
        for (int x = 0; x < X_MAP; x++) {
            blockTable[Y_MAP - 1][x] = BOTTOM;
        }

        int x = 0, y = 0, cnt = 0;
    }

    void createBlock(int sel) {
        if (sel == 0) {
            block = new STRAIGT();
        }
        else if (sel == 1) {
            block = new SQUARE();
        }
        else if (sel == 2) {
            block = new ZBLOCK();
        }
        else if (sel == 3) {
            block = new SBLOCK();
        }
        else if (sel == 4) {
            block = new JBLOCK();
        }
        else if (sel == 5) {
            block = new LBLOCK();
        }
        else if (sel == 6) {
            block = new TBLOCK();
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int Y = i + block->getY();
                int X = j + block->getX();
                blockTable[Y][X] = block->getBlock(block->getRotate(), i, j);
            }
        }
    }

    void drawTable() {
        for (int i = 0; i < Y_MAP; i++) {
            for (int j = 0; j < X_MAP; j++) {
                if (blockTable[i][j] == MAP || blockTable[i][j] == BOTTOM) cout << "▦";
                else if (blockTable[i][j] == BLOCK) cout << "■";
                else if (blockTable[i][j] == SAVEBLOCK) cout << "□";
                else cout << "  ";
            }
            cout << "\n";
        }
    }

    void setBlock() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (block->getBlock(block->getRotate(), i, j) == BLOCK) {
                    int Y = i + block->getY();
                    int X = j + block->getX();
                    blockTable[Y][X] = BLOCK;
                }
            }
        }
    }

    bool checkBottomBlock() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int X = j + block->getX();
                int Y = i + block->getY();
                int blockValue = block->getBlock(block->getRotate(), i, j);
                if (blockValue == 1 && blockTable[Y + 1][X] == SAVEBLOCK) {
                    return true;
                }

                if (blockValue == BLOCK && Y >= Y_MAP - 2) {
                    return true;
                }

            }
        }
        return false;
    }

    void checkSaveBlock() {
        int i, j;
        for (i = Y_MAP - 1; i >= 5;) {
            for (j = 1; j < X_MAP - 1; j++) {
                if (blockTable[i][j] != SAVEBLOCK) {
                    break;
                }
            }
            if (j == X_MAP - 1) {
                for (int k = i; k >= 6; k--) {
                    for (int l = 1; l < X_MAP - 1; l++) {
                        if (blockTable[k][l] == SAVEBLOCK) {
                            blockTable[k][l] = AIR;
                            if (blockTable[k + 1][l] != MAP)
                                blockTable[k][l] = blockTable[k - 1][l];
                        }
                    }
                }
            }
            else
                i--;
        }
    }

    void removeBlock() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (block->getBlock(block->getRotate(), i, j) == BLOCK) {
                    int Y = i + block->getY();
                    int X = j + block->getX();
                    blockTable[Y][X] = AIR;
                }
            }
        }
    }

    bool moveBlock(wchar_t key) {
        removeBlock();
        if (key == LEFT) {
            block->left();
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    int Y = i + block->getY();
                    int X = j + block->getX();
                    int blockValue = block->getBlock(block->getRotate(), i, j);
                    if (blockValue != BLOCK) continue; // 블럭이 아니면 무시 (블럭은 2로 이루어져있음)
                    if (blockTable[Y][X] == AIR) { // 빈공간이면 (갱신)
                        blockTable[Y][X] = blockValue; // 블럭을 이동시킴
                    }
                    else if (blockTable[Y][X] == MAP) { // 블럭이 양 옆 벽면에 닿으면 (취소)
                        block->right();
                        return false; // 함수 종료
                    }
                }
            }
        }
        else if (key == RIGHT) {
            block->right();
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    int Y = i + block->getY();
                    int X = j + block->getX();
                    int blockValue = block->getBlock(block->getRotate(), i, j);
                    if (blockValue != BLOCK) continue; // 블럭이 아니면 무시 (블럭은 2로 이루어져있음)
                    if (blockTable[Y][X] == AIR) { // 빈공간이면 (갱신)
                        blockTable[Y][X] = blockValue; // 블럭을 이동시킴
                    }
                    else if (blockTable[Y][X] == MAP) { // 블럭이 양 옆 벽면에 닿으면 (취소)
                        block->left();
                        return false; // 함수 종료
                    }
                }
            }
        }
        else if (key == DOWN) {
            block->down();
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    int Y = i + block->getY();
                    int X = j + block->getX();
                    int blockValue = block->getBlock(block->getRotate(), i, j);
                    if (blockValue != BLOCK) continue; // 블럭이 아니면 무시 (블럭은 2로 이루어져있음)
                    if (blockTable[Y][X] == AIR) { // 빈공간이면 (갱신)
                        blockTable[Y][X] = blockValue; // 블럭을 이동시킴
                    }
                    else if (blockTable[Y][X] == SAVEBLOCK || blockTable[Y][X] == BOTTOM) { // 블럭이 양 옆 벽면에 닿으면 (취소)
                        return false; // 함수 종료
                    }
                }
            }
        }


        return true;
    }

    bool rotateBlock() {
        removeBlock();

        block->rotate();

        bool flag = false;
        int moveCnt = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int Y = i + block->getY();
                int X = j + block->getX();
                int blockValue = block->getBlock(block->getRotate(), i, j);
                if (blockValue != BLOCK) continue; // 블럭이 아니면 무시 (블럭은 2로 이루어져있음)
                if (blockTable[Y][X] == AIR) { // 빈공간이면 (갱신)
                    blockTable[Y][X] = blockValue; // 블럭을 이동시킴
                }
                else if (blockTable[Y][X] == MAP) {
                    // 백업 후 이동
                    moveCnt = -4;
                    setX(20);
                    return -10;
                }

            }

        }


        return moveCnt;
    }

    void buildBlock() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int Y = i + block->getY();
                int X = j + block->getX();
                int blockValue = block->getBlock(block->getRotate(), i, j);
                if (blockValue == BLOCK) {
                    blockTable[Y][X] = SAVEBLOCK;
                }
            }
        }
    }

    void setX(int tX) {
        this->tX = tX;
        block->setX(tX);
    }
    void setY(int tY) {
        this->tY = tY;
        block->setY(tY);
    }
    int getX() {
        return tX;
    }
    int getY() {
        return tY;
    }
private:
    vector<vector<int>> blockTable;
    int tX, tY, cnt;
    Block* block;
};

void titlePage();
void playGame(GameTable*);

int main(void) {
    system("mode con cols=100 lines=40 | title 테트리스 게임");
    setCursor();
    titlePage();
    GameTable* gt = new GameTable();
    playGame(gt);

    return 0;
}

void setCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.bVisible = 0;
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void gotoxy(int x, int y) {
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void drawTitle() {
    int x = 5;
    int y = 4;

    gotoxy(x, y + 0); printf("■□□□■■■□□■■□□■■");
    gotoxy(x, y + 1); printf("■■■□  ■□□    ■■□□■");
    gotoxy(x, y + 2); printf("□□□■              □■  ■");
    gotoxy(x, y + 3); printf("■■□■■  □  ■  □□■□□");
    gotoxy(x, y + 4); printf("■■  ■□□□■■■□■■□□");
    gotoxy(x + 9, y + 2); printf("T E T R I S");
    gotoxy(x, y + 7); printf("Please Enter Any Key to Start..");
    gotoxy(x, y + 9);  printf("  △   : Shift");
    gotoxy(x, y + 10); printf("◁  ▷ : Left / Right");
    gotoxy(x, y + 11); printf("  ▽   : Soft Drop");
    gotoxy(x, y + 12); printf(" SPACE : Hard Drop");
    gotoxy(x, y + 13); printf("   P   : Pause");
    gotoxy(x, y + 14); printf("  ESC  : Quit");
    gotoxy(x, y + 16); printf("BONUS FOR HARD DROPS / COMBOS");
}

void titlePage() {
    while (true) {
        drawTitle();

        wchar_t key = getch();

        // ESC키 입력 시
        if (key == 27) {
            break;
        }
        // Enter키 입력 시
        else if (key == 13) {
            // 해당 되는 선택 지 실행
            // title 지우기
            system("cls");
            break;
        }
    }
}

void playGame(GameTable* gt) {
    while (true) {
        srand(time(NULL));
        int sel = rand() % 7;

        gt->createBlock(sel);

        int bX = X_MAP - 1 - 4;
        int bY = 1;
        while (true) {
            gotoxy(0, 0);
            gt->setX(bX);
            gt->setY(bY);
            gt->setBlock();
            gt->drawTable();
            gt->checkSaveBlock();
            for (int i = 0; i < 5; i++) {
                int moveCnt = 0;
                if (kbhit()) {
                    wchar_t key = getch();
                    if (key == 224) {
                        key = getch();
                        switch (key) {
                        case UP:
                            if ((moveCnt = gt->rotateBlock())) {
                                bX += moveCnt;
                            }
                            gotoxy(0, 0);
                            gt->setX(bX);
                            gt->setBlock();
                            gt->drawTable();

                            break;
                        case DOWN:
                            if (gt->moveBlock(DOWN)) {
                                if (gt->checkBottomBlock()) {
                                    goto CON;
                                }
                                bY++;
                            }
                            gotoxy(0, 0);
                            gt->setBlock();
                            gt->drawTable();
                            break;
                        case LEFT:
                            if (gt->moveBlock(LEFT)) {
                                bX--;
                            }
                            gotoxy(0, 0);
                            gt->setBlock();
                            gt->drawTable();
                            break;
                        case RIGHT:
                            if (gt->moveBlock(RIGHT)) {
                                bX++;
                            }
                            gotoxy(0, 0);
                            gt->setBlock();
                            gt->drawTable();
                            break;
                        default:
                            break;
                        }
                    }
                }
                Sleep(300 / 5 / 5);
                gt->checkSaveBlock();
            }
            if (gt->checkBottomBlock()) {
            CON:
                gt->removeBlock();
                gt->buildBlock();
                break;
            }
            bY++;
            gt->removeBlock();
        }
    }


}



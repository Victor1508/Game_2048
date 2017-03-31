#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <ctime>
#include <vector>
#include <iomanip>

using namespace std;

const int DIMENSION = 4;
enum Direction {STOP, UP, DOWN, LEFT, RIGHT, NO_INPUT};

class Game2048
{
private:
    bool gameOver;
    bool change = true;        // this variable to check if the board has changed after GetDirection
    vector<vector<int>, 4> board;
    int board[4][4];
    int score;
    Direction dir;

public:
    Game2048() : gameOver(false), board( {}), score(0)
    {
        for(int i = 0; i < 2; ++i)
            CreateValue();
        change = false;
    }

    bool IsGameOver()
    {
        return gameOver;
    }

    void CreateValue()
    {
        if(!gameOver && dir != NO_INPUT && change == true) {
            int i, j;
            srand(time(0));
            do {
                i = rand() % DIMENSION;
                j = rand() % DIMENSION;
            } while(board[i][j] != 0);
            // create number with 80% create number 2 and 20% create number 4
            bool randomNumberTwo = (rand() % 100) < 80;
            if(randomNumberTwo)
                board[i][j] = 2;
            else
                board[i][j] = 4;
        }
    }

    Direction GetDirection()
    {
        switch (_getch()) {
        case 'w':
        case 'W':
            dir = UP;
            break;
        case 's':
        case 'S':
            dir = DOWN;
            break;
        case 'a':
        case 'A':
            dir = LEFT;
            break;
        case 'd':
        case 'D':
            dir = RIGHT;
            break;
        case 'x':
        case 'X':
            dir = STOP;
            break;
        default:
            dir = NO_INPUT;
        }
    }

    void Logic()
    {
        // do something when get direction
        bool isMove = false,
             isMerge = false,
             isMoveAgain = false;

        // UP
        if(dir == UP) {
            // move number up to top
            isMove = false;
            for(int j = 0; j < DIMENSION; ++j) {
                for(int i = 0; i < DIMENSION; ++i) {
                    if(board[i][j] != 0) {
                        int flag = 0;
                        while (flag < i && board[flag][j] != 0)
                            ++flag;
                        if(flag < i && board[i][j] != 0) {
                            board[flag][j] = board[i][j];
                            board[i][j] = 0;
                            isMove = true;
                        }
                    }
                }
            }

            // merge 2 equal number
            isMerge = false;
            for(int j = 0; j < DIMENSION; ++j) {
                for(int i = 0; i < DIMENSION-1; ++i) {
                    if(board[i][j] != 0 && board[i][j] == board[i+1][j]) {
                        board[i][j] += board[i+1][j];
                        score += board[i][j];
                        board[i+1][j] = 0;
                        isMerge = true;
                    }

                }
            }

            // after move and merge, the result will be like 4 4 4 4 -> 8 0 8 0 (it for move to left, but all directions are similar)
            // so we have some cases (only with board[i][j] != 0 because we don't need to move zero-number):
            // at index [0][j], we don't need to move the result because it already at top
            // at index [1][j], we don't need to move, because after move and merge, if board[1][j] != 0 => board[0][j] != 0 => can't move
            // so we only need to check for index [2][j] and [3][j]
            isMoveAgain = false;
            for(int j = 0; j < DIMENSION; ++j) {
                for(int i = 2; i < DIMENSION; ++i) {
                    if(board[i][j] != 0 && board[i-1][j] == 0) {
                        board[i-1][j] = board[i][j];
                        board[i][j] = 0;
                        isMoveAgain = true;
                    }
                }
            }


            // if any step about change the board, then change = true
            change = (isMove || isMerge || isMoveAgain);
        }

        // DOWN
        else if(dir == DOWN) {
            isMove = false;
            for(int j = 0; j < DIMENSION; ++j) {
                for(int i = DIMENSION-1; i >= 0; --i) {
                    if(board[i][j] != 0) {
                        int flag = DIMENSION-1;
                        while (flag > i && board[flag][j] != 0)
                            --flag;
                        if(flag > i && board[i][j] != 0) {
                            board[flag][j] = board[i][j];
                            board[i][j] = 0;
                            isMove = true;
                        }
                    }
                }
            }

            isMerge = false;
            for(int j = 0; j < DIMENSION; ++j) {
                for(int i = DIMENSION-1; i > 0; --i) {
                    if(board[i][j] != 0 && board[i][j] == board[i-1][j]) {
                        board[i][j] *= 2;
                        score += board[i][j];
                        board[i-1][j] = 0;
                        isMerge = true;
                    }
                }
            }

            isMoveAgain = false;
            for(int j = 0; j < DIMENSION; ++j) {
                for(int i = DIMENSION-1; i >= 0; --i) {
                    if(board[i][j] != 0) {
                        int flag = DIMENSION-1;
                        while (flag > i && board[flag][j] != 0)
                            --flag;
                        if(flag > i) {
                            board[flag][j] = board[i][j];
                            board[i][j] = 0;
                            isMoveAgain = true;
                        }
                    }
                }
            }

            change = (isMove || isMerge || isMoveAgain);
        }

        // LEFT
        else if(dir == LEFT) {
            isMove = false;
            for(int i = 0; i < DIMENSION; ++i) {
                for(int j = 0; j < DIMENSION; ++j) {
                    if(board[i][j] != 0) {
                        int flag = 0;
                        while (flag < j && board[i][flag] != 0)
                            ++flag;
                        if(flag < j && board[i][j] != 0) {
                            board[i][flag] = board[i][j];
                            board[i][j] = 0;
                            isMove = true;
                        }
                    }
                }
            }

            isMerge = false;
            for(int i = 0; i < DIMENSION; ++i) {
                for(int j = 0; j < DIMENSION-1; ++j) {
                    if(board[i][j] != 0 && board[i][j] == board[i][j+1]) {
                        board[i][j] += board[i][j+1];
                        board[i][j+1] = 0;
                        score += board[i][j];
                        isMerge = true;
                    }

                }
            }

            isMoveAgain = false;
            for(int i = 0; i < DIMENSION; ++i) {
                for(int j = 2; j < DIMENSION; ++j) {
                    if(board[i][j] != 0 && board[i][j-1] == 0) {
                        board[i][j-1] = board[i][j];
                        board[i][j] = 0;
                        isMoveAgain = true;
                    }
                }
            }

            change = (isMove || isMerge || isMoveAgain);
        }

        // RIGHT
        else if(dir == RIGHT) {
            isMove = false;
            for(int i = 0; i < DIMENSION; ++i) {
                for(int j = DIMENSION-1; j >= 0; --j) {
                    if(board[i][j] != 0) {
                        int flag = DIMENSION-1;
                        while (flag > j && board[i][flag] != 0)
                            --flag;
                        if(flag > j && board[i][j] != 0) {
                            board[i][flag] = board[i][j];
                            board[i][j] = 0;
                            isMove = true;
                        }
                    }
                }
            }

            isMerge = false;
            for(int i = 0; i < DIMENSION; ++i) {
                for(int j = DIMENSION-1; j > 0; --j) {
                    if(board[i][j] != 0 && board[i][j] == board[i][j-1]) {
                        board[i][j] *= 2;
                        score += board[i][j];
                        board[i][j-1] = 0;
                        isMerge = true;
                    }
                }
            }

            isMoveAgain = false;
            for(int i = 0; i < DIMENSION; ++i) {
                for(int j = DIMENSION-1; j >= 0; --j) {
                    if(board[i][j] != 0) {
                        int flag = DIMENSION-1;
                        while (flag > j && board[i][flag] != 0)
                            --flag;
                        if(flag > j) {
                            board[i][flag] = board[i][j];
                            board[i][j] = 0;
                            isMoveAgain = true;
                        }
                    }
                }
            }

            change = (isMove || isMerge || isMoveAgain);
        }

        // STOP
        else if(dir == STOP) {
            gameOver = true;
        }
        // end direction


        // if no space left, then gameOver = true
        int spaceCell = DIMENSION*DIMENSION;
        for(int i = 0; i < DIMENSION; ++i)
            for (int j = 0; j < DIMENSION; ++j) {
                if(board[i][j] != 0)
                    --spaceCell;
            }
        // if no space left, check if no 2 adjacent numbers are equal, then gameOver = true
        if(spaceCell == 0)
            CheckGameOver();
    }

    void CheckGameOver() {
        gameOver = true;
        for(int i = 0; i < DIMENSION && gameOver; ++i) {
            for(int j = 0; j < DIMENSION && gameOver; ++j) {
                if(i < DIMENSION-1 && j < DIMENSION-1 && (board[i][j] == board[i+1][j] || board[i][j] == board[i][j+1]))
                    gameOver = false;
                else if(i < DIMENSION-1 && j == DIMENSION-1 && board[i][j] == board[i+1][j])
                    gameOver  = false;
                else if(j < DIMENSION-1 && i == DIMENSION-1 && board[i][j] == board[i][j+1])
                    gameOver = false;
            }
        }
    }

    void Draw()
    {
        system("cls");
        cout << setw(14) << "GAME 2048" << endl;
        cout << "Score:\t" << score << endl;
        for(int i = 0; i < DIMENSION; ++i) {
            cout << endl;
            for(int j = 0; j < DIMENSION; ++j) {
                if(board[i][j] == 0)
                    cout << "----" << " ";
                else if(board[i][j] < 10)
                    cout << "--" << board[i][j] << "-" << " ";
                else if(board[i][j] << 100)
                    cout << "-" << board[i][j] << "-" << " ";
                else if(board[i][j] < 1000)
                    cout << "-" << board[i][j] << " ";
                else
                    cout << board[i][j] << endl;
            }
            cout << endl;
        }
    }
};

int main()
{
    Game2048 game2048;
    while(game2048.IsGameOver() == false) {
        game2048.Draw();
        game2048.GetDirection();
        game2048.Logic();
        game2048.CreateValue();
    }
    cout << endl << "GAME OVER!" << endl;

    return 0;
}

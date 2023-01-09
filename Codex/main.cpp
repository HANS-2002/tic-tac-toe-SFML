#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <bits/stdc++.h>

using namespace sf;
using namespace std;

int N = 5, M = 5;
int blockSize = 128;
int w = blockSize * N;
int h = blockSize * M;
char board[3][3] = {
    {'.', '.', '.'},
    {'.', '.', '.'},
    {'.', '.', '.'}};
char currPlay = 'X';

Texture backgroundTexture, crossBackgroundTexture, circleBackgroundTexture, boardBackgroundTexture;
SoundBuffer bgmBuffer;

Sprite backgroundSprite, crossBackgroundSprite, circleBackgroundSprite, boardBackgroundSprite;
Sound bgmSound;
Music bgmMusic;

RenderWindow window(VideoMode(w, h), "TIC-TAC-TOE");

Font font;

bool gameOver = false;

void init()
{
    window.setKeyRepeatEnabled(false);
    font.loadFromFile("./assets/font.ttf");

    backgroundTexture.loadFromFile("./assets/images/background.png");
    boardBackgroundTexture.loadFromFile("./assets/images/board.png");
    crossBackgroundTexture.loadFromFile("./assets/images/cross.png");
    circleBackgroundTexture.loadFromFile("./assets/images/circle.png");
    bgmBuffer.loadFromFile("./assets/sounds/bgm.wav");

    backgroundSprite.setTexture(backgroundTexture);
    crossBackgroundSprite.setTexture(crossBackgroundTexture);
    circleBackgroundSprite.setTexture(circleBackgroundTexture);
    boardBackgroundSprite.setTexture(boardBackgroundTexture);
    // bgmSound.setBuffer(bgmBuffer);
    // bgmSound.play();
    // bgmSound.setVolume(10);
    // bgmSound.setLoop(true);

    bgmMusic.openFromFile("./assets/sounds/bgm.wav");
    bgmMusic.play();
    bgmMusic.setVolume(10);
    bgmMusic.setLoop(true);
}

bool isTie()
{
    return (board[0][0] != '.' && board[0][1] != '.' && board[0][2] != '.' && board[1][0] != '.' && board[1][1] != '.' && board[1][2] != '.' && board[2][0] != '.' && board[2][1] != '.' && board[2][2] != '.');
}

bool hasWon(char x)
{
    if (board[0][0] == x && board[1][1] == x && board[2][2] == x)
        return true;
    if (board[0][2] == x && board[1][1] == x && board[2][0] == x)
        return true;

    if (board[0][0] == x && board[0][1] == x && board[0][2] == x)
        return true;
    if (board[1][0] == x && board[1][1] == x && board[1][2] == x)
        return true;
    if (board[2][0] == x && board[2][1] == x && board[2][2] == x)
        return true;

    if (board[0][0] == x && board[1][0] == x && board[2][0] == x)
        return true;
    if (board[0][1] == x && board[1][1] == x && board[2][1] == x)
        return true;
    if (board[0][2] == x && board[1][2] == x && board[2][2] == x)
        return true;

    return false;
}

void drawBoard(RenderWindow *window, int N, int M, int size)
{
    Texture background;
    Sprite backgroundSprite;
    background.loadFromFile("./assets/images/background.png");
    backgroundSprite.setTexture(background);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            backgroundSprite.setPosition(i * size, j * size);
            window->draw(backgroundSprite);
        }
    }
}

void drawMatrix(RenderWindow *window, char board[][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == '.')
            {
                boardBackgroundSprite.setPosition((i + 1) * blockSize, (j + 1) * blockSize);
                window->draw(boardBackgroundSprite);
            }
            else if (board[i][j] == 'X')
            {
                crossBackgroundSprite.setPosition((i + 1) * blockSize, (j + 1) * blockSize);
                window->draw(crossBackgroundSprite);
            }
            else if (board[i][j] == 'O')
            {
                circleBackgroundSprite.setPosition((i + 1) * blockSize, (j + 1) * blockSize);
                window->draw(circleBackgroundSprite);
            }
        }
    }
}

void drawText(RenderWindow *window, int w, int h, int size, Font font, string str)
{
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString(str);
    gameOverText.setCharacterSize(24);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setStyle(Text::Bold);
    gameOverText.setOrigin(gameOverText.getGlobalBounds().width / 2.0f, gameOverText.getGlobalBounds().height / 2.0f);
    gameOverText.setPosition(w / 2.0f, size / 2.0f);
    window->draw(gameOverText);
}

void updateMatrix(int x, int y)
{
    if (board[x][y] == '.')
    {
        board[x][y] = currPlay;
        currPlay = (currPlay == 'X') ? 'O' : 'X';
    }
}

void reset()
{
    gameOver = false;
    currPlay = 'X';
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = '.';
}

int main()
{
    init();
    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
            else if (e.type == Event::MouseButtonReleased && e.mouseButton.button == Mouse::Left && !gameOver)
            {
                Vector2i mousePos = Mouse::getPosition(window);
                int x = -1, y = -1;
                if (mousePos.x >= blockSize && mousePos.x <= blockSize * 4 && mousePos.y >= blockSize && mousePos.y <= blockSize * 4)
                {
                    x = mousePos.x / blockSize - 1;
                    y = mousePos.y / blockSize - 1;
                    updateMatrix(x, y);
                }
            }
            else if (e.type == Event::KeyPressed && e.key.code == Keyboard::Enter && gameOver)
                reset();
        }
        if (!gameOver)
        {
            window.clear();
            if (hasWon('X'))
            {
                gameOver = true;
                string str = "\t\t\t\tX won!\nPress Enter to restart.";
                drawBoard(&window, N, M, blockSize);
                drawMatrix(&window, board);
                drawText(&window, w, h, blockSize, font, str);
            }
            else if (hasWon('O'))
            {
                gameOver = true;
                string str = "\t\t\t\tO won!\nPress Enter to restart.";
                drawBoard(&window, N, M, blockSize);
                drawMatrix(&window, board);
                drawText(&window, w, h, blockSize, font, str);
            }
            else if (isTie())
            {
                gameOver = true;
                string str = "It's a draw!";
                drawBoard(&window, N, M, blockSize);
                drawMatrix(&window, board);
                drawText(&window, w, h, blockSize, font, str);
            }
            else
            {
                string str = "Turn for ";
                if (currPlay == 'X')
                    str += 'X';
                else
                    str += 'O';
                drawBoard(&window, N, M, blockSize);
                drawMatrix(&window, board);
                drawText(&window, w, h, blockSize, font, str);
            }
            window.display();
        }
    }

    return 0;
}

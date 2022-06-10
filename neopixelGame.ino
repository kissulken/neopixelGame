#include <TrueRandom.h>
#include <Adafruit_NeoPixel.h>
#include <OneButton.h>
#define neopin 4
#define pixelcount 24
#define button_pin 3

OneButton button(button_pin, false);
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(pixelcount, neopin, NEO_GRB + NEO_KHZ800);

int index = -1;
int indexTrail = -1;
int Direction = 1;
float nextMove = 0;
float currentPlayerSpeed = 150;
unsigned long countdown = 0;
int enemyIndex = -1;
int coinIndex = -1;
int score = 0;
int bestScore = 0;
int lastScore = 0;
bool gameOver = false;

void setup()
{
    pinMode(button_pin, INPUT);
    button.attachClick(singleClick);
    button.attachLongPressStart(longClick);
    pixel.begin();
    pixel.setBrightness(20);
    countdown = millis() + 2000;
}

void loop()
{
    button.tick();
    if (gameOver)
    {
        return;
    }
    SetLevel();
    if (countdown > millis())
    {
        pixel.show();
        return;
    }
    DisplayPlayer();
    pixel.show();
}

void ClearLevel()
{
    for (int i = 0; i < pixelcount; i++)
    {
        pixel.setPixelColor(i, pixel.Color(0, 0, 0));
    }
    pixel.show();
}

void BestScore()
{
    for (int i = 0; i < pixelcount; i++)
    {
        if (i < bestScore)
            pixel.setPixelColor(i, pixel.Color(255, 155, 0));
        else
            pixel.setPixelColor(i, pixel.Color(0, 0, 0));
    }
    if (lastScore < bestScore)
    {
        for (int i = 0; i < lastScore; i++)
            pixel.setPixelColor(i, pixel.Color(255, 0, 0));
    }
    pixel.show();
}

void GameOver()
{
    int a = enemyIndex;
    int b = enemyIndex;
    for (int i = 0; i < pixelcount / 2; i++)
    {
        pixel.setPixelColor(a, pixel.Color(255, 0, 0));
        pixel.setPixelColor(b, pixel.Color(255, 0, 0));
        a = (a + 1) % pixelcount;
        b--;
        if (b == -1)
        {
            b = pixelcount;
        }
        pixel.show();
        delay(20);
    }
    delay(100);
    a = enemyIndex;
    b = enemyIndex;
    for (int i = 0; i < pixelcount / 2; i++)
    {
        pixel.setPixelColor(a, pixel.Color(0, 0, 0));
        pixel.setPixelColor(b, pixel.Color(0, 0, 0));
        a = (a + 1) % pixelcount;
        b--;
        if (b == -1)
        {
            b = pixelcount;
        }
        pixel.show();
        delay(20);
    }
    delay(100);
    BestScore();
}

void SetLevel()
{
    if (enemyIndex < 0)
    {
        if (index < 0)
        {
            enemyIndex = pixelcount / 2;
        }
        else
        {
            enemyIndex = TrueRandom.random(0, pixelcount);
            while (abs(enemyIndex - index) < (pixelcount / 4))
            {
                enemyIndex = TrueRandom.random(0, pixelcount);
            }
        }
    }
    if (coinIndex < 0)
    {
        coinIndex = TrueRandom.random(0, pixelcount);
        while (abs(coinIndex - index) < 7 || (abs(coinIndex - enemyIndex) < 7))
        {
            coinIndex = TrueRandom.random(0, pixelcount);
        }
    }
    pixel.setPixelColor(enemyIndex, pixel.Color(255, 0, 0));
    pixel.setPixelColor(coinIndex, pixel.Color(255, 255, 0));
}

void DisplayPlayer()
{
    if (nextMove < millis())
    {
        nextMove = millis() + currentPlayerSpeed;
        if (indexTrail >= 0)
        {
            pixel.setPixelColor(indexTrail, pixel.Color(0, 0, 0));
        }
        if (index >= 0)
        {
            pixel.setPixelColor(index, pixel.Color(0, 100, 0));
            indexTrail = index;
        }
        index += Direction;
        if (index < 0)
        {
            index = pixelcount - 1;
        }
        else if (index == pixelcount)
        {
            index = 0;
        }
        pixel.setPixelColor(index, pixel.Color(0, 255, 0));
        if (index == coinIndex)
        {
            enemyIndex = -1;
            coinIndex = -1;
            score++;
            currentPlayerSpeed = constrain(currentPlayerSpeed - 10, 50, 150);
            ClearLevel();
            pixel.setPixelColor(index, pixel.Color(0, 255, 0));
        }
        else if (index == enemyIndex)
        {
            lastScore = score;
            if (score >= bestScore)
            {
                bestScore = score;
            }
            GameOver();
            gameOver = true;
            enemyIndex = -1;
            coinIndex = -1;
            index = -1;
        }
    }
}

void singleClick()
{
    if (countdown > millis())
    {
        return;
    }
    Direction = -Direction;
}

void longClick()
{
    gameOver = !gameOver;
    if (gameOver)
    {
        enemyIndex = -1;
        coinIndex = -1;
        index = -1;
        currentPlayerSpeed = 150;
        ClearLevel();
    }
    else
    {
        ClearLevel();
        score = 0;
        currentPlayerSpeed = 150;
        countdown = millis() + 2000;
    }
}
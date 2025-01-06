#include <stdio.h>
#include <stdlib.h>
#include<windows.h>
#include<time.h>
#include"function.h"

int main()
{
    system("mode con cols=160 lines=45");
    system("title SNAKE");
    border();
    (game_start())?gameover(1):gameover(0);
    system("PAUSE");
    return 0;
}



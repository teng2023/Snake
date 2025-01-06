#include <stdio.h>
#include <stdlib.h>
#include<windows.h>
#include<time.h>
#include"function.h"

#define length 26
#define fruit_total 20
#define obstacle_amount 20

void border()       //running space clos=2~128, lines=1~40
{
    int i,j,k;
    for(i=0;i<42;i++)
    {
        for(j=0;j<130;j++)
        {
            if(i==0||i==41)
            {
                for(k=0;k<130;k++)
                {
                    printf("\33[43m \33[m");
                }
                break;
            }
            else if(j==0||j==127)
            {
                printf("\33[43m  \33[m");
            }
            else if(j<127)
            {
                printf("\33[47m \33[m");
            }
        }
        printf("\n");
    }
}

void snake()
{
    printf("\033[34m¡´\033[m");
}

void fruit()
{
    printf("\33[41m@\33[m");
}

void obstacle()
{
    printf("\33[1m\33[46mX\33[0m");
}

void information(int body_number,int loop_time,time_t t1,time_t t2)
{
    COORD xy;
    int t=t2-t1;
    xy.X=140;
    xy.Y=7;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
    printf("Snake Length:");
    xy.X=154;
    xy.Y=7;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
    printf("%d",body_number);
    xy.X=140;
    xy.Y=10;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
    printf("Score:");
    xy.X=147;
    xy.Y=10;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
    printf("%d",loop_time);
    xy.X=140;
    xy.Y=13;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
    printf("Time:");
    xy.X=146;
    xy.Y=13;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
    printf("%d (sec)",t);
    xy.X=140;
    xy.Y=20;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
    printf("\33[1m\33[42mObjects:\33[0m");
    xy.X=140;
    xy.Y=22;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
    printf("Fruits:   ");
    fruit();
    xy.X=140;
    xy.Y=24;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
    printf("Obstacle: ");
    obstacle();
    xy.X=140;
    xy.Y=26;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
    printf("Snake: ");
    snake();
    xy.X=140;
    xy.Y=31;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
    printf("\33[1m\33[45mRules:\33[0m");
    xy.X=136;
    xy.Y=33;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
    printf("Eat 20 fruits then win.");
    xy.X=138;
    xy.Y=35;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
    printf("(snake's length=25)");

}

void generate_fruit(int *fx,int *fy,int *fruit_remain,int *ox,int *oy)
{
    srand(time(NULL));
    int i,j,key=0;
    COORD xy;
    for(i=fruit_total-*fruit_remain;i<fruit_total;i++)
    {
        if(*fruit_remain==0)
        {
            break;
        }
        do
        {
            fx[i]=rand()%(125)+2;
            fy[i]=rand()%(39)+1;
            key=0;
            if(object_vs_obstacle(fx,fy,ox,oy))
            {
                key=1;
                continue;
            }
            for(j=0;j<=i;j++)
            {
                if(fx[i]==fx[j]&&fy[i]==fy[j])
                {
                    key=1;
                    break;
                }
            }
        }while(key==0);
        xy.X=fx[i];
        xy.Y=fy[i];
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
        fruit();
        (*fruit_remain)--;
        break;
    }
}

void generate_obstacle(int *ox,int *oy)
{
    srand(time(NULL));
    int i,j,key=0;
    COORD xy;
    for(i=0;i<obstacle_amount;i++)
    {
        do
        {
            *(ox+i)=rand()%(123)+2;
            *(oy+i)=rand()%(39)+1;
            key=0;
            if(*(ox+i)%2!=0)
            {
                key=1;
            }
            else
            {
                for(j=0;j<=i;j++)
                {
                    if(ox[i]==ox[j]&&oy[i]==oy[j])
                    {
                        key=1;
                        break;
                    }
                    else if(ox[j]==ox[i]+1&&oy[j]==oy[i])
                    {
                        key=1;
                        break;
                    }
                    else if(ox[i]==ox[j]&&oy[i]+1==oy[j])
                    {
                        key=1;
                        break;
                    }
                    else if(ox[i]+1==ox[j]&&oy[i]+1==oy[j])
                    {
                        key=1;
                        break;
                    }
                }
            }

        }while(key==0);
        xy.X=ox[i];
        xy.Y=oy[i];
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
        obstacle();
        xy.X=ox[i]+1;
        xy.Y=oy[i];
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
        obstacle();
        xy.X=ox[i];
        xy.Y=oy[i]+1;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
        obstacle();
        xy.X=ox[i]+1;
        xy.Y=oy[i]+1;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
        obstacle();
    }
}

void eat_fruit(int *x,int *y,int *fx,int *fy,int *body_number)
{
    int i;
    for(i=0;i<fruit_total;i++)
    {
        if((*body_number)==length-1)    //avoid growing too long
        {
            break;
        }
        if(x[0]==fx[i]&&y[0]==fy[i])
        {
            (*body_number)++;
            fx[i]=0;
            fy[i]=0;
        }
        else if(x[0]+1==fx[i]&&y[0]==fy[i])
        {
            (*body_number)++;
            fx[i]=0;
            fy[i]=0;
        }
    }
}

int obstacle_crash(int *ox,int *oy,int snake_x,int snake_y)
{
    int i;
    for(i=0;i<obstacle_amount;i++)
    {
        if(ox[i]==snake_x&&oy[i]==snake_y)  //left head
        {
            return 1;
        }
        else if(ox[i]+1==snake_x&&oy[i]==snake_y)
        {
            return 1;
        }
        else if(ox[i]+1==snake_x&&oy[i]+1==snake_y)
        {
            return 1;
        }
        else if(ox[i]==snake_x&&oy[i]+1==snake_y)
        {
            return 1;
        }
        else if(ox[i]==snake_x+1&&oy[i]==snake_y)   //right head
        {
            return 1;
        }
        else if(ox[i]+1==snake_x+1&&oy[i]==snake_y)
        {
            return 1;
        }
        else if(ox[i]+1==snake_x+1&&oy[i]+1==snake_y)
        {
            return 1;
        }
        else if(ox[i]==snake_x+1&&oy[i]+1==snake_y)
        {
            return 1;
        }
    }
    return 0;
}

int snake_crash(int *x,int *y,int body_number)
{
    int i;
    for(i=1;i<body_number;i++)
    {
        if(x[0]==x[i]&&y[0]==y[i])
        {
            return 1;
        }
    }
    return 0;
}

void speed(int body_number)
{
    if(body_number>=3&&body_number<=8)      //3~8
    {
        Sleep(110);
    }
    else if(body_number>8&&body_number<14)  //9~13
    {
        Sleep(70);
    }
    else if(body_number>13&&body_number<=length)    //14~length
    {
        Sleep(45);
    }
}

int exceed_border(int *x,int *y,int body_number)
{
    if((*x)<2||(*x)>127)        //can not exceed the border while running
    {
        return 1;
    }
    else if((*y)<1||(*y)>40)
    {
        return 1;
    }

    if(*(x+body_number)>126)       //initial snake can not exceed the border
    {
        return 1;
    }
    return 0;
}

void gameover(int z)
{
    int i,j;
    COORD xy;
    xy.X=0;
    xy.Y=0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
    for(i=0;i<42;i++)
    {
        for(j=0;j<131;j++)
        {
            printf("\33[40m \33[m");
        }
        printf("\n");
        Sleep(10);
    }
    xy.X=142;
    xy.Y=16;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),xy);
    if(z==1)
    {
        printf("\33[1m\33[41mGame Over\33[0m");
    }
    else
    {
        printf("\33[41mWin\33[0m");
    }
    printf("\n");
}

int direction(COORD *current_xy,int *x,int *y,int body_number)
{
    int i,j;
    int change=0;
    static int w=0;
    static int a=1;
    static int s=0;
    static int d=0;

    if(GetAsyncKeyState(0x57))  //w(up)
    {
        if(s)       //head and tail change the direction
        {
            for(i=0;i<body_number-1;i++)
            {
                for(j=0;j<body_number-1;j++)
                {
                    change=*(x+j);
                    *(x+j)=*(x+j+1);
                    *(x+j+1)=change;
                    change=*(y+j);
                    *(y+j)=*(y+j+1);
                    *(y+j+1)=change;
                }
            }
            current_xy->X=*x;
            current_xy->Y=*y;
        }
        w=1;
        a=0;
        s=0;
        d=0;
    }
    else if(GetAsyncKeyState(0x53)) //s(down)
    {
        if(w)       //head and tail change the direction
        {
            for(i=0;i<body_number-1;i++)
            {
                for(j=0;j<body_number-1;j++)
                {
                    change=*(x+j);
                    *(x+j)=*(x+j+1);
                    *(x+j+1)=change;
                    change=*(y+j);
                    *(y+j)=*(y+j+1);
                    *(y+j+1)=change;
                }
            }
            current_xy->X=*x;
            current_xy->Y=*y;
        }
        s=1;
        a=0;
        w=0;
        d=0;
    }
    else if(GetAsyncKeyState(0x41))     //a(left)
    {
        if(d)       //head and tail change the direction
        {
            for(i=0;i<body_number-1;i++)
            {
                for(j=0;j<body_number-1;j++)
                {
                    change=*(x+j);
                    *(x+j)=*(x+j+1);
                    *(x+j+1)=change;
                    change=*(y+j);
                    *(y+j)=*(y+j+1);
                    *(y+j+1)=change;
                }
            }
            current_xy->X=*x;
            current_xy->Y=*y;
        }
        a=1;
        w=0;
        s=0;
        d=0;
    }
    else if(GetAsyncKeyState(0x44))     //d(right)
    {
        if(a)       //head and tail change the direction
        {
            for(i=0;i<body_number-1;i++)
            {
                for(j=0;j<body_number-1;j++)
                {
                    change=*(x+j);
                    *(x+j)=*(x+j+1);
                    *(x+j+1)=change;
                    change=*(y+j);
                    *(y+j)=*(y+j+1);
                    *(y+j+1)=change;
                }
            }
            current_xy->X=*x;
            current_xy->Y=*y;
        }
        d=1;
        a=0;
        w=0;
        s=0;
    }

    if(w)
    {
        current_xy->Y-=1;

         for(i=body_number;i>0;i--)
        {
            x[i]=x[i-1];
            y[i]=y[i-1];
        }
        //x[0]=current_xy->X;
        y[0]=current_xy->Y;

        return 0;
    }
    else if(s)
    {
        current_xy->Y+=1;

         for(i=body_number;i>0;i--)
        {
            x[i]=x[i-1];
            y[i]=y[i-1];
        }
        //x[0]=current_xy->X;
        y[0]=current_xy->Y;

        return 0;
    }
    else if(a)
    {
        current_xy->X-=2;

         for(i=body_number;i>0;i--)
        {
            x[i]=x[i-1];
            y[i]=y[i-1];
        }
        x[0]=current_xy->X;
        //y[0]=current_xy->Y;

        return 0;
    }
    else if(d)
    {
        current_xy->X+=2;

        for(i=body_number;i>0;i--)
        {
            x[i]=x[i-1];
            y[i]=y[i-1];
        }
        x[0]=current_xy->X;
        //y[0]=current_xy->Y;

        return 0;
    }
    return 0;
}

int object_vs_obstacle(int *fx,int *fy,int *ox,int *oy)     //avoid two things generating at the same coordinate
{
   int i,j;
   for(i=0;i<fruit_total;i++)
   {
       for(j=0;j<obstacle_amount;j++)
       {
           if(fx[i]==ox[j]&&fy[i]==oy[j])
           {
               return 1;
           }
           else if(fx[i]==ox[j]&&fy[i]==oy[j])
           {
               return 1;
           }
           else if(fx[i]==ox[j]&&fy[i]==oy[j])
           {
               return 1;
           }
           else if(fx[i]==ox[j]&&fy[i]==oy[j])
           {
               return 1;
           }
       }
   }
   return 0;
}

int game_start()
{
    srand(time(NULL));
    int i;
    int x[length]={0};  //snake's coordinate
    int y[length]={0};
    int fx[fruit_total]={0};      //fruit's coordinate
    int fy[fruit_total]={0};
    int ox[obstacle_amount]={0};
    int oy[obstacle_amount]={0};
    //int last_x,last_y;          //coordinate of last snake
    int fruit_number=rand()%10+10;
    int fruit_remain=fruit_total-fruit_number;
    int body_number=5;
    int loop_time=1;
    COORD current_xy;

    generate_obstacle(ox,oy);

    current_xy.X=(rand()%(127-2+1))*2+2;    //snake's initial position(x must be even number because of the border)
    current_xy.Y=rand()%(40-1+1)+1;
    for(i=0;i<=body_number;i++)
    {
        x[i]=current_xy.X+i*2;
        y[i]=current_xy.Y;
        if(object_vs_obstacle(x,y,ox,oy))
        {
            i=-1;
            continue;
        }
        if(i==body_number)
        {
            if(exceed_border(x,y,body_number))
            {
                i=-1;
                current_xy.X=(rand()%(127-2+1))*2+2;
                current_xy.Y=rand()%(40-1+1)+1;
            }
        }
    }

    time_t t1,t2;
    t1=time(NULL);

    for(i=0;i<fruit_number;i++)     //initial fruits position
    {
        do
        {
            fx[i]=rand()%(125)+2;
            fy[i]=rand()%(39)+1;
        }while(object_vs_obstacle(fx,fy,ox,oy));
        current_xy.X=fx[i];
        current_xy.Y=fy[i];
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),current_xy);
        fruit();
    }

    while(1)
    {
        t2=time(NULL);
        loop_time++;
        if(loop_time>9999)
        {
            loop_time=1;
        }
        information(body_number,loop_time,t1,t2);
        for(i=0;i<=body_number;i++)
        {
            current_xy.X=x[i];
            current_xy.Y=y[i];
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),current_xy);
            if(i<body_number)
            {
                snake();
            }
            else
            {
                printf("\33[47m  \33[m");
            }
        }

        current_xy.X=x[0];
        current_xy.Y=y[0];
        /*last_x=x[body_number-1];
        last_y=y[body_number-1];*/

        direction(&current_xy,x,y,body_number);
        eat_fruit(x,y,fx,fy,&body_number);

        if(loop_time%50==0)
        {
            generate_fruit(fx,fy,&fruit_remain,ox,oy);
        }
        if(snake_crash(x,y,body_number))
        {
            return 1;
        }
        if(obstacle_crash(ox,oy,x[0],y[0]))
        {
            return 1;
        }
        if(exceed_border(x,y,body_number))
        {
            return 1;
        }
        if(body_number==25)
        {
            return 0;
        }
        /*if(last_x!=x[body_number-1]||last_y!=y[body_number-1])      //if snake is moving or not
        {
            current_xy.X=last_x;
            current_xy.Y=last_y;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),current_xy);
            printf("\33[47m  \33[m");
        }*/

        speed(body_number);
    }
    return 0;
}

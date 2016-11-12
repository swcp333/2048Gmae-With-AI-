#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void InitCheker(int checker[16],int *nowFree,int *score)
{
    int addPlace1=rand()%16;    //0~15  4   0123(4)
    int addPlace2=rand()%15;    //0~14  6   0123(4)56(7)
    if(addPlace2>=addPlace1)
        ++addPlace2;
    int addNum1=rand()%10<9?2:4;
    int addNum2=rand()%10<9?2:4;
    int i;
    for(i=0; i<=15; i++)
    {
        if(i==addPlace1)
        {
            checker[i]=addNum1;
        }
        else if(i==addPlace2)
        {
            checker[i]=addNum2;
        }
        else
        {
            checker[i]=0;
        }
    }
    *nowFree=14;
    *score=0;
    printf("已初始化游戏\n");
}

void PrintChecker(int checker[4][4],int *score)
{
    int i,j;
    printf("\n当前得分：%8d",*score);
    for(i=0; i<4; ++i)
    {
        printf("\n█████████████████\n█");
        for(j=0; j<4; ++j)
        {
            if(checker[i][j]!=0)
            {
                printf("%5d █",checker[i][j]);
            }
            else
            {
                printf("      █");
            }
        }
    }
    printf("\n█████████████████\n\n> ");
}

int CheckerUp(int checker[4][4],int *nowFree,int *score)
{
    int i,j,k;
    int temp;
    int flag=0;
    int move=0;
    for(j=0; j<4; ++j)
    {
        for(i=1; i<4; ++i)       //跳过最上排
        {
            if(checker[i][j]==0)    //如果某位是0，则跳过
                continue;
            else
            {
                for(k=i; checker[k-1][j]==0 && k!=0; --k);
                //现在k的值是：紧邻上一个非0元素的位置（或边界）
                if(k==0 || checker[k-1][j]!=checker[i][j])
                {
                    //k在边界，或者当前值与上一个非0元素不等，则跳到到边界（或紧邻处）
                    if(k==i)
                        continue;
                    temp=checker[k][j];
                    checker[k][j]=checker[i][j];
                    checker[i][j]=temp;
                    flag=0;
                }
                else  //如果相等，则合并
                {
                    if(flag==0)
                    {
                        checker[k-1][j]*=2;
                        checker[i][j]=0;
                        ++(*nowFree);
                        flag=1; //标记已经合并
                        *score+=checker[k-1][j];
                    }
                    else
                    {
                        temp=checker[k][j];
                        checker[k][j]=checker[i][j];
                        checker[i][j]=temp;
                        flag=0;
                    }
                }
                ++move;
            }
        }
        flag=0;
    }
    return move;
}

int CheckerDown(int checker[4][4],int *nowFree,int *score)
{
    int i,j,k;
    int temp;
    int flag=0;
    int move=0;
    for(j=0; j<4; ++j)
    {
        for(i=2; i>-1; --i)
        {
            if(checker[i][j]==0)
                continue;
            else
            {
                for(k=i; checker[k+1][j]==0 && k!=3; ++k);
                if(k==3 || checker[k+1][j]!=checker[i][j])
                {
                    if(k==i)
                        continue;
                    temp=checker[k][j];
                    checker[k][j]=checker[i][j];
                    checker[i][j]=temp;
                    flag=0;
                }
                else
                {
                    if(flag==0)
                    {
                        checker[k+1][j]*=2;
                        checker[i][j]=0;
                        ++(*nowFree);
                        flag=1;
                        *score+=checker[k+1][j];
                    }
                    else
                    {
                        temp=checker[k][j];
                        checker[k][j]=checker[i][j];
                        checker[i][j]=temp;
                        flag=0;
                    }
                }
                ++move;
            }
        }
        flag=0;
    }
    return move;
}

int CheckerLeft(int checker[4][4],int *nowFree,int *score)
{
    int i,j,k;
    int temp;
    int flag=0;
    int move=0;
    for(i=0; i<4; ++i)
    {
        for(j=1; j<4; ++j)
        {
            if(checker[i][j]==0)
                continue;
            else
            {
                for(k=j; checker[i][k-1]==0 && k!=0; --k);
                if(k==0 || checker[i][k-1]!=checker[i][j])
                {
                    if(k==j)
                        continue;
                    temp=checker[i][k];
                    checker[i][k]=checker[i][j];
                    checker[i][j]=temp;
                    flag=0;
                }
                else
                {
                    if(flag==0)
                    {
                        checker[i][k-1]*=2;
                        checker[i][j]=0;
                        ++(*nowFree);
                        flag=1;
                        *score+=checker[i][k-1];
                    }
                    else
                    {
                        temp=checker[i][k];
                        checker[i][k]=checker[i][j];
                        checker[i][j]=temp;
                        flag=0;
                    }
                }
                ++move;
            }
        }
        flag=0;
    }
    return move;
}

int CheckerRight(int checker[4][4],int *nowFree,int *score)
{
    int i,j,k;
    int temp;
    int flag=0;
    int move=0;
    for(i=0; i<4; ++i)
    {
        for(j=2; j>-1; --j)
        {
            if(checker[i][j]==0)
                continue;
            else
            {
                for(k=j; checker[i][k+1]==0 && k!=3; ++k);
                if(k==3 || checker[i][k+1]!=checker[i][j])
                {
                    if(k==j)
                        continue;
                    temp=checker[i][k];
                    checker[i][k]=checker[i][j];
                    checker[i][j]=temp;
                    flag=0;
                }
                else
                {
                    if(flag==0)
                    {
                        checker[i][k+1]*=2;
                        checker[i][j]=0;
                        ++(*nowFree);
                        flag=1;
                        *score+=checker[i][k+1];
                    }
                    else
                    {
                        temp=checker[i][k];
                        checker[i][k]=checker[i][j];
                        checker[i][j]=temp;
                        flag=0;
                    }
                }
                ++move;
            }
        }
        flag=0;
    }
    return move;
}

void AddNum(int checker[16],int *nowFree)
{
    if(*nowFree==0)
    {
        printf("GAME OVER!\n输入 R 键重开游戏");
    }
    else
    {
        int addPlace=rand()%(*nowFree);     //在剩余方块中随机选出一个位置
        int addNum=rand()%10<9?2:4;
        int i;
        for(i=0; i<=15; i++)
        {
            if(checker[i]==0)
            {
                if(addPlace--==0)
                {
                    break;
                }
            }
        }
        checker[i]=addNum;
        --(*nowFree);
        printf("第%d行，第%d列，生成了%d\n",i/4+1,i%4+1,addNum);
    }
}

int main()
{
    srand(time(0));
    int checker[4][4];
    int nowFree;
    int score;
    char input;
    InitCheker(checker,&nowFree,&score);
    PrintChecker(checker,&score);
    scanf(" %c",&input);

    while(1)
    {
        system("cls");      //清屏
        switch(input)
        {
        case 'w':
            if(CheckerUp(checker,&nowFree,&score)!=0)
                AddNum(checker,&nowFree);
            else
                printf("该方向无可移动数字\n");
            break;
        case 's':
            if(CheckerDown(checker,&nowFree,&score)!=0)
                AddNum(checker,&nowFree);
            else
                printf("该方向无可移动数字\n");
            break;
        case 'a':
            if(CheckerLeft(checker,&nowFree,&score)!=0)
                AddNum(checker,&nowFree);
            else
                printf("该方向无可移动数字\n");
            break;
        case 'd':
            if(CheckerRight(checker,&nowFree,&score)!=0)
                AddNum(checker,&nowFree);
            else
                printf("该方向无可移动数字\n");
            break;

        case 'R':
            InitCheker(checker,&nowFree,&score);
            break;
        }
        PrintChecker(checker,&score);
        scanf(" %c",&input);
    }
    return 0;
}

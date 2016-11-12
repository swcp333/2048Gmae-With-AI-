#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define OUTER_INIT(c) (c<2?(j=0):(i=0))	//外层循环-初始化
#define OUTER_COND(c) (c<2?(j<4):(i<4))	//外层循环-条件
#define OUTER_DO(c) (c<2?(++j):(++i))	//外层循环-执行
#define INNER_INIT(c) (c<3?c<2?c<1?(i=1):(i=2):(j=1):(j=2))		//内层循环-初始化
#define INNER_COND(c) (c<3?c<2?c<1?(i<4):(i>-1):(j<4):(j>-1))	//内层循环-条件
#define INNER_DO(c) (c<3?c<2?c<1?(++i):(--i):(++j):(--j))		//内层循环-执行
#define SKIP_INIT(c) (c<2?(k=i):(k=j))	//跳过空位循环-初始化
#define SKIP_DO(c) (c%2==0?(--k):(++k))	//跳过空位循环-执行
#define IS_BORDER(c) (c%2==0?0:3)			//到达边界（跳出）
#define IS_OVERLAP(c) (c<2?(k==i):(k==j))	//元素重叠（不交换）
#define NEXT_X(c) (c<2?c<1?k-1:k+1:i)	//下一个非空元素-行
#define NEXT_Y(c) (c<3?c<2?j:k-1:k+1)	//下一个非空元素-列
#define SWAP_X(c) (c<2?k:i)		//交换位置-行
#define SWAP_Y(c) (c<2?j:k)		//交换位置-列

int node;
int bestChoose;
int depthMAX;

void InitCheker(int checker[16],int *nowFree)
{
    int addPlace1=rand()%16;    //0~15  4   0123(4)
    int addPlace2=rand()%15;    //0~14  6   0123(4)56(7)
    if(addPlace2>=addPlace1)
        ++addPlace2;
    int addNum1=rand()%10<9?2:4;
    int addNum2=rand()%10<9?2:4;
    int i;
    for(i=0; i<=15; i++)
        if(i==addPlace1)
            checker[i]=addNum1;
        else if(i==addPlace2)
            checker[i]=addNum2;
        else
            checker[i]=0;
    *nowFree=14;
    printf("初始化完成，R重开，T自动，Enter键步进");
}

void PrintChecker(int checker[4][4])
{
    int i,j;
    for(i=0; i<4; ++i)
    {
        printf("\n█████████████████\n█");
        for(j=0; j<4; ++j)
            if(checker[i][j]!=0)
                printf("%5d █",checker[i][j]);
            else
                printf("      █");
    }
    printf("\n█████████████████\n\n> ");
}

int CheckerMove(int checker[4][4],int c,int *score)
{
    int i,j,k,temp,flag=0,move=0;
    for(OUTER_INIT(c); OUTER_COND(c); OUTER_DO(c))
    {
        for(INNER_INIT(c); INNER_COND(c); INNER_DO(c))	//跳过第一排
        {
            if(checker[i][j]==0)    //如果某位是0，则跳过
                continue;
            else
            {
                for(SKIP_INIT(c); checker[NEXT_X(c)][NEXT_Y(c)]==0 && k!=IS_BORDER(c); SKIP_DO(c));
                if(k==IS_BORDER(c) || checker[NEXT_X(c)][NEXT_Y(c)]!=checker[i][j])   //此时k的值是：紧邻上一个非0元素的位置（或边界）
                {
                    if(IS_OVERLAP(c))   //k在边界，或者当前值与上一个非0元素不等，则跳到到边界（或紧邻处）
                        continue;
                    temp=checker[SWAP_X(c)][SWAP_Y(c)];
                    checker[SWAP_X(c)][SWAP_Y(c)]=checker[i][j];
                    checker[i][j]=temp;
                    flag=0;
                }
                else  //如果相等，则合并
                {
                    if(flag==0)
                    {
                        checker[i][j]=0;
                        checker[NEXT_X(c)][NEXT_Y(c)]*=2;
                        (*score)+=checker[NEXT_X(c)][NEXT_Y(c)];
                        flag=1; //表示已经合并过一次
                    }
                    else
                    {
                        temp=checker[SWAP_X(c)][SWAP_Y(c)];
                        checker[SWAP_X(c)][SWAP_Y(c)]=checker[i][j];
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

int isFinish(int checker[4][4])
{
    int i,j;
    for(i=0; i<4; i++)
        for(j=0; j<3; j++)
            if(checker[i][j]==checker[i][j+1] || checker[j][i]==checker[j+1][i])
                return 1;
    return 0;
}

void AddNum(int checker[16],int *nowFree)
{
    int addPlace=rand()%(*nowFree);     //在剩余方块中随机选出一个位置
    int addNum=rand()%10<9?2:4;
    int i;
    for(i=0; i<=15; i++)
        if(checker[i]==0 && addPlace--==0)
            break;
    checker[i]=addNum;
    --(*nowFree);
}

int Estimate (int checker[4][4])
{
    int diff = 0;
    int sum = 0;
    int i,j;
    for(i=0; i<4; i++)
        for(j=0; j<3; j++)
        {
            sum += checker[i][j];
            diff += abs(checker[i][j]-checker[i][j+1]);
            diff += abs(checker[j][i]-checker[j+1][i]);
        }
    return (sum*4-diff)*2;
};

int StratAI(int checker[4][4],int *nowFree)
{
    depthMAX = 3;
    int i,temp;
    while (1)
    {
        node = 0;
        if(AI(checker,0)==-1)
            return 1;
        if (node >= 10000 || depthMAX >= 8)
            break;
        depthMAX += 1;
    }
    CheckerMove(checker,bestChoose,&temp);
    *nowFree=0;
    for(i=0; i<16; ++i)
        if(checker[0][i]==0)
            ++(*nowFree);
    AddNum(checker,nowFree);
    if(*nowFree==0 && isFinish(checker)==0)  //检测是否结束
        return 1;
    return 0;
}

int AI(int checker[4][4],int deep)
{
    ++node;  //经历的节点数
    int eChecker[4][4];
    if (deep >= depthMAX)
        return Estimate(checker);//统计当前局面得分
    int i,j;
    int free;
    int tempScore;
    int nowScroe=0;
    int bestScroe = -1;
    for (i = 0; i < 4; ++i) 	//尝试四个方向
    {
        memcpy(eChecker,checker, 16*4);     //拷贝一份原始数据进行评估
        if (CheckerMove(eChecker,i,&nowScroe)!=0)     //该方向可以移动
        {
            tempScore = 0;
            free = 0;
            for (j = 0; j < 16; ++j)
            {
                if (eChecker[0][j] == 0)
                {
                    ++free;
                    eChecker[0][j] = 2;
                    tempScore += AI(eChecker, deep+1) * 0.9;
                    eChecker[0][j] = 4;
                    tempScore += AI(eChecker, deep+1) * 0.1;
                    eChecker[0][j] = 0;
                }
            }

            if (free!=0)
                tempScore /= free;
            else
                tempScore = -99999999;

            if (nowScroe + tempScore > bestScroe)
            {
                bestScroe = nowScroe + tempScore;
                if (deep == 0)        //修改最优方向
                    bestChoose=i;
            }
        }
    }
    return bestScroe;
}

int main()
{
    srand(time(0));
    int checker[4][4];
    int nowFree,max;
    char input;
    InitCheker(checker,&nowFree);
    PrintChecker(checker);
    input=getchar();
    while(1)
    {
        system("cls");
        if(input!='R')
        {
            if(input=='T')
                while(StratAI(checker,&nowFree)!=1)
                {
                    PrintChecker(checker);
                    system("cls");
                }
            else if(StratAI(checker,&nowFree)==1)
                printf("游戏结束");
        }
        else
        {
            InitCheker(checker,&nowFree);
            getchar();
        }
        PrintChecker(checker);
        input=getchar();
    }
    return 0;
}

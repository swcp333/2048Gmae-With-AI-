#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define OUTER_INIT(c) (c<2?(j=0):(i=0))	//���ѭ��-��ʼ��
#define OUTER_COND(c) (c<2?(j<4):(i<4))	//���ѭ��-����
#define OUTER_DO(c) (c<2?(++j):(++i))	//���ѭ��-ִ��
#define INNER_INIT(c) (c<3?c<2?c<1?(i=1):(i=2):(j=1):(j=2))		//�ڲ�ѭ��-��ʼ��
#define INNER_COND(c) (c<3?c<2?c<1?(i<4):(i>-1):(j<4):(j>-1))	//�ڲ�ѭ��-����
#define INNER_DO(c) (c<3?c<2?c<1?(++i):(--i):(++j):(--j))		//�ڲ�ѭ��-ִ��
#define SKIP_INIT(c) (c<2?(k=i):(k=j))	//������λѭ��-��ʼ��
#define SKIP_DO(c) (c%2==0?(--k):(++k))	//������λѭ��-ִ��
#define IS_BORDER(c) (c%2==0?0:3)			//����߽磨������
#define IS_OVERLAP(c) (c<2?(k==i):(k==j))	//Ԫ���ص�����������
#define NEXT_X(c) (c<2?c<1?k-1:k+1:i)	//��һ���ǿ�Ԫ��-��
#define NEXT_Y(c) (c<3?c<2?j:k-1:k+1)	//��һ���ǿ�Ԫ��-��
#define SWAP_X(c) (c<2?k:i)		//����λ��-��
#define SWAP_Y(c) (c<2?j:k)		//����λ��-��

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
    printf("��ʼ����ɣ�R�ؿ���T�Զ���Enter������");
}

void PrintChecker(int checker[4][4])
{
    int i,j;
    for(i=0; i<4; ++i)
    {
        printf("\n����������������������������������\n��");
        for(j=0; j<4; ++j)
            if(checker[i][j]!=0)
                printf("%5d ��",checker[i][j]);
            else
                printf("      ��");
    }
    printf("\n����������������������������������\n\n> ");
}

int CheckerMove(int checker[4][4],int c,int *score)
{
    int i,j,k,temp,flag=0,move=0;
    for(OUTER_INIT(c); OUTER_COND(c); OUTER_DO(c))
    {
        for(INNER_INIT(c); INNER_COND(c); INNER_DO(c))	//������һ��
        {
            if(checker[i][j]==0)    //���ĳλ��0��������
                continue;
            else
            {
                for(SKIP_INIT(c); checker[NEXT_X(c)][NEXT_Y(c)]==0 && k!=IS_BORDER(c); SKIP_DO(c));
                if(k==IS_BORDER(c) || checker[NEXT_X(c)][NEXT_Y(c)]!=checker[i][j])   //��ʱk��ֵ�ǣ�������һ����0Ԫ�ص�λ�ã���߽磩
                {
                    if(IS_OVERLAP(c))   //k�ڱ߽磬���ߵ�ǰֵ����һ����0Ԫ�ز��ȣ����������߽磨����ڴ���
                        continue;
                    temp=checker[SWAP_X(c)][SWAP_Y(c)];
                    checker[SWAP_X(c)][SWAP_Y(c)]=checker[i][j];
                    checker[i][j]=temp;
                    flag=0;
                }
                else  //�����ȣ���ϲ�
                {
                    if(flag==0)
                    {
                        checker[i][j]=0;
                        checker[NEXT_X(c)][NEXT_Y(c)]*=2;
                        (*score)+=checker[NEXT_X(c)][NEXT_Y(c)];
                        flag=1; //��ʾ�Ѿ��ϲ���һ��
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
    int addPlace=rand()%(*nowFree);     //��ʣ�෽�������ѡ��һ��λ��
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
    if(*nowFree==0 && isFinish(checker)==0)  //����Ƿ����
        return 1;
    return 0;
}

int AI(int checker[4][4],int deep)
{
    ++node;  //�����Ľڵ���
    int eChecker[4][4];
    if (deep >= depthMAX)
        return Estimate(checker);//ͳ�Ƶ�ǰ����÷�
    int i,j;
    int free;
    int tempScore;
    int nowScroe=0;
    int bestScroe = -1;
    for (i = 0; i < 4; ++i) 	//�����ĸ�����
    {
        memcpy(eChecker,checker, 16*4);     //����һ��ԭʼ���ݽ�������
        if (CheckerMove(eChecker,i,&nowScroe)!=0)     //�÷�������ƶ�
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
                if (deep == 0)        //�޸����ŷ���
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
                printf("��Ϸ����");
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

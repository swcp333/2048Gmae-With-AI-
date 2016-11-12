#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void InitCheker(int checker[16],int *nowFree,int *score,int *max)
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
    *score=0;
    *max=addNum1>addNum2?addNum1:addNum2;
    printf("��ʼ����ɣ�wsad������R�ؿ�\n");
}

void PrintChecker(int checker[4][4],int *score,int *max)
{
    int i,j;
    printf("\n��ǰ�÷֣�%8d  ���ֵΪ��%4d",*score,*max);
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

int CheckerMove(int checker[4][4],int *nowFree,int *score,int *max,int c)
{
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
                        checker[NEXT_X(c)][NEXT_Y(c)]*=2;
                        checker[i][j]=0;
                        ++(*nowFree);
                        flag=1; //��ʾ�Ѿ��ϲ���һ��
                        *score+=checker[NEXT_X(c)][NEXT_Y(c)];
                        if(checker[NEXT_X(c)][NEXT_Y(c)]>*max)
                            *max=checker[NEXT_X(c)][NEXT_Y(c)];
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
    printf("��%d�У���%d�У�������%d��ʣ��%d����λ\n",i/4+1,i%4+1,addNum,*nowFree);
}

int main()
{
    srand(time(0));
    int checker[4][4];
    int nowFree,score,max;
    char input;
    int choose;
    InitCheker(checker,&nowFree,&score,&max);
    PrintChecker(checker,&score,&max);
    scanf(" %c",&input);
    while(1)
    {
        system("cls");      //����
        switch(input)
        {
        case 'w':
            choose=0;
            break;
        case 's':
            choose=1;
            break;
        case 'a':
            choose=2;
            break;
        case 'd':
            choose=3;
            break;
        case 'R':
            InitCheker(checker,&nowFree,&score,&max);
        default:
            choose=-1;
        }
        if(choose>-1)
        {
            if(CheckerMove(checker,&nowFree,&score,&max,choose)!=0)  //���ƶ�ʱ�����ƶ�����������Ԫ��
            {
                if(max==2048)
                    printf("�Ѵﵽ2048\n");
                AddNum(checker,&nowFree);
            }
            else
                printf("�÷����޿��ƶ�\n");
            if(nowFree==0 && isFinish(checker)==0)  //����Ƿ������nowFree��Ϊ0ʱ������ִ��isFinish()
                printf("��Ϸ����");
        }
        PrintChecker(checker,&score,&max);
        scanf(" %c",&input);
    }
    return 0;
}

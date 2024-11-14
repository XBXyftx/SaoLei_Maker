#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<easyx.h>

#define ROW 10          //���г���
#define COL 10
#define MINENUM 10      //�׵�����
#define IMGSIZE 40      //ͼƬ��С

//����ͼƬ��Դ
IMAGE imgs[12];
void loadResource()
{
    for (int i = 0; i < 12; i++)
    {
        char imgPath[20] = { 0 };
        sprintf_s(imgPath, "./imgs/%d.png", i);
        loadimage(&imgs[i], imgPath, IMGSIZE, IMGSIZE);
    }
}

//��������
void show(int map[][COL]);
void init(int map[][COL]);
void draw(int map[][COL]);
void mouseMsg(ExMessage* msg, int map[][COL]);
void boomBlank(int map[][COL], int row, int col);
int judge(int map[][COL], int row, int col);

int main()
{
    //��������
    initgraph(ROW * IMGSIZE, COL * IMGSIZE);
    //����ɨ�׵�ͼ
    int map[ROW][COL] = { 0 };

    init(map);

    while (1)
    {
        //������Ϣ
        ExMessage msg;
        while (peekmessage(&msg, EX_MOUSE))
        {
            switch (msg.message)
            {
            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
                mouseMsg(&msg, map);
                //���֮���ж�
                int ret = judge(map, msg.y / IMGSIZE, msg.x / IMGSIZE);
                if (ret == -1)
                {
                    draw(map);
                    int select = MessageBox(GetHWnd(), "again��", "��", MB_OKCANCEL);
                    //����һ��
                    if (select == IDOK)
                    {
                        init(map);
                    }
                    //�Ƴ�
                    else {
                        exit(0);
                    }
                }
                else if (ret == 1)
                {
                    int select = MessageBox(GetHWnd(), "again��", "Ӯ", MB_OKCANCEL);
                    //����һ��
                    if (select == IDOK)
                    {
                        init(map);
                    }
                    //�Ƴ�
                    else {
                        exit(0);
                    }
                }
                break;
            }
        }
        draw(map);
    }

    //show(map);

    //��ֹ����
    getchar();
    return 0;
}

//չʾ��ͼ
void show(int map[][COL])
{
    for (int i = 0; i < ROW; i++)
    {
        for (int k = 0; k < COL; k++)
        {
            printf("%2d", map[i][k]);
        }
        printf("\n");
    }
}

//��ʼ������
void init(int map[][COL])
{
    loadResource();

    //�������������
    srand((unsigned)time(NULL));

    //��mapȫ����ʼ��Ϊ0
    memset(map, 0, sizeof(int) * ROW * COL);

    //��������ף���-1��ʾ
    for (int i = 0; i < MINENUM;)
    {
        int r = rand() % ROW;
        int c = rand() % COL;

        //��������ܻ�����ظ�
        if (map[r][c] == 0)
        {
            map[r][c] = -1;
            i++;
        }
    }

    //������Ϊ���ĵ����ݼ�1
    for (int i = 0; i < ROW; i++)
    {
        for (int k = 0; k < COL; k++)
        {
            //�ҵ��ף���ʼ�����Ź��񲢼�1
            if (map[i][k] == -1)
            {
                for (int r = i - 1; r <= i + 1; r++)
                {
                    for (int c = k - 1; c <= k + 1; c++)
                    {
                        //����Χ���ݼ�һ
                        if ((r >= 0 && r < ROW) && (c >= 0 && c < COL) && map[r][c] != -1)
                        {
                            map[r][c]++;
                        }
                    }
                }
            }
        }
    }

    //���ܸ���
    for (int i = 0; i < ROW; i++)
    {
        for (int k = 0; k < COL; k++)
        {
            map[i][k] += 20;
        }
    }
}

//����ͼƬ
void draw(int map[][COL])
{
    //����map�������������Ӧ��ͼ
    for (int i = 0; i < ROW; i++)
    {
        for (int k = 0; k < COL; k++)
        {
            //������
            if (map[i][k] >= 0 && map[i][k] <= 8)
            {
                putimage(k * IMGSIZE, i * IMGSIZE, &imgs[map[i][k]]);
            }
            //����
            else if (map[i][k] == -1)
            {
                putimage(k * IMGSIZE, i * IMGSIZE, &imgs[9]);
            }
            //�����û�д򿪵ĸ���
            else if (map[i][k] >= 19 && map[i][k] <= 28)
            {
                putimage(k * IMGSIZE, i * IMGSIZE, &imgs[10]);
            }
            //��ǵĸ���
            else if (map[i][k] >= 39)
            {
                putimage(k * IMGSIZE, i * IMGSIZE, &imgs[11]);
            }
        }
    }
}

//����������
void mouseMsg(ExMessage* msg, int map[][COL])
{
    //�ȸ�����������������Ӧ��������±�
    int r = msg->y / IMGSIZE;
    int c = msg->x / IMGSIZE;

    //����򿪸���
    if (msg->message == WM_LBUTTONDOWN)
    {
        //�����Ч
        if (map[r][c] >= 19 && map[r][c] <= 28)
        {
            map[r][c] -= 20;
            //����ǲ��ǿհ׸���
            boomBlank(map, r, c);
        }
    }
    //�Ҽ���Ǹ���
    else
    {
        //�Ƿ��ܹ����
        if (map[r][c] >= 19 && map[r][c] <= 28)
        {
            map[r][c] += 20;
        }
        //ȡ�����
        else if (map[r][c] >= 39)
        {
            map[r][c] -= 20;
        }
    }
}

//����հ׸��������������пհ׸��Ӻ���
void boomBlank(int map[][COL], int row, int col)
{
    //�ж�row col�ǲ��ǿհ׸���
    if (map[row][col] == 0)
    {
        for (int r = row - 1; r <= row + 1; r++)
        {
            for (int c = col - 1; c <= col + 1; c++)
            {
                if ((r >= 0 && r < ROW) && (c >= 0 && c < COL) && map[r][c] >= 19 && map[r][c] <= 28)
                {
                    map[r][c] -= 20;
                    boomBlank(map, r, c);
                }
            }
        }
    }
}

//��Ϸ�������� ���˷���-1 û��������0 Ӯ�˷���1
int judge(int map[][COL], int row, int col)
{
    //�㵽����Ϸ����������
    if (map[row][col] == -1 || map[row][col] == 19)
    {
        return -1;
    }
    //���������Ϸ������Ӯ��
    int cnt = 0;
    for (int i = 0;i < ROW;i++)
    {
        for (int k = 0;k < COL;k++)
        {
            //ͳ�ƴ򿪵ĸ�����
            if (map[i][k] >= 0 && map[i][k] <= 8)
            {
                ++cnt;
            }
        }
    }
    if (ROW * COL - MINENUM == cnt)
    {
        return 1;
    }
    return 0;
}
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<easyx.h>

#define ROW 10          //行列常量
#define COL 10
#define MINENUM 10      //雷的数量
#define IMGSIZE 40      //图片大小

//定义图片资源
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

//方法声明
void show(int map[][COL]);
void init(int map[][COL]);
void draw(int map[][COL]);
void mouseMsg(ExMessage* msg, int map[][COL]);
void boomBlank(int map[][COL], int row, int col);
int judge(int map[][COL], int row, int col);

int main()
{
    //创建窗口
    initgraph(ROW * IMGSIZE, COL * IMGSIZE);
    //定义扫雷地图
    int map[ROW][COL] = { 0 };

    init(map);

    while (1)
    {
        //处理消息
        ExMessage msg;
        while (peekmessage(&msg, EX_MOUSE))
        {
            switch (msg.message)
            {
            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
                mouseMsg(&msg, map);
                //点击之后判断
                int ret = judge(map, msg.y / IMGSIZE, msg.x / IMGSIZE);
                if (ret == -1)
                {
                    draw(map);
                    int select = MessageBox(GetHWnd(), "again？", "寄", MB_OKCANCEL);
                    //再来一吧
                    if (select == IDOK)
                    {
                        init(map);
                    }
                    //推出
                    else {
                        exit(0);
                    }
                }
                else if (ret == 1)
                {
                    int select = MessageBox(GetHWnd(), "again？", "赢", MB_OKCANCEL);
                    //再来一吧
                    if (select == IDOK)
                    {
                        init(map);
                    }
                    //推出
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

    //防止闪退
    getchar();
    return 0;
}

//展示地图
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

//初始化数据
void init(int map[][COL])
{
    loadResource();

    //设置随机数种子
    srand((unsigned)time(NULL));

    //把map全部初始化为0
    memset(map, 0, sizeof(int) * ROW * COL);

    //随机设置雷，用-1表示
    for (int i = 0; i < MINENUM;)
    {
        int r = rand() % ROW;
        int c = rand() % COL;

        //随机数可能会出现重复
        if (map[r][c] == 0)
        {
            map[r][c] = -1;
            i++;
        }
    }

    //把以雷为中心的数据加1
    for (int i = 0; i < ROW; i++)
    {
        for (int k = 0; k < COL; k++)
        {
            //找到雷，开始遍历九宫格并加1
            if (map[i][k] == -1)
            {
                for (int r = i - 1; r <= i + 1; r++)
                {
                    for (int c = k - 1; c <= k + 1; c++)
                    {
                        //对周围数据加一
                        if ((r >= 0 && r < ROW) && (c >= 0 && c < COL) && map[r][c] != -1)
                        {
                            map[r][c]++;
                        }
                    }
                }
            }
        }
    }

    //加密格子
    for (int i = 0; i < ROW; i++)
    {
        for (int k = 0; k < COL; k++)
        {
            map[i][k] += 20;
        }
    }
}

//绘制图片
void draw(int map[][COL])
{
    //根据map里面的数据贴对应的图
    for (int i = 0; i < ROW; i++)
    {
        for (int k = 0; k < COL; k++)
        {
            //不是雷
            if (map[i][k] >= 0 && map[i][k] <= 8)
            {
                putimage(k * IMGSIZE, i * IMGSIZE, &imgs[map[i][k]]);
            }
            //是雷
            else if (map[i][k] == -1)
            {
                putimage(k * IMGSIZE, i * IMGSIZE, &imgs[9]);
            }
            //输出还没有打开的格子
            else if (map[i][k] >= 19 && map[i][k] <= 28)
            {
                putimage(k * IMGSIZE, i * IMGSIZE, &imgs[10]);
            }
            //标记的格子
            else if (map[i][k] >= 39)
            {
                putimage(k * IMGSIZE, i * IMGSIZE, &imgs[11]);
            }
        }
    }
}

//鼠标操作数据
void mouseMsg(ExMessage* msg, int map[][COL])
{
    //先根据鼠标点击坐标求出对应的数组的下标
    int r = msg->y / IMGSIZE;
    int c = msg->x / IMGSIZE;

    //左键打开格子
    if (msg->message == WM_LBUTTONDOWN)
    {
        //点击有效
        if (map[r][c] >= 19 && map[r][c] <= 28)
        {
            map[r][c] -= 20;
            //检测是不是空白格子
            boomBlank(map, r, c);
        }
    }
    //右键标记格子
    else
    {
        //是否能够标记
        if (map[r][c] >= 19 && map[r][c] <= 28)
        {
            map[r][c] += 20;
        }
        //取消标记
        else if (map[r][c] >= 39)
        {
            map[r][c] -= 20;
        }
    }
}

//点击空白格子连环爆开所有空白格子和雷
void boomBlank(int map[][COL], int row, int col)
{
    //判断row col是不是空白格子
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

//游戏结束条件 输了返回-1 没结束返回0 赢了返回1
int judge(int map[][COL], int row, int col)
{
    //点到雷游戏结束，输了
    if (map[row][col] == -1 || map[row][col] == 19)
    {
        return -1;
    }
    //点完格子游戏结束，赢了
    int cnt = 0;
    for (int i = 0;i < ROW;i++)
    {
        for (int k = 0;k < COL;k++)
        {
            //统计打开的格子数
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
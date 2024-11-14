#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<easyx.h>

#define ROW 10
#define COL 10
#define MINENUM 10
#define IMGSIZE 40

void loadResource();
void draw(int map[][COL]);
IMAGE imgs[12];
int main() {
	int map[ROW][COL] = { 0 };
	//��������
	initgraph(ROW * IMGSIZE, COL * IMGSIZE);
	//���������
	srand(time(NULL));
	loadResource();
	for (int i = 0; i < MINENUM;) {
		int c = rand() % ROW;
		int r = rand() % COL;
		if (map[r][c] == 0) {
			map[r][c] = -1;
			i++;
		}

	}

	for (int i = 0; i < ROW; i++) {
		for (int k = 0; k < COL;k++) {

			//�ҵ���
			if (map[i][k]==-1) {

				//�����Ź���
				for (int r = i - 1;r <= i + 1;r++) {
					for (int c = k - 1;c <= k + 1;c++) {
						//����Χ���ݼ�1
						if ((r >= 0 && r < ROW) && (c >= 0 && c < COL) && map[r][c] != -1) {
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

	for (int i = 0;i < ROW;i++) {
		for (int k = 0;k < COL;k++) {
			//��Ⱦ�򿪺��������ͼƬ
			if (map[i][k] >= 0 && map[i][k] <= 8) {
				putimage(k * IMGSIZE, i * IMGSIZE, &imgs[map[i][k]]);
			}
			//��Ⱦ��ͼƬ
			else if (map[i][k]==-1) {
				putimage(k * IMGSIZE, i * IMGSIZE, &imgs[9]);
			}
			//��Ⱦδ�򿪵ĸ���
			else if (map[i][k] >= 19 && map[i][k] <= 28) {
				putimage(k * IMGSIZE, i * IMGSIZE, &imgs[10]);
			}
			//��Ⱦ����ǵĸ���
			else if(map[i][k]>=39) {
				putimage(k * IMGSIZE, i * IMGSIZE, &imgs[11]);
			}
		}
	}

	

	draw(map);

	getchar();
	return 0;
}

void draw(int map[][COL]) {

	for (int i = 0; i < ROW; i++) {

		for (int j = 0; j < COL; j++) {
			printf("%2d", map[i][j]);
		}
		printf("\n");

	}

}

void loadResource() {
	for (int i = 0; i < 12; i++) {
		char imgPath[20] = { 0 };
		sprintf_s(imgPath, "./imgs/%d.png", i);
		loadimage(&imgs[i], imgPath, IMGSIZE, IMGSIZE);
	}
}
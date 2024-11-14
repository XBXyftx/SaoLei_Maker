#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include<easyx.h>

#define ROW 10
#define COL 10
#define MINENUM 10

void draw(int map[][COL]);

int main() {
	int map[ROW][COL] = { 0 };

	//随机数种子
	srand(time(NULL));

	for (int i = 0; i < MINENUM;) {
		int c = rand() % ROW;
		int r = rand() % COL;
		if (map[r][c] == 0) {
			map[r][c] = -1;
			i++;
		}
		
	}

	draw(map);

	return 0;
}

void draw(int map[][COL]) {

	for (int i = 0; i < ROW; i++) {

		for (int j = 0; j < COL; j++) {
			printf("%2d", map[i][j]);
		}
		printf("/n");

	}

}
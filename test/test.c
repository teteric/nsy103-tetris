#define BOARD_HEIGHT 25
#define BOARD_WIDTH 12


#include <stdio.h>

int board[BOARD_WIDTH][BOARD_HEIGHT];

void swap(int * i, int * j) {
	int temp;
	temp = *i;
	*i = *j;
	*j = temp;
}

void b_remove_line(int line) {
	int x, y;

	if (line >= BOARD_HEIGHT) {
		return;
	}

	if (line > 0) {
		for (y = line; y > 0; y--) {
			for (x = 0; x < BOARD_WIDTH; x++) {
				board[x][y] = board[x][y-1];
			}
		}
	}
	// Set the first line (y = 0) to 0
	for (x = 0; x < BOARD_WIDTH; x++) {
		board[x][0] = 0;
	}
}

void b_insert_line(int data[BOARD_WIDTH]) {
	int x, y;
	for (y = 0; y < BOARD_HEIGHT - 1; y++) {
		for (x = 0; x < BOARD_WIDTH; x++) {
			board[x][y] = board[x][y+1];
		}
	}

	for (x = 0; x < BOARD_WIDTH; x++) {
		board[x][BOARD_HEIGHT-1] = data[x];
	}
}



int test1() {
	int i, j;
	i = 4;
	j = 5;
	printf("\nTest1\n\n");
	printf("i is set to %d\n", i);
	printf("j is set to %d\n", j);
	swap(&i, &j);
	if (i == 5 && j == 4) {
		printf("SUCCESS\n");
		return 0;
	}
	else {
		return 1;
	}
}

int test2() {
	int skyline[BOARD_WIDTH];
	int x, y;

	for (x = 0; x < BOARD_WIDTH; x++) {
		for (y = 0; y < BOARD_HEIGHT; y++) {
			board[x][y] = 0;
		}
	}

	board[0][5] = 1;
	board[1][9] = 1;
	board[2][16] = 1;
	board[5][5] = 1;
	board[8][2] = 1;
	board[9][15] = 1;

	// for (x = 0; x < BOARD_WIDTH; x++) {
	// 	board[x][x] = 1;
	// }

	for (x = 0; x < BOARD_WIDTH; x++) {
		y = 0;
		while (board[x][y] == 0 && y < BOARD_HEIGHT-1) {
			y++;
		}
		skyline[x] = y;
	}
 
	for (x = 0; x < BOARD_WIDTH; x++) {
		printf("%d\n", skyline[x]);
	}

	return 0;
}

int test3(void) {
	int line[BOARD_WIDTH];
	int x;

	bzero(board, sizeof(board));

	for (x = 0; x < BOARD_WIDTH; x++) {
		line[x] = (x+1)%2;
	}
	b_insert_line(line);
	for (x = 0; x < BOARD_WIDTH; x++) {
		line[x] = x%2;
	}
	b_insert_line(line);
	if (
		board[ 0][23] == 1 &&
		board[ 1][23] == 0 &&
		board[ 2][23] == 1 &&
		board[ 3][23] == 0 &&
		board[ 4][23] == 1 &&
		board[ 5][23] == 0 &&
		board[ 6][23] == 1 &&
		board[ 7][23] == 0 &&
		board[ 8][23] == 1 &&
		board[ 9][23] == 0 &&
		board[10][23] == 1 &&
		board[11][23] == 0 &&
		board[ 0][24] == 0 &&
		board[ 1][24] == 1 &&
		board[ 2][24] == 0 &&
		board[ 3][24] == 1 &&
		board[ 4][24] == 0 &&
		board[ 5][24] == 1 &&
		board[ 6][24] == 0 &&
		board[ 7][24] == 1 &&
		board[ 8][24] == 0 &&
		board[ 9][24] == 1 &&
		board[10][24] == 0 &&
		board[11][24] == 1
		) {
		printf("b_insert_line works\n\n");
	} else {
		printf("b_insert_line failed!\n\n");
	}
}

int main(void) {
	int result = 0;
	result += test1();

	result += test2();

	result += test3();
	return result;
}
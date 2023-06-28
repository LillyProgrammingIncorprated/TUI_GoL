// TO DO:
// SWITCH THE PLACE OF THE VALUE CHECK & ERROR CHECK IN EDITING FUNCTIONS
// PUT REASONABLE LIMITS ON ALL SIZES 
// RENAME SOME FUNCTIONS
// 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h> 

// global values for the row length and column length
// can't pass 2D arrays as arrays otherwise
// and this is the easiest method 
// although this means the functions only work for 2D char arrays of this size
// thank you C, very cool
int rlen = 0; //row length
int clen = 0; // column length

// just returns the nth power of 10
int ten_pow(int n) { 
	int a = 1;
	for (int i = 0; i < n; i++)
		a *= 10;
	return a;
}

// does the cool printing function
void grid_print_2(char grid[rlen][clen], int lp) { // finish here 
	// it doesn't need to print if it's the first time so this is a check
	static int lrl = 0;
	static char first_print = 1; 
	if (first_print) {
		for (int i = 0; i < rlen; i++) {
			for (int j = 0; j < clen; j++) {
				printf("\033[%dm%d\033[0m ",
					(grid[i][j] == 1) ? 32 : 31,
					grid[i][j]);
			}
			printf("\n");
		}
	}
	else {
		// Goes up #lp (which is always the row count+1 in this program)
		// this allows us to override all that has been written
		printf("\033[%dA\r", lp);  
		for (int i = 0; i < rlen; i++) {
			for (int j = 0; j < clen; j++) { 
				printf("\033[%dm%d\033[0m ",
					(grid[i][j] == 1) ? 32 : 31,
					grid[i][j],
					lp);
			}
			printf("\n", i);
		}
	}
	lrl += 1;
	first_print = 0;
	printf("\n%d", lrl);
}

// prints the entire game grid without ANSI esc codes
// except for colours but that's easy
void grid_print(char grid[rlen][clen]) {
	for (int i = 0; i < rlen; i++) {
		for (int j = 0; j < clen; j++) {
			printf("\033[%dm%d\033[0m ",
				(grid[i][j] == 1) ? 32 : 31,
				grid[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

// converts inputted str into an int and store it in ret
int str2num(char* str, int len, int *ret) {
	*(int*)ret = 0;
	for (int i = 0; i < len; i++) {
		if ((*(str+i) - '0') > 10) {
			printf("Invalid character, please only enter decimal numerical characters\n");
			return -1;
		}
		*(int*)ret += ((*(str+i) - '0') * ten_pow(len-i-1));
	}
	return 0;
}

#define MAX_IN_BUFF_SIZE 8
// gets a string SAFELY(ish) to turn into a number and store it in num
int getnum(int *num) {
	int nlen; 
	int len = 2;
	char* str = malloc(len * sizeof(char));
	char n;
	// ends when a newline char is inputted
	// because that's how you normally input
	for (nlen = 0; (n = getc(stdin)) != '\n'; nlen++) {
		if (nlen > (len-1)) {
			if ((len*2) <= MAX_IN_BUFF_SIZE) {	
				len += 2;
				str = realloc(str, len);
			}
			else 
				printf("Input digit limit exceeded\n");
		}
		if (n == EOF) {
			printf("EOF while reading\n");
			return -1;
		}
		*(char*)(str+nlen) = (char)n;
	}
	str = realloc(str, (nlen * sizeof(char)));

	if (str2num(str, nlen, num) == -1) {
			printf("Error while converting input into a number\n");
			return -1;
	}
	free(str);
	return 0;
}

// manual cell editing function
// Start work on putting errors before value checks from here cunt
void man_cell_edit(char grid[rlen][clen]) {
	// an error storing int, X coords, Y coords, and the state you want the cell to be 
	int err, x, y, alive; 
get_manual_answers:
	printf("Input the X coordinate of the cell: ");
	err = getnum(&x);
	if (x < 0 || x >= clen) {
		printf("Invalid X coordinate\n");
		goto get_manual_answers; 
	}
	else if (err == -1) {
		printf("Error getting value\n");
		goto get_manual_answers;
	}
	printf("Input the Y coordinate of the cell: ");
	err = getnum(&y);
	if (y < 0 || y >= rlen) {
		printf("Invalid Y coordinates\n");
		goto get_manual_answers;
	}
	else if (err == -1) {
		printf("Error getting value\n");
		goto get_manual_answers;
	}
	printf("Do you want the cell to be alive (1) or dead (0): ");
	err = getnum(&alive);
	if (alive != 1 && alive != 0) {
		printf("Invalid cell state\n");
		goto get_manual_answers;
	}

	grid[y][x] = (alive == 1) ? 1 : 0;
}

// start working on directional input here
void holine(char grid[rlen][clen]) {
	int err, x, y, line_len;
	int lr;
get_holine_in:
	printf("Should the line be drawn Left to Right (1) or Right to Left (2): ");
	err = getnum(&lr);
	if (err == -1) {
		printf("Error getting value\n");
		goto get_holine_in;
	}
	if (lr != 1 && lr != 2) {
		printf("Invalid option\n");
		goto get_holine_in;
	}

	printf("Input the X coordinates: ");
	err = getnum(&x);
	if (err == -1) {
		printf("Error getting value\n");
		goto get_holine_in;
	}
	if (x >= clen || x < 0) {
		printf("Invalid X coordinates for line beginning\n");
		goto get_holine_in;
	}
	
	printf("Input the Y coordinates: ");
	err = getnum(&y);
	if (err == -1) {
		printf("Error getting value\n");
		goto get_holine_in;
	}
	if (y >= rlen || y < 0) {
		printf("Invalid Y coordinates for line beginning\n");	
		goto get_holine_in;
	}
	
	printf("Input the line length: ");
	err = getnum(&line_len);
	if (err == -1) {
		printf("Error getting value\n");
		goto get_holine_in;
	}
	if (line_len > clen || line_len < 0) {
		printf("Invalid length for horizontal line\n");
		goto get_holine_in;
	}
	
	if (lr == 1) {
		for(int i = 0; i < line_len; i++)
			grid[y][(x+i) % clen] = 1;
	}
	else {
		for(int i = 0; i < line_len; i++)
			grid[y][((x-i)<0) ? (clen - i) : ((x-i) % clen)] = 1;
	}
}

void verline(char grid[rlen][clen]) {
	int err, lr, x, y, line_len;
get_verline_in:
	printf("Should the line be drawn Top to Bottom (1) or Bottom to Top (2): ");
	err = getnum(&lr);
	if (err == -1) {
		printf("Error getting value\n");
		goto get_verline_in;
	}
	if (lr != 1 && lr != 2) {
		printf("Invalid option\n");
		goto get_verline_in;
	}
	
	printf("Input the X coordinates: ");
	err = getnum(&x);
	if (err == -1) {
		printf("Error getting value\n");
		goto get_verline_in; 
	}
	if (x >= clen || x < 0) {
		printf("Invalid X coordinates for line beginning\n");
		goto get_verline_in;
	}
	
	printf("Input the Y coordinates: ");
	err = getnum(&y);
	if (err == -1) {
		printf("Error getting value\n");
		goto get_verline_in;	
	}
	if (y >= rlen || y < 0) {
		printf("Invalid Y coordinates for line beginning\n");
		goto get_verline_in;
	}
	
	printf("Input the line length: ");
	err = getnum(&line_len);
	if (err == -1) {
		printf("Error getting value\n");
		goto get_verline_in;	
	}
	if (line_len > rlen || line_len < 0) {
		printf("Invalid length for horizontal line\n");
		goto get_verline_in;
	}

	if (lr == 1) {
		for(int i = 0; i < line_len; i++)
			grid[(y+i) % rlen][x] = 1;
	}
	else {
		for(int i = 0; i < line_len; i++)
			grid[((y-i)<0) ? (rlen - i) : ((y-i) % rlen)][x] = 1;
	}
}


void dialine(char grid[rlen][clen]) {
	int err, lr, x, y, line_len;
get_dialine_in:	
	printf("Should the line be drawn Top Left to Bottom Right (1) or Top Right to Bottom Left (2): ");
	err = getnum(&lr);
	if (err == -1) {
		printf("Error getting value\n");
		goto get_dialine_in;
	}
	if (lr != 1 && lr != 2) {
		printf("Invalid option\n");
		goto get_dialine_in;
	}
	
	printf("Input the X coordinates: ");
	err = getnum(&x);
	if (err == -1) {
		printf("Error getting value\n");
		goto get_dialine_in;
	}
	if (x >= clen || x < 0) {
		printf("Invalid X coordinates for line beginning\n");
		goto get_dialine_in;
	}
	
	printf("Input the Y coordinates: ");
	err = getnum(&y);
	if (err == -1) {
		printf("Error getting value\n");
		goto get_dialine_in;
	}
	if (y >= rlen || y < 0) {
		printf("Invalid Y coordinates for line beginning\n");
		goto get_dialine_in;
	}
	
	printf("Input the line length: ");
	err = getnum(&line_len);
	int maxL = (int)sqrt((double)(clen*clen + rlen*rlen));
	if (err == -1) {
		printf("Error getting value\n");
		goto get_dialine_in;
	}
	if (line_len > maxL || line_len < 0) {
		printf("Invalid length for horizontal line\n");
		goto get_dialine_in;
	}

	if (lr == 1) {
		for(int i = 0; i < line_len; i++)
			grid[(y+i) % rlen][(x+i) % clen] = 1;
	}
	else {
		int i = 0;
		while (i < line_len) {
			grid[(y+i) % rlen][((x-i)<0) ? (clen - i) : ((x-i) % clen)] = 1;
			i += 1;
		}
	}
}

void draw_cell_line(char grid[rlen][clen]) {
	int err, ans;
get_line_ans:
	printf("To make a horizontal line, input 1 | To make a vertical line, input 2 | To make a diagonal line, input 3\nInput: ");
	err = getnum(&ans);
	if (ans < 0 || ans > 3) {
		printf("Invalid line type input\n");
		goto get_line_ans;
	} 
	else if (err == -1) {
		printf("Error getting value\n");
		goto get_line_ans;
	}
	switch(ans) {
		case (1):
			holine(grid);			
			break;
		case (2):
			verline(grid);
			break;
		case(3):
			dialine(grid);
			break;
		default:
			printf("Erroneous value for line type\n");
			return;
	}
}

void edit_grid(char grid[rlen][clen]) {
	while (1) {
		grid_print(grid);
		printf("To exit, input 0 | To edit a single cell, input 1 | To draw a line, input 2\nInput: ");
		int err, ans;
		err = getnum(&ans);	
		if (err == -1)
			printf("Error getting value\n");
		else if (ans == 0)
			break;
		else if (ans == 1)
			man_cell_edit(grid);
		else if (ans == 2)
			draw_cell_line(grid);
		else
			printf("Erroneous input(%d) during editing\n", ans);
	}
}


void game_of_life(char arr[rlen][clen]) {
	int AN;
	for (int i = 0; i < rlen; i++) {
		for (int j = 0; j < clen; j++) {
			AN = arr[i][(j+1) % clen] // check the right neighbor
				+ arr[i][((j-1)<0) ? (clen-1) : ((j-1)%clen)] // check the left neighbor
				+ arr[(i+1)%rlen][j] // check the top neighbor
				+ arr[((i-1)<0) ? (rlen-1) : ((i-1) % rlen)][j]; // and lastly check the bottom neighbor
			switch(AN) {
				case (0):
					arr[i][j] = 0;
					break;
				case (1):
					arr[i][j] = 0;
					break;
				case(2):
					arr[i][j] = arr[i][j];
					break;
				case(3):
					arr[i][j] = 1;
					break;
				case(4): 
					arr[i][j] = 0;
					break;
				default:
					printf("Error during neighbor count at i=%d and j=%d\n", i, j);
			}
		}
	}
	grid_print_2(arr, rlen+1);
	usleep(500000);
}

int main(int argc, char ** argv) {
	if (argc != 4) {
		printf("usage: ./GoL [row_count] [column count] [iteration count]");
		return -1;
	}
	int ic;

	// get the row & column size for the player grid, and number of iterations
	int err = str2num(argv[1], strlen(argv[1]), &rlen);
	err += str2num(argv[2], strlen(argv[2]), &clen);
	err += str2num(argv[3], strlen(argv[3]), &ic);	
	// if either number is smaller than or equal 0
	// that's no good
	if (err < 0) {
		printf("Error getting cl arguments\n");
		return -1;
	}
	if (!(rlen > 0 && clen > 0 && ic > 0)) {
		printf("Ivalid grid size parameters or iteration count\n");
		return -1;
	}

	// create a local array
	// that really should have been a global pointer
	char grid[rlen][clen];

	// set each cell to 0
	for (int i = 0; i < rlen; i++) {
		for (int j = 0; j < clen; j++) {
			grid[i][j] = 0;

		}
	}
	
	edit_grid(grid);
	printf("\033[2J\033[HThe Game: \n");
	for (int i = 0; i < ic; i++)
		game_of_life(grid);

	return 0;
}

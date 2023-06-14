#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rlen = 5;
int clen = 5;

int ten_pow(int n) { 
    int a = 1;  
    for (int i = 0; i < n; i++)
        a *= 10;
    return a;
}

size_t str2num(char* str, int len) {
    size_t ret = 0;
    for (int i = 0; i < len; i++) {
        if ((*(str+i) - '0') > 10) {
            printf("Invalid character, please only enter decimal numerical characters\n");
            return 0;
        }
        ret += ((*(str+i) - '0') * ten_pow(len-i-1));
    }
    return ret;
}

int getnum(int *a) {
	size_t len = 2;
    int i;
    char* str = malloc(len * sizeof(char));
    char n;
    for (i = 0; (n = getc(stdin)) != '\n'; i++) {
        if (i > (len-1)) {
            len *= 2;
            str = realloc(str, len);
        }
		if (n == EOF) {
			printf("EOF error\n");
			return -1;
		}
        *(char*)(str+i) = (char)n;
    }
    str = realloc(str, (i * sizeof(char*)));

    *(int*)a = str2num(str, i);
    free(str);
    return 0;
}
void grid_print_2(char grid[rlen][clen], int lp) { // finish here
    static int first_print = 1;
    if (first_print) {
        for (int i = 0; i < rlen; i++) {
            for (int j = 0; j < clen; j++) {
                printf("\033[%dm%c\033[0m ",
                    (grid[i][j] == 1) ? 32 : 31,
                    grid[i][j]);
            }
            printf("\n");
        }
    }
    else { 
        printf("\033[%dA", lp);
        for (int i = 0; i < rlen; i++) {
            for (int j = 0; j < clen; j++) { //WIP
                printf("\033[%dm%c\033[0m ",
                    (grid[i][j] == 1) ? 32 : 31,
                    grid[i][j],
                    lp);
            }
            printf("\n");
        }
    }
    first_print = 0;
    printf("\n");
}

int main() {
	char arr[5][5] = {"12345", "12345", "12345", "12345", "12345"};
	char arr_2[5][5] = {"23456", "23456", "23456", "23456", "23456"};
	
	grid_print_2(arr, 0);
	grid_print_2(arr_2, 6);
	int a;
	getnum(&a);
	printf("%d \n", a);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int var = 1;
	int status;
	
	int r = fork();
	if(r == 0) {
		var++;
		r = fork();
		if(r == 0) { // process X
			var++;
			exit(var);
		} else { // process Y
			if(wait(&status) != -1) {
				if(WIFEXITED(status)) {
					printf("A %d ", WEXITSTATUS(status));
				}
			}
			var += 2; 
		}
	} else { // process Z
		printf("W %d ", var);
		if(wait(&status) != -1) {
			if(WIFEXITED(status)) {
				printf("B %d ", WEXITSTATUS(status));
			} 
		}
	}
	printf("C %d ", var);
	return 0;
}
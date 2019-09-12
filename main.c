#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>

#define WRITE 1
#define READ 0

int main() {
	int request[2];
	int response[2];

	pipe(request);
	pipe(response);

	pid_t pid = fork();
	if(pid == 0) {
		// Child
		close(request[WRITE]);
		close(response[READ]);

		// stdout --> response[WRITE]
		dup2(response[WRITE], STDOUT_FILENO);
		close(response[WRITE]);

		// request[READ] --> stdin
		dup2(request[READ], STDIN_FILENO);
		close(request[READ]);

		int max_read = 1024;
		char buffer[max_read];
		fgets(buffer, max_read, stdin);
		printf("%s", buffer);

		fflush(stdout);
		close(STDOUT_FILENO);
		close(STDIN_FILENO);
		_exit(0);
	} else {
		// Parent
		close(request[READ]);
		close(response[WRITE]);

		write(request[WRITE], "Eat more vegetables\n", strlen("Eat more vegetables\n"));
		close(request[WRITE]);

		size_t n = 1023;
		char msg[n + 1];
		n = read(response[READ], msg, n);
		msg[n] = 0 ;
		close(response[READ]);

		printf("%s", msg);
		exit(0);
	}
}

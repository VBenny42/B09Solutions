#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int turn = 0;
void handler(int code) {
    if(turn == 0) {
        fprintf(stderr, "First\n");
        turn = 1;
        /* D */
        sleep(15);
}
else {
        fprintf(stderr, "Second\n");
        kill(getpid(), SIGQUIT);
    }
    fprintf(stderr, "Here\n");
}
int main(void) {
    char buf[20];
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    /* A */
    printf("a\n");
    sigaction(SIGTERM, &sa, NULL);
    /* B */
    printf("b\n");
    sleep(100);
    fprintf(stderr, "Done\n");
    /* C */
    printf("c\n");
    sleep(100);
return 0; }
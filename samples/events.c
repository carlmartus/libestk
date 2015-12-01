#include <stdio.h>
#include <SDL/SDL.h>
#include "estk.h"

static void loop_frame(float fr) {
}

static void callback_quit(int sdlkey, int down) {
	esGame_loopQuit();
}

static void callback_message(int sdlkey, int down) {
	printf("Message! down=%d\n", down);
}

int main(int argc, char **argv) {

	esGame_init(400, 300);
	esLogVersion();

	esGame_registerKey(SDLK_q, callback_quit);
	printf("Press 'q' to exit\n");

	esGame_registerKey(SDLK_m, callback_message);
	printf("Press 'm' to print message\n");

	esGame_loop(loop_frame, NULL, 0);
	return 0;
}


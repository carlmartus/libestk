#include <stdio.h>
#include <SDL2/SDL.h>
#include "estk.h"

static int capturedMouse;

static void loop_frame(float fr) {
}

static void callback_quit(int sdlkey, int down) {
	esGame_loopQuit();
}

static void callback_message(int sdlkey, int down) {
	printf("Message! down=%d\n", down);
}

static void callback_mouse(int button, int down,
		int x, int y, int dx, int dy) {

	esLog(ES_INFO,
			"Mouse : x=%d, y=%d, dx=%d, dy=%d, buttonId=%d, buttonDown=%d",
			x, y, dx, dy, button, down);
}

static void callback_capture(int sdlkey, int down) {
	if (down) {
		capturedMouse = 1 - capturedMouse;
		printf("Capture mouse=%d\n", capturedMouse);
		esGame_captureMouse(capturedMouse);
	}
}

int main(int argc, char **argv) {
	capturedMouse = 0;

	esGame_init("Events sample", 400, 300);
	esLogVersion();

	esGame_registerKey(SDLK_q, callback_quit);
	printf("Press 'q' to exit\n");

	esGame_registerKey(SDLK_m, callback_message);
	printf("Press 'm' to print message\n");

	esGame_registerKey(SDLK_c, callback_capture);
	printf("Press 'c' to toggle mouse capture\n");

	esGame_registerMouse(callback_mouse);

	esGame_loop(loop_frame, NULL, 0);
	return 0;
}


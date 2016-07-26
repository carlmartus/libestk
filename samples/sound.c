#include <stdio.h>
#include <math.h>
#include "estk.h"

static esSound sfx;
static esMusic mus;

static void frame(float time) {
	static int frame_count = 0;
	if (++frame_count > 300) esGame_loopQuit();
}

static void loop_exit() {
	esSound_free(&sfx);
	printf("Good bye!\n");
	esGame_quit();
}

int main() {

	esGame_init(400, 300);
	esLogVersion();

	if (!esSound_create(&sfx, "samples/resources/sound.ogg")) {
		printf("Cannot load sound file\n");
		return 1;
	}

	if (!esMusic_create(&mus, "samples/resources/music.ogg")) {
		printf("Cannot load music file\n");
		return 1;
	}

	printf("Play music\n");
	esMusic_play(&mus);

	printf("Play sound\n");
	esSound_play(&sfx);

	esGame_loop(frame, loop_exit, 0);
	return 0;
}


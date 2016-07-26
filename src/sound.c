#include "estk.h"
#include <SDL2/SDL_mixer.h>

esErr esSound_create(esSound *sn, const char *file_name) {
	sn->chunk = Mix_LoadWAV(file_name);
	return (sn->chunk) ? ES_OK : ES_FAIL;
}

void esSound_free(esSound *sn) {
	Mix_FreeChunk(sn->chunk);
	sn->chunk = NULL;
}

void esSound_play(esSound *sn) {
	Mix_PlayChannel(-1, sn->chunk, 0);
}

esErr esMusic_create(esMusic *mu, const char *file_name) {
	mu->music = Mix_LoadMUS(file_name);
	return (mu->music) ? ES_OK : ES_FAIL;
}

void esMusic_free(esMusic *mu) {
	Mix_FreeMusic(mu->music);
	mu->music = NULL;
}

void esMusic_play(esMusic *mu) {
	Mix_PlayMusic(mu->music, -1);
}

void esMusic_halt(void) {
	Mix_HaltMusic();
}


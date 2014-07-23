#include "estk.h"
#include <SDL/SDL_mixer.h>

int esSoundLoad(esSound *sn, const char *file_name) {
	sn->chunk = Mix_LoadWAV(file_name);
	return sn->chunk == 0;
}

void esSoundUnLoad(esSound *sn) {
	Mix_FreeChunk(sn->chunk);
	sn->chunk = NULL;
}

void esSoundPlay(esSound *sn) {
	Mix_PlayChannel(-1, sn->chunk, 0);
}

int esMusicLoad(esMusic *mu, const char *file_name) {
	mu->music = Mix_LoadMUS(file_name);
	return mu->music == 0;
}

void esMusicUnLoad(esMusic *mu) {
	Mix_FreeMusic(mu->music);
	mu->music = NULL;
}

void esMusicPlay(esMusic *mu) {
	Mix_PlayMusic(mu->music, -1);
}

void esMusicHalt(void) {
	Mix_HaltMusic();
}


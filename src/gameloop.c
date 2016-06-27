#include "estk.h"
#include <string.h>
#include <SDL/SDL.h>
#ifdef ES_OPT_GLEW
#include <GL/glew.h>
#else
#include <GL/gl.h>
#endif

#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#endif

#define MAX_KEYS 2000

static int window_w, window_h;

static int loop_run;
static int key_regs = 0;
static struct {
	void (*callback) (int key, int down);
} keys[MAX_KEYS];

static void (*mouse_callback) (int button, int down, int x, int y) = 0;

void esGame_init(int screen_width, int screen_height) {
	window_w = screen_width;
	window_h = screen_height;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_SetVideoMode(screen_width, screen_height, 0, SDL_OPENGL);

#ifdef ES_OPT_AUDIO
	if (Mix_OpenAudio(22050, AUDIO_S16, 1, 4096)) {
		esLog(ES_ERRO, "Cannot open audio\n");
	}
#endif

#ifdef ES_OPT_GLEW
	glewInit();
#endif
}

static void event_key(int sdlkey, int down) {
	if (keys[sdlkey].callback) {
		keys[sdlkey].callback(sdlkey, down);
	}
}

static void event_mouse(int mouseButton, int down, int x, int y) {
	if (mouse_callback) {
		mouse_callback(mouseButton, down, x, y);
	}
}

static void events(void) {
	SDL_Event event;

	static const int button_map[] = {
		[SDL_BUTTON_LEFT] = 1,
		[SDL_BUTTON_RIGHT] = 2,
		[SDL_BUTTON_MIDDLE] = 3,
	};

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT : loop_run = 0; break;

			case SDL_KEYDOWN :	event_key(event.key.keysym.sym, 1); break;
			case SDL_KEYUP :	event_key(event.key.keysym.sym, 0); break;

			case SDL_MOUSEMOTION : event_mouse(0, 0, event.motion.x, event.motion.y); break;

			case SDL_MOUSEBUTTONUP :
			case SDL_MOUSEBUTTONDOWN :
				event_mouse(button_map[event.button.button],
						event.button.type == SDL_MOUSEBUTTONDOWN ? 1:0,
						event.button.x, event.button.y); break;
				break;
		}
	}
}

void esGame_glSwap(void) {
	SDL_GL_SwapBuffers();
}

#ifdef EMSCRIPTEN
static Uint32 emscripten_last_frame;
static void (*emscripten_frame)(float t);
static void (*emscripten_exit)();

static void emscripten_mainloop(void) {
	if (!loop_run) {
		esMusic_halt();
		if (emscripten_exit) emscripten_exit();
		emscripten_cancel_main_loop();
		return;
	}

	Uint32 current = SDL_GetTicks();
	float pass = 0.001f * (float) (current - emscripten_last_frame);
	emscripten_last_frame = current;

	events();
	if (!loop_run) return;
	emscripten_frame(pass);
}

#endif

void esGame_loop(
		void (*frame)(float t),
		void (*exit)(),
		int frame_rate) {

	loop_run = 1;

#ifdef EMSCRIPTEN
	emscripten_frame = frame;
	emscripten_exit = exit;

	emscripten_last_frame = SDL_GetTicks();
	emscripten_set_main_loop(emscripten_mainloop, frame_rate, 0);
#else

	if (frame_rate == 0) frame_rate = 60;
	int loop_delay = 1000 / frame_rate;

	float pass = 0.1f;
	Uint32 start = SDL_GetTicks();
	while (1) {
		events();
		if (!loop_run) break;
		frame(pass);
		if (!loop_run) break;

		Uint32 diff = SDL_GetTicks() - start;

		if (diff < loop_delay) {
			SDL_Delay(loop_delay - diff);
			pass = (float) loop_delay * 0.001f;
		} else {
			pass = (float) diff * 0.001f;
		}

		start = SDL_GetTicks();
	}

#ifdef ES_OPT_AUDIO
	esMusic_halt();
#endif
	if (exit) exit();
#endif
}

void esGame_loopQuit(void) {
	loop_run = 0;
}

void esGame_registerKey(int sdlkey,
		void (*callback)(int key, int down)) {
	if (key_regs == 0) {
		memset(keys, 0, sizeof(keys));
	}
	key_regs++;

	keys[sdlkey].callback = callback;
}

void esGame_registerMouse(
		void (*callback) (int button, int down, int x, int y)) {
	mouse_callback = callback;
}

void esGame_windowSize(int *w, int *h) {
	*w = window_w;
	*h = window_h;
}

void esGame_setViewport(void) {
	glViewport(0, 0, window_w, window_h);
}

void esGame_delay(unsigned int timeMs) {
	SDL_Delay(timeMs);
}

void esGame_quit(void) {
	SDL_Quit();
}


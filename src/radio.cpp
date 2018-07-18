#include <Windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "ini.h"

SDL_Window* window;
Mix_Chunk* ptt_open, *ptt_close;

typedef struct
{
	const char* loadout;
	SDL_Keycode key;
	short vk_key;

	const char* open_path;
	bool open_enabled;

	const char* close_path;
	bool close_enabled;
} Config;
Config settings;

void load_hotkey()
{
	switch( settings.key )
	{
		case SDLK_CAPSLOCK: settings.vk_key = VK_CAPITAL; break;
		case SDLK_LCTRL:
		case SDLK_RCTRL: 	settings.vk_key = VK_CONTROL; break;
		case SDLK_LSHIFT:
		case SDLK_RSHIFT: 	settings.vk_key = VK_SHIFT; break;
		case SDLK_LALT:
		case SDLK_RALT: 	settings.vk_key = VK_MENU; break;
		case SDLK_PAUSE: 	settings.vk_key = VK_PAUSE; break;
		case SDLK_INSERT: 	settings.vk_key = VK_INSERT; break;
		case SDLK_HOME: 	settings.vk_key = VK_HOME; break;
		case SDLK_PAGEUP: 	settings.vk_key = VK_PRIOR; break;
		case SDLK_PAGEDOWN: settings.vk_key = VK_NEXT; break;
		case SDLK_DELETE: 	settings.vk_key = VK_DELETE; break;
		case SDLK_END: 		settings.vk_key = VK_END; break;
		case SDLK_SCROLLLOCK: settings.vk_key = VK_SCROLL; break;

		case SDLK_F1:	 	settings.vk_key = VK_F1; break;
		case SDLK_F2:	 	settings.vk_key = VK_F2; break;
		case SDLK_F3:	 	settings.vk_key = VK_F3; break;
		case SDLK_F4:	 	settings.vk_key = VK_F4; break;
		case SDLK_F5:	 	settings.vk_key = VK_F5; break;
		case SDLK_F6:	 	settings.vk_key = VK_F6; break;
		case SDLK_F7:	 	settings.vk_key = VK_F7; break;
		case SDLK_F8:	 	settings.vk_key = VK_F8; break;
		case SDLK_F9:	 	settings.vk_key = VK_F9; break;
		case SDLK_F10:	 	settings.vk_key = VK_F10; break;
		case SDLK_F11:	 	settings.vk_key = VK_F11; break;
		case SDLK_F12:	 	settings.vk_key = VK_F12; break;

		case SDLK_KP_DIVIDE: 	settings.vk_key = VK_DIVIDE; break;
		case SDLK_KP_MULTIPLY: 	settings.vk_key = VK_MULTIPLY; break;
		case SDLK_KP_MINUS: 	settings.vk_key = VK_SUBTRACT; break;
		case SDLK_KP_PLUS: 		settings.vk_key = VK_ADD; break;
		case SDLK_KP_ENTER: 	settings.vk_key = VK_RETURN; break;
		case SDLK_KP_0: 		settings.vk_key = VK_NUMPAD0; break;
		case SDLK_KP_1: 		settings.vk_key = VK_NUMPAD1; break;
		case SDLK_KP_2: 		settings.vk_key = VK_NUMPAD2; break;
		case SDLK_KP_3: 		settings.vk_key = VK_NUMPAD3; break;
		case SDLK_KP_4: 		settings.vk_key = VK_NUMPAD4; break;
		case SDLK_KP_5: 		settings.vk_key = VK_NUMPAD5; break;
		case SDLK_KP_6: 		settings.vk_key = VK_NUMPAD6; break;
		case SDLK_KP_7: 		settings.vk_key = VK_NUMPAD7; break;
		case SDLK_KP_8: 		settings.vk_key = VK_NUMPAD8; break;
		case SDLK_KP_9: 		settings.vk_key = VK_NUMPAD9; break;
		case SDLK_KP_PERIOD: 	settings.vk_key = VK_DECIMAL; break;

		default:
			settings.vk_key = VkKeyScanEx(settings.key, GetKeyboardLayout(0));
			break;
	}
}

bool init()
{
	if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0 )
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to init SDL", NULL);
		return false;
	}

	// Init mixer
	int flags = MIX_INIT_OGG | MIX_INIT_MP3;
	int initted = Mix_Init(flags);
	if( initted&flags != flags )
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to init mixer", NULL);
		return false;
	}
	if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1 )
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to open audio", window);
		return false;
	}

	// Create window
	window = SDL_CreateWindow("Comms",
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				300, 300,
				SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);
	if(!window)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to create window", NULL);
		return false;
	}

	return true;
}

void quit()
{
	SDL_DestroyWindow(window);

	Mix_FreeChunk(ptt_open);
	Mix_FreeChunk(ptt_close);
	Mix_CloseAudio();

	Mix_Quit();
	SDL_Quit();
}

static int handler(void* user, const char* section, const char* name, const char* value)
{
	Config* pconf = (Config*)user;

	#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	//#define GETBOOL(v) strcmp("true", v) == 0 ? true : false

	// Load values
	if( MATCH("settings", "loadout") ) {
		pconf->loadout = strdup(value);
	}
	else if( MATCH(pconf->loadout, "ptt_open") ) {
		pconf->open_path = strdup(value);
		pconf->open_enabled = true;
	}
	else if( MATCH(pconf->loadout, "ptt_close") ) {
		pconf->close_path = strdup(value);
		pconf->close_enabled = true;
	}
	else if( MATCH(pconf->loadout, "key") ) {
		pconf->key = SDL_GetKeyFromName(value);
	}
}

bool load()
{
	// Default settings
	settings.loadout = "default";
	settings.key = SDLK_t;
	settings.open_enabled = false;
	settings.close_enabled = false;

	// Load config.ini
	if( ini_parse("config.ini", handler, &settings) < 0 )
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to load config.ini", NULL);
		return false;
	}

	// Convert key
	load_hotkey();

	// Load audio files
	if( settings.open_enabled )
	{
		ptt_open = Mix_LoadWAV(settings.open_path);
		if( !ptt_open )
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "ptt_open.ogg not valid/found", NULL);
			settings.open_enabled = false;
		}
	}
	if( settings.close_enabled )
	{
		ptt_close = Mix_LoadWAV(settings.close_path);
		if( !ptt_close )
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "ptt_close.ogg not valid/found", NULL);
			settings.close_enabled = false;
		}
	}

	return true;
}


int main(int argc, char* argv[])
{
	if( !init() )
		return 0;

	if( !load() )
		return 0;

	bool pressed = false, close = false;
	
	while(!close)
	{
		close = SDL_QuitRequested();

		short key = GetAsyncKeyState(settings.vk_key);

		if( !pressed && key != 0 )
		{
			pressed = true;
			Mix_PlayChannel(-1, ptt_open, 0);
		}
		if( pressed && key == 0 )
		{
			pressed = false;
			Mix_PlayChannel(-1, ptt_close, 0);
			keybd_event( VK_CAPITAL, 0x3a, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
		}
	}

	quit();
	return 0;
}

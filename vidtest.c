/*
 *  Copyright (C) 2024 Bernhard Schelling
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "libretro.h"
#include "libretro_features_cpu.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_RETRO_SOFT_IMPLEMENTATION
#define NK_API static
#define RSDL_GFXPRIMITIVES_SCOPE static
#include "nuklear.h"
#include "nuklear_retro_soft.h"

enum { AUDIO_BUFFER_SIZE = 4096, INP_QUEUE = 16 };
enum { MOUSEMODE_ABSOLUTE, MOUSEMODE_RELATIVE, MOUSEMODE_TOUCHPAD };
static int inp_mousemode =
	#if defined(ANDROID) || defined(DBP_IOS) || defined(HAVE_LIBNX) || defined(_3DS) || defined(WIIU) || defined(VITA)
	MOUSEMODE_TOUCHPAD
	#else
	MOUSEMODE_ABSOLUTE
	#endif
	;
static enum nk_keys inp_keydown_queue[INP_QUEUE], inp_keyup_queue[INP_QUEUE];
static nk_uint inp_char_queue[INP_QUEUE], inp_keydown_num, inp_keyup_num, inp_char_num;
static double audio_remain;
static int16_t audio_buffer[AUDIO_BUFFER_SIZE];
static struct retro_system_av_info av, av_prev;

static retro_perf_get_time_usec_t time_cb = cpu_features_get_time_usec;
static retro_environment_t environ_cb;
static retro_video_refresh_t video_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static retro_log_printf_t log_cb;

#if 1 /* our gui code */
static void retro_run_gui(struct nk_context *ctx)
{
	if (nk_begin(ctx, "", nk_rect(10, 10, 500, 330), NK_WINDOW_NO_SCROLLBAR))
	{
		enum EParDar { OP_PAR, OP_DAR };
		static int prop_width, prop_height, prop_pardar;
		static double prop_par, prop_fps, prop_sample_rate;
		if (!prop_width)
		{
			prop_width = av.geometry.base_width;
			prop_height = av.geometry.base_height;
			prop_pardar = OP_PAR;
			prop_par = 1.0;
			prop_fps = av.timing.fps;
			prop_sample_rate = av.timing.sample_rate;
		}

		static const float ratio1[] = {460.f}, ratio2big[] = {100.f,350.f}, ratio2[] = {160.f, 290.f}, ratio3[] = {50.f, 200.f, 200.f};
		if (nk_tree_push(ctx, NK_TREE_TAB, "Input", NK_MAXIMIZED))
		{
			nk_layout_row(ctx, NK_STATIC, 25, 2, ratio2big);
			nk_label(ctx, "Mouse Mode:", NK_TEXT_LEFT);
			static const char *mouse_modes[] = {"Direct Controlled Mouse (Absolute)","Virtual Mouse (Relative or Controller)","Touchpad Mode (Click and Drag/Tap)"};
			inp_mousemode = nk_combo(ctx, mouse_modes, 3, inp_mousemode, 25, nk_vec2(ratio2big[1],200));
			nk_tree_pop(ctx);
		}

		if (nk_tree_push(ctx, NK_TREE_TAB, "Video", NK_MAXIMIZED))
		{
			nk_layout_row(ctx, NK_STATIC, 25, 1, ratio1);
			nk_property_int(ctx, "Width:" , 256, &prop_width,  av.geometry.max_width,  32, 1);
			nk_property_int(ctx, "Height:", 256, &prop_height, av.geometry.max_height, 32, 1);

			const int last_pardar = prop_pardar;
			nk_layout_row(ctx, NK_STATIC, 25, 3, ratio3);
			nk_label(ctx, "Set:", NK_TEXT_LEFT);
			prop_pardar = (nk_option_label(ctx, "Pixel Aspect Ratio", prop_pardar == OP_PAR)   ? OP_PAR : prop_pardar);
			prop_pardar = (nk_option_label(ctx, "Display Aspect Ratio", prop_pardar == OP_DAR) ? OP_DAR : prop_pardar);

			double new_aspect_ratio = av.geometry.aspect_ratio;
			if (prop_pardar == OP_PAR)
			{
				if (last_pardar != prop_pardar)
				{
					prop_par = av.geometry.base_width / new_aspect_ratio / av.geometry.base_height;
					if (prop_par > 0.99 && prop_par < 1.01) prop_par = 1.0;
				}
				nk_layout_row(ctx, NK_STATIC, 25, 1, ratio1);
				nk_property_double(ctx, "Pixel Aspect Ratio:", 0.001, &prop_par, 10.0, 0.1, 0.01f);
				nk_layout_row(ctx, NK_STATIC, 25, 2, ratio2);
				nk_label(ctx, "Display Aspect Ratio:", NK_TEXT_LEFT); nk_labelf(ctx, NK_TEXT_RIGHT, "%.6f", new_aspect_ratio);
				new_aspect_ratio = av.geometry.base_width / prop_par / av.geometry.base_height;
			}
			else // DAR
			{
				nk_layout_row(ctx, NK_STATIC, 25, 2, ratio2);
				nk_label(ctx, "Pixel Aspect Ratio:", NK_TEXT_LEFT);  nk_labelf(ctx, NK_TEXT_RIGHT, "%.6f", av.geometry.base_width / new_aspect_ratio / av.geometry.base_height);
				nk_layout_row(ctx, NK_STATIC, 25, 1, ratio1);
				nk_property_double(ctx, "Display Aspect Ratio" , 0.001, &new_aspect_ratio, 10.0, 0.1, 0.01f);
			}

			nk_layout_row(ctx, NK_STATIC, 25, 1, ratio1);
			if (nk_button_label(ctx, "Reset Aspect Ratio"))
			{
				new_aspect_ratio = av.geometry.base_width / (double)av.geometry.base_height;
				prop_par = 1.0;
			}

			nk_tree_pop(ctx);

			if (prop_width != av.geometry.base_width || prop_height != av.geometry.base_height || new_aspect_ratio != av.geometry.aspect_ratio)
			{
				av.geometry.base_width = prop_width;
				av.geometry.base_height = prop_height;
				av.geometry.aspect_ratio = (float)new_aspect_ratio;
			}
		}
		if (nk_tree_push(ctx, NK_TREE_TAB, "Timing", NK_MAXIMIZED))
		{
			nk_layout_row(ctx, NK_STATIC, 25, 1, ratio1);
			nk_property_double(ctx, "Frames Per Second:", 5.0, &prop_fps, 1000.0, 1.0, 1.0f);
			nk_property_double(ctx, "Audio Sample Rate:", 100.0, &prop_sample_rate, 96000.0, 100.0, 100.0f);
			nk_tree_pop(ctx);

			if (prop_fps != av.timing.fps || (prop_sample_rate != av.timing.sample_rate && !nk_input_is_mouse_down(&ctx->input, NK_BUTTON_LEFT)))
			{
				av.timing.fps = prop_fps;
				av.timing.sample_rate = prop_sample_rate;
			}
		}
	}
	nk_end(ctx);
}
#else /* show the nuklear gui examples */
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])
#include "nuklear_overview.c"
static void retro_run_gui(struct nk_context *ctx) { overview(ctx); }
#endif

static void fallback_log(enum retro_log_level level, const char *fmt, ...)
{
	(void)level;
	va_list va;
	va_start(va, fmt);
	vfprintf(stderr, fmt, va);
	va_end(va);
}

void retro_set_audio_sample(retro_audio_sample_t cb)             { }
void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb)                 { input_poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb)               { input_state_cb = cb; }
void retro_set_video_refresh(retro_video_refresh_t cb)           { video_cb = cb; }

void retro_get_system_info(struct retro_system_info *info) // #1, #6, #7 (and shutdown)
{
	memset(info, 0, sizeof(*info));
	info->library_name     = "VidTest";
	info->library_version  = "0.1";
	info->valid_extensions = NULL;
	info->need_fullpath    = false;
	info->block_extract    = true;
}

void retro_set_environment(retro_environment_t cb) // #2, #8, #9
{
	environ_cb = cb;
	bool allow_no_game = true;
	cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &allow_no_game);
}

static void RETRO_CALLCONV keyboard_event(bool down, unsigned keycode, uint32_t character, uint16_t key_modifiers)
{
	// RetroArch doesn't really ever forward character so all we can do is come up with a character code manually
	// which won't correctly interpret the users keyboard layout or most special characters...
	character = (
		(keycode >= RETROK_SPACE && keycode < RETROK_a   ) ? keycode :
		(keycode >= RETROK_a     && keycode <= RETROK_z  ) ? keycode - ((key_modifiers & RETROKMOD_SHIFT) ? 0x20 : 0) :
		(keycode >= RETROK_KP0   && keycode <= RETROK_KP9) ? keycode - (RETROK_KP0 - RETROK_0) :
		keycode >= RETROK_KP_PERIOD   ? RETROK_PERIOD :
		keycode >= RETROK_KP_DIVIDE   ? RETROK_SLASH :
		keycode >= RETROK_KP_MULTIPLY ? RETROK_ASTERISK :
		keycode >= RETROK_KP_MINUS    ? RETROK_MINUS :
		keycode >= RETROK_KP_PLUS     ? RETROK_PLUS :
		keycode >= RETROK_KP_ENTER    ? RETROK_SEMICOLON :
		keycode >= RETROK_KP_EQUALS   ? RETROK_EQUALS :
		0);

	enum nk_keys newkey =
		(keycode == RETROK_LSHIFT || keycode == RETROK_RSHIFT  ) ? NK_KEY_SHIFT :
		(keycode == RETROK_LCTRL  || keycode == RETROK_RCTRL   ) ? NK_KEY_CTRL  :
		(keycode == RETROK_RETURN || keycode == RETROK_KP_ENTER) ? NK_KEY_ENTER :
		keycode == RETROK_DELETE    ? NK_KEY_DEL :
		keycode == RETROK_TAB       ? NK_KEY_TAB :
		keycode == RETROK_BACKSPACE ? NK_KEY_BACKSPACE :
		keycode == RETROK_UP        ? NK_KEY_UP :
		keycode == RETROK_DOWN      ? NK_KEY_DOWN :
		keycode == RETROK_LEFT      ? ((key_modifiers & RETROKMOD_CTRL) ? NK_KEY_TEXT_WORD_LEFT  : NK_KEY_LEFT           ) :
		keycode == RETROK_RIGHT     ? ((key_modifiers & RETROKMOD_CTRL) ? NK_KEY_TEXT_WORD_RIGHT : NK_KEY_RIGHT          ) :
		keycode == RETROK_HOME      ? ((key_modifiers & RETROKMOD_CTRL) ? NK_KEY_TEXT_START      : NK_KEY_TEXT_LINE_START) :
		keycode == RETROK_END       ? ((key_modifiers & RETROKMOD_CTRL) ? NK_KEY_TEXT_END        : NK_KEY_TEXT_LINE_END  ) :
		keycode == RETROK_PAGEUP    ? ((key_modifiers & RETROKMOD_CTRL) ? NK_KEY_SCROLL_START    : NK_KEY_SCROLL_UP      ) :
		keycode == RETROK_PAGEDOWN  ? ((key_modifiers & RETROKMOD_CTRL) ? NK_KEY_SCROLL_END      : NK_KEY_SCROLL_DOWN    ) :
		(character == 'C' && (key_modifiers & RETROKMOD_CTRL)) ? NK_KEY_COPY            :
		(character == 'X' && (key_modifiers & RETROKMOD_CTRL)) ? NK_KEY_CUT             :
		(character == 'V' && (key_modifiers & RETROKMOD_CTRL)) ? NK_KEY_PASTE           :
		(character == 'Z' && (key_modifiers & RETROKMOD_CTRL)) ? NK_KEY_TEXT_UNDO       :
		(character == 'Y' && (key_modifiers & RETROKMOD_CTRL)) ? NK_KEY_TEXT_REDO       :
		(character == 'A' && (key_modifiers & RETROKMOD_CTRL)) ? NK_KEY_TEXT_SELECT_ALL :
		NK_KEY_NONE; /* Unhandled: NK_KEY_TEXT_INSERT_MODE : NK_KEY_TEXT_REPLACE_MODE : NK_KEY_TEXT_RESET_MODE */

	if (newkey)
	{
		enum nk_keys secondaryupkey = down ? NK_KEY_NONE :
			keycode == RETROK_LEFT      ? ((key_modifiers & RETROKMOD_CTRL) ? NK_KEY_LEFT         : NK_KEY_TEXT_WORD_LEFT ) :
			keycode == RETROK_RIGHT     ? ((key_modifiers & RETROKMOD_CTRL) ? NK_KEY_RIGHT        : NK_KEY_TEXT_WORD_RIGHT) :
			keycode == RETROK_HOME      ? ((key_modifiers & RETROKMOD_CTRL) ? NK_KEY_TEXT_START   : NK_KEY_TEXT_LINE_START) :
			keycode == RETROK_END       ? ((key_modifiers & RETROKMOD_CTRL) ? NK_KEY_TEXT_END     : NK_KEY_TEXT_LINE_END  ) :
			keycode == RETROK_PAGEUP    ? ((key_modifiers & RETROKMOD_CTRL) ? NK_KEY_SCROLL_START : NK_KEY_SCROLL_UP      ) :
			keycode == RETROK_PAGEDOWN  ? ((key_modifiers & RETROKMOD_CTRL) ? NK_KEY_SCROLL_END   : NK_KEY_SCROLL_DOWN    ) :
			character == 'C' ? NK_KEY_COPY            :
			character == 'X' ? NK_KEY_CUT             :
			character == 'V' ? NK_KEY_PASTE           :
			character == 'Z' ? NK_KEY_TEXT_UNDO       :
			character == 'Y' ? NK_KEY_TEXT_REDO       :
			character == 'A' ? NK_KEY_TEXT_SELECT_ALL :
			NK_KEY_NONE;

		if (          down && inp_keydown_num < INP_QUEUE) inp_keydown_queue[inp_keydown_num++] = newkey;
		if (         !down && inp_keyup_num   < INP_QUEUE)   inp_keyup_queue[inp_keyup_num++]   = newkey;
		if (secondaryupkey && inp_keyup_num   < INP_QUEUE)   inp_keyup_queue[inp_keyup_num++]   = secondaryupkey;
	}
	else if (character >= ' ' && down && inp_char_num < INP_QUEUE) inp_char_queue[inp_char_num++] = character;
}

void retro_init(void) // #3
{
	struct retro_log_callback logging;
	log_cb = (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging) ? logging.log : fallback_log);

	struct retro_perf_callback perf;
	if (environ_cb(RETRO_ENVIRONMENT_GET_PERF_INTERFACE, &perf) && perf.get_time_usec) time_cb = perf.get_time_usec;

	static const struct retro_keyboard_callback kc = { keyboard_event };
	environ_cb(RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK, (void*)&kc);

	static const struct retro_controller_description controllers[] = {
		{ "Joypad", RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 0) },
	};
	static const struct retro_controller_info ports[] = {
		{ controllers, 1 },
		{ NULL, 0 },
	};
	environ_cb(RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports);

	struct retro_input_descriptor desc[] =
	{
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Left" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Up" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Down" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
		{ 0 },
	};
	environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);
}

static void check_variables(void)
{
}

bool retro_load_game(const struct retro_game_info *info) // #4
{
	(void)info;

	enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
	if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
	{
		log_cb(RETRO_LOG_ERROR, "XRGB8888 is not supported.\n");
		return false;
	}

	av.geometry.base_width   = 640;
	av.geometry.base_height  = 480;
	av.geometry.max_width    = 2048;
	av.geometry.max_height   = 2048;
	av.geometry.aspect_ratio = av.geometry.base_width / (float)av.geometry.base_height;
	av.timing.fps = 60.0f;
	av.timing.sample_rate = 44100.0f;
	av_prev = av;

	retro.screen_surface = Retro_CreateRGBSurface32((int)av.geometry.base_width, (int)av.geometry.base_height, 32, 0, 0, 0, 0);
	if (!retro.screen_surface) return false;

	retro.rsdlfont = (nk_retro_Font*)calloc(1, sizeof(nk_retro_Font));
	if (!retro.rsdlfont) return false;
	retro.rsdlfont->width = 8; 
	retro.rsdlfont->height = 8; 

	nk_retro_init();
	retro.ctx.input.mouse.pos.x = (float)(500 / 2);
	retro.ctx.input.mouse.pos.y = (float)(330 / 2);
	retro.ctx.style.option.border = -2.0f;
	retro.ctx.style.option.spacing = 6.0f;
	retro.ctx.style.option.cursor_normal.data.color = nk_rgb(200,200,200);
	retro.ctx.style.option.cursor_hover.data.color = nk_rgb(255,255,255);
	retro.ctx.style.property.dec_button.touch_padding = nk_vec2(5.0f, 5.0f);
	retro.ctx.style.property.inc_button.touch_padding = nk_vec2(5.0f, 5.0f);

	check_variables();

	return true;
}

void retro_get_system_av_info(struct retro_system_av_info *info) // #5
{
	*info = av;
}

void retro_set_controller_port_device(unsigned port, unsigned device) // #10
{
	//log_cb(RETRO_LOG_INFO, "Plugging device %u into port %u.\n", device, port);
}

static void retro_run_input(struct nk_context* ctx)
{
	int mposx = (int)ctx->input.mouse.pos.x, mposy = (int)ctx->input.mouse.pos.y, mrelx = 0, mrely = 0;
	nk_input_begin(ctx);
	if (inp_mousemode == MOUSEMODE_TOUCHPAD)
	{
		bool has_press = !!input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_PRESSED);
		int16_t ptrx = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X);
		int16_t ptry = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y);

		static uint8_t last_presses, down_btn, is_move, is_tap;
		static int16_t lastx, lasty, remx, remy;
		static retro_time_t press_tick, down_tick;
		retro_time_t tick = time_cb();
		uint8_t presses = 0;
		if (has_press) for (presses = 1; presses < 3; presses++) if (!input_state_cb(0, RETRO_DEVICE_POINTER, presses, RETRO_DEVICE_ID_POINTER_PRESSED)) break;
		if (last_presses != presses)
		{
			const bool add_press = (presses > last_presses);
			if (add_press)
				press_tick = tick;
			if (!down_tick && !add_press && press_tick && (!is_move || presses))
				{ down_tick = tick; is_tap = true; down_btn = presses; nk_input_button(ctx, (enum nk_buttons)down_btn, mposx, mposy, 1); press_tick = 0; }
			else if (down_tick && (!presses || add_press))
				{ nk_input_button(ctx, (enum nk_buttons)down_btn, mposx, mposy, 0); down_tick = 0; }
			if (!presses)
				is_move = false;
			if (!last_presses || !add_press)
				lastx = ptrx, lasty = ptry, remx = remy = 0;
			last_presses = presses;
		}
		if (presses == 1 && (ptrx != lastx || ptry != lasty))
		{
			int dx = ptrx - lastx, dy = ptry - lasty;
			if (is_move || abs(dx) >= 256 || abs(dy) >= 256)
			{
				lastx = ptrx; dx += remx; remx = (int16_t)(dx % 64); mrelx = dx / 64; mposx += mrelx;
				lasty = ptry; dy += remy; remy = (int16_t)(dy % 64); mrely = dy / 64; mposy += mrely;
				is_move = true;
			}
		}
		if (!down_tick && presses && !is_move && press_tick && (tick - press_tick) >= 500000)
			{ down_tick = tick; is_tap = false; down_btn = presses - 1; nk_input_button(ctx, (enum nk_buttons)down_btn, mposx, mposy, 1); }
		else if (down_tick && is_tap && (tick - down_tick) >= 100000)
			{ nk_input_button(ctx, (enum nk_buttons)down_btn, mposx, mposy, 0); down_tick = 0; }
	}
	else
	{
		mrelx = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
		mrely = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y);
		if (inp_mousemode == MOUSEMODE_ABSOLUTE)
		{
			int ptrx = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X);
 			int ptry = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y);
			mposx = ((ptrx + 0x7fff) * av.geometry.base_width  + (av.geometry.base_width  / 2 - 1)) / 0xFFFE;
			mposy = ((ptry + 0x7fff) * av.geometry.base_height + (av.geometry.base_height / 2 - 1)) / 0xFFFE;
		}
		else if (inp_mousemode == MOUSEMODE_RELATIVE)
		{
			mposx += mrelx;
			mposy += mrely;
		}
		nk_input_button(ctx, NK_BUTTON_LEFT,   mposx, mposy, (input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT)   ? 1 : 0));
		nk_input_button(ctx, NK_BUTTON_MIDDLE, mposx, mposy, (input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_MIDDLE) ? 1 : 0));
		nk_input_button(ctx, NK_BUTTON_RIGHT , mposx, mposy, (input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_RIGHT)  ? 1 : 0));
	}

	nk_input_motion(ctx, mposx, mposy);
	if (ctx->input.mouse.pos.x < 0) ctx->input.mouse.pos.x = 0;
	if (ctx->input.mouse.pos.x > av.geometry.base_width - 10) ctx->input.mouse.pos.x = (float)(av.geometry.base_width - 10); // don't go near right edge
	if (ctx->input.mouse.pos.y < 0) ctx->input.mouse.pos.y = 0;
	if (ctx->input.mouse.pos.y > av.geometry.base_height - 1) ctx->input.mouse.pos.y = (float)(av.geometry.base_height - 1);
	ctx->input.mouse.delta.x = (float)mrelx;
	ctx->input.mouse.delta.y = (float)mrely;
	nk_input_scroll(ctx, nk_vec2(0, (input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_WHEELUP) * 1.0f) + (input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_WHEELDOWN) * -1.0f)));
	for (nk_uint q1 = 0; q1 != inp_keydown_num; q1++) nk_input_key(ctx, inp_keydown_queue[q1], 1);
	for (nk_uint q2 = 0; q2 != inp_keyup_num;   q2++) nk_input_key(ctx, inp_keyup_queue[q2], 0);
	for (nk_uint q3 = 0; q3 != inp_char_num;    q3++) nk_input_unicode(ctx, inp_char_queue[q3]);
	inp_keydown_num = inp_keyup_num = inp_char_num = 0;
	nk_input_end(ctx);
}

void retro_run(void)
{
	bool variable_updated = false;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &variable_updated) && variable_updated)
		check_variables();

	struct nk_context* ctx = &retro.ctx;
	retro_run_input(ctx);
	retro_run_gui(ctx);

	if (memcmp(&av, &av_prev, sizeof(av)))
	{
		log_cb(RETRO_LOG_INFO, "[VIDTEST] Resolution changed %ux%u @ %.3fHz AR: %.5f => %ux%u @ %.3fHz AR: %.5f\n",
			av_prev.geometry.base_width, av_prev.geometry.base_height, av_prev.timing.fps, av_prev.geometry.aspect_ratio,
			av.geometry.base_width, av.geometry.base_height, av.timing.fps, av.geometry.aspect_ratio);

		bool newfps = (av_prev.timing.fps != av.timing.fps), newmax = (av_prev.geometry.max_width != av.geometry.max_width || av_prev.geometry.max_height != av.geometry.max_height), newsr = (av_prev.timing.sample_rate != av.timing.sample_rate);
		environ_cb(((newfps || newmax || newsr) ? RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO : RETRO_ENVIRONMENT_SET_GEOMETRY), &av);

		if (av_prev.geometry.base_width != av.geometry.base_width || av_prev.geometry.base_height != av.geometry.base_height)
		{
			Retro_FreeSurface(retro.screen_surface);
			retro.screen_surface = Retro_CreateRGBSurface32((int)av.geometry.base_width, (int)av.geometry.base_height, 32, 0, 0, 0, 0);
		}

		av_prev = av;
	}

	nk_retro_render(nk_rgb(30, 30, 30));
	draw_cross(retro.screen_surface, (int)ctx->input.mouse.pos.x, (int)ctx->input.mouse.pos.y);

	video_cb(retro.screen_surface->pixels, av.geometry.base_width, av.geometry.base_height, 0);

	audio_remain += (av.timing.sample_rate / av.timing.fps);
	audio_batch_cb(audio_buffer, (size_t)(audio_remain > AUDIO_BUFFER_SIZE ? AUDIO_BUFFER_SIZE : audio_remain));
	audio_remain -= (size_t)audio_remain;
}

void retro_unload_game(void) { }

void retro_reset(void) { }

void retro_deinit(void)
{
	nk_retro_shutdown();
	Retro_FreeSurface(retro.screen_surface);
	free(retro.rsdlfont);
}

unsigned retro_api_version(void) { return RETRO_API_VERSION; }
unsigned retro_get_region(void) { return RETRO_REGION_NTSC; }
bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num) { return false; }
size_t retro_serialize_size(void) { return false; }
bool retro_serialize(void *data_, size_t size) { return false; }
bool retro_unserialize(const void *data_, size_t size) { return false; }
void *retro_get_memory_data(unsigned id) { (void)id; return NULL; }
size_t retro_get_memory_size(unsigned id) { (void)id; return 0; }
void retro_cheat_reset(void) { }
void retro_cheat_set(unsigned index, bool enabled, const char *code) { (void)index; (void)enabled; (void)code; }

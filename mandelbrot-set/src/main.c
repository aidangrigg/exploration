#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "raylib.h"

const int WIDTH = 1920 / 1.5;
const int HEIGHT = 1080 / 1.5;

const uint32_t MAX_DEPTH = 200;

const Vector2 DEFAULT_X_BOUNDARY = { -2.5, 1.0 };
const Vector2 DEFAULT_Y_BOUNDARY = { -1.0, 1.0 };

typedef struct {
	Color* pixels;

	Vector2 x_boundary;
	Vector2 y_boundary;

	double x_offset;
	double y_offset;
	double zoom;

	bool needs_recompute;
} state_t;

/* (-2.00, 0.47)), (-1.12, 1.12)*/
double scale_over_range(double val, double in_min, double in_max, double out_min, double out_max)
{
    return out_min + (val - in_min) * (out_max - out_min) / (in_max - in_min);
}

Color depth_to_color(uint32_t depth)
{
	if (depth == MAX_DEPTH)
		return BLACK;

	if (depth < MAX_DEPTH / 2)
	{
		Color c = {
			.r = scale_over_range(depth, 0, MAX_DEPTH / 2, 0, 255),
			.g = 0,
			.b = 0,
			.a = 255
		};

		return c;
	}
	else
	{
		Color c = {
			.r = 255,
			.g = scale_over_range(depth, MAX_DEPTH / 2, MAX_DEPTH, 0, 255),
			.b = 0,
			.a = 255
		};

		return c;
	}
}

Vector2 map_pixel_to_complex(int x, int y, Vector2 x_range, Vector2 y_range)
{
	Vector2 res = {
		.x = scale_over_range(x, 0, WIDTH, x_range.x, x_range.y),
		.y = scale_over_range(y, 0, HEIGHT, y_range.x, y_range.y),
	};

	return res;
}

uint32_t compute_mandelbrot_pixel_depth(Vector2 complex)
{
	double x = 0.0;
	double y = 0.0;

	double x2 = x*x;
	double y2 = y*y;

	int it = 0;
	while (x2 + y2 <= 4 && it < MAX_DEPTH)
	{
		double tmp = x2 - y2 + complex.x;
		y = 2 * x * y + complex.y;
		x = tmp;
		it++;

		x2 = x*x;
		y2 = y*y;
	}

	return it;
}

/*
  if zoom is 2 then
  original:

  e - s = diff
  diff / 2

  new diff = e - s

  s_diff = (m - s) / zoom
  e_diff = (e - m) / zoom

  s      m      e
      s     e
*/

void try_compute_mendelbrot(state_t *state)
{
	if (!state->needs_recompute)
		return;

	double x_mid = (DEFAULT_X_BOUNDARY.y - DEFAULT_X_BOUNDARY.x) / 2 + DEFAULT_X_BOUNDARY.x;
	double y_mid = (DEFAULT_Y_BOUNDARY.y - DEFAULT_Y_BOUNDARY.x) / 2 + DEFAULT_Y_BOUNDARY.x;

	state->x_boundary.x = x_mid - ((x_mid - DEFAULT_X_BOUNDARY.x) / state->zoom);;
	state->x_boundary.y = x_mid + ((DEFAULT_X_BOUNDARY.y - x_mid) / state->zoom);;

	state->y_boundary.x = y_mid - ((y_mid - DEFAULT_Y_BOUNDARY.x) / state->zoom);;
	state->y_boundary.y = y_mid + ((DEFAULT_Y_BOUNDARY.y - y_mid) / state->zoom);;

	state->x_boundary.x += state->x_offset;
	state->x_boundary.y += state->x_offset;
	state->y_boundary.x += state->y_offset;
	state->y_boundary.y += state->y_offset;

	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			Vector2 complex = map_pixel_to_complex(x, y, state->x_boundary, state->y_boundary);
			uint32_t depth = compute_mandelbrot_pixel_depth(complex);
			Color col = depth_to_color(depth);
			state->pixels[x + WIDTH * y] = col;
		}
	}

	state->needs_recompute = false;
}

void draw(state_t *state)
{
	ClearBackground(BLACK);

	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			DrawPixel(x, y, state->pixels[x + WIDTH * y]);
		}
	}
}

const double OFFSET_DELTA = 0.1;

int main()
{
	InitWindow(WIDTH, HEIGHT, "Mandelbrot Set");
	SetTargetFPS(30);

	state_t state = {
		.pixels = malloc(sizeof(Color) * WIDTH * HEIGHT),
		.x_boundary = DEFAULT_X_BOUNDARY,
		.y_boundary = DEFAULT_Y_BOUNDARY,
		.zoom = 1.0,
		.needs_recompute = true
	};

	while (!WindowShouldClose())
    {
		float mouse_wheel_move = GetMouseWheelMove();

		if (IsKeyDown(KEY_RIGHT))
		{
			state.x_offset += OFFSET_DELTA / state.zoom;
			state.needs_recompute = true;
		}

		if (IsKeyDown(KEY_LEFT))
		{
			state.x_offset -= OFFSET_DELTA / state.zoom;
			state.needs_recompute = true;
		}

		if (IsKeyDown(KEY_DOWN))
		{
			state.y_offset += OFFSET_DELTA / state.zoom;
			state.needs_recompute = true;
		}

		if (IsKeyDown(KEY_UP))
		{
			state.y_offset -= OFFSET_DELTA / state.zoom;
			state.needs_recompute = true;
		}

		if (mouse_wheel_move != 0)
		{
			printf("mouse_wheel_move: %f\n", mouse_wheel_move);
			double increment = 0.5 * state.zoom;

			state.zoom += mouse_wheel_move * increment;
			state.zoom = fmax(state.zoom, 1.0);
			printf("state.zoom: %f\n", state.zoom);

			state.needs_recompute = true;
		}

		try_compute_mendelbrot(&state);
		BeginDrawing();
		draw(&state);
		EndDrawing();
    }

	CloseWindow();
	return 0;
}

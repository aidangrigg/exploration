#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "raylib.h"

const int WIDTH = 1920;
const int HEIGHT = 1080;

const uint32_t DEPTH = 200;

typedef struct {
	uint32_t* values;
} state_t;

/* (-2.00, 0.47)), (-1.12, 1.12)*/
float scale_over_range(float val, float in_min, float in_max, float out_min, float out_max)
{
    return out_min + (val - in_min) * (out_max - out_min) / (in_max - in_min);
}

Vector2 map_pixel_to_complex(int x, int y)
{
	Vector2 res = {
		.x = scale_over_range(x, 0, WIDTH, -2.5, 1.0),
		.y = scale_over_range(y, 0, HEIGHT, -1.0, 1.0),
	};

	return res;
}

uint32_t compute_pixel(Vector2 complex)
{
	float x = 0.0;
	float y = 0.0;

	int it = 0;
	while (x * x + y * y <= 4 && it < DEPTH)
	{
		float tmp = x * x - y * y + complex.x;
		y = 2 * x * y + complex.y;
		x = tmp;
		it++;
	}

	return it;
}

void allocate(state_t *state)
{
	state->values = (uint32_t*) malloc(sizeof(uint32_t) * WIDTH * HEIGHT);
}

void compute_mendelbrot(state_t *state)
{
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			uint32_t val = compute_pixel(map_pixel_to_complex(x, y));
			state->values[x + WIDTH * y] = val;
		}
	}
}

void draw(state_t *state)
{
	ClearBackground(RAYWHITE);

	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			uint32_t val = state->values[x + WIDTH * y];
			Color c = { scale_over_range(val, 0, DEPTH, 0, 255), 0, 0, 255 };
			DrawPixel(x, y, c);
		}
	}
}

int main()
{
	InitWindow(WIDTH, HEIGHT, "Mandelbrot Set");
	SetTargetFPS(1);

	state_t state;
	allocate(&state);

	compute_mendelbrot(&state);
	while (!WindowShouldClose())
    {
		BeginDrawing();
		draw(&state);
		EndDrawing();
    }

	CloseWindow();
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define PLOT_LEN 50 

typedef struct
{
	int head;
	double data[PLOT_LEN];
	Rectangle area;
} plot_t;

void plot_draw(plot_t* plot)
{
	// Draw plot border
	DrawRectangleLinesEx(plot->area, 5.0, BLACK);

	double maxVal = 0.0;
	double minVal = 0.0;

	for (int i = 0; i < PLOT_LEN; i++)
	{
		if (plot->data[i] > maxVal) maxVal = plot->data[i];
		if (plot->data[i] < minVal) minVal = plot->data[i];
	}

	if (maxVal == 0.0 && minVal == 0.0)
	{
		maxVal = 5.0;
		minVal = -maxVal;
	}

	double di = maxVal - minVal;

	double step = plot->area.width/(PLOT_LEN-1);
	double xpos = 0.0;

	for (int i = 0; i < PLOT_LEN-1; i++)
	{
		int idx1 = i+plot->head < PLOT_LEN ? i+plot->head : i+plot->head-PLOT_LEN;
		int idx2 = i+plot->head+1 < PLOT_LEN ? i+plot->head+1 : i+1+plot->head-PLOT_LEN;
		Vector2 start = {
			.x = plot->area.x + xpos, 
			.y = plot->area.y + plot->area.height*(0.5-plot->data[idx1]/(di*2.0))
		};
		xpos+=step;
		Vector2 end = {
			.x = plot->area.x + xpos, 
			.y = plot->area.y + plot->area.height*(0.5-plot->data[idx2]/(di*2.0))
		};

		DrawLineEx(start, end, 10, RED);
	}
}

int main(int argc, char **argv)
{
	plot_t plt = { .area = (Rectangle){.x=10,.y=10,.width=780,.height=580}, 0 };

	for (int i = 0; i < PLOT_LEN; i++) plt.data[i] = 100 - (rand()%200);

	InitWindow(800, 600, "raylibPlot");
	SetTargetFPS(60);

	int cnt = 0;

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);

		plot_draw(&plt);

		cnt++;

		if (cnt > 1)
		{
			cnt = 0;
			plt.head = plt.head+1 < PLOT_LEN ? plt.head+1 : 0;
			int tail = plt.head-1 < 0 ? PLOT_LEN-1 : plt.head-1;
			plt.data[tail] = 100 - (rand()%200);
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#define PLOT_LEN 100 

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

		DrawLineEx(start, end, 3, RED);
	}
}

void plot_push_smpl(plot_t* plot, double sample)
{
	plot->head = plot->head+1 < PLOT_LEN ? plot->head+1 : 0;
	int tail = plot->head-1 < 0 ? PLOT_LEN-1 : plot->head-1;
	plot->data[tail] = sample;
}

int main(int argc, char **argv)
{
	plot_t plts[4] = {
		(plot_t){ .area = (Rectangle){.x=10,.y=10,.width=380,.height=280}},
		(plot_t){ .area = (Rectangle){.x=410,.y=10,.width=380,.height=280}},
		(plot_t){ .area = (Rectangle){.x=10,.y=310,.width=380,.height=280}},
		(plot_t){ .area = (Rectangle){.x=410,.y=310,.width=380,.height=280}},
	};

	//for (int i = 0; i < PLOT_LEN; i++) plt.data[i] = 100 - (rand()%200);

	InitWindow(800, 800, "raylibPlot");
	GuiLoadStyleDefault();
	SetTargetFPS(60);

	char serialPort[256] = { 0 };

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);

		for (int i = 0; i < 4; i++)
		{
			plot_draw(plts+i);

			switch (i)
			{
			case 0:
				plot_push_smpl(plts+i, sin(2*PI*GetTime()));
				break;
			default: 
				plot_push_smpl(plts+i, 100 - (rand()%200));
				break;
			}
		}

		/* GUI */
		
		/* The serial port textbox */
		GuiTextBox(
				(Rectangle){.x=10, .y=610, .width=100, .height=20},
				serialPort,
				256,
				1
		);
		

		/* Connect button */
		if (GuiButton((Rectangle){.x=120, .y=610, .width=50, .height=20}, "CONNECT"))
			printf("Conecting to %s...\n", serialPort);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}

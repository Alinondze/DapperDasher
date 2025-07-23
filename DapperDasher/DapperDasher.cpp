#include "raylib.h"

struct AnimData
{
	Rectangle rec;
	Vector2 pos;
	int frame;
	float updateTime;
	float runningTime;
};
int main()
{
	//window dimensions
	const int windowWidth{ 512 };
	const int windowHeight{ 380 };

	//acceleration due to gravity(pixels/s)/s
	const int gravity{ 1'000 };


	//initialize the window
	InitWindow(windowWidth, windowHeight, "Dapper Dasher");

	//nebula variables
	Texture2D nebula= LoadTexture("../textures/12_nebula_spritesheet.png");

	//AnimData for nebula
	AnimData nebData
	{ 
		{0.0,0.0,nebula.width / 8,nebula.height / 8}, //Rectangle rec
		{windowWidth,windowHeight - nebula.height / 8}, // Vector2 pos
		0, //int frame 
		{1.0 / 12.0}, // float updateTime
		0 //float runningTime
	};

	AnimData neb2Data
	{ 
		{0.0,0.0,nebula.width / 8,nebula.height / 8}, //Rectangle rec2
		{windowWidth + 300 ,windowHeight - nebula.width / 8}, // Vector2 pos2
		0,  //int frame 
		{1.0 / 16.0}, // float updateTime2
		0  //float runningTime2
	};

	//nebula X velocity(pixels/second)
	int nebVel{ -300 };

	//scarfy variables 
	Texture2D scarfy = LoadTexture("../textures/scarfy.png");
	AnimData scarfyData;
	scarfyData.rec.width= scarfy.width / 6;
	scarfyData.rec.height= scarfy.height;
	scarfyData.rec.x = 0;
	scarfyData.rec.y = 0;
	scarfyData.pos.x= windowWidth / 2 - scarfyData.rec.width / 2;
	scarfyData.pos.y = windowHeight  - scarfyData.rec.height;
	scarfyData.frame = 0;
	scarfyData.updateTime = 1.0 / 12.0;
	scarfyData.runningTime = 0.0;

	int velocity{ 0 };

	//is the rectangle in the air
	bool isInAir = false;
	//jump velocity (pixels/second)
	const int jumpVel{ -600 };

	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		//start drawing
		BeginDrawing();

		ClearBackground(WHITE);

		 const float dt = GetFrameTime();

		//perform ground check
		if (scarfyData.pos.y >= windowHeight- scarfyData.rec.height)
		{
			//rectangle is on the ground
			velocity = 0;

			isInAir = false;
		}
		else
		{
			//rectangle is in the air
			velocity += gravity* dt;

			isInAir = true;
		}

		//Jump check
		if (IsKeyPressed(KEY_SPACE) && !isInAir )
		{
			velocity += jumpVel;

		}

		// update nabula animation frame
		neb2Data.runningTime += dt;

		if (neb2Data.runningTime >= neb2Data.updateTime)
		{
			neb2Data.runningTime = 0.0;
			
			neb2Data.rec.x = neb2Data.frame * neb2Data.rec.width;

			neb2Data.frame++;
			if (neb2Data.frame > 7)
			{
				neb2Data.frame = 0;
			}
		}

		// update nabula animation frame
		nebData.runningTime += dt;
		if (nebData.runningTime >= nebData.updateTime)
		{
			nebData.runningTime = 0.0;

			nebData.rec.x = nebData.frame * nebData.rec.width;

			nebData.frame++;
			if (nebData.frame > 7)
			{
				nebData.frame = 0;
			}
		}

		//update nebula position
		nebData.pos.x += nebVel * dt;
		
		//update position
		scarfyData.pos.y += velocity * dt;

		//update the second nebula`s posiliton 
		neb2Data.pos.x += nebVel * dt;
		if (!isInAir)
		{
			//update running time
			scarfyData.runningTime += dt;
			// update scarfy  animation frame
			if (scarfyData.runningTime >= scarfyData.updateTime)
			{
				scarfyData.runningTime = 0.0;

				// update animation frame
				scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;

				scarfyData.frame++;

				if (scarfyData.frame > 5)
				{
					scarfyData.frame = 0;
				}
			}
		}
		//draw nebula
		DrawTextureRec(nebula, nebData.rec, nebData.pos, WHITE);
        //dra the second nebula
		DrawTextureRec(nebula, neb2Data.rec, neb2Data.pos, RED);
		// draw scarfy
		DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
		//stop drawing
		EndDrawing();
	}
	UnloadTexture(scarfy);
	UnloadTexture(nebula);
	CloseWindow();
	return 0;
}
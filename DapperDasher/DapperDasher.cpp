#include "raylib.h"

struct AnimData
{
	Rectangle rec;
	Vector2 pos;
	int frame;
	float updateTime;
	float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
	return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime,int maxFrame)
{
	//update running time
	data.runningTime += deltaTime;
	if (data.runningTime >= data.updateTime)
	{
		data.runningTime = 0.0;
		//update animation frame
		data.rec.x = data.frame *data.rec.width;
		data.frame++;
			if (data.frame > maxFrame)
			{
				data.frame = 0;
			}
	}
	return data;
}
int main()
{
	

	//acceleration due to gravity(pixels/s)/s
	const int gravity{ 1'000 };

	int windowDimension[2]{ 512,380 };
	//initialize the window
	InitWindow(windowDimension[0], windowDimension[1], "Dapper Dasher");

	//nebula variables
	Texture2D nebula= LoadTexture("../textures/12_nebula_spritesheet.png");
	const int sizeOfNebule{ 6 };
	AnimData nebule[sizeOfNebule]{};

	for (int i = 0; i < sizeOfNebule; i++)
	{
		nebule[i].rec.x = 0.0;
		nebule[i].rec.y = 0.0;
		nebule[i].rec.width = nebula.width / 8;
		nebule[i].rec.height = nebula.height / 8;
		nebule[i].pos.y = windowDimension[1] - nebula.height / 8;
		nebule[i].frame = 0;
		nebule[i].runningTime = 0;
		nebule[i].updateTime = 1.0 / 16.0;
		nebule[i].pos.x = windowDimension[0] + i * 300;

	}
	float finishLine{nebule[sizeOfNebule-1].pos.x};
	//nebula X velocity(pixels/second)
	int nebVel{ -300 };

	//scarfy variables 
	Texture2D scarfy = LoadTexture("../textures/scarfy.png");
	AnimData scarfyData;
	scarfyData.rec.width= scarfy.width / 6;
	scarfyData.rec.height= scarfy.height;
	scarfyData.rec.x = 0;
	scarfyData.rec.y = 0;
	scarfyData.pos.x= windowDimension[0] / 2 - scarfyData.rec.width / 2;
	scarfyData.pos.y = windowDimension[1] - scarfyData.rec.height;
	scarfyData.frame = 0;
	scarfyData.updateTime = 1.0 / 12.0;
	scarfyData.runningTime = 0.0;

	int velocity{ 0 };
	Texture2D background = LoadTexture("../textures/far-buildings.png");
	Texture2D midground = LoadTexture("../textures/back-buildings.png");
	Texture2D foreground = LoadTexture("../textures/foreground.png");
	float bgX{};
	float mgX{};
	float fgX{};
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
		
		//draw the backgrounds
		Vector2 bg1Pos{ bgX,0.0 };
		DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
		const float dt = GetFrameTime();
		bgX -= 20 * dt;
		Vector2 bg2Pos{ bgX + background.width * 2,0.0 };
		DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);
		//Scroll background
		if (bgX <= -background.width * 2)
		{
			bgX = 0.0;
		}

		//Scroll midground
		mgX -= 40 * dt;
		Vector2 mg1Pos{ mgX, 0.0 };
		DrawTextureEx(midground,mg1Pos, 0.0, 2.0, WHITE );
		Vector2 mg2Pos{ mgX + midground.width * 2,0.0 };
		DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

		if (mgX <= -midground.width * 2)
		{
			mgX = 0.0;
		}

		//Scroll foreground
		fgX -= 80 * dt;
		Vector2 fg1Pos{ fgX,0.0 };
		DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
		Vector2 fg2Pos{ fgX + foreground.width * 2,0.0 };
		DrawTextureEx(foreground,fg2Pos,0.0,2.0,WHITE);
		if (fgX <= -foreground.width)
		{
			fgX = 0.0;
		}

		//perform ground check
		if (isOnGround(scarfyData,windowDimension[1]))
		{
			//rectangle is on the ground
			velocity = 0;

			isInAir = false;
		}
		else
		{
			//rectangle is in the air
			velocity += gravity * dt;

			isInAir = true;
		}

		//Jump check
		if (IsKeyPressed(KEY_SPACE) && !isInAir)
		{
			velocity += jumpVel;
			
		}

		for (int i = 0; i < sizeOfNebule; i++)
		{
				// update nabula animation frame
			nebule[i] = updateAnimData(nebule[i], dt, 7);
		}

		for (int i = 0; i < sizeOfNebule; i++)
		{
			//update the position of each nebula
			nebule[i].pos.x += nebVel * dt;
		}
		//update finishline
		finishLine += nebVel * dt;
		//update position
		scarfyData.pos.y += velocity * dt;

		if (!isInAir)
		{
			scarfyData = updateAnimData(scarfyData, dt, 5);
		}
		for (int i = 0; i < sizeOfNebule; i++)
		{
			//draw nebula
			DrawTextureRec(nebula, nebule[i].rec, nebule[i].pos, WHITE);
		}
        
		// draw scarfy
		DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
		//stop drawing
		EndDrawing();
	}
	UnloadTexture(scarfy);
	UnloadTexture(nebula);
	UnloadTexture(background);
	UnloadTexture(midground);
	UnloadTexture(foreground);
	CloseWindow();
	return 0;
}
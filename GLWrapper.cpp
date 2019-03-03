#define GLW_APPLICATION
#include "GLW.h"

#include <vector>
#include <algorithm>

// Override base class with your custom functionality
class Example : public glw::GLWrapper
{

private:
	glw::Sprite *spr;
	float playerX = 0.0f;
	float playerY = 0.0f;
	float speed = 250.0f;

public:
	Example()
	{
		sAppName = "Demo";
	}

	struct vec2d
	{
		float x;
		float y;
	};

	struct polygon
	{
		std::vector<vec2d> p;	// Transformed Points
		vec2d pos;				// Position of shape
		float angle;			// Direction of shape
		std::vector<vec2d> o;	// "Model" of shape							
		bool overlap = false;	// Flag to indicate if overlap has occurred
	};

	std::vector<polygon> vecShapes;

	int nMode = 0;

public:
	bool OnUserCreate() override {
		spr = new glw::Sprite("cursor.png");



		polygon s1;
		float fTheta = 3.14159f * 2.0f / 5.0f;
		s1.pos = { 100, 100 };
		s1.angle = 0.0f;
		for (int i = 0; i < 5; i++) {
			s1.o.push_back({ 30.0f * cosf(fTheta * i), 30.0f * sinf(fTheta * i) });
			s1.p.push_back({ 30.0f * cosf(fTheta * i), 30.0f * sinf(fTheta * i) });
		}

		// Create Triangle
		polygon s2;
		fTheta = 3.14159f * 2.0f / 3.0f;
		s2.pos = { 200, 150 };
		s2.angle = 0.0f;
		for (int i = 0; i < 3; i++) {
			s2.o.push_back({ 20.0f * cosf(fTheta * i), 20.0f * sinf(fTheta * i) });
			s2.p.push_back({ 20.0f * cosf(fTheta * i), 20.0f * sinf(fTheta * i) });
		}

		// Create Quad
		/*
		polygon s3;
		s3.pos = { 50, 200 };
		s3.angle = 0.0f;
		s3.o.push_back({ -30, -30 });
		s3.o.push_back({ -30, +30 });
		s3.o.push_back({ +30, +30 });
		s3.o.push_back({ +30, -30 });
		s3.p.resize(4);
		vecShapes.push_back(s3);
		*/


		vecShapes.push_back(s1);
		vecShapes.push_back(s2);
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		
		//SetPixelMode(glw::Pixel::ALPHA);
		//DrawSprite(GetMouseX(), GetMouseY(), spr, glw::Pixel::MASK);
		//FillRect(playerX, playerY, 20, 20);

		/*
		if (GetKey(glw::Key::LEFT).bHeld) playerX -= speed * fElapsedTime;
		if (GetKey(glw::Key::RIGHT).bHeld) playerX += speed * fElapsedTime;
		if (GetKey(glw::Key::UP).bHeld) playerY -= speed * fElapsedTime;
		if (GetKey(glw::Key::DOWN).bHeld) playerY += speed * fElapsedTime;
		*/

		if (GetKey(glw::Key::F1).bReleased) nMode = 0;
		if (GetKey(glw::Key::F2).bReleased) nMode = 1;
		if (GetKey(glw::Key::F3).bReleased) nMode = 2;
		if (GetKey(glw::Key::F4).bReleased) nMode = 3;

		// Shape 1
		if (GetKey(glw::Key::LEFT).bHeld) vecShapes[1].angle -= 2.0f * fElapsedTime;
		if (GetKey(glw::Key::RIGHT).bHeld) vecShapes[1].angle += 2.0f * fElapsedTime;

		if (GetKey(glw::Key::UP).bHeld) {
			vecShapes[1].pos.x += cosf(vecShapes[1].angle) * speed * fElapsedTime;
			vecShapes[1].pos.y += sinf(vecShapes[1].angle) * speed * fElapsedTime;
		}

		if (GetKey(glw::Key::DOWN).bHeld) {
			vecShapes[1].pos.x -= cosf(vecShapes[1].angle) * speed * fElapsedTime;
			vecShapes[1].pos.y -= sinf(vecShapes[1].angle) * speed * fElapsedTime;
		}

		// Shape 2
		if (GetKey(glw::Key::A).bHeld) vecShapes[0].angle -= 2.0f * fElapsedTime;
		if (GetKey(glw::Key::D).bHeld) vecShapes[0].angle += 2.0f * fElapsedTime;

		if (GetKey(glw::Key::W).bHeld) {
			vecShapes[0].pos.x += cosf(vecShapes[0].angle) * speed * fElapsedTime;
			vecShapes[0].pos.y += sinf(vecShapes[0].angle) * speed * fElapsedTime;
		}

		if (GetKey(glw::Key::S).bHeld) {
			vecShapes[0].pos.x -= cosf(vecShapes[0].angle) * speed * fElapsedTime;
			vecShapes[0].pos.y -= sinf(vecShapes[0].angle) * speed * fElapsedTime;
		}

		Clear(glw::BLACK);

		for (auto &r : vecShapes)
		{
			for (int i = 0; i < r.o.size(); i++)
				r.p[i] =
			{	// 2D Rotation Transform + 2D Translation
				(r.o[i].x * cosf(r.angle)) - (r.o[i].y * sinf(r.angle)) + r.pos.x,
				(r.o[i].x * sinf(r.angle)) + (r.o[i].y * cosf(r.angle)) + r.pos.y,
			};

			r.overlap = false;
		}

		for (auto &r : vecShapes)
		{
			// Draw Boundary
			for (int i = 0; i < r.p.size(); i++)
				DrawLine(r.p[i].x, r.p[i].y, r.p[(i + 1) % r.p.size()].x, r.p[(i + 1) % r.p.size()].y, glw::WHITE);

			// Draw Direction
			DrawLine(r.p[0].x, r.p[0].y, r.pos.x, r.pos.y, glw::WHITE);
		}

		SetPixelMode(glw::Pixel::ALPHA);
		DrawSprite(GetMouseX()-5, GetMouseY()-5, spr, glw::Pixel::MASK);

		return true;
	}

};

int main() {
	Example demo;
	if (demo.Construct(480, 320, 1, 1)) demo.Start();
	return 0;
}

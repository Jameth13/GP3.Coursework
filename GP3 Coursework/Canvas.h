#pragma once
#include "pch.h"
#include "common.h"
#include "Renderer.h"

class Text
{
public:
	Text(std::string text, Vector3 posScale, Vector3 color)
	{
		this->text = text;
		this->posScale = posScale;
		this->color = color;
	}

	std::string text;
	Vector3 posScale;
	Vector3 color;
};

class Canvas
{
public:
	~Canvas();
	static Canvas* GetInstance();

	void RenderText(std::string text, Vector3 posScale, Vector3 color);

	void Draw();
	void Quit();

private:
	Canvas();
	static Canvas* instance;

	Renderer* renderer = Renderer::GetInstance();

	std::vector<Text>* texts = new std::vector<Text>();

};


#include "Canvas.h"

Canvas* Canvas::instance = 0;

Canvas::Canvas() {}

Canvas::~Canvas() {}

Canvas* Canvas::GetInstance()
{
	if (instance == 0)
		instance = new Canvas();

	return instance;
}

void Canvas::RenderText(std::string text, Vector3 posScale, Vector3 color)
{
	texts->push_back(Text(text, posScale, color));
}

void Canvas::Draw()
{
	for (unsigned int i = 0u; i < texts->size(); i++)
		renderer->RenderText(texts->at(i).text, texts->at(i).posScale, texts->at(i).color);
	texts->clear();
}

void Canvas::Quit()
{
	//Delete texts
	texts->clear();
	texts->shrink_to_fit();
	delete texts;

	instance = nullptr;
	delete this;
}

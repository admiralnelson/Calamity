#include "SegmentControl.h"

namespace clm {

Segment::Segment(SpriteManager* spriteManager, RenderLayer* renderLayer, wstring label, unsigned int fontId, int value) :
	spriteManager(spriteManager),
	renderLayer(renderLayer),
	fontId(fontId),
	x(0),
	y(0),
	width(0),
	left(0),
	right(0),
	bPressed(false),
	leftId(-1),
	middleId(-1),
	rightId(-1),
	textId(spriteManager->addText(label, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true)),
	value(value)
{}

Segment::~Segment() {
	spriteManager->removeSprite(leftId);
	spriteManager->removeSprite(middleId);
	spriteManager->removeSprite(rightId);
	spriteManager->removeText(textId);
}

bool Segment::contains(int x, int y) {
	int relX = x - this->x;
	int relY = this->y - y;
	return (relX > 0 && relX < width - 1 && relY > 0 && relY < 19);
}

void Segment::setIgnoreValue(bool bIgnoreValue) {
	this->bIgnoreValue = bIgnoreValue;
}

int Segment::getValue() {
	return value;
}

void Segment::setLeft(int left) {
	this->left = left;
}

void Segment::setRight(int right) {
	this->right = right;
}

void Segment::setSprites() {
	if (!bPressed) {
		middleId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_middle.png", renderLayer);

		if (left == 0)
			leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_left_sqsq.png", renderLayer);
		else if (left == 1)
			leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_left_rdsq.png", renderLayer);
		else if (left == 2)
			leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_left_sqrd.png", renderLayer);
		else if (left == 3)
			leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_left_rdrd.png", renderLayer);

		if (right == 0)
			rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_right_sqsq.png", renderLayer);
		else if (right == 1)
			rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_right_rdsq.png", renderLayer);
		else if (right == 2)
			rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_right_sqrd.png", renderLayer);
		else if (right == 3)
			rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_right_rdrd.png", renderLayer);
	}
	else {
		middleId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_middle_pr.png", renderLayer);

		if (left == 0)
			leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_left_pr_sqsq.png", renderLayer);
		else if (left == 1)
			leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_left_pr_rdsq.png", renderLayer);
		else if (left == 2)
			leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_left_pr_sqrd.png", renderLayer);
		else if (left == 3)
			leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_left_pr_rdrd.png", renderLayer);

		if (right == 0)
			rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_right_pr_sqsq.png", renderLayer);
		else if (right == 1)
			rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_right_pr_rdsq.png", renderLayer);
		else if (right == 2)
			rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_right_pr_sqrd.png", renderLayer);
		else if (right == 3)
			rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/segmentControl_right_pr_rdrd.png", renderLayer);
	}
}

void Segment::setPressed(bool bPressed) {
	this->bPressed = bPressed;
	spriteManager->removeSprite(leftId);
	spriteManager->removeSprite(middleId);
	spriteManager->removeSprite(rightId);
	setSprites();
	spriteManager->getRenderer()->getTextureManager()->upload();
}

void Segment::setPosition(float x, float y) {
	this->x = x;
	this->y = y;
}

void Segment::setWidth(float width) {
	this->width = width;
}

void Segment::updateSprites(int scValue) {
	if (!bIgnoreValue) {
		if (bPressed && scValue != value)
			setPressed(false);
		else if (!bPressed && scValue == value)
			setPressed(true);
	}

	spriteManager->setSpritePosition(leftId, x, y);
	spriteManager->setSpritePositionAndSize(middleId, x + 4, y, width - 8, 20);
	spriteManager->setSpritePosition(rightId, x + width - 4, y);

	int textWidth = spriteManager->getTextWidth(textId);
	spriteManager->setTextPosition(textId, floor(x + width / 2 - textWidth / 2), y - 14);
}

void Segment::prepare() {
	spriteManager->flagSpriteForRender(leftId);
	spriteManager->flagSpriteForRender(middleId);
	spriteManager->flagSpriteForRender(rightId);

	spriteManager->flagTextForRender(textId);
}

SegmentControl::SegmentControl(SpriteManager* spriteManager, unsigned int fontId, int width, Properties* properties, IntProperty* property, bool fillHorizontal) :
	renderLayer(nullptr),
	spriteManager(spriteManager),
	minWidth(width),
	minHeight(0),
	fillHorizontal(fillHorizontal),
	fillVertical(false),

	topLeftId(-1),
	topId(-1),
	topRightId(-1),
	leftId(-1),
	rightId(-1),
	bottomLeftId(-1),
	bottomId(-1),
	bottomRightId(-1),

	fontId(fontId),
	rows(),

	properties(properties),
	property(property)
{
	this->width = width;
}

SegmentControl::~SegmentControl() {
	spriteManager->removeSprite(topLeftId);
	spriteManager->removeSprite(topId);
	spriteManager->removeSprite(topRightId);
	spriteManager->removeSprite(leftId);
	spriteManager->removeSprite(rightId);
	spriteManager->removeSprite(bottomLeftId);
	spriteManager->removeSprite(bottomId);
	spriteManager->removeSprite(bottomRightId);

	for (auto& itRow : rows) {
		for (auto& itButton : itRow)
			delete itButton;
	}
}

void SegmentControl::initialize() {
	for (unsigned int i = 0; i < rows.size(); ++i) {
		for (unsigned int j = 0; j < rows[i].size(); ++j) {
			int top = (i == 0) ? 1 : 0;
			int bottom = (i == rows.size() - 1) ? 2 : 0;

			int topLeft = (j == 0) ? 1 : 0;
			topLeft &= top;
			int bottomLeft = (j == 0) ? 2 : 0;
			bottomLeft &= bottom;
			int left = topLeft | bottomLeft;

			int topRight = (j == rows[i].size() - 1) ? 1 : 0;
			topRight &= top;
			int bottomRight = (j == rows[i].size() - 1) ? 2 : 0;
			bottomRight &= bottom;
			int right = topRight | bottomRight;

			rows[i][j]->setLeft(left);
			rows[i][j]->setRight(right);
			rows[i][j]->setSprites();
		}
	}
}

void SegmentControl::setRenderLayer(RenderLayer* renderLayer) {
	this->renderLayer = renderLayer;

	topLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/buttons_sh_top_left.png", renderLayer);
	topId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/buttons_sh_top.png", renderLayer);
	topRightId = spriteManager->addSprite(posX + width - 5, posY + 10, "Calamity/Gui/Images/buttons_sh_top_right.png", renderLayer);

	leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/buttons_sh_left.png", renderLayer);
	rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/buttons_sh_right.png", renderLayer);

	bottomLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/buttons_sh_bottom_left.png", renderLayer);
	bottomId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/buttons_sh_bottom.png", renderLayer);
	bottomRightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/buttons_sh_bottom_right.png", renderLayer);
}

WidgetSize SegmentControl::calculateMinSize() {
	WidgetSize result;
	result.width = minWidth;
	result.height = minHeight;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void SegmentControl::setSize(int width, int height) {
	this->width = width;
	this->height = height;
}

WidgetSize SegmentControl::getSize() {
	WidgetSize result;
	result.width = minWidth;
	result.height = minHeight;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void SegmentControl::computeLayout() {
	spriteManager->setSpritePosition(topLeftId, posX - 5, posY + 4);
	spriteManager->setSpritePositionAndSize(topId, posX + 9, posY + 4, width - 18, 4);
	spriteManager->setSpritePosition(topRightId, posX + width - 9, posY + 4);

	spriteManager->setSpritePositionAndSize(leftId, posX - 5, posY - 9, 5, height - 18);
	spriteManager->setSpritePositionAndSize(rightId, posX + width, posY - 9, 5, height - 18);

	spriteManager->setSpritePosition(bottomLeftId, posX - 5, posY - height + 9);
	spriteManager->setSpritePositionAndSize(bottomId, posX + 9, posY - height, width - 18, 6);
	spriteManager->setSpritePosition(bottomRightId, posX + width - 9, posY - height + 9);

	spriteManager->flagSpriteForRender(topLeftId);
	spriteManager->flagSpriteForRender(topId);
	spriteManager->flagSpriteForRender(topRightId);
	spriteManager->flagSpriteForRender(leftId);
	spriteManager->flagSpriteForRender(rightId);
	spriteManager->flagSpriteForRender(bottomLeftId);
	spriteManager->flagSpriteForRender(bottomId);
	spriteManager->flagSpriteForRender(bottomRightId);

	for (unsigned int i = 0; i < rows.size(); ++i) {
		for (unsigned int j = 0; j < rows[i].size(); ++j) {
			int cWidth = width / rows[i].size() + 1;
			int offsetX = posX + j * (cWidth - 1);
			if (j == rows[i].size() - 1) {
				int rest = width % rows[i].size();
				cWidth += rest - 1;
			}
			rows[i][j]->setPosition(offsetX, posY - i * 19);
			rows[i][j]->setWidth(cWidth);
			if (property)
				rows[i][j]->updateSprites(property->getValue());
			else
				rows[i][j]->updateSprites(-1);
			rows[i][j]->prepare();
		}
	}
}

PriorityWidget SegmentControl::getHighestPriorityWidget(int x, int y, SDL_Event* event) {
	PriorityWidget result;
	result.priority = 1;
	result.widget = this;
	return result;
}

ActionHandler* SegmentControl::createActionHandler(int x, int y, SDL_Event* event) {
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT &&
			 x > posX && x < posX + width && y < posY && y > posY - height) { // can't click on border
		for (unsigned int i = 0; i < rows.size(); ++i) {
			for (unsigned int j = 0; j < rows[i].size(); ++j) {
				if (rows[i][j]->contains(x, y)) {
					rows[i][j]->setPressed(true);
					return new SegmentControlHandler(rows[i][j], properties, property, spriteManager->getScreenHeight());
				}
			}
		}
	}
	return nullptr;
}

void SegmentControl::addRow() {
	if (rows.size() == 0)
		minHeight += 20;
	else
		minHeight += 19;
	height = minHeight;
	vector<Segment*> newRow;
	rows.push_back(newRow);
}

void SegmentControl::addSegment(unsigned int row, wstring label, int value) {
	Segment* newSegment = new Segment(spriteManager, renderLayer, label, fontId, value);
	rows[row].push_back(newSegment);
}

void SegmentControl::draw() {

}

}

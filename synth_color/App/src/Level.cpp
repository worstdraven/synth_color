# include "Level.hpp"

Level::Level(const InitData& init)
	: IScene{ init }
{
}

void Level::update() {
	for (int i = 0; i < 15; ++i) {
		if (SimpleGUI::Button(U"Level" + Format(i + 1), Vec2{ 10 + (i % 5) * 120, 10 + std::floor(i / 5) * 70 })) {
			setLevelDesign(0);
			changeScene(State::Game, 100);
		}
	}
}

void Level::draw() const {

}

void Level::setLevelDesign(int level) {
	const int g = getData().gridSize;

	// レベルデザインの設定
	getData().pieces << Piece{ Vec2{ g * 15.0, g * 6.0 }, g * 2.0, ColorF{ 1.0, 0.0, 0.0, 1.0 } };
	getData().pieces << Piece{ Vec2{ g * 17.0, g * 7.0 }, g * 2.0, ColorF{ 0.0, 1.0, 0.0, 1.0 } };
	getData().pieces << Piece{ Vec2{ g * 19.0, g * 8.0 }, g * 2.0, ColorF{ 1.0, 0.0, 0.0, 1.0 } };

	getData().correctPositions << Point{ 0, 0 };
	getData().correctPositions << Point{ 90, 90 };
	getData().correctPositions << Point{ 180, 90 };

	Print << getData().pieces.size();

}

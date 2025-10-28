# include "Level.hpp"

Level::Level(const InitData& init)
	: IScene{ init }
{
}

void Level::update() {
	for (int i = 0; i < 15; ++i) {
		if (SimpleGUI::Button(U"Level" + Format(i + 1), Vec2{ 10 + (i % 5) * 120, 10 + std::floor(i / 5) * 70 })) {
			changeScene(State::Game);
		}
	}
}

void Level::draw() const {

}

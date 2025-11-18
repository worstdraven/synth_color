# include "Level.hpp"

struct ClickedButtonEffect : IEffect
{
	Array<Piece> m_coloredButtons;
	Duration m_duration, m_delay;
	int m_index;

	explicit ClickedButtonEffect(Piece& pushedButton, int index, Duration duration, Duration delay)
		: m_index(index + 1)
		, m_duration(duration)
		, m_delay(delay)
	{
		m_coloredButtons << Piece{ pushedButton.poly, SubtractiveCyan };
		m_coloredButtons << Piece{ pushedButton.poly, SubtractiveMagenta };
		m_coloredButtons << Piece{ pushedButton.poly, SubtractiveYellow };
	};
	bool update(double timeSec) override
	{
		for (auto&& [i, button] : IndexedRef(m_coloredButtons)) {
			if (Duration{ timeSec } > m_delay) {
				const double angle = 120_deg * i;
				const double radius = Width() / 4.5 * std::pow(timeSec, 4.3);
				button.poly.moveBy(Vec2{ std::cos(angle) * radius, std::sin(angle) * radius } *Scene::DeltaTime());
			}
			{
				const ScopedRenderStates2D blend{ BlendState::Subtractive };
				button.draw();
			}
			FontAsset(U"Bold")(m_index).drawAt(button.poly.centroid(), Palette::White);
		}
		return Duration{ timeSec } < m_duration + m_delay;
	}
};

Level::Level(const InitData& init)
	: IScene{ init }
{
	getData().fetchGridSize();

	// ボタンの位置を初期化
	for (int i = 0; i < 8; ++i) {
		const Vec2 center = Vec2{ Width() / 8.0 * (1 + (i % 4) * 2), Height() / 3.0 * (1 + (i / 4)) };
		m_pieceButtons << Piece{ center, getData().gridSize * 2.2, Palette::White };
	}

	// GameStruct内のクリア情報配列を初期化
	if (getData().isCleared.size() == 0) {
		getData().isCleared = Array<bool>(8, false);
	}
}

void Level::update() {
	for (auto&& [i, button] : IndexedRef(m_pieceButtons)) {
		const Vec2 center = Vec2{ Width() / 8.0 * (1 + (i % 4) * 2), Height() / 3.0 * (1 + (i / 4)) };
		if (button.poly.leftClicked()) {
			m_selectedButtonIndex = i;
			m_effect.add<ClickedButtonEffect>(button, m_selectedButtonIndex, EffectDuration, DisappearDuration);
			setLevelDesign(i);
			changeScene(State::Game, LevelToGameDuration);
		}
	}

	m_effect.update();
}

void Level::draw() const {
	const int16 g = getData().fetchGridSize();
	getData().drawGrid();

	for (int i = 0; i < 8; ++i) {
		if (i == m_selectedButtonIndex) {
			continue;
		}
		const Vec2 center = Vec2{ Width() / 8.0 * (1 + (i % 4) * 2), Height() / 3.0 * (1 + (i / 4)) };
		m_pieceButtons[i].poly.draw(ColorF{ 0.0, m_changeSceneTransition.value() });
		FontAsset(U"Bold")(i + 1).drawAt(center, ColorF{ 1.0, m_changeSceneTransition.value() });
		// クリア済みステージにマークを付ける
		if (getData().isCleared[i]) {
			Shape2D::Star(g * 0.8, center - Vec2{ g * 1.6, g * 1.6 })
				.draw(ColorF{ 1.0, 1.0, 0.0, m_changeSceneTransition.value() })
				.drawFrame(2, ColorF{ 47.0 / 255.0, m_changeSceneTransition.value() });
		}
	}
}

void Level::setLevelDesign(int level) {
	const int g = getData().fetchGridSize();

	// レベルデザインの設定

	getData().currentLevel = level;

	switch (level) {
	case 0:
		getData().pieces << Piece{ Vec2{ 0, 0 }, g * 2.0, SubtractiveCyan };
		getData().pieces << Piece{ Vec2{ 0, 0 }, g * 2.0, SubtractiveMagenta };
		getData().pieces << Piece{ Vec2{ 0, 0 }, g * 2.0, SubtractiveYellow };
		getData().correctPositions << Point{ 0, 0 };
		getData().correctPositions << Point{ 2 * g, 0 * g };
		getData().correctPositions << Point{ 1 * g, 2 * g };

		getData().correctCenter = Point{ g * 1, g * 2 };
		break;
	case 1:
		getData().pieces << Piece{ Array<Vec2>{Vec2{ 0, 0 }, Vec2{g * 3, 0}, Vec2{g * 3, g * 5}, Vec2{0 , g * 5}}, SubtractiveYellow };
		getData().pieces << Piece{ Vec2{ 0, 0 }, g * 2.0, SubtractiveCyan };
		getData().pieces << Piece{ Vec2{ 0, 0 }, g * 1.0, SubtractiveMagenta };
		getData().correctPositions << Point{ 0, 0 };
		getData().correctPositions << Point{ 2 * g, 3 * g };
		getData().correctPositions << Point{ 1 * g, 0 * g };

		getData().correctCenter = Point{ g * 2, g * 2 };
		break;
	case 2:
		getData().pieces << Piece{ Array<Vec2>{Vec2{ 0, 0 }, Vec2{g * 3, 0}, Vec2{g * 3, g * 5}, Vec2{0 , g * 5}}, SubtractiveYellow };
		getData().pieces << Piece{ Vec2{ 0, 0 }, g * 2.0, SubtractiveCyan };
		getData().pieces << Piece{ Vec2{ 0, 0 }, g * 1.0, SubtractiveMagenta };
		getData().correctPositions << Point{ 0, 0 };
		getData().correctPositions << Point{ 2 * g, 3 * g };
		getData().correctPositions << Point{ 1 * g, 0 * g };

		getData().correctCenter = Point{ g * 2, g * 2 };
		break;
	case 3:
		getData().pieces << Piece{ Array<Vec2>{ Vec2{ 0, 0 }, Vec2{g * 3, g * 5}, Vec2{0, g * 5} }, SubtractiveMagenta };
		getData().pieces << Piece{ Array<Vec2>{ Vec2{ 0, 0 }, Vec2{0, g * 5}, Vec2{-g * 3, g * 5} }, SubtractiveMagenta };
		getData().pieces << Piece{ Vec2{ 0, 0 }, g * 1.0, SubtractiveMagenta };
		getData().pieces << Piece{ Array<Vec2>{ Vec2{ 0, 0 }, Vec2{g * 3, 0}, Vec2{g * 3, g * 3}, Vec2{0, g * 3 } }, SubtractiveCyan };
		getData().pieces << Piece{ Array<Vec2>{ Vec2{ 0, 0 }, Vec2{g * 3, 0}, Vec2{g * 3, g * 4}, Vec2{0, g * 4 } }, SubtractiveCyan };
		getData().pieces << Piece{ Array<Vec2>{ Vec2{ 0, 0 }, Vec2{g * 1, 0}, Vec2{g * 1, g * 1}, Vec2{0, g * 1 } }, SubtractiveYellow };
		getData().pieces << Piece{ Array<Vec2>{ Vec2{ 0, 0 }, Vec2{g * 2, 0}, Vec2{g * 2, g * 2}, Vec2{0, g * 2 } }, SubtractiveYellow };
		getData().correctPositions << Point{ 0, 0 };
		getData().correctPositions << Point{ g * 6, 0 };
		getData().correctPositions << Point{ g * 3, g * 2 };
		getData().correctPositions << Point{ g * 0, g * 2 };
		getData().correctPositions << Point{ g * 3, g * 1 };
		getData().correctPositions << Point{ g * 5, g * 0 };
		getData().correctPositions << Point{ g * 0, g * 0 };

		getData().correctCenter = Point{ g * 3, g * 3 };
		break;
	}
}

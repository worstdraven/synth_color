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
		Print << U"Effect created:";
	};
	bool update(double timeSec) override
	{
		if (Duration{ timeSec } < m_delay) {
			for (const auto& button : m_coloredButtons) {
				{
					const ScopedRenderStates2D blend{ BlendState::Subtractive };
					button.draw();
				}
				FontAsset(U"Bold")(m_index).drawAt(button.poly.centroid(), ColorF{ 1.0 });
			}
			return true;
		}
		for (auto&& [i, button] : IndexedRef(m_coloredButtons)) {
			const double angle = 120_deg * i;
			const double radius = Scene::Width() / 4.5 * std::pow(timeSec, 4.3);
			button.poly.moveBy(Vec2{ std::cos(angle) * radius, std::sin(angle) * radius } *Scene::DeltaTime());
			{
				const ScopedRenderStates2D blend{ BlendState::Subtractive };
				button.draw();
			}
			FontAsset(U"Bold")(m_index).drawAt(button.poly.centroid(), ColorF{ 1.0 });
		}
		ClearPrint();
		Print << timeSec;
		return Duration{ timeSec } < m_duration + m_delay;
	}
};

Level::Level(const InitData& init)
	: IScene{ init }
{
	const Array<ColorF>& colors = {
		SubtractiveCyan,
		SubtractiveMagenta,
		SubtractiveYellow
	};

	for (int i = 0; i < 8; ++i) {
		const Vec2 center = Vec2{ Scene::Width() / 8.0 * (1 + (i % 4) * 2),	 Scene::Height() / 3.0 * (1 + (i / 4)) };
		m_pieceButtons << Piece{ center, getData().gridSize * 2.2, ColorF{ 1.0 } };
	}

}

void Level::update() {
	for (auto&& [i, button] : IndexedRef(m_pieceButtons)) {
		const Vec2 center = Vec2{ Scene::Width() / 8.0 * (1 + (i % 4) * 2),	 Scene::Height() / 3.0 * (1 + (i / 4)) };
		if (button.poly.leftClicked()) {
			m_selectedButtonIndex = i;
			m_effect.add<ClickedButtonEffect>(button, m_selectedButtonIndex, EffectDuration, DisappearDuration);
			setLevelDesign(0);
			changeScene(State::Game, (DisappearDuration + EffectDuration) * 2);
		}
	}

	m_effect.update();
}

void Level::draw() const {
	getData().drawGrid();

	for (int i = 0; i < 8; ++i) {
		if (i == m_selectedButtonIndex) {
			continue;
		}
		const Vec2 center = Vec2{ Scene::Width() / 8.0 * (1 + (i % 4) * 2),	 Scene::Height() / 3.0 * (1 + (i / 4)) };
		//m_pieceButtons[i].draw();
		m_pieceButtons[i].poly.draw(ColorF{ 0.0, m_changeSceneTransition.value() });
		FontAsset(U"Bold")(i + 1).drawAt(center, ColorF{ 1.0, m_changeSceneTransition.value() });
	}
}

void Level::setLevelDesign(int level) {
	const int g = getData().gridSize;

	// レベルデザインの設定
	getData().pieces << Piece{ Vec2{ g * 15.0, g * 6.0 }, g * 2.0, SubtractiveCyan };
	getData().pieces << Piece{ Vec2{ g * 17.0, g * 7.0 }, g * 2.0, SubtractiveMagenta };
	getData().pieces << Piece{ Vec2{ g * 19.0, g * 8.0 }, g * 2.0, SubtractiveYellow };

	getData().correctPositions << Point{ 0, 0 };
	getData().correctPositions << Point{ 90, 90 };
	getData().correctPositions << Point{ 180, 90 };

	Print << getData().pieces.size();

}

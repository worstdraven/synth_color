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
				const double radius = Width();
				button.poly.moveBy(Vec2{ std::cos(angle) * radius, std::sin(angle) * radius } * Scene::DeltaTime() * (2.5 * pow(timeSec - m_delay.count(), 1.5)));
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
		getData().isCleared = Array<bool>(16, false);
	}

	m_nextButton = Triangle{ Vec2{ Width() - 35, Height() / 2 }, 50, 90_deg };
	m_prevButton = Triangle{ Vec2{ 35, Height() / 2 }, 50, -90_deg };
}

void Level::update() {
	if (m_pageTransitionSw.isStarted()) {
		if (m_pageTransitionSw > PageSwitchDuration) {
			m_currentPage += m_transitionDirection;
			m_transitionDirection = 0;
			m_pageTransitionSw.reset();
		}
		m_effect.update();
		return;
	}

	for (auto&& [i, button] : IndexedRef(m_pieceButtons)) {
		const Vec2 center = Vec2{ Width() / 8.0 * (1 + (i % 4) * 2), Height() / 3.0 * (1 + (i / 4)) };
		if (button.poly.leftClicked() && m_selectedButtonIndex < 0) {
			m_buttonAudio.play();
			m_selectedButtonIndex = i;
			m_effect.add<ClickedButtonEffect>(button, m_selectedButtonIndex + m_currentPage * 8, EffectDuration, DisappearDuration);
			getData().currentLevel = i + m_currentPage * 8;
			changeScene(State::Game, ChangeSceneDuration);
		}
	}
    // 最終ページなら右矢印ボタンを有効化
	if (m_currentPage < m_nPages - 1) {
		if (m_nextButton.leftClicked()) {
			m_buttonAudio.play();
			m_transitionDirection = 1;
			m_pageTransitionSw.restart();
		}
	}
    // 最初のページなら左矢印ボタンを有効化
	if (m_currentPage > 0) {
		if (m_prevButton.leftClicked()) {
			m_buttonAudio.play();
			m_transitionDirection = -1;
			m_pageTransitionSw.restart();
		}
	}

	m_effect.update();
}

void Level::drawPage(int pageIndex, double xOffset) const {
	const int16 g = getData().fetchGridSize();
	const Transformer2D transform{ Mat3x2::Translate(xOffset, 0) };

	for (int i = 0; i < 8; ++i) {
		if (pageIndex == m_currentPage && i == m_selectedButtonIndex) {
			continue;
		}
		const Vec2 center = Vec2{ Width() / 8.0 * (1 + (i % 4) * 2), Height() / 3.0 * (1 + (i / 4)) };
		m_pieceButtons[i].poly.draw(ColorF{ 0.0, m_changeSceneTransition.value() });
		FontAsset(U"Bold")(i + 1 + pageIndex * 8).drawAt(center, ColorF{ 1.0, m_changeSceneTransition.value() });
		// クリア済みステージにマークを付ける
		if (getData().isCleared[i + pageIndex * 8]) {
			Shape2D::Star(g * 0.8, center - Vec2{ g * 1.6, g * 1.6 })
				.draw(ColorF{ 1.0, 1.0, 0.0, m_changeSceneTransition.value() })
				.drawFrame(1, ColorF{ 0.0 }.withA(m_changeSceneTransition.value()));
		}
	}
}

void Level::draw() const {
	getData().drawGrid();

	if (m_pageTransitionSw.isStarted()) {
		const double t = EaseOutCubic(m_pageTransitionSw.sF() / PageSwitchDuration.count());
		if (m_transitionDirection == 1) {
			// Next: Current moves Left (-Width), Next moves in from Right (Width -> 0)
			drawPage(m_currentPage, -Width() * t);
			drawPage(m_currentPage + 1, Width() * (1.0 - t));
		}
		else if (m_transitionDirection == -1) {
			// Prev: Current moves Right (Width), Prev moves in from Left (-Width -> 0)
			drawPage(m_currentPage, Width() * t);
			drawPage(m_currentPage - 1, -Width() * (1.0 - t));
		}
	}
	else {
		drawPage(m_currentPage, 0);
	}

	// 最終ページなら右矢印を表示
	if (m_currentPage < m_nPages - 1 &&
        not (m_transitionDirection == 1 && m_pageTransitionSw.isStarted())
    ) {
		m_nextButton.draw(ColorF{ 0.0, m_changeSceneTransition.value() });
	}
    // 最終ページなら左矢印を表示
	if (m_currentPage > 0 &&
        not (m_transitionDirection == -1 && m_pageTransitionSw.isStarted())
    ) {
		m_prevButton.draw(ColorF{ 0.0, m_changeSceneTransition.value() });
	}
}

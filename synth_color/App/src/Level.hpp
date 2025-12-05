# pragma once
# include "Common.hpp"

class Level : public App::Scene
{
public:

	Level(const InitData& init);

	void update() override;

	void draw() const override;

	inline void updateFadeIn(double t) override {
		m_changeSceneTransition.update(true);
		update();
	}

	inline void drawFadeIn(double t) const override {
		draw();
	}

	inline void updateFadeOut(double t) override {
		m_changeSceneTransition.update(false);
		update();
	}

	inline void drawFadeOut(double t) const override {
		draw();
	}


private:
	Array<Piece> m_pieceButtons;

	int m_selectedButtonIndex = -1;

	static constexpr Duration DisappearDuration = ChangeSceneDuration / 2.0 * 0.3;
	static constexpr Duration EffectDuration = ChangeSceneDuration / 2.0 * 0.7;

	Effect m_effect;

	Transition m_changeSceneTransition{ ChangeSceneDuration / 2.0, DisappearDuration };

	const Audio m_buttonAudio{ U"audio/button.mp3" };

	static constexpr int m_nPages = 2;

	int m_currentPage = 0;

	Triangle m_nextButton;
	Triangle m_prevButton;

	Stopwatch m_pageTransitionSw;
	int m_transitionDirection = 0; // 0: none, 1: next, -1: prev

	static constexpr Duration PageSwitchDuration = 0.5s;

	void drawPage(int pageIndex, double xOffset) const;
};

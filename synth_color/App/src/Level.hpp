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
	void setLevelDesign(int level);

	Array<Piece> m_pieceButtons;

	int m_selectedButtonIndex = -1;

	static constexpr Duration DisappearDuration = 0.5s, EffectDuration = 1.4s;

	Effect m_effect;

	Transition m_changeSceneTransition{ DisappearDuration, DisappearDuration };
};

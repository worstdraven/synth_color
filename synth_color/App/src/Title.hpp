# pragma once
# include "Common.hpp"

// タイトルシーン
class Title : public App::Scene
{
public:

	Title(const InitData& init);

	~Title();

	void update() override;

	void draw() const override;

	inline void updateFadeOut(double t) override
	{
		m_changeSceneTransition.update(true);
	}

	inline void drawFadeOut(double t) const override
	{
		draw();
	}

private:

	const Texture m_titleLogo{ U"img/title_logo.png" };

	Transition m_changeSceneTransition{ ChangeSceneDuration / 2.0, 0.0s };

	Array<Piece> m_pieces;
	Array<double> m_periods, m_phases, m_radiuses;
};

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

	RoundRect m_exitButton{ Arg::center(400, 500), 300, 60, 8 };

	Transition m_exitTransition{ 0.4s, 0.2s };
	Transition m_changeSceneTransition{ TitleToLevelDuration / 2.0, 0.0s };

	Array<Piece> m_pieces;
	Array<double> m_periods, m_phases, m_radiuses;
};

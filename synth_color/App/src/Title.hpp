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

	const Audio m_buttonAudio{ U"audio/button.mp3" };

	const Button m_fullscreenButton{ U"\U000F0293", ratioVec(0.90, 0.03), Font{ FontMethod::MSDF, 48, Typeface::Icon_MaterialDesign }, 48.0 * Width() / 800.0};
};

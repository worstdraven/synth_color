# include "Title.hpp"

Title::Title(const InitData& init)
	: IScene{ init }
{
	int16 g = getData().fetchGridSize();

	m_pieces << Piece{ ratioVec(0.28, 0.97), 3.1 * g, SubtractiveMagenta };
	m_pieces << Piece{
		Array<Vec2>{ ratioVec(0.36, 0.76), ratioVec(0.46, 0.92), ratioVec(0.23, 1.1) },
		SubtractiveYellow
	};
	Piece temp = Piece{
		Polygon { { ratioVec(0.03, 0.76), ratioVec(0.20, 0.76), ratioVec(0.20, 1.1), ratioVec(0.03, 1.1) } },
		SubtractiveCyan
	};
	temp.poly.rotateAt(temp.poly.centroid(), -19_deg);
	m_pieces << temp;

	// right yellow circle
	m_pieces << Piece{
		Circle{ ratioVec(0.7, 0.9) , g * 1.8 }.asPolygon(30),
		SubtractiveYellow
	};

	// right cyantriangle
	m_pieces << Piece{
		Array<Vec2>{ ratioVec(0.76, 0.78), ratioVec(0.87, 1.02), ratioVec(0.62, 0.92) },
		SubtractiveCyan
	};

	// right magenta rectangle
	temp = Piece{
		Polygon { { ratioVec(0.82, 0.79), ratioVec(0.93	,0.79), ratioVec(0.93, 1.1), ratioVec(0.82, 1.1) } },
		SubtractiveMagenta
	};
	temp.poly.rotateAt(temp.poly.centroid(), 80_deg);
	m_pieces << temp;


	m_periods = { 12.0, 10.0, 14.0, 11.0, 9.4, 10.2 };
	m_phases = { 1.0, 0.0, 1.0, 2.0, 1.2, 3,2 };
	m_radiuses = { g / 3.3, g / 3.2, g / 3.0, g / 2.5, g / 3.2, g / 4.4 };
}

Title::~Title() {}

void Title::update()
{
	if (m_fullscreenButton.pushed()) {
		Window::SetFullscreen(Window::GetState().fullscreen ? false : true);
		changeScene(State::Title, 1);
	}
	else if (Scene::Rect().leftClicked())
	{
		m_buttonAudio.play();
		changeScene(State::Level, ChangeSceneDuration);
	}


	for (int i = 0; auto& piece : m_pieces) {
		piece.poly.moveBy(
			m_radiuses[i] * Scene::DeltaTime() * Periodic::Sine1_1(Duration{ m_periods[i] }, Scene::Time() + m_phases[i]) * (i % 2 == 0 ? 1 : -1),
			m_radiuses[i] * Scene::DeltaTime() * Periodic::Sine1_1(Duration{ m_periods[i] }, Scene::Time() + m_phases[i] + m_periods[i] / 4.0)
		);
		++i;
	}

}

void Title::draw() const
{
	// グリッド描画
	getData().drawGrid();

	// タイトル描画
	m_titleLogo.scaled(0.4 * Width() / 800).drawAt(ratioVec(0.473, 0.454), ColorF{ 1.0 , 1.0 - m_changeSceneTransition.value() });

	FontAsset(U"CommonFont")(U"- クリックしてスタート -")
		.drawAt(ratioVec(0.5, 0.65), ColorF{0.0 , 1.0 - m_changeSceneTransition.value()});

	{
		const ScopedRenderStates2D blend{ BlendState::Subtractive };
		for (const auto& piece : m_pieces) {
			piece.draw(1 - m_changeSceneTransition.value());
		}
	}

	m_fullscreenButton.draw(1 - m_changeSceneTransition.value());
}

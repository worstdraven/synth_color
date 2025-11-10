# include "Title.hpp"

Title::Title(const InitData& init)
	: IScene{ init }
{
	int32 w = Scene::Width();
	int32 h = Scene::Height();
	int16& g = getData().gridSize;

	m_pieces << Piece{ Vec2{ w * 0.28, h * 0.97 }, 3.1 * g, ColorF{ 0.0, 1.0, 0.0, 1.0 } };
	m_pieces << Piece{
		Array<Vec2>{ Vec2{ w * 0.36, h * 0.76 }, Vec2{ w * 0.46, h * 0.92 }, Vec2{ w * 0.23, h * 1.1 } },
		ColorF{ 0.0, 0.0, 1.0, 1.0 }
	};
	Piece temp = Piece{
		Polygon { { Vec2{ w * 0.03, h * 0.76 }, Vec2{ w * 0.20, h * 0.76 }, Vec2{ w * 0.20, h * 1.1 }, { w * 0.03, h * 1.1 } } },
		ColorF{ 1.0, 0.0, 0.0, 1.0 }
	};
	temp.poly.rotateAt(temp.poly.centroid(), -19_deg);
	m_pieces << temp;

	// right yellow circle
	m_pieces << Piece{
		Circle{ Vec2{ w * 0.7, h * 0.9} , g * 1.8 }.asPolygon(30),
		ColorF{ 0.0, 0.0, 1.0, 1.0 }
	};

	// right cyantriangle
	m_pieces << Piece{
		Array<Vec2>{ Vec2{ w * 0.76, h * 0.78 }, Vec2{ w * 0.87, h * 1.02 }, Vec2{ w * 0.62, h * 0.92 } },
		ColorF{ 1.0, 0.0, 0.0, 1.0 }
	};

	// right magenta rectangle
	temp = Piece{
		Polygon { { Vec2{ w * 0.82, h * 0.79 }, Vec2{ w * 0.93	, h * 0.79 }, Vec2{ w * 0.93, h * 1.1 }, { w * 0.82, h * 1.1 } } },
		ColorF{ 0.0, 1.0, 0.0, 1.0 }
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
	// ボタンの更新
	{
		m_exitTransition.update(m_exitButton.mouseOver());

		if (m_exitButton.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}
	}
	// ボタンのクリック処理
	if (m_exitButton.leftClicked()) // 終了
	{
		System::Exit();
	}
	else if (SimpleGUI::Button(U"change resolution", Vec2{ Width() * 0.90, Height() * 0.03 })) {
		Window::SetFullscreen(Window::GetState().fullscreen ? false : true);
	}
	else if (Scene::Rect().leftClicked())
	{
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
	m_titleLogo.scaled(0.4).drawAt(Scene::Width() / 2.11, Scene::Height() / 2.2, ColorF{ 1.0 , 1.0 - m_changeSceneTransition.value() });

	FontAsset(U"CommonFont")(U"- クリックしてスタート -")
		.drawAt(Scene::Width() / 2.0, Scene::Height() * 0.65, ColorF{ 0.0 , 1.0 - m_changeSceneTransition.value() });

	{
		const ScopedRenderStates2D blend{ BlendState::Subtractive };
		for (const auto& piece : m_pieces) {
			piece.poly.draw(ColorF{ piece.color.r / 255.0, piece.color.g / 255.0, piece.color.b / 255.0, 1.0 - m_changeSceneTransition.value() });
			piece.poly.drawFrame(2, ColorF{ 47.0 / 255.0, 1 - m_changeSceneTransition.value() });
		}
	}
}

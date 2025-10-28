# include "Game.hpp"

Game::Game(const InitData& init)
	: IScene{ init }
{
	m_pieces << std::make_unique<CirclePiece>(Vec2{ 100, 100 }, 75.0, ColorF{ 1.0, 0.0, 0.0, 1.0 });
	m_pieces << std::make_unique<CirclePiece>(Vec2{ 150, 200 }, 75.0, ColorF{ 0.0, 1.0, 0.0, 1.0 });
	m_pieces << std::make_unique<CirclePiece>(Vec2{ 200, 300 }, 75.0, ColorF{ 0.0, 0.0, 1.0, 1.0 });

}

void Game::update()
{
	const double w = Scene::Width();
	const double h = Scene::Height();
	const Point viewportTopLeft = Vec2{ w * 0.45, h * 0.15 }.asPoint();

	{
		const Transformer2D t{ Mat3x2::Identity(), Mat3x2::Translate(viewportTopLeft) };

		// 図形のドラッグ処理
		if (m_selectedPieceIndex != -1) {
			// マウスが話されたとき
			if (MouseL.up()) {
				Vec2 pos = m_pieces[m_selectedPieceIndex]->getPrimaryPos() + viewportTopLeft;
				Vec2 dest = { std::ceil(pos.x / m_gridSize - 0.5) * m_gridSize, ::ceil(pos.y / m_gridSize - 0.5) * m_gridSize };
				Print << dest << pos;
				m_pieces[m_selectedPieceIndex]->moveBy(dest - pos);
				m_selectedPieceIndex = -1;
			}
			// ドラッグされている間
			else {
				m_pieces[m_selectedPieceIndex]->moveBy(Cursor::DeltaF());
			}
		}
		else {
			for (int idx = 0; auto& poly : m_pieces) {
				if (poly->isPolygonPressed()) {
					m_selectedPieceIndex = idx;
					break;
				}
				++idx;
			}
		}
	}

	if (SimpleGUI::Button(U"Back to Title", Vec2{ 20, 20 })) {
		changeScene(State::Title, 1000);
	}


}

void Game::draw() const
{
	Scene::SetBackground(Palette::Steelblue);

	const double w = Scene::Width();
	const double h = Scene::Height();
	const Point viewportTopLeft = Vec2{ w * 0.45, h * 0.15 }.asPoint();
	const Rect viewportRect{ viewportTopLeft, static_cast<int>(w * 0.5), static_cast<int>(h * 0.75) };
	viewportRect.rounded(10.0).draw(Palette::White).drawFrame(2, Palette::Black);

	{
		// パズルビューポート
		const ScopedViewport2D viewport{ viewportRect };

		const Transformer2D t{ Mat3x2::Identity(), Mat3x2::Translate(viewportTopLeft) };

		for (int i = m_gridSize; i < viewportRect.w; i += m_gridSize) {
			for (int j = m_gridSize; j < viewportRect.h; j += m_gridSize) {
				Circle{ i, j, 1 }.draw(Palette::Black);
			}
		}

		{
			// 減算ブレンドで描画
			const ScopedRenderStates2D blend{ BlendState::Subtractive };

			for (const auto& poly : m_pieces) {
				poly->draw();
			}
		}

		// 任意の2つのピースの組み合わせを走査
		for (int i = 0; i < m_pieces.size() - 1; ++i) {
			for (int j = i + 1; j < m_pieces.size(); ++j) {
				const Array<Polygon> intersection_polygon = Geometry2D::And(m_pieces[i]->getPolygon(), m_pieces[j]->getPolygon());
				for (const auto& polygon : intersection_polygon)
				{
					// 共通部分を黒で描画
					polygon.drawFrame(2, Palette::Black);
				}
			}
		}
	}

	RectF{ w * 0.05, h * 0.2, w * 0.35, h * 0.6 }.rounded(10.0).draw(Palette::Black);
}

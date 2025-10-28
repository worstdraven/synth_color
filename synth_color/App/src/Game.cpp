# include "Game.hpp"

Game::Game(const InitData& init)
	: IScene{ init }
{
	m_pieces << std::make_unique<TrianglePiece>(Vec2{ 100, 500 }, Vec2{ 200, 600 }, Vec2{ 100, 600 }, ColorF{ 1.0, 0.0, 0.0, 1.0 });
	m_pieces << std::make_unique<TrianglePiece>(Vec2{ 300, 500 }, Vec2{ 400, 600 }, Vec2{ 300, 600 }, ColorF{ 0.0, 1.0, 0.0, 1.0 });
	m_pieces << std::make_unique<TrianglePiece>(Vec2{ 500, 500 }, Vec2{ 600, 600 }, Vec2{ 500, 600 }, ColorF{ 0.0, 0.0, 1.0, 1.0 });
	m_pieces << std::make_unique<TrianglePiece>(Vec2{ 700, 500 }, Vec2{ 800, 600 }, Vec2{ 700, 600 }, ColorF{ 1.0, 0.0, 0.0, 1.0 });
	m_pieces << std::make_unique<RectanglePiece>(Vec2{ 100, 200 }, Size{ 100, 150 }, ColorF{ 0.0, 1.0, 0.0, 1.0 });
	m_pieces << std::make_unique<CirclePiece>(Vec2{ 300, 300 }, 75.0, ColorF{ 1.0, 0.0, 0.0, 1.0 });
	m_pieces << std::make_unique<CirclePiece>(Vec2{ 400, 300 }, 75.0, ColorF{ 0.0, 1.0, 0.0, 1.0 });
	m_pieces << std::make_unique<CirclePiece>(Vec2{ 500, 300 }, 75.0, ColorF{ 0.0, 0.0, 1.0, 1.0 });

}

void Game::update()
{
	// 図形のドラッグ処理
	if (selected_piece_index != -1) {
		// マウスが話されたとき
		if (MouseL.up()) {
			Vec2 pos = m_pieces[selected_piece_index]->getPrimaryPos();
			Vec2 dest = { std::ceil(pos.x / grid_size - 0.5) * grid_size, ::ceil(pos.y / grid_size - 0.5) * grid_size };
			Print << dest << pos;
			m_pieces[selected_piece_index]->moveBy(dest - pos);
			selected_piece_index = -1;
		}
		// ドラッグされている間
		else {
			m_pieces[selected_piece_index]->moveBy(Cursor::DeltaF());
		}
	}
	else {
		for (int idx = 0; auto& poly : m_pieces) {
			if (poly->isPolygonPressed()) {
				selected_piece_index = idx;
				break;
			}
			++idx;
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

void Game::draw() const
{
	Scene::SetBackground(Palette::Steelblue);

	// UI
	const double w = Scene::Width();
	const double h = Scene::Height();

	const RoundRect puzzleWindow = RectF{ w * 0.55, h * 0.1, w * 0.4, h * 0.8 }.rounded(5.0).draw(Palette::White).drawFrame(2, Palette::Black);

	for (int i = grid_size; i < w; i += grid_size) {
		for (int j = grid_size; j < h; j += grid_size) {
			Circle gridPoint{ i, j, 2 };
			if (puzzleWindow.intersects(gridPoint)) {
				gridPoint.draw(Palette::Black);
			}
		}
	}

	{
		// 減算ブレンドで描画
		const ScopedRenderStates2D blend{ BlendState::Subtractive };

		for (const auto& poly : m_pieces) {
			poly->draw();
		}
	}
}


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
		if (selected_piece_index != -1) {
			// マウスが話されたとき
			if (MouseL.up()) {
				Vec2 pos = m_pieces[selected_piece_index]->getPrimaryPos() + viewportTopLeft;
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
	}


}

void Game::draw() const
{
	Scene::SetBackground(Palette::Steelblue);

	const double w = Scene::Width();
	const double h = Scene::Height();
	const Point viewportTopLeft = Vec2{ w * 0.45, h * 0.15 }.asPoint();
	const Rect viewportRect{ viewportTopLeft, static_cast<int>(w * 0.5), static_cast<int>(h * 0.75) };
	viewportRect.rounded(5.0).draw(Palette::White).drawFrame(2, Palette::Black);

	{
		// UI
		const ScopedViewport2D viewport{ viewportRect };

		const Transformer2D t{ Mat3x2::Identity(), Mat3x2::Translate(viewportTopLeft) };

		for (int i = grid_size; i < viewportRect.w; i += grid_size) {
			for (int j = grid_size; j < viewportRect.h; j += grid_size) {
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
}


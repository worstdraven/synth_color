# include "Game.hpp"

Game::Game(const InitData& init)
	: IScene{ init }, m_pieces{ getData().pieces }
{
}

void Game::update()
{
	{
		const Transformer2D t{ Mat3x2::Identity(), Mat3x2::Translate(m_puzzleViewportTopLeft) };

		// 図形のドラッグ処理
		if (m_selectedPieceIndex != -1) {
			// マウスが離されたときの処理
			if (MouseL.up()) {
				Vec2 pos = m_pieces[m_selectedPieceIndex]->getPrimaryPos();
				// グリッドにスナップ
				Vec2 dest{ std::ceil(pos.x / m_gridSize - 0.5) * m_gridSize, ::ceil(pos.y / m_gridSize - 0.5) * m_gridSize };
				// ビューポート外に出ないように制限
				dest.x = std::min(std::max(20.0, dest.x), static_cast<double>(m_puzzleViewportRect.w - 20.0));
				dest.y = std::min(std::max(20.0, dest.y), static_cast<double>(m_puzzleViewportRect.h - 20.0));
				Print << dest << pos;
				m_pieces[m_selectedPieceIndex]->moveBy(dest - pos);
				m_selectedPieceIndex = -1;
			}
			// ドラッグされている間、位置をマウスに追従させる
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

	if (SimpleGUI::Button(U"Back to level select", Vec2{ 20, 20 })) {
		changeScene(State::Level, 1000);
	}


}

void Game::draw() const
{
	Scene::SetBackground(Palette::Gray);

	FontAsset(U"Bold")(U"ステージ1").draw(48, Arg::bottomLeft(Scene::Width() * .68, Scene::Height() * .14));
	m_puzzleViewportRect.rounded(10.0).draw(Palette::White).drawFrame(2, Palette::Black);

	{
		// パズルビューポート
		const ScopedViewport2D viewport{ m_puzzleViewportRect };
		const Transformer2D t{ Mat3x2::Identity(), Mat3x2::Translate(m_puzzleViewportTopLeft) };

		for (int i = m_gridSize; i < m_puzzleViewportRect.w; i += m_gridSize) {
			for (int j = m_gridSize; j < m_puzzleViewportRect.h; j += m_gridSize) {
				Circle{ i, j, 1 }.draw(Palette::Black);
			}
		}

		{
			// 減算ブレンドで全ピースを描画
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

	// 左側のお手本パネル
	RectF{ Scene::Width() * 0.05, Scene::Height() * 0.2, Scene::Width() * 0.35, Scene::Height() * 0.6 }.rounded(10.0).draw(Palette::Black);
}

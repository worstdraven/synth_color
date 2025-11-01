# include "Game.hpp"

Game::Game(const InitData& init)
	: IScene{ init }, m_pieces{ getData().pieces }
{
}

Game::~Game() {
	//Print << U"Game destructor called.";
	// ピース配列をクリア
	m_pieces.clear();
	getData().correctPositions.clear();
}

void Game::update()
{
	const int g = getData().gridSize;
	// 図形のドラッグ処理
	if (m_selectedPieceIndex != -1) {
		// マウスが離されたときの処理
		if (MouseL.up()) {
			Vec2 pos = m_pieces[m_selectedPieceIndex].getPrimaryPos();
			// グリッドにスナップ
			Vec2 dest{ std::ceil(pos.x / g - 0.5) * g, ::ceil(pos.y / g - 0.5) * g };
			// ビューポート外に出ないように制限
			dest.x = std::max(static_cast<double>(m_puzzleViewportRect.leftX()), dest.x);
			dest.x = std::min(dest.x, static_cast<double>(m_puzzleViewportRect.rightX()));
			dest.y = std::max(static_cast<double>(m_puzzleViewportRect.topY()), dest.y);
			dest.y = std::min(dest.y, static_cast<double>(m_puzzleViewportRect.bottomY()));
			//Print << dest << pos;
			m_pieces[m_selectedPieceIndex].poly.moveBy(dest - pos);
			m_selectedPieceIndex = -1;

			checkPuzzleClear();
		}
		// ドラッグされている間、位置をマウスに追従させる
		else {
			m_pieces[m_selectedPieceIndex].poly.moveBy(Cursor::DeltaF());
		}
	}
	else {
		for (int idx = 0; auto& piece : m_pieces) {
			if (piece.poly.leftPressed()) {
				m_selectedPieceIndex = idx;
				break;
			}
			++idx;
		}
	}

	if (SimpleGUI::Button(U"Back to level select", Vec2{ 20, 20 })) {
		changeScene(State::Level, 1000);
	}


}

void Game::draw() const
{
	const int g = getData().gridSize;

	FontAsset(U"Bold")(U"ステージ1").draw(48, Arg::bottomLeft(Scene::Width() * .68, Scene::Height() * .14), Palette::Black);

	// グリッドの描画
	m_puzzleViewportRect.drawFrame(2, Palette::Darkgray);

	getData().drawGrid();

	{
		// 減算ブレンドで全ピースを描画
		const ScopedRenderStates2D blend{ BlendState::Subtractive };

		for (const auto& piece : m_pieces) {
			piece.draw();
		}
	}

	// 任意の2つのピースの組み合わせを走査
	for (int i = 0; i < m_pieces.size() - 1; ++i) {
		for (int j = i + 1; j < m_pieces.size(); ++j) {
			const Array<Polygon> intersection_polygon = Geometry2D::And(m_pieces[i].poly, m_pieces[j].poly);
			for (const auto& polygon : intersection_polygon)
			{
				// 共通部分を黒で描画
				polygon.drawFrame(2, Palette::Black);
			}
		}
	}

	// 左側のお手本パネル
	Circle{ g * 6.0, g * 10.0 , g * 5.0}.draw(Palette::Black);
}

bool Game::checkPuzzleClear() const {
	if (m_pieces.isEmpty()) {
		Print << U"Cannot judge clear: no pieces.";
		return false;
	}
	Array<Point> rerativePositions;
	Point firstPos = m_pieces[0].getPrimaryPos().asPoint();
	for (const auto& piece : m_pieces) {
		rerativePositions << piece.getPrimaryPos().asPoint() - firstPos;
	}
	if (rerativePositions.size() != getData().correctPositions.size()) {
		Print << U"Cannot judge clear: size mismatch.";
		return false;
	}
	for (int i = 0; i < rerativePositions.size(); ++i) {
		if (rerativePositions[i] != getData().correctPositions[i]) {
			//Print << U"Piece " << rerativePositions[i] << getData().correctPositions[i] << U" are incorrect.";
			return false;
		}
	}
	Print << U"Puzzle cleared!";
	return true;
};

void Game::updateFadeOut(double t) {
	for (auto& piece : m_pieces) {
		piece.poly.moveBy(Vec2{ 0.0, 1000.0 * t * Scene::DeltaTime() });
	}
}

void Game::drawFadeOut(double t) const {
	draw();
}

# include "Game.hpp"

Game::Game(const InitData& init)
	: IScene{ init }, m_pieces{ getData().pieces }
{
	const double radius = m_puzzleViewport.w / 3.0;
	for (auto&& [i, piece] : IndexedRef(m_pieces)) {
		const Vec2 angle{ std::sin(360_deg * i / m_pieces.size()) ,std::cos(360_deg * i / m_pieces.size()) };
		piece.destPos = m_puzzleViewportDest + angle * radius;
		piece.origPos = m_puzzleViewportDest + angle * radius * 7;
		piece.poly.moveBy(-m_pieces[i].poly.centroid() + angle * radius * 7 + m_puzzleViewportDest);
	}
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
			dest.x = std::max(static_cast<double>(m_puzzleViewport.leftX()), dest.x);
			dest.x = std::min(dest.x, static_cast<double>(m_puzzleViewport.rightX()));
			dest.y = std::max(static_cast<double>(m_puzzleViewport.topY()), dest.y);
			dest.y = std::min(dest.y, static_cast<double>(m_puzzleViewport.bottomY()));
			//Print << dest << pos;
			m_pieces[m_selectedPieceIndex].poly.moveBy(dest - pos);
			m_selectedPieceIndex = -1;

			m_clearTransition.update(checkPuzzleClear());
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

	if (not m_clearTransition.isZero()) {
		m_deltaT = 0;
		changeScene(State::Level, 5000);
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
	getData().drawGrid();

	{
		// 減算ブレンドで全ピースを描画
		const ScopedRenderStates2D blend{ BlendState::Subtractive };

		for (const auto& piece : m_pieces) {
			piece.draw();
			piece.poly.drawFrame(2, ColorF{ 47.0 / 255.0 });
		}
	}

	Polygon subedView = m_answerViewport.asPolygon(30);

	// 任意の2つのピースの組み合わせを走査
	for (int i = 0; i < m_pieces.size() - 1; ++i) {
		for (int j = i + 1; j < m_pieces.size(); ++j) {
			if (m_pieces[i].color == m_pieces[j].color) {
				continue;
			}
			const Array<Polygon> intersection_polygon = Geometry2D::And(
				m_pieces[i].poly.movedBy(-m_pieces[i].getPrimaryPos() + m_answerViewportDest - Vec2{ g * 1, g * 2 } + getData().correctPositions[i]),
				m_pieces[j].poly.movedBy(-m_pieces[j].getPrimaryPos() + m_answerViewportDest - Vec2{ g * 1, g * 2 } + getData().correctPositions[j])
			);
			for (const auto& polygon : intersection_polygon)
			{
				// 共通部分を黒縁で描画
				//polygon.drawFrame(2, ColorF{ 47.0 / 255.0 });
				subedView = Geometry2D::Subtract(subedView, polygon)[0];
			}
		}
	}

	subedView.draw(Palette::Black);
	//subedView.drawWireframe(2, Palette::Gray).draw(Palette::Lightgray);

	// 左側のお手本パネル
	//m_answerViewport.draw(Palette::Black);



	{
		const ScopedRenderStates2D blend{ BlendState::Subtractive };
		for (const auto&& [i, piece] : IndexedRef(m_pieces)) {
			piece.poly.movedBy(-piece.getPrimaryPos() + m_answerViewportDest - Vec2{ g * 1 , g * 2 } + getData().correctPositions[i]).draw(piece.color);
		}
	}
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
	//Print << U"Puzzle cleared!";
	//const Audio audio{ U"audio/clear.mp3" };
	//audio.play();
	return true;
};

void Game::updateFadeIn(double t) {
	for (auto&& [i, piece] : IndexedRef(m_pieces)) {
		piece.poly.moveBy((piece.destPos - piece.origPos) * (t - m_deltaT) * (2 * t));
	}
	//m_answerViewport.moveBy((m_answerViewportDest - m_answerViewportOrig) * (t - m_deltaT) * (3 * t * t));
	//ClearPrint();
	//Print << t - m_deltaT;
	m_deltaT = t;
}

void Game::drawFadeIn(double t) const {
	draw();
}

void Game::updateFadeOut(double t) {
	for (auto&& [i, piece] : IndexedRef(m_pieces)) {
		piece.poly.moveBy((piece.origPos - piece.destPos ) * (t - m_deltaT) * (3 * t * t));
	}
	m_deltaT = t;
}

void Game::drawFadeOut(double t) const {
	draw();
}

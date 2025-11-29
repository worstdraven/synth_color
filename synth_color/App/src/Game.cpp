# include "Game.hpp"

Game::Game(const InitData& init)
	: IScene{ init }, m_pieces{ getData().pieces }
{
	getData().loadLevelDesign();

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
	getData().pieces.clear();
	getData().correctPositions.clear();
	getData().currentLevel++;
}

void Game::update()
{
	const int g = getData().gridSize;

	// 図形のドラッグ処理
	if (not m_clearTransition.isZero()) {
		// クリア判定時は一定時間演出
		m_clearTransition.update(true);
	}
	else if (m_selectedPieceIndex != -1) {
		// マウスが離されたときの処理
		if (MouseL.up()) {
			Vec2 pos = m_pieces[m_selectedPieceIndex].getPrimaryPos();
			// グリッドにスナップ
			Vec2 dest{ std::ceil(pos.x / g - 0.5) * g, ::ceil(pos.y / g - 0.5) * g };
			// ビューポート外に出ないように制限
			dest.x = std::max(m_puzzleViewport.leftX(), dest.x);
			dest.x = std::min(dest.x, m_puzzleViewport.rightX());
			dest.y = std::max(m_puzzleViewport.topY(), dest.y);
			dest.y = std::min(dest.y, m_puzzleViewport.bottomY());
			//Print << dest << pos;
			m_pieces[m_selectedPieceIndex].poly.moveBy(dest - pos);
			m_selectedPieceIndex = -1;

			m_clearTransition.update(checkPuzzleClear());
		}
		else {
			// ドラッグされている間、位置をマウスに追従させる
			m_pieces[m_selectedPieceIndex].poly.moveBy(Cursor::DeltaF());
		}
	}
	else {
		for (int idx = 0; auto& piece : m_pieces) {
			// 選択されたピースを記憶
			if (piece.poly.leftPressed()) {
				m_selectedPieceIndex = idx;
				break;
			}
			++idx;
		}
	}

	if (m_clearTransition.isOne()) {
		// トランジション中のデルタ時間を初期化
		m_deltaT = 0;
		// クリア情報を記録
		getData().isCleared[getData().currentLevel] = true;
		// 次のステージにシーン切り替え
		changeScene(State::Game, ChangeSceneDuration);
	}

	if (SimpleGUI::Button(U"Back to level select", Vec2{ 20, 20 })) {
		// ステージ選択画面に戻る
		changeScene(State::Level, ChangeSceneDuration);
	}
}

void Game::draw() const
{
	const int g = getData().gridSize;

	FontAsset(U"Bold")(U"ステージ {}"_fmt(getData().currentLevel + 1))
		.draw(48, Arg::bottomLeft(Width() * 0.68, Height() * 0.14), ColorF{ 0.0, m_changeSceneTransition.value() });

	// グリッドの描画
	getData().drawGrid();

	// 減算ブレンドで全ピースを描画
	{
		const ScopedRenderStates2D blend{ BlendState::Subtractive };
		for (const auto& piece : m_pieces) {
			piece.draw();
		}
	}
	// お手本ビューポートを描写
	m_answerViewport.draw(Palette::Black);
	// 任意の2つのピースの組み合わせを走査
	for (int i = 0; i < m_pieces.size() - 1; ++i) {
		for (int j = i + 1; j < m_pieces.size(); ++j) {
			if (m_pieces[i].color == m_pieces[j].color) {
				// 同じ色のピースは混色しないので無視
				continue;
			}
			// 共通部分を取得
			const Array<Polygon> intersectionPolygons = Geometry2D::And(
				m_pieces[i].poly.movedBy(-m_pieces[i].getPrimaryPos() + m_answerViewportDest - getData().correctCenter + getData().correctPositions[i]),
				m_pieces[j].poly.movedBy(-m_pieces[j].getPrimaryPos() + m_answerViewportDest - getData().correctCenter + getData().correctPositions[j])
			);
			for (const auto& polygon : intersectionPolygons) {
				// 共通部分を白で塗りつぶす
				polygon.draw(ColorF(1.0).withA(m_changeSceneTransition.value()));
			}
		}
	}
	// 共通部分を正解の色で塗る
	{
		const ScopedRenderStates2D blend{ BlendState::Subtractive };
		for (const auto&& [i, piece] : IndexedRef(m_pieces)) {
			piece.poly.movedBy(-piece.getPrimaryPos() + m_answerViewportDest - getData().correctCenter + getData().correctPositions[i]).draw(piece.color);
		}
	}

	if (m_clearTransition.isOne()) {
		printf("stop\n");
	}
	if (not m_clearTransition.isZero()) {
		ScopedLightBloom target{ m_lightBloom };
		//m_pieces[0].poly.draw(ColorF{ 1.0, Periodic::Sine0_1(2.0s, Scene::Time()) });
		for (int i = 0; i < m_pieces.size() - 1; ++i) {
			for (int j = i + 1; j < m_pieces.size(); ++j) {
				if (m_pieces[i].color == m_pieces[j].color) {
					// 同じ色のピースは混色しないので無視
					continue;
				}
				//const Array<Polygon> intersectionPolygons = Geometry2D::And(m_pieces[i].poly, m_pieces[j].poly);
				//for (const auto& polygon : intersectionPolygons) {
				for (const auto& polygon : Geometry2D::And(m_pieces[i].poly, m_pieces[j].poly)) {
					//polygon.draw(ColorF{ 0.4, EaseOutQuint(m_clearTransition.value()) });
					polygon.draw(getDimmedColorF(getAddictiveColorF(m_pieces[i].color, m_pieces[j].color)).withA(0.8 * EaseOutQuint(m_clearTransition.value())));
					//polygon.draw(getAddictiveColorF(m_pieces[i].color, m_pieces[j].color).withA(EaseOutQuint(m_clearTransition.value())));
					//polygon.drawFrame(20, ColorF{ 0.9, 0.85, 0.1, EaseOutQuint(m_clearTransition.value()) }); // 黄色に縁を光らせる
					//polygon.drawFrame(10, getAddictiveColorF(m_pieces[i].color, m_pieces[j].color).withA(EaseOutQuint(m_clearTransition.value())));

				}
			}
		}
	}
	Circle circle{ ratioVec(0.5, 0.5), getData().gridSize * 2.0 };
	circle.draw(getDimmedColorF(getAddictiveColorF(m_pieces[0].color, m_pieces[1].color)));
	m_lightBloom.draw();
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
			return false;
		}
	}
	//Print << U"Puzzle cleared!";
	m_clearAudio.play();
	return true;
};

void Game::updateFadeIn(double t) {
	m_changeSceneTransition.update(true);
	for (auto&& [i, piece] : IndexedRef(m_pieces)) {
		piece.poly.moveBy((piece.destPos - piece.origPos) * (t - m_deltaT) * (2 * t));
	}
	m_deltaT = t;
}

void Game::updateFadeOut(double t) {
	m_changeSceneTransition.update(false);
	m_clearTransition.update(true);
	for (auto&& [i, piece] : IndexedRef(m_pieces)) {
		piece.poly.moveBy((piece.origPos - piece.destPos) * (t - m_deltaT) * (3 * t * t));
	}
	m_deltaT = t;
}

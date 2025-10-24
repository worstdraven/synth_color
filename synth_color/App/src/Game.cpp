# include "Game.hpp"

Game::Game(const InitData& init)
	: IScene{ init }
{
	for (int32 y = 0; y < 5; ++y)
	{
		for (int32 x = 0; x < (800 / BrickSize.x); ++x)
		{
			m_bricks << Rect{ (x * BrickSize.x), (60 + y * BrickSize.y), BrickSize };
		}
	}

	m_polygons << std::make_unique<TrianglePoly>(Vec2{ 100, 100 }, Vec2{ 150, 100 }, Vec2{ 150, 150 }, ColorF{ 1.0, 0.0, 0.0 });
	m_polygons << std::make_unique<TrianglePoly>(Vec2{ 100, 200 }, Vec2{ 150, 200 }, Vec2{ 150, 250 }, ColorF{ 1.0, 0.0, 0.0 });
	m_polygons << std::make_unique<TrianglePoly>(Vec2{ 100, 300 }, Vec2{ 150, 300 }, Vec2{ 150, 350 }, ColorF{ 1.0, 0.0, 0.0 });
	m_polygons << std::make_unique<TrianglePoly>(Vec2{ 100, 400 }, Vec2{ 150, 400 }, Vec2{ 150, 450 }, ColorF{ 1.0, 0.0, 0.0 });
}

void Game::update()
{
	// ボールを移動させる
	m_ball.moveBy(m_ballVelocity * Scene::DeltaTime());

	// ブロックを順にチェックする
	for (auto it = m_bricks.begin(); it != m_bricks.end(); ++it)
	{
		// ブロックとボールが交差していたら
		if (it->intersects(m_ball))
		{
			// ブロックの上辺、または底辺と交差していたら
			if (it->bottom().intersects(m_ball) || it->top().intersects(m_ball))
			{
				m_ballVelocity.y *= -1;
			}
			else // ブロックの左辺または右辺と交差していたら
			{
				m_ballVelocity.x *= -1;
			}

			// ブロックを配列から削除する（イテレータは無効になる）
			m_bricks.erase(it);

			m_brickSound.playOneShot(0.5);

			++m_score;

			break;
		}
	}

	// 天井にぶつかったら
	if ((m_ball.y < 0) && (m_ballVelocity.y < 0))
	{
		m_ballVelocity.y *= -1;
	}

	// 左右の壁にぶつかったら
	if (((m_ball.x < 0) && (m_ballVelocity.x < 0))
		|| ((800 < m_ball.x) && (0 < m_ballVelocity.x)))
	{
		m_ballVelocity.x *= -1;
	}

	// パドルにあたったらはね返る
	if (const Rect paddle = getPaddle();
		(0 < m_ballVelocity.y) && paddle.intersects(m_ball))
	{
		// パドルの中心からの距離に応じてはね返る方向を変える
		m_ballVelocity = Vec2{ (m_ball.x - paddle.center().x) * 10, -m_ballVelocity.y }.setLength(BallSpeed);
	}

	for (auto& poly : m_polygons) {
		poly->update();
	}
}

void Game::draw() const
{
	Scene::SetBackground(ColorF{ 0.2 });

	// すべてのブロックを描画する
	for (const auto& brick : m_bricks)
	{
		brick.stretched(-1).draw(HSV{ brick.y - 40 });
	}

	// ボールを描く
	m_ball.draw();

	// パドルを描く
	getPaddle().rounded(3).draw();


	// スコアを描く
	FontAsset(U"Bold")(m_score).draw(24, Vec2{ 400, 16 });

	for (const auto& poly : m_polygons) {
		poly->draw();
	}
}

Rect Game::getPaddle() const
{
	return{ Arg::center(Cursor::Pos().x, 500), 60, 10 };
}

TrianglePoly::TrianglePoly(const Vec2& p1, const Vec2& p2, const Vec2& p3, const ColorF& color)
	: BasePoly{ color }, m_p1(p1), m_p2(p2), m_p3(p3) {
}

void TrianglePoly::update() {
	// Current mouse position (float)
	const Vec2 mouse = Cursor::PosF();

	// Triangle built from current vertices
	const Triangle tri{ m_p1, m_p2, m_p3 };

	// Single-instance drag state (static so state persists across frames)
	static bool sDragging = false;
	static Vec2 sGrabOffset{ 0.0, 0.0 };

	// Start dragging if mouse pressed inside triangle
	if (MouseL.down())
	{
		if (tri.contains(mouse))
		{
			sDragging = true;
			const Vec2 center = (m_p1 + m_p2 + m_p3) / 3.0;
			sGrabOffset = mouse - center;
		}
	}

	// While dragging, move the triangle to follow the mouse (preserve initial grab offset)
	if (sDragging)
	{
		if (MouseL.pressed())
		{
			const Vec2 center = (m_p1 + m_p2 + m_p3) / 3.0;
			const Vec2 newCenter = mouse - sGrabOffset;
			const Vec2 delta = newCenter - center;

			m_p1 += delta;
			m_p2 += delta;
			m_p3 += delta;
		}
		else if (MouseL.up())
		{
			// End dragging on button release
			sDragging = false;
		}
	}
}

void TrianglePoly::draw() const {
	Triangle{ m_p1, m_p2, m_p3 }.draw(m_color);
}

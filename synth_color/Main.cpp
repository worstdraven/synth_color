# include "App/src/Common.hpp"
# include "App/src/Title.hpp"
# include "App/src/Level.hpp"
# include "App/src/Game.hpp"
# include "App/src/Ranking.hpp"

void Main()
{
	//FontAsset::Register(U"TitleFont", FontMethod::MSDF, 48, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
	//FontAsset::Register(U"TitleFont", FontMethod::MSDF, 48, Typeface::Black);
	//FontAsset(U"TitleFont").setBufferThickness(4);

	FontAsset::Register(U"CommonFont", FontMethod::MSDF, 24, Typeface::Regular);

	FontAsset::Register(U"Bold", FontMethod::MSDF, 48, Typeface::Bold);

	App manager;
	manager.add<Title>(State::Title);
	manager.add<Level>(State::Level);
	manager.add<Game>(State::Game);
	manager.add<Ranking>(State::Ranking);

	manager.init(State::Title, 0.5s);

	Scene::SetBackground(Palette::White);


	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}

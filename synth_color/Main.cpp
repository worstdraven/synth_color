# include "App/src/Common.hpp"
# include "App/src/Title.hpp"
# include "App/src/Game.hpp"
# include "App/src/Ranking.hpp"

void Main()
{
	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 48, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
	FontAsset(U"TitleFont").setBufferThickness(4);

	FontAsset::Register(U"Bold", FontMethod::MSDF, 48, Typeface::Bold);

	App manager;
	//manager.add<Title>(State::Title);
	manager.add<Game>(State::Game);
	manager.add<Ranking>(State::Ranking);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}

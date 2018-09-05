#ifndef __BOT_GAMEPLAYER
#define __BOT_GAMEPLAYER

#include "asl/string.h"
#include "gamemarket.h"

class GamePlayer
{
	friend class GameClient;

	asl::String nick;
	int money, raw, product, plants, autoplants;
	GameMarket trades;
public:
	GamePlayer();
	asl::String getNick() const;
	int getMoney() const;
	int getRaw() const;
	int getProduct() const;
	int getPlants() const;
	int getAutoPlants() const;
	const GameMarket& getTradeResults() const;
};

#endif

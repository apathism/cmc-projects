#include "asl/string.h"
#include "gameplayer.h"

GamePlayer::GamePlayer() {}

asl::String GamePlayer::getNick() const
{
	return nick;
}

int GamePlayer::getMoney() const
{
	return money;
}

int GamePlayer::getRaw() const
{
	return raw;
}

int GamePlayer::getProduct() const
{
	return product;
}

int GamePlayer::getPlants() const
{
	return plants;
}

int GamePlayer::getAutoPlants() const
{
	return autoplants;
}

const GameMarket& GamePlayer::getTradeResults() const
{
	return trades;
}

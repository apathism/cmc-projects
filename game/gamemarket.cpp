#include "gamemarket.h"

GameMarket::GameMarket()
{
	qRaw = qProduct = cRaw = cProduct = 0;
}

int GameMarket::AvailableRaw() const
{
	return qRaw;
}

int GameMarket::AvailableProduct() const
{
	return qProduct;
}

int GameMarket::RawMinCost() const
{
	return cRaw;
}

int GameMarket::ProductMaxCost() const
{
	return cProduct;
}

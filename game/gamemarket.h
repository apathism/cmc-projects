#ifndef __BOT_GAMEMARKET
#define __BOT_GAMEMARKET

class GameMarket
{
	friend class GameClient;

	int qRaw, qProduct;
	int cRaw, cProduct;
public:
	GameMarket();
	int AvailableRaw() const;
	int AvailableProduct() const;
	int RawMinCost() const;
	int ProductMaxCost() const;
};

#endif

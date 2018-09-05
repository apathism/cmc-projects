#ifndef __BOT_GAMECLIENT
#define __BOT_GAMECLIENT

#include "asl/string.h"
#include "asl/streamsocket.h"
#include "connectionsettings.h"
#include "gameplayer.h"
#include "gamemarket.h"

class GameClient
{
	asl::StreamSocket socket;
	asl::Array<GamePlayer> players;
	GameMarket market;
	asl::String nick;
	bool bankrupt, gameover, draw;

	void setNick(const asl::String&);
	void createGame(int);
	void joinGame(const asl::String&);

	void refreshInfo();
	GamePlayer& _getPlayer(const asl::String &nick);

public:
	GameClient(const ConnectionSettings &s);
	~GameClient();

	const GamePlayer& getPlayer(const asl::String &nick) const;
	const asl::Array<GamePlayer>& getPlayers() const;
	const GameMarket& Market() const;

	asl::String getNick() const;

	void Buy(int, int);
	void Sell(int, int);
	void Produce(int);
	void Upgrade();
	void Build();
	void BuildAutomatic();
	void nextTurn();

	bool isBankrupt() const;
	bool gameOver() const;
	bool gameWin() const;
	bool gameLoose() const;
	bool gameDraw() const;
};

#endif

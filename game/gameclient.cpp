#include "asl/array.h"
#include "asl/string.h"
#include "asl/messageexception.h"
#include "gameclient.h"

using namespace asl;

static const char* errInvalidData = "The data recieved from the server has invalid format";
static const char* errGameOver = "The game for you is over, so you can't turn";

void GameClient::setNick(const String &s)
{
	nick = s;
	socket.Clean();
	socket.Send(s);
	if (socket.Recieve().startsWith("%-"))
		throw MessageException("Unable to select such nick");
}

String GameClient::getNick() const
{
	return nick;
}

void GameClient::createGame(int pwait)
{
	socket.Clean();
	socket.Send(".create");
	int players = 0;
	while (players < pwait)
	{
		String st = socket.Recieve();
		if (st.startsWith("@+ JOIN"))
			players++;
		else if (st.startsWith("@- LEFT"))
			players--;
	}
	socket.Clean();
	socket.Send("start");
	while (socket.Recieve() != "& START");
}

void GameClient::joinGame(const String &s)
{
	socket.Clean();
	socket.Send(".join " + s);
	if (socket.Recieve().startsWith("%-"))
		throw MessageException("Unable to join the game");
	while (socket.Recieve() != "& START");
}

GameClient::GameClient(const ConnectionSettings &s): socket(s.getIP(), s.getPort())
{
	bankrupt = gameover = draw = false;
	setNick(s.getNick());
	if (s.createGame())
		createGame(s.getPlayers());
	else if (s.joinGame())
		joinGame(s.getJoinArgument());
	refreshInfo();
}

GameClient::~GameClient()
{
	socket.Send(".quit");
}

void GameClient::refreshInfo()
{
	socket.Clean();
	socket.Send("info");
	String s;
	while (!(s = socket.Recieve()).startsWith("& PLAYERS"))
	{
		if (!s.startsWith("& INFO")) continue;
		Array<String> t = s.Split(' ');
		if (t.Length() != 8)
			throw MessageException(errInvalidData);
		GamePlayer *z;
		try
		{
			z = &_getPlayer(t[2]);
		}
		catch (const MessageException &e)
		{
			GamePlayer tz;
			tz.nick = t[2];
			players.Push(tz);
			z = &_getPlayer(t[2]);
		}
		z->nick = t[2];
		if
			((!t[3].toInt(z->raw)) ||
			(!t[4].toInt(z->product)) ||
			(!t[5].toInt(z->money)) ||
			(!t[6].toInt(z->plants)) ||
			(!t[7].toInt(z->autoplants)))
				throw MessageException(errInvalidData);
	}
	socket.Clean();
	socket.Send("market");
	while (!(s = socket.Recieve()).startsWith("& MARKET"));
	Array<String> t = s.Split(' ');
	if (t.Length() != 6)
		throw MessageException(errInvalidData);
	if
		((!t[2].toInt(market.qRaw)) ||
		(!t[3].toInt(market.cRaw)) ||
		(!t[4].toInt(market.qProduct)) ||
		(!t[5].toInt(market.cProduct)))
			throw MessageException(errInvalidData);
}

void GameClient::Buy(int q, int c)
{
	if (bankrupt || gameover)
		throw MessageException(errGameOver);
	socket.Clean();
	socket.Send("buy " + String::fromInt(q) + " " + String::fromInt(c));
	for (;;)
	{
		String s = socket.Recieve();
		if (s.startsWith("&-"))
			throw MessageException("Unable to buy the raw");
		if (s.startsWith("& OK")) return;
	}
}

void GameClient::Sell(int q, int c)
{
	if (bankrupt || gameover)
		throw MessageException(errGameOver);
	socket.Clean();
	socket.Send("sell " + String::fromInt(q) + " " + String::fromInt(c));
	for (;;)
	{
		String s = socket.Recieve();
		if (s.startsWith("&-"))
			throw MessageException("Unable to sell the product");
		if (s.startsWith("& OK")) return;
	}
}

void GameClient::Produce(int q)
{
	if (bankrupt || gameover)
		throw MessageException(errGameOver);
	socket.Clean();
	socket.Send("prod " + String::fromInt(q));
	for (;;)
	{
		String s = socket.Recieve();
		if (s.startsWith("&-"))
			throw MessageException("Unable to start production");
		if (s.startsWith("& OK")) return;
	}
}

void GameClient::Upgrade()
{
	if (bankrupt || gameover)
		throw MessageException(errGameOver);
	socket.Clean();
	socket.Send("upgrade");
	for (;;)
	{
		String s = socket.Recieve();
		if (s.startsWith("&-"))
			throw MessageException("Unable to start upgrade of a plant");
		if (s.startsWith("& OK")) return;
	}
}

void GameClient::Build()
{
	if (bankrupt || gameover)
		throw MessageException(errGameOver);
	socket.Clean();
	socket.Send("build");
	for (;;)
	{
		String s = socket.Recieve();
		if (s.startsWith("&-"))
			throw MessageException("Unable to build a new plant");
		if (s.startsWith("& OK")) return;
	}
}

void GameClient::BuildAutomatic()
{
	if (bankrupt || gameover)
		throw MessageException(errGameOver);
	socket.Clean();
	socket.Send("abuild");
	for (;;)
	{
		String s = socket.Recieve();
		if (s.startsWith("&-"))
			throw MessageException("Unable to build a new automatic");
		if (s.startsWith("& OK")) return;
	}
}



void GameClient::nextTurn()
{
	if (bankrupt || gameover)
		throw MessageException(errGameOver);
	socket.Clean();
	socket.Send("turn");
	String s;
	bool notf = true;
	for (int j=0; j<players.Length(); ++j)
		players[j].trades = GameMarket();
	while (((s = socket.Recieve()) != "&- Unknown command") || notf)
	{
		if (s.startsWith("& BANKRUPT"))
		{
			String t = s.Split(' ')[2];
			if (t == nick)
				bankrupt = true;
		}
		else if (s.startsWith("& WINNER") || s.startsWith("& YOU_WIN"))
			gameover = true;
		else if (s.startsWith("& NOWINNER"))
		{
			gameover = true;
			draw = true;
		}
		else if (s.startsWith("& ENDTURN"))
		{
			socket.Send("ping");
			notf = false;
		}
		else if (s.startsWith("& BOUGHT"))
		{
			Array<String> t = s.Split(' ');
			if (t.Length() != 5)
				throw MessageException(errInvalidData);
			GameMarket &tr = _getPlayer(t[2]).trades;
			if
				((!t[3].toInt(tr.qRaw)) ||
				(!t[4].toInt(tr.cRaw)))
					throw MessageException(errInvalidData);
		}
		else if (s.startsWith("& SOLD"))
		{
			Array<String> t = s.Split(' ');
			if (t.Length() != 5)
				throw MessageException(errInvalidData);
			GameMarket &tr = _getPlayer(t[2]).trades;
			if
				((!t[3].toInt(tr.qProduct)) ||
				(!t[4].toInt(tr.cProduct)))
					throw MessageException(errInvalidData);
		}
	}
	if (!gameover) refreshInfo();
}

const GamePlayer& GameClient::getPlayer(const String &s) const
{
	for (int i=0; i<players.Length(); ++i)
		if (players[i].getNick() == s) return players[i];
	throw MessageException("No such player found");
}

GamePlayer& GameClient::_getPlayer(const String &s)
{
	for (int i=0; i<players.Length(); ++i)
		if (players[i].getNick() == s) return players[i];
	throw MessageException("No such player found");
}

const GameMarket& GameClient::Market() const
{
	return market;
}

bool GameClient::isBankrupt() const
{
	return bankrupt;
}

bool GameClient::gameOver() const
{
	return gameover;
}

bool GameClient::gameWin() const
{
	return gameover && !bankrupt;
}

bool GameClient::gameLoose() const
{
	return bankrupt && gameover && !draw;
}

bool GameClient::gameDraw() const
{
	return draw;
}

const Array<GamePlayer>& GameClient::getPlayers() const
{
	return players;
}

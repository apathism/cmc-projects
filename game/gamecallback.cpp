#include "gamecallback.h"
#include "script/runtimeexception.h"
#include "script/polizconst.h"

#define CHECKARGS(x) if (args.Length() != (x)) throw \
	RuntimeException(f,errWrongArgc)
#define PUSHOK stack.Push(PolizConst("1"))
#define GETARG(x) (dynamic_cast < StackItem& > (args[x]) ).getValue(var)

using namespace asl;
using namespace script;

static const char *errWrongArgc = "wrong number of arguments";
static const char *errNoSuchFunction = "there is no such function available";

GameCallback::GameCallback(GameClient &client)
{
	callback = 0;
	this->client = &client;
}

void GameCallback::Execute(const PolizItem &f, const String &fname, Array<String> &var, PointerArray &args, PointerArray &stack) const
{
	if (fname == "MyNick")
	{
		CHECKARGS(0);
		stack.Push(PolizConst(client->getNick()));
	}
	else if (fname == "Buy")
	{
		CHECKARGS(2);
		client->Buy(GETARG(1).toIntForce(),GETARG(0).toIntForce());
		PUSHOK;
	}
	else if (fname == "Sell")
	{
		CHECKARGS(2);
		client->Sell(GETARG(1).toIntForce(),GETARG(0).toIntForce());
		PUSHOK;
	}
	else if (fname == "Produce")
	{
		CHECKARGS(1);
		client->Produce(GETARG(0).toIntForce());
		PUSHOK;
	}
	else if (fname == "Upgrade")
	{
		CHECKARGS(0);
		client->Upgrade();
		PUSHOK;
	}
	else if (fname == "Build")
	{
		CHECKARGS(0);
		client->Build();
		PUSHOK;
	}
	else if (fname == "BuildAuto")
	{
		CHECKARGS(0);
		client->BuildAutomatic();
		PUSHOK;
	}
	else if (fname == "NextTurn")
	{
		CHECKARGS(0);
		client->nextTurn();
		PUSHOK;
	}
	else if (fname == "Bankrupt")
	{
		CHECKARGS(0);
		stack.Push(PolizConst(String::fromInt(
			client->isBankrupt())));
	}
	else if (fname == "GameOver")
	{
		CHECKARGS(0);
		stack.Push(PolizConst(String::fromInt(
			client->gameOver())));
	}
	else if (fname == "GameWin")
	{
		CHECKARGS(0);
		stack.Push(PolizConst(String::fromInt(
			client->gameWin())));
	}
	else if (fname == "GameLoose")
	{
		CHECKARGS(0);
		stack.Push(PolizConst(String::fromInt(
			client->gameLoose())));
	}
	else if (fname == "GameDraw")
	{
		CHECKARGS(0);
		stack.Push(PolizConst(String::fromInt(
			client->gameDraw())));
	}
	else if (fname == "MarketRawAvailable")
	{
		CHECKARGS(0);
		stack.Push(PolizConst(String::fromInt(
			client->Market().AvailableRaw())));
	}
	else if (fname == "MarketProductAvailable")
	{
		CHECKARGS(0);
		stack.Push(PolizConst(String::fromInt(
			client->Market().AvailableProduct())));
	}
	else if (fname == "MarketRawCost")
	{
		CHECKARGS(0);
		stack.Push(PolizConst(String::fromInt(
			client->Market().RawMinCost())));
	}
	else if (fname == "MarketProductCost")
	{
		CHECKARGS(0);
		stack.Push(PolizConst(String::fromInt(
			client->Market().ProductMaxCost())));
	}
	else if (fname == "GetPlayerRaw")
	{
		CHECKARGS(1);
		stack.Push(PolizConst(String::fromInt(
			client->getPlayer(GETARG(0)).getRaw())));
	}
	else if (fname == "GetPlayerProduct")
	{
		CHECKARGS(1);
		stack.Push(PolizConst(String::fromInt(
			client->getPlayer(GETARG(0)).getRaw())));
	}
	else if (fname == "GameWin")
	{
		CHECKARGS(0);
		stack.Push(PolizConst(String::fromInt(
			client->gameWin())));
	}
	else if (fname == "GameLoose")
	{
		CHECKARGS(0);
		stack.Push(PolizConst(String::fromInt(
			client->gameLoose())));
	}
	else if (fname == "GameDraw")
	{
		CHECKARGS(0);
		stack.Push(PolizConst(String::fromInt(
			client->gameDraw())));
	}
	else if (fname == "PlayersInGame")
	{
		CHECKARGS(0);
		stack.Push(PolizConst(String::fromInt(client->getPlayers().Length())));
	}
	else if (fname == "PlayerNick")
	{
		CHECKARGS(1);
		stack.Push(PolizConst(client->getPlayers()[GETARG(0).toIntForce()].getNick()));
	}
	else if (fname == "GetPlayerMoney")
	{
		CHECKARGS(1);
		stack.Push(PolizConst(String::fromInt(
			client->getPlayer(GETARG(0)).getMoney())));
	}
	else if (fname == "GetPlayerPlants")
	{
		CHECKARGS(1);
		stack.Push(PolizConst(String::fromInt(
			client->getPlayer(GETARG(0)).getPlants())));
	}
	else if (fname == "GetPlayerAutoPlants")
	{
		CHECKARGS(1);
		stack.Push(PolizConst(String::fromInt(
			client->getPlayer(GETARG(0)).getAutoPlants())));
	}
	else if (fname == "GetPlayerBoughtAmount")
	{
		CHECKARGS(1);
		stack.Push(PolizConst(String::fromInt(
			client->getPlayer(GETARG(0)).getTradeResults().AvailableRaw())));
	}
	else if (fname == "GetPlayerSoldAmount")
	{
		CHECKARGS(1);
		stack.Push(PolizConst(String::fromInt(
			client->getPlayer(GETARG(0)).getTradeResults().AvailableProduct())));
	}
	else if (fname == "GetPlayerBoughtCost")
	{
		CHECKARGS(1);
		stack.Push(PolizConst(String::fromInt(
			client->getPlayer(GETARG(0)).getTradeResults().RawMinCost())));
	}
	else if (fname == "GetPlayerSoldCost")
	{
		CHECKARGS(1);
		stack.Push(PolizConst(String::fromInt(
			client->getPlayer(GETARG(0)).getTradeResults().ProductMaxCost())));
	}
	else
	{
		if (!callback) throw RuntimeException(f,errNoSuchFunction);
		callback->Execute(f, fname, var, args, stack);
	}
}

void GameCallback::setNextCallback(Callback *callback)
{
	this->callback = callback;
}

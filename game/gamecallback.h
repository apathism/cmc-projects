#ifndef __BOT_GAMECALLBACK
#define __BOT_GAMECALLBACK

#include "script/callback.h"
#include "gameclient.h"

class GameCallback: public script::Callback
{
	script::Callback* callback;
	GameClient *client;
public:
	GameCallback(GameClient&);
	void Execute(const script::PolizItem&, const asl::String&,
		asl::Array<asl::String>&, asl::PointerArray&, asl::PointerArray&) const;
	void setNextCallback(script::Callback*);
};

#endif

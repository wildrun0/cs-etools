#include <core.h>
#include <plugin.h>
#include <command.h>
#include <world.h>
#include <client.h>
#include <log.h>

Plugin_SetVersion(1);
Plugin_SetURL("https://github.com/wildrun0/cs-etools");

COMMAND_FUNC(Clear) {
	COMMAND_SETUSAGE("/clear");
	Client *cl = ccdata->caller;
	for (int i = 0; i < 12; i++ ) { Client_Chat(cl, MESSAGE_TYPE_CHAT, " "); }
}

COMMAND_FUNC(Tp) {
	COMMAND_SETUSAGE("/Tp <to> or /Tp <whom> <to>");

	Client *targ = ccdata->caller;
	Client *subj;

	cs_char u1[64];
	cs_char u2[64];
	cs_char teleportString[64];
	if(COMMAND_GETARG(u1, 64, 0)) {
		subj = Client_GetByName(u1);
		String_FormatBuf(teleportString, 64, "&aTeleported to &e%s", u1);
		if(COMMAND_GETARG(u2, 64, 1)) {
			targ = subj;
			subj = Client_GetByName(u2);
			String_FormatBuf(teleportString, 64, "&e%s &awas teleported to &e%s", u1, u2);
		}
	}
	if (!subj || !targ) {
		COMMAND_PRINT("&cPlayer not found");
	} else {
		World *targWorld = Client_GetWorld(targ);
		World *subjWorld = Client_GetWorld(subj);

		Vec subjPos;
		Ang targAng;
		Client_GetPosition(subj, &subjPos, NULL);
		Client_GetPosition(targ, NULL, &targAng);

		if (targWorld != subjWorld) {
			Client_Chat(targ, MESSAGE_TYPE_CHAT, "&eTeleporting to player's world");
			Client_ChangeWorld(targ, subjWorld);
		}
		Client_TeleportTo(targ, &subjPos, &targAng);
		COMMAND_PRINT(teleportString);

	}
	COMMAND_PRINTUSAGE;
}


COMMAND_FUNC(TpPos) {
	COMMAND_SETUSAGE("/TpPos <x> <y> <z> or /TpPos <whom> <x> <y> <z>");
	Client *player = ccdata->caller;

	cs_char data[16];

	Vec newPos;
	Ang myAng;

	cs_char teleportString[64];
	if (COMMAND_GETARG(data, 16, 3)){
		newPos.z = String_ToFloat(data);
		if (COMMAND_GETARG(data, 16, 0)){
			player = Client_GetByName(data);
			if (!player) { COMMAND_PRINT("&cPlayer not found"); }
			if (COMMAND_GETARG(data, 16, 1)){
				newPos.x = String_ToFloat(data);
				if (COMMAND_GETARG(data, 16, 2)){
					newPos.y = String_ToFloat(data)+2;
				}
			}
		}
		String_FormatBuf(teleportString, 64, "&e%s &awas teleported to %.3f %.3f %.3f", Client_GetName(player), newPos.x, newPos.y-2, newPos.z);
	} else {
		if (COMMAND_GETARG(data, 16, 0)){
			newPos.x = String_ToFloat(data);
			if (COMMAND_GETARG(data, 16, 1)){
				newPos.y = String_ToFloat(data)+2;
				if (COMMAND_GETARG(data, 16, 2)){
					newPos.z = String_ToFloat(data);
				}
			}
		}
		String_FormatBuf(teleportString, 64, "&aTeleported to %.3f %.3f %.3f", newPos.x, newPos.y-2, newPos.z);
	}
	if ((ccdata->args) != NULL ) {
		Client_GetPosition(player, NULL, &myAng);
		Client_TeleportTo(player, &newPos, &myAng);
		COMMAND_PRINT(teleportString);
	}
	COMMAND_PRINTUSAGE;
}


Command_DeclareBunch (Commands) { 
	COMMAND_BUNCH_ADD(Clear, CMDF_CLIENT, "Clear chat"),
	COMMAND_BUNCH_ADD(Tp, CMDF_CLIENT, "Teleport to a player"),
	COMMAND_BUNCH_ADD(TpPos, CMDF_CLIENT, "Teleport to specific coords"),
	COMMAND_BUNCH_END
};


cs_bool Plugin_Load(void) {
    return Command_RegisterBunch(Commands);
}

cs_bool Plugin_Unload(cs_bool force) {
	Command_UnregisterBunch(Commands);
	return true;
}

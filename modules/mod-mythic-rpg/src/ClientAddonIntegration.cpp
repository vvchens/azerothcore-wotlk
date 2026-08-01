#include "Player.h"
#include "ScriptMgr.h"
#include "Item.h"
#include "Chat.h"
#include "SpellInfo.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "MythicRPGShared.h"
#include <sstream>

class AddonCommPlayerScript : public PlayerScript
{
public:
    AddonCommPlayerScript() : PlayerScript("AddonCommPlayerScript") { }

    // When a player inspects an item, we can send its custom stats to the client addon.
    // For 3.3.5, Addon Messages (SMSG_MESSAGECHAT with type CHAT_MSG_ADDON) are commonly used.

    // An easy hook is when an item is added to inventory, or we can add a custom command for the addon to request data.
};

class AddonCommCommandScript : public CommandScript
{
public:
    AddonCommCommandScript() : CommandScript("AddonCommCommandScript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> commandTable =
        {
            { "mythicrpg_req", rbac::RBAC_PERM_PLAYER_COMMANDS, false, &HandleAddonRequestCommand, "" }
        };
        return commandTable;
    }

    static bool HandleAddonRequestCommand(ChatHandler* handler, const char* args)
    {
        Player* player = handler->GetSession()->GetPlayer();
        if (!player || !args)
            return false;

        // Expected args: ItemGUID
        uint32 lowGuid = atoi(args);
        ObjectGuid guid = ObjectGuid(HighGuid::Item, 0, lowGuid);

        std::ostringstream ssStats;
        std::ostringstream ssLeg;

        auto itStats = g_ItemStatsCache.find(guid);
        if (itStats != g_ItemStatsCache.end())
        {
            CustomItemStats st = itStats->second;
            if (st.stamina > 0) ssStats << "耐力 +" << st.stamina << " ";
            if (st.strength > 0) ssStats << "力量 +" << st.strength << " ";
            if (st.agility > 0) ssStats << "敏捷 +" << st.agility << " ";
            if (st.intellect > 0) ssStats << "智力 +" << st.intellect << " ";
            if (st.spirit > 0) ssStats << "精神 +" << st.spirit << " ";
        }
        else
        {
            ssStats << "none";
        }

        auto itLeg = g_LegendaryItemPowers.find(guid);
        if (itLeg != g_LegendaryItemPowers.end())
        {
            LegendaryPower lp = itLeg->second;
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(lp.spellId))
            {
                // Note: The spell name will be the server's locale name.
                // A true multi-locale setup would send the spell ID and the addon would resolve the localized name.
                ssLeg << "触发: " << lp.spellId << " (" << lp.procChance << "%)";
            }
            else
            {
                ssLeg << "触发未知法术 (" << lp.procChance << "%)";
            }
        }
        else
        {
            ssLeg << "none";
        }

        // Format: itemLink|stats|legendary
        // Since we can't easily rebuild itemlink from just guid here without searching bags,
        // the client sends itemLink in the command request (e.g. .mythicrpg_req [itemLink] GUID)
        // Wait, command parsing itemlinks is tricky.

        // Let's change the addon to request by sending an Addon message, but 3.3.5 server doesn't have an easy Addon message hook
        // exposed to scripts without modifying core or using WorldScript OnBeforeConfigLoad etc.
        // For simplicity, we just send a Whisper to the player formatted specifically for the addon to intercept.
        // The addon can hide whispers starting with [MythicRPG]

        std::string reply = "[MythicRPG]" + std::to_string(lowGuid) + "|" + ssStats.str() + "|" + ssLeg.str();

        // Send whisper
        WorldPacket data(SMSG_MESSAGECHAT, 200);
        data << uint8(CHAT_MSG_WHISPER);
        data << int32(LANG_UNIVERSAL);
        data << player->GetGUID(); // sender
        data << uint32(0); // chat tag
        data << player->GetName(); // sender name
        data << player->GetGUID(); // target (self)
        data << uint32(reply.length() + 1);
        data << reply;
        data << uint8(0);
        player->GetSession()->SendPacket(&data);

        return true;
    }
};

void AddClientIntegrationScripts()
{
    new AddonCommPlayerScript();
    new AddonCommCommandScript();
}

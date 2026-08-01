#include "Player.h"
#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "Creature.h"
#include "Map.h"
#include "Chat.h"
#include "SpellInfo.h"

static std::map<uint32, uint32> s_InstanceMythicLevels;
static std::set<ObjectGuid> s_ScaledCreatures;

class MythicPlusCommandScript : public CommandScript
{
public:
    MythicPlusCommandScript() : CommandScript("MythicPlusCommandScript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> commandTable =
        {
            { "mythicplus", rbac::RBAC_PERM_PLAYER_COMMANDS, false, &HandleMythicPlusCommand, "" }
        };
        return commandTable;
    }

    static bool HandleMythicPlusCommand(ChatHandler* handler, const char* args)
    {
        Player* player = handler->GetSession()->GetPlayer();
        if (!player)
            return false;

        Map* map = player->GetMap();
        if (!map || !map->IsDungeon())
        {
            handler->PSendSysMessage("You must be inside a dungeon to activate Mythic+.");
            return true;
        }

        uint32 instanceId = map->GetInstanceId();
        if (instanceId == 0)
        {
            handler->PSendSysMessage("This map does not have an instance ID.");
            return true;
        }

        if (!args || !*args)
        {
            uint32 currentLevel = s_InstanceMythicLevels[instanceId];
            handler->PSendSysMessage("Current Mythic+ level for this instance is %u.", currentLevel);
            return true;
        }

        uint32 level = atoi(args);
        s_InstanceMythicLevels[instanceId] = level;
        handler->PSendSysMessage("Mythic+ level set to %u for this instance.", level);
        return true;
    }
};

class MythicPlusAllCreatureScript : public AllCreatureScript
{
public:
    MythicPlusAllCreatureScript() : AllCreatureScript("MythicPlusAllCreatureScript") { }

    void OnAllCreatureUpdate(Creature* creature, uint32 /*diff*/) override
    {
        if (s_ScaledCreatures.find(creature->GetGUID()) != s_ScaledCreatures.end())
            return;

        Map* map = creature->GetMap();
        if (!map || !map->IsDungeon())
        {
            s_ScaledCreatures.insert(creature->GetGUID());
            return;
        }

        uint32 instanceId = map->GetInstanceId();
        if (instanceId == 0)
        {
            s_ScaledCreatures.insert(creature->GetGUID());
            return;
        }

        auto it = s_InstanceMythicLevels.find(instanceId);
        if (it != s_InstanceMythicLevels.end())
        {
            uint32 level = it->second;
            if (level > 0)
            {
                float multiplier = 1.0f + (level * 0.15f); // +15% per level

                uint32 newHealth = creature->GetMaxHealth() * multiplier;
                creature->SetMaxHealth(newHealth);
                creature->SetHealth(newHealth);

                float minDmg = creature->GetBaseDamage(BASE_ATTACK, MINDAMAGE) * multiplier;
                float maxDmg = creature->GetBaseDamage(BASE_ATTACK, MAXDAMAGE) * multiplier;
                creature->SetBaseDamage(BASE_ATTACK, MINDAMAGE, minDmg);
                creature->SetBaseDamage(BASE_ATTACK, MAXDAMAGE, maxDmg);
            }
        }

        s_ScaledCreatures.insert(creature->GetGUID());
    }
};

void AddMythicPlusScripts()
{
    new MythicPlusCommandScript();
    new MythicPlusAllCreatureScript();
}

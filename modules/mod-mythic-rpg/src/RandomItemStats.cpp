#include "MythicRPGShared.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Item.h"
#include "DatabaseEnv.h"
#include "Chat.h"

// Define custom item stats memory mapping

std::map<ObjectGuid, CustomItemStats> g_ItemStatsCache;

class RandomItemStatsPlayerScript : public PlayerScript
{
public:
    RandomItemStatsPlayerScript() : PlayerScript("RandomItemStatsPlayerScript") { }

    void OnPlayerLootItem(Player* player, Item* item, uint32 /*count*/, ObjectGuid /*lootobj*/) override
    {
        RollStatsForItem(player, item);
    }

    // Also catch items created via craft or quest
    void OnItemCreate(Player* player, Item* item)
    {
        RollStatsForItem(player, item);
    }

    // When an item is equipped, apply the custom stats
    void OnEquip(Player* player, Item* item, uint8 /*bag*/, uint8 /*slot*/, bool /*update*/)
    {
        ApplyItemStats(player, item, true);
    }

    // When an item is unequipped, remove the custom stats
    void OnUnequip(Player* player, Item* item, uint8 /*bag*/, uint8 /*slot*/, bool /*update*/)
    {
        ApplyItemStats(player, item, false);
    }

private:
    void RollStatsForItem(Player* player, Item* item)
    {
        if (!item)
            return;

        // Only roll for equippable gear (armor/weapons)
        if (item->GetTemplate()->Class != ITEM_CLASS_ARMOR && item->GetTemplate()->Class != ITEM_CLASS_WEAPON)
            return;

        // 20% chance to roll extra stats like Diablo
        if (urand(1, 100) > 20)
            return;

        uint32 itemLevel = item->GetTemplate()->ItemLevel;
        uint32 statBudget = itemLevel / 2; // Rough stat budget based on item level

        CustomItemStats stats = {0, 0, 0, 0, 0};

        uint32 numStats = urand(1, 3);
        for (uint32 i = 0; i < numStats; ++i)
        {
            uint32 statType = urand(1, 5);
            uint32 amount = urand(statBudget / 2, statBudget);

            switch (statType)
            {
                case 1: stats.stamina += amount; break;
                case 2: stats.strength += amount; break;
                case 3: stats.agility += amount; break;
                case 4: stats.intellect += amount; break;
                case 5: stats.spirit += amount; break;
            }
        }

        g_ItemStatsCache[item->GetGUID()] = stats;

        // Save to DB
        CharacterDatabase.PExecute("REPLACE INTO item_instance_custom_stats (itemGuid, stamina, strength, agility, intellect, spirit) VALUES (%u, %u, %u, %u, %u, %u)",
            item->GetGUID().GetCounter(), stats.stamina, stats.strength, stats.agility, stats.intellect, stats.spirit);

        ChatHandler(player->GetSession()).PSendSysMessage("You looted an item with random bonus stats!");
    }

    void ApplyItemStats(Player* player, Item* item, bool apply)
    {
        if (!item || !player)
            return;

        auto it = g_ItemStatsCache.find(item->GetGUID());
        if (it == g_ItemStatsCache.end())
            return;

        CustomItemStats stats = it->second;

        player->HandleStatModifier(UNIT_MOD_STAT_STAMINA, BASE_VALUE, float(stats.stamina), apply);
        player->HandleStatModifier(UNIT_MOD_STAT_STRENGTH, BASE_VALUE, float(stats.strength), apply);
        player->HandleStatModifier(UNIT_MOD_STAT_AGILITY, BASE_VALUE, float(stats.agility), apply);
        player->HandleStatModifier(UNIT_MOD_STAT_INTELLECT, BASE_VALUE, float(stats.intellect), apply);
        player->HandleStatModifier(UNIT_MOD_STAT_SPIRIT, BASE_VALUE, float(stats.spirit), apply);
    }
};

class RandomItemStatsWorldScript : public WorldScript
{
public:
    RandomItemStatsWorldScript() : WorldScript("RandomItemStatsWorldScript") { }

    void OnStartup() override
    {
        // Load custom stats from DB on startup
        QueryResult result = CharacterDatabase.Query("SELECT itemGuid, stamina, strength, agility, intellect, spirit FROM item_instance_custom_stats");
        if (!result)
            return;

        do
        {
            Field* fields = result->Fetch();
            ObjectGuid guid = ObjectGuid(HighGuid::Item, 0, fields[0].GetUInt32());

            CustomItemStats stats;
            stats.stamina = fields[1].GetUInt32();
            stats.strength = fields[2].GetUInt32();
            stats.agility = fields[3].GetUInt32();
            stats.intellect = fields[4].GetUInt32();
            stats.spirit = fields[5].GetUInt32();

            g_ItemStatsCache[guid] = stats;
        } while (result->NextRow());
    }
};

void AddRandomItemStatsScripts()
{
    new RandomItemStatsPlayerScript();
    new RandomItemStatsWorldScript();
}

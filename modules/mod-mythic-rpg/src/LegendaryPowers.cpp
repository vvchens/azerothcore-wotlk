#include "MythicRPGShared.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Item.h"
#include "DatabaseEnv.h"
#include "Chat.h"
#include "SpellInfo.h"

// Define our legendary powers mapping
// For example, spell ID 48181 (Haunt), or 42231 (Hurricane), etc.

// Map Item GUID to a Legendary Power
std::map<ObjectGuid, LegendaryPower> g_LegendaryItemPowers;

class LegendaryPowerUnitScript : public UnitScript
{
public:
    LegendaryPowerUnitScript() : UnitScript("LegendaryPowerUnitScript") { }

    void OnDamage(Unit* attacker, Unit* victim, uint32& /*damage*/) override
    {
        if (attacker->GetTypeId() == TYPEID_PLAYER)
        {
            TriggerPowers(attacker->ToPlayer(), victim);
        }
    }

private:
    void TriggerPowers(Player* player, Unit* victim)
    {
        if (!victim)
            return;

        for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
        {
            if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            {
                auto it = g_LegendaryItemPowers.find(item->GetGUID());
                if (it != g_LegendaryItemPowers.end())
                {
                    LegendaryPower power = it->second;
                    float roll = (float)urand(1, 10000) / 100.0f;

                    if (roll <= power.procChance)
                    {
                        // Check if spell isn't already on cooldown for simplicity or to avoid spam
                        if (!player->HasSpellCooldown(power.spellId))
                        {
                            player->CastSpell(victim, power.spellId, true);
                            player->AddSpellCooldown(power.spellId, 0, 5000); // 5 sec internal cd
                        }
                    }
                }
            }
        }
    }
};

class LegendaryPowerPlayerScript : public PlayerScript
{
public:
    LegendaryPowerPlayerScript() : PlayerScript("LegendaryPowerPlayerScript") { }

    void OnLootItem(Player* player, Item* item, uint32 /*count*/, uint64 /*lootguid*/) override
    {
        RollLegendaryPower(player, item);
    }

private:
    void RollLegendaryPower(Player* player, Item* item)
    {
        if (!item)
            return;

        // Only roll for equippable gear
        if (item->GetTemplate()->Class != ITEM_CLASS_ARMOR && item->GetTemplate()->Class != ITEM_CLASS_WEAPON)
            return;

        // 5% chance to roll a legendary power
        if (urand(1, 100) > 5)
            return;

        // Pick a random spell power (these are examples, can be customized or pulled from DB)
        uint32 powerSpells[] = {
            42231, // Hurricane
            48181, // Haunt
            47825, // Soul Fire
            53385, // Divine Storm
            48819, // Consecration
        };

        uint32 numSpells = sizeof(powerSpells) / sizeof(uint32);
        uint32 selectedSpell = powerSpells[urand(0, numSpells - 1)];
        float procChance = (float)urand(5, 15); // 5% to 15% proc chance

        LegendaryPower power = { selectedSpell, procChance };
        g_LegendaryItemPowers[item->GetGUID()] = power;

        // Save to DB
        CharacterDatabase.PExecute("REPLACE INTO item_instance_legendary_powers (itemGuid, spellId, procChance) VALUES (%u, %u, %f)",
            item->GetGUID().GetCounter(), power.spellId, power.procChance);

        ChatHandler(player->GetSession()).PSendSysMessage("You found an item with a Legendary Power!");
    }
};

class LegendaryPowerWorldScript : public WorldScript
{
public:
    LegendaryPowerWorldScript() : WorldScript("LegendaryPowerWorldScript") { }

    void OnStartup() override
    {
        QueryResult result = CharacterDatabase.Query("SELECT itemGuid, spellId, procChance FROM item_instance_legendary_powers");
        if (!result)
            return;

        do
        {
            Field* fields = result->Fetch();
            ObjectGuid guid = ObjectGuid(HighGuid::Item, 0, fields[0].GetUInt32());

            LegendaryPower power;
            power.spellId = fields[1].GetUInt32();
            power.procChance = fields[2].GetFloat();

            g_LegendaryItemPowers[guid] = power;
        } while (result->NextRow());
    }
};

void AddLegendaryPowerScripts()
{
    new LegendaryPowerPlayerScript();
    new LegendaryPowerUnitScript();
    new LegendaryPowerWorldScript();
}

#include "Player.h"
#include "ScriptMgr.h"
#include "Creature.h"
#include "LootMgr.h"
#include "Group.h"
#include "Chat.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "Item.h"
#include "ObjectMgr.h"
#include "Map.h"
#include "LootMgr.h"

// AoE loot is best triggered when the player tries to loot a single corpse.
// To avoid infinite loops, we need a flag or custom handling, but 3.3.5 doesn't easily support opening
// a combined loot window. Instead, we automatically loot everything nearby directly into the bags
// when the player interacts with a single corpse (e.g., via a spell cast or right-click that we hook).

class aoe_loot_commandscript : public CommandScript
{
public:
    aoe_loot_commandscript() : CommandScript("aoe_loot_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> commandTable =
        {
            { "aoeloot", rbac::RBAC_PERM_PLAYER_COMMANDS, false, &HandleAoeLootCommand, "" }
        };
        return commandTable;
    }

    static bool HandleAoeLootCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
        if (!player)
            return false;

        float range = 40.0f;
        std::list<Creature*> creatureList;
        Acore::AnyDeadUnitSpellTargetInRangeCheck checker(player, range, 0, false);
        Acore::CreatureListSearcher<Acore::AnyDeadUnitSpellTargetInRangeCheck> searcher(player, creatureList, checker);
        Cell::VisitGridObjects(player, searcher, range);

        uint32 lootedItemsCount = 0;

        for (Creature* creature : creatureList)
        {
            if (creature->isDead() && creature->hasLootRecipient() && creature->IsLootAllowedFor(player))
            {
                Loot* loot = &creature->loot;

                // If loot hasn't been generated yet, we might need to skip or generate it.
                // Normally it is generated when the creature dies if they are tagged.
                if (!loot->isLooted())
                {
                    if (loot->items.empty() && loot->quest_items.empty() && loot->gold == 0)
                    {
                        // Some creatures don't drop loot, ignore them
                        continue;
                    }

                    if (loot->gold > 0)
                    {
                        player->ModifyMoney(loot->gold);
                        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY, loot->gold);
                        loot->gold = 0;
                    }

                    // Autoloot normal items
                    for (uint32 i = 0; i < loot->items.size(); ++i)
                    {
                        LootItem* item = &loot->items[i];
                        if (item->is_looted || item->freeforall)
                            continue;

                        ItemPosCountVec dest;
                        InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, item->itemid, item->count);
                        if (msg == EQUIP_ERR_OK)
                        {
                            Item* addedItem = player->StoreNewItem(dest, item->itemid, true, item->randomPropertyId);
                            if (addedItem)
                            {
                                player->SendNewItem(addedItem, item->count, true, false);
                                item->is_looted = true;
                                lootedItemsCount++;
                            }
                        }
                    }
                }
            }
        }

        handler->PSendSysMessage("AoE Looted %u items.", lootedItemsCount);
        return true;
    }
};

class AoeLootPlayerScript : public PlayerScript
{
public:
    AoeLootPlayerScript() : PlayerScript("AoeLootPlayerScript") { }

    void OnLootItem(Player* player, Item* item, uint32 count, uint64 lootguid) override
    {
        // When a player successfully loots an item, we can trigger AoE loot to grab the rest
        // However, this might interfere with normal looting. We will keep it simple and provide the command
        // for now, and players can macro it.
    }
};

void AddAoeLootScripts()
{
    new AoeLootPlayerScript();
    new aoe_loot_commandscript();
}

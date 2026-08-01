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
#include "Config.h"

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

        float range = sConfigMgr->GetFloatDefault("MythicRPG.AoeLoot.Range", 40.0f);

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

                if (!loot->isLooted())
                {
                    if (loot->items.empty() && loot->quest_items.empty() && loot->gold == 0)
                    {
                        continue;
                    }

                    if (loot->gold > 0)
                    {
                        player->ModifyMoney(loot->gold);
                        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY, loot->gold);
                        loot->gold = 0;
                    }

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
    }
};

void AddAoeLootScripts()
{
    new AoeLootPlayerScript();
    new aoe_loot_commandscript();
}

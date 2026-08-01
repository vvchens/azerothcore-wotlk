#ifndef MYTHIC_RPG_SHARED_H
#define MYTHIC_RPG_SHARED_H

#include "ObjectGuid.h"
#include <map>

struct CustomItemStats {
    uint32 stamina;
    uint32 strength;
    uint32 agility;
    uint32 intellect;
    uint32 spirit;
};

struct LegendaryPower {
    uint32 spellId;
    float procChance;
};

extern std::map<ObjectGuid, CustomItemStats> g_ItemStatsCache;
extern std::map<ObjectGuid, LegendaryPower> g_LegendaryItemPowers;

#endif

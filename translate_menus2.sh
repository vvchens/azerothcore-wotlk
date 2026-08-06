#!/bin/bash
sed -i 's/"|cFFFFFFFF4.|r Select a dungeon or go random"/"|cFFFFFFFF4.|r 选择一个地下城或随机"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"|cFFFFFFFF1.|r Talk to me"/"|cFFFFFFFF1.|r 跟我交谈"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"|cFFFFFFFF2.|r Pick a difficulty tier"/"|cFFFFFFFF2.|r 选择一个难度等级"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"|cFFFFFFFF3.|r Pick an enemy theme"/"|cFFFFFFFF3.|r 选择一个敌人主题"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"|cFFFFFFFF5.|r Kill everything, collect loot, teleport out!"/"|cFFFFFFFF5.|r 杀光怪物，收集战利品，传送离开！"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp

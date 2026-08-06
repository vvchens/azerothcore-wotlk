#!/bin/bash
sed -i 's/"Undead"/"亡灵"/g' modules/mod-dungeon-master/src/DMConfig.cpp
sed -i 's/"Humanoids"/"人形生物"/g' modules/mod-dungeon-master/src/DMConfig.cpp
sed -i 's/"Demons"/"恶魔"/g' modules/mod-dungeon-master/src/DMConfig.cpp
sed -i 's/"Beasts"/"野兽"/g' modules/mod-dungeon-master/src/DMConfig.cpp
sed -i 's/"Elementals"/"元素生物"/g' modules/mod-dungeon-master/src/DMConfig.cpp
sed -i 's/"Dragonkin"/"龙类"/g' modules/mod-dungeon-master/src/DMConfig.cpp
sed -i 's/"Novice"/"新手"/g' modules/mod-dungeon-master/src/DMConfig.cpp
sed -i 's/"Adept"/"学徒"/g' modules/mod-dungeon-master/src/DMConfig.cpp
sed -i 's/"Veteran"/"老兵"/g' modules/mod-dungeon-master/src/DMConfig.cpp
sed -i 's/"Expert"/"专家"/g' modules/mod-dungeon-master/src/DMConfig.cpp
sed -i 's/"Master"/"大师"/g' modules/mod-dungeon-master/src/DMConfig.cpp
sed -i 's/"Grandmaster"/"宗师"/g' modules/mod-dungeon-master/src/DMConfig.cpp

sed -i 's/"Use Dungeon Difficulty"/"使用地下城难度"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"Scale to Party"/"缩放至小队"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp

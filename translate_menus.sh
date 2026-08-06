#!/bin/bash
sed -i 's/"Never mind"/"不用了"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"Begin Challenge"/"开始挑战"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"|cFF00FFFFRoguelike Mode|r"/"|cFF00FFFFRoguelike 模式|r"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"How does this work?"/"这是怎么运作的？"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"|cFFFFD700Statistics & Leaderboards|r"/"|cFFFFD700统计与排行榜|r"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"|cFFFF0000<< Back|r"/"|cFFFF0000<< 返回|r"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"|cFFFFD700Random Dungeon|r"/"|cFFFFD700随机地下城|r"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"|cFF00FF00<< Previous Page|r"/"|cFF00FF00<< 上一页|r"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"|cFF00FF00Next Page >>|r"/"|cFF00FF00下一页 >>|r"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"|cFF00FF00>> START CHALLENGE <<|r"/"|cFF00FF00>> 开始挑战 <<|r"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"|cFFFF0000<< Cancel|r"/"|cFFFF0000<< 取消|r"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"<< Back"/"<< 返回"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"My Normal Run Stats"/"我的普通挑战统计"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"|cFF00FFFFMy Roguelike Stats|r"/"|cFF00FFFF我的Roguelike统计|r"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"|cFFFFD700Leaderboards|r"/"|cFFFFD700排行榜|r"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp
sed -i 's/"|cFFFFD700View Leaderboards|r"/"|cFFFFD700查看排行榜|r"/g' modules/mod-dungeon-master/src/scripts/npc_dungeon_master.cpp

# MythicRPG 客户端插件

这个插件用于配套服务端的 `mod-mythic-rpg` 模块。它负责将装备的“暗黑随机属性”以及“传奇技能”显示在物品的提示框（Tooltip）上，支持 3.3.5 (12340) 中文客户端。

## 安装方法
1. 复制整个 `MythicRPG` 文件夹。
2. 将其粘贴到你的魔兽世界客户端目录：`World of Warcraft/Interface/AddOns/` 中。
3. 确保目录结构为 `Interface/AddOns/MythicRPG/MythicRPG.toc`。
4. 启动游戏并在插件列表中启用。

## 工作原理
当鼠标悬停在物品上时，如果本地没有缓存该物品的强化信息，插件会发送一条隐藏的聊天指令 `.mythicrpg_req <GUID>` 给服务端。服务端会通过私聊的形式发送格式化的密文信息（例如 `[MythicRPG]xxxx|耐力 +10...`）。插件拦截并解析此密文，然后刷新该物品的 Tooltip 显示。

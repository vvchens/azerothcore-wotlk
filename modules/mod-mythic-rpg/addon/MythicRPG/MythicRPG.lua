local Frame = CreateFrame("Frame")
Frame:RegisterEvent("CHAT_MSG_WHISPER")
Frame:RegisterEvent("CHAT_MSG_SYSTEM")

local CustomItemCache = {}
local PendingRequests = {}

local function ExtractGuidFromLink(itemLink)
    local _, _, color, Ltype, Id, Enchant, Gem1, Gem2, Gem3, Gem4, Suffix, Unique, LinkLvl, Name = string.find(itemLink, "|?c?f?f?(%x*)|?H?([^:]*):(%d+):(%d+):(%d+):(%d+):(%d+):(%d+):(%-?%d+):(%-?%d+):(%d+)|?h?%[?([^%[%]]*)%]?|?h?|?r?")
    -- In 3.3.5, Unique is often the lower 32 bits of the item GUID for instantiated items.
    return Unique
end

Frame:SetScript("OnEvent", function(self, event, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12)
    if event == "CHAT_MSG_WHISPER" then
        local msg = arg1
        local sender = arg2
        if sender == UnitName("player") and string.sub(msg, 1, 11) == "[MythicRPG]" then
            -- Format: [MythicRPG]guid|stats|legendary
            local data = string.sub(msg, 12)
            local guidStr, stats, legendary = strsplit("|", data, 3)
            if guidStr and stats and legendary then
                CustomItemCache[guidStr] = { stats = stats, legendary = legendary }
            end
            -- Return true to hook to suppress chat if needed, but WoW API for chat filters is better.
        end
    end
end)

-- Hide the system whispers from the chat window
ChatFrame_AddMessageEventFilter("CHAT_MSG_WHISPER", function(self, event, msg)
    if string.sub(msg, 1, 11) == "[MythicRPG]" then
        return true -- filter it out
    end
    return false
end)

local function OnTooltipSetItem(tooltip)
    local itemName, itemLink = tooltip:GetItem()
    if not itemLink then return end

    local guidStr = ExtractGuidFromLink(itemLink)
    if not guidStr or guidStr == "0" then return end

    local customInfo = CustomItemCache[guidStr]
    if customInfo then
        if customInfo.stats and customInfo.stats ~= "none" then
            tooltip:AddLine(" ")
            tooltip:AddLine("【暗黑附加属性】", 1, 0.5, 0)
            tooltip:AddLine(customInfo.stats, 1, 1, 1)
        end
        if customInfo.legendary and customInfo.legendary ~= "none" then
            tooltip:AddLine(" ")
            tooltip:AddLine("【传奇技能】", 1, 0.8, 0)
            tooltip:AddLine(customInfo.legendary, 1, 1, 1)
        end
        tooltip:Show()
    else
        -- Request info from server
        if not PendingRequests[guidStr] then
            PendingRequests[guidStr] = true
            SendChatMessage(".mythicrpg_req " .. guidStr, "SAY")
            -- Use Say or Guild chat with a hidden prefix command to trigger the server command
            -- Actually, server handles chat commands starting with '.' from any chat type.
            -- We can just send a SAY message with ".mythicrpg_req guid"
            -- And we should filter our own SAY messages starting with .mythicrpg_req
        end
    end
end

ChatFrame_AddMessageEventFilter("CHAT_MSG_SAY", function(self, event, msg)
    if string.sub(msg, 1, 14) == ".mythicrpg_req" then
        return true
    end
    return false
end)

GameTooltip:HookScript("OnTooltipSetItem", OnTooltipSetItem)
ItemRefTooltip:HookScript("OnTooltipSetItem", OnTooltipSetItem)

print("|cFFFF8000MythicRPG|r addon loaded.")

set(scripts_mythic_rpg
  ${CMAKE_CURRENT_SOURCE_DIR}/src/MP_loader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/AoeLoot.cpp
)

message(STATUS "mod-mythic-rpg loaded.")
set(scripts_mythic_rpg
  ${scripts_mythic_rpg}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/MythicPlus.cpp
)
set(scripts_mythic_rpg
  ${scripts_mythic_rpg}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/RandomItemStats.cpp
)
set(scripts_mythic_rpg
  ${scripts_mythic_rpg}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/LegendaryPowers.cpp
)
set(scripts_mythic_rpg
  ${scripts_mythic_rpg}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ClientAddonIntegration.cpp
)

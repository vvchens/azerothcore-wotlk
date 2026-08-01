CREATE TABLE IF NOT EXISTS `item_instance_legendary_powers` (
  `itemGuid` int(10) unsigned NOT NULL,
  `spellId` int(10) unsigned NOT NULL,
  `procChance` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`itemGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

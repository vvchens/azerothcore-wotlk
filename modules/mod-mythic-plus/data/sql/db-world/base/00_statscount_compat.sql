-- mod-mythic-plus item SQL (09_custom_items, 14_emblem_gear, 16_molten_core_mythic_gear) declares an
-- `item_template`.`StatsCount` column.
-- We add it conditionally.
DROP PROCEDURE IF EXISTS AddStatsCountColumn;
DELIMITER //
CREATE PROCEDURE AddStatsCountColumn()
BEGIN
    IF NOT EXISTS(
        SELECT 1 FROM information_schema.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
        AND TABLE_NAME = 'item_template'
        AND COLUMN_NAME = 'StatsCount'
    ) THEN
        ALTER TABLE `item_template` ADD COLUMN `StatsCount` TINYINT UNSIGNED NOT NULL DEFAULT 0;
    END IF;
END //
DELIMITER ;
CALL AddStatsCountColumn();
DROP PROCEDURE AddStatsCountColumn;

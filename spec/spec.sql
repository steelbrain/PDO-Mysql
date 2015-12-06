create database if not exists `test`;
use `test`;
CREATE TABLE `test` ( `id` INT NOT NULL AUTO_INCREMENT , `name` VARCHAR(255) NOT NULL , PRIMARY KEY (`id`)) ENGINE = InnoDB;
INSERT INTO `test`.`test` (`id`, `name`) VALUES ('1', 'Wow');
INSERT INTO `test`.`test` (`id`, `name`) VALUES ('2', 'Hey');

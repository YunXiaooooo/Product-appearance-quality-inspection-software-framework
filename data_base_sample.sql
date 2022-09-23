/*
SQLyog Professional v12.09 (64 bit)
MySQL - 5.7.38 : Database - db_model
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`db_model` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `db_model`;

/*Table structure for table `bolttest_setting_param` */

DROP TABLE IF EXISTS `bolttest_setting_param`;

CREATE TABLE `bolttest_setting_param` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `Check1` int(10) unsigned NOT NULL DEFAULT '0',
  `param1` int(10) unsigned NOT NULL DEFAULT '0',
  `param2` int(10) unsigned NOT NULL DEFAULT '0',
  `Check2` int(10) unsigned NOT NULL DEFAULT '0',
  `Check3` int(10) unsigned NOT NULL DEFAULT '0',
  `param3` int(10) unsigned NOT NULL DEFAULT '0',
  `param4` int(10) unsigned NOT NULL DEFAULT '0',
  `Check4` int(10) unsigned NOT NULL DEFAULT '0',
  `param5` int(10) unsigned NOT NULL DEFAULT '0',
  `Check5` int(10) unsigned NOT NULL DEFAULT '0',
  `param6` int(10) unsigned NOT NULL DEFAULT '0',
  `param7` int(10) unsigned NOT NULL DEFAULT '0',
  `Check6` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `bolttest_setting_param` */

insert  into `bolttest_setting_param`(`id`,`Check1`,`param1`,`param2`,`Check2`,`Check3`,`param3`,`param4`,`Check4`,`param5`,`Check5`,`param6`,`param7`,`Check6`) values (0,1,14,23,1,0,0,0,0,0,0,0,0,0),(1,0,0,0,0,1,19,29,1,0,0,0,0,0),(2,0,0,0,0,0,0,0,0,0,1,1,3,1),(3,1,13,23,1,0,0,0,0,0,0,0,0,0),(4,0,0,0,0,1,21,28,1,0,0,0,0,0),(5,0,0,0,0,0,0,0,0,0,1,1,3,1);

/*Table structure for table `model_block` */

DROP TABLE IF EXISTS `model_block`;

CREATE TABLE `model_block` (
  `model_id` int(8) NOT NULL DEFAULT '0',
  `id` int(16) NOT NULL DEFAULT '0',
  `x1` int(16) unsigned NOT NULL DEFAULT '0',
  `y1` int(16) unsigned NOT NULL DEFAULT '0',
  `x2` int(16) unsigned NOT NULL DEFAULT '0',
  `y2` int(16) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`model_id`,`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `model_block` */

insert  into `model_block`(`model_id`,`id`,`x1`,`y1`,`x2`,`y2`) values (1,0,147,295,501,590),(0,5,753,1232,1347,1750),(0,4,688,1019,1436,1262),(0,3,843,162,1291,1055),(0,2,750,1239,1339,1740),(0,1,679,1017,1443,1265),(0,0,841,158,1287,1065),(1,1,256,655,769,846),(1,2,1471,809,1738,1150),(2,0,580,246,1107,660),(2,1,1101,765,1259,932),(2,2,850,710,994,845),(2,3,1036,934,1251,1248),(2,4,830,1056,1315,1437);

/*Table structure for table `model_block_to_camera_index` */

DROP TABLE IF EXISTS `model_block_to_camera_index`;

CREATE TABLE `model_block_to_camera_index` (
  `model_id` int(16) NOT NULL DEFAULT '0',
  `block_idx` int(16) NOT NULL DEFAULT '0',
  `camera_idx` int(8) NOT NULL DEFAULT '0',
  PRIMARY KEY (`model_id`,`block_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `model_block_to_camera_index` */

insert  into `model_block_to_camera_index`(`model_id`,`block_idx`,`camera_idx`) values (1,0,0),(0,5,1),(0,4,1),(0,3,1),(0,2,0),(0,1,0),(0,0,0),(1,1,0),(1,2,1),(2,0,0),(2,1,0),(2,2,0),(2,3,1),(2,4,1);

/*Table structure for table `model_camera_to_block_index` */

DROP TABLE IF EXISTS `model_camera_to_block_index`;

CREATE TABLE `model_camera_to_block_index` (
  `model_id` int(16) unsigned NOT NULL DEFAULT '0',
  `camera_idx` int(8) unsigned NOT NULL DEFAULT '0',
  `block_idx` varchar(32) NOT NULL DEFAULT '##',
  PRIMARY KEY (`model_id`,`camera_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `model_camera_to_block_index` */

insert  into `model_camera_to_block_index`(`model_id`,`camera_idx`,`block_idx`) values (1,1,'#2#'),(1,0,'#0#1#'),(0,1,'#3#4#5#'),(0,0,'#0#1#2#'),(2,0,'#0#1#2#'),(2,1,'#3#4#');

/*Table structure for table `model_name` */

DROP TABLE IF EXISTS `model_name`;

CREATE TABLE `model_name` (
  `id` int(8) DEFAULT NULL,
  `name` varchar(16) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `model_name` */

insert  into `model_name`(`id`,`name`) values (0,'boltTest'),(1,'test'),(2,'test3');

/*Table structure for table `system_log` */

DROP TABLE IF EXISTS `system_log`;

CREATE TABLE `system_log` (
  `id` int(32) unsigned NOT NULL AUTO_INCREMENT,
  `log_str` varchar(128) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=96 DEFAULT CHARSET=utf8;

/*Data for the table `system_log` */

insert  into `system_log`(`id`,`log_str`) values (1,'2021-12-22 10:20:16 [info]: System Start\n'),(2,'2021-12-22 10:20:21 [info]: System Close\n'),(3,'2022-04-10 15:28:23 [info]: System Start\n'),(4,'2022-04-10 15:30:05 [info]: System Close\n'),(5,'2022-04-10 15:30:19 [info]: System Start\n'),(6,'2022-04-10 15:32:56 [info]: System Close\n'),(7,'2022-04-10 15:33:28 [info]: System Start\n'),(8,'2022-04-10 15:37:31 [info]: System Close\n'),(9,'2022-04-10 15:37:33 [info]: System Start\n'),(10,'2022-04-10 15:40:43 [info]: System Close\n'),(11,'2022-04-10 18:47:10 [info]: System Start\n'),(12,'2022-04-11 19:45:41 [info]: System Start\n'),(13,'2022-04-11 19:46:26 [info]: System Close\n'),(14,'2022-04-11 19:47:37 [info]: System Start\n'),(15,'2022-04-15 18:20:09 [info]: System Start\n'),(16,'2022-04-17 18:34:22 [info]: System Start\n'),(17,'2022-04-17 18:34:26 [info]: System Close\n'),(18,'2022-04-19 19:06:33 [info]: System Start\n'),(19,'2022-04-19 19:08:25 [info]: System Start\n'),(20,'2022-04-19 19:09:19 [info]: System Start\n'),(21,'2022-04-19 19:13:00 [info]: System Start\n'),(22,'2022-04-19 19:13:11 [info]: System Close\n'),(23,'2022-04-19 19:13:48 [info]: System Start\n'),(24,'2022-04-20 17:27:18 [info]: System Start\n'),(25,'2022-04-20 17:37:28 [info]: System Start\n'),(26,'2022-04-20 17:39:00 [info]: System Start\n'),(27,'2022-04-20 17:41:07 [info]: System Start\n'),(28,'2022-04-20 17:42:40 [info]: System Start\n'),(29,'2022-04-20 18:01:40 [info]: System Start\n'),(30,'2022-04-20 18:07:46 [info]: System Start\n'),(31,'2022-04-20 18:12:02 [info]: System Start\n'),(32,'2022-04-20 18:14:19 [info]: System Start\n'),(33,'2022-04-20 18:16:49 [info]: System Start\n'),(34,'2022-04-20 18:32:31 [info]: System Close\n'),(35,'2022-04-21 17:51:45 [info]: System Start\n'),(36,'2022-04-21 18:02:34 [info]: System Close\n'),(37,'2022-04-21 18:02:40 [info]: System Start\n'),(38,'2022-04-24 20:04:12 [info]: System Start\n'),(39,'2022-04-24 20:04:25 [info]: System Close\n'),(40,'2022-04-29 10:36:55 [info]: System Start\n'),(41,'2022-04-29 11:43:11 [info]: System Close\n'),(42,'2022-06-04 18:45:25 [info]: System Start\n'),(43,'2022-06-04 18:47:19 [info]: System Start\n'),(44,'2022-06-04 18:48:26 [info]: System Close\n'),(45,'2022-06-05 19:26:10 [info]: System Start\n'),(46,'2022-06-05 19:27:07 [info]: System Close\n'),(47,'2022-06-05 19:46:29 [info]: System Start\n'),(48,'2022-06-05 19:50:39 [info]: System Close\n'),(49,'2022-06-06 20:32:46 [info]: System Start\n'),(50,'2022-06-06 20:35:20 [info]: System Start\n'),(51,'2022-06-06 20:36:13 [info]: System Close\n'),(52,'2022-06-06 20:36:24 [info]: System Start\n'),(53,'2022-06-06 20:38:21 [info]: System Close\n'),(54,'2022-06-06 20:38:38 [info]: System Start\n'),(55,'2022-06-06 20:43:19 [info]: System Close\n'),(56,'2022-06-06 20:43:22 [info]: System Start\n'),(57,'2022-06-06 20:46:17 [info]: System Start\n'),(58,'2022-06-06 21:08:46 [info]: System Close\n'),(59,'2022-06-06 21:09:12 [info]: System Start\n'),(60,'2022-06-06 21:14:36 [info]: System Close\n'),(61,'2022-06-08 19:27:37 [info]: System Start\n'),(62,'2022-06-08 19:28:28 [info]: System Close\n'),(63,'2022-06-08 19:30:20 [info]: System Start\n'),(64,'2022-06-08 19:33:10 [info]: System Close\n'),(65,'2022-06-09 20:37:10 [info]: System Start\n'),(66,'2022-06-09 20:52:16 [info]: System Start\n'),(67,'2022-06-09 20:55:03 [info]: System Start\n'),(68,'2022-06-09 21:03:20 [info]: System Start\n'),(69,'2022-06-09 21:04:04 [info]: System Close\n'),(70,'2022-06-09 21:37:27 [info]: System Start\n'),(71,'2022-06-09 21:41:03 [info]: System Close\n'),(72,'2022-06-10 10:38:46 [info]: System Start\n'),(73,'2022-06-10 10:38:50 [info]: System Close\n'),(74,'2022-06-13 11:16:54 [info]: System Start\n'),(75,'2022-06-13 11:19:20 [info]: System Start\n'),(76,'2022-06-13 11:20:28 [info]: System Close\n'),(77,'2022-06-18 21:03:57 [info]: System Start\n'),(78,'2022-06-18 21:07:36 [info]: System Close\n'),(79,'2022-06-18 21:10:30 [info]: System Start\n'),(80,'2022-06-18 21:14:20 [info]: System Close\n'),(81,'2022-06-18 21:14:35 [info]: System Start\n'),(82,'2022-06-18 21:16:53 [info]: System Start\n'),(83,'2022-06-18 21:17:22 [info]: System Close\n'),(84,'2022-06-18 21:21:14 [info]: System Start\n'),(85,'2022-06-18 21:42:11 [info]: System Close\n'),(86,'2022-06-27 10:25:21 [info]: System Start\n'),(87,'2022-06-27 10:27:11 [info]: System Close\n'),(88,'2022-06-27 11:24:19 [info]: System Start\n'),(89,'2022-06-27 11:26:24 [info]: System Close\n'),(90,'2022-06-27 11:29:33 [info]: System Start\n'),(91,'2022-06-27 11:29:44 [info]: System Close\n'),(92,'2022-06-27 11:33:31 [info]: System Start\n'),(93,'2022-06-27 11:35:30 [info]: System Close\n'),(94,'2022-06-27 14:32:52 [info]: System Start\n'),(95,'2022-06-27 14:38:26 [info]: System Close\n');

/*Table structure for table `task_and_user_info` */

DROP TABLE IF EXISTS `task_and_user_info`;

CREATE TABLE `task_and_user_info` (
  `id` int(16) unsigned NOT NULL DEFAULT '0',
  `task_number` varchar(24) NOT NULL DEFAULT '/',
  `user_id` varchar(24) NOT NULL DEFAULT '/',
  `task_quantity` int(16) unsigned NOT NULL DEFAULT '0',
  `already_finish_quantity` int(16) unsigned NOT NULL DEFAULT '0',
  `no_finish_quantity` int(16) unsigned NOT NULL DEFAULT '0',
  `already_ok_quantity` int(16) unsigned NOT NULL DEFAULT '0',
  `already_ng_quantity` int(16) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `task_and_user_info` */

insert  into `task_and_user_info`(`id`,`task_number`,`user_id`,`task_quantity`,`already_finish_quantity`,`no_finish_quantity`,`already_ok_quantity`,`already_ng_quantity`) values (1,'o20220503','u21721337',500,0,500,0,0),(0,'o20220503','u21721331',500,0,500,0,0),(2,'o20220504','u21721331',100,0,100,0,0),(3,'o20220627','u21721331',300,0,300,0,0),(4,'o20220627','u21721337',600,0,600,0,0);

/*Table structure for table `task_info` */

DROP TABLE IF EXISTS `task_info`;

CREATE TABLE `task_info` (
  `id` int(16) unsigned NOT NULL DEFAULT '0',
  `task_number` varchar(24) NOT NULL DEFAULT '/',
  `task_create_time` datetime NOT NULL,
  `task_quantity` int(16) unsigned NOT NULL DEFAULT '0',
  `task_already_finish_quantity` int(16) unsigned NOT NULL DEFAULT '0',
  `task_already_finish_ok_quantity` int(16) unsigned NOT NULL DEFAULT '0',
  `task_already_finish_ng_quantity` int(16) unsigned NOT NULL DEFAULT '0',
  `model_id` int(16) NOT NULL,
  PRIMARY KEY (`id`,`task_number`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `task_info` */

insert  into `task_info`(`id`,`task_number`,`task_create_time`,`task_quantity`,`task_already_finish_quantity`,`task_already_finish_ok_quantity`,`task_already_finish_ng_quantity`,`model_id`) values (0,'o20220503','2022-06-06 21:03:54',1000,0,0,0,0),(1,'o20220504','2022-06-13 11:18:39',100,0,0,0,0),(2,'o20220627','2022-06-27 14:34:45',900,0,0,0,2);

/*Table structure for table `test3_setting_param` */

DROP TABLE IF EXISTS `test3_setting_param`;

CREATE TABLE `test3_setting_param` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `Check1` int(10) unsigned NOT NULL DEFAULT '0',
  `param1` int(10) unsigned NOT NULL DEFAULT '0',
  `param2` int(10) unsigned NOT NULL DEFAULT '0',
  `Check2` int(10) unsigned NOT NULL DEFAULT '0',
  `Check3` int(10) unsigned NOT NULL DEFAULT '0',
  `param3` int(10) unsigned NOT NULL DEFAULT '0',
  `param4` int(10) unsigned NOT NULL DEFAULT '0',
  `Check4` int(10) unsigned NOT NULL DEFAULT '0',
  `param5` int(10) unsigned NOT NULL DEFAULT '0',
  `Check5` int(10) unsigned NOT NULL DEFAULT '0',
  `param6` int(10) unsigned NOT NULL DEFAULT '0',
  `param7` int(10) unsigned NOT NULL DEFAULT '0',
  `Check6` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `test3_setting_param` */

insert  into `test3_setting_param`(`id`,`Check1`,`param1`,`param2`,`Check2`,`Check3`,`param3`,`param4`,`Check4`,`param5`,`Check5`,`param6`,`param7`,`Check6`) values (0,0,0,0,0,0,0,0,0,0,0,0,0,0),(1,1,0,0,0,1,0,0,0,0,0,0,0,0),(2,0,0,0,0,0,0,0,1,0,0,0,0,0),(3,0,0,0,0,1,0,0,0,0,0,0,0,0),(4,0,0,0,1,0,0,0,0,0,0,0,0,0);

/*Table structure for table `test_setting_param` */

DROP TABLE IF EXISTS `test_setting_param`;

CREATE TABLE `test_setting_param` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `Check1` int(10) unsigned NOT NULL DEFAULT '0',
  `param1` int(10) unsigned NOT NULL DEFAULT '0',
  `param2` int(10) unsigned NOT NULL DEFAULT '0',
  `Check2` int(10) unsigned NOT NULL DEFAULT '0',
  `Check3` int(10) unsigned NOT NULL DEFAULT '0',
  `param3` int(10) unsigned NOT NULL DEFAULT '0',
  `param4` int(10) unsigned NOT NULL DEFAULT '0',
  `Check4` int(10) unsigned NOT NULL DEFAULT '0',
  `param5` int(10) unsigned NOT NULL DEFAULT '0',
  `Check5` int(10) unsigned NOT NULL DEFAULT '0',
  `param6` int(10) unsigned NOT NULL DEFAULT '0',
  `param7` int(10) unsigned NOT NULL DEFAULT '0',
  `Check6` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `test_setting_param` */

insert  into `test_setting_param`(`id`,`Check1`,`param1`,`param2`,`Check2`,`Check3`,`param3`,`param4`,`Check4`,`param5`,`Check5`,`param6`,`param7`,`Check6`) values (0,1,0,0,0,0,0,0,0,0,0,0,0,0),(1,0,0,0,0,0,0,0,0,0,0,0,0,1),(2,0,0,0,0,1,0,0,0,0,0,0,0,0);

/*Table structure for table `user_authorities` */

DROP TABLE IF EXISTS `user_authorities`;

CREATE TABLE `user_authorities` (
  `id` int(8) unsigned NOT NULL DEFAULT '0',
  `authority` varchar(24) NOT NULL DEFAULT '/',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `user_authorities` */

insert  into `user_authorities`(`id`,`authority`) values (0,'SuperAdministrator'),(1,'SystemAdministrator'),(2,'TechnicalStaff'),(3,'CommonStaff');

/*Table structure for table `user_info` */

DROP TABLE IF EXISTS `user_info`;

CREATE TABLE `user_info` (
  `id` int(32) unsigned NOT NULL DEFAULT '0',
  `user_id` varchar(24) NOT NULL DEFAULT '/',
  `user_passward` varchar(24) NOT NULL DEFAULT '/',
  `user_true_name` varchar(24) NOT NULL DEFAULT '/',
  `user_production_line` varchar(24) NOT NULL DEFAULT '/',
  `user_authority` varchar(24) NOT NULL DEFAULT '/',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `user_info` */

insert  into `user_info`(`id`,`user_id`,`user_passward`,`user_true_name`,`user_production_line`,`user_authority`) values (0,'u','000000','SuperAdamin','Developer','SuperAdministrator'),(1,'u21721331','000000','Name01','bolt Inspection 1','CommonStaff'),(2,'u21721337','000000','Name02','bolt Inspection 1','CommonStaff'),(3,'u21721333','000000','Name03','bolt Inspection 1','CommonStaff'),(4,'u21721334','000000','Name51','bolt Inspection 1','TechnicalStaff'),(5,'u21721335','000000','Name52','bolt Inspection 2','TechnicalStaff'),(6,'u21721336','000000','Name99','bolt Inspection ','SystemAdministrator'),(7,'u21721332','123456','name00','bolt Inspection','SystemAdministrator');

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

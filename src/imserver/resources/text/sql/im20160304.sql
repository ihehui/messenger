CREATE DATABASE  IF NOT EXISTS `im` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `im`;
-- MySQL dump 10.13  Distrib 5.7.9, for Win32 (AMD64)
--
-- Host: 200.200.200.40    Database: im
-- ------------------------------------------------------
-- Server version	5.5.33-MariaDB-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `CachedChatMessages`
--

DROP TABLE IF EXISTS `CachedChatMessages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `CachedChatMessages` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `Sender` varchar(16) NOT NULL COMMENT '发送者ID',
  `Reciever` varchar(16) NOT NULL COMMENT '接收者ID',
  `Message` varchar(255) NOT NULL COMMENT '消息',
  `TransmittingTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '发送时间',
  `ClientRead` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Font_Size` tinyint(2) unsigned DEFAULT NULL,
  `Font_Bold` tinyint(1) unsigned DEFAULT NULL,
  `Font_Italic` tinyint(1) unsigned DEFAULT NULL,
  `Font_Underline` tinyint(1) unsigned DEFAULT NULL,
  `Font_Color` char(7) DEFAULT NULL,
  `Font_Family` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `FK-CCM_Sender-USI_UserID` (`Sender`),
  KEY `FK-CCM_Receiver-USI_UserID` (`Reciever`),
  CONSTRAINT `FK-CCM_Receiver-USI_UserID` FOREIGN KEY (`Reciever`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK-CCM_Sender-USI_UserID` FOREIGN KEY (`Sender`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=58 DEFAULT CHARSET=utf8 COMMENT='缓存的消息';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `CachedChatMessages`
--

LOCK TABLES `CachedChatMessages` WRITE;
/*!40000 ALTER TABLE `CachedChatMessages` DISABLE KEYS */;
INSERT INTO `CachedChatMessages` VALUES (1,'test1','hehui',';28;b;;;|gggg','2014-03-17 17:06:07',0,NULL,NULL,NULL,NULL,NULL,NULL),(2,'hehui','test1','|3','2014-03-17 17:07:29',0,NULL,NULL,NULL,NULL,NULL,NULL),(3,'test1','hehui','|www','2014-03-17 17:07:48',0,NULL,NULL,NULL,NULL,NULL,NULL),(4,'hehui','test1','|ww','2014-03-17 17:30:34',0,NULL,NULL,NULL,NULL,NULL,NULL),(5,'hehui','test1','|333','2014-03-17 17:33:06',0,NULL,NULL,NULL,NULL,NULL,NULL),(6,'hehui','test1','|ddd','2014-03-17 17:35:32',0,NULL,NULL,NULL,NULL,NULL,NULL),(7,'hehui','test1','|ffffffffff','2014-03-17 17:36:08',0,NULL,NULL,NULL,NULL,NULL,NULL),(8,'hehui','test1','|333333333333','2014-03-17 17:36:16',0,NULL,NULL,NULL,NULL,NULL,NULL),(9,'hehui','test1','|222','2014-03-17 17:40:56',0,NULL,NULL,NULL,NULL,NULL,NULL),(10,'hehui','test1','|eeeeeeeee','2014-03-17 17:42:48',0,NULL,NULL,NULL,NULL,NULL,NULL),(11,'hehui','test1','|4444444444','2014-03-17 17:44:10',0,NULL,NULL,NULL,NULL,NULL,NULL),(12,'hehui','test1','|5555555555','2014-03-17 17:44:24',0,NULL,NULL,NULL,NULL,NULL,NULL),(13,'hehui','test1','|777777777','2014-03-17 17:44:28',0,NULL,NULL,NULL,NULL,NULL,NULL),(14,'hehui','test1','|66666666666','2014-03-17 17:44:53',0,NULL,NULL,NULL,NULL,NULL,NULL),(15,'hehui','test1','|66666666666666666','2014-03-17 17:45:04',0,NULL,NULL,NULL,NULL,NULL,NULL),(16,'hehui','test1','|555','2014-03-17 17:48:47',0,NULL,NULL,NULL,NULL,NULL,NULL),(17,'hehui','test1','|4444','2014-03-17 17:48:54',0,NULL,NULL,NULL,NULL,NULL,NULL),(18,'hehui','test1','|ggggg','2014-03-17 18:05:36',0,NULL,NULL,NULL,NULL,NULL,NULL),(19,'hehui','test1','|yyyyyyy','2014-03-17 18:05:42',0,NULL,NULL,NULL,NULL,NULL,NULL),(20,'hehui','test1','|5434542334','2014-03-17 18:05:45',0,NULL,NULL,NULL,NULL,NULL,NULL),(21,'hehui','test1','|hjgfhjhgjffj','2014-03-17 18:05:49',0,NULL,NULL,NULL,NULL,NULL,NULL),(22,'hehui','test1','|888888888888','2014-03-17 18:05:54',0,NULL,NULL,NULL,NULL,NULL,NULL),(23,'test1','hehui','|dddd','2014-03-17 18:06:00',0,NULL,NULL,NULL,NULL,NULL,NULL),(24,'admin','hehui','|1','2014-03-18 15:30:58',0,NULL,NULL,NULL,NULL,NULL,NULL),(25,'hehui','admin','|2','2014-03-18 15:31:31',0,NULL,NULL,NULL,NULL,NULL,NULL),(26,'admin','hehui','|3','2014-03-18 15:31:42',0,NULL,NULL,NULL,NULL,NULL,NULL),(27,'hehui','admin','|<img src=\"qrc:/emoticons/0.gif\"/> ','2014-03-18 15:31:54',0,NULL,NULL,NULL,NULL,NULL,NULL),(28,'hehui','admin','|A<img src=\"66f8e8fcaa4f9878dfc27737c67c6114.png\"/>','2014-03-18 15:32:16',0,NULL,NULL,NULL,NULL,NULL,NULL),(29,'admin','hehui','|<img src=\"0a31d054d07fdbdef284f70cfb76e158.png\"/>','2014-03-18 15:40:48',0,NULL,NULL,NULL,NULL,NULL,NULL),(30,'admin','hehui','|<img src=\"af6de018ea47dff4b52d57540ff3f01a.png\"/>','2014-03-18 15:41:47',0,NULL,NULL,NULL,NULL,NULL,NULL),(31,'admin','hehui','|<img src=\"e5d4ab6d5850ccd64a3b5de06ddec1f0.png\"/>','2014-03-18 17:09:35',0,NULL,NULL,NULL,NULL,NULL,NULL),(32,'admin','hehui','|<img src=\"14435cca6fdcedde59baa63737ebb771.png\"/>','2014-03-18 17:13:58',0,NULL,NULL,NULL,NULL,NULL,NULL),(33,'admin','hehui','|<img src=\"7538ca04b5b2d38a67bc636b69a06756.png\"/>','2014-03-18 17:31:56',0,NULL,NULL,NULL,NULL,NULL,NULL),(34,'admin','hehui','|<img src=\"10062176dae5c554c717b6c3374dc1e4.png\"/>','2014-03-18 17:36:32',0,NULL,NULL,NULL,NULL,NULL,NULL),(35,'admin','hehui','|<img src=\"bdfbb668557d7a3f7b2b8fb582f52657.png\"/>','2014-03-18 17:37:20',0,NULL,NULL,NULL,NULL,NULL,NULL),(36,'admin','hehui','|<img src=\"eaf19fce8e870fc2659383dcfb642e24.jpg\"/>','2014-03-18 17:38:23',0,NULL,NULL,NULL,NULL,NULL,NULL),(37,'hehui','admin','|<img src=\"431fa655fed46196152c4ca706aaf69e.jpg\"/>','2014-03-18 17:40:22',0,NULL,NULL,NULL,NULL,NULL,NULL),(38,'admin','hehui','|<img src=\"d8102fcac6f0037bd0cdd95a7549fb83.jpg\"/>','2014-03-18 17:54:14',0,NULL,NULL,NULL,NULL,NULL,NULL),(39,'admin','hehui','|111','2014-03-18 18:03:01',0,NULL,NULL,NULL,NULL,NULL,NULL),(40,'admin','hehui','|<img src=\"qrc:/emoticons/0.gif\"/> ','2014-03-18 18:03:06',0,NULL,NULL,NULL,NULL,NULL,NULL),(41,'hehui','admin','|<img src=\"7d43f4bea643749946cd7f11fce46f14.jpg\"/>','2014-03-19 11:26:59',0,NULL,NULL,NULL,NULL,NULL,NULL),(42,'hehui','test1','|<img src=\"96a234e886857a904fb5e0c5dcd85d22.jpg\"/>','2014-03-19 11:27:19',0,NULL,NULL,NULL,NULL,NULL,NULL),(43,'hehui','test1','|adfsa','2014-03-19 11:27:29',0,NULL,NULL,NULL,NULL,NULL,NULL),(44,'hehui','test1','|tttt','2014-03-19 11:27:32',0,NULL,NULL,NULL,NULL,NULL,NULL),(45,'hehui','test1','|ttttt','2014-03-19 11:27:35',0,NULL,NULL,NULL,NULL,NULL,NULL),(46,'hehui','test1','|tttttt','2014-03-19 11:27:37',0,NULL,NULL,NULL,NULL,NULL,NULL),(47,'hehui','test1','|tttttttttt','2014-03-19 11:27:41',0,NULL,NULL,NULL,NULL,NULL,NULL),(48,'hehui','test1','|55gsfdgs','2014-03-19 11:27:48',0,NULL,NULL,NULL,NULL,NULL,NULL),(49,'hehui','test1','|ggggggggggggggggggg','2014-03-19 11:27:54',0,NULL,NULL,NULL,NULL,NULL,NULL),(50,'hehui','test1','|<img src=\"qrc:/emoticons/1.gif\"/> ','2014-03-19 11:27:58',0,NULL,NULL,NULL,NULL,NULL,NULL),(51,'hehui','test1','|<img src=\"qrc:/emoticons/34.gif\"/> ','2014-03-19 11:28:02',0,NULL,NULL,NULL,NULL,NULL,NULL),(52,'hehui','test1','|<img src=\"qrc:/emoticons/0.gif\"/> ','2014-03-19 11:28:09',0,NULL,NULL,NULL,NULL,NULL,NULL),(53,'hehui','test1','|<img src=\"qrc:/emoticons/31.gif\"/> ','2014-03-19 11:28:16',0,NULL,NULL,NULL,NULL,NULL,NULL),(54,'hehui','test1','|sss','2014-03-19 12:08:48',0,NULL,NULL,NULL,NULL,NULL,NULL),(55,'hehui','test1','|3','2014-05-26 11:59:04',0,NULL,NULL,NULL,NULL,NULL,NULL),(56,'admin','hehui','|<img src=\"qrc:/emoticons/100.gif\"/> ','2014-07-23 02:10:24',0,NULL,NULL,NULL,NULL,NULL,NULL),(57,'hehui','admin','|<img src=\"qrc:/emoticons/31.gif\"/> ','2014-07-23 02:36:17',0,NULL,NULL,NULL,NULL,NULL,NULL);
/*!40000 ALTER TABLE `CachedChatMessages` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `CachedInterestGroupChatMessages`
--

DROP TABLE IF EXISTS `CachedInterestGroupChatMessages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `CachedInterestGroupChatMessages` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `Sender` varchar(16) NOT NULL COMMENT '发送者ID',
  `GroupID` int(10) unsigned NOT NULL COMMENT '接收组ID',
  `Message` varchar(255) NOT NULL COMMENT '消息',
  `TransmittingTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '发送时间',
  `Font_Size` tinyint(2) unsigned DEFAULT NULL,
  `Font_Bold` tinyint(1) unsigned DEFAULT NULL,
  `Font_Italic` tinyint(1) unsigned DEFAULT NULL,
  `Font_Underline` tinyint(1) unsigned DEFAULT NULL,
  `Font_Color` char(7) DEFAULT NULL,
  `Font_Family` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `FK-CIGCM_Sender-USI_UserID` (`Sender`),
  KEY `FK-CIGCM_GroupID-IG_GroupID` (`GroupID`),
  CONSTRAINT `FK-CIGCM_GroupID-IG_GroupID` FOREIGN KEY (`GroupID`) REFERENCES `InterestGroups` (`GroupID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK-CIGCM_Sender-USI_UserID` FOREIGN KEY (`Sender`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=34 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='缓存的群组消息';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `CachedInterestGroupChatMessages`
--

LOCK TABLES `CachedInterestGroupChatMessages` WRITE;
/*!40000 ALTER TABLE `CachedInterestGroupChatMessages` DISABLE KEYS */;
INSERT INTO `CachedInterestGroupChatMessages` VALUES (1,'hehui',12,'|222','2014-03-18 11:49:43',NULL,NULL,NULL,NULL,NULL,NULL),(2,'hehui',12,'|222','2014-03-18 11:57:19',NULL,NULL,NULL,NULL,NULL,NULL),(3,'hehui',12,'|1111','2014-03-18 12:04:32',NULL,NULL,NULL,NULL,NULL,NULL),(4,'hehui',12,'|222','2014-03-18 12:06:59',NULL,NULL,NULL,NULL,NULL,NULL),(5,'hehui',12,'|1111','2014-03-18 12:11:35',NULL,NULL,NULL,NULL,NULL,NULL),(6,'hehui',12,'|qqq','2014-03-18 12:42:39',NULL,NULL,NULL,NULL,NULL,NULL),(7,'hehui',12,'|rrr','2014-03-18 12:42:49',NULL,NULL,NULL,NULL,NULL,NULL),(8,'hehui',12,'|12345','2014-03-18 12:42:54',NULL,NULL,NULL,NULL,NULL,NULL),(9,'hehui',12,'|sdsad','2014-03-18 12:43:01',NULL,NULL,NULL,NULL,NULL,NULL),(10,'hehui',12,'|adfasfdasad','2014-03-18 12:43:09',NULL,NULL,NULL,NULL,NULL,NULL),(11,'hehui',12,'|1','2014-03-18 12:43:30',NULL,NULL,NULL,NULL,NULL,NULL),(12,'hehui',12,'|2','2014-03-18 12:43:32',NULL,NULL,NULL,NULL,NULL,NULL),(13,'hehui',12,'|3','2014-03-18 12:43:33',NULL,NULL,NULL,NULL,NULL,NULL),(14,'hehui',12,'|4','2014-03-18 12:43:34',NULL,NULL,NULL,NULL,NULL,NULL),(15,'hehui',12,'|1111','2014-03-18 14:57:14',NULL,NULL,NULL,NULL,NULL,NULL),(16,'hehui',12,'|wwww','2014-03-18 14:57:26',NULL,NULL,NULL,NULL,NULL,NULL),(17,'test1',12,'|777777','2014-03-18 14:57:30',NULL,NULL,NULL,NULL,NULL,NULL),(18,'hehui',12,'|test','2014-03-18 15:07:24',NULL,NULL,NULL,NULL,NULL,NULL),(19,'hehui',12,'|444','2014-03-19 11:28:37',NULL,NULL,NULL,NULL,NULL,NULL),(20,'hehui',12,'|111','2014-03-19 11:29:09',NULL,NULL,NULL,NULL,NULL,NULL),(21,'hehui',12,'|qqq','2014-03-19 11:32:47',NULL,NULL,NULL,NULL,NULL,NULL),(22,'hehui',12,'|222','2014-03-19 11:33:09',NULL,NULL,NULL,NULL,NULL,NULL),(23,'hehui',12,'|3333','2014-03-19 11:33:37',NULL,NULL,NULL,NULL,NULL,NULL),(24,'hehui',12,'|4444','2014-03-19 11:35:30',NULL,NULL,NULL,NULL,NULL,NULL),(25,'hehui',12,'|5555','2014-03-19 11:36:21',NULL,NULL,NULL,NULL,NULL,NULL),(26,'hehui',12,'|444','2014-03-19 12:08:09',NULL,NULL,NULL,NULL,NULL,NULL),(27,'hehui',12,'|ggg','2014-03-19 12:09:19',NULL,NULL,NULL,NULL,NULL,NULL),(28,'hehui',12,'|111','2014-03-19 12:22:28',NULL,NULL,NULL,NULL,NULL,NULL),(29,'hehui',12,'|www','2014-03-19 12:22:38',NULL,NULL,NULL,NULL,NULL,NULL),(30,'hehui',12,'|1111','2014-03-19 12:22:40',NULL,NULL,NULL,NULL,NULL,NULL),(31,'hehui',12,'|rrrr','2014-03-19 12:22:44',NULL,NULL,NULL,NULL,NULL,NULL),(32,'test1',12,'|dsdds','2014-03-19 12:23:00',NULL,NULL,NULL,NULL,NULL,NULL),(33,'test1',12,'|dsfsdf','2014-03-19 12:23:05',NULL,NULL,NULL,NULL,NULL,NULL);
/*!40000 ALTER TABLE `CachedInterestGroupChatMessages` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `FriendshipApply`
--

DROP TABLE IF EXISTS `FriendshipApply`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `FriendshipApply` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `Sender` varchar(16) NOT NULL,
  `Receiver` varchar(16) NOT NULL,
  `ExtraMessage` varchar(32) DEFAULT NULL,
  `Result` tinyint(2) NOT NULL DEFAULT '0' COMMENT '0:未知,1:接受,2:拒绝',
  `SenderRead` tinyint(2) NOT NULL DEFAULT '0' COMMENT '0:未读,1：已读',
  `ReceiverRead` tinyint(2) NOT NULL DEFAULT '0' COMMENT '0:未读,1：已读',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `index-FA_Sender_Receiver` (`Sender`,`Receiver`),
  KEY `FK-FA_Sender-USI_UserID` (`Sender`),
  KEY `FK-FA_Receiver-USI_UserID` (`Receiver`),
  CONSTRAINT `FK-FA_Receiver-USI_UserID` FOREIGN KEY (`Receiver`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK-FA_Sender-USI_UserID` FOREIGN KEY (`Sender`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='好友请求';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `FriendshipApply`
--

LOCK TABLES `FriendshipApply` WRITE;
/*!40000 ALTER TABLE `FriendshipApply` DISABLE KEYS */;
/*!40000 ALTER TABLE `FriendshipApply` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `InterestGroupLogs`
--

DROP TABLE IF EXISTS `InterestGroupLogs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `InterestGroupLogs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `GroupID` int(10) unsigned NOT NULL,
  `Sender` varchar(16) NOT NULL DEFAULT '1',
  `Reciver` varchar(16) DEFAULT NULL,
  `LogType` tinyint(3) unsigned NOT NULL COMMENT 'LOG_GroupCreated=1,\nLog_GroupDisbanded=2,\nLOG_ContactInvitedToJoin=3,\nLog_ContactRequestToJoin=4,\nLOG_ContactJoined=5,\nLOG_ContactQuit=6,\nLOG_ContactFired=7,\nLOG_ContactBanned=8,\nLOG_ContactPromoted=9,\nLOG_ContactDemoted=10,\nLOG_ContactRenamed=11',
  `Time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `FK-IGL_GroupID_idx` (`GroupID`),
  KEY `FK-IGL_Reciver_idx` (`Reciver`),
  KEY `FK-IGL_Sender_idx` (`Sender`),
  CONSTRAINT `FK-IGL_GroupID` FOREIGN KEY (`GroupID`) REFERENCES `InterestGroups` (`GroupID`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `FK-IGL_Reciver` FOREIGN KEY (`Reciver`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE,
  CONSTRAINT `FK-IGL_Sender` FOREIGN KEY (`Sender`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `InterestGroupLogs`
--

LOCK TABLES `InterestGroupLogs` WRITE;
/*!40000 ALTER TABLE `InterestGroupLogs` DISABLE KEYS */;
/*!40000 ALTER TABLE `InterestGroupLogs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `InterestGroupMemberRoles-`
--

DROP TABLE IF EXISTS `InterestGroupMemberRoles-`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `InterestGroupMemberRoles-` (
  `RoleID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `RoleName` varchar(16) NOT NULL,
  `Description` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`RoleID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 COMMENT='组员在组中的角色';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `InterestGroupMemberRoles-`
--

LOCK TABLES `InterestGroupMemberRoles-` WRITE;
/*!40000 ALTER TABLE `InterestGroupMemberRoles-` DISABLE KEYS */;
INSERT INTO `InterestGroupMemberRoles-` VALUES (1,'Creator',NULL),(2,'Administrator',NULL),(3,'Member',NULL);
/*!40000 ALTER TABLE `InterestGroupMemberRoles-` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `InterestGroupMembers`
--

DROP TABLE IF EXISTS `InterestGroupMembers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `InterestGroupMembers` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` varchar(16) NOT NULL COMMENT '成员ID',
  `GroupID` int(10) unsigned NOT NULL COMMENT '兴趣组ID',
  `RoleID` int(10) unsigned NOT NULL DEFAULT '3' COMMENT '1:Creator; 2:Administrator; 3:Member',
  `UserRemarkName` varchar(16) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `Index_UserID_GroupID` (`UserID`,`GroupID`),
  KEY `FK-IGM_GroupID-IG_GroupID` (`GroupID`),
  CONSTRAINT `FK-IGM_GroupID-IG_GroupID` FOREIGN KEY (`GroupID`) REFERENCES `InterestGroups` (`GroupID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK-IGM_UserID-USI_UserID` FOREIGN KEY (`UserID`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8 COMMENT='组的成员';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `InterestGroupMembers`
--

LOCK TABLES `InterestGroupMembers` WRITE;
/*!40000 ALTER TABLE `InterestGroupMembers` DISABLE KEYS */;
INSERT INTO `InterestGroupMembers` VALUES (6,'admin',7,1,NULL),(7,'admin',8,1,NULL),(8,'admin',9,1,NULL),(9,'admin',10,1,NULL),(10,'admin',11,1,NULL),(11,'hehui',12,1,NULL),(12,'test1',7,3,NULL),(13,'test1',12,3,NULL);
/*!40000 ALTER TABLE `InterestGroupMembers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `InterestGroupMembershipApplication`
--

DROP TABLE IF EXISTS `InterestGroupMembershipApplication`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `InterestGroupMembershipApplication` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` varchar(16) NOT NULL,
  `GroupID` int(10) unsigned NOT NULL,
  `Message` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `idx-IGMA-UserID-GroupID` (`UserID`,`GroupID`),
  KEY `FK-IGMA_AUID-USI_UserID` (`UserID`),
  KEY `FK-IGMA_GI_IG_GroupID` (`GroupID`),
  CONSTRAINT `FK-IGMA_AUID-USI_UserID` FOREIGN KEY (`UserID`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK-IGMA_GI_IG_ID` FOREIGN KEY (`GroupID`) REFERENCES `InterestGroups` (`GroupID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='加入组请求';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `InterestGroupMembershipApplication`
--

LOCK TABLES `InterestGroupMembershipApplication` WRITE;
/*!40000 ALTER TABLE `InterestGroupMembershipApplication` DISABLE KEYS */;
/*!40000 ALTER TABLE `InterestGroupMembershipApplication` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `InterestGroupMembershipInvitation`
--

DROP TABLE IF EXISTS `InterestGroupMembershipInvitation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `InterestGroupMembershipInvitation` (
  `ID` int(10) unsigned NOT NULL,
  `UserID` varchar(16) NOT NULL,
  `GroupID` int(10) unsigned NOT NULL,
  `Message` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `idx-IGMI-UserID-GroupID` (`UserID`,`GroupID`),
  KEY `FK-IGMI_UserID_idx` (`UserID`),
  KEY `FK-IGMI_GroupID_idx` (`GroupID`),
  CONSTRAINT `FK-IGMI_GroupID` FOREIGN KEY (`GroupID`) REFERENCES `InterestGroups` (`GroupID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK-IGMI_UserID` FOREIGN KEY (`UserID`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='邀请成员加入组';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `InterestGroupMembershipInvitation`
--

LOCK TABLES `InterestGroupMembershipInvitation` WRITE;
/*!40000 ALTER TABLE `InterestGroupMembershipInvitation` DISABLE KEYS */;
/*!40000 ALTER TABLE `InterestGroupMembershipInvitation` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `InterestGroupTypes-`
--

DROP TABLE IF EXISTS `InterestGroupTypes-`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `InterestGroupTypes-` (
  `TypeID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `ParentType` int(10) unsigned DEFAULT NULL,
  `TypeName` varchar(45) NOT NULL,
  `Description` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`TypeID`),
  UNIQUE KEY `TypeName_UNIQUE` (`TypeName`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 COMMENT='组类型';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `InterestGroupTypes-`
--

LOCK TABLES `InterestGroupTypes-` WRITE;
/*!40000 ALTER TABLE `InterestGroupTypes-` DISABLE KEYS */;
INSERT INTO `InterestGroupTypes-` VALUES (1,NULL,'System',NULL),(2,NULL,'User',NULL);
/*!40000 ALTER TABLE `InterestGroupTypes-` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `InterestGroups`
--

DROP TABLE IF EXISTS `InterestGroups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `InterestGroups` (
  `GroupID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `TypeID` int(10) unsigned NOT NULL DEFAULT '2' COMMENT '1:System Group; 2:User Created',
  `ParentGroup` int(10) unsigned DEFAULT NULL COMMENT '父组的ID',
  `Creator` varchar(16) NOT NULL COMMENT '创建者',
  `GroupName` varchar(32) NOT NULL COMMENT '组名称',
  `CreationTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
  `GroupInfoVersion` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '组信息版本',
  `MemberListVersion` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '组成员列表版本',
  `Description` varchar(64) DEFAULT NULL,
  `Announcement` varchar(64) DEFAULT NULL COMMENT '公告',
  `Remark` varchar(64) DEFAULT NULL,
  `State` tinyint(1) unsigned NOT NULL DEFAULT '1' COMMENT '0:disband\n1:active\n',
  `DisbandedTime` datetime DEFAULT NULL,
  `Privacy` tinyint(1) unsigned NOT NULL DEFAULT '1' COMMENT 'Allow_Anyone_To_Join=1,\nRequest_Verfication_To_Join=2,\nOnly_Invited_Can_Join=3',
  PRIMARY KEY (`GroupID`),
  UNIQUE KEY `idx-Creator-GroupName` (`Creator`,`GroupName`),
  KEY `FK-IG_Creator-USI_UserID` (`Creator`),
  CONSTRAINT `FK-IG_Creator-USI_UserID` FOREIGN KEY (`Creator`) REFERENCES `UsersSummaryInfo` (`UserID`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8 COMMENT='兴趣组';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `InterestGroups`
--

LOCK TABLES `InterestGroups` WRITE;
/*!40000 ALTER TABLE `InterestGroups` DISABLE KEYS */;
INSERT INTO `InterestGroups` VALUES (7,2,NULL,'admin','TestGroup1','2013-09-19 04:46:43',1,13,NULL,NULL,NULL,1,NULL,1),(8,2,NULL,'admin','Group1','2013-09-20 06:27:49',1,4,NULL,NULL,NULL,1,NULL,1),(9,2,NULL,'admin','Group2','2013-09-21 10:31:14',1,4,NULL,NULL,NULL,1,NULL,1),(10,2,NULL,'admin','Group3','2013-09-21 10:31:38',1,1,NULL,NULL,NULL,1,NULL,1),(11,2,NULL,'admin','Group4','2013-09-21 10:50:37',1,1,NULL,NULL,NULL,1,NULL,1),(12,2,NULL,'hehui','QQQ','2013-11-22 06:36:51',1,2,NULL,NULL,NULL,1,NULL,1);
/*!40000 ALTER TABLE `InterestGroups` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `LoginHistories`
--

DROP TABLE IF EXISTS `LoginHistories`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `LoginHistories` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` varchar(16) NOT NULL,
  `ExtIPAddress` varchar(64) NOT NULL,
  `ExtPort` smallint(6) unsigned NOT NULL,
  `IntIPAddress` varchar(64) DEFAULT NULL,
  `IntPort` smallint(6) unsigned DEFAULT NULL,
  `LoginTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `LogoutTime` datetime DEFAULT NULL,
  `DeviceInfo` varchar(64) DEFAULT NULL,
  `HostInfo` varchar(64) DEFAULT NULL,
  `ClientVersion` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `FK-LH_UserID-USI_UserID` (`UserID`),
  CONSTRAINT `FK-LH_UserID-USI_UserID` FOREIGN KEY (`UserID`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=116 DEFAULT CHARSET=utf8 COMMENT='登陆历史';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `LoginHistories`
--

LOCK TABLES `LoginHistories` WRITE;
/*!40000 ALTER TABLE `LoginHistories` DISABLE KEYS */;
INSERT INTO `LoginHistories` VALUES (1,'hehui','200.200.200.17',50395,NULL,NULL,'2014-03-17 16:15:15','2014-03-18 00:16:02','PC',NULL,NULL),(2,'test1','200.200.200.17',47397,NULL,NULL,'2014-03-17 16:16:11','2014-03-18 00:16:26','PC',NULL,NULL),(3,'test2','200.200.200.17',49318,NULL,NULL,'2014-03-17 16:16:33','2014-03-18 00:23:04','PC',NULL,NULL),(4,'test1','200.200.200.17',44487,NULL,NULL,'2014-03-17 16:24:58','2014-03-18 00:25:25','PC',NULL,NULL),(5,'test2','200.200.200.17',54964,NULL,NULL,'2014-03-17 16:25:33','2014-03-18 00:25:46','PC',NULL,NULL),(6,'test2','200.200.200.17',53685,NULL,NULL,'2014-03-17 16:46:07','2014-03-18 00:48:15','PC',NULL,NULL),(7,'hehui','200.200.200.17',40041,NULL,NULL,'2014-03-17 16:46:16','2014-03-18 00:50:10','PC',NULL,NULL),(8,'hehui','200.200.200.17',55742,NULL,NULL,'2014-03-17 16:56:25','2014-03-18 02:06:09','PC',NULL,NULL),(9,'test1','200.200.200.17',35544,NULL,NULL,'2014-03-17 16:56:36','2014-03-18 00:58:34','PC',NULL,NULL),(10,'test1','200.200.200.17',50044,NULL,NULL,'2014-03-17 16:58:41','2014-03-18 00:58:49','PC',NULL,NULL),(11,'test1','200.200.200.17',46575,NULL,NULL,'2014-03-17 16:58:57','2014-03-18 01:00:34','PC',NULL,NULL),(12,'test1','200.200.200.17',48541,NULL,NULL,'2014-03-17 17:00:41','2014-03-18 01:04:31','PC',NULL,NULL),(13,'test1','200.200.200.17',41889,NULL,NULL,'2014-03-17 17:05:47','2014-03-18 01:29:17','PC',NULL,NULL),(14,'test1','200.200.200.17',46194,NULL,NULL,'2014-03-17 17:30:21','2014-03-18 01:33:17','PC',NULL,NULL),(15,'test1','200.200.200.17',42554,NULL,NULL,'2014-03-17 17:35:27','2014-03-18 01:36:24','PC',NULL,NULL),(16,'test1','200.200.200.17',38373,NULL,NULL,'2014-03-17 17:40:45','2014-03-18 01:41:52','PC',NULL,NULL),(17,'test1','200.200.200.17',38553,NULL,NULL,'2014-03-17 17:42:42','2014-03-18 01:43:02','PC',NULL,NULL),(18,'test1','200.200.200.17',55517,NULL,NULL,'2014-03-17 17:44:03','2014-03-18 01:45:11','PC',NULL,NULL),(19,'test1','200.200.200.17',58597,NULL,NULL,'2014-03-17 17:48:43','2014-03-18 01:51:48','PC',NULL,NULL),(20,'test1','200.200.200.17',35367,NULL,NULL,'2014-03-17 18:05:30','2014-03-18 02:06:04','PC',NULL,NULL),(21,'test1','200.200.200.17',54547,NULL,NULL,'2014-03-17 18:14:02','2014-03-18 02:16:06','PC',NULL,NULL),(22,'hehui','200.200.200.17',47588,NULL,NULL,'2014-03-17 18:14:29','2014-03-18 02:16:07','PC',NULL,NULL),(23,'hehui','200.200.200.17',56519,NULL,NULL,'2014-03-17 18:24:45','2014-03-18 02:27:27','PC',NULL,NULL),(24,'test1','200.200.200.17',36194,NULL,NULL,'2014-03-17 18:25:00','2014-03-18 02:27:05','PC',NULL,NULL),(25,'hehui','200.200.200.17',60269,NULL,NULL,'2014-03-17 18:27:27','2014-03-18 02:27:36','PC',NULL,NULL),(26,'hehui','200.200.200.17',49107,NULL,NULL,'2014-03-17 18:27:52','2014-03-18 19:39:47','PC',NULL,NULL),(27,'test1','200.200.200.17',46684,NULL,NULL,'2014-03-17 18:28:09','2014-03-18 19:39:59','PC',NULL,NULL),(28,'hehui','200.200.200.17',48045,NULL,NULL,'2014-03-18 11:40:12','2014-03-18 19:43:58','PC',NULL,NULL),(29,'test1','200.200.200.17',36353,NULL,NULL,'2014-03-18 11:40:30','2014-03-18 19:43:57','PC',NULL,NULL),(30,'test1','200.200.200.17',47143,NULL,NULL,'2014-03-18 11:45:20','2014-03-18 19:47:15','PC',NULL,NULL),(31,'hehui','200.200.200.17',36930,NULL,NULL,'2014-03-18 11:45:34','2014-03-18 19:47:16','PC',NULL,NULL),(32,'hehui','200.200.200.17',34750,NULL,NULL,'2014-03-18 11:49:18','2014-03-18 19:56:44','PC',NULL,NULL),(33,'test1','200.200.200.17',46454,NULL,NULL,'2014-03-18 11:50:06','2014-03-18 19:56:43','PC',NULL,NULL),(34,'hehui','200.200.200.17',40540,NULL,NULL,'2014-03-18 11:56:55','2014-03-18 19:58:19','PC',NULL,NULL),(35,'test1','200.200.200.17',46059,NULL,NULL,'2014-03-18 11:57:10','2014-03-18 19:58:18','PC',NULL,NULL),(36,'hehui','200.200.200.17',58871,NULL,NULL,'2014-03-18 12:04:14','2014-03-18 20:05:35','PC',NULL,NULL),(37,'test1','200.200.200.17',50984,NULL,NULL,'2014-03-18 12:04:27','2014-03-18 20:05:30','PC',NULL,NULL),(38,'hehui','200.200.200.17',48790,NULL,NULL,'2014-03-18 12:06:15','2014-03-18 20:07:56','PC',NULL,NULL),(39,'test1','200.200.200.17',55455,NULL,NULL,'2014-03-18 12:06:50','2014-03-18 20:07:46','PC',NULL,NULL),(40,'hehui','200.200.200.17',44882,NULL,NULL,'2014-03-18 12:11:22','2014-03-18 20:12:25','PC',NULL,NULL),(41,'test1','200.200.200.17',57499,NULL,NULL,'2014-03-18 12:11:29','2014-03-18 20:12:20','PC',NULL,NULL),(42,'hehui','200.200.200.17',55827,NULL,NULL,'2014-03-18 12:12:45','2014-03-18 20:14:24','PC',NULL,NULL),(43,'hehui','200.200.200.17',38842,NULL,NULL,'2014-03-18 12:18:03','2014-03-18 20:18:15','PC',NULL,NULL),(44,'hehui','200.200.200.17',52503,NULL,NULL,'2014-03-18 12:36:37','2014-03-18 20:36:47','PC',NULL,NULL),(45,'hehui','200.200.200.17',36174,NULL,NULL,'2014-03-18 12:36:51','2014-03-18 20:38:43','PC',NULL,NULL),(46,'hehui','200.200.200.17',47026,NULL,NULL,'2014-03-18 12:42:07','2014-03-18 22:29:39','PC',NULL,NULL),(47,'test1','200.200.200.17',37183,NULL,NULL,'2014-03-18 12:42:22','2014-03-18 20:43:44','PC',NULL,NULL),(48,'test1','200.200.200.17',60931,NULL,NULL,'2014-03-18 12:43:50','2014-03-18 20:43:59','PC',NULL,NULL),(49,'test1','200.200.200.17',46307,NULL,NULL,'2014-03-18 14:55:05','2014-03-18 22:55:10','PC',NULL,NULL),(50,'test1','200.200.200.17',56298,NULL,NULL,'2014-03-18 14:56:37','2014-03-18 22:57:43','PC',NULL,NULL),(51,'hehui','200.200.200.17',37178,NULL,NULL,'2014-03-18 14:57:05','2014-03-18 22:57:37','PC',NULL,NULL),(52,'hehui','200.200.200.17',42783,NULL,NULL,'2014-03-18 15:06:46','2014-03-18 23:07:29','PC',NULL,NULL),(53,'test1','200.200.200.17',59206,NULL,NULL,'2014-03-18 15:07:05','2014-03-18 23:07:30','PC',NULL,NULL),(54,'admin','192.168.56.1',61647,NULL,NULL,'2014-03-18 15:30:40','2014-03-18 23:33:36','PC',NULL,NULL),(55,'hehui','192.168.56.1',58481,NULL,NULL,'2014-03-18 15:31:22','2014-03-18 23:33:43','PC',NULL,NULL),(56,'hehui','192.168.56.1',54213,NULL,NULL,'2014-03-18 15:38:32','2014-03-18 23:41:03','PC',NULL,NULL),(57,'admin','192.168.56.1',63058,NULL,NULL,'2014-03-18 15:40:36','2014-03-18 23:41:27','PC',NULL,NULL),(58,'admin','192.168.56.1',56541,NULL,NULL,'2014-03-18 15:41:27','2014-03-18 23:41:56','PC',NULL,NULL),(59,'admin','192.168.56.1',61158,NULL,NULL,'2014-03-18 17:09:21','2014-03-19 01:09:41','PC',NULL,NULL),(60,'admin','192.168.56.1',49706,NULL,NULL,'2014-03-18 17:13:50','2014-03-19 01:14:58','PC',NULL,NULL),(61,'admin','192.168.56.1',50643,NULL,NULL,'2014-03-18 17:31:45','2014-03-19 01:32:03','PC',NULL,NULL),(62,'admin','192.168.56.1',61778,NULL,NULL,'2014-03-18 17:36:23','2014-03-19 01:36:42','PC',NULL,NULL),(63,'admin','192.168.56.1',61866,NULL,NULL,'2014-03-18 17:37:12','2014-03-19 01:37:32','PC',NULL,NULL),(64,'admin','192.168.56.1',54580,NULL,NULL,'2014-03-18 17:38:16','2014-03-19 01:38:26','PC',NULL,NULL),(65,'hehui','192.168.56.1',59242,NULL,NULL,'2014-03-18 17:39:39','2014-03-19 01:40:26','PC',NULL,NULL),(66,'admin','192.168.56.1',63169,NULL,NULL,'2014-03-18 17:43:41','2014-03-19 01:44:01','PC',NULL,NULL),(67,'admin','192.168.56.1',56876,NULL,NULL,'2014-03-18 17:54:07','2014-03-19 01:54:18','PC',NULL,NULL),(68,'admin','192.168.56.1',53494,NULL,NULL,'2014-03-18 17:57:50','2014-03-19 01:58:44','PC',NULL,NULL),(69,'admin','192.168.56.1',63941,NULL,NULL,'2014-03-18 18:02:44','2014-03-19 02:03:13','PC',NULL,NULL),(70,'hehui','200.200.200.17',53658,NULL,NULL,'2014-03-19 11:26:17','2014-03-19 19:29:22','PC',NULL,NULL),(71,'test1','200.200.200.17',47936,NULL,NULL,'2014-03-19 11:26:48','2014-03-19 19:29:23','PC',NULL,NULL),(72,'hehui','200.200.200.17',53211,NULL,NULL,'2014-03-19 11:32:30','2014-03-19 19:45:59','PC',NULL,NULL),(73,'test1','200.200.200.17',52149,NULL,NULL,'2014-03-19 11:32:38','2014-03-19 19:32:55','PC',NULL,NULL),(74,'test1','200.200.200.17',33749,NULL,NULL,'2014-03-19 11:33:04','2014-03-19 19:35:05','PC',NULL,NULL),(75,'test1','200.200.200.17',38378,NULL,NULL,'2014-03-19 11:35:25','2014-03-19 19:36:16','PC',NULL,NULL),(76,'test1','200.200.200.17',50587,NULL,NULL,'2014-03-19 11:36:16','2014-03-19 19:37:36','PC',NULL,NULL),(77,'hehui','200.200.200.17',54883,NULL,NULL,'2014-03-19 12:07:50','2014-03-19 20:09:27','PC',NULL,NULL),(78,'test1','200.200.200.17',51787,NULL,NULL,'2014-03-19 12:07:58','2014-03-19 20:09:25','PC',NULL,NULL),(79,'hehui','200.200.200.17',48331,NULL,NULL,'2014-03-19 12:22:17','2014-03-19 20:23:14','PC',NULL,NULL),(80,'test1','200.200.200.17',46433,NULL,NULL,'2014-03-19 12:22:24','2014-03-19 20:23:17','PC',NULL,NULL),(81,'hehui','200.200.200.17',60938,NULL,NULL,'2014-03-19 14:47:37','2014-03-19 22:48:02','PC',NULL,NULL),(82,'test1','200.200.200.17',60686,NULL,NULL,'2014-03-19 14:47:50','2014-03-19 22:47:57','PC',NULL,NULL),(83,'hehui','200.200.200.17',33848,NULL,NULL,'2014-03-19 14:48:47','2014-03-19 22:49:32','PC',NULL,NULL),(84,'test1','200.200.200.17',55439,NULL,NULL,'2014-03-19 14:48:56','2014-03-19 22:49:15','PC',NULL,NULL),(85,'hehui','200.200.200.17',49146,NULL,NULL,'2014-03-19 14:53:00','2014-03-19 22:53:05','PC',NULL,NULL),(86,'hehui','200.200.200.17',48011,NULL,NULL,'2014-03-19 14:53:19','2014-03-19 23:04:03','PC',NULL,NULL),(87,'test1','200.200.200.17',33172,NULL,NULL,'2014-03-19 14:54:07','2014-03-19 22:54:32','PC',NULL,NULL),(88,'test1','200.200.200.17',36821,NULL,NULL,'2014-03-19 14:58:59','2014-03-19 22:59:10','PC',NULL,NULL),(89,'hehui','200.200.200.17',47769,NULL,NULL,'2014-03-19 15:04:56','2014-03-19 23:05:25','PC',NULL,NULL),(90,'test1','200.200.200.17',51469,NULL,NULL,'2014-03-19 15:05:17','2014-03-19 23:05:23','PC',NULL,NULL),(91,'hehui','200.200.200.17',50677,NULL,NULL,'2014-04-21 17:19:33','2014-04-22 01:21:18','PC',NULL,NULL),(92,'test1','200.200.200.17',58587,NULL,NULL,'2014-04-21 17:19:55','2014-04-22 01:20:20','PC',NULL,NULL),(93,'test1','200.200.200.17',50217,NULL,NULL,'2014-04-21 17:29:45','2014-04-22 01:29:56','PC',NULL,NULL),(94,'test1','200.200.200.17',56808,NULL,NULL,'2014-04-21 17:35:40','2014-04-22 01:35:45','PC',NULL,NULL),(95,'test1','200.200.200.17',41874,NULL,NULL,'2014-04-21 17:52:27','2014-04-22 01:54:05','PC',NULL,NULL),(96,'hehui','200.200.200.17',39163,NULL,NULL,'2014-04-21 17:52:43','2014-04-22 01:53:57','PC',NULL,NULL),(97,'hehui','200.200.200.17',48997,NULL,NULL,'2014-05-26 11:43:07','2014-05-26 19:45:31','PC',NULL,NULL),(98,'hehui','200.200.200.17',38761,NULL,NULL,'2014-05-26 11:58:32','2014-05-26 20:01:24','PC',NULL,NULL),(99,'test1','200.200.200.17',38974,NULL,NULL,'2014-05-26 11:58:48','2014-05-26 20:01:30','PC',NULL,NULL),(100,'hehui','200.200.200.17',59058,NULL,NULL,'2014-05-26 12:13:07','2014-05-26 20:14:01','PC',NULL,NULL),(101,'test1','200.200.200.17',42599,NULL,NULL,'2014-05-26 12:13:18','2014-05-26 20:14:02','PC',NULL,NULL),(102,'hehui','200.200.200.17',56690,NULL,NULL,'2014-05-26 12:20:18','2014-05-26 20:22:03','PC',NULL,NULL),(103,'test1','200.200.200.17',59016,NULL,NULL,'2014-05-26 12:20:36','2014-05-26 20:26:22','PC',NULL,NULL),(104,'hehui','200.200.200.17',39311,NULL,NULL,'2014-05-26 12:28:31','2014-05-26 20:31:50','PC',NULL,NULL),(105,'test1','200.200.200.17',49471,NULL,NULL,'2014-05-26 12:28:45','2014-05-26 20:41:21','PC',NULL,NULL),(106,'hehui','200.200.200.17',43575,NULL,NULL,'2014-05-26 12:33:00','2014-05-26 20:41:19','PC',NULL,NULL),(107,'hehui','200.200.200.17',49587,NULL,NULL,'2014-05-26 12:42:03','2014-05-26 20:43:19','PC',NULL,NULL),(108,'hehui','200.200.200.17',55407,NULL,NULL,'2014-05-26 12:44:05','2014-05-26 20:44:05','PC',NULL,NULL),(109,'hehui','200.200.200.17',36094,NULL,NULL,'2014-05-26 12:55:21','2014-05-26 20:55:21','PC',NULL,NULL),(110,'hehui','200.200.200.17',39014,NULL,NULL,'2014-05-26 12:57:48','2014-05-26 22:23:58','PC',NULL,NULL),(111,'hehui','200.200.200.17',36433,NULL,NULL,'2014-05-26 15:13:28','2014-05-26 23:14:46','PC',NULL,NULL),(112,'hehui','200.200.200.17',51071,NULL,NULL,'2014-05-26 15:17:10','2014-05-26 23:18:09','PC',NULL,NULL),(113,'hehui','200.200.200.17',44007,NULL,NULL,'2014-05-26 15:22:03','2014-05-27 00:54:12','PC',NULL,NULL),(114,'admin','200.200.200.17',49363,NULL,NULL,'2014-07-23 02:07:47','2014-07-23 11:15:04','PC',NULL,NULL),(115,'hehui','200.200.200.17',60625,NULL,NULL,'2014-07-23 02:35:37','2014-07-23 11:15:03','PC',NULL,NULL);
/*!40000 ALTER TABLE `LoginHistories` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `PersonalContactGroups`
--

DROP TABLE IF EXISTS `PersonalContactGroups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `PersonalContactGroups` (
  `SysID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `GroupID` int(10) unsigned NOT NULL COMMENT '0:Blacklist, 1:Friend',
  `GroupName` varchar(32) NOT NULL,
  `Creator` varchar(16) NOT NULL,
  `GroupInfoVersion` int(10) unsigned NOT NULL DEFAULT '1',
  `MemberListVersion` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`SysID`),
  UNIQUE KEY `index-Creator-GroupName` (`Creator`,`GroupName`),
  UNIQUE KEY `index-Creator-GroupID` (`Creator`,`GroupID`),
  KEY `FK-PCG_Creator-USI_UserID` (`Creator`),
  CONSTRAINT `FK-PCG_Creator-USI_UserID` FOREIGN KEY (`Creator`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1012 DEFAULT CHARSET=utf8 COMMENT='个人联系人组';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `PersonalContactGroups`
--

LOCK TABLES `PersonalContactGroups` WRITE;
/*!40000 ALTER TABLE `PersonalContactGroups` DISABLE KEYS */;
INSERT INTO `PersonalContactGroups` VALUES (1004,1,'Friends','hehui',1,0),(1005,1,'Friends','admin',1,0),(1006,1,'Friends','test1',1,0),(1007,1,'Friends','test2',1,0),(1008,1,'Friends','test3',1,0),(1009,1,'Friends','test4',1,0),(1010,1,'Friends','test5',1,0),(1011,1,'Friends','test6',1,0);
/*!40000 ALTER TABLE `PersonalContactGroups` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `PersonalRelationship`
--

DROP TABLE IF EXISTS `PersonalRelationship`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `PersonalRelationship` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` varchar(16) NOT NULL,
  `ContactID` varchar(16) NOT NULL,
  `PersonalContactGroupSysID` int(10) unsigned NOT NULL COMMENT '联系人所在的个人组。0:Blacklist 1:Friends',
  `ContactRemarkName` varchar(32) DEFAULT NULL COMMENT '名称备注',
  `LastUpdateTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `-Relationship` tinyint(4) NOT NULL DEFAULT '1' COMMENT '成员关系\\n-1:blacklisted\\n0:stranger\\n1:approved',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `index-UserID-ContactID` (`UserID`,`ContactID`),
  KEY `FK-UF_UserID-USI_UserID` (`UserID`),
  KEY `FK-UF_ContactID-USI_UserID` (`ContactID`),
  KEY `FK-UF_PCGID-PCG_CGID` (`PersonalContactGroupSysID`),
  CONSTRAINT `FK-PR_PCGSysID-PCG_CGSysID` FOREIGN KEY (`PersonalContactGroupSysID`) REFERENCES `PersonalContactGroups` (`SysID`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `FK-UF_ContactID-USI_UserID` FOREIGN KEY (`ContactID`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK-UF_UserID-USI_UserID` FOREIGN KEY (`UserID`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=112 DEFAULT CHARSET=utf8 COMMENT='成员关系表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `PersonalRelationship`
--

LOCK TABLES `PersonalRelationship` WRITE;
/*!40000 ALTER TABLE `PersonalRelationship` DISABLE KEYS */;
INSERT INTO `PersonalRelationship` VALUES (104,'hehui','admin',1004,NULL,'2013-11-18 02:31:38',1),(105,'admin','hehui',1005,NULL,'2013-11-18 02:31:38',1),(106,'test1','hehui',1006,NULL,'2014-03-17 16:16:16',1),(107,'hehui','test1',1004,NULL,'2014-03-17 16:16:16',1),(108,'test2','hehui',1007,NULL,'2014-03-17 16:16:38',1),(109,'hehui','test2',1004,NULL,'2014-03-17 16:16:38',1),(110,'test2','test1',1007,NULL,'2014-03-17 16:25:40',1),(111,'test1','test2',1006,NULL,'2014-03-17 16:25:40',1);
/*!40000 ALTER TABLE `PersonalRelationship` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `UserRoles`
--

DROP TABLE IF EXISTS `UserRoles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UserRoles` (
  `ID` int(10) unsigned NOT NULL,
  `RoleName` varchar(16) NOT NULL,
  `Description` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `UserRoles`
--

LOCK TABLES `UserRoles` WRITE;
/*!40000 ALTER TABLE `UserRoles` DISABLE KEYS */;
INSERT INTO `UserRoles` VALUES (1,'Super Admin',''),(2,'Admin',''),(3,'user','');
/*!40000 ALTER TABLE `UserRoles` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `UsersDetailedInfo`
--

DROP TABLE IF EXISTS `UsersDetailedInfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UsersDetailedInfo` (
  `SysID` int(10) unsigned NOT NULL,
  `HomeAddress` varchar(255) DEFAULT NULL COMMENT '家庭住址',
  `HomePhoneNumber` varchar(32) DEFAULT NULL COMMENT '家庭电话号码',
  `HomeZipCode` varchar(12) DEFAULT NULL COMMENT '家庭邮编',
  `PersonalHomepage` varchar(255) DEFAULT NULL COMMENT '个人主页',
  `PersonalEmailAddress` varchar(255) DEFAULT NULL COMMENT '个人电子邮箱',
  `QuestionForSecurity` varchar(255) DEFAULT NULL,
  `AnswerForSecurity` varchar(255) DEFAULT NULL,
  `EmailForSecurity` varchar(255) DEFAULT NULL,
  `CompanyName` varchar(255) DEFAULT NULL,
  `JobTitle` varchar(255) DEFAULT NULL,
  `BusinessAddress` varchar(255) DEFAULT NULL,
  `BusinessPhoneNumber` varchar(255) DEFAULT NULL,
  `BusinessZipCode` varchar(255) DEFAULT NULL,
  `BusinessFaxNumber` varchar(255) DEFAULT NULL,
  `BusinessHomepage` varchar(255) DEFAULT NULL,
  `BusinessEmailAddress` varchar(255) DEFAULT NULL,
  `RegistrationTime` datetime DEFAULT NULL,
  PRIMARY KEY (`SysID`),
  KEY `FK-UDI_UserSysID-USI_SysID` (`SysID`),
  CONSTRAINT `FK-UDI_SysID-USI_SysID` FOREIGN KEY (`SysID`) REFERENCES `UsersSummaryInfo` (`SysID`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `UsersDetailedInfo`
--

LOCK TABLES `UsersDetailedInfo` WRITE;
/*!40000 ALTER TABLE `UsersDetailedInfo` DISABLE KEYS */;
INSERT INTO `UsersDetailedInfo` VALUES (16,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(17,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(18,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(19,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(20,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(21,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(22,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(23,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
/*!40000 ALTER TABLE `UsersDetailedInfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `UsersSummaryInfo`
--

DROP TABLE IF EXISTS `UsersSummaryInfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UsersSummaryInfo` (
  `SysID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` varchar(16) NOT NULL,
  `UserPassword` varchar(32) NOT NULL DEFAULT 'W6ph5Mm5Pz8GgiULbPgzG37mj9g=' COMMENT '密码',
  `TrueName` varchar(32) DEFAULT NULL,
  `NickName` varchar(32) DEFAULT NULL,
  `Gender` tinyint(1) DEFAULT '0' COMMENT '0:unknown, 1:male, 2:Female',
  `Age` tinyint(3) unsigned DEFAULT '0',
  `Face` varchar(255) DEFAULT NULL,
  `PersonalContactGroupsInfoVersion-` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '联系人组版本',
  `InterestGroupsInfoVersion` int(10) unsigned NOT NULL DEFAULT '0',
  `PersonalSummaryInfoVersion` int(11) NOT NULL DEFAULT '1',
  `PersonalDetailInfoVersion` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '个人信息版本',
  `PersonalMessageInfoVersion` int(10) unsigned NOT NULL DEFAULT '0',
  `FriendshipApply` tinyint(1) NOT NULL DEFAULT '0' COMMENT '0:AUTO_ACCEPT , 1:REQUIRE_AUTHENTICATION',
  `ShortTalk` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'AUTO_ACCEPT = 0, PROMPT = 1, DENY = 2',
  `Role` int(10) unsigned NOT NULL DEFAULT '3',
  `Description` varchar(256) DEFAULT NULL COMMENT '自我介绍',
  `AccountState` tinyint(1) NOT NULL DEFAULT '1' COMMENT '0:Invalid 1:Normal 2:Banned 3:Limitted',
  `PersonalMessage` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`SysID`),
  UNIQUE KEY `index_UsersSummaryInfo_UserID` (`UserID`)
) ENGINE=InnoDB AUTO_INCREMENT=24 DEFAULT CHARSET=utf8 COMMENT='基本信息表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `UsersSummaryInfo`
--

LOCK TABLES `UsersSummaryInfo` WRITE;
/*!40000 ALTER TABLE `UsersSummaryInfo` DISABLE KEYS */;
INSERT INTO `UsersSummaryInfo` VALUES (1,'System','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,0,'0',0,0,1,1,0,0,0,3,NULL,1,NULL),(16,'hehui','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,29,'1',0,1,1,1,0,1,0,3,NULL,1,NULL),(17,'admin','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,1,60,'2',0,2,1,1,0,0,0,3,NULL,1,NULL),(18,'test1','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,2,17,'3',0,2,1,1,0,1,0,3,NULL,1,NULL),(19,'test2','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,0,'0',0,0,1,1,0,2,0,3,NULL,1,NULL),(20,'test3','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,1,40,'1',0,0,1,1,0,1,0,3,NULL,1,NULL),(21,'test4','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,0,'2',0,20,1,1,0,0,0,3,NULL,1,NULL),(22,'test5','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,1,0,'3',0,0,1,1,0,0,0,3,NULL,1,NULL),(23,'test6','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,0,'0',0,0,1,1,0,0,0,3,NULL,1,NULL);
/*!40000 ALTER TABLE `UsersSummaryInfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'im'
--
/*!50003 DROP PROCEDURE IF EXISTS `sp_CacheChatMessage_FromContact_Get` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`%` PROCEDURE `sp_CacheChatMessage_FromContact_Get`(in p_userID varchar(16), in p_startTime datetime )
BEGIN


select Sender, Message, TransmittingTime 
from CachedChatMessages 
where Reciever=p_userID and TransmittingTime>p_startTime ; 


END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_CacheChatMessage_FromContact_Save` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_CacheChatMessage_FromContact_Save`(in p_senderID varchar(16), in p_recieverID varchar(16), in p_message varchar(255)  )
BEGIN

###刪除聯繫人###
#p_senderID:用戶ID
#p_recieverID：聯系人ID
#p_message:消息


insert into CachedChatMessages(Sender, Reciever, Message)  values(p_senderID, p_recieverID, p_message) ;


END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_CachedChatMessages_FromInterestGroup_Get` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`%` PROCEDURE `sp_CachedChatMessages_FromInterestGroup_Get`(in p_userID varchar(16), in p_startTime datetime )
BEGIN


select cigc.Sender, cigc.GroupID, cigc.Message, cigc.TransmittingTime 
from CachedInterestGroupChatMessages cigc, InterestGroupMembers igm
where igm.UserID=p_userID and igm.GroupID=cigc.GroupID and cigc.TransmittingTime>p_startTime ; 


END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_CachedChatMessages_FromInterestGroup_Save` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`%` PROCEDURE `sp_CachedChatMessages_FromInterestGroup_Save`(in p_senderID varchar(16), in p_groupID int(10) unsigned, in p_message varchar(255)  )
BEGIN

insert into CachedInterestGroupChatMessages(Sender, GroupID, Message) values(p_senderID, p_groupID, p_message );


END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_ContactGroup_CreateOrDelete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`%` PROCEDURE `sp_ContactGroup_CreateOrDelete`(in p_userID varchar(16), in p_groupID int(10) unsigned, in p_groupName varchar(32), in p_create tinyint )
BEGIN


if p_create = 0 then
    delete from PersonalContactGroups where GroupID = p_groupID and Creator = p_userID ;
else
    insert into PersonalContactGroups(GroupID, Creator, GroupName) values(p_groupID, p_userID, p_groupName) ;
end if;


END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_ContactGroup_UpdateName` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`%` PROCEDURE `sp_ContactGroup_UpdateName`(in p_userID varchar(16), in p_groupID int(10) unsigned , in p_newGroupName varchar(32) )
BEGIN

update PersonalContactGroups set GroupName = p_newGroupName  where Creator = p_userID and GroupID = p_groupID ;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_Contact_Add` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_Contact_Add`(in p_userID varchar(16), in p_contactID varchar(16), in p_contactGroupID int(10) unsigned )
BEGIN

###刪除聯繫人###
#p_userID:用戶ID
#p_contactID：聯系人ID
#p_contactGroupID:聯系人組ID


set @groupSysID = 0;
select SysID into @groupSysID from PersonalContactGroups where Creator=p_userID and GroupID=p_contactGroupID;

insert into PersonalRelationship(UserID, ContactID, PersonalContactGroupSysID)  values(p_userID, p_contactID, @groupSysID) ;


END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_Contact_Blacklisted_AddOrDelete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_Contact_Blacklisted_AddOrDelete`(in p_userID varchar(16), in p_contactID varchar(16), in p_addToBlacklist tinyint )
BEGIN

###新增或刪除被拉黑人員###
#p_userID:用戶ID
#p_contactID：被拉黑人員ID
#p_addToBlacklist：增加或刪除


if p_addToBlacklist <> 0 then
	insert into PersonalBlacklist values(p_userID, p_contactID, NULL) ;
else
	delete from PersonalBlacklist where Creator = p_userID and ContactID = p_contactID;
end if;

update UsersSummaryInfo set BlacklistInfoVersion = BlacklistInfoVersion+1 where UserID = p_userID;


END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_Contact_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_Contact_Delete`(in p_userID varchar(16), in p_contactID varchar(16), in p_deleteMeFromOpposition tinyint, in p_addToBlacklist tinyint )
BEGIN

###刪除聯繫人###
#p_userID:用戶ID
#p_contactID：聯系人ID
#p_deleteMeFromOpposition：從對方聯系人列表中刪除自己
#p_addToBlacklist：是否加入黑名單


if p_addToBlacklist <> 0 then
	#call sp_Contact_Blacklisted_AddOrDelete(p_userID, p_contactID, 1);
	
	set @groupSysID = null;
	select SysID into @groupSysID from PersonalContactGroups where GroupID=0 and Creator=p_userID;
	if @groupSysID is null then
		insert into PersonalContactGroups(GroupID, GroupName, Creator) values(0, 'Blacklist', p_userID) ;
		select SysID into @groupSysID from PersonalContactGroups where GroupID=0 and Creator=p_userID;
	end if;

	update PersonalRelationship  set PersonalContactGroupSysID=@groupSysID where UserID = p_userID and ContactID = p_contactID;

else
	delete from PersonalRelationship  where UserID = p_userID and ContactID = p_contactID;
end if;



if p_deleteMeFromOpposition <> 0 then
	delete from PersonalRelationship  where UserID = p_contactID and ContactID = p_userID;
end if;

#if p_addToBlacklist <> 0 then
#	call sp_Contact_Blacklisted_AddOrDelete(p_userID, p_contactID, 1);
#end if;




END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_Contact_MoveToAnotherGroup` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_Contact_MoveToAnotherGroup`(in p_userID varchar(16), in p_contactID varchar(16), in p_newContactGroupID int(10) unsigned)
BEGIN

###移動聯繫人###
#p_userID:用戶ID
#p_contactID：聯系人ID
#p_newContactGroupID:新的聯系人組


set @groupSysID = null;
select SysID into @groupSysID from PersonalContactGroups where Creator=p_userID and GroupID=p_newContactGroupID;

update PersonalRelationship  set PersonalContactGroupSysID=@groupSysID where UserID = p_userID and ContactID = p_contactID;


END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_FriendshipApplyRequest_Add` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_FriendshipApplyRequest_Add`(in p_sender varchar(16), in p_receiver varchar(16), in p_extraMessage varchar(32), in p_Result tinyint, in p_SenderRead tinyint, in p_ReceiverRead tinyint  )
BEGIN

insert into FriendshipApply(Sender, Receiver, ExtraMessage, Result, SenderRead, ReceiverRead) values(p_sender, p_receiver, p_extraMessage, p_Result, p_SenderRead, p_ReceiverRead);

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_FriendshipApplyRequest_Delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_FriendshipApplyRequest_Delete`(in p_sender varchar(16), in p_receiver varchar(16)  )
BEGIN

delete from FriendshipApply where Sender=p_sender and Receiver=p_receiver;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_FriendshipApplyRequest_ForUser_Update` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_FriendshipApplyRequest_ForUser_Update`(in p_userID varchar(16)  )
BEGIN


update FriendshipApply set SenderRead = 1 where Sender=p_userID and SenderRead = 0 ;
 
update FriendshipApply set ReceiverRead = 1 where Receiver=p_userID and ReceiverRead = 0 ;

delete from FriendshipApply where SenderRead = 1 and ReceiverRead = 1;
    

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_FriendshipApplyRequest_SentByUser_Get` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_FriendshipApplyRequest_SentByUser_Get`(in p_userID varchar(16)  )
BEGIN

select Receiver, Result, ExtraMessage from FriendshipApply where Sender = p_userID and ReceiverRead = 1;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_FriendshipApplyRequest_SentToUser_Get` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_FriendshipApplyRequest_SentToUser_Get`(in p_userID varchar(16)  )
BEGIN

select Sender, Result, ExtraMessage from FriendshipApply where Receiver=p_userID and ReceiverRead = 0;


END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_GetAllContactGroupsInfoForUserAsString` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_GetAllContactGroupsInfoForUserAsString`(in p_userID varchar(16)  )
BEGIN

select pcg.GroupID, pcg.GroupName,

(select 
GROUP_CONCAT( pr.ContactID SEPARATOR ',' ) as Contacts
from PersonalRelationship pr
where pr.UserID = p_userID and pr.PersonalContactGroupSysID = pcg.SysID
) as Contacts

from PersonalContactGroups pcg, PersonalRelationship pr
where pcg.Creator = p_userID and pcg.Creator = pr.UserID
group by pcg.GroupID;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_Contact_Search_MatchExactly` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_Contact_Search_MatchExactly`(in p_userID varchar(16), in p_nickName varchar(16) )
BEGIN

###查找聯繫人###
#p_userID:用戶ID
#p_nickName:暱稱

#select UserID, NickName, Gender, Age, Face, FriendshipApply from UsersSummaryInfo where UserID like p_userID and NickName like p_nickName ;


declare whereStatement varchar(1024) default ' ';


if CHAR_LENGTH(p_userID) <> 0 then
set whereStatement = CONCAT_WS('', whereStatement, ' and usi.UserID = \'', p_userID, '\'');
end if;

if CHAR_LENGTH(p_nickName) <> 0 then
set whereStatement = CONCAT_WS('', whereStatement, ' and usi.NickName = \'', p_nickName, '\'');
end if;


set @fullStatement = CONCAT_WS(' ', 'select usi.UserID, usi.NickName, usi.Gender, usi.Age, usi.Face, usi.FriendshipApply, udi.BusinessAddress from UsersSummaryInfo usi, UsersDetailedInfo udi where usi.SysID=udi.SysID ', whereStatement, ' limit 1' );


SET @sqlQuery=@fullStatement;
PREPARE s1 FROM @sqlQuery;
EXECUTE s1;
DEALLOCATE PREPARE s1;





END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_Contact_Search_MatchWildcard` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_Contact_Search_MatchWildcard`( in p_startAge tinyint(3) unsigned, in p_endAge tinyint(3) unsigned,  in p_gender tinyint(1), in p_hometown varchar(255), in p_businessAddress varchar(255), in p_startIndex int(10) unsigned, in p_pageSize int(10) unsigned )
BEGIN

###查找聯繫人###

declare whereStatement varchar(1024) default ' ';
declare limitStatement varchar(1024) default ' ';


if p_endAge <> 0 then
set whereStatement = CONCAT_WS(' ', whereStatement, ' and (usi.Age between ', p_startAge, ' and ', p_endAge, ') ');
end if;

if p_gender <> 0 then
set whereStatement = CONCAT_WS(' ', whereStatement, ' and usi.Gender=', p_gender);
end if;

if CHAR_LENGTH(p_hometown) <> 0 then
set whereStatement = CONCAT_WS('', whereStatement, ' and udi.HomeAddress like \'%', p_hometown, '%\'');
end if;

if CHAR_LENGTH(p_businessAddress) <> 0 then
set whereStatement = CONCAT_WS('', whereStatement, ' and udi.BusinessAddress like \'%', p_businessAddress, '%\'');
end if;


set limitStatement = CONCAT_WS(' ', ' limit ', p_startIndex, ',', p_pageSize);

set @fullStatement = CONCAT_WS(' ', 'select usi.UserID, usi.NickName, usi.Gender, usi.Age, usi.Face, usi.FriendshipApply, udi.BusinessAddress from UsersSummaryInfo usi, UsersDetailedInfo udi where usi.SysID=udi.SysID ', whereStatement, limitStatement );


#select @fullStatement;

SET @sqlQuery=@fullStatement;
PREPARE s1 FROM @sqlQuery;
EXECUTE s1;
DEALLOCATE PREPARE s1;





END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_CreateNewUser` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`%` PROCEDURE `sp_CreateNewUser`(in p_userID varchar(16), in p_password varchar(32), out p_sysid int(10) unsigned )
BEGIN

select SysID into p_sysid from UsersSummaryInfo where UserID = p_userID ;
if p_sysid is null then
    insert into UsersSummaryInfo(UserID, UserPassword) values(p_userID, p_password);
    select SysID into p_sysid from UsersSummaryInfo where UserID = p_userID ;
	insert into UsersDetailedInfo(SysID) values(p_sysid);

	insert into PersonalContactGroups(GroupID, GroupName, Creator) values(1, 'Friends', p_userID) ;

else    
    #如果记录已存在则u_sysid返回0
    set p_sysid = 0; 
end if;

#insert into UsersSummaryInfo(UserID, UserPassword, NickName, Gender) values(u_id, u_password, u_nickname, u_gender);
#select SysID into u_sysid from UsersSummaryInfo where UserID = u_id ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_GetAllContactsInfoForUser` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_GetAllContactsInfoForUser`(in p_userID varchar(16)  )
BEGIN

select pcg.GroupID, pcg.GroupName,

(
select GROUP_CONCAT( (select CONCAT_WS(',', usi.UserID, usi.PersonalSummaryInfoVersion, usi.PersonalDetailInfoVersion, usi.PersonalMessageInfoVersion )) SEPARATOR ';' ) 
from PersonalRelationship pr, UsersSummaryInfo usi
where pr.UserID = p_userID and pr.PersonalContactGroupSysID = pcg.SysID and usi.UserID = pr.ContactID 
) as Contacts

from PersonalContactGroups pcg, PersonalRelationship pr
where pcg.Creator = p_userID and pcg.Creator = pr.UserID
group by pcg.GroupID;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_GetAllContactsInfoForUserAsString` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_GetAllContactsInfoForUserAsString`(in p_userID varchar(16)  )
BEGIN

select  GROUP_CONCAT( (select CONCAT_WS(',', usi.UserID, usi.PersonalSummaryInfoVersion, usi.PersonalDetailInfoVersion, usi.PersonalMessageInfoVersion )) SEPARATOR ';' ) as Contacts
from  UsersSummaryInfo usi, PersonalRelationship pr 
where pr.UserID = p_userID and usi.UserID = pr.ContactID 
;#group by pr.PersonalContactGroup;



END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_GetInterestGroupInfo` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`200.200.200.17` PROCEDURE `sp_GetInterestGroupInfo`(in p_groupID int(10) unsigned)
BEGIN
	select * from InterestGroups where GroupID = p_groupID;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_GetInterestGroupMembers` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`200.200.200.17` PROCEDURE `sp_GetInterestGroupMembers`(in p_groupID int(10) unsigned)
BEGIN
	select UserID, RoleID from InterestGroupMembers where GroupID = p_groupID ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_GetUserContactGroups-` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_GetUserContactGroups-`(in p_userID int(10) unsigned, in p_fieldSepartor char(5) )
BEGIN

#UNUSED

#	select GROUP_CONCAT(GroupID SEPARATOR ',' ) from PersonalContactGroups where Creator = p_userID ;
    select CONCAT_WS(p_fieldSepartor, GroupID, GroupName, GroupInfoVersion, MemberListVersion) from PersonalContactGroups where Creator = p_userID ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_GetUserInfo` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_GetUserInfo`(in p_userID varchar(16) )
BEGIN
SELECT * FROM UsersSummaryInfo usi left join UsersDetailedInfo udi on usi.SysID=udi.SysID WHERE usi.UserID = p_userID;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_GetUserInfoBySysID` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_GetUserInfoBySysID`(in p_userSysID int(10) unsigned )
BEGIN
SELECT * FROM UsersSummaryInfo usi left join UsersDetailedInfo udi on usi.SysID=udi.SysID WHERE usi.SysID = p_userSysID;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_GetUserInterestGroups` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_GetUserInterestGroups`(in p_userID varchar(16)  )
BEGIN

select GROUP_CONCAT(igm.GroupID SEPARATOR ',' ) 
from InterestGroupMembers igm, InterestGroups ig
where igm.UserID = p_userID and igm.GroupID = ig.GroupID and ig.State<>0;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_GetUserLastLoginInfo` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_GetUserLastLoginInfo`(in p_userID varchar(16) )
BEGIN
    select ExtIPAddress , LoginTime, LogoutTime, DeviceInfo from LoginHistories  where UserID = p_userID order by LoginTime desc limit 1 ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_InterestGroup_Create` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`200.200.200.17` PROCEDURE `sp_InterestGroup_Create`(in p_creator varchar(16), in p_groupName varchar(32), out p_groupID int(10) unsigned)
BEGIN

	set p_groupID = 0;
	insert into InterestGroups(Creator, GroupName) values(p_creator, p_groupName);

	select GroupID into p_groupID from InterestGroups where Creator=p_creator and GroupName=p_groupName;
	insert into InterestGroupMembers(UserID, GroupID, RoleID) values(p_creator, p_groupID, 1);

	update UsersSummaryInfo set InterestGroupsInfoVersion = InterestGroupsInfoVersion+1 where UserID=p_creator;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_InterestGroup_Disband` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`200.200.200.17` PROCEDURE `sp_InterestGroup_Disband`(in p_groupID int(10) unsigned)
BEGIN

##LOG_GroupCreated=1,
##Log_GroupDisbanded=2,
##LOG_ContactInvitedToJoin=3,
##Log_ContactRequestToJoin=4,
##LOG_ContactJoined=5,
##LOG_ContactQuit=6,
##LOG_ContactFired=7,
##LOG_ContactBanned=8,
##LOG_ContactPromoted=9,
##LOG_ContactDemoted=10,
##LOG_ContactRenamed=11

  DECLARE done INT DEFAULT 0;
  DECLARE v_userID VARCHAR(16);
  DECLARE cur1 CURSOR FOR select UserID from InterestGroupMembers where GroupID = p_groupID;
  DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
 
  OPEN cur1;

  REPEAT
    FETCH cur1 INTO v_userID;

	IF NOT done THEN
		update UsersSummaryInfo set InterestGroupsInfoVersion = InterestGroupsInfoVersion+1 where UserID=v_userID;
		#insert into InterestGroupLogs(GroupID, Reciver, LogType) values(p_groupID, v_userID, 2);
	END IF;

  UNTIL done END REPEAT;
 
  CLOSE cur1;


	insert into InterestGroupLogs(GroupID, LogType) values(p_groupID, 2);

	#delete from InterestGroups where GroupID=p_groupID;
	update InterestGroups set State=0, DisbandedTime=now() where GroupID=p_groupID;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_InterestGroup_MemberJoinOrQuit` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`200.200.200.17` PROCEDURE `sp_InterestGroup_MemberJoinOrQuit`(in p_memberID varchar(16), in p_groupID int(10) unsigned, in p_join tinyint(1) unsigned, out p_membersVersion int(10) unsigned)
BEGIN


if p_join=0 then
	delete from InterestGroupMembers where UserID=p_memberID and GroupID=p_groupID;
else
	insert into InterestGroupMembers(UserID, GroupID, RoleID) values(p_memberID, p_groupID, 3);
end if;

	update InterestGroups set MemberListVersion = MemberListVersion+1 where GroupID=p_groupID;
	update UsersSummaryInfo set InterestGroupsInfoVersion = InterestGroupsInfoVersion+1 where UserID=p_memberID;

	select MemberListVersion into p_membersVersion from InterestGroups where GroupID=p_groupID;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_InterestGroup_SaveOrDeleteMembershipApplication` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`200.200.200.17` PROCEDURE `sp_InterestGroup_SaveOrDeleteMembershipApplication`(in p_userID varchar(16), in p_groupID int(10) unsigned, in p_message varchar(32), in p_save tinyint(1) unsigned)
BEGIN


if p_join=0 then
	delete from InterestGroupMembershipApplication where UserID=p_userID and GroupID=p_groupID;
else
	insert into InterestGroupMembershipApplication(UserID, GroupID, Message) values(p_userID, p_groupID, p_message);
end if;


END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_InterestGroup_Search` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`200.200.200.17` PROCEDURE `sp_InterestGroup_Search`(in p_groupID int(10) unsigned, in p_keyword varchar(32), in p_startIndex int(10) unsigned, in p_pageSize int(10) unsigned)
BEGIN


if p_groupID is null then
	select GroupID, GroupName, Creator, Description, Privacy  from InterestGroups where (GroupName like p_keyword or Description like p_keyword) and State=1 limit p_startIndex, p_pageSize;
else
	select GroupID, GroupName, Creator, Description, Privacy  from InterestGroups where GroupID=p_groupID and State=1 limit p_startIndex, p_pageSize;
end if;
	

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_LogUserIn` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_LogUserIn`(in p_userID varchar(16),  in p_hostAddress varchar(64), in p_hostPort smallint unsigned, in p_deviceInfo varchar(64), out p_loginTime datetime)
BEGIN

    set p_loginTime = now();
    insert into LoginHistories(UserID, ExtIPAddress, ExtPort, LoginTime, LogoutTime, DeviceInfo) values(p_userID, p_hostAddress, p_hostPort, p_loginTime, p_loginTime, p_deviceInfo); 

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_LogUserOut` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_LogUserOut`(in p_userID varchar(16),  in p_loginTime datetime, out p_logoutTime datetime)
BEGIN
	set p_logoutTime = now();
    update LoginHistories set LogoutTime = p_logoutTime where UserID = p_userID and LoginTime = p_loginTime;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-03-04 14:16:59


DELIMITER ;;
CREATE DEFINER=`root`@`200.200.200.17` PROCEDURE `sp_Contact_Search`(in p_MatchExactly tinyint(1) unsigned, in p_keyword varchar(16), in p_startAge tinyint(3) unsigned, in p_endAge tinyint(3) unsigned,  in p_gender tinyint(1), in p_hometown varchar(255), in p_businessAddress varchar(255), in p_startIndex int(10) unsigned, in p_pageSize int(10) unsigned )
BEGIN

###查找聯繫人###

declare whereStatement varchar(1024) default ' ';
declare limitStatement varchar(1024) default ' ';


if p_MatchExactly = 0 then
set whereStatement = CONCAT_WS('', whereStatement, ' and (usi.UserID like \'%', p_keyword, '%\'', ' or usi.NickName like \'%', p_keyword, '%\')');
else
set whereStatement = CONCAT_WS('', whereStatement, ' and (usi.UserID = \'', p_keyword, '\'', ' or usi.NickName = \'', p_keyword, '\')');
end if;

if p_endAge <> 0 then
set whereStatement = CONCAT_WS(' ', whereStatement, ' and (usi.Age between ', p_startAge, ' and ', p_endAge, ') ');
end if;

if p_gender <> 0 then
set whereStatement = CONCAT_WS(' ', whereStatement, ' and usi.Gender=', p_gender);
end if;

if CHAR_LENGTH(p_hometown) <> 0 then
set whereStatement = CONCAT_WS('', whereStatement, ' and udi.HomeAddress like \'%', p_hometown, '%\'');
end if;

if CHAR_LENGTH(p_businessAddress) <> 0 then
set whereStatement = CONCAT_WS('', whereStatement, ' and udi.BusinessAddress like \'%', p_businessAddress, '%\'');
end if;


set limitStatement = CONCAT_WS(' ', ' limit ', p_startIndex, ',', p_pageSize);

set @fullStatement = CONCAT_WS(' ', 'select usi.UserID, usi.NickName, usi.Gender, usi.Age, usi.Face, usi.FriendshipApply, udi.BusinessAddress from UsersSummaryInfo usi, UsersDetailedInfo udi where usi.SysID=udi.SysID ', whereStatement, limitStatement );


#select @fullStatement;

SET @sqlQuery=@fullStatement;
PREPARE s1 FROM @sqlQuery;
EXECUTE s1;
DEALLOCATE PREPARE s1;


END;;

DELIMITER ;
CREATE DATABASE  IF NOT EXISTS `im` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `im`;
-- MySQL dump 10.13  Distrib 5.6.11, for Win32 (x86)
--
-- Host: 200.200.200.40    Database: im
-- ------------------------------------------------------
-- Server version	5.5.28a-MariaDB-log

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
  `TransmittingTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '发送时间',
  `Message` varchar(255) NOT NULL COMMENT '消息',
  `ClientRead` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`),
  KEY `FK-CCM_Sender-USI_UserID` (`Sender`),
  KEY `FK-CCM_Receiver-USI_UserID` (`Reciever`),
  CONSTRAINT `FK-CCM_Receiver-USI_UserID` FOREIGN KEY (`Reciever`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK-CCM_Sender-USI_UserID` FOREIGN KEY (`Sender`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='缓存的消息';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `CachedChatMessages`
--

LOCK TABLES `CachedChatMessages` WRITE;
/*!40000 ALTER TABLE `CachedChatMessages` DISABLE KEYS */;
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
  PRIMARY KEY (`ID`),
  KEY `FK-CIGCM_Sender-USI_UserID` (`Sender`),
  KEY `FK-CIGCM_GroupID-IG_GroupID` (`GroupID`),
  CONSTRAINT `FK-CIGCM_GroupID-IG_GroupID` FOREIGN KEY (`GroupID`) REFERENCES `InterestGroups` (`GroupID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK-CIGCM_Sender-USI_UserID` FOREIGN KEY (`Sender`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='缓存的群组消息';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `CachedInterestGroupChatMessages`
--

LOCK TABLES `CachedInterestGroupChatMessages` WRITE;
/*!40000 ALTER TABLE `CachedInterestGroupChatMessages` DISABLE KEYS */;
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
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 COMMENT='好友请求';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `FriendshipApply`
--

LOCK TABLES `FriendshipApply` WRITE;
/*!40000 ALTER TABLE `FriendshipApply` DISABLE KEYS */;
INSERT INTO `FriendshipApply` VALUES (1,'admin','test1','QQQQQ',0,0,0),(7,'test1','test2','',0,0,0);
/*!40000 ALTER TABLE `FriendshipApply` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `InterestGroupMemberRoles`
--

DROP TABLE IF EXISTS `InterestGroupMemberRoles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `InterestGroupMemberRoles` (
  `RoleID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `RoleName` varchar(16) NOT NULL,
  `Description` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`RoleID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 COMMENT='组员在组中的角色';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `InterestGroupMemberRoles`
--

LOCK TABLES `InterestGroupMemberRoles` WRITE;
/*!40000 ALTER TABLE `InterestGroupMemberRoles` DISABLE KEYS */;
INSERT INTO `InterestGroupMemberRoles` VALUES (1,'Creator',NULL),(2,'Administrator',NULL),(3,'Member',NULL);
/*!40000 ALTER TABLE `InterestGroupMemberRoles` ENABLE KEYS */;
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
  `RoleID` int(10) unsigned NOT NULL COMMENT '成员角色',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `Index_UserID_GroupID` (`UserID`,`GroupID`) USING BTREE,
  KEY `FK-IGm_RoleID-IGMR_RoleID` (`RoleID`),
  KEY `FK-IGM_GroupID-IG_GroupID` (`GroupID`),
  CONSTRAINT `FK-IGM_GroupID-IG_GroupID` FOREIGN KEY (`GroupID`) REFERENCES `InterestGroups` (`GroupID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK-IGm_RoleID-IGMR_RoleID` FOREIGN KEY (`RoleID`) REFERENCES `InterestGroupMemberRoles` (`RoleID`) ON UPDATE CASCADE,
  CONSTRAINT `FK-IGM_UserID-USI_UserID` FOREIGN KEY (`UserID`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='组的成员';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `InterestGroupMembers`
--

LOCK TABLES `InterestGroupMembers` WRITE;
/*!40000 ALTER TABLE `InterestGroupMembers` DISABLE KEYS */;
/*!40000 ALTER TABLE `InterestGroupMembers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `InterestGroupMembershipApply`
--

DROP TABLE IF EXISTS `InterestGroupMembershipApply`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `InterestGroupMembershipApply` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `ApplicantUserID` varchar(16) NOT NULL,
  `GroupID` int(10) unsigned NOT NULL,
  `ExtraMessage` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `index-AUID-GroupID` (`ApplicantUserID`,`GroupID`),
  KEY `FK-IGMA_AUID-USI_UserID` (`ApplicantUserID`),
  KEY `FK-IGMA_GI_IG_GroupID` (`GroupID`),
  CONSTRAINT `FK-IGMA_AUID-USI_UserID` FOREIGN KEY (`ApplicantUserID`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK-IGMA_GI_IG_ID` FOREIGN KEY (`GroupID`) REFERENCES `InterestGroups` (`GroupID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='加入组请求';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `InterestGroupMembershipApply`
--

LOCK TABLES `InterestGroupMembershipApply` WRITE;
/*!40000 ALTER TABLE `InterestGroupMembershipApply` DISABLE KEYS */;
/*!40000 ALTER TABLE `InterestGroupMembershipApply` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `InterestGroupTypes`
--

DROP TABLE IF EXISTS `InterestGroupTypes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `InterestGroupTypes` (
  `TypeID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `ParentType` int(10) unsigned DEFAULT NULL,
  `TypeName` varchar(45) NOT NULL,
  `Description` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`TypeID`),
  UNIQUE KEY `TypeName_UNIQUE` (`TypeName`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 COMMENT='组类型';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `InterestGroupTypes`
--

LOCK TABLES `InterestGroupTypes` WRITE;
/*!40000 ALTER TABLE `InterestGroupTypes` DISABLE KEYS */;
INSERT INTO `InterestGroupTypes` VALUES (1,NULL,'System',NULL),(2,NULL,'User',NULL);
/*!40000 ALTER TABLE `InterestGroupTypes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `InterestGroups`
--

DROP TABLE IF EXISTS `InterestGroups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `InterestGroups` (
  `GroupID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `TypeID` int(10) unsigned NOT NULL COMMENT '类型',
  `ParentGroup` int(10) unsigned DEFAULT NULL COMMENT '父组的ID',
  `Creator` varchar(10) NOT NULL COMMENT '创建者',
  `GroupName` varchar(32) NOT NULL COMMENT '组名称',
  `CreationTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
  `GroupInfoVersion` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '组信息版本',
  `MemberListVersion` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '组成员列表版本',
  `Description` varchar(64) DEFAULT NULL,
  `Announcement` varchar(64) DEFAULT NULL COMMENT '公告',
  `Remark` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`GroupID`),
  KEY `FK-IG_Creator-USI_UserID` (`Creator`),
  KEY `FK-IF_TypeID-IGT_TypeID` (`TypeID`),
  CONSTRAINT `FK-IF_TypeID-IGT_TypeID` FOREIGN KEY (`TypeID`) REFERENCES `InterestGroupTypes` (`TypeID`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK-IG_Creator-USI_UserID` FOREIGN KEY (`Creator`) REFERENCES `UsersSummaryInfo` (`UserID`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='兴趣组';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `InterestGroups`
--

LOCK TABLES `InterestGroups` WRITE;
/*!40000 ALTER TABLE `InterestGroups` DISABLE KEYS */;
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
) ENGINE=InnoDB AUTO_INCREMENT=169 DEFAULT CHARSET=utf8 COMMENT='登陆历史';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `LoginHistories`
--

LOCK TABLES `LoginHistories` WRITE;
/*!40000 ALTER TABLE `LoginHistories` DISABLE KEYS */;
INSERT INTO `LoginHistories` VALUES (30,'admin','200.200.200.17',58583,NULL,NULL,'2013-08-14 06:08:26','2013-08-14 14:12:52','PC',NULL,NULL),(31,'admin','200.200.200.17',57157,NULL,NULL,'2013-08-14 06:30:59','2013-08-14 14:47:39','PC',NULL,NULL),(32,'admin','200.200.200.17',58802,NULL,NULL,'2013-08-14 06:48:07','2013-08-14 14:52:02','PC',NULL,NULL),(33,'admin','200.200.200.17',52940,NULL,NULL,'2013-08-14 06:52:58',NULL,'PC',NULL,NULL),(34,'admin','200.200.200.17',44607,NULL,NULL,'2013-08-14 06:59:10','2013-08-14 14:59:57','PC',NULL,NULL),(35,'admin','200.200.200.17',45754,NULL,NULL,'2013-08-14 07:03:03','2013-08-14 15:27:20','PC',NULL,NULL),(36,'admin','200.200.200.17',51967,NULL,NULL,'2013-08-14 08:03:10','2013-08-14 16:04:20','PC',NULL,NULL),(37,'admin','200.200.200.17',45484,NULL,NULL,'2013-08-14 08:05:40','2013-08-14 16:06:45','PC',NULL,NULL),(38,'admin','200.200.200.17',42369,NULL,NULL,'2013-08-14 08:11:24','2013-08-14 16:12:24','PC',NULL,NULL),(39,'admin','200.200.200.17',43323,NULL,NULL,'2013-08-14 08:32:27','2013-08-14 16:33:26','PC',NULL,NULL),(40,'admin','200.200.200.17',34539,NULL,NULL,'2013-08-14 08:35:39','2013-08-14 16:36:08','PC',NULL,NULL),(41,'admin','200.200.200.17',60313,NULL,NULL,'2013-08-14 08:41:17','2013-08-14 16:42:46','PC',NULL,NULL),(42,'admin','200.200.200.17',38813,NULL,NULL,'2013-08-14 08:49:44','2013-08-14 16:50:10','PC',NULL,NULL),(43,'admin','200.200.200.17',46148,NULL,NULL,'2013-08-14 08:51:38','2013-08-14 16:52:25','PC',NULL,NULL),(44,'admin','200.200.200.17',39081,NULL,NULL,'2013-08-14 08:55:32','2013-08-14 17:07:18','PC',NULL,NULL),(45,'admin','200.200.200.17',43348,NULL,NULL,'2013-08-14 09:08:26',NULL,'PC',NULL,NULL),(46,'admin','200.200.200.17',46028,NULL,NULL,'2013-08-14 09:15:58',NULL,'PC',NULL,NULL),(47,'admin','200.200.200.17',42796,NULL,NULL,'2013-08-14 09:24:45','2013-08-14 17:25:32','PC',NULL,NULL),(48,'admin','200.200.200.17',60501,NULL,NULL,'2013-08-14 12:00:36','2013-08-14 20:02:07','PC',NULL,NULL),(49,'admin','200.200.200.17',57261,NULL,NULL,'2013-08-14 12:11:07','2013-08-14 20:13:00','PC',NULL,NULL),(50,'admin','200.200.200.17',47414,NULL,NULL,'2013-08-17 00:41:40','2013-08-17 08:42:22','PC',NULL,NULL),(51,'admin','200.200.200.17',50403,NULL,NULL,'2013-08-17 00:44:46','2013-08-17 08:46:09','PC',NULL,NULL),(52,'admin','200.200.200.17',34977,NULL,NULL,'2013-08-26 02:04:43','2013-08-26 10:06:25','PC',NULL,NULL),(53,'admin','200.200.200.17',56984,NULL,NULL,'2013-08-26 03:13:57','2013-08-26 11:14:19','PC',NULL,NULL),(54,'admin','200.200.200.17',52566,NULL,NULL,'2013-08-26 03:15:22','2013-08-26 11:15:39','PC',NULL,NULL),(55,'admin','200.200.200.17',36593,NULL,NULL,'2013-08-26 03:40:50','2013-08-26 11:40:55','PC',NULL,NULL),(56,'admin','200.200.200.17',40570,NULL,NULL,'2013-08-26 03:42:49','2013-08-26 11:43:03','PC',NULL,NULL),(57,'admin','200.200.200.17',34010,NULL,NULL,'2013-08-26 03:46:26','2013-08-26 11:46:37','PC',NULL,NULL),(58,'admin','200.200.200.17',55138,NULL,NULL,'2013-08-26 04:35:20','2013-08-26 12:35:43','PC',NULL,NULL),(59,'admin','200.200.200.17',48405,NULL,NULL,'2013-08-27 08:56:36','2013-08-27 16:56:49','PC',NULL,NULL),(60,'admin','200.200.200.17',50470,NULL,NULL,'2013-08-29 04:06:51','2013-08-29 12:08:25','PC',NULL,NULL),(61,'admin','200.200.200.17',42027,NULL,NULL,'2013-08-29 04:20:08','2013-08-29 12:27:48','PC',NULL,NULL),(62,'admin','200.200.200.17',38419,NULL,NULL,'2013-08-29 04:33:26','2013-08-29 12:34:49','PC',NULL,NULL),(63,'admin','200.200.200.17',51330,NULL,NULL,'2013-08-29 05:58:53','2013-08-29 14:00:29','PC',NULL,NULL),(64,'admin','200.200.200.17',50981,NULL,NULL,'2013-08-29 06:02:52','2013-08-29 14:52:45','PC',NULL,NULL),(65,'admin','200.200.200.17',43008,NULL,NULL,'2013-08-29 06:57:26','2013-08-29 14:57:31','PC',NULL,NULL),(66,'admin','200.200.200.17',59986,NULL,NULL,'2013-08-29 06:58:01','2013-08-29 14:58:49','PC',NULL,NULL),(67,'admin','200.200.200.17',43906,NULL,NULL,'2013-08-29 07:14:12','2013-08-29 15:14:30','PC',NULL,NULL),(68,'admin','200.200.200.17',41732,NULL,NULL,'2013-08-30 02:15:34','2013-08-30 10:16:20','PC',NULL,NULL),(69,'admin','200.200.200.17',34309,NULL,NULL,'2013-08-30 07:33:55','2013-08-30 15:34:43','PC',NULL,NULL),(70,'admin','200.200.200.17',49731,NULL,NULL,'2013-08-30 08:04:43','2013-08-30 16:05:30','PC',NULL,NULL),(71,'admin','200.200.200.17',40522,NULL,NULL,'2013-08-30 08:15:50','2013-08-30 16:16:37','PC',NULL,NULL),(72,'admin','200.200.200.17',45488,NULL,NULL,'2013-08-30 08:31:34','2013-08-30 16:32:50','PC',NULL,NULL),(73,'admin','200.200.200.17',36882,NULL,NULL,'2013-08-30 08:34:54','2013-08-30 16:36:22','PC',NULL,NULL),(74,'admin','200.200.200.17',37600,NULL,NULL,'2013-08-30 08:36:50','2013-08-30 16:38:22','PC',NULL,NULL),(75,'admin','200.200.200.17',55583,NULL,NULL,'2013-08-30 08:49:27','2013-08-30 16:50:37','PC',NULL,NULL),(76,'admin','200.200.200.17',50151,NULL,NULL,'2013-08-30 08:53:34',NULL,'PC',NULL,NULL),(77,'admin','200.200.200.17',40982,NULL,NULL,'2013-08-30 09:11:07','2013-08-30 17:11:51','PC',NULL,NULL),(78,'admin','200.200.200.17',48896,NULL,NULL,'2013-08-30 09:12:07','2013-08-30 17:13:41','PC',NULL,NULL),(79,'admin','200.200.200.17',38434,NULL,NULL,'2013-08-30 09:31:18',NULL,'PC',NULL,NULL),(80,'admin','200.200.200.17',49092,NULL,NULL,'2013-08-30 09:46:00','2013-08-30 17:46:06','PC',NULL,NULL),(81,'admin','200.200.200.17',38303,NULL,NULL,'2013-08-30 09:52:37','2013-08-30 17:53:03','PC',NULL,NULL),(82,'admin','200.200.200.17',57292,NULL,NULL,'2013-08-30 09:54:26','2013-08-30 17:54:59','PC',NULL,NULL),(83,'admin','200.200.200.17',47860,NULL,NULL,'2013-08-30 09:55:20','2013-08-30 17:55:32','PC',NULL,NULL),(84,'admin','200.200.200.17',45037,NULL,NULL,'2013-08-30 09:56:06','2013-08-30 17:57:36','PC',NULL,NULL),(85,'admin','200.200.200.17',57843,NULL,NULL,'2013-08-30 09:57:39','2013-08-30 17:59:16','PC',NULL,NULL),(86,'admin','200.200.200.17',44975,NULL,NULL,'2013-08-31 01:34:13','2013-08-31 09:34:45','PC',NULL,NULL),(87,'admin','200.200.200.17',60530,NULL,NULL,'2013-08-31 01:35:07','2013-08-31 09:35:23','PC',NULL,NULL),(88,'admin','200.200.200.17',33351,NULL,NULL,'2013-08-31 09:37:43','2013-08-31 17:38:49','PC',NULL,NULL),(89,'admin','200.200.200.17',43360,NULL,NULL,'2013-08-31 09:40:25','2013-08-31 17:44:26','PC',NULL,NULL),(90,'admin','200.200.200.17',58344,NULL,NULL,'2013-08-31 10:05:09','2013-08-31 18:06:09','PC',NULL,NULL),(91,'admin','200.200.200.17',39669,NULL,NULL,'2013-08-31 10:07:50','2013-08-31 18:08:13','PC',NULL,NULL),(92,'admin','200.200.200.17',44611,NULL,NULL,'2013-08-31 10:13:14','2013-08-31 18:13:35','PC',NULL,NULL),(93,'admin','200.200.200.17',49562,NULL,NULL,'2013-08-31 10:23:09','2013-08-31 18:25:00','PC',NULL,NULL),(94,'admin','200.200.200.17',44454,NULL,NULL,'2013-08-31 11:16:42','2013-08-31 19:17:06','PC',NULL,NULL),(95,'admin','200.200.200.17',41096,NULL,NULL,'2013-08-31 11:21:31','2013-08-31 19:22:54','PC',NULL,NULL),(96,'admin','200.200.200.17',39540,NULL,NULL,'2013-08-31 11:23:14','2013-08-31 19:23:20','PC',NULL,NULL),(97,'admin','200.200.200.17',41640,NULL,NULL,'2013-08-31 11:27:47','2013-08-31 19:28:34','PC',NULL,NULL),(98,'admin','200.200.200.17',35497,NULL,NULL,'2013-08-31 11:33:29','2013-08-31 19:34:16','PC',NULL,NULL),(99,'admin','200.200.200.17',38430,NULL,NULL,'2013-08-31 11:35:54','2013-08-31 19:35:58','PC',NULL,NULL),(100,'admin','200.200.200.17',49720,NULL,NULL,'2013-08-31 11:36:12','2013-08-31 19:39:10','PC',NULL,NULL),(101,'admin','200.200.200.17',33891,NULL,NULL,'2013-08-31 12:01:10','2013-08-31 20:03:16','PC',NULL,NULL),(102,'admin','200.200.200.17',51886,NULL,NULL,'2013-09-01 08:19:49','2013-09-01 16:22:17','PC',NULL,NULL),(103,'admin','200.200.200.17',53557,NULL,NULL,'2013-09-01 09:07:47','2013-09-01 17:08:35','PC',NULL,NULL),(104,'admin','200.200.200.17',43999,NULL,NULL,'2013-09-01 10:16:20','2013-09-01 18:17:11','PC',NULL,NULL),(105,'admin','200.200.200.17',37330,NULL,NULL,'2013-09-02 00:56:46','2013-09-02 08:57:14','PC',NULL,NULL),(106,'admin','200.200.200.17',42913,NULL,NULL,'2013-09-02 00:57:25','2013-09-02 08:57:35','PC',NULL,NULL),(107,'admin','200.200.200.17',45904,NULL,NULL,'2013-09-02 01:20:07','2013-09-02 09:20:22','PC',NULL,NULL),(108,'admin','200.200.200.17',37864,NULL,NULL,'2013-09-02 01:20:31','2013-09-02 09:24:00','PC',NULL,NULL),(109,'admin','200.200.200.17',35060,NULL,NULL,'2013-09-02 01:32:34','2013-09-02 09:38:28','PC',NULL,NULL),(110,'admin','200.200.200.17',53377,NULL,NULL,'2013-09-02 01:41:19','2013-09-02 09:42:35','PC',NULL,NULL),(111,'admin','200.200.200.17',50493,NULL,NULL,'2013-09-02 02:33:20','2013-09-02 10:34:19','PC',NULL,NULL),(112,'admin','200.200.200.17',59316,NULL,NULL,'2013-09-02 02:57:18','2013-09-02 11:00:51','PC',NULL,NULL),(113,'admin','200.200.200.17',36402,NULL,NULL,'2013-09-02 03:04:47','2013-09-02 11:05:55','PC',NULL,NULL),(114,'admin','200.200.200.17',34725,NULL,NULL,'2013-09-02 03:13:50','2013-09-02 11:14:02','PC',NULL,NULL),(115,'admin','200.200.200.17',35976,NULL,NULL,'2013-09-02 03:14:16',NULL,'PC',NULL,NULL),(116,'admin','200.200.200.17',34589,NULL,NULL,'2013-09-02 03:25:02','2013-09-02 11:26:04','PC',NULL,NULL),(117,'admin','200.200.200.17',59313,NULL,NULL,'2013-09-02 03:26:13','2013-09-02 11:33:19','PC',NULL,NULL),(118,'admin','200.200.200.17',58624,NULL,NULL,'2013-09-02 03:33:46','2013-09-02 11:56:11','PC',NULL,NULL),(119,'admin','200.200.200.17',60396,NULL,NULL,'2013-09-02 04:01:02','2013-09-02 12:23:48','PC',NULL,NULL),(120,'admin','200.200.200.17',38895,NULL,NULL,'2013-09-02 04:25:04','2013-09-02 12:28:52','PC',NULL,NULL),(121,'admin','200.200.200.17',55173,NULL,NULL,'2013-09-02 06:01:16','2013-09-02 14:20:07','PC',NULL,NULL),(122,'admin','200.200.200.17',37877,NULL,NULL,'2013-09-02 06:25:36','2013-09-02 14:33:14','PC',NULL,NULL),(123,'admin','200.200.200.17',56723,NULL,NULL,'2013-09-02 06:35:16','2013-09-02 14:39:54','PC',NULL,NULL),(124,'admin','200.200.200.17',48868,NULL,NULL,'2013-09-02 06:53:53','2013-09-02 14:58:40','PC',NULL,NULL),(125,'admin','200.200.200.17',36752,NULL,NULL,'2013-09-02 06:59:06','2013-09-02 15:01:09','PC',NULL,NULL),(126,'admin','200.200.200.17',34809,NULL,NULL,'2013-09-02 07:32:21','2013-09-02 15:36:45','PC',NULL,NULL),(127,'admin','200.200.200.17',33331,NULL,NULL,'2013-09-02 08:24:43','2013-09-02 16:26:02','PC',NULL,NULL),(128,'test1','200.200.200.17',48544,NULL,NULL,'2013-09-02 08:26:30','2013-09-02 16:35:43','PC',NULL,NULL),(129,'admin','200.200.200.17',48564,NULL,NULL,'2013-09-02 08:42:14','2013-09-02 16:43:35','PC',NULL,NULL),(130,'test1','200.200.200.17',57915,NULL,NULL,'2013-09-02 08:45:37','2013-09-02 16:46:59','PC',NULL,NULL),(131,'test1','200.200.200.17',55824,NULL,NULL,'2013-09-02 08:47:08','2013-09-02 16:48:02','PC',NULL,NULL),(132,'admin','200.200.200.17',60307,NULL,NULL,'2013-09-02 09:43:47','2013-09-02 17:44:15','PC',NULL,NULL),(133,'test1','200.200.200.17',53872,NULL,NULL,'2013-09-02 09:44:24','2013-09-02 17:46:05','PC',NULL,NULL),(134,'test1','200.200.200.17',47741,NULL,NULL,'2013-09-02 09:46:24','2013-09-02 17:47:11','PC',NULL,NULL),(135,'test1','200.200.200.17',41766,NULL,NULL,'2013-09-02 09:56:50','2013-09-02 17:57:23','PC',NULL,NULL),(136,'test1','200.200.200.17',50198,NULL,NULL,'2013-09-02 10:01:38','2013-09-02 18:02:04','PC',NULL,NULL),(137,'test1','200.200.200.17',37862,NULL,NULL,'2013-09-02 10:07:33','2013-09-02 18:08:52','PC',NULL,NULL),(138,'test1','200.200.200.17',49080,NULL,NULL,'2013-09-03 02:51:35','2013-09-03 10:53:05','PC',NULL,NULL),(139,'test1','200.200.200.17',40329,NULL,NULL,'2013-09-03 03:01:00','2013-09-03 11:01:48','PC',NULL,NULL),(140,'test1','200.200.200.17',42241,NULL,NULL,'2013-09-03 03:02:02','2013-09-03 11:04:46','PC',NULL,NULL),(141,'test1','200.200.200.17',37620,NULL,NULL,'2013-09-03 03:06:37','2013-09-03 11:06:55','PC',NULL,NULL),(142,'admin','200.200.200.17',57788,NULL,NULL,'2013-09-05 12:25:16','2013-09-05 20:25:41','PC',NULL,NULL),(143,'admin','200.200.200.17',43648,NULL,NULL,'2013-09-05 12:25:55','2013-09-05 20:26:03','PC',NULL,NULL),(144,'admin','200.200.200.17',58528,NULL,NULL,'2013-09-05 12:32:00','2013-09-05 20:34:11','PC',NULL,NULL),(145,'admin','200.200.200.17',34009,NULL,NULL,'2013-09-06 00:34:21','2013-09-06 08:39:32','PC',NULL,NULL),(146,'admin','200.200.200.17',37329,NULL,NULL,'2013-09-06 03:24:59','2013-09-06 11:33:27','PC',NULL,NULL),(147,'admin','200.200.200.17',40909,NULL,NULL,'2013-09-06 04:32:30','2013-09-06 12:35:06','PC',NULL,NULL),(148,'admin','200.200.200.17',49501,NULL,NULL,'2013-09-06 06:53:55','2013-09-06 14:56:29','PC',NULL,NULL),(149,'admin','200.200.200.17',33693,NULL,NULL,'2013-09-06 06:59:13','2013-09-06 15:00:18','PC',NULL,NULL),(150,'admin','200.200.200.17',34542,NULL,NULL,'2013-09-06 08:01:24','2013-09-06 16:06:16','PC',NULL,NULL),(151,'admin','200.200.200.17',44228,NULL,NULL,'2013-09-06 08:06:25','2013-09-06 16:06:43','PC',NULL,NULL),(152,'admin','200.200.200.17',56967,NULL,NULL,'2013-09-06 08:26:24','2013-09-06 16:28:05','PC',NULL,NULL),(153,'admin','200.200.200.17',56476,NULL,NULL,'2013-09-06 08:29:21','2013-09-06 16:30:08','PC',NULL,NULL),(154,'admin','200.200.200.17',43523,NULL,NULL,'2013-09-06 08:43:24','2013-09-06 16:43:31','PC',NULL,NULL),(155,'admin','200.200.200.17',46496,NULL,NULL,'2013-09-06 08:51:26','2013-09-06 16:52:56','PC',NULL,NULL),(156,'admin','200.200.200.17',55021,NULL,NULL,'2013-09-06 09:56:06','2013-09-06 17:56:19','PC',NULL,NULL),(157,'admin','200.200.200.17',59407,NULL,NULL,'2013-09-09 05:59:44','2013-09-09 14:00:45','PC',NULL,NULL),(158,'test4','200.200.200.17',43399,NULL,NULL,'2013-09-09 08:14:41','2013-09-09 16:15:06','PC',NULL,NULL),(159,'test4','200.200.200.17',42589,NULL,NULL,'2013-09-09 08:15:16','2013-09-09 16:16:58','PC',NULL,NULL),(160,'test4','200.200.200.17',49614,NULL,NULL,'2013-09-09 08:17:29','2013-09-09 16:17:55','PC',NULL,NULL),(161,'test4','200.200.200.17',38837,NULL,NULL,'2013-09-09 08:18:30','2013-09-09 16:19:07','PC',NULL,NULL),(162,'test4','200.200.200.17',42337,NULL,NULL,'2013-09-09 08:21:03','2013-09-09 16:40:38','PC',NULL,NULL),(163,'test4','200.200.200.17',50581,NULL,NULL,'2013-09-09 08:42:40','2013-09-09 16:53:48','PC',NULL,NULL),(164,'test4','200.200.200.17',49915,NULL,NULL,'2013-09-09 08:54:10','2013-09-09 16:55:21','PC',NULL,NULL),(165,'test4','200.200.200.17',40137,NULL,NULL,'2013-09-09 08:55:55','2013-09-09 16:56:15','PC',NULL,NULL),(166,'test4','200.200.200.17',39696,NULL,NULL,'2013-09-09 08:58:33','2013-09-09 17:00:20','PC',NULL,NULL),(167,'test4','200.200.200.17',58583,NULL,NULL,'2013-09-09 09:02:42','2013-09-09 17:03:52','PC',NULL,NULL),(168,'test4','200.200.200.17',38425,NULL,NULL,'2013-09-09 09:27:36','2013-09-09 17:27:41','PC',NULL,NULL);
/*!40000 ALTER TABLE `LoginHistories` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `PersonalBlacklist`
--

DROP TABLE IF EXISTS `PersonalBlacklist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `PersonalBlacklist` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Creator` varchar(16) NOT NULL,
  `ContactID` varchar(16) NOT NULL,
  `BlacklistedTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx-Creator-ContactID` (`Creator`,`ContactID`),
  KEY `FK-Creator-USI_UserID_idx` (`Creator`),
  KEY `FK-ContactID-USI_UserID_idx` (`ContactID`),
  CONSTRAINT `FK-Creator-USI_UserID` FOREIGN KEY (`Creator`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE,
  CONSTRAINT `FK-ContactID-USI_UserID` FOREIGN KEY (`ContactID`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='黑名單';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `PersonalBlacklist`
--

LOCK TABLES `PersonalBlacklist` WRITE;
/*!40000 ALTER TABLE `PersonalBlacklist` DISABLE KEYS */;
/*!40000 ALTER TABLE `PersonalBlacklist` ENABLE KEYS */;
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
) ENGINE=InnoDB AUTO_INCREMENT=1029 DEFAULT CHARSET=utf8 COMMENT='个人联系人组';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `PersonalContactGroups`
--

LOCK TABLES `PersonalContactGroups` WRITE;
/*!40000 ALTER TABLE `PersonalContactGroups` DISABLE KEYS */;
INSERT INTO `PersonalContactGroups` VALUES (1004,1,'Friends','hehui',1,0),(1005,1,'Friends','admin',1,0),(1006,1,'Friends','test1',1,0),(1007,1,'Friends','test2',1,0),(1008,1,'Friends','test3',1,0),(1009,1,'Friends','test4',1,0),(1010,1,'Friends','test5',1,0),(1011,1,'Friends','test6',1,0),(1028,2,'TG11','admin',2,0);
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
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8 COMMENT='成员关系表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `PersonalRelationship`
--

LOCK TABLES `PersonalRelationship` WRITE;
/*!40000 ALTER TABLE `PersonalRelationship` DISABLE KEYS */;
INSERT INTO `PersonalRelationship` VALUES (6,'admin','test4',1005,NULL,'2013-08-14 06:10:51',1),(7,'test4','admin',1009,NULL,'2013-08-14 06:10:51',1),(8,'admin','test5',1005,NULL,'2013-09-01 09:00:09',1),(9,'admin','test6',1005,NULL,'2013-09-01 09:01:48',1);
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
  `PersonalContactGroupsInfoVersion` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '联系人组版本',
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
INSERT INTO `UsersSummaryInfo` VALUES (16,'hehui','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,29,NULL,0,0,1,1,0,1,0,3,NULL,1,NULL),(17,'admin','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,1,60,NULL,25,0,1,1,0,0,0,3,NULL,1,NULL),(18,'test1','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,2,17,NULL,0,0,1,1,0,1,0,3,NULL,1,NULL),(19,'test2','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,0,NULL,0,0,1,1,0,2,0,3,NULL,1,NULL),(20,'test3','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,1,40,NULL,0,0,1,1,0,1,0,3,NULL,1,NULL),(21,'test4','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,0,NULL,0,0,1,1,0,0,0,3,NULL,1,NULL),(22,'test5','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,1,0,NULL,0,0,1,1,0,0,0,3,NULL,1,NULL),(23,'test6','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,0,NULL,0,0,1,1,0,0,0,3,NULL,1,NULL);
/*!40000 ALTER TABLE `UsersSummaryInfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'im'
--
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



set @fullStatement = CONCAT_WS(' ', 'select usi.UserID, usi.NickName, usi.Gender, usi.Age, usi.Face, usi.FriendshipApply, udi.BusinessAddress from UsersSummaryInfo usi, UsersDetailedInfo udi where usi.SysID=udi.SysID ', whereStatement );


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
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_Contact_Search_MatchWildcard`( in p_startAge tinyint(3) unsigned, in p_endAge tinyint(3) unsigned,  in p_gender tinyint(1), in p_hometown varchar(255), in p_businessAddress varchar(255) )
BEGIN

###查找聯繫人###

declare whereStatement varchar(1024) default ' ';


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




#select usi.UserID, usi.NickName, usi.Gender, usi.Age, usi.Face, usi.FriendshipApply 
#from UsersSummaryInfo usi, UsersDetailedInfo udi 
##where usi.SysID=udi.SysID and (usi.Age between p_startAge and p_endAge) and CONCAT_ws(' ', whereStatement) ;
#where usi.SysID=udi.SysID and CONCAT_ws(' ', whereStatement) ;


set @fullStatement = CONCAT_WS(' ', 'select usi.UserID, usi.NickName, usi.Gender, usi.Age, usi.Face, usi.FriendshipApply, udi.BusinessAddress from UsersSummaryInfo usi, UsersDetailedInfo udi where usi.SysID=udi.SysID ', whereStatement );


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
where pr.UserID = 'admin' and pr.PersonalContactGroupSysID = pcg.SysID and usi.UserID = pr.ContactID 
) as Contacts

from PersonalContactGroups pcg, PersonalRelationship pr
where pcg.Creator = 'admin' and pcg.Creator = pr.UserID
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
/*!50003 DROP PROCEDURE IF EXISTS `sp_GetBlacklistedContactsInfoForUserAsString` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_GetBlacklistedContactsInfoForUserAsString`(in p_userID varchar(16) )
BEGIN

select GROUP_CONCAT(ContactID SEPARATOR ',') from PersonalBlacklist where Creator = p_userID ;


END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_GetCachedChatMessages` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`%` PROCEDURE `sp_GetCachedChatMessages`(in p_userID varchar(16), in p_startTime datetime )
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
/*!50003 DROP PROCEDURE IF EXISTS `sp_GetCachedInterestGroupChatMessages` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`%` PROCEDURE `sp_GetCachedInterestGroupChatMessages`(in p_userID varchar(16), in p_startTime datetime )
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
/*!50003 DROP PROCEDURE IF EXISTS `sp_GetFriendshipApplyRequestSentByUser` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_GetFriendshipApplyRequestSentByUser`(in p_userID varchar(16)  )
BEGIN

select Receiver, Result, ExtraMessage from FriendshipApply where Sender = p_userID and ReceiverRead = 1;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_GetFriendshipApplyRequestSentToUser` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_GetFriendshipApplyRequestSentToUser`(in p_userID varchar(16)  )
BEGIN

select Sender, Result, ExtraMessage from FriendshipApply where Receiver=p_userID and ReceiverRead = 0;


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
	select GROUP_CONCAT(GroupID SEPARATOR ',' ) from InterestGroupMembers where UserID = p_userID ;
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
    insert into LoginHistories(UserID, ExtIPAddress, ExtPort, LoginTime, DeviceInfo) values(p_userID, p_hostAddress, p_hostPort, p_loginTime, p_deviceInfo); 

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
/*!50003 DROP PROCEDURE IF EXISTS `sp_UpdateFriendshipApplyRequestForUser` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_UpdateFriendshipApplyRequestForUser`(in p_userID varchar(16)  )
BEGIN


update FriendshipApply set SenderRead = 1 where SenderID=p_userID and SenderRead = 0 ;
 
update FriendshipApply set ReceiverRead = 1 where ReceiverID=p_userID and ReceiverRead = 0 ;
    

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

-- Dump completed on 2013-09-09 17:47:17

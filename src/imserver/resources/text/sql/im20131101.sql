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
) ENGINE=InnoDB AUTO_INCREMENT=225 DEFAULT CHARSET=utf8 COMMENT='缓存的消息';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `CachedChatMessages`
--

LOCK TABLES `CachedChatMessages` WRITE;
/*!40000 ALTER TABLE `CachedChatMessages` DISABLE KEYS */;
INSERT INTO `CachedChatMessages` VALUES (174,'admin','test5','2013-09-20 07:03:12','<div><p><img src=\"qrc:/emoticons/32.gif/\"/> </p></div>',0),(175,'test4','admin','2013-09-24 08:46:45','<div><p>EEEEEE</p></div>',0),(176,'test4','admin','2013-09-24 08:46:48','<div><p><img src=\"qrc:/emoticons/2.gif/\"/> </p></div>',0),(177,'test4','admin','2013-10-09 09:30:53','<div><p>QQQQQQQQQQQQQQQQQQQQQQQQQQEEEEEEEEEEEEEEEEE</p></div>',0),(178,'test4','admin','2013-10-09 09:31:08','<div><p>QQQQQQQQQQQQQ QQQQQQQQQQQQQEEEEEEEEEEEEEEEEE</p></div>',0),(179,'test4','admin','2013-10-10 07:07:10','<div><p>QQQQQ</p></div>',0),(180,'test4','admin','2013-10-10 07:07:51','<div><p>eeeeee</p></div>',0),(181,'test4','admin','2013-10-10 07:08:16','<div><p>rrrrrrewew</p></div>',0),(182,'test4','admin','2013-10-10 07:09:07','<div><p>vvvvvv</p></div>',0),(183,'test4','admin','2013-10-10 07:16:38','<div><p>eeeee</p></div>',0),(184,'test4','admin','2013-10-10 07:18:21','<div><p>ddd</p></div>',0),(185,'test4','admin','2013-10-10 07:23:56','<div><p>sssss</p></div>',0),(186,'test4','admin','2013-10-10 07:24:31','<div><p>eeeeee</p></div>',0),(187,'test4','admin','2013-10-11 00:49:07','<div><p>dddddd</p></div>',0),(188,'test4','admin','2013-10-11 00:51:24','<div><p>fads</p></div>',0),(189,'admin','test4','2013-10-11 00:53:56','<div><p>ddd</p></div>',0),(190,'admin','test4','2013-10-11 00:54:24','<div><p><img src=\"88f103f00cce766f31ca25ca9ee7d3e2.png\"/></p></div>',0),(191,'test4','admin','2013-10-11 04:28:05','<div><p><img src=\"8c26d736b1401904729b0ed0af7748e4.png\"/></p></div>',0),(192,'admin','test4','2013-10-11 08:11:43','<div><p>dddddddd</p></div>',0),(193,'admin','test4','2013-10-11 08:12:55','<div><p>ssss</p></div>',0),(194,'admin','test4','2013-10-11 08:13:00','<div><p>rrrrr</p></div>',0),(195,'admin','test4','2013-10-11 08:18:58','<div><p>dddd</p></div>',0),(196,'admin','test4','2013-10-11 08:19:02','<div><p>sssssssssssss</p></div>',0),(197,'admin','test4','2013-10-11 08:31:48','<div><p>dfdfdfdf</p></div>',0),(198,'admin','test4','2013-10-14 02:26:07','<div><p>qqqqq</p></div>',0),(199,'admin','test5','2013-10-14 02:26:58','<div><p>dddd</p></div>',0),(200,'admin','test6','2013-10-14 02:27:40','<div><p>ffffffffffffff</p></div>',0),(201,'test4','admin','2013-10-14 02:29:38','<div><p>vvvvvvvvvvvvv</p></div>',0),(202,'test4','admin','2013-10-14 02:39:52','<div><p>iiiiiiiiiiiiiiiii</p></div>',0),(203,'test4','admin','2013-10-14 02:39:59','<div><p>bbbbbbbbbbbb</p></div>',0),(204,'test4','admin','2013-10-14 02:40:02','<div><p>xxxxxxxxxxxx</p></div>',0),(205,'hehui','admin','2013-10-31 08:22:21','<div><p>eeeeeeeee</p></div>',0),(206,'hehui','admin','2013-10-31 08:29:10','<div><p>wwww</p></div>',0),(207,'admin','hehui','2013-10-31 08:38:02','<div><p>dddd</p></div>',0),(208,'admin','hehui','2013-10-31 10:56:00','<div><p>dddd</p></div>',0),(209,'hehui','admin','2013-10-31 10:56:30','<div><p>eee</p></div>',0),(210,'admin','hehui','2013-10-31 11:54:09','<div><p>aaa</p></div>',0),(211,'hehui','admin','2013-10-31 11:57:35','<div><p>dddd</p></div>',0),(212,'admin','hehui','2013-10-31 11:58:44','<div><p>sssss</p></div>',0),(213,'admin','hehui','2013-11-01 03:10:26','<div><p>qqq</p></div>',0),(214,'admin','hehui','2013-11-01 03:10:27','<div><p>qqq</p></div>',0),(215,'admin','hehui','2013-11-01 03:10:29','<div><p>qqqq</p></div>',0),(216,'admin','hehui','2013-11-01 03:10:30','<div><p>qqqq</p></div>',0),(217,'admin','hehui','2013-11-01 03:13:59','<div><p>rrr</p></div>',0),(218,'admin','hehui','2013-11-01 03:14:02','<div><p>ttt</p></div>',0),(219,'hehui','admin','2013-11-01 03:14:53','<div><p>q</p></div>',0),(220,'hehui','admin','2013-11-01 03:14:55','<div><p>w</p></div>',0),(221,'hehui','admin','2013-11-01 03:14:56','<div><p>e</p></div>',0),(222,'hehui','admin','2013-11-01 03:14:57','<div><p>r</p></div>',0),(223,'hehui','admin','2013-11-01 03:14:58','<div><p>t</p></div>',0),(224,'hehui','admin','2013-11-01 03:15:17','<div><p>qq</p></div>',0);
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
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8 COMMENT='好友请求';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `FriendshipApply`
--

LOCK TABLES `FriendshipApply` WRITE;
/*!40000 ALTER TABLE `FriendshipApply` DISABLE KEYS */;
INSERT INTO `FriendshipApply` VALUES (1,'admin','test1','QQQQQ',0,1,0),(7,'test1','test2','',0,0,0),(8,'test4','test5','',1,1,0),(9,'hehui','admin','',1,1,1);
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
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8 COMMENT='组的成员';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `InterestGroupMembers`
--

LOCK TABLES `InterestGroupMembers` WRITE;
/*!40000 ALTER TABLE `InterestGroupMembers` DISABLE KEYS */;
INSERT INTO `InterestGroupMembers` VALUES (6,'admin',7,1,NULL),(7,'admin',8,1,NULL),(8,'admin',9,1,NULL),(9,'admin',10,1,NULL),(10,'admin',11,1,NULL);
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
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8 COMMENT='兴趣组';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `InterestGroups`
--

LOCK TABLES `InterestGroups` WRITE;
/*!40000 ALTER TABLE `InterestGroups` DISABLE KEYS */;
INSERT INTO `InterestGroups` VALUES (7,2,NULL,'admin','TestGroup1','2013-09-19 04:46:43',1,12,NULL,NULL,NULL,1,NULL,1),(8,2,NULL,'admin','Group1','2013-09-20 06:27:49',1,4,NULL,NULL,NULL,1,NULL,1),(9,2,NULL,'admin','Group2','2013-09-21 10:31:14',1,4,NULL,NULL,NULL,1,NULL,1),(10,2,NULL,'admin','Group3','2013-09-21 10:31:38',1,1,NULL,NULL,NULL,1,NULL,1),(11,2,NULL,'admin','Group4','2013-09-21 10:50:37',1,1,NULL,NULL,NULL,1,NULL,1);
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
) ENGINE=InnoDB AUTO_INCREMENT=680 DEFAULT CHARSET=utf8 COMMENT='登陆历史';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `LoginHistories`
--

LOCK TABLES `LoginHistories` WRITE;
/*!40000 ALTER TABLE `LoginHistories` DISABLE KEYS */;
INSERT INTO `LoginHistories` VALUES (326,'admin','200.200.200.17',55927,NULL,NULL,'2013-09-19 04:37:16','2013-09-19 12:41:18','PC',NULL,NULL),(327,'admin','200.200.200.17',39231,NULL,NULL,'2013-09-19 04:41:35','2013-09-19 12:42:47','PC',NULL,NULL),(328,'admin','200.200.200.17',41425,NULL,NULL,'2013-09-19 04:45:28','2013-09-19 12:47:34','PC',NULL,NULL),(329,'admin','200.200.200.17',37370,NULL,NULL,'2013-09-19 05:08:42','2013-09-19 13:10:35','PC',NULL,NULL),(330,'admin','200.200.200.17',46289,NULL,NULL,'2013-09-19 05:11:34','2013-09-19 13:12:22','PC',NULL,NULL),(331,'admin','200.200.200.17',33730,NULL,NULL,'2013-09-19 05:34:49','2013-09-19 13:35:07','PC',NULL,NULL),(332,'admin','200.200.200.17',33722,NULL,NULL,'2013-09-20 03:40:25','2013-09-20 11:40:59','PC',NULL,NULL),(333,'admin','200.200.200.17',51803,NULL,NULL,'2013-09-20 06:26:39','2013-09-20 14:44:48','PC',NULL,NULL),(334,'admin','200.200.200.17',59825,NULL,NULL,'2013-09-20 06:45:06','2013-09-20 16:15:07','PC',NULL,NULL),(335,'admin','200.200.200.17',55881,NULL,NULL,'2013-09-21 08:14:49','2013-09-21 16:16:24','PC',NULL,NULL),(336,'admin','200.200.200.17',51306,NULL,NULL,'2013-09-21 08:18:47','2013-09-21 16:19:26','PC',NULL,NULL),(337,'admin','200.200.200.17',33015,NULL,NULL,'2013-09-21 08:20:23','2013-09-21 16:22:10','PC',NULL,NULL),(338,'admin','200.200.200.17',49321,NULL,NULL,'2013-09-21 08:22:27','2013-09-21 16:25:03','PC',NULL,NULL),(339,'admin','200.200.200.17',42624,NULL,NULL,'2013-09-21 08:41:09','2013-09-21 16:47:32','PC',NULL,NULL),(340,'admin','200.200.200.17',32887,NULL,NULL,'2013-09-21 08:47:52','2013-09-21 16:49:06','PC',NULL,NULL),(341,'admin','200.200.200.17',41970,NULL,NULL,'2013-09-21 09:53:11','2013-09-21 17:59:00','PC',NULL,NULL),(342,'admin','200.200.200.17',55507,NULL,NULL,'2013-09-21 09:59:25','2013-09-21 17:59:49','PC',NULL,NULL),(343,'admin','200.200.200.17',43414,NULL,NULL,'2013-09-21 10:05:29','2013-09-21 18:05:57','PC',NULL,NULL),(344,'admin','200.200.200.17',56520,NULL,NULL,'2013-09-21 10:07:01','2013-09-21 18:08:20','PC',NULL,NULL),(345,'admin','200.200.200.17',48120,NULL,NULL,'2013-09-21 10:49:36','2013-09-21 18:54:34','PC',NULL,NULL),(346,'admin','200.200.200.17',51275,NULL,NULL,'2013-09-21 10:54:50','2013-09-21 18:57:24','PC',NULL,NULL),(347,'admin','200.200.200.17',39040,NULL,NULL,'2013-09-21 11:18:07','2013-09-21 19:19:25','PC',NULL,NULL),(348,'admin','200.200.200.17',53349,NULL,NULL,'2013-09-22 07:00:38','2013-09-22 15:09:41','PC',NULL,NULL),(349,'admin','200.200.200.17',44175,NULL,NULL,'2013-09-22 07:13:47','2013-09-22 15:19:29','PC',NULL,NULL),(350,'admin','200.200.200.17',42813,NULL,NULL,'2013-09-22 07:20:40','2013-09-22 15:21:49','PC',NULL,NULL),(351,'test4','200.200.200.17',48023,NULL,NULL,'2013-09-22 11:48:57','2013-09-22 19:48:57','PC',NULL,NULL),(352,'test4','200.200.200.17',49197,NULL,NULL,'2013-09-22 11:53:15','2013-09-22 19:53:15','PC',NULL,NULL),(353,'test4','200.200.200.17',58144,NULL,NULL,'2013-09-22 11:56:11','2013-09-22 19:57:44','PC',NULL,NULL),(354,'test4','200.200.200.17',36256,NULL,NULL,'2013-09-22 11:58:34','2013-09-22 19:59:39','PC',NULL,NULL),(355,'test4','200.200.200.17',44728,NULL,NULL,'2013-09-22 12:00:04','2013-09-22 20:00:44','PC',NULL,NULL),(356,'test4','200.200.200.17',49310,NULL,NULL,'2013-09-22 12:03:18','2013-09-22 20:03:24','PC',NULL,NULL),(357,'test4','200.200.200.17',59182,NULL,NULL,'2013-09-22 12:04:42','2013-09-22 20:04:42','PC',NULL,NULL),(358,'test4','200.200.200.17',50728,NULL,NULL,'2013-09-22 12:19:16','2013-09-22 20:19:16','PC',NULL,NULL),(359,'test4','200.200.200.17',34462,NULL,NULL,'2013-09-22 12:19:46','2013-09-22 20:20:02','PC',NULL,NULL),(360,'test4','200.200.200.17',53066,NULL,NULL,'2013-09-22 12:20:56','2013-09-22 20:22:16','PC',NULL,NULL),(361,'test4','200.200.200.17',40921,NULL,NULL,'2013-09-22 12:28:15','2013-09-22 20:29:02','PC',NULL,NULL),(362,'test4','200.200.200.17',32878,NULL,NULL,'2013-09-22 12:39:20','2013-09-22 20:40:45','PC',NULL,NULL),(363,'test4','200.200.200.17',46211,NULL,NULL,'2013-09-23 00:33:40','2013-09-23 08:34:14','PC',NULL,NULL),(364,'test4','200.200.200.17',46885,NULL,NULL,'2013-09-23 00:36:38','2013-09-23 08:40:38','PC',NULL,NULL),(365,'test4','200.200.200.17',60448,NULL,NULL,'2013-09-23 00:41:05','2013-09-23 08:42:39','PC',NULL,NULL),(366,'test4','200.200.200.17',49636,NULL,NULL,'2013-09-23 00:43:06','2013-09-23 08:44:37','PC',NULL,NULL),(367,'test4','200.200.200.17',35945,NULL,NULL,'2013-09-23 00:45:13','2013-09-23 08:45:38','PC',NULL,NULL),(368,'test4','200.200.200.17',54910,NULL,NULL,'2013-09-23 00:48:07','2013-09-23 08:48:07','PC',NULL,NULL),(369,'test4','200.200.200.17',52240,NULL,NULL,'2013-09-23 01:01:06','2013-09-23 09:10:20','PC',NULL,NULL),(370,'test4','200.200.200.17',55564,NULL,NULL,'2013-09-23 01:12:00','2013-09-23 09:12:00','PC',NULL,NULL),(371,'test4','200.200.200.17',43996,NULL,NULL,'2013-09-23 01:15:29','2013-09-23 09:21:48','PC',NULL,NULL),(372,'test4','200.200.200.17',52219,NULL,NULL,'2013-09-23 01:25:28','2013-09-23 09:26:04','PC',NULL,NULL),(373,'test4','200.200.200.17',53034,NULL,NULL,'2013-09-23 01:32:43','2013-09-23 09:33:07','PC',NULL,NULL),(374,'test4','200.200.200.17',51439,NULL,NULL,'2013-09-23 01:43:32','2013-09-23 10:19:34','PC',NULL,NULL),(375,'test4','200.200.200.17',46737,NULL,NULL,'2013-09-23 02:24:11','2013-09-23 10:24:58','PC',NULL,NULL),(376,'test4','200.200.200.17',35784,NULL,NULL,'2013-09-24 03:20:19','2013-09-24 11:20:24','PC',NULL,NULL),(377,'test4','200.200.200.17',33493,NULL,NULL,'2013-09-24 03:20:43','2013-09-24 11:21:53','PC',NULL,NULL),(378,'test4','200.200.200.17',37796,NULL,NULL,'2013-09-24 03:46:24','2013-09-24 11:47:13','PC',NULL,NULL),(379,'test4','200.200.200.17',50807,NULL,NULL,'2013-09-24 03:53:57','2013-09-24 12:01:43','PC',NULL,NULL),(380,'test4','200.200.200.17',50799,NULL,NULL,'2013-09-24 04:05:45','2013-09-24 12:05:45','PC',NULL,NULL),(381,'test4','200.200.200.17',44251,NULL,NULL,'2013-09-24 04:12:07','2013-09-24 12:13:14','PC',NULL,NULL),(382,'test4','200.200.200.17',44586,NULL,NULL,'2013-09-24 04:20:20','2013-09-24 12:21:27','PC',NULL,NULL),(383,'test4','200.200.200.17',36072,NULL,NULL,'2013-09-24 06:41:41','2013-09-24 16:17:43','PC',NULL,NULL),(384,'test4','200.200.200.17',36672,NULL,NULL,'2013-09-24 08:46:37','2013-09-24 16:46:53','PC',NULL,NULL),(385,'test4','200.200.200.17',40609,NULL,NULL,'2013-10-09 09:30:30','2013-10-09 17:31:18','PC',NULL,NULL),(386,'test4','200.200.200.17',48066,NULL,NULL,'2013-10-10 01:14:03','2013-10-10 09:34:37','PC',NULL,NULL),(387,'test4','200.200.200.17',58159,NULL,NULL,'2013-10-10 01:35:17','2013-10-10 09:36:23','PC',NULL,NULL),(388,'test4','200.200.200.17',47248,NULL,NULL,'2013-10-10 01:36:48','2013-10-10 09:38:16','PC',NULL,NULL),(389,'test4','200.200.200.17',53641,NULL,NULL,'2013-10-10 01:38:27','2013-10-10 09:50:13','PC',NULL,NULL),(390,'test4','200.200.200.17',34410,NULL,NULL,'2013-10-10 01:50:29','2013-10-10 09:54:06','PC',NULL,NULL),(391,'test4','200.200.200.17',50926,NULL,NULL,'2013-10-10 01:54:19','2013-10-10 09:54:46','PC',NULL,NULL),(392,'test4','200.200.200.17',35895,NULL,NULL,'2013-10-10 02:02:59','2013-10-10 10:02:59','PC',NULL,NULL),(393,'test4','200.200.200.17',42645,NULL,NULL,'2013-10-10 02:38:33','2013-10-10 11:25:31','PC',NULL,NULL),(394,'test4','200.200.200.17',56128,NULL,NULL,'2013-10-10 03:26:00','2013-10-10 11:31:32','PC',NULL,NULL),(395,'test4','200.200.200.17',35646,NULL,NULL,'2013-10-10 03:31:41','2013-10-10 11:45:31','PC',NULL,NULL),(396,'test4','200.200.200.17',57492,NULL,NULL,'2013-10-10 03:46:41','2013-10-10 11:48:55','PC',NULL,NULL),(397,'test4','200.200.200.17',34604,NULL,NULL,'2013-10-10 03:49:04','2013-10-10 12:28:41','PC',NULL,NULL),(398,'test4','200.200.200.17',57795,NULL,NULL,'2013-10-10 04:32:05','2013-10-10 12:35:30','PC',NULL,NULL),(399,'test4','200.200.200.17',57241,NULL,NULL,'2013-10-10 07:07:00','2013-10-10 15:09:19','PC',NULL,NULL),(400,'test4','200.200.200.17',53128,NULL,NULL,'2013-10-10 07:16:31','2013-10-10 15:17:05','PC',NULL,NULL),(401,'test4','200.200.200.17',49622,NULL,NULL,'2013-10-10 07:18:16','2013-10-10 15:22:03','PC',NULL,NULL),(402,'test4','200.200.200.17',38734,NULL,NULL,'2013-10-10 07:23:47','2013-10-10 15:26:11','PC',NULL,NULL),(403,'test4','200.200.200.17',35970,NULL,NULL,'2013-10-11 00:48:44','2013-10-11 08:51:38','PC',NULL,NULL),(404,'test4','200.200.200.17',57322,NULL,NULL,'2013-10-11 00:52:50','2013-10-11 08:52:53','PC',NULL,NULL),(405,'admin','200.200.200.17',43805,NULL,NULL,'2013-10-11 00:53:01','2013-10-11 08:54:34','PC',NULL,NULL),(406,'admin','200.200.200.17',35788,NULL,NULL,'2013-10-11 00:54:57','2013-10-11 08:55:03','PC',NULL,NULL),(407,'test4','200.200.200.17',32842,NULL,NULL,'2013-10-11 00:55:13','2013-10-11 08:59:03','PC',NULL,NULL),(408,'test4','200.200.200.17',54218,NULL,NULL,'2013-10-11 04:27:53','2013-10-11 12:28:23','PC',NULL,NULL),(409,'admin','200.200.200.17',56858,NULL,NULL,'2013-10-11 04:28:31','2013-10-11 12:28:41','PC',NULL,NULL),(410,'test4','200.200.200.17',47217,NULL,NULL,'2013-10-11 07:29:31','2013-10-11 16:08:25','PC',NULL,NULL),(411,'admin','200.200.200.17',40971,NULL,NULL,'2013-10-11 08:11:27','2013-10-11 16:12:42','PC',NULL,NULL),(412,'admin','200.200.200.17',56853,NULL,NULL,'2013-10-11 08:12:50','2013-10-11 16:18:12','PC',NULL,NULL),(413,'admin','200.200.200.17',46279,NULL,NULL,'2013-10-11 08:18:49','2013-10-11 16:41:00','PC',NULL,NULL),(414,'admin','200.200.200.17',32805,NULL,NULL,'2013-10-11 08:43:24','2013-10-11 17:49:07','PC',NULL,NULL),(415,'admin','200.200.200.17',53964,NULL,NULL,'2013-10-14 02:26:01','2013-10-14 10:26:39','PC',NULL,NULL),(416,'admin','200.200.200.17',53898,NULL,NULL,'2013-10-14 02:26:51','2013-10-14 10:28:01','PC',NULL,NULL),(417,'admin','200.200.200.17',59952,NULL,NULL,'2013-10-14 02:28:10','2013-10-14 10:28:12','PC',NULL,NULL),(418,'test4','200.200.200.17',54643,NULL,NULL,'2013-10-14 02:28:26','2013-10-14 10:29:41','PC',NULL,NULL),(419,'admin','200.200.200.17',51927,NULL,NULL,'2013-10-14 02:29:51','2013-10-14 10:39:17','PC',NULL,NULL),(420,'test4','200.200.200.17',44543,NULL,NULL,'2013-10-14 02:39:45','2013-10-14 10:40:05','PC',NULL,NULL),(421,'admin','200.200.200.17',41285,NULL,NULL,'2013-10-14 02:40:14','2013-10-14 10:40:14','PC',NULL,NULL),(422,'admin','200.200.200.17',41851,NULL,NULL,'2013-10-24 04:24:18','2013-10-24 12:24:30','PC',NULL,NULL),(423,'admin','200.200.200.17',39793,NULL,NULL,'2013-10-24 04:27:09','2013-10-24 12:27:14','PC',NULL,NULL),(424,'admin','200.200.200.17',49934,NULL,NULL,'2013-10-24 04:31:51','2013-10-24 12:31:58','PC',NULL,NULL),(425,'admin','200.200.200.17',41698,NULL,NULL,'2013-10-24 04:34:28','2013-10-24 12:34:42','PC',NULL,NULL),(426,'admin','200.200.200.17',54291,NULL,NULL,'2013-10-24 07:37:05','2013-10-24 15:37:20','PC',NULL,NULL),(427,'admin','200.200.200.17',57379,NULL,NULL,'2013-10-24 07:41:01','2013-10-24 15:41:39','PC',NULL,NULL),(428,'admin','200.200.200.17',36576,NULL,NULL,'2013-10-24 07:43:30','2013-10-24 15:47:43','PC',NULL,NULL),(429,'admin','200.200.200.17',34813,NULL,NULL,'2013-10-24 07:48:05','2013-10-24 15:48:20','PC',NULL,NULL),(430,'admin','200.200.200.17',48923,NULL,NULL,'2013-10-24 07:52:29','2013-10-24 15:53:03','PC',NULL,NULL),(431,'admin','200.200.200.17',37101,NULL,NULL,'2013-10-24 07:54:38','2013-10-24 15:54:58','PC',NULL,NULL),(432,'admin','200.200.200.17',60256,NULL,NULL,'2013-10-24 07:55:12','2013-10-24 15:55:16','PC',NULL,NULL),(433,'admin','200.200.200.17',33460,NULL,NULL,'2013-10-24 07:57:31','2013-10-24 15:58:27','PC',NULL,NULL),(434,'admin','200.200.200.17',49903,NULL,NULL,'2013-10-24 08:02:16','2013-10-24 16:02:42','PC',NULL,NULL),(435,'admin','200.200.200.17',54243,NULL,NULL,'2013-10-24 08:05:07','2013-10-24 16:06:19','PC',NULL,NULL),(436,'admin','200.200.200.17',35133,NULL,NULL,'2013-10-28 11:11:29','2013-10-28 19:13:06','PC',NULL,NULL),(437,'admin','200.200.200.17',42584,NULL,NULL,'2013-10-28 11:15:40','2013-10-28 19:17:51','PC',NULL,NULL),(438,'admin','200.200.200.17',41578,NULL,NULL,'2013-10-28 11:32:43','2013-10-28 19:34:59','PC',NULL,NULL),(439,'admin','200.200.200.17',32881,NULL,NULL,'2013-10-28 11:37:10','2013-10-28 19:38:15','PC',NULL,NULL),(440,'admin','200.200.200.17',56413,NULL,NULL,'2013-10-28 11:38:31','2013-10-28 19:39:18','PC',NULL,NULL),(441,'admin','200.200.200.17',46520,NULL,NULL,'2013-10-28 11:40:56','2013-10-28 19:41:01','PC',NULL,NULL),(442,'admin','200.200.200.17',33724,NULL,NULL,'2013-10-28 11:46:09','2013-10-28 19:47:17','PC',NULL,NULL),(443,'admin','200.200.200.17',44350,NULL,NULL,'2013-10-28 11:47:53','2013-10-28 19:48:23','PC',NULL,NULL),(444,'admin','200.200.200.17',47180,NULL,NULL,'2013-10-29 03:24:33','2013-10-29 11:26:39','PC',NULL,NULL),(445,'admin','200.200.200.17',55268,NULL,NULL,'2013-10-29 06:43:55','2013-10-29 14:45:25','PC',NULL,NULL),(446,'admin','200.200.200.17',45382,NULL,NULL,'2013-10-29 07:10:18','2013-10-29 15:11:10','PC',NULL,NULL),(447,'admin','200.200.200.17',39099,NULL,NULL,'2013-10-29 07:56:17','2013-10-29 15:56:35','PC',NULL,NULL),(448,'admin','200.200.200.17',41377,NULL,NULL,'2013-10-29 07:57:32','2013-10-29 15:58:10','PC',NULL,NULL),(449,'admin','200.200.200.17',36121,NULL,NULL,'2013-10-29 08:00:53','2013-10-29 16:01:09','PC',NULL,NULL),(450,'admin','200.200.200.17',42256,NULL,NULL,'2013-10-29 08:11:03','2013-10-29 16:13:09','PC',NULL,NULL),(451,'admin','200.200.200.17',53563,NULL,NULL,'2013-10-29 08:13:58','2013-10-29 16:19:04','PC',NULL,NULL),(452,'admin','200.200.200.17',40383,NULL,NULL,'2013-10-29 08:19:33','2013-10-29 16:19:49','PC',NULL,NULL),(453,'admin','200.200.200.17',53542,NULL,NULL,'2013-10-29 08:20:05','2013-10-29 16:20:38','PC',NULL,NULL),(454,'admin','200.200.200.17',40352,NULL,NULL,'2013-10-29 08:32:25','2013-10-29 16:32:59','PC',NULL,NULL),(455,'admin','200.200.200.17',59216,NULL,NULL,'2013-10-29 08:35:49','2013-10-29 16:36:13','PC',NULL,NULL),(456,'admin','200.200.200.17',40326,NULL,NULL,'2013-10-29 08:38:00','2013-10-29 16:38:40','PC',NULL,NULL),(457,'admin','200.200.200.17',48251,NULL,NULL,'2013-10-29 08:39:07','2013-10-29 16:39:21','PC',NULL,NULL),(458,'admin','200.200.200.17',50174,NULL,NULL,'2013-10-29 08:49:58','2013-10-29 16:50:46','PC',NULL,NULL),(459,'admin','200.200.200.17',49211,NULL,NULL,'2013-10-29 08:51:24','2013-10-29 16:52:13','PC',NULL,NULL),(460,'admin','200.200.200.17',38971,NULL,NULL,'2013-10-29 08:58:21','2013-10-29 16:58:21','PC',NULL,NULL),(461,'admin','200.200.200.17',34282,NULL,NULL,'2013-10-29 08:58:58','2013-10-29 16:59:50','PC',NULL,NULL),(462,'admin','200.200.200.17',50497,NULL,NULL,'2013-10-29 09:04:09','2013-10-29 17:04:37','PC',NULL,NULL),(463,'admin','200.200.200.17',38142,NULL,NULL,'2013-10-29 09:10:20','2013-10-29 17:10:35','PC',NULL,NULL),(464,'admin','200.200.200.17',58064,NULL,NULL,'2013-10-29 09:11:19','2013-10-29 17:11:51','PC',NULL,NULL),(465,'admin','200.200.200.17',60475,NULL,NULL,'2013-10-29 09:12:46','2013-10-29 17:12:53','PC',NULL,NULL),(466,'admin','200.200.200.17',59757,NULL,NULL,'2013-10-29 09:21:07','2013-10-29 17:21:30','PC',NULL,NULL),(467,'admin','200.200.200.17',58238,NULL,NULL,'2013-10-29 09:25:30','2013-10-29 17:26:27','PC',NULL,NULL),(468,'admin','200.200.200.17',49027,NULL,NULL,'2013-10-29 09:27:08','2013-10-29 17:27:25','PC',NULL,NULL),(469,'admin','200.200.200.17',60507,NULL,NULL,'2013-10-29 09:28:58','2013-10-29 17:29:13','PC',NULL,NULL),(470,'admin','200.200.200.17',53882,NULL,NULL,'2013-10-29 09:30:23','2013-10-29 17:30:32','PC',NULL,NULL),(471,'admin','200.200.200.17',43712,NULL,NULL,'2013-10-29 09:31:50','2013-10-29 17:32:16','PC',NULL,NULL),(472,'admin','200.200.200.17',37047,NULL,NULL,'2013-10-29 09:33:45','2013-10-29 17:34:09','PC',NULL,NULL),(473,'admin','200.200.200.17',51467,NULL,NULL,'2013-10-29 09:35:15','2013-10-29 17:35:31','PC',NULL,NULL),(474,'admin','200.200.200.17',53403,NULL,NULL,'2013-10-29 09:38:37','2013-10-29 17:38:54','PC',NULL,NULL),(475,'admin','200.200.200.17',49625,NULL,NULL,'2013-10-29 09:44:47','2013-10-29 17:45:14','PC',NULL,NULL),(476,'admin','200.200.200.17',34854,NULL,NULL,'2013-10-29 09:46:21','2013-10-29 17:46:53','PC',NULL,NULL),(477,'admin','200.200.200.17',43472,NULL,NULL,'2013-10-29 09:48:32','2013-10-29 17:49:07','PC',NULL,NULL),(478,'admin','200.200.200.17',55685,NULL,NULL,'2013-10-29 09:50:50','2013-10-29 17:51:10','PC',NULL,NULL),(479,'admin','200.200.200.17',44006,NULL,NULL,'2013-10-29 09:51:39','2013-10-29 17:52:01','PC',NULL,NULL),(480,'admin','200.200.200.17',52951,NULL,NULL,'2013-10-29 09:52:30','2013-10-29 17:52:42','PC',NULL,NULL),(481,'admin','200.200.200.17',55939,NULL,NULL,'2013-10-29 09:53:14','2013-10-29 17:53:35','PC',NULL,NULL),(482,'admin','200.200.200.17',43532,NULL,NULL,'2013-10-29 09:56:08','2013-10-29 17:56:24','PC',NULL,NULL),(483,'admin','200.200.200.17',50292,NULL,NULL,'2013-10-29 09:58:25','2013-10-29 17:58:45','PC',NULL,NULL),(484,'admin','200.200.200.17',36832,NULL,NULL,'2013-10-29 09:59:37','2013-10-29 18:00:14','PC',NULL,NULL),(485,'admin','200.200.200.17',41650,NULL,NULL,'2013-10-29 10:00:32','2013-10-29 18:00:38','PC',NULL,NULL),(486,'admin','200.200.200.17',34453,NULL,NULL,'2013-10-29 10:01:01','2013-10-29 18:01:06','PC',NULL,NULL),(487,'admin','200.200.200.17',41386,NULL,NULL,'2013-10-29 10:01:38','2013-10-29 18:01:51','PC',NULL,NULL),(488,'admin','200.200.200.17',50968,NULL,NULL,'2013-10-29 10:02:29','2013-10-29 18:02:47','PC',NULL,NULL),(489,'admin','200.200.200.17',34210,NULL,NULL,'2013-10-29 10:03:17','2013-10-29 18:03:27','PC',NULL,NULL),(490,'admin','200.200.200.17',60163,NULL,NULL,'2013-10-29 10:06:06','2013-10-29 18:06:26','PC',NULL,NULL),(491,'admin','200.200.200.17',58515,NULL,NULL,'2013-10-29 10:07:41','2013-10-29 18:08:52','PC',NULL,NULL),(492,'admin','200.200.200.17',54038,NULL,NULL,'2013-10-29 10:09:15','2013-10-29 18:09:54','PC',NULL,NULL),(493,'admin','200.200.200.17',35496,NULL,NULL,'2013-10-29 10:29:31','2013-10-29 18:29:37','PC',NULL,NULL),(494,'admin','200.200.200.17',38400,NULL,NULL,'2013-10-29 10:30:04','2013-10-29 18:30:40','PC',NULL,NULL),(495,'admin','200.200.200.17',47151,NULL,NULL,'2013-10-29 10:31:30','2013-10-29 18:31:58','PC',NULL,NULL),(496,'admin','200.200.200.17',58899,NULL,NULL,'2013-10-29 10:34:44','2013-10-29 18:35:10','PC',NULL,NULL),(497,'admin','200.200.200.17',42205,NULL,NULL,'2013-10-29 10:36:05','2013-10-29 18:36:53','PC',NULL,NULL),(498,'admin','200.200.200.17',41177,NULL,NULL,'2013-10-29 10:37:50','2013-10-29 18:39:20','PC',NULL,NULL),(499,'admin','200.200.200.17',58796,NULL,NULL,'2013-10-29 10:40:02','2013-10-29 18:40:40','PC',NULL,NULL),(500,'admin','200.200.200.17',59924,NULL,NULL,'2013-10-29 10:40:48','2013-10-29 18:42:33','PC',NULL,NULL),(501,'admin','200.200.200.17',49015,NULL,NULL,'2013-10-29 10:45:56','2013-10-29 18:46:59','PC',NULL,NULL),(502,'admin','200.200.200.17',34738,NULL,NULL,'2013-10-29 10:48:01','2013-10-29 18:48:30','PC',NULL,NULL),(503,'admin','200.200.200.17',59854,NULL,NULL,'2013-10-29 10:48:55','2013-10-29 18:49:17','PC',NULL,NULL),(504,'admin','200.200.200.17',48653,NULL,NULL,'2013-10-29 10:50:47','2013-10-29 18:51:14','PC',NULL,NULL),(505,'admin','200.200.200.17',43718,NULL,NULL,'2013-10-29 10:52:09','2013-10-29 18:52:16','PC',NULL,NULL),(506,'admin','200.200.200.17',45067,NULL,NULL,'2013-10-29 10:53:18','2013-10-29 18:53:36','PC',NULL,NULL),(507,'admin','200.200.200.17',44496,NULL,NULL,'2013-10-29 10:55:19','2013-10-29 18:55:27','PC',NULL,NULL),(508,'admin','200.200.200.17',54424,NULL,NULL,'2013-10-29 10:56:24','2013-10-29 18:56:35','PC',NULL,NULL),(509,'admin','200.200.200.17',47176,NULL,NULL,'2013-10-29 10:57:27','2013-10-29 18:57:36','PC',NULL,NULL),(510,'admin','200.200.200.17',37561,NULL,NULL,'2013-10-29 11:01:12','2013-10-29 19:01:28','PC',NULL,NULL),(511,'admin','200.200.200.17',57395,NULL,NULL,'2013-10-29 11:02:08','2013-10-29 19:02:49','PC',NULL,NULL),(512,'admin','200.200.200.17',57751,NULL,NULL,'2013-10-29 11:05:04','2013-10-29 19:05:34','PC',NULL,NULL),(513,'admin','200.200.200.17',37018,NULL,NULL,'2013-10-29 11:05:41','2013-10-29 19:06:13','PC',NULL,NULL),(514,'admin','200.200.200.17',48928,NULL,NULL,'2013-10-29 11:06:34','2013-10-29 19:10:27','PC',NULL,NULL),(515,'admin','200.200.200.17',41830,NULL,NULL,'2013-10-29 11:10:52','2013-10-29 19:11:40','PC',NULL,NULL),(516,'admin','200.200.200.17',60296,NULL,NULL,'2013-10-29 11:12:33','2013-10-29 19:13:31','PC',NULL,NULL),(517,'admin','200.200.200.17',51188,NULL,NULL,'2013-10-29 11:14:50','2013-10-29 19:15:17','PC',NULL,NULL),(518,'admin','200.200.200.17',48732,NULL,NULL,'2013-10-29 11:16:06','2013-10-29 19:16:42','PC',NULL,NULL),(519,'admin','200.200.200.17',49616,NULL,NULL,'2013-10-29 11:17:19','2013-10-29 19:17:46','PC',NULL,NULL),(520,'admin','200.200.200.17',57815,NULL,NULL,'2013-10-29 11:18:34','2013-10-29 19:18:50','PC',NULL,NULL),(521,'admin','200.200.200.17',54706,NULL,NULL,'2013-10-29 11:19:51','2013-10-29 19:20:54','PC',NULL,NULL),(522,'admin','200.200.200.17',59376,NULL,NULL,'2013-10-29 11:34:12','2013-10-29 19:34:39','PC',NULL,NULL),(523,'admin','200.200.200.17',32993,NULL,NULL,'2013-10-30 04:18:43','2013-10-30 12:19:01','PC',NULL,NULL),(524,'admin','200.200.200.17',52256,NULL,NULL,'2013-10-30 04:19:27','2013-10-30 12:19:36','PC',NULL,NULL),(525,'admin','200.200.200.17',56941,NULL,NULL,'2013-10-30 04:20:21','2013-10-30 12:20:32','PC',NULL,NULL),(526,'admin','200.200.200.17',40020,NULL,NULL,'2013-10-30 04:21:01','2013-10-30 12:21:10','PC',NULL,NULL),(527,'admin','200.200.200.17',46161,NULL,NULL,'2013-10-30 04:23:05','2013-10-30 12:23:17','PC',NULL,NULL),(528,'admin','200.200.200.17',44590,NULL,NULL,'2013-10-30 04:23:31','2013-10-30 12:23:48','PC',NULL,NULL),(529,'admin','200.200.200.17',49618,NULL,NULL,'2013-10-30 04:27:46','2013-10-30 12:28:10','PC',NULL,NULL),(530,'admin','200.200.200.17',35729,NULL,NULL,'2013-10-30 04:28:36','2013-10-30 12:28:45','PC',NULL,NULL),(531,'admin','200.200.200.17',43565,NULL,NULL,'2013-10-30 04:29:59','2013-10-30 12:30:08','PC',NULL,NULL),(532,'admin','200.200.200.17',58290,NULL,NULL,'2013-10-30 04:31:56','2013-10-30 12:32:11','PC',NULL,NULL),(533,'admin','200.200.200.17',49980,NULL,NULL,'2013-10-30 04:33:49','2013-10-30 12:33:58','PC',NULL,NULL),(534,'admin','200.200.200.17',37282,NULL,NULL,'2013-10-30 04:36:11','2013-10-30 12:36:23','PC',NULL,NULL),(535,'admin','200.200.200.17',38653,NULL,NULL,'2013-10-30 04:36:48','2013-10-30 12:36:58','PC',NULL,NULL),(536,'admin','200.200.200.17',54233,NULL,NULL,'2013-10-30 04:37:50','2013-10-30 12:38:00','PC',NULL,NULL),(537,'admin','200.200.200.17',47038,NULL,NULL,'2013-10-30 04:38:17','2013-10-30 12:38:31','PC',NULL,NULL),(538,'admin','200.200.200.17',57196,NULL,NULL,'2013-10-30 04:39:36','2013-10-30 12:39:42','PC',NULL,NULL),(539,'admin','200.200.200.17',33712,NULL,NULL,'2013-10-30 04:40:28','2013-10-30 12:40:32','PC',NULL,NULL),(540,'admin','200.200.200.17',56549,NULL,NULL,'2013-10-30 04:40:42','2013-10-30 12:40:51','PC',NULL,NULL),(541,'admin','200.200.200.17',53770,NULL,NULL,'2013-10-30 06:02:17','2013-10-30 14:02:50','PC',NULL,NULL),(542,'admin','200.200.200.17',33416,NULL,NULL,'2013-10-30 06:03:28','2013-10-30 14:03:44','PC',NULL,NULL),(543,'admin','200.200.200.17',55564,NULL,NULL,'2013-10-30 06:04:03','2013-10-30 14:04:11','PC',NULL,NULL),(544,'admin','200.200.200.17',47570,NULL,NULL,'2013-10-30 06:04:26','2013-10-30 14:04:30','PC',NULL,NULL),(545,'admin','200.200.200.17',49026,NULL,NULL,'2013-10-30 06:05:24','2013-10-30 14:05:34','PC',NULL,NULL),(546,'admin','200.200.200.17',43283,NULL,NULL,'2013-10-30 06:09:03','2013-10-30 14:09:10','PC',NULL,NULL),(547,'admin','200.200.200.17',60267,NULL,NULL,'2013-10-30 06:09:31','2013-10-30 14:09:45','PC',NULL,NULL),(548,'admin','200.200.200.17',33263,NULL,NULL,'2013-10-30 06:14:16','2013-10-30 14:14:38','PC',NULL,NULL),(549,'admin','200.200.200.17',40133,NULL,NULL,'2013-10-30 06:15:40','2013-10-30 14:15:46','PC',NULL,NULL),(550,'admin','200.200.200.17',43571,NULL,NULL,'2013-10-30 06:22:41','2013-10-30 14:22:53','PC',NULL,NULL),(551,'admin','200.200.200.17',33384,NULL,NULL,'2013-10-30 06:26:21','2013-10-30 14:30:14','PC',NULL,NULL),(552,'admin','200.200.200.17',57665,NULL,NULL,'2013-10-30 06:38:35','2013-10-30 14:39:17','PC',NULL,NULL),(553,'admin','200.200.200.17',58900,NULL,NULL,'2013-10-30 06:41:08','2013-10-30 14:41:47','PC',NULL,NULL),(554,'admin','200.200.200.17',57775,NULL,NULL,'2013-10-30 06:43:51','2013-10-30 14:44:09','PC',NULL,NULL),(555,'admin','200.200.200.17',52654,NULL,NULL,'2013-10-30 07:03:20','2013-10-30 15:03:51','PC',NULL,NULL),(556,'admin','200.200.200.17',33013,NULL,NULL,'2013-10-30 07:11:18','2013-10-30 15:11:43','PC',NULL,NULL),(557,'admin','200.200.200.17',44093,NULL,NULL,'2013-10-30 07:29:52','2013-10-30 15:30:10','PC',NULL,NULL),(558,'admin','200.200.200.17',43259,NULL,NULL,'2013-10-30 07:31:46','2013-10-30 15:32:13','PC',NULL,NULL),(559,'admin','200.200.200.17',50501,NULL,NULL,'2013-10-30 07:32:43','2013-10-30 15:32:54','PC',NULL,NULL),(560,'admin','200.200.200.17',34725,NULL,NULL,'2013-10-30 07:37:59','2013-10-30 15:38:13','PC',NULL,NULL),(561,'admin','200.200.200.17',39444,NULL,NULL,'2013-10-30 07:38:47','2013-10-30 15:40:02','PC',NULL,NULL),(562,'admin','200.200.200.17',57141,NULL,NULL,'2013-10-30 07:42:33','2013-10-30 15:42:42','PC',NULL,NULL),(563,'admin','200.200.200.17',46264,NULL,NULL,'2013-10-30 07:48:21','2013-10-30 15:48:41','PC',NULL,NULL),(564,'admin','200.200.200.17',52958,NULL,NULL,'2013-10-30 07:50:28','2013-10-30 15:51:18','PC',NULL,NULL),(565,'admin','200.200.200.17',44784,NULL,NULL,'2013-10-30 07:57:52','2013-10-30 15:59:12','PC',NULL,NULL),(566,'admin','200.200.200.17',44864,NULL,NULL,'2013-10-30 08:00:10','2013-10-30 16:00:24','PC',NULL,NULL),(567,'admin','200.200.200.17',36251,NULL,NULL,'2013-10-30 08:04:51','2013-10-30 16:05:24','PC',NULL,NULL),(568,'admin','200.200.200.17',41416,NULL,NULL,'2013-10-30 08:06:01','2013-10-30 16:06:26','PC',NULL,NULL),(569,'admin','200.200.200.17',46981,NULL,NULL,'2013-10-30 08:08:46','2013-10-30 16:08:55','PC',NULL,NULL),(570,'admin','200.200.200.17',60409,NULL,NULL,'2013-10-30 08:09:18','2013-10-30 16:09:35','PC',NULL,NULL),(571,'admin','200.200.200.17',54693,NULL,NULL,'2013-10-30 08:12:48','2013-10-30 16:13:16','PC',NULL,NULL),(572,'admin','200.200.200.17',40204,NULL,NULL,'2013-10-30 08:14:34','2013-10-30 16:15:10','PC',NULL,NULL),(573,'admin','200.200.200.17',42934,NULL,NULL,'2013-10-30 08:16:32','2013-10-30 16:16:52','PC',NULL,NULL),(574,'admin','200.200.200.17',53310,NULL,NULL,'2013-10-30 08:17:47','2013-10-30 16:18:17','PC',NULL,NULL),(575,'admin','200.200.200.17',59678,NULL,NULL,'2013-10-30 08:20:32','2013-10-30 16:22:06','PC',NULL,NULL),(576,'admin','200.200.200.17',41135,NULL,NULL,'2013-10-30 08:30:13','2013-10-30 16:30:37','PC',NULL,NULL),(577,'admin','200.200.200.17',36401,NULL,NULL,'2013-10-30 08:33:32','2013-10-30 16:35:44','PC',NULL,NULL),(578,'admin','200.200.200.17',58442,NULL,NULL,'2013-10-30 08:36:17','2013-10-30 16:36:30','PC',NULL,NULL),(579,'admin','200.200.200.17',44156,NULL,NULL,'2013-10-30 08:38:07','2013-10-30 16:38:33','PC',NULL,NULL),(580,'admin','200.200.200.17',48203,NULL,NULL,'2013-10-30 08:41:04','2013-10-30 16:41:13','PC',NULL,NULL),(581,'admin','200.200.200.17',41322,NULL,NULL,'2013-10-30 08:41:42','2013-10-30 16:42:12','PC',NULL,NULL),(582,'admin','200.200.200.17',40248,NULL,NULL,'2013-10-30 08:43:22','2013-10-30 16:44:25','PC',NULL,NULL),(583,'admin','200.200.200.17',32986,NULL,NULL,'2013-10-30 08:45:01','2013-10-30 16:45:19','PC',NULL,NULL),(584,'admin','200.200.200.17',45550,NULL,NULL,'2013-10-30 08:46:54','2013-10-30 16:48:19','PC',NULL,NULL),(585,'admin','200.200.200.17',59505,NULL,NULL,'2013-10-30 08:52:21','2013-10-30 16:53:05','PC',NULL,NULL),(586,'admin','200.200.200.17',42601,NULL,NULL,'2013-10-30 08:58:09','2013-10-30 16:58:31','PC',NULL,NULL),(587,'admin','200.200.200.17',41352,NULL,NULL,'2013-10-30 08:59:03','2013-10-30 16:59:41','PC',NULL,NULL),(588,'admin','200.200.200.17',51511,NULL,NULL,'2013-10-30 09:00:29','2013-10-30 17:00:49','PC',NULL,NULL),(589,'admin','200.200.200.17',35981,NULL,NULL,'2013-10-30 09:02:41','2013-10-30 17:04:15','PC',NULL,NULL),(590,'admin','200.200.200.17',49205,NULL,NULL,'2013-10-30 09:05:07','2013-10-30 17:05:41','PC',NULL,NULL),(591,'admin','200.200.200.17',46186,NULL,NULL,'2013-10-30 09:07:40','2013-10-30 17:08:10','PC',NULL,NULL),(592,'admin','200.200.200.17',35458,NULL,NULL,'2013-10-30 09:12:02','2013-10-30 17:18:52','PC',NULL,NULL),(593,'admin','200.200.200.17',37042,NULL,NULL,'2013-10-30 09:18:59','2013-10-30 17:19:46','PC',NULL,NULL),(594,'admin','200.200.200.17',42760,NULL,NULL,'2013-10-30 09:23:10','2013-10-30 17:23:39','PC',NULL,NULL),(595,'admin','200.200.200.17',60837,NULL,NULL,'2013-10-30 09:25:48','2013-10-30 17:26:17','PC',NULL,NULL),(596,'admin','200.200.200.17',33532,NULL,NULL,'2013-10-30 09:28:36','2013-10-30 17:30:03','PC',NULL,NULL),(597,'admin','200.200.200.17',43155,NULL,NULL,'2013-10-30 09:30:29','2013-10-30 17:30:43','PC',NULL,NULL),(598,'admin','200.200.200.17',44786,NULL,NULL,'2013-10-30 09:31:23','2013-10-30 17:33:01','PC',NULL,NULL),(599,'admin','200.200.200.17',56832,NULL,NULL,'2013-10-30 09:33:08','2013-10-30 17:33:57','PC',NULL,NULL),(600,'admin','200.200.200.17',51610,NULL,NULL,'2013-10-30 09:39:40','2013-10-30 17:40:26','PC',NULL,NULL),(601,'admin','200.200.200.17',38447,NULL,NULL,'2013-10-30 09:53:19','2013-10-30 17:53:41','PC',NULL,NULL),(602,'admin','200.200.200.17',51976,NULL,NULL,'2013-10-30 09:58:45','2013-10-30 17:59:17','PC',NULL,NULL),(603,'admin','200.200.200.17',38780,NULL,NULL,'2013-10-30 10:02:08','2013-10-30 18:02:39','PC',NULL,NULL),(604,'admin','200.200.200.17',47799,NULL,NULL,'2013-10-30 10:03:11','2013-10-30 18:03:40','PC',NULL,NULL),(605,'admin','200.200.200.17',41572,NULL,NULL,'2013-10-30 10:04:31','2013-10-30 18:05:08','PC',NULL,NULL),(606,'admin','200.200.200.17',35920,NULL,NULL,'2013-10-31 03:29:26','2013-10-31 11:29:52','PC',NULL,NULL),(607,'hehui','200.200.200.17',33036,NULL,NULL,'2013-10-31 03:30:02','2013-10-31 11:30:39','PC',NULL,NULL),(608,'admin','200.200.200.17',59285,NULL,NULL,'2013-10-31 03:32:03','2013-10-31 11:32:48','PC',NULL,NULL),(609,'admin','200.200.200.17',43568,NULL,NULL,'2013-10-31 04:24:15','2013-10-31 12:26:28','PC',NULL,NULL),(610,'admin','200.200.200.17',44383,NULL,NULL,'2013-10-31 04:26:35','2013-10-31 12:28:02','PC',NULL,NULL),(611,'admin','200.200.200.17',36252,NULL,NULL,'2013-10-31 06:15:58','2013-10-31 14:16:15','PC',NULL,NULL),(612,'admin','200.200.200.17',42431,NULL,NULL,'2013-10-31 06:18:06','2013-10-31 14:18:48','PC',NULL,NULL),(613,'admin','200.200.200.17',58955,NULL,NULL,'2013-10-31 06:19:25','2013-10-31 14:19:31','PC',NULL,NULL),(614,'admin','200.200.200.17',50412,NULL,NULL,'2013-10-31 06:25:20','2013-10-31 14:25:54','PC',NULL,NULL),(615,'admin','200.200.200.17',50239,NULL,NULL,'2013-10-31 06:50:53','2013-10-31 14:51:59','PC',NULL,NULL),(616,'admin','200.200.200.17',58278,NULL,NULL,'2013-10-31 06:52:19','2013-10-31 14:52:26','PC',NULL,NULL),(617,'admin','200.200.200.17',53328,NULL,NULL,'2013-10-31 06:53:24','2013-10-31 14:53:34','PC',NULL,NULL),(618,'admin','200.200.200.17',48368,NULL,NULL,'2013-10-31 06:54:00','2013-10-31 14:54:00','PC',NULL,NULL),(619,'admin','200.200.200.17',56451,NULL,NULL,'2013-10-31 06:54:10','2013-10-31 14:54:48','PC',NULL,NULL),(620,'admin','200.200.200.17',60919,NULL,NULL,'2013-10-31 06:59:15','2013-10-31 14:59:49','PC',NULL,NULL),(621,'admin','200.200.200.17',50442,NULL,NULL,'2013-10-31 07:00:23','2013-10-31 15:01:36','PC',NULL,NULL),(622,'admin','200.200.200.17',43002,NULL,NULL,'2013-10-31 07:24:10','2013-10-31 15:24:31','PC',NULL,NULL),(623,'admin','200.200.200.17',38329,NULL,NULL,'2013-10-31 08:21:03','2013-10-31 16:21:29','PC',NULL,NULL),(624,'hehui','200.200.200.17',60132,NULL,NULL,'2013-10-31 08:21:41','2013-10-31 16:22:24','PC',NULL,NULL),(625,'admin','200.200.200.17',58592,NULL,NULL,'2013-10-31 08:22:32','2013-10-31 16:23:19','PC',NULL,NULL),(626,'hehui','200.200.200.17',49497,NULL,NULL,'2013-10-31 08:29:04','2013-10-31 16:29:13','PC',NULL,NULL),(627,'hehui','200.200.200.17',60692,NULL,NULL,'2013-10-31 08:29:23','2013-10-31 16:30:01','PC',NULL,NULL),(628,'admin','200.200.200.17',57794,NULL,NULL,'2013-10-31 08:36:56','2013-10-31 16:38:35','PC',NULL,NULL),(629,'admin','200.200.200.17',57135,NULL,NULL,'2013-10-31 10:54:55','2013-10-31 18:56:03','PC',NULL,NULL),(630,'hehui','200.200.200.17',38092,NULL,NULL,'2013-10-31 10:56:13','2013-10-31 18:56:35','PC',NULL,NULL),(631,'admin','200.200.200.17',39116,NULL,NULL,'2013-10-31 10:56:43','2013-10-31 18:57:17','PC',NULL,NULL),(632,'admin','200.200.200.17',47557,NULL,NULL,'2013-10-31 10:58:32','2013-10-31 18:58:59','PC',NULL,NULL),(633,'admin','200.200.200.17',38307,NULL,NULL,'2013-10-31 11:00:27','2013-10-31 19:00:54','PC',NULL,NULL),(634,'admin','200.200.200.17',55672,NULL,NULL,'2013-10-31 11:02:27','2013-10-31 19:03:09','PC',NULL,NULL),(635,'admin','200.200.200.17',45486,NULL,NULL,'2013-10-31 11:06:34','2013-10-31 19:07:00','PC',NULL,NULL),(636,'admin','200.200.200.17',41366,NULL,NULL,'2013-10-31 11:09:55','2013-10-31 19:10:18','PC',NULL,NULL),(637,'admin','200.200.200.17',53054,NULL,NULL,'2013-10-31 11:11:08','2013-10-31 19:11:30','PC',NULL,NULL),(638,'admin','200.200.200.17',41701,NULL,NULL,'2013-10-31 11:15:04','2013-10-31 19:15:40','PC',NULL,NULL),(639,'admin','200.200.200.17',55694,NULL,NULL,'2013-10-31 11:16:57','2013-10-31 19:18:20','PC',NULL,NULL),(640,'admin','200.200.200.17',47382,NULL,NULL,'2013-10-31 11:18:58','2013-10-31 19:19:56','PC',NULL,NULL),(641,'admin','200.200.200.17',38008,NULL,NULL,'2013-10-31 11:21:22','2013-10-31 19:21:22','PC',NULL,NULL),(642,'admin','200.200.200.17',40175,NULL,NULL,'2013-10-31 11:22:34','2013-10-31 19:23:17','PC',NULL,NULL),(643,'admin','200.200.200.17',44819,NULL,NULL,'2013-10-31 11:26:34','2013-10-31 19:27:31','PC',NULL,NULL),(644,'admin','200.200.200.17',34331,NULL,NULL,'2013-10-31 11:28:26','2013-10-31 19:29:19','PC',NULL,NULL),(645,'admin','200.200.200.17',51581,NULL,NULL,'2013-10-31 11:30:12','2013-10-31 19:30:41','PC',NULL,NULL),(646,'admin','200.200.200.17',59150,NULL,NULL,'2013-10-31 11:30:57','2013-10-31 19:31:19','PC',NULL,NULL),(647,'admin','200.200.200.17',46839,NULL,NULL,'2013-10-31 11:32:39','2013-10-31 19:33:18','PC',NULL,NULL),(648,'admin','200.200.200.17',46504,NULL,NULL,'2013-10-31 11:35:07','2013-10-31 19:35:35','PC',NULL,NULL),(649,'admin','200.200.200.17',36450,NULL,NULL,'2013-10-31 11:44:44','2013-10-31 19:44:58','PC',NULL,NULL),(650,'admin','200.200.200.17',44096,NULL,NULL,'2013-10-31 11:46:41','2013-10-31 19:48:20','PC',NULL,NULL),(651,'admin','200.200.200.17',46035,NULL,NULL,'2013-10-31 11:50:44','2013-10-31 19:51:03','PC',NULL,NULL),(652,'admin','200.200.200.17',50845,NULL,NULL,'2013-10-31 11:54:05','2013-10-31 19:54:18','PC',NULL,NULL),(653,'hehui','200.200.200.17',53320,NULL,NULL,'2013-10-31 11:54:28','2013-10-31 19:56:17','PC',NULL,NULL),(654,'hehui','200.200.200.17',40176,NULL,NULL,'2013-10-31 11:57:30','2013-10-31 19:57:39','PC',NULL,NULL),(655,'admin','200.200.200.17',48449,NULL,NULL,'2013-10-31 11:57:48','2013-10-31 19:58:02','PC',NULL,NULL),(656,'admin','200.200.200.17',42386,NULL,NULL,'2013-10-31 11:58:39','2013-10-31 19:58:48','PC',NULL,NULL),(657,'hehui','200.200.200.17',44710,NULL,NULL,'2013-10-31 11:58:58','2013-10-31 19:59:24','PC',NULL,NULL),(658,'hehui','200.200.200.17',51752,NULL,NULL,'2013-10-31 12:08:45','2013-10-31 20:09:49','PC',NULL,NULL),(659,'hehui','200.200.200.17',60982,NULL,NULL,'2013-10-31 12:14:39','2013-10-31 20:16:21','PC',NULL,NULL),(660,'hehui','200.200.200.17',37013,NULL,NULL,'2013-10-31 12:26:08','2013-10-31 20:26:55','PC',NULL,NULL),(661,'hehui','200.200.200.17',56735,NULL,NULL,'2013-10-31 12:31:08','2013-10-31 20:31:36','PC',NULL,NULL),(662,'hehui','200.200.200.17',52179,NULL,NULL,'2013-10-31 12:39:14','2013-10-31 20:39:49','PC',NULL,NULL),(663,'hehui','200.200.200.17',42801,NULL,NULL,'2013-10-31 12:40:02','2013-10-31 20:42:54','PC',NULL,NULL),(664,'hehui','200.200.200.17',60305,NULL,NULL,'2013-10-31 12:43:03','2013-10-31 20:44:15','PC',NULL,NULL),(665,'hehui','200.200.200.17',36566,NULL,NULL,'2013-10-31 12:45:27','2013-10-31 20:46:14','PC',NULL,NULL),(666,'hehui','200.200.200.17',58115,NULL,NULL,'2013-10-31 12:46:44','2013-10-31 20:46:59','PC',NULL,NULL),(667,'hehui','200.200.200.17',38879,NULL,NULL,'2013-10-31 12:47:56','2013-10-31 20:48:21','PC',NULL,NULL),(668,'hehui','200.200.200.17',56227,NULL,NULL,'2013-10-31 12:49:24','2013-10-31 20:49:49','PC',NULL,NULL),(669,'hehui','200.200.200.17',50675,NULL,NULL,'2013-10-31 12:57:21','2013-10-31 20:57:33','PC',NULL,NULL),(670,'admin','200.200.200.17',35917,NULL,NULL,'2013-10-31 12:57:43','2013-10-31 20:58:12','PC',NULL,NULL),(671,'admin','200.200.200.17',52169,NULL,NULL,'2013-10-31 13:00:26','2013-10-31 21:00:48','PC',NULL,NULL),(672,'admin','200.200.200.17',55113,NULL,NULL,'2013-10-31 13:02:15','2013-10-31 21:02:24','PC',NULL,NULL),(673,'admin','200.200.200.17',42165,NULL,NULL,'2013-10-31 13:03:36','2013-10-31 21:03:46','PC',NULL,NULL),(674,'admin','200.200.200.17',42695,NULL,NULL,'2013-11-01 03:10:17','2013-11-01 11:11:15','PC',NULL,NULL),(675,'admin','200.200.200.17',49663,NULL,NULL,'2013-11-01 03:13:52','2013-11-01 11:14:05','PC',NULL,NULL),(676,'hehui','200.200.200.17',46090,NULL,NULL,'2013-11-01 03:14:13','2013-11-01 11:15:20','PC',NULL,NULL),(677,'admin','200.200.200.17',49302,NULL,NULL,'2013-11-01 03:15:28','2013-11-01 11:15:50','PC',NULL,NULL),(678,'admin','200.200.200.17',48165,NULL,NULL,'2013-11-01 03:16:34','2013-11-01 11:17:00','PC',NULL,NULL),(679,'admin','200.200.200.17',48645,NULL,NULL,'2013-11-01 03:17:42','2013-11-01 11:17:48','PC',NULL,NULL);
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
) ENGINE=InnoDB AUTO_INCREMENT=1040 DEFAULT CHARSET=utf8 COMMENT='个人联系人组';
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
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8 COMMENT='成员关系表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `PersonalRelationship`
--

LOCK TABLES `PersonalRelationship` WRITE;
/*!40000 ALTER TABLE `PersonalRelationship` DISABLE KEYS */;
INSERT INTO `PersonalRelationship` VALUES (6,'admin','test4',1005,NULL,'2013-08-14 06:10:51',1),(7,'test4','admin',1009,NULL,'2013-08-14 06:10:51',1),(8,'admin','test5',1005,NULL,'2013-09-01 09:00:09',1),(9,'admin','test6',1028,NULL,'2013-09-01 09:01:48',1),(10,'test4','test5',1009,NULL,'2013-09-09 11:42:00',1),(11,'test5','test4',1010,NULL,'2013-09-09 11:42:00',1),(12,'hehui','admin',1004,NULL,'2013-10-31 08:22:08',1),(13,'admin','hehui',1005,NULL,'2013-10-31 08:22:08',1);
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
INSERT INTO `UsersSummaryInfo` VALUES (1,'System','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,0,'0',0,0,1,1,0,0,0,3,NULL,1,NULL),(16,'hehui','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,29,'1',0,0,1,1,0,1,0,3,NULL,1,NULL),(17,'admin','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,1,60,'2',0,2,1,1,0,0,0,3,NULL,1,NULL),(18,'test1','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,2,17,'3',0,0,1,1,0,1,0,3,NULL,1,NULL),(19,'test2','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,0,'0',0,0,1,1,0,2,0,3,NULL,1,NULL),(20,'test3','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,1,40,'1',0,0,1,1,0,1,0,3,NULL,1,NULL),(21,'test4','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,0,'2',0,20,1,1,0,0,0,3,NULL,1,NULL),(22,'test5','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,1,0,'3',0,0,1,1,0,0,0,3,NULL,1,NULL),(23,'test6','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,0,'0',0,0,1,1,0,0,0,3,NULL,1,NULL);
/*!40000 ALTER TABLE `UsersSummaryInfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'im'
--
/*!50003 DROP PROCEDURE IF EXISTS `sp_CacheChatMessage_FromContact` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`hehui`@`200.200.200.17` PROCEDURE `sp_CacheChatMessage_FromContact`(in p_senderID varchar(16), in p_recieverID varchar(16), in p_message varchar(255)  )
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

-- Dump completed on 2013-11-01 12:32:42

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
) ENGINE=InnoDB AUTO_INCREMENT=174 DEFAULT CHARSET=utf8 COMMENT='缓存的消息';
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
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8 COMMENT='好友请求';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `FriendshipApply`
--

LOCK TABLES `FriendshipApply` WRITE;
/*!40000 ALTER TABLE `FriendshipApply` DISABLE KEYS */;
INSERT INTO `FriendshipApply` VALUES (1,'admin','test1','QQQQQ',0,0,0),(7,'test1','test2','',0,0,0),(8,'test4','test5','',1,1,0);
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
  KEY `FK-GroupID_idx` (`GroupID`),
  KEY `FK-Sender_idx` (`Sender`),
  KEY `FK-Reciver_idx` (`Reciver`),
  CONSTRAINT `FK-Reciver` FOREIGN KEY (`Reciver`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE,
  CONSTRAINT `FK-GroupID` FOREIGN KEY (`GroupID`) REFERENCES `InterestGroups` (`GroupID`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `FK-Sender` FOREIGN KEY (`Sender`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE
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
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 COMMENT='组的成员';
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
  PRIMARY KEY (`GroupID`),
  UNIQUE KEY `idx-Creator-GroupName` (`Creator`,`GroupName`),
  KEY `FK-IG_Creator-USI_UserID` (`Creator`),
  CONSTRAINT `FK-IG_Creator-USI_UserID` FOREIGN KEY (`Creator`) REFERENCES `UsersSummaryInfo` (`UserID`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 COMMENT='兴趣组';
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
) ENGINE=InnoDB AUTO_INCREMENT=326 DEFAULT CHARSET=utf8 COMMENT='登陆历史';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `LoginHistories`
--

LOCK TABLES `LoginHistories` WRITE;
/*!40000 ALTER TABLE `LoginHistories` DISABLE KEYS */;
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
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8 COMMENT='成员关系表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `PersonalRelationship`
--

LOCK TABLES `PersonalRelationship` WRITE;
/*!40000 ALTER TABLE `PersonalRelationship` DISABLE KEYS */;
INSERT INTO `PersonalRelationship` VALUES (6,'admin','test4',1005,NULL,'2013-08-14 06:10:51',1),(7,'test4','admin',1009,NULL,'2013-08-14 06:10:51',1),(8,'admin','test5',1005,NULL,'2013-09-01 09:00:09',1),(9,'admin','test6',1005,NULL,'2013-09-01 09:01:48',1),(10,'test4','test5',1009,NULL,'2013-09-09 11:42:00',1),(11,'test5','test4',1010,NULL,'2013-09-09 11:42:00',1);
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
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8 COMMENT='基本信息表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `UsersSummaryInfo`
--

LOCK TABLES `UsersSummaryInfo` WRITE;
/*!40000 ALTER TABLE `UsersSummaryInfo` DISABLE KEYS */;
INSERT INTO `UsersSummaryInfo` VALUES (1,'System','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,0,NULL,0,0,1,1,0,0,0,3,NULL,1,NULL),(16,'hehui','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,29,NULL,0,0,1,1,0,1,0,3,NULL,1,NULL),(17,'admin','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,1,60,NULL,0,0,1,1,0,0,0,3,NULL,1,NULL),(18,'test1','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,2,17,NULL,0,0,1,1,0,1,0,3,NULL,1,NULL),(19,'test2','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,0,NULL,0,0,1,1,0,2,0,3,NULL,1,NULL),(20,'test3','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,1,40,NULL,0,0,1,1,0,1,0,3,NULL,1,NULL),(21,'test4','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,0,NULL,0,0,1,1,0,0,0,3,NULL,1,NULL),(22,'test5','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,1,0,NULL,0,0,1,1,0,0,0,3,NULL,1,NULL),(23,'test6','P66eKnCMcBPetxi+LMKG2P7cBSA=',NULL,NULL,0,0,NULL,0,0,1,1,0,0,0,3,NULL,1,NULL);
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

select GROUP_CONCAT(igm.GroupID SEPARATOR ',' ) 
from InterestGroupMembers igm, InterestGroups ig
where igm.UserID = igm.p_userID and igm.GroupID = ig.GroupID and ig.State<>0;

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

-- Dump completed on 2013-09-18 17:03:24

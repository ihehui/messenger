-- MySQL dump 10.13  Distrib 5.5.15, for Win32 (x86)
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
-- Current Database: `im`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `im` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `im`;

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
  `TransmittingTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '发送时间',
  `Message` varchar(255) NOT NULL COMMENT '消息',
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
  `Result` char(1) NOT NULL DEFAULT '0' COMMENT '0:未知,1:接受,2:拒绝',
  `SenderRead` char(1) NOT NULL DEFAULT '0' COMMENT '0:未读,1：已读',
  `ReceiverRead` char(1) NOT NULL DEFAULT '0' COMMENT '0:未读,1：已读',
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
  CONSTRAINT `FK-IGM_UserID-USI_UserID` FOREIGN KEY (`UserID`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK-IGM_GroupID-IG_GroupID` FOREIGN KEY (`GroupID`) REFERENCES `InterestGroups` (`GroupID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK-IGm_RoleID-IGMR_RoleID` FOREIGN KEY (`RoleID`) REFERENCES `InterestGroupMemberRoles` (`RoleID`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 COMMENT='组的成员';
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
) ENGINE=InnoDB AUTO_INCREMENT=1002 DEFAULT CHARSET=utf8 COMMENT='兴趣组';
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
  `IntIPAddress` varchar(64) NOT NULL,
  `LoginTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `LogoutTime` datetime DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `FK-LH_UserID-USI_UserID` (`UserID`),
  CONSTRAINT `FK-LH_UserID-USI_UserID` FOREIGN KEY (`UserID`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='登陆历史';
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
  `ContactGroupID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `Creator` varchar(16) NOT NULL,
  `GroupName` varchar(45) NOT NULL,
  PRIMARY KEY (`ContactGroupID`),
  UNIQUE KEY `index-Creator-GroupName` (`Creator`,`GroupName`),
  KEY `FK-PCG_Creator-USI_UserID` (`Creator`),
  CONSTRAINT `FK-PCG_Creator-USI_UserID` FOREIGN KEY (`Creator`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='个人联系人组';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `PersonalContactGroups`
--

LOCK TABLES `PersonalContactGroups` WRITE;
/*!40000 ALTER TABLE `PersonalContactGroups` DISABLE KEYS */;
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
  `Relationship` tinyint(4) NOT NULL DEFAULT '1' COMMENT '成员关系\n-1:blacklisted\n0:stranger\n1:approved',
  `PersonalContactGroup` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '联系人所在的个人组。0:Blacklist 1:Friends',
  `RemarkName` varchar(32) DEFAULT NULL COMMENT '名称备注',
  PRIMARY KEY (`ID`),
  KEY `FK-UF_UserID-USI_UserID` (`UserID`),
  KEY `FK-UF_ContactID-USI_UserID` (`ContactID`),
  KEY `FK-UF_PCG-PCG_IPCGID` (`PersonalContactGroup`),
  KEY `FK-UF_PCGID-PCG_CGID` (`PersonalContactGroup`),
  CONSTRAINT `FK-UF_PCGID-PCG_CGID` FOREIGN KEY (`PersonalContactGroup`) REFERENCES `PersonalContactGroups` (`ContactGroupID`) ON UPDATE CASCADE,
  CONSTRAINT `FK-UF_ContactID-USI_UserID` FOREIGN KEY (`ContactID`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK-UF_UserID-USI_UserID` FOREIGN KEY (`UserID`) REFERENCES `UsersSummaryInfo` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='成员关系表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `PersonalRelationship`
--

LOCK TABLES `PersonalRelationship` WRITE;
/*!40000 ALTER TABLE `PersonalRelationship` DISABLE KEYS */;
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
  `Age` tinyint(3) unsigned DEFAULT '0',
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
  `Description` varchar(256) DEFAULT NULL COMMENT '自我介绍',
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
INSERT INTO `UsersDetailedInfo` VALUES (1,29,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
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
  `Gender` tinyint(1) DEFAULT '0' COMMENT '0:unknown 1:male 2:Female',
  `Face` varchar(255) DEFAULT NULL,
  `PersonalContactGroupsInfoVersion` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '联系人组版本',
  `InterestGroupsInfoVersion` int(10) unsigned NOT NULL DEFAULT '0',
  `BlacklistInfoVersion` int(10) unsigned DEFAULT '0',
  `Blacklist` varchar(512) DEFAULT NULL COMMENT '黒名单,用“,”分隔联系人的ID',
  `PersonalSummaryInfoVersion` int(11) NOT NULL DEFAULT '1',
  `PersonalDetailInfoVersion` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '个人信息版本',
  `FriendshipApply` char(1) NOT NULL DEFAULT '0' COMMENT '好友请求(V:要求认证;A:自动接受)',
  `ShortTalk` char(1) NOT NULL DEFAULT '0' COMMENT '临时佳话(P:提示;D:拒绝;A:自动接受)',
  `Role` int(10) unsigned NOT NULL DEFAULT '3',
  `Description` varchar(256) DEFAULT NULL COMMENT '自我介绍',
  `AccountState` tinyint(1) NOT NULL DEFAULT '1' COMMENT '0:Invalid 1:Normal 2:Banned 3:Limitted',
  PRIMARY KEY (`SysID`),
  UNIQUE KEY `index_UsersSummaryInfo_UserID` (`UserID`)
) ENGINE=InnoDB AUTO_INCREMENT=10000 DEFAULT CHARSET=utf8 COMMENT='基本信息表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `UsersSummaryInfo`
--

LOCK TABLES `UsersSummaryInfo` WRITE;
/*!40000 ALTER TABLE `UsersSummaryInfo` DISABLE KEYS */;
INSERT INTO `UsersSummaryInfo` VALUES (1,'hehui','s0jwcxN/IhFAz7GaJEk29xPToSw=',NULL,NULL,0,NULL,0,0,0,NULL,1,1,'0','0',3,NULL,1),(2,'admin','s0jwcxN/IhFAz7GaJEk29xPToSw=',NULL,NULL,0,NULL,0,0,0,NULL,1,1,'0','0',3,NULL,1),(9,'test','s0jwcxN/IhFAz7GaJEk29xPToSw=',NULL,NULL,0,NULL,0,0,0,NULL,1,1,'0','0',3,NULL,1);
/*!40000 ALTER TABLE `UsersSummaryInfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'im'
--
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
/*!50003 CREATE*/ /*!50020 DEFINER=`hehui`@`%`*/ /*!50003 PROCEDURE `sp_CreateNewUser`(in u_id varchar(16), in u_password varchar(32), out u_sysid int(10) unsigned )
BEGIN

select SysID into u_sysid from UsersSummaryInfo where UserID = u_id ;
if u_sysid is null then
    insert into UsersSummaryInfo(UserID, UserPassword) values(u_id, u_password);
    select SysID into u_sysid from UsersSummaryInfo where UserID = u_id ;
else    
    #如果记录已存在则u_sysid返回0
    set u_sysid = 0; 
end if;

#insert into UsersSummaryInfo(UserID, UserPassword, NickName, Gender) values(u_id, u_password, u_nickname, u_gender);
#select SysID into u_sysid from UsersSummaryInfo where UserID = u_id ;
END */;;
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
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`200.200.200.17`*/ /*!50003 PROCEDURE `sp_GetInterestGroupInfo`(in p_groupID int(10) unsigned)
BEGIN
	select * from InterestGroups where GroupID = p_groupID;
END */;;
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
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`200.200.200.17`*/ /*!50003 PROCEDURE `sp_GetInterestGroupMembers`(in p_groupID int(10) unsigned)
BEGIN
	select UserID, RoleID from InterestGroupMembers where GroupID = p_groupID ;
END */;;
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
/*!50003 CREATE*/ /*!50020 DEFINER=`hehui`@`200.200.200.17`*/ /*!50003 PROCEDURE `sp_GetUserInfo`(in p_userID varchar(16) )
BEGIN
SELECT * FROM UsersSummaryInfo usi left join UsersDetailedInfo udi on usi.SysID=udi.SysID WHERE usi.UserID = p_userID;
END */;;
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
/*!50003 CREATE*/ /*!50020 DEFINER=`hehui`@`200.200.200.17`*/ /*!50003 PROCEDURE `sp_GetUserInfoBySysID`(in p_userSysID int(10) unsigned )
BEGIN
SELECT * FROM UsersSummaryInfo usi left join UsersDetailedInfo udi on usi.SysID=udi.SysID WHERE usi.SysID = p_userSysID;
END */;;
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
/*!50003 CREATE*/ /*!50020 DEFINER=`hehui`@`200.200.200.17`*/ /*!50003 PROCEDURE `sp_GetUserInterestGroups`(in p_userSysID int(10) unsigned )
BEGIN
	select GROUP_CONCAT(GroupID SEPARATOR ',' ) from InterestGroupMembers where MemberSysID = p_userSysID ;
END */;;
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

-- Dump completed on 2013-06-04 10:10:39

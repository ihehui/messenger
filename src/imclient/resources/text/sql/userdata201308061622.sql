
PRAGMA foreign_keys = OFF;

-- ----------------------------
-- Table structure for contactchatmessages
-- ----------------------------
DROP TABLE IF EXISTS "main"."contactchatmessages";
CREATE TABLE "contactchatmessages"(
[ID] integer PRIMARY KEY AUTOINCREMENT NOT NULL
,[SenderID] varchar(16) NOT NULL
,[ReceiverID] varchar(16) NOT NULL
,[Message] varchar(512) NOT NULL
,[Time] timestamp NOT NULL DEFAULT current_timestamp
   
);

-- ----------------------------
-- Records of contactchatmessages
-- ----------------------------

-- ----------------------------
-- Table structure for contactgroups
-- ----------------------------
DROP TABLE IF EXISTS "main"."contactgroups";
CREATE TABLE "contactgroups" (
"GroupID"  INTEGER NOT NULL ON CONFLICT FAIL,
"GroupName"  varchar(32) NOT NULL,
"GroupInfoVersion"  INTEGER NOT NULL DEFAULT 0,
"MemberListVersion"  INTEGER NOT NULL DEFAULT 0,
PRIMARY KEY ("GroupID" ASC),
UNIQUE ("GroupName" ASC)
);

-- ----------------------------
-- Records of contactgroups
-- ----------------------------
INSERT INTO "main"."contactgroups" VALUES (-1, 'Strangers', 1, 1);
INSERT INTO "main"."contactgroups" VALUES (1, 'Friends', 0, 0);

-- ----------------------------
-- Table structure for contacts_detailed_info
-- ----------------------------
DROP TABLE IF EXISTS "main"."contacts_detailed_info";
CREATE TABLE "contacts_detailed_info" (
"UserID"  varchar(16) NOT NULL,
"TrueName"  varchar(32) DEFAULT NULL,
"NickName"  varchar(32) DEFAULT NULL,
"Gender"  char(1) DEFAULT NULL,
"Age"  tinyint DEFAULT 0,
"Face"  varchar(255) DEFAULT NULL,
"ContactGroupID"  INTEGER NOT NULL ON CONFLICT REPLACE DEFAULT -1,
"PersonalSummaryInfoVersion"  INTEGER NOT NULL DEFAULT 1,
"PersonalDetailInfoVersion"  INTEGER NOT NULL DEFAULT 0,
"Role"  int(10) DEFAULT '3',
"Description"  varchar(255),
"HomeAddress"  varchar(255) DEFAULT NULL,
"HomePhoneNumber"  varchar(32) DEFAULT NULL,
"HomeZipCode"  varchar(12) DEFAULT NULL,
"PersonalHomepage"  varchar(255) DEFAULT NULL,
"PersonalEmailAddress"  varchar(255) DEFAULT NULL,
"CompanyName"  varchar(255) DEFAULT NULL,
"JobTitle"  varchar(255) DEFAULT NULL,
"BusinessAddress"  varchar(255) DEFAULT NULL,
"BusinessPhoneNumber"  varchar(255) DEFAULT NULL,
"BusinessZipCode"  varchar(255) DEFAULT NULL,
"BusinessFaxNumber"  varchar(255) DEFAULT NULL,
"BusinessHomepage"  varchar(255) DEFAULT NULL,
"BusinessEmailAddress"  varchar(255) DEFAULT NULL,
"RemarkName"  varchar(32) DEFAULT NULL,
PRIMARY KEY ("UserID" ASC),
CONSTRAINT "FK-ContactGroupID" FOREIGN KEY ("ContactGroupID") REFERENCES "contactgroups" ("GroupID") ON DELETE SET NULL ON UPDATE CASCADE,
UNIQUE ("UserID" ASC)
);

-- ----------------------------
-- Records of contacts_detailed_info
-- ----------------------------

-- ----------------------------
-- Table structure for interestgroupchatmessages
-- ----------------------------
DROP TABLE IF EXISTS "main"."interestgroupchatmessages";
CREATE TABLE "interestgroupchatmessages"(
[ID] integer PRIMARY KEY AUTOINCREMENT NOT NULL
,[SenderID] varchar(16) NOT NULL
,[GroupID] integer NOT NULL
,[Message] varchar(512)
,[Time] timestamp NOT NULL DEFAULT current_timestamp
   
);

-- ----------------------------
-- Records of interestgroupchatmessages
-- ----------------------------

-- ----------------------------
-- Table structure for interestgroupmembers
-- ----------------------------
DROP TABLE IF EXISTS "main"."interestgroupmembers";
CREATE TABLE "interestgroupmembers"(
[ID] integer PRIMARY KEY AUTOINCREMENT
,[MemberID] varchar(16) NOT NULL
,[GroupID] int NOT NULL
,[MemberRole] int NOT NULL
   
);

-- ----------------------------
-- Records of interestgroupmembers
-- ----------------------------

-- ----------------------------
-- Table structure for interestgroups
-- ----------------------------
DROP TABLE IF EXISTS "main"."interestgroups";
CREATE TABLE "interestgroups"(
[GroupID] integer(10) NOT NULL
,[TypeID] int(10)
,[ParentGroup] int(10) DEFAULT NULL
,[Creator] int(10)
,[GroupName] varchar(32)
,[CreationTime] datetime
,[GroupInfoVersion] int(10) NOT NULL DEFAULT '1'
,[MemberListVersion] int(10) NOT NULL DEFAULT '1'
,[Description] varchar(64)
,[Announcement] varchar(64) DEFAULT NULL
,[Remark] varchar(64) DEFAULT NULL
, Primary Key(GroupID)   
);

-- ----------------------------
-- Records of interestgroups
-- ----------------------------

-- ----------------------------
-- Table structure for my_detailed_info
-- ----------------------------
DROP TABLE IF EXISTS "main"."my_detailed_info";
CREATE TABLE "my_detailed_info" (
"SysID"  INTEGER,
"UserID"  varchar(16) NOT NULL,
"TrueName"  varchar(20) DEFAULT NULL,
"NickName"  varchar(20) DEFAULT NULL,
"Gender"  char(1) NOT NULL DEFAULT 0,
"Age"  tinyint NOT NULL DEFAULT 0,
"Face"  varchar(255) DEFAULT 0,
"PersonalContactGroupsInfoVersion"  int(10) NOT NULL DEFAULT 0,
"InterestGroupsInfoVersion"  int(10) NOT NULL DEFAULT 0,
"BlacklistInfoVersion"  int(10) NOT NULL DEFAULT 0,
"PersonalSummaryInfoVersion"  int(10) NOT NULL DEFAULT 0,
"PersonalDetailInfoVersion"  INTEGER NOT NULL,
"FriendshipApply"  char(1),
"ShortTalk"  char(1),
"Role"  INTEGER,
"Description"  VARCHAR(255),
"HomeAddress"  varchar(255) DEFAULT NULL,
"HomePhoneNumber"  varchar(32) DEFAULT NULL,
"HomeZipCode"  varchar(12) DEFAULT NULL,
"PersonalHomepage"  varchar(255) DEFAULT NULL,
"PersonalEmailAddress"  varchar(255) DEFAULT NULL,
"CompanyName"  varchar(255) DEFAULT NULL,
"JobTitle"  varchar(255) DEFAULT NULL,
"BusinessAddress"  varchar(255) DEFAULT NULL,
"BusinessPhoneNumber"  varchar(255) DEFAULT NULL,
"BusinessZipCode"  varchar(255) DEFAULT NULL,
"BusinessFaxNumber"  varchar(255) DEFAULT NULL,
"BusinessHomepage"  varchar(255) DEFAULT NULL,
"BusinessEmailAddress"  varchar(255) DEFAULT NULL,
"LastLoginHostAddress"  char(15) DEFAULT NULL,
"LastLoginHostPort"  smallint(5) DEFAULT NULL
);

-- ----------------------------
-- Records of my_detailed_info
-- ----------------------------

-- ----------------------------
-- Table structure for sqlite_sequence
-- ----------------------------
DROP TABLE IF EXISTS "main"."sqlite_sequence";
CREATE TABLE sqlite_sequence(name,seq);

-- ----------------------------
-- Records of sqlite_sequence
-- ----------------------------

-- ----------------------------
-- Table structure for sqlite_stat1
-- ----------------------------
DROP TABLE IF EXISTS "main"."sqlite_stat1";
CREATE TABLE sqlite_stat1(tbl,idx,stat);

-- ----------------------------
-- Records of sqlite_stat1
-- ----------------------------

-- ----------------------------
-- Indexes structure for table contacts_detailed_info
-- ----------------------------
CREATE INDEX "main"."idx-UserID"
ON "contacts_detailed_info" ("UserID" ASC);

-- ----------------------------
-- Indexes structure for table interestgroupmembers
-- ----------------------------
CREATE UNIQUE INDEX "main"."MemberID_GroupID"
ON "interestgroupmembers" ("MemberID" ASC, "GroupID" ASC);

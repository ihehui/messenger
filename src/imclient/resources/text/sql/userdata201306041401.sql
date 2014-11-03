Begin Transaction;
Drop Table If Exists [contactchatmessages];
CREATE TABLE IF NOT EXISTS  "contactchatmessages"(
[ID] integer PRIMARY KEY AUTOINCREMENT NOT NULL
,[SenderID] varchar(16) NOT NULL
,[ReceiverID] varchar(16) NOT NULL
,[Message] varchar(512) NOT NULL
,[Time] timestamp NOT NULL DEFAULT current_timestamp
   
);
Drop Table If Exists [contactgroups];
CREATE TABLE IF NOT EXISTS  "contactgroups"(
[GroupName] varchar(32) UNIQUE NOT NULL
,[GroupID] integer NOT NULL
, Primary Key(GroupID)   
);
Drop Table If Exists [contacts_detailed_info];
CREATE TABLE IF NOT EXISTS  "contacts_detailed_info" (
"UserID"  varchar(16) NOT NULL,
"TrueName"  varchar(32) DEFAULT NULL,
"NickName"  varchar(32) DEFAULT NULL,
"Gender"  char(1) DEFAULT NULL,
"Face"  varchar(255) DEFAULT NULL,
"ContactGroupID"  int(10) DEFAULT 0,
"PersonalSummaryInfoVersion"  int(10) NOT NULL DEFAULT '1',
"PersonalDetailInfoVersion"  INTEGER NOT NULL,
"Description"  varchar(255),
"Age"  tinyint DEFAULT 0,
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
"Role"  int(10) DEFAULT '3',
"RemarkName"  varchar(32) DEFAULT NULL,
PRIMARY KEY ("UserID" ASC),
UNIQUE ("UserID" ASC)
);
Drop Table If Exists [interestgroupchatmessages];
CREATE TABLE IF NOT EXISTS  "interestgroupchatmessages"(
[ID] integer PRIMARY KEY AUTOINCREMENT NOT NULL
,[SenderID] varchar(16) NOT NULL
,[GroupID] integer NOT NULL
,[Message] varchar(512)
,[Time] timestamp NOT NULL DEFAULT current_timestamp
   
);
Drop Table If Exists [interestgroupmembers];
CREATE TABLE IF NOT EXISTS  "interestgroupmembers"(
[ID] integer PRIMARY KEY AUTOINCREMENT
,[MemberID] varchar(16) NOT NULL
,[GroupID] int NOT NULL
,[MemberRole] int NOT NULL
   
);
Drop Table If Exists [interestgroups];
CREATE TABLE IF NOT EXISTS  "interestgroups"(
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
Drop Table If Exists [my_detailed_info];
CREATE TABLE IF NOT EXISTS  "my_detailed_info" (
"SysID"  INTEGER,
"UserID"  varchar(16) NOT NULL,
"TrueName"  varchar(20) DEFAULT NULL,
"NickName"  varchar(20) DEFAULT NULL,
"Gender"  char(1) NOT NULL DEFAULT 0,
"Face"  varchar(255) DEFAULT 0,
"PersonalContactGroupsInfoVersion"  int(10) NOT NULL DEFAULT 0,
"InterestGroupsInfoVersion"  int(10) NOT NULL DEFAULT 0,
"BlacklistInfoVersion"  int(10) NOT NULL DEFAULT 0,
"PersonalSummaryInfoVersion"  int(10) NOT NULL DEFAULT 0,
"PersonalDetailInfoVersion"  INTEGER NOT NULL,
"FriendshipApply"  char(1),
"ShortTalk"  char(1),
"Role"  INTEGER,
"Age"  tinyint NOT NULL DEFAULT 0,
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
Drop Index  If Exists [MemberID_GroupID];
CREATE UNIQUE INDEX IF NOT EXISTS  [MemberID_GroupID] On [interestgroupmembers] ([MemberID] ,[GroupID] );
Commit Transaction;

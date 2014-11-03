Begin Transaction;
Drop Table If Exists [contactchatmessages];
CREATE TABLE "contactchatmessages"(
[ID] integer PRIMARY KEY AUTOINCREMENT NOT NULL
,[ContactID] varchar(24) NOT NULL
,[Message] varchar(512) NOT NULL
,[Time] timestamp NOT NULL
   
);
Drop Table If Exists [contactgroups];
CREATE TABLE "contactgroups"(
[GroupName] varchar(32) UNIQUE NOT NULL
,[GroupID] integer NOT NULL
, Primary Key(GroupID)   
);
Drop Table If Exists [contacts_detailed_info];
CREATE TABLE "contacts_detailed_info"(
[UserID] varchar(16) PRIMARY KEY UNIQUE NOT NULL
,[TrueName] varchar(20) DEFAULT NULL
,[NickName] varchar(20) DEFAULT NULL
,[Gender] char(1) DEFAULT NULL
,[Age] tinyint DEFAULT 0
,[Face] varchar(255) DEFAULT NULL
,[InterestGroupID] int(10) DEFAULT 0
,[SystemGroupID] int(10) DEFAULT 0
,[ContactGroupID] int(10) DEFAULT 0
,[PersonalInfoVersion] int(10) NOT NULL DEFAULT '1'
,[HomeAddress] varchar(255) DEFAULT NULL
,[HomePhoneNumber] varchar(32) DEFAULT NULL
,[HomeZipCode] varchar(12) DEFAULT NULL
,[PersonalHomepage] varchar(255) DEFAULT NULL
,[PersonalEmailAddress] varchar(255) DEFAULT NULL
,[LastLoginTime] timestamp DEFAULT CURRENT_TIMESTAMP
,[LastLoginHostAddress] char(15) DEFAULT NULL
,[LastLoginHostPort] smallint(5) DEFAULT NULL
,[CompanyName] varchar(255) DEFAULT NULL
,[JobTitle] varchar(255) DEFAULT NULL
,[BusinessAddress] varchar(255) DEFAULT NULL
,[BusinessPhoneNumber] varchar(255) DEFAULT NULL
,[BusinessZipCode] varchar(255) DEFAULT NULL
,[BusinessFaxNumber] varchar(255) DEFAULT NULL
,[BusinessHomepage] varchar(255) DEFAULT NULL
,[BusinessEmailAddress] varchar(255) DEFAULT NULL
,[RegistrationTime] datetime DEFAULT NULL
,[Role] int(10) DEFAULT '3'
   
);
Drop Table If Exists [interestgroupchatmessages];
CREATE TABLE "interestgroupchatmessages"(
[ID] integer PRIMARY KEY AUTOINCREMENT NOT NULL
,[GroupID] integer NOT NULL
,[SenderID] varchar(24) NOT NULL
,[Message] varchar(512)
,[Time] timestamp NOT NULL
   
);
Drop Table If Exists [interestgroupmembers];
CREATE TABLE "interestgroupmembers"(
[ID] integer PRIMARY KEY AUTOINCREMENT
,[MemberID] varchar(16) NOT NULL
,[GroupID] int NOT NULL
,[MemberRole] int NOT NULL
   
);
Drop Table If Exists [interestgroups];
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
Drop Table If Exists [my_detailed_info];
CREATE TABLE "my_detailed_info"(
[UserID] varchar(16) NOT NULL
,[TrueName] varchar(20) DEFAULT NULL
,[NickName] varchar(20) DEFAULT NULL
,[Gender] char(1) NOT NULL DEFAULT 0
,[Age] tinyint NOT NULL DEFAULT 0
,[Face] varchar(255) DEFAULT 0
,[SystemGroupsInfoVersion-] int(10) DEFAULT 0
,[InterestGroupsInfoVersion] int(10) NOT NULL DEFAULT 0
,[PersonalContactGroupsInfoVersion] int(10) NOT NULL DEFAULT 0
,[BlacklistInfoVersion] int(10) NOT NULL DEFAULT 0
,[Blacklist] varchar(512)
,[PersonalInfoVersion] int(10) NOT NULL DEFAULT 0
,[HomeAddress] varchar(255) DEFAULT NULL
,[HomePhoneNumber] varchar(32) DEFAULT NULL
,[HomeZipCode] varchar(12) DEFAULT NULL
,[PersonalHomepage] varchar(255) DEFAULT NULL
,[PersonalEmailAddress] varchar(255) DEFAULT NULL
,[LastLoginTime] timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
,[LastLoginHostAddress] char(15) DEFAULT NULL
,[LastLoginHostPort] smallint(5) DEFAULT NULL
,[QuestionForSecurity] varchar(255) DEFAULT NULL
,[AnswerForSecurity] varchar(255) DEFAULT NULL
,[CompanyName] varchar(255) DEFAULT NULL
,[JobTitle] varchar(255) DEFAULT NULL
,[BusinessAddress] varchar(255) DEFAULT NULL
,[BusinessPhoneNumber] varchar(255) DEFAULT NULL
,[BusinessZipCode] varchar(255) DEFAULT NULL
,[BusinessFaxNumber] varchar(255) DEFAULT NULL
,[BusinessHomepage] varchar(255) DEFAULT NULL
,[BusinessEmailAddress] varchar(255) DEFAULT NULL
,[RegistrationTime] datetime DEFAULT NULL
,[LoginTimes] int(10) DEFAULT NULL
,[FriendshipApply] char(1) DEFAULT NULL
,[ShortTalk] char(1) DEFAULT NULL
,[Role] int(10) NOT NULL DEFAULT '3'
   
);
Drop Table If Exists [sqlite_stat2];
CREATE TABLE sqlite_stat2(tbl,idx,sampleno,sample);
Drop Index  If Exists [MemberID_GroupID];
CREATE UNIQUE INDEX [MemberID_GroupID] On [interestgroupmembers] (
[MemberID] ,
[GroupID] );
Commit Transaction;

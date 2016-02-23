
/*
---------------------------------
说明 : 服务器帐号情况统计表
表名 : tb_jxex_stat_account
----------------------------------
*/
CREATE TABLE IF NOT EXISTS tb_jxex_stat_account
(
  ID                  int(11)                             not null        auto_increment,          -- ID,主键
  tStatDate           date                                not null,                                -- 日期
  nTotalAccount       int(11)                             not null,                                -- 帐号总数
  nActiveAccount      int(11)                             not null,                                -- 当前活跃帐号数
  nLostAccount        int(11)                             not null,                                -- 本日流失帐号数
  primary key(ID),
  unique(tStatDate)
)ENGINE=InnoDB DEFAULT CHARSET=latin1;		


/*
---------------------------------
说明 : 服务器角色情况统计表
表名 : tb_jxex_stat_role
----------------------------------
*/
CREATE TABLE IF NOT EXISTS tb_jxex_stat_role
(
  ID                  int(11)                             not null        auto_increment,          -- ID,主键
  tStatDate           date                                not null,                                -- 日期
  nTotalRole          int(11)                             not null,                                -- 角色总数
  nActiveRole         int(11)                             not null,                                -- 当前活跃角色数量
  nRole1              int(11)                             not null,                                -- 1角色帐号数量
  nRole2              int(11)                             not null,                                -- 2角色帐号数量
  nRole3              int(11)                             not null,                                -- 3角色帐号数量
  nRole4              int(11)                             not null,                                -- 4角色帐号数量
  nRole5              int(11)                             not null,                                -- 5角色帐号数量
  nRole6              int(11)                             not null,                                -- 6角色帐号数量
  nRole7              int(11)                             not null,                                -- 7角色帐号数量
  nRole8              int(11)                             not null,                                -- 8角色帐号数量
  nRole9              int(11)                             not null,                                -- 9角色帐号数量
  nRole10             int(11)                             not null,                                -- 10角色帐号数量
  nRole11             int(11)                             not null,                                -- 11角色帐号数量
  nRole12             int(11)                             not null,                                -- 12角色帐号数量
  primary key(ID),
  unique(tStatDate)
)ENGINE=InnoDB DEFAULT CHARSET=latin1;


/*
---------------------------------
说明 : 角色基本资料统计表
表名 : tb_jxex_stat_roleinfo
----------------------------------
*/
CREATE TABLE IF NOT EXISTS tb_jxex_stat_roleinfo
(
  ID                  int(11)                             not null        auto_increment,          -- ID,主键
  tStatDate           date                                not null,                                -- 日期
  sField              varchar(200)                        not null,                                -- 段名
  sValue              varchar(200)                        not null,                                -- 段值
  nTotal              int(11)                             not null,                                -- 人数
  primary key(ID),
  index(tStatDate)
)ENGINE=InnoDB DEFAULT CHARSET=latin1;		


/*
---------------------------------
说明 : 剑侠币统计表
表名 : tb_jxex_stat_gamemoney
----------------------------------
*/
CREATE TABLE IF NOT EXISTS tb_jxex_stat_gamemoney
(
  ID                  int(11)                             not null        auto_increment,          -- ID,主键
  tStatDate           date                                not null,                                -- 日期
  sName               varchar(200)                        not null,                                -- 途径
  sType               varchar(200)                        not null,                                -- 类型
  nValue              int(11)                             not null,                                -- 段值
  primary key(ID),
  index(tStatDate)
)ENGINE=InnoDB DEFAULT CHARSET=latin1;
	

/*
---------------------------------
说明 : 奇珍阁统计表
表名 : tb_jxex_stat_ibshop
----------------------------------
*/
CREATE TABLE IF NOT EXISTS tb_jxex_stat_ibshop
(
  ID                  int(11)                             not null        auto_increment,          -- ID,主键
  tStatDate           date                                not null,                                -- 日期
  sItem_Name          varchar(200)                        not null,                                -- 道具名
  nItem_Type          int(11)                             not null,                                -- 道具类型
  nJB_Amount          int(11)                             not null,                                -- 金币销售总数
  nBindJB_Amount      int(11)                             not null,                                -- 绑定金币销售总数
  nPrice              int(11)                             not null,                                -- 金币单价
  nBindPrice          int(11)                             not null,                                -- 绑定金币单价
  primary key(ID),
  index(tStatDate)	
)ENGINE=InnoDB DEFAULT CHARSET=latin1;

		
/*
---------------------------------
说明 : 道具类型表
表名 : tb_jxex_stat_itemtype
----------------------------------
*/
CREATE TABLE IF NOT EXISTS tb_jxex_stat_itemtype
(
  nTypeID             int(11)                             not null,                                -- 类型ID,主键
  sTypeName           varchar(200)                        not null,                                -- 类型名
  sRemark             varchar(200)                            null,                                -- 描述
  primary key(nTypeID)
)ENGINE=InnoDB DEFAULT CHARSET=latin1;

		
/*
---------------------------------
说明 : 错误信息表
表名 : tb_jxex_stat_err
----------------------------------
*/
CREATE TABLE IF NOT EXISTS tb_jxex_stat_err
(
  Error            varchar(200)                           not null,        -- 错误
  Time_Stamp       timestamp             default now()    not null         -- 日期
)ENGINE=InnoDB DEFAULT CHARSET=latin1;


/*剑侠世界运营数据统计
统计服务器帐号情况
统计服务器角色情况
统计角色基本资料
统计剑侠币
统计奇珍阁
*/
DROP PROCEDURE IF EXISTS proc_jxex_stat;
CREATE PROCEDURE proc_jxex_stat()
main:BEGIN
 declare v_Table_Role varchar(200);
 declare v_Table_GameMoney varchar(200);
 declare v_Table_IBShop varchar(200);
 declare v_Table_ibitem varchar(200);
 declare v_postfix varchar(200);
 
 declare v_Field_Role varchar(200);
 declare v_Field_Account varchar(200);
 declare v_Field_Grade varchar(200);
 declare v_Field_Mana varchar(200);
 declare v_Field_LastLogin varchar(200);
 declare v_Field_GameTime varchar(200);
 declare v_Field_TrustTime varchar(200);
 declare v_Field_GameMoney varchar(200);
 declare v_Field_EquipValue varchar(200);
 declare v_Field_Friends varchar(200);
 declare v_Field_MakeSkill varchar(200);
 
 declare v_Field_MoneyType varchar(200);
 declare v_Field_Value varchar(200);
 
 declare v_Field_ItemName varchar(200);
 declare v_Field_JBAmount varchar(200);
 declare v_Field_BindJBAmount varchar(200);
 
 declare v_Field_Item varchar(200);
 declare v_Field_Price varchar(200);
 declare v_Field_BindPrice varchar(200);
 declare v_Field_ItemTyp varchar(200);
 
 declare v_TotalAccount int;
 declare v_ActiveAccount int;
 declare v_LostAccount int;
 declare v_TotalRole int;
 declare v_ActiveRole int;
 declare v_Mana int;
 
 declare v_Role1 int;
 declare v_Role2 int;
 declare v_Role3 int;
 declare v_Role4 int;
 declare v_Role5 int;
 declare v_Role6 int;
 declare v_Role7 int;
 declare v_Role8 int;
 declare v_Role9 int;
 declare v_Role10 int;
 declare v_Role11 int;
 declare v_Role12 int;
 
 declare v_Field varchar(50);
 declare v_Value varchar(50);
 declare v_Total int;
 
 declare v_Max int;
 declare v_Min int;
 declare v_num int;
 declare v_varchar varchar(200);
 declare v_Date date;
 declare v_TimeStamp datetime;
 declare v_BaseMana int;
 declare v_ActiveDate int;
 declare v_DatabaseName varchar(200);
 declare v_errmsg varchar(200);
 
 START TRANSACTION;
 
 set v_BaseMana = 10;
 set v_ActiveDate = 3;
 set v_Date = SUBDATE(CURRENT_DATE(),1);
 set v_DatabaseName = database();
  
 set v_postfix = v_Date + 0;
 set v_Table_Role = CONCAT('roleinfo_' , v_postfix);
 set v_Table_GameMoney = CONCAT('jxb_' , v_postfix);
 set v_Table_IBShop = CONCAT('ibshop_' , v_postfix);
 set v_Table_ibitem = CONCAT('ibitem_' , v_postfix);
 
 -- RoleInfo 
 select sequenumber into v_Field_Role from tblfieldname where tablename = 'RoleInfo' and fieldname = '角色名';
 set v_Field_Role = concat('F',v_Field_Role);

 select sequenumber into v_Field_Account from tblfieldname where tablename = 'RoleInfo' and fieldname = '帐号名';
 set v_Field_Account = concat('F',v_Field_Account); 

 select sequenumber into v_Field_Grade from tblfieldname where tablename = 'RoleInfo' and fieldname = '等级';
 set v_Field_Grade = concat('F',v_Field_Grade); 

 select sequenumber into v_Field_Mana from tblfieldname where tablename = 'RoleInfo' and fieldname = '江湖威望';
 set v_Field_Mana = concat('F',v_Field_Mana);

 select sequenumber into v_Field_LastLogin from tblfieldname where tablename = 'RoleInfo' and fieldname = '上次登录时间';
 set v_Field_LastLogin = concat('F',v_Field_LastLogin);

 select sequenumber into v_Field_GameTime from tblfieldname where tablename = 'RoleInfo' and fieldname = '总在线时间';
 set v_Field_GameTime = concat('F',v_Field_GameTime);

 select sequenumber into v_Field_TrustTime from tblfieldname where tablename = 'RoleInfo' and fieldname = '总托管时间';
 set v_Field_TrustTime = concat('F',v_Field_TrustTime);  

 select sequenumber into v_Field_GameMoney from tblfieldname where tablename = 'RoleInfo' and fieldname = '剑侠币数';
 set v_Field_GameMoney = concat('F',v_Field_GameMoney);

 select sequenumber into v_Field_EquipValue from tblfieldname where tablename = 'RoleInfo' and fieldname = '装备价值';
 set v_Field_EquipValue = concat('F',v_Field_EquipValue);
     
 select sequenumber into v_Field_Friends from tblfieldname where tablename = 'RoleInfo' and fieldname = '好友数';
 set v_Field_Friends = concat('F',v_Field_Friends);
     
 select sequenumber into v_Field_MakeSkill from tblfieldname where tablename = 'RoleInfo' and fieldname = '最高生活技能等级';
 set v_Field_MakeSkill = concat('F',v_Field_MakeSkill);
   
 -- JXB      
 select sequenumber into v_Field_MoneyType from tblfieldname where tablename = 'JXB' and fieldname = '途径';
 set v_Field_MoneyType = concat('F',v_Field_MoneyType);
     
 select sequenumber into v_Field_Value from tblfieldname where tablename = 'JXB' and fieldname = '总量';
 set v_Field_Value = concat('F',v_Field_Value);
 
 -- IBShop     
 select sequenumber into v_Field_ItemName from tblfieldname where tablename = 'IBShop' and fieldname = '道具名称';
 set v_Field_ItemName = concat('F',v_Field_ItemName);
     
 select sequenumber into v_Field_JBAmount from tblfieldname where tablename = 'IBShop' and fieldname = '金币销售总数';
 set v_Field_JBAmount = concat('F',v_Field_JBAmount);
    
 select sequenumber into v_Field_BindJBAmount from tblfieldname where tablename = 'IBShop' and fieldname = '绑定金币销售总数';
 set v_Field_BindJBAmount = concat('F',v_Field_BindJBAmount);
  
 -- ibitem  
 select sequenumber into v_Field_Item from tblfieldname where tablename = 'ibitem' and fieldname = '道具名称';
 set v_Field_Item = concat('F',v_Field_Item);
  
 select sequenumber into v_Field_Price from tblfieldname where tablename = 'ibitem' and fieldname = '金币单价';
 set v_Field_Price = concat('F',v_Field_Price); 
 
 select sequenumber into v_Field_BindPrice from tblfieldname where tablename = 'ibitem' and fieldname = '绑定金币单价';
 set v_Field_BindPrice = concat('F',v_Field_BindPrice); 
 
 select sequenumber into v_Field_ItemTyp from tblfieldname where tablename = 'ibitem' and fieldname = '类别';
 set v_Field_ItemTyp = concat('F',v_Field_ItemTyp);

 t_account:begin
 set @v_table_exist = 0;
 set @v_SQL = concat('select 1 into @v_table_exist from information_schema.tables where table_name = "',v_Table_Role,'" and table_schema = "',v_DatabaseName,'"');
 PREPARE stmt from @v_SQL;
 EXECUTE stmt;
 DEALLOCATE PREPARE stmt;  
 if (@v_table_exist <> 1) then
   set v_errmsg = concat(v_Table_Role,' is not exists');
   
   insert into tb_jxex_stat_err(error) values(v_errmsg);
   commit;
   leave t_account;
 end if;
 
 set @v_record_exist = 0;
 set @v_SQL = concat('select 1 into @v_record_exist from tb_jxex_stat_account where tStatDate = "',v_Date,'" limit 1');
 PREPARE stmt from @v_SQL;
 EXECUTE stmt;
 DEALLOCATE PREPARE stmt;  
 if (@v_record_exist = 1) then
   leave t_account;
 end if;
 
 if (v_Field_Account is null or v_Field_LastLogin is null) then 
   set v_errmsg = 'Field is not exist: roleinfo帐号名 or roleinfo上次登录时间';
   insert into tb_jxex_stat_err(error) values(v_errmsg);
   commit;
   leave t_account;
 end if;

 set @v_count = 0;
 set @v_SQL = concat('select count(distinct ',v_Field_Account,') into @v_count from ',v_Table_Role);
 PREPARE stmt from @v_SQL;
 EXECUTE stmt;
 DEALLOCATE PREPARE stmt;   
 select @v_count into v_TotalAccount;
   
 set v_ActiveDate = v_ActiveDate - 1;
 set @v_count = 0;
 set @v_SQL = concat('select count(distinct ',v_Field_Account,') into @v_count from ',v_Table_Role,' where (',v_Field_LastLogin,' is not null and ',v_Field_LastLogin,'>SUBDATE(STR_TO_DATE("',v_Date,'","%Y-%m-%d"),',v_ActiveDate,'))');
 PREPARE stmt from @v_SQL;
 EXECUTE stmt;
 DEALLOCATE PREPARE stmt;
 select @v_count into v_ActiveAccount;
     
 set v_LostAccount = v_TotalAccount - v_ActiveAccount;
 
 insert into tb_jxex_stat_account(ID,tStatDate,nTotalAccount,nActiveAccount,nLostAccount)
   values(null,v_Date,v_TotalAccount,v_ActiveAccount,v_LostAccount);
  commit;
 
 end t_account;
 
 t_role:begin
 set @v_table_exist = 0;
 set @v_SQL = concat('select 1 into @v_table_exist from information_schema.tables where table_name = "',v_Table_Role,'" and table_schema = "',v_DatabaseName,'"');
 PREPARE stmt from @v_SQL;
 EXECUTE stmt;
 DEALLOCATE PREPARE stmt;
 if (@v_table_exist <> 1) then
   set v_errmsg = concat(v_Table_Role,' is not exists');
   
   insert into tb_jxex_stat_err(error) values(v_errmsg);
   commit; 
   leave t_role;
 end if;
 
 set @v_record_exist = 0;
 set @v_SQL = concat('select 1 into @v_record_exist from tb_jxex_stat_role where tStatDate = "',v_Date,'" limit 1');
 PREPARE stmt from @v_SQL;
 EXECUTE stmt;
 DEALLOCATE PREPARE stmt; 
 if (@v_record_exist = 1) then
   leave t_role;
 end if;
 
 end t_role;

 t_gamemoney:begin
 set @v_table_exist = 0;
 set @v_SQL = concat('select 1 into @v_table_exist from information_schema.tables where table_name = "',v_Table_GameMoney,'" and table_schema = "',v_DatabaseName,'"');
 PREPARE stmt from @v_SQL;
 EXECUTE stmt;
 DEALLOCATE PREPARE stmt;  
 if (@v_table_exist <> 1) then
   set v_errmsg = concat(v_Table_GameMoney,' is not exists');
   
   insert into tb_jxex_stat_err(error) values(v_errmsg);
   commit;
   leave t_gamemoney;
 end if;
 
 set @v_record_exist = 0;
 set @v_SQL = concat('select 1 into @v_record_exist from tb_jxex_stat_gamemoney where tStatDate = "',v_Date,'" limit 1');
 PREPARE stmt from @v_SQL;
 EXECUTE stmt;
 DEALLOCATE PREPARE stmt;  
 if (@v_record_exist = 1) then
   leave t_gamemoney;
 end if;
 
 if (v_Field_MoneyType is null or v_Field_Value is null) then 
   set v_errmsg = 'Field is not exist:jxb总量 or jxb途径';
   insert into tb_jxex_stat_err(error) values(v_errmsg);
   commit;
   leave t_gamemoney;
 end if;
 
 set @v_SQL = concat('insert into tb_jxex_stat_gamemoney(ID,tStatDate,sName,sType,nValue) select null,STR_TO_DATE("',v_Date,'","%Y-%m-%d"),cast(',v_Field_MoneyType,' as binary),cast((substring_index(substring_index(',v_Field_MoneyType,',"]",1),"[",-1)) as binary),(CASE WHEN ',v_Field_Value,' is null THEN 0 else ',v_Field_Value,' END) nValue from ',v_Table_GameMoney); 
 PREPARE stmt from @v_SQL;
 EXECUTE stmt;
 DEALLOCATE PREPARE stmt;   
 commit; 
 end t_gamemoney;
 
 
 t_ibshop:begin
 set @v_table_exist = 0;
 set @v_SQL = concat('select 1 into @v_table_exist from information_schema.tables where table_name = "',v_Table_ibitem,'" and table_schema = "',v_DatabaseName,'"');
 PREPARE stmt from @v_SQL;
 EXECUTE stmt;
 DEALLOCATE PREPARE stmt;  
 if (@v_table_exist <> 1) then
   set v_errmsg = concat(v_Table_ibitem,' is not exists');   
   insert into tb_jxex_stat_err(error) values(v_errmsg);
   commit;
   leave t_ibshop;
 end if;
 
 set @v_table_exist = 0;
 set @v_SQL = concat('select 1 into @v_table_exist from information_schema.tables where table_name = "',v_Table_IBShop,'" and table_schema = "',v_DatabaseName,'"');
 PREPARE stmt from @v_SQL;
 EXECUTE stmt;
 DEALLOCATE PREPARE stmt;  
 if (@v_table_exist <> 1) then
   set v_errmsg = concat(v_Table_IBShop,' is not exists');
   
   insert into tb_jxex_stat_err(error) values(v_errmsg);
   commit;
   leave t_ibshop;
 end if;
 
 set @v_record_exist = 0;
 set @v_SQL = concat('select 1 into @v_record_exist from tb_jxex_stat_ibshop where tStatDate = "',v_Date,'" limit 1');
 PREPARE stmt from @v_SQL;
 EXECUTE stmt;
 DEALLOCATE PREPARE stmt;  
 if (@v_record_exist = 1) then
   leave t_ibshop;
 end if;
 
 if (v_Field_ItemName is null or v_Field_JBAmount is null or v_Field_BindJBAmount is null) then
   set v_errmsg = 'Field is not exists:ibshop道具名称 or ibshop金币销售总数 or ibshop绑定金币销售总数';
   insert into tb_jxex_stat_err(error) values(v_errmsg);
   commit;
   leave t_ibshop;
 end if;
 
 if (v_Field_Item is null or v_Field_Price is null or v_Field_BindPrice is null or v_Field_ItemTyp is null) then
   set v_errmsg = 'Field is not exists:ibitem道具名称 or ibitem金币单价 or ibitem绑定金币单价 or ibitem类别';
   insert into tb_jxex_stat_err(error) values(v_errmsg);
   commit;
   leave t_ibshop;
 end if;
  
 set @v_SQL = concat('insert into tb_jxex_stat_ibshop(ID,tStatDate,sItem_Name,nItem_Type,nJB_Amount,nBindJB_Amount,nPrice,nBindPrice) select null,STR_TO_DATE("',v_Date,'","%Y-%m-%d"),cast(i.',v_Field_Item,' as binary),(CASE WHEN i.',v_Field_ItemTyp,' is null THEN 0 else i.',v_Field_ItemTyp,' END) nItem_Type,(CASE WHEN s.',v_Field_JBAmount,' is null THEN 0 else s.',v_Field_JBAmount,' END) nJB_Amount,(CASE WHEN s.',v_Field_BindJBAmount,' is null THEN 0 else s.',v_Field_BindJBAmount,' END) nBindJB_Amount,(CASE WHEN i.',v_Field_Price,' is null THEN 0 else i.',v_Field_Price,' END) nPrice,(CASE WHEN i.',v_Field_BindPrice,' is null THEN 0 else i.',v_Field_BindPrice,' END) nBindPrice from ',v_Table_ibitem,' i left outer join ',v_Table_IBShop,' s on i.',v_Field_Item,'=s.',v_Field_ItemName); 
 PREPARE stmt from @v_SQL;
 EXECUTE stmt;
 DEALLOCATE PREPARE stmt;
 end t_ibshop;
end main;
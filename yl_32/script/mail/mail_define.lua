--
-- FileName: mail_define.lua
-- Author: 	 xiewenzhe
-- Time: 	 2011/5/25 10:32
-- Comment:	
--

if not (MODULE_GAMECLIENT or MODULE_GAMESERVER) then
	return
end

Mail.MAIL_COST = 100				-- 邮资
Mail.SEND_LIMIT = 10000				-- 一天发送邮件的限制次数(够多的了吧，超过这个可以看成是外挂了)
Mail.MAX_PLAYER_COUNT = 20			-- 最大玩家邮件个数

-- 邮件图标
Mail.ICONID_PLAYER = 1				-- 玩家邮件图标

-- 邮件错误信息
Mail.FAILREASON_UNKNOWN = 0			-- 未知原因
Mail.FAILREASON_BOXFULL = 1			-- 邮箱已满
Mail.FAILREASON_RECEIVER = 2		-- 收件人错误
Mail.FAILREASON_MONEY = 3			-- 金钱不足
Mail.FAILREASON_ITEM = 4			-- 物品错误
Mail.FAILREASON_FORBID = 5			-- 禁止发送
Mail.FAILRESON_DELIVERY = 6			-- 急件错误

-- 请求邮件列表类型
Mail.MAILLIST_PLAYER = 1			-- 玩家邮件
Mail.MAILLIST_SYSTEM = 2			-- 系统邮件

-- 邮件类型
Mail.TYPE_SYSTEM = 0				-- 系统邮件
Mail.TYPE_PLAYER = 1				-- 玩家邮件
Mail.TYPE_DELIVERY = 2				-- 急件

-- 邮件发送人类型
Mail.SEND_TYPE_PLAYER = 0			-- 玩家发送
Mail.SEND_TYPE_SYSTEM = 1			-- 系统发送

-- 邮件过滤条件的操作
Mail.COND_OP_IGNORE = 0				-- 忽略
Mail.COND_OP_GREATER = 1			-- 大于
Mail.COND_OP_GREATEREQUAL = 2		-- 大于等于
Mail.COND_OP_LESS = 3				-- 小于
Mail.COND_OP_LESSEQUAL = 4			-- 小于等于
Mail.COND_OP_EQUAL = 5				-- 等于
Mail.COND_OP_NOTEQUAL = 6			-- 不等于
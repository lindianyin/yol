-- 编写者：hejingxian
-- 版本号：37

Include('script/ai/customfunctions.lua')
Include('script/ai/ai_param.lua')
Include('script/ai/argumentstrings.lua')

local g_AIDebugInfo = {}

function AI:Setup(ai)
    local state
    local action
    local param = AI.g_AIParam[ai.nAIType]
    AI[ai.nAIType] = g_AIDebugInfo

    -- 注册自定义动作
    ai.RegisterUserAction(2044, 'CustomAction44')
    ai.RegisterUserAction(2053, 'CustomAction53')
    ai.RegisterUserAction(2192, 'CustomAction192')

    -- 巨人BOSS
    state = ai.NewState(36)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 3)

    -- 记录原点
    action = ai.NewAction(3, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(160)
    g_AIDebugInfo[3] = '[3] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(160, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(263)
    g_AIDebugInfo[160] = '[160] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(263, AI_ACTION.SET_STATE)
    action.SetParam(161) -- 待机状态
    g_AIDebugInfo[263] = '[263] SetParam(State161)'

    -- 待机状态
    state = ai.NewState(161)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 9)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 59)

    -- 选择事件发起者
    action = ai.NewAction(9, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(115, 135)
    g_AIDebugInfo[9] = '[9] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(115, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(108)
    g_AIDebugInfo[115] = '[115] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(108, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(69)
    g_AIDebugInfo[108] = '[108] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(69, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(56)
    g_AIDebugInfo[69] = '[69] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(56, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(265)
    g_AIDebugInfo[56] = '[56] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(265, AI_ACTION.SET_STATE)
    action.SetParam(119) -- 战斗状态
    g_AIDebugInfo[265] = '[265] SetParam(State119)'

    -- 战斗状态
    state = ai.NewState(119)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 40)

    -- 选择第一威胁
    action = ai.NewAction(40, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(8, 135)
    g_AIDebugInfo[40] = '[40] SELECT_FIRST_THREAT()'

    -- 设置系统无敌
    action = ai.NewAction(135, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(80)
    g_AIDebugInfo[135] = '[135] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(80, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(148, 148)
    g_AIDebugInfo[80] = '[80] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(148, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(267)
    g_AIDebugInfo[148] = '[148] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(267, AI_ACTION.SET_STATE)
    action.SetParam(65) -- 无敌返回状态
    g_AIDebugInfo[267] = '[267] SetParam(State65)'

    -- 无敌返回状态
    state = ai.NewState(65)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 96)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 96)

    -- 返回
    action = ai.NewAction(96, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(165, 165)
    g_AIDebugInfo[96] = '[96] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(165, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(269)
    g_AIDebugInfo[165] = '[165] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(269, AI_ACTION.SET_STATE)
    action.SetParam(122) -- 返回成功
    g_AIDebugInfo[269] = '[269] SetParam(State122)'

    -- 返回成功
    state = ai.NewState(122)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 146)

    -- 清除DEBUFF
    action = ai.NewAction(146, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(76)
    g_AIDebugInfo[146] = '[146] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(76, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(160)
    g_AIDebugInfo[76] = '[76] SET_SYSTEM_SHEILD(0)'

    -- 检查生命
    action = ai.NewAction(8, AI_ACTION.CHECK_LIFE)
    action.SetParam(80)
    action.SetBranch(66, 66, 61)
    g_AIDebugInfo[8] = '[8] CHECK_LIFE(80)'

    -- 设置主定时器
    action = ai.NewAction(66, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(271)
    g_AIDebugInfo[66] = '[66] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(271, AI_ACTION.SET_STATE)
    action.SetParam(51) -- 愤怒（眼着火）
    g_AIDebugInfo[271] = '[271] SetParam(State51)'

    -- 愤怒（眼着火）
    state = ai.NewState(51)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 100)

    -- Npc标准技能选择
    action = ai.NewAction(100, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(144, 66)
    g_AIDebugInfo[100] = '[100] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(144, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(44, 44)
    g_AIDebugInfo[144] = '[144] NPC_KEEP_SKILL_CAST_RANGE()'

    -- NPC喊话
    action = ai.NewAction(44, 2044)
    action.SetBranch(19)
    g_AIDebugInfo[44] = '[44] CustomAction44()'

    -- 设置主定时器
    action = ai.NewAction(19, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(273)
    g_AIDebugInfo[19] = '[19] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(273, AI_ACTION.SET_STATE)
    action.SetParam(11) -- 喊话结束
    g_AIDebugInfo[273] = '[273] SetParam(State11)'

    -- 喊话结束
    state = ai.NewState(11)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 81)

    -- 释放技能
    action = ai.NewAction(81, AI_ACTION.CAST_SKILL)
    action.SetParam(402, 1)
    action.SetBranch(83, 83)
    g_AIDebugInfo[81] = '[81] CAST_SKILL(402, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(83, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(22, 22)
    g_AIDebugInfo[83] = '[83] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(22, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(275)
    g_AIDebugInfo[22] = '[22] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(275, AI_ACTION.SET_STATE)
    action.SetParam(28) -- 待机一会
    g_AIDebugInfo[275] = '[275] SetParam(State28)'

    -- 待机一会
    state = ai.NewState(28)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 32)

    -- 设置主定时器
    action = ai.NewAction(32, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(277)
    g_AIDebugInfo[32] = '[32] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(277, AI_ACTION.SET_STATE)
    action.SetParam(70) -- 进入下一状态
    g_AIDebugInfo[277] = '[277] SetParam(State70)'

    -- 进入下一状态
    state = ai.NewState(70)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 137)

    -- 选择第一威胁
    action = ai.NewAction(137, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(125, 135)
    g_AIDebugInfo[137] = '[137] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(125, AI_ACTION.CHECK_LIFE)
    action.SetParam(60)
    action.SetBranch(10, 10, 13)
    g_AIDebugInfo[125] = '[125] CHECK_LIFE(60)'

    -- Npc标准技能选择
    action = ai.NewAction(10, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(98, 98)
    g_AIDebugInfo[10] = '[10] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(98, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(53, 53)
    g_AIDebugInfo[98] = '[98] NPC_KEEP_SKILL_CAST_RANGE()'

    -- NPC喊话
    action = ai.NewAction(53, 2053)
    action.SetBranch(38)
    g_AIDebugInfo[53] = '[53] CustomAction53()'

    -- 设置主定时器
    action = ai.NewAction(38, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(279)
    g_AIDebugInfo[38] = '[38] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(279, AI_ACTION.SET_STATE)
    action.SetParam(107) -- 喊话结束
    g_AIDebugInfo[279] = '[279] SetParam(State107)'

    -- 喊话结束
    state = ai.NewState(107)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 162)

    -- 释放技能
    action = ai.NewAction(162, AI_ACTION.CAST_SKILL)
    action.SetParam(405, 1)
    action.SetBranch(16, 16)
    g_AIDebugInfo[162] = '[162] CAST_SKILL(405, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(16, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(142, 142)
    g_AIDebugInfo[16] = '[16] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(142, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(281)
    g_AIDebugInfo[142] = '[142] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(281, AI_ACTION.SET_STATE)
    action.SetParam(77) -- 待机一会
    g_AIDebugInfo[281] = '[281] SetParam(State77)'

    -- 待机一会
    state = ai.NewState(77)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 187)

    -- 设置主定时器
    action = ai.NewAction(187, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(283)
    g_AIDebugInfo[187] = '[187] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(283, AI_ACTION.SET_STATE)
    action.SetParam(183) -- 进入下一状态
    g_AIDebugInfo[283] = '[283] SetParam(State183)'

    -- 进入下一状态
    state = ai.NewState(183)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 200)

    -- 选择第一威胁
    action = ai.NewAction(200, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(186, 135)
    g_AIDebugInfo[200] = '[200] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(186, AI_ACTION.CHECK_LIFE)
    action.SetParam(40)
    action.SetBranch(193, 193, 212)
    g_AIDebugInfo[186] = '[186] CHECK_LIFE(40)'

    -- Npc标准技能选择
    action = ai.NewAction(193, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(204, 204)
    g_AIDebugInfo[193] = '[193] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(204, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(192, 192)
    g_AIDebugInfo[204] = '[204] NPC_KEEP_SKILL_CAST_RANGE()'

    -- NPC喊话
    action = ai.NewAction(192, 2192)
    action.SetBranch(223)
    g_AIDebugInfo[192] = '[192] CustomAction192()'

    -- 设置主定时器
    action = ai.NewAction(223, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(285)
    g_AIDebugInfo[223] = '[223] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(285, AI_ACTION.SET_STATE)
    action.SetParam(203) -- 喊话结束
    g_AIDebugInfo[285] = '[285] SetParam(State203)'

    -- 喊话结束
    state = ai.NewState(203)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 191)

    -- 释放技能
    action = ai.NewAction(191, AI_ACTION.CAST_SKILL)
    action.SetParam(405, 1)
    action.SetBranch(218, 218)
    g_AIDebugInfo[191] = '[191] CAST_SKILL(405, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(218, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(190, 190)
    g_AIDebugInfo[218] = '[218] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(190, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(287)
    g_AIDebugInfo[190] = '[190] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(287, AI_ACTION.SET_STATE)
    action.SetParam(214) -- 待机一会
    g_AIDebugInfo[287] = '[287] SetParam(State214)'

    -- 待机一会
    state = ai.NewState(214)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 247)

    -- 设置主定时器
    action = ai.NewAction(247, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(289)
    g_AIDebugInfo[247] = '[247] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(289, AI_ACTION.SET_STATE)
    action.SetParam(242) -- 攻击状态2
    g_AIDebugInfo[289] = '[289] SetParam(State242)'

    -- 攻击状态2
    state = ai.NewState(242)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 239)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 135)

    -- 双选项随机
    action = ai.NewAction(239, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(90, 10)
    action.SetBranch(253, 248)
    g_AIDebugInfo[239] = '[239] RANDOM_BI_BRANCH(90, 10)'

    -- Npc标准技能选择
    action = ai.NewAction(253, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(249, 247)
    g_AIDebugInfo[253] = '[253] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(249, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(243, 243)
    g_AIDebugInfo[249] = '[249] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(243, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(247, 244)
    g_AIDebugInfo[243] = '[243] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(244, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(247, 135)
    g_AIDebugInfo[244] = '[244] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 释放技能
    action = ai.NewAction(248, AI_ACTION.CAST_SKILL)
    action.SetParam(402, 1)
    action.SetBranch(240, 244)
    g_AIDebugInfo[248] = '[248] CAST_SKILL(402, 1)'

    -- 设置主定时器
    action = ai.NewAction(240, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(247)
    g_AIDebugInfo[240] = '[240] SET_PRIMARY_TIMER(48)'

    -- 设置主定时器
    action = ai.NewAction(212, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(291)
    g_AIDebugInfo[212] = '[212] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(291, AI_ACTION.SET_STATE)
    action.SetParam(194) -- 攻击状态2
    g_AIDebugInfo[291] = '[291] SetParam(State194)'

    -- 攻击状态2
    state = ai.NewState(194)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 179)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 135)

    -- 双选项随机
    action = ai.NewAction(179, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(90, 10)
    action.SetBranch(217, 225)
    g_AIDebugInfo[179] = '[179] RANDOM_BI_BRANCH(90, 10)'

    -- 释放技能
    action = ai.NewAction(225, AI_ACTION.CAST_SKILL)
    action.SetParam(402, 1)
    action.SetBranch(216, 233)
    g_AIDebugInfo[225] = '[225] CAST_SKILL(402, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(233, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(186, 135)
    g_AIDebugInfo[233] = '[233] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 设置主定时器
    action = ai.NewAction(216, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(186)
    g_AIDebugInfo[216] = '[216] SET_PRIMARY_TIMER(48)'

    -- Npc标准技能选择
    action = ai.NewAction(217, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(228, 187)
    g_AIDebugInfo[217] = '[217] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(228, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(206, 206)
    g_AIDebugInfo[228] = '[228] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(206, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(186, 233)
    g_AIDebugInfo[206] = '[206] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(13, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(293)
    g_AIDebugInfo[13] = '[13] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(293, AI_ACTION.SET_STATE)
    action.SetParam(118) -- 攻击状态2
    g_AIDebugInfo[293] = '[293] SetParam(State118)'

    -- 攻击状态2
    state = ai.NewState(118)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 135)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 167)

    -- 双选项随机
    action = ai.NewAction(167, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(90, 10)
    action.SetBranch(151, 169)
    g_AIDebugInfo[167] = '[167] RANDOM_BI_BRANCH(90, 10)'

    -- Npc标准技能选择
    action = ai.NewAction(151, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(111, 32)
    g_AIDebugInfo[151] = '[151] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(111, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(109, 109)
    g_AIDebugInfo[111] = '[111] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(109, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(125, 110)
    g_AIDebugInfo[109] = '[109] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(110, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(125, 135)
    g_AIDebugInfo[110] = '[110] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 释放技能
    action = ai.NewAction(169, AI_ACTION.CAST_SKILL)
    action.SetParam(402, 1)
    action.SetBranch(176, 110)
    g_AIDebugInfo[169] = '[169] CAST_SKILL(402, 1)'

    -- 设置主定时器
    action = ai.NewAction(176, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(125)
    g_AIDebugInfo[176] = '[176] SET_PRIMARY_TIMER(48)'

    -- 设置主定时器
    action = ai.NewAction(61, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(295)
    g_AIDebugInfo[61] = '[61] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(295, AI_ACTION.SET_STATE)
    action.SetParam(105) -- 普通攻击
    g_AIDebugInfo[295] = '[295] SetParam(State105)'

    -- 普通攻击
    state = ai.NewState(105)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 84)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 135)

    -- Npc标准技能选择
    action = ai.NewAction(84, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(129, 56)
    g_AIDebugInfo[84] = '[84] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(129, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(93, 93)
    g_AIDebugInfo[129] = '[129] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(93, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(8, 7)
    g_AIDebugInfo[93] = '[93] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(7, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(8, 135)
    g_AIDebugInfo[7] = '[7] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 搜索敌人
    action = ai.NewAction(59, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(115, 160)
    g_AIDebugInfo[59] = '[59] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(36)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction44(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction53(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end

function AI:CustomAction192(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout3)
    return 1
end


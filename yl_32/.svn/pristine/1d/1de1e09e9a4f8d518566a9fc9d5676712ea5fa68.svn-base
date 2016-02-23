-- 编写者：hejingxian
-- 版本号：16

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
    ai.RegisterUserAction(2029, 'CustomAction29')
    ai.RegisterUserAction(2037, 'CustomAction37')
    ai.RegisterUserAction(2089, 'CustomAction89')
    ai.RegisterUserAction(2111, 'CustomAction111')
    ai.RegisterUserAction(2155, 'CustomAction155')

    -- 副本-巨灵神
    state = ai.NewState(126)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 129)

    -- 记录原点
    action = ai.NewAction(129, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(3)
    g_AIDebugInfo[129] = '[129] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(3, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(157)
    g_AIDebugInfo[3] = '[3] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(157, AI_ACTION.SET_STATE)
    action.SetParam(87) -- 待机状态
    g_AIDebugInfo[157] = '[157] SetParam(State87)'

    -- 待机状态
    state = ai.NewState(87)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 15)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 119)

    -- 选择事件发起者
    action = ai.NewAction(15, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(17, 3)
    g_AIDebugInfo[15] = '[15] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(17, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(52)
    g_AIDebugInfo[17] = '[17] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(52, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(115)
    g_AIDebugInfo[52] = '[52] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(115, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(130)
    g_AIDebugInfo[115] = '[115] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(130, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(159)
    g_AIDebugInfo[130] = '[130] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(159, AI_ACTION.SET_STATE)
    action.SetParam(47) -- 战斗状态
    g_AIDebugInfo[159] = '[159] SetParam(State47)'

    -- 战斗状态
    state = ai.NewState(47)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 49)

    -- 选择第一威胁
    action = ai.NewAction(49, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(63, 3)
    g_AIDebugInfo[49] = '[49] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(63, AI_ACTION.CHECK_LIFE)
    action.SetParam(60)
    action.SetBranch(37, 37, 83)
    g_AIDebugInfo[63] = '[63] CHECK_LIFE(60)'

    -- NPC喊话
    action = ai.NewAction(37, 2037)
    action.SetBranch(113)
    g_AIDebugInfo[37] = '[37] CustomAction37()'

    -- 设置主定时器
    action = ai.NewAction(113, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(161)
    g_AIDebugInfo[113] = '[113] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(161, AI_ACTION.SET_STATE)
    action.SetParam(80) -- 喊话结束
    g_AIDebugInfo[161] = '[161] SetParam(State80)'

    -- 喊话结束
    state = ai.NewState(80)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 111)

    -- NPC变大
    action = ai.NewAction(111, 2111)
    action.SetBranch(65)
    g_AIDebugInfo[111] = '[111] CustomAction111()'

    -- 设置主定时器
    action = ai.NewAction(65, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(163)
    g_AIDebugInfo[65] = '[65] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(163, AI_ACTION.SET_STATE)
    action.SetParam(10) -- 变大结束
    g_AIDebugInfo[163] = '[163] SetParam(State10)'

    -- 变大结束
    state = ai.NewState(10)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 22)

    -- 设置主定时器
    action = ai.NewAction(22, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(165)
    g_AIDebugInfo[22] = '[22] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(165, AI_ACTION.SET_STATE)
    action.SetParam(102) -- 暴力状态
    g_AIDebugInfo[165] = '[165] SetParam(State102)'

    -- 暴力状态
    state = ai.NewState(102)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 26)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 62)

    -- 设置系统无敌
    action = ai.NewAction(26, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(76)
    g_AIDebugInfo[26] = '[26] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(76, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(5, 5)
    g_AIDebugInfo[76] = '[76] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(5, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(155)
    g_AIDebugInfo[5] = '[5] SET_PRIMARY_TIMER(8)'

    -- NPC变小
    action = ai.NewAction(155, 2155)
    action.SetBranch(167)
    g_AIDebugInfo[155] = '[155] CustomAction155()'

    -- 设置状态
    action = ai.NewAction(167, AI_ACTION.SET_STATE)
    action.SetParam(135) -- 无敌返回状态
    g_AIDebugInfo[167] = '[167] SetParam(State135)'

    -- 无敌返回状态
    state = ai.NewState(135)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 123)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 123)

    -- 返回
    action = ai.NewAction(123, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(19, 19)
    g_AIDebugInfo[123] = '[123] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(19, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(169)
    g_AIDebugInfo[19] = '[19] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(169, AI_ACTION.SET_STATE)
    action.SetParam(81) -- 返回成功
    g_AIDebugInfo[169] = '[169] SetParam(State81)'

    -- 返回成功
    state = ai.NewState(81)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 68)

    -- 清除DEBUFF
    action = ai.NewAction(68, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(7)
    g_AIDebugInfo[68] = '[68] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(7, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(3)
    g_AIDebugInfo[7] = '[7] SET_SYSTEM_SHEILD(0)'

    -- 双选项随机
    action = ai.NewAction(62, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(20, 80)
    action.SetBranch(29, 79)
    g_AIDebugInfo[62] = '[62] RANDOM_BI_BRANCH(20, 80)'

    -- NPC喊话
    action = ai.NewAction(29, 2029)
    action.SetBranch(131)
    g_AIDebugInfo[29] = '[29] CustomAction29()'

    -- 设置主定时器
    action = ai.NewAction(131, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(171)
    g_AIDebugInfo[131] = '[131] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(171, AI_ACTION.SET_STATE)
    action.SetParam(136) -- 喊话结束
    g_AIDebugInfo[171] = '[171] SetParam(State136)'

    -- 喊话结束
    state = ai.NewState(136)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 79)

    -- Npc标准技能选择
    action = ai.NewAction(79, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(145, 22)
    g_AIDebugInfo[79] = '[79] NPC_STANDARD_SKILL_SELECTOR()'

    -- 双选项随机
    action = ai.NewAction(145, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(70, 30)
    action.SetBranch(38, 147)
    g_AIDebugInfo[145] = '[145] RANDOM_BI_BRANCH(70, 30)'

    -- Npc保持技能释放距离
    action = ai.NewAction(38, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(42, 16)
    g_AIDebugInfo[38] = '[38] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(16, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(173)
    g_AIDebugInfo[16] = '[16] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(173, AI_ACTION.SET_STATE)
    action.SetParam(101) -- 追击状态
    g_AIDebugInfo[173] = '[173] SetParam(State101)'

    -- 追击状态
    state = ai.NewState(101)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 4)

    -- 比较返回点距离
    action = ai.NewAction(4, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(26, 38, 38)
    g_AIDebugInfo[4] = '[4] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(42, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(22, 35)
    g_AIDebugInfo[42] = '[42] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(35, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(50)
    action.SetBranch(22, 26)
    g_AIDebugInfo[35] = '[35] CHECK_SKILL_FAILED_COUNTER(50)'

    -- 释放技能
    action = ai.NewAction(147, AI_ACTION.CAST_SKILL)
    action.SetParam(424, 1)
    action.SetBranch(144, 35)
    g_AIDebugInfo[147] = '[147] CAST_SKILL(424, 1)'

    -- 设置主定时器
    action = ai.NewAction(144, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(175)
    g_AIDebugInfo[144] = '[144] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(175, AI_ACTION.SET_STATE)
    action.SetParam(152) -- 旋转结束
    g_AIDebugInfo[175] = '[175] SetParam(State152)'

    -- 旋转结束
    state = ai.NewState(152)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 22)

    -- 设置主定时器
    action = ai.NewAction(83, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(177)
    g_AIDebugInfo[83] = '[83] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(177, AI_ACTION.SET_STATE)
    action.SetParam(40) -- 普通攻击
    g_AIDebugInfo[177] = '[177] SetParam(State40)'

    -- 普通攻击
    state = ai.NewState(40)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 26)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 110)

    -- 双选项随机
    action = ai.NewAction(110, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(10, 90)
    action.SetBranch(89, 69)
    g_AIDebugInfo[110] = '[110] RANDOM_BI_BRANCH(10, 90)'

    -- NPC喊话
    action = ai.NewAction(89, 2089)
    action.SetBranch(6)
    g_AIDebugInfo[89] = '[89] CustomAction89()'

    -- 设置主定时器
    action = ai.NewAction(6, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(179)
    g_AIDebugInfo[6] = '[6] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(179, AI_ACTION.SET_STATE)
    action.SetParam(73) -- 喊话结束
    g_AIDebugInfo[179] = '[179] SetParam(State73)'

    -- 喊话结束
    state = ai.NewState(73)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 69)

    -- Npc标准技能选择
    action = ai.NewAction(69, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(127, 130)
    g_AIDebugInfo[69] = '[69] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(127, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(67, 133)
    g_AIDebugInfo[127] = '[127] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(67, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(63, 132)
    g_AIDebugInfo[67] = '[67] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(132, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(50)
    action.SetBranch(63, 26)
    g_AIDebugInfo[132] = '[132] CHECK_SKILL_FAILED_COUNTER(50)'

    -- 设置主定时器
    action = ai.NewAction(133, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(181)
    g_AIDebugInfo[133] = '[133] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(181, AI_ACTION.SET_STATE)
    action.SetParam(104) -- 追击状态
    g_AIDebugInfo[181] = '[181] SetParam(State104)'

    -- 追击状态
    state = ai.NewState(104)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 139)

    -- 比较返回点距离
    action = ai.NewAction(139, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(26, 127, 127)
    g_AIDebugInfo[139] = '[139] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 搜索敌人
    action = ai.NewAction(119, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(17, 3)
    g_AIDebugInfo[119] = '[119] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(126)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction37(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction111(pNpc, ai)
    -- 输入自定义动作实现脚本
    Dialog:StartScaleAnimationToNearBy(pNpc.dwId,150)
    return 1
end

function AI:CustomAction155(pNpc, ai)
    -- 输入自定义动作实现脚本
    Dialog:StartScaleAnimationToNearBy(pNpc.dwId,100)
    return 1
end

function AI:CustomAction29(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout3)
    return 1
end



function AI:CustomAction89(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end


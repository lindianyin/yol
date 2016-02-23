-- 编写者：hejingxian
-- 版本号：23

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
    ai.RegisterUserAction(2125, 'CustomAction125')
    ai.RegisterUserAction(2148, 'CustomAction148')

    -- 初始
    state = ai.NewState(72)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 50)

    -- 记录原点
    action = ai.NewAction(50, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(58)
    g_AIDebugInfo[50] = '[50] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(58, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(174)
    g_AIDebugInfo[58] = '[58] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(174, AI_ACTION.SET_STATE)
    action.SetParam(67) -- 待机状态
    g_AIDebugInfo[174] = '[174] SetParam(State67)'

    -- 待机状态
    state = ai.NewState(67)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 155)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 100)

    -- 搜索敌人
    action = ai.NewAction(155, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(133, 58)
    g_AIDebugInfo[155] = '[155] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(133, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(8)
    g_AIDebugInfo[133] = '[133] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(8, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(123)
    g_AIDebugInfo[8] = '[8] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(123, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(82)
    g_AIDebugInfo[123] = '[123] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(82, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(176)
    g_AIDebugInfo[82] = '[82] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(176, AI_ACTION.SET_STATE)
    action.SetParam(164) -- 战斗状态
    g_AIDebugInfo[176] = '[176] SetParam(State164)'

    -- 战斗状态
    state = ai.NewState(164)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 37)

    -- 选择第一威胁
    action = ai.NewAction(37, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(24, 35)
    g_AIDebugInfo[37] = '[37] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(24, AI_ACTION.CHECK_LIFE)
    action.SetParam(80)
    action.SetBranch(90, 90, 109)
    g_AIDebugInfo[24] = '[24] CHECK_LIFE(80)'

    -- 设置主定时器
    action = ai.NewAction(90, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(178)
    g_AIDebugInfo[90] = '[90] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(178, AI_ACTION.SET_STATE)
    action.SetParam(27) -- 发飙状态1
    g_AIDebugInfo[178] = '[178] SetParam(State27)'

    -- 发飙状态1
    state = ai.NewState(27)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 124)

    -- Npc标准技能选择
    action = ai.NewAction(124, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(129, 90)
    g_AIDebugInfo[124] = '[124] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(129, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(125, 125)
    g_AIDebugInfo[129] = '[129] NPC_KEEP_SKILL_CAST_RANGE()'

    -- NPC喊话
    action = ai.NewAction(125, 2125)
    action.SetBranch(134)
    g_AIDebugInfo[125] = '[125] CustomAction125()'

    -- 设置主定时器
    action = ai.NewAction(134, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(180)
    g_AIDebugInfo[134] = '[134] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(180, AI_ACTION.SET_STATE)
    action.SetParam(30) -- 喊话结束
    g_AIDebugInfo[180] = '[180] SetParam(State30)'

    -- 喊话结束
    state = ai.NewState(30)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 7)

    -- 释放技能
    action = ai.NewAction(7, AI_ACTION.CAST_SKILL)
    action.SetParam(376, 1)
    action.SetBranch(94, 94)
    g_AIDebugInfo[7] = '[7] CAST_SKILL(376, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(94, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(5, 5)
    g_AIDebugInfo[94] = '[94] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(5, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(182)
    g_AIDebugInfo[5] = '[5] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(182, AI_ACTION.SET_STATE)
    action.SetParam(168) -- 待机一会
    g_AIDebugInfo[182] = '[182] SetParam(State168)'

    -- 待机一会
    state = ai.NewState(168)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 130)

    -- 设置主定时器
    action = ai.NewAction(130, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(184)
    g_AIDebugInfo[130] = '[130] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(184, AI_ACTION.SET_STATE)
    action.SetParam(88) -- 进入下一状态
    g_AIDebugInfo[184] = '[184] SetParam(State88)'

    -- 进入下一状态
    state = ai.NewState(88)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 98)

    -- 选择第一威胁
    action = ai.NewAction(98, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(1, 35)
    g_AIDebugInfo[98] = '[98] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(1, AI_ACTION.CHECK_LIFE)
    action.SetParam(40)
    action.SetBranch(53, 53, 118)
    g_AIDebugInfo[1] = '[1] CHECK_LIFE(40)'

    -- Npc标准技能选择
    action = ai.NewAction(53, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(112, 112)
    g_AIDebugInfo[53] = '[53] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(112, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(148, 148)
    g_AIDebugInfo[112] = '[112] NPC_KEEP_SKILL_CAST_RANGE()'

    -- NPC喊话
    action = ai.NewAction(148, 2148)
    action.SetBranch(3)
    g_AIDebugInfo[148] = '[148] CustomAction148()'

    -- 设置主定时器
    action = ai.NewAction(3, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(186)
    g_AIDebugInfo[3] = '[3] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(186, AI_ACTION.SET_STATE)
    action.SetParam(14) -- 喊话结束
    g_AIDebugInfo[186] = '[186] SetParam(State14)'

    -- 喊话结束
    state = ai.NewState(14)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 122)

    -- 释放技能
    action = ai.NewAction(122, AI_ACTION.CAST_SKILL)
    action.SetParam(376, 1)
    action.SetBranch(64, 64)
    g_AIDebugInfo[122] = '[122] CAST_SKILL(376, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(64, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(149, 149)
    g_AIDebugInfo[64] = '[64] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(149, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(188)
    g_AIDebugInfo[149] = '[149] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(188, AI_ACTION.SET_STATE)
    action.SetParam(116) -- 待机一会
    g_AIDebugInfo[188] = '[188] SetParam(State116)'

    -- 待机一会
    state = ai.NewState(116)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 87)

    -- 设置主定时器
    action = ai.NewAction(87, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(25)
    action.SetBranch(190)
    g_AIDebugInfo[87] = '[87] SET_PRIMARY_TIMER(25)'

    -- 设置状态
    action = ai.NewAction(190, AI_ACTION.SET_STATE)
    action.SetParam(11) -- 攻击状态2
    g_AIDebugInfo[190] = '[190] SetParam(State11)'

    -- 攻击状态2
    state = ai.NewState(11)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 35)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 111)

    -- 设置系统无敌
    action = ai.NewAction(35, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(102)
    g_AIDebugInfo[35] = '[35] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(102, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(29, 29)
    g_AIDebugInfo[102] = '[102] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(29, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(192)
    g_AIDebugInfo[29] = '[29] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(192, AI_ACTION.SET_STATE)
    action.SetParam(170) -- 无敌返回状态
    g_AIDebugInfo[192] = '[192] SetParam(State170)'

    -- 无敌返回状态
    state = ai.NewState(170)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 13)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 13)

    -- 返回
    action = ai.NewAction(13, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(121, 121)
    g_AIDebugInfo[13] = '[13] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(121, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(194)
    g_AIDebugInfo[121] = '[121] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(194, AI_ACTION.SET_STATE)
    action.SetParam(49) -- 返回成功
    g_AIDebugInfo[194] = '[194] SetParam(State49)'

    -- 返回成功
    state = ai.NewState(49)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 156)

    -- 清除DEBUFF
    action = ai.NewAction(156, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(57)
    g_AIDebugInfo[156] = '[156] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(57, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(58)
    g_AIDebugInfo[57] = '[57] SET_SYSTEM_SHEILD(0)'

    -- Npc标准技能选择
    action = ai.NewAction(111, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(47, 87)
    g_AIDebugInfo[111] = '[111] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(47, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(136, 136)
    g_AIDebugInfo[47] = '[47] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(136, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(87, 76)
    g_AIDebugInfo[136] = '[136] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(76, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(87, 35)
    g_AIDebugInfo[76] = '[76] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 设置主定时器
    action = ai.NewAction(118, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(196)
    g_AIDebugInfo[118] = '[118] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(196, AI_ACTION.SET_STATE)
    action.SetParam(139) -- 攻击状态2
    g_AIDebugInfo[196] = '[196] SetParam(State139)'

    -- 攻击状态2
    state = ai.NewState(139)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 97)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 35)

    -- Npc标准技能选择
    action = ai.NewAction(97, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(65, 130)
    g_AIDebugInfo[97] = '[97] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(65, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(31, 31)
    g_AIDebugInfo[65] = '[65] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(31, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(1, 128)
    g_AIDebugInfo[31] = '[31] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(128, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(1, 35)
    g_AIDebugInfo[128] = '[128] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 设置主定时器
    action = ai.NewAction(109, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(198)
    g_AIDebugInfo[109] = '[109] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(198, AI_ACTION.SET_STATE)
    action.SetParam(86) -- 攻击状态1
    g_AIDebugInfo[198] = '[198] SetParam(State86)'

    -- 攻击状态1
    state = ai.NewState(86)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 131)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 35)

    -- Npc标准技能选择
    action = ai.NewAction(131, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(73, 82)
    g_AIDebugInfo[131] = '[131] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(73, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(171, 171)
    g_AIDebugInfo[73] = '[73] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(171, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(24, 103)
    g_AIDebugInfo[171] = '[171] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(103, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(20)
    action.SetBranch(24, 35)
    g_AIDebugInfo[103] = '[103] CHECK_SKILL_FAILED_COUNTER(20)'

    -- 选择事件发起者
    action = ai.NewAction(100, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(133, 35)
    g_AIDebugInfo[100] = '[100] SELECT_EVENT_SRC()'

    ai.SetInitState(72)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction125(pNpc, ai)
    -- 输入自定义动作实现脚本
    	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction148(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end


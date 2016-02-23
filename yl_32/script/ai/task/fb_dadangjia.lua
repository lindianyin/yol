-- 编写者：hejingxian
-- 版本号：30

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
    ai.RegisterUserAction(2092, 'AI:CustomAction92')
    ai.RegisterUserAction(2135, 'AI:CustomAction135')
    ai.RegisterUserAction(2145, 'AI:CustomAction145')
    ai.RegisterUserAction(2158, 'AI:CustomAction158')

    -- 副本-大当家
    state = ai.NewState(27)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 22)

    -- 记录原点
    action = ai.NewAction(22, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(13)
    g_AIDebugInfo[22] = '[22] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(13, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(162)
    g_AIDebugInfo[13] = '[13] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(162, AI_ACTION.SET_STATE)
    action.SetParam(12) -- 待机状态
    g_AIDebugInfo[162] = '[162] SetParam(State12)'

    -- 待机状态
    state = ai.NewState(12)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 26)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 10)

    -- 选择事件发起者
    action = ai.NewAction(26, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(15, 13)
    g_AIDebugInfo[26] = '[26] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(15, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(5)
    g_AIDebugInfo[15] = '[15] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(5, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(23)
    g_AIDebugInfo[5] = '[5] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(23, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(7)
    g_AIDebugInfo[23] = '[23] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(7, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(164)
    g_AIDebugInfo[7] = '[7] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(164, AI_ACTION.SET_STATE)
    action.SetParam(18) -- 战斗状态
    g_AIDebugInfo[164] = '[164] SetParam(State18)'

    -- 战斗状态
    state = ai.NewState(18)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 24)

    -- 选择第一威胁
    action = ai.NewAction(24, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(29, 13)
    g_AIDebugInfo[24] = '[24] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(29, AI_ACTION.CHECK_LIFE)
    action.SetParam(70)
    action.SetBranch(92, 92, 58)
    g_AIDebugInfo[29] = '[29] CHECK_LIFE(70)'

    -- NPC喊话
    action = ai.NewAction(92, 2092)
    action.SetBranch(86)
    g_AIDebugInfo[92] = '[92] AI:CustomAction92()'

    -- 设置主定时器
    action = ai.NewAction(86, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(166)
    g_AIDebugInfo[86] = '[86] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(166, AI_ACTION.SET_STATE)
    action.SetParam(78) -- 喊话结束
    g_AIDebugInfo[166] = '[166] SetParam(State78)'

    -- 喊话结束
    state = ai.NewState(78)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 158)

    -- NPC变大
    action = ai.NewAction(158, 2158)
    action.SetBranch(84)
    g_AIDebugInfo[158] = '[158] AI:CustomAction158()'

    -- 释放技能
    action = ai.NewAction(84, AI_ACTION.CAST_SKILL)
    action.SetParam(364, 1)
    action.SetBranch(90, 90)
    g_AIDebugInfo[84] = '[84] CAST_SKILL(364, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(90, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(88, 88)
    g_AIDebugInfo[90] = '[90] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(88, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(168)
    g_AIDebugInfo[88] = '[88] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(168, AI_ACTION.SET_STATE)
    action.SetParam(89) -- 变大结束
    g_AIDebugInfo[168] = '[168] SetParam(State89)'

    -- 变大结束
    state = ai.NewState(89)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 106)

    -- 设置主定时器
    action = ai.NewAction(106, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(170)
    g_AIDebugInfo[106] = '[106] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(170, AI_ACTION.SET_STATE)
    action.SetParam(98) -- 暴力状态
    g_AIDebugInfo[170] = '[170] SetParam(State98)'

    -- 暴力状态
    state = ai.NewState(98)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 146)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 50)

    -- 双选项随机
    action = ai.NewAction(146, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(10, 90)
    action.SetBranch(145, 113)
    g_AIDebugInfo[146] = '[146] RANDOM_BI_BRANCH(10, 90)'

    -- Npc标准技能选择
    action = ai.NewAction(113, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(99, 106)
    g_AIDebugInfo[113] = '[113] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(99, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(115, 100)
    g_AIDebugInfo[99] = '[99] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(100, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(172)
    g_AIDebugInfo[100] = '[100] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(172, AI_ACTION.SET_STATE)
    action.SetParam(114) -- 追击状态
    g_AIDebugInfo[172] = '[172] SetParam(State114)'

    -- 追击状态
    state = ai.NewState(114)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 119)

    -- 比较返回点距离
    action = ai.NewAction(119, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(50, 99, 99)
    g_AIDebugInfo[119] = '[119] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 设置系统无敌
    action = ai.NewAction(50, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(39)
    g_AIDebugInfo[50] = '[50] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(39, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(33, 33)
    g_AIDebugInfo[39] = '[39] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(33, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(174)
    g_AIDebugInfo[33] = '[33] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(174, AI_ACTION.SET_STATE)
    action.SetParam(34) -- 无敌返回状态
    g_AIDebugInfo[174] = '[174] SetParam(State34)'

    -- 无敌返回状态
    state = ai.NewState(34)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 41)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 41)

    -- 返回
    action = ai.NewAction(41, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(32, 32)
    g_AIDebugInfo[41] = '[41] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(32, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(176)
    g_AIDebugInfo[32] = '[32] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(176, AI_ACTION.SET_STATE)
    action.SetParam(49) -- 返回成功
    g_AIDebugInfo[176] = '[176] SetParam(State49)'

    -- 返回成功
    state = ai.NewState(49)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 35)

    -- 清除DEBUFF
    action = ai.NewAction(35, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(43)
    g_AIDebugInfo[35] = '[35] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(43, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(13)
    g_AIDebugInfo[43] = '[43] SET_SYSTEM_SHEILD(0)'

    -- Npc释放选定技能
    action = ai.NewAction(115, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(106, 96)
    g_AIDebugInfo[115] = '[115] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(96, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(50)
    action.SetBranch(106, 50)
    g_AIDebugInfo[96] = '[96] CHECK_SKILL_FAILED_COUNTER(50)'

    -- NPC喊话
    action = ai.NewAction(145, 2145)
    action.SetBranch(147)
    g_AIDebugInfo[145] = '[145] AI:CustomAction145()'

    -- 设置主定时器
    action = ai.NewAction(147, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(178)
    g_AIDebugInfo[147] = '[147] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(178, AI_ACTION.SET_STATE)
    action.SetParam(144) -- 喊话结束
    g_AIDebugInfo[178] = '[178] SetParam(State144)'

    -- 喊话结束
    state = ai.NewState(144)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 113)

    -- 设置主定时器
    action = ai.NewAction(58, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(180)
    g_AIDebugInfo[58] = '[58] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(180, AI_ACTION.SET_STATE)
    action.SetParam(56) -- 普通攻击
    g_AIDebugInfo[180] = '[180] SetParam(State56)'

    -- 普通攻击
    state = ai.NewState(56)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 140)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 50)

    -- 双选项随机
    action = ai.NewAction(140, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(10, 90)
    action.SetBranch(135, 60)
    g_AIDebugInfo[140] = '[140] RANDOM_BI_BRANCH(10, 90)'

    -- NPC喊话
    action = ai.NewAction(135, 2135)
    action.SetBranch(136)
    g_AIDebugInfo[135] = '[135] AI:CustomAction135()'

    -- 设置主定时器
    action = ai.NewAction(136, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(182)
    g_AIDebugInfo[136] = '[136] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(182, AI_ACTION.SET_STATE)
    action.SetParam(139) -- 喊话结束
    g_AIDebugInfo[182] = '[182] SetParam(State139)'

    -- 喊话结束
    state = ai.NewState(139)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 60)

    -- Npc标准技能选择
    action = ai.NewAction(60, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(55, 7)
    g_AIDebugInfo[60] = '[60] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(55, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(62, 59)
    g_AIDebugInfo[55] = '[55] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(59, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(184)
    g_AIDebugInfo[59] = '[59] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(184, AI_ACTION.SET_STATE)
    action.SetParam(70) -- 追击状态
    g_AIDebugInfo[184] = '[184] SetParam(State70)'

    -- 追击状态
    state = ai.NewState(70)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 66)

    -- 比较返回点距离
    action = ai.NewAction(66, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(50, 55, 55)
    g_AIDebugInfo[66] = '[66] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(62, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(29, 68)
    g_AIDebugInfo[62] = '[62] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(68, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(50)
    action.SetBranch(29, 50)
    g_AIDebugInfo[68] = '[68] CHECK_SKILL_FAILED_COUNTER(50)'

    -- 搜索敌人
    action = ai.NewAction(10, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(15, 13)
    g_AIDebugInfo[10] = '[10] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(27)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction158(pNpc, talkingStr)
    -- 输入自定义动作实现脚本
    Dialog:StartScaleAnimationToNearBy(pNpc.dwId,150)
    return 1
end

function AI:CustomAction92(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction145(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout3)
    return 1
end

function AI:CustomAction135(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end


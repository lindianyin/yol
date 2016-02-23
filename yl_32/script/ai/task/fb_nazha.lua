-- 编写者：hejingxian
-- 版本号：7

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
    ai.RegisterUserAction(2082, 'CustomAction82')
    ai.RegisterUserAction(2118, 'CustomAction118')
    ai.RegisterUserAction(2122, 'CustomAction122')

    -- 副本-哪咤
    state = ai.NewState(136)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 87)

    -- 记录原点
    action = ai.NewAction(87, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(113)
    g_AIDebugInfo[87] = '[87] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(113, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(150)
    g_AIDebugInfo[113] = '[113] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(150, AI_ACTION.SET_STATE)
    action.SetParam(2) -- 待机状态
    g_AIDebugInfo[150] = '[150] SetParam(State2)'

    -- 待机状态
    state = ai.NewState(2)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 28)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 115)

    -- 搜索敌人
    action = ai.NewAction(28, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(39, 113)
    g_AIDebugInfo[28] = '[28] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(39, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(64)
    g_AIDebugInfo[39] = '[39] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(64, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(38)
    g_AIDebugInfo[64] = '[64] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(38, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(50)
    g_AIDebugInfo[38] = '[38] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(50, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(152)
    g_AIDebugInfo[50] = '[50] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(152, AI_ACTION.SET_STATE)
    action.SetParam(104) -- 战斗状态
    g_AIDebugInfo[152] = '[152] SetParam(State104)'

    -- 战斗状态
    state = ai.NewState(104)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 52)

    -- 选择第一威胁
    action = ai.NewAction(52, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(5, 113)
    g_AIDebugInfo[52] = '[52] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(5, AI_ACTION.CHECK_LIFE)
    action.SetParam(50)
    action.SetBranch(82, 82, 74)
    g_AIDebugInfo[5] = '[5] CHECK_LIFE(50)'

    -- NPC喊话
    action = ai.NewAction(82, 2082)
    action.SetBranch(8)
    g_AIDebugInfo[82] = '[82] CustomAction82()'

    -- 设置主定时器
    action = ai.NewAction(8, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(154)
    g_AIDebugInfo[8] = '[8] SET_PRIMARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(154, AI_ACTION.SET_STATE)
    action.SetParam(65) -- 喊话结束
    g_AIDebugInfo[154] = '[154] SetParam(State65)'

    -- 喊话结束
    state = ai.NewState(65)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 137)

    -- 释放技能
    action = ai.NewAction(137, AI_ACTION.CAST_SKILL)
    action.SetParam(421, 1)
    action.SetBranch(128, 128)
    g_AIDebugInfo[137] = '[137] CAST_SKILL(421, 1)'

    -- 设置主定时器
    action = ai.NewAction(128, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(156)
    g_AIDebugInfo[128] = '[128] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(156, AI_ACTION.SET_STATE)
    action.SetParam(73) -- 变大结束
    g_AIDebugInfo[156] = '[156] SetParam(State73)'

    -- 变大结束
    state = ai.NewState(73)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 9)

    -- 设置主定时器
    action = ai.NewAction(9, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(158)
    g_AIDebugInfo[9] = '[9] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(158, AI_ACTION.SET_STATE)
    action.SetParam(70) -- 暴力状态
    g_AIDebugInfo[158] = '[158] SetParam(State70)'

    -- 暴力状态
    state = ai.NewState(70)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 124)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 42)

    -- 双选项随机
    action = ai.NewAction(124, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(10, 90)
    action.SetBranch(122, 57)
    g_AIDebugInfo[124] = '[124] RANDOM_BI_BRANCH(10, 90)'

    -- Npc标准技能选择
    action = ai.NewAction(57, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(140, 9)
    g_AIDebugInfo[57] = '[57] NPC_STANDARD_SKILL_SELECTOR()'

    -- 双选项随机
    action = ai.NewAction(140, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(80, 20)
    action.SetBranch(40, 142)
    g_AIDebugInfo[140] = '[140] RANDOM_BI_BRANCH(80, 20)'

    -- Npc保持技能释放距离
    action = ai.NewAction(40, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(13, 88)
    g_AIDebugInfo[40] = '[40] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(88, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(160)
    g_AIDebugInfo[88] = '[88] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(160, AI_ACTION.SET_STATE)
    action.SetParam(4) -- 追击状态
    g_AIDebugInfo[160] = '[160] SetParam(State4)'

    -- 追击状态
    state = ai.NewState(4)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 23)

    -- 比较返回点距离
    action = ai.NewAction(23, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(42, 40, 40)
    g_AIDebugInfo[23] = '[23] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 设置系统无敌
    action = ai.NewAction(42, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(95)
    g_AIDebugInfo[42] = '[42] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(95, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(58, 58)
    g_AIDebugInfo[95] = '[95] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(58, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(162)
    g_AIDebugInfo[58] = '[58] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(162, AI_ACTION.SET_STATE)
    action.SetParam(105) -- 无敌返回状态
    g_AIDebugInfo[162] = '[162] SetParam(State105)'

    -- 无敌返回状态
    state = ai.NewState(105)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 32)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 32)

    -- 返回
    action = ai.NewAction(32, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(75, 75)
    g_AIDebugInfo[32] = '[32] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(75, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(164)
    g_AIDebugInfo[75] = '[75] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(164, AI_ACTION.SET_STATE)
    action.SetParam(101) -- 返回成功
    g_AIDebugInfo[164] = '[164] SetParam(State101)'

    -- 返回成功
    state = ai.NewState(101)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 20)

    -- 清除DEBUFF
    action = ai.NewAction(20, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(66)
    g_AIDebugInfo[20] = '[20] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(66, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(113)
    g_AIDebugInfo[66] = '[66] SET_SYSTEM_SHEILD(0)'

    -- Npc释放选定技能
    action = ai.NewAction(13, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(9, 119)
    g_AIDebugInfo[13] = '[13] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(119, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(50)
    action.SetBranch(9, 42)
    g_AIDebugInfo[119] = '[119] CHECK_SKILL_FAILED_COUNTER(50)'

    -- 释放技能
    action = ai.NewAction(142, AI_ACTION.CAST_SKILL)
    action.SetParam(422, 1)
    action.SetBranch(144, 119)
    g_AIDebugInfo[142] = '[142] CAST_SKILL(422, 1)'

    -- 设置主定时器
    action = ai.NewAction(144, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(166)
    g_AIDebugInfo[144] = '[144] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(166, AI_ACTION.SET_STATE)
    action.SetParam(145) -- 大招结束
    g_AIDebugInfo[166] = '[166] SetParam(State145)'

    -- 大招结束
    state = ai.NewState(145)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 9)

    -- NPC喊话
    action = ai.NewAction(122, 2122)
    action.SetBranch(16)
    g_AIDebugInfo[122] = '[122] CustomAction122()'

    -- 设置主定时器
    action = ai.NewAction(16, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(168)
    g_AIDebugInfo[16] = '[16] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(168, AI_ACTION.SET_STATE)
    action.SetParam(102) -- 喊话结束
    g_AIDebugInfo[168] = '[168] SetParam(State102)'

    -- 喊话结束
    state = ai.NewState(102)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 57)

    -- 设置主定时器
    action = ai.NewAction(74, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(170)
    g_AIDebugInfo[74] = '[74] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(170, AI_ACTION.SET_STATE)
    action.SetParam(46) -- 普通攻击
    g_AIDebugInfo[170] = '[170] SetParam(State46)'

    -- 普通攻击
    state = ai.NewState(46)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 125)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 42)

    -- 双选项随机
    action = ai.NewAction(125, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(10, 90)
    action.SetBranch(118, 92)
    g_AIDebugInfo[125] = '[125] RANDOM_BI_BRANCH(10, 90)'

    -- Npc标准技能选择
    action = ai.NewAction(92, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(80, 50)
    g_AIDebugInfo[92] = '[92] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(80, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(48, 93)
    g_AIDebugInfo[80] = '[80] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(93, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(172)
    g_AIDebugInfo[93] = '[93] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(172, AI_ACTION.SET_STATE)
    action.SetParam(21) -- 追击状态
    g_AIDebugInfo[172] = '[172] SetParam(State21)'

    -- 追击状态
    state = ai.NewState(21)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 29)

    -- 比较返回点距离
    action = ai.NewAction(29, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(42, 80, 80)
    g_AIDebugInfo[29] = '[29] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(48, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(5, 91)
    g_AIDebugInfo[48] = '[48] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(91, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(50)
    action.SetBranch(5, 42)
    g_AIDebugInfo[91] = '[91] CHECK_SKILL_FAILED_COUNTER(50)'

    -- NPC喊话
    action = ai.NewAction(118, 2118)
    action.SetBranch(55)
    g_AIDebugInfo[118] = '[118] CustomAction118()'

    -- 设置主定时器
    action = ai.NewAction(55, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(174)
    g_AIDebugInfo[55] = '[55] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(174, AI_ACTION.SET_STATE)
    action.SetParam(81) -- 喊话结束
    g_AIDebugInfo[174] = '[174] SetParam(State81)'

    -- 喊话结束
    state = ai.NewState(81)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 92)

    -- 选择事件发起者
    action = ai.NewAction(115, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(39, 113)
    g_AIDebugInfo[115] = '[115] SELECT_EVENT_SRC()'

    ai.SetInitState(136)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction82(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction122(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout3)
    return 1
end

function AI:CustomAction118(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end

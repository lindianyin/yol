-- 编写者：hejingxian
-- 版本号：6

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
    ai.RegisterUserAction(2102, 'CustomAction102')

    -- 初始
    state = ai.NewState(28)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 22)

    -- 记录原点
    action = ai.NewAction(22, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(3)
    g_AIDebugInfo[22] = '[22] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(3, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(166)
    g_AIDebugInfo[3] = '[3] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(166, AI_ACTION.SET_STATE)
    action.SetParam(32) -- 待机状态
    g_AIDebugInfo[166] = '[166] SetParam(State32)'

    -- 待机状态
    state = ai.NewState(32)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 65)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 42)

    -- 选择事件发起者
    action = ai.NewAction(65, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(94, 19)
    g_AIDebugInfo[65] = '[65] SELECT_EVENT_SRC()'

    -- 站立
    action = ai.NewAction(19, AI_ACTION.STAND)
    action.SetBranch(44)
    g_AIDebugInfo[19] = '[19] STAND()'

    -- 设置主定时器
    action = ai.NewAction(44, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(168)
    g_AIDebugInfo[44] = '[44] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(168, AI_ACTION.SET_STATE)
    action.SetParam(66) -- 待机状态
    g_AIDebugInfo[168] = '[168] SetParam(State66)'

    -- 待机状态
    state = ai.NewState(66)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 60)

    -- 搜索敌人
    action = ai.NewAction(60, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(6000)
    action.SetBranch(86, 62)
    g_AIDebugInfo[60] = '[60] SEARCH_ENEMY(6000)'

    -- 释放技能
    action = ai.NewAction(62, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(19, 53)
    g_AIDebugInfo[62] = '[62] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(53, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(19, 67)
    g_AIDebugInfo[53] = '[53] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置系统无敌
    action = ai.NewAction(67, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(78)
    g_AIDebugInfo[67] = '[67] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(78, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(91, 91)
    g_AIDebugInfo[78] = '[78] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(91, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(170)
    g_AIDebugInfo[91] = '[91] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(170, AI_ACTION.SET_STATE)
    action.SetParam(39) -- 无敌返回状态
    g_AIDebugInfo[170] = '[170] SetParam(State39)'

    -- 无敌返回状态
    state = ai.NewState(39)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 96)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 96)

    -- 返回
    action = ai.NewAction(96, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(57, 57)
    g_AIDebugInfo[96] = '[96] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(57, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(172)
    g_AIDebugInfo[57] = '[57] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(172, AI_ACTION.SET_STATE)
    action.SetParam(27) -- 返回成功
    g_AIDebugInfo[172] = '[172] SetParam(State27)'

    -- 返回成功
    state = ai.NewState(27)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 14)

    -- 清除DEBUFF
    action = ai.NewAction(14, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(77)
    g_AIDebugInfo[14] = '[14] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(77, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(3)
    g_AIDebugInfo[77] = '[77] SET_SYSTEM_SHEILD(0)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(86, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(95)
    g_AIDebugInfo[86] = '[86] ADD_TARGET_TO_THREAT_LIST()'

    -- 选择第一威胁
    action = ai.NewAction(95, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(1, 19)
    g_AIDebugInfo[95] = '[95] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(1, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(81, 41)
    g_AIDebugInfo[1] = '[1] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(81, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(24, 85)
    g_AIDebugInfo[81] = '[81] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(85, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(174)
    g_AIDebugInfo[85] = '[85] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(174, AI_ACTION.SET_STATE)
    action.SetParam(58) -- 追击状态
    g_AIDebugInfo[174] = '[174] SetParam(State58)'

    -- 追击状态
    state = ai.NewState(58)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 19)
    state.HandleEvent(AI_EVENT.ON_CAST_SKILL_FAILED, 3)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 160)

    -- 目标知否存在
    action = ai.NewAction(160, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(7, 8)
    g_AIDebugInfo[160] = '[160] IS_TARGET_EXIST()'

    -- 比较返回点距离
    action = ai.NewAction(7, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(19, 81, 81)
    g_AIDebugInfo[7] = '[7] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 选择第一威胁
    action = ai.NewAction(8, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(97, 19)
    g_AIDebugInfo[8] = '[8] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(97, AI_ACTION.CHECK_LIFE)
    action.SetParam(40)
    action.SetBranch(102, 102, 1)
    g_AIDebugInfo[97] = '[97] CHECK_LIFE(40)'

    -- NPC喊话
    action = ai.NewAction(102, 2102)
    action.SetBranch(101)
    g_AIDebugInfo[102] = '[102] CustomAction102()'

    -- 设置主定时器
    action = ai.NewAction(101, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(176)
    g_AIDebugInfo[101] = '[101] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(176, AI_ACTION.SET_STATE)
    action.SetParam(100) -- 喊话结束
    g_AIDebugInfo[176] = '[176] SetParam(State100)'

    -- 喊话结束
    state = ai.NewState(100)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 99)

    -- 释放技能
    action = ai.NewAction(99, AI_ACTION.CAST_SKILL)
    action.SetParam(1418, 1)
    action.SetBranch(108, 108)
    g_AIDebugInfo[99] = '[99] CAST_SKILL(1418, 1)'

    -- 设置主定时器
    action = ai.NewAction(108, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(178)
    g_AIDebugInfo[108] = '[108] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(178, AI_ACTION.SET_STATE)
    action.SetParam(109) -- 变身结束
    g_AIDebugInfo[178] = '[178] SetParam(State109)'

    -- 变身结束
    state = ai.NewState(109)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 140)

    -- 选择第一威胁
    action = ai.NewAction(140, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(155, 131)
    g_AIDebugInfo[140] = '[140] SELECT_FIRST_THREAT()'

    -- 设置主定时器
    action = ai.NewAction(155, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(180)
    g_AIDebugInfo[155] = '[155] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(180, AI_ACTION.SET_STATE)
    action.SetParam(156) -- 变身结束
    g_AIDebugInfo[180] = '[180] SetParam(State156)'

    -- 变身结束
    state = ai.NewState(156)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 114)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 131)

    -- Npc标准技能选择
    action = ai.NewAction(114, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(123, 155)
    g_AIDebugInfo[114] = '[114] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(123, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(116, 126)
    g_AIDebugInfo[123] = '[123] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(116, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(155, 120)
    g_AIDebugInfo[116] = '[116] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(120, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(155, 131)
    g_AIDebugInfo[120] = '[120] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 站立
    action = ai.NewAction(131, AI_ACTION.STAND)
    action.SetBranch(137)
    g_AIDebugInfo[131] = '[131] STAND()'

    -- 设置主定时器
    action = ai.NewAction(137, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(182)
    g_AIDebugInfo[137] = '[137] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(182, AI_ACTION.SET_STATE)
    action.SetParam(134) -- 待机状态
    g_AIDebugInfo[182] = '[182] SetParam(State134)'

    -- 待机状态
    state = ai.NewState(134)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 132)

    -- 搜索敌人
    action = ai.NewAction(132, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(6000)
    action.SetBranch(136, 138)
    g_AIDebugInfo[132] = '[132] SEARCH_ENEMY(6000)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(136, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(140)
    g_AIDebugInfo[136] = '[136] ADD_TARGET_TO_THREAT_LIST()'

    -- 释放技能
    action = ai.NewAction(138, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(131, 139)
    g_AIDebugInfo[138] = '[138] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(139, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(131, 67)
    g_AIDebugInfo[139] = '[139] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置主定时器
    action = ai.NewAction(126, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(184)
    g_AIDebugInfo[126] = '[126] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(184, AI_ACTION.SET_STATE)
    action.SetParam(125) -- 追击状态
    g_AIDebugInfo[184] = '[184] SetParam(State125)'

    -- 追击状态
    state = ai.NewState(125)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 163)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 131)

    -- 目标知否存在
    action = ai.NewAction(163, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(115, 140)
    g_AIDebugInfo[163] = '[163] IS_TARGET_EXIST()'

    -- 比较返回点距离
    action = ai.NewAction(115, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(131, 123, 123)
    g_AIDebugInfo[115] = '[115] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(24, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(41, 70)
    g_AIDebugInfo[24] = '[24] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(70, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(8, 19)
    g_AIDebugInfo[70] = '[70] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 设置主定时器
    action = ai.NewAction(41, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(186)
    g_AIDebugInfo[41] = '[41] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(186, AI_ACTION.SET_STATE)
    action.SetParam(63) -- 战斗状态
    g_AIDebugInfo[186] = '[186] SetParam(State63)'

    -- 战斗状态
    state = ai.NewState(63)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 19)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 8)

    -- 将目标添加到威胁列表
    action = ai.NewAction(94, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(68)
    g_AIDebugInfo[94] = '[94] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(68, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(50)
    g_AIDebugInfo[68] = '[68] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(50, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(41)
    g_AIDebugInfo[50] = '[50] NPC_INIT_SKILL_CD()'

    -- 搜索敌人
    action = ai.NewAction(42, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(94, 3)
    g_AIDebugInfo[42] = '[42] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(28)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction102(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end


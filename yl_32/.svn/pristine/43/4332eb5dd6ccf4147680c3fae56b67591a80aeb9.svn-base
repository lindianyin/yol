-- 编写者：wushiwen1
-- 版本号：1

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

    -- 初始
    state = ai.NewState(48)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 11)

    -- 记录原点
    action = ai.NewAction(11, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(17)
    g_AIDebugInfo[11] = '[11] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(17, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(83)
    g_AIDebugInfo[17] = '[17] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(83, AI_ACTION.SET_STATE)
    action.SetParam(14) -- 待机状态
    g_AIDebugInfo[83] = '[83] SetParam(State14)'

    -- 待机状态
    state = ai.NewState(14)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 60)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 43)

    -- 选择事件发起者
    action = ai.NewAction(60, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(79, 47)
    g_AIDebugInfo[60] = '[60] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(79, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(18)
    g_AIDebugInfo[79] = '[79] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(18, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(34)
    g_AIDebugInfo[18] = '[18] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(34, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(5)
    g_AIDebugInfo[34] = '[34] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(5, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(85)
    g_AIDebugInfo[5] = '[5] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(85, AI_ACTION.SET_STATE)
    action.SetParam(38) -- 战斗状态
    g_AIDebugInfo[85] = '[85] SetParam(State38)'

    -- 战斗状态
    state = ai.NewState(38)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 80)

    -- 选择第一威胁
    action = ai.NewAction(80, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(9, 47)
    g_AIDebugInfo[80] = '[80] SELECT_FIRST_THREAT()'

    -- 返回
    action = ai.NewAction(47, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(74, 74)
    g_AIDebugInfo[47] = '[47] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(74, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(87)
    g_AIDebugInfo[74] = '[74] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(87, AI_ACTION.SET_STATE)
    action.SetParam(62) -- 无敌返回状态
    g_AIDebugInfo[87] = '[87] SetParam(State62)'

    -- 无敌返回状态
    state = ai.NewState(62)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 49)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 49)

    -- 返回
    action = ai.NewAction(49, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(3, 3)
    g_AIDebugInfo[49] = '[49] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(3, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(89)
    g_AIDebugInfo[3] = '[3] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(89, AI_ACTION.SET_STATE)
    action.SetParam(7) -- 返回成功
    g_AIDebugInfo[89] = '[89] SetParam(State7)'

    -- 返回成功
    state = ai.NewState(7)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 27)

    -- 清除DEBUFF
    action = ai.NewAction(27, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(17)
    g_AIDebugInfo[27] = '[27] CLEAN_DEBUFF()'

    -- Npc标准技能选择
    action = ai.NewAction(9, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(66, 5)
    g_AIDebugInfo[9] = '[9] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(66, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(75, 75)
    g_AIDebugInfo[66] = '[66] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(75, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(91)
    g_AIDebugInfo[75] = '[75] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(91, AI_ACTION.SET_STATE)
    action.SetParam(6) -- 战斗状态
    g_AIDebugInfo[91] = '[91] SetParam(State6)'

    -- 战斗状态
    state = ai.NewState(6)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 42)

    -- 选择第一威胁
    action = ai.NewAction(42, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(13, 47)
    g_AIDebugInfo[42] = '[42] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(13, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(76, 75)
    g_AIDebugInfo[13] = '[13] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(76, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(12, 50)
    g_AIDebugInfo[76] = '[76] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(12, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(75, 26)
    g_AIDebugInfo[12] = '[12] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(26, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(42, 47)
    g_AIDebugInfo[26] = '[26] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 设置主定时器
    action = ai.NewAction(50, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(93)
    g_AIDebugInfo[50] = '[50] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(93, AI_ACTION.SET_STATE)
    action.SetParam(53) -- 追击状态
    g_AIDebugInfo[93] = '[93] SetParam(State53)'

    -- 追击状态
    state = ai.NewState(53)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 19)

    -- 比较返回点距离
    action = ai.NewAction(19, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(47, 76, 76)
    g_AIDebugInfo[19] = '[19] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 搜索敌人
    action = ai.NewAction(43, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(79, 17)
    g_AIDebugInfo[43] = '[43] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(48)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

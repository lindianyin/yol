-- 编写者：hejingxian
-- 版本号：2

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
    state = ai.NewState(63)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 2)

    -- 记录原点
    action = ai.NewAction(2, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(20)
    g_AIDebugInfo[2] = '[2] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(20, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(79)
    g_AIDebugInfo[20] = '[20] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(79, AI_ACTION.SET_STATE)
    action.SetParam(45) -- 待机状态
    g_AIDebugInfo[79] = '[79] SetParam(State45)'

    -- 待机状态
    state = ai.NewState(45)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 9)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 5)

    -- 选择事件发起者
    action = ai.NewAction(9, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(43, 27)
    g_AIDebugInfo[9] = '[9] SELECT_EVENT_SRC()'

    -- 设置系统无敌
    action = ai.NewAction(27, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(39)
    g_AIDebugInfo[27] = '[27] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(39, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(6, 6)
    g_AIDebugInfo[39] = '[39] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(6, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(81)
    g_AIDebugInfo[6] = '[6] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(81, AI_ACTION.SET_STATE)
    action.SetParam(38) -- 无敌返回状态
    g_AIDebugInfo[81] = '[81] SetParam(State38)'

    -- 无敌返回状态
    state = ai.NewState(38)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 22)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 22)

    -- 返回
    action = ai.NewAction(22, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(12, 12)
    g_AIDebugInfo[22] = '[22] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(12, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(83)
    g_AIDebugInfo[12] = '[12] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(83, AI_ACTION.SET_STATE)
    action.SetParam(69) -- 返回成功
    g_AIDebugInfo[83] = '[83] SetParam(State69)'

    -- 返回成功
    state = ai.NewState(69)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 56)

    -- 清除DEBUFF
    action = ai.NewAction(56, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(13)
    g_AIDebugInfo[56] = '[56] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(13, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(20)
    g_AIDebugInfo[13] = '[13] SET_SYSTEM_SHEILD(0)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(43, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(32)
    g_AIDebugInfo[43] = '[43] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(32, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(71)
    g_AIDebugInfo[32] = '[32] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(71, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(59)
    g_AIDebugInfo[71] = '[71] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(59, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(85)
    g_AIDebugInfo[59] = '[59] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(85, AI_ACTION.SET_STATE)
    action.SetParam(7) -- 战斗状态
    g_AIDebugInfo[85] = '[85] SetParam(State7)'

    -- 战斗状态
    state = ai.NewState(7)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 48)

    -- 选择第一威胁
    action = ai.NewAction(48, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(29, 27)
    g_AIDebugInfo[48] = '[48] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(29, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(73, 59)
    g_AIDebugInfo[29] = '[29] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(73, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(23, 19)
    g_AIDebugInfo[73] = '[73] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(19, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(87)
    g_AIDebugInfo[19] = '[19] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(87, AI_ACTION.SET_STATE)
    action.SetParam(74) -- 追击状态
    g_AIDebugInfo[87] = '[87] SetParam(State74)'

    -- 追击状态
    state = ai.NewState(74)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 28)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 17)
    state.HandleEvent(AI_EVENT.ON_CAST_SKILL_FAILED, 20)

    -- 选择事件发起者
    action = ai.NewAction(28, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(17, 17)
    g_AIDebugInfo[28] = '[28] SELECT_EVENT_SRC()'

    -- 比较返回点距离
    action = ai.NewAction(17, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(27, 73, 73)
    g_AIDebugInfo[17] = '[17] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(23, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(59, 1)
    g_AIDebugInfo[23] = '[23] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(1, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(48, 27)
    g_AIDebugInfo[1] = '[1] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 搜索玩家 不带任何效果
    action = ai.NewAction(5, AI_ACTION.SEARCH_PLAYER_WITHOUT_ANY_AFFECT)
    action.SetParam(param['AlertRange'])
    action.SetBranch(76, 20)
    g_AIDebugInfo[5] = '[5] SEARCH_PLAYER_WITHOUT_ANY_AFFECT(param[AlertRange])'

    -- 搜索敌人
    action = ai.NewAction(76, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(2000)
    action.SetBranch(43, 20)
    g_AIDebugInfo[76] = '[76] SEARCH_ENEMY(2000)'

    ai.SetInitState(63)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

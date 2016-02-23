-- 编写者：hejingxian
-- 版本号：3

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
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 42)

    -- 记录原点
    action = ai.NewAction(42, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(43)
    g_AIDebugInfo[42] = '[42] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(43, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(79)
    g_AIDebugInfo[43] = '[43] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(79, AI_ACTION.SET_STATE)
    action.SetParam(3) -- 待机状态
    g_AIDebugInfo[79] = '[79] SetParam(State3)'

    -- 待机状态
    state = ai.NewState(3)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 55)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 38)

    -- 搜索敌人
    action = ai.NewAction(55, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(58, 43)
    g_AIDebugInfo[55] = '[55] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(58, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(8)
    g_AIDebugInfo[58] = '[58] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(8, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(36)
    g_AIDebugInfo[8] = '[8] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(36, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(20)
    g_AIDebugInfo[36] = '[36] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(20, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(81)
    g_AIDebugInfo[20] = '[20] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(81, AI_ACTION.SET_STATE)
    action.SetParam(47) -- 战斗状态
    g_AIDebugInfo[81] = '[81] SetParam(State47)'

    -- 战斗状态
    state = ai.NewState(47)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 10)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 43)

    -- 选择第一威胁
    action = ai.NewAction(10, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(34, 26)
    g_AIDebugInfo[10] = '[10] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(34, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(44, 20)
    g_AIDebugInfo[34] = '[34] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(44, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(29, 72)
    g_AIDebugInfo[44] = '[44] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(29, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(20, 22)
    g_AIDebugInfo[29] = '[29] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(22, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(10, 26)
    g_AIDebugInfo[22] = '[22] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 设置系统无敌
    action = ai.NewAction(26, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(61)
    g_AIDebugInfo[26] = '[26] SET_SYSTEM_SHEILD(1)'

    -- 站立
    action = ai.NewAction(61, AI_ACTION.STAND)
    action.SetBranch(65)
    g_AIDebugInfo[61] = '[61] STAND()'

    -- 设置主定时器
    action = ai.NewAction(65, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(83)
    g_AIDebugInfo[65] = '[65] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(83, AI_ACTION.SET_STATE)
    action.SetParam(63) -- 无敌返回状态
    g_AIDebugInfo[83] = '[83] SetParam(State63)'

    -- 无敌返回状态
    state = ai.NewState(63)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 18)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 18)

    -- 站立
    action = ai.NewAction(18, AI_ACTION.STAND)
    action.SetBranch(28)
    g_AIDebugInfo[18] = '[18] STAND()'

    -- 设置主定时器
    action = ai.NewAction(28, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(85)
    g_AIDebugInfo[28] = '[28] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(85, AI_ACTION.SET_STATE)
    action.SetParam(69) -- 返回成功
    g_AIDebugInfo[85] = '[85] SetParam(State69)'

    -- 返回成功
    state = ai.NewState(69)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 62)

    -- 清除DEBUFF
    action = ai.NewAction(62, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(46)
    g_AIDebugInfo[62] = '[62] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(46, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(43)
    g_AIDebugInfo[46] = '[46] SET_SYSTEM_SHEILD(0)'

    -- 设置主定时器
    action = ai.NewAction(72, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(87)
    g_AIDebugInfo[72] = '[72] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(87, AI_ACTION.SET_STATE)
    action.SetParam(56) -- 追击状态
    g_AIDebugInfo[87] = '[87] SetParam(State56)'

    -- 追击状态
    state = ai.NewState(56)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 24)
    state.HandleEvent(AI_EVENT.ON_CAST_SKILL_FAILED, 43)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 15)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 43)

    -- 选择事件发起者
    action = ai.NewAction(24, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(15, 15)
    g_AIDebugInfo[24] = '[24] SELECT_EVENT_SRC()'

    -- 比较返回点距离
    action = ai.NewAction(15, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(26, 44, 44)
    g_AIDebugInfo[15] = '[15] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 选择事件发起者
    action = ai.NewAction(38, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(58, 26)
    g_AIDebugInfo[38] = '[38] SELECT_EVENT_SRC()'

    ai.SetInitState(1)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

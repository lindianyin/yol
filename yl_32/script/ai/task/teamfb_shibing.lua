-- 编写者：hejingxian
-- 版本号：13

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
    state = ai.NewState(62)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 63)

    -- 记录原点
    action = ai.NewAction(63, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(69)
    g_AIDebugInfo[63] = '[63] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(69, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(98)
    g_AIDebugInfo[69] = '[69] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(98, AI_ACTION.SET_STATE)
    action.SetParam(52) -- 待机状态
    g_AIDebugInfo[98] = '[98] SetParam(State52)'

    -- 待机状态
    state = ai.NewState(52)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 30)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 45)

    -- 选择事件发起者
    action = ai.NewAction(30, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(20, 85)
    g_AIDebugInfo[30] = '[30] SELECT_EVENT_SRC()'

    -- 站立
    action = ai.NewAction(85, AI_ACTION.STAND)
    action.SetBranch(86)
    g_AIDebugInfo[85] = '[85] STAND()'

    -- 设置主定时器
    action = ai.NewAction(86, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(100)
    g_AIDebugInfo[86] = '[86] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(100, AI_ACTION.SET_STATE)
    action.SetParam(82) -- 待机状态
    g_AIDebugInfo[100] = '[100] SetParam(State82)'

    -- 待机状态
    state = ai.NewState(82)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 76)

    -- 搜索敌人
    action = ai.NewAction(76, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(6000)
    action.SetBranch(79, 85)
    g_AIDebugInfo[76] = '[76] SEARCH_ENEMY(6000)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(79, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(84)
    g_AIDebugInfo[79] = '[79] ADD_TARGET_TO_THREAT_LIST()'

    -- 选择第一威胁
    action = ai.NewAction(84, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(34, 85)
    g_AIDebugInfo[84] = '[84] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(34, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(21, 9)
    g_AIDebugInfo[34] = '[34] NPC_STANDARD_SKILL_SELECTOR()'

    -- 设置主定时器
    action = ai.NewAction(9, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(102)
    g_AIDebugInfo[9] = '[9] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(102, AI_ACTION.SET_STATE)
    action.SetParam(39) -- 战斗状态
    g_AIDebugInfo[102] = '[102] SetParam(State39)'

    -- 战斗状态
    state = ai.NewState(39)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 4)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 69)

    -- 选择第一威胁
    action = ai.NewAction(4, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(34, 85)
    g_AIDebugInfo[4] = '[4] SELECT_FIRST_THREAT()'

    -- Npc保持技能释放距离
    action = ai.NewAction(21, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(23, 12)
    g_AIDebugInfo[21] = '[21] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(23, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(9, 17)
    g_AIDebugInfo[23] = '[23] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(17, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(9, 85)
    g_AIDebugInfo[17] = '[17] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 设置主定时器
    action = ai.NewAction(12, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(104)
    g_AIDebugInfo[12] = '[12] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(104, AI_ACTION.SET_STATE)
    action.SetParam(36) -- 追击状态
    g_AIDebugInfo[104] = '[104] SetParam(State36)'

    -- 追击状态
    state = ai.NewState(36)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 91)
    state.HandleEvent(AI_EVENT.ON_CAST_SKILL_FAILED, 69)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 85)

    -- 目标知否存在
    action = ai.NewAction(91, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(13, 4)
    g_AIDebugInfo[91] = '[91] IS_TARGET_EXIST()'

    -- 比较返回点距离
    action = ai.NewAction(13, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(85, 21, 21)
    g_AIDebugInfo[13] = '[13] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(20, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(43)
    g_AIDebugInfo[20] = '[20] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(43, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(65)
    g_AIDebugInfo[43] = '[43] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(65, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(9)
    g_AIDebugInfo[65] = '[65] NPC_INIT_SKILL_CD()'

    -- 搜索敌人
    action = ai.NewAction(45, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(20, 69)
    g_AIDebugInfo[45] = '[45] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(62)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

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
    state = ai.NewState(34)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 18)

    -- 记录原点
    action = ai.NewAction(18, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(80)
    g_AIDebugInfo[18] = '[18] RECORD_ORIGIN_POSITION()'

    -- 进入巡逻路径
    action = ai.NewAction(80, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(57)
    g_AIDebugInfo[80] = '[80] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(57, AI_ACTION.DO_PATROL)
    action.SetBranch(30, 30)
    g_AIDebugInfo[57] = '[57] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(30, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(81)
    g_AIDebugInfo[30] = '[30] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(81, AI_ACTION.SET_STATE)
    action.SetParam(55) -- 寻路结束
    g_AIDebugInfo[81] = '[81] SetParam(State55)'

    -- 寻路结束
    state = ai.NewState(55)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 57)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 57)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 43)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 15)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 32)

    -- 设置副定时器
    action = ai.NewAction(43, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(30)
    g_AIDebugInfo[43] = '[43] SET_SECONDARY_TIMER(16)'

    -- 搜索敌人
    action = ai.NewAction(15, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(48, 30)
    g_AIDebugInfo[15] = '[15] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(48, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(74)
    g_AIDebugInfo[48] = '[48] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(74, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(31)
    g_AIDebugInfo[74] = '[74] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(31, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(75)
    g_AIDebugInfo[31] = '[31] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(75, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(83)
    g_AIDebugInfo[75] = '[75] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(83, AI_ACTION.SET_STATE)
    action.SetParam(13) -- 战斗状态
    g_AIDebugInfo[83] = '[83] SetParam(State13)'

    -- 战斗状态
    state = ai.NewState(13)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 65)

    -- 选择第一威胁
    action = ai.NewAction(65, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(61, 80)
    g_AIDebugInfo[65] = '[65] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(61, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(2, 75)
    g_AIDebugInfo[61] = '[61] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(2, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(59, 42)
    g_AIDebugInfo[2] = '[2] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(42, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(85)
    g_AIDebugInfo[42] = '[42] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(85, AI_ACTION.SET_STATE)
    action.SetParam(68) -- 追击状态
    g_AIDebugInfo[85] = '[85] SetParam(State68)'

    -- 追击状态
    state = ai.NewState(68)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 30)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 33)

    -- 比较返回点距离
    action = ai.NewAction(33, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(80, 2, 2)
    g_AIDebugInfo[33] = '[33] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(59, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(75, 29)
    g_AIDebugInfo[59] = '[59] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(29, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(100)
    action.SetBranch(65, 80)
    g_AIDebugInfo[29] = '[29] CHECK_SKILL_FAILED_COUNTER(100)'

    -- 选择事件发起者
    action = ai.NewAction(32, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(48, 80)
    g_AIDebugInfo[32] = '[32] SELECT_EVENT_SRC()'

    ai.SetInitState(34)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

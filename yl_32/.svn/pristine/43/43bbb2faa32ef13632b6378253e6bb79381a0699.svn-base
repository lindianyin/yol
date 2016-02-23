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
    state = ai.NewState(5)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 44)

    -- 记录原点
    action = ai.NewAction(44, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(53)
    g_AIDebugInfo[44] = '[44] RECORD_ORIGIN_POSITION()'

    -- 记录返回点
    action = ai.NewAction(53, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(24)
    g_AIDebugInfo[53] = '[53] RECORD_RETURN_POSITION()'

    -- 进入巡逻路径
    action = ai.NewAction(24, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(42)
    g_AIDebugInfo[24] = '[24] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(42, AI_ACTION.DO_PATROL)
    action.SetBranch(45, 73)
    g_AIDebugInfo[42] = '[42] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(73, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(96)
    g_AIDebugInfo[73] = '[73] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(96, AI_ACTION.SET_STATE)
    action.SetParam(75) -- 攻击仙女
    g_AIDebugInfo[96] = '[96] SetParam(State75)'

    -- 攻击仙女
    state = ai.NewState(75)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 74)

    -- 搜索敌人
    action = ai.NewAction(74, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(55, 73)
    g_AIDebugInfo[74] = '[74] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(55, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(58)
    g_AIDebugInfo[55] = '[55] ADD_TARGET_TO_THREAT_LIST()'

    -- Npc初始化技能CD
    action = ai.NewAction(58, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(28)
    g_AIDebugInfo[58] = '[58] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(28, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(98)
    g_AIDebugInfo[28] = '[28] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(98, AI_ACTION.SET_STATE)
    action.SetParam(84) -- 普通攻击
    g_AIDebugInfo[98] = '[98] SetParam(State84)'

    -- 普通攻击
    state = ai.NewState(84)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 82)

    -- Npc标准技能选择
    action = ai.NewAction(82, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(83, 28)
    g_AIDebugInfo[82] = '[82] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(83, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(85, 85)
    g_AIDebugInfo[83] = '[83] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(85, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(28, 81)
    g_AIDebugInfo[85] = '[85] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(81, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(50)
    action.SetBranch(28, 73)
    g_AIDebugInfo[81] = '[81] CHECK_SKILL_FAILED_COUNTER(50)'

    -- 设置主定时器
    action = ai.NewAction(45, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(100)
    g_AIDebugInfo[45] = '[45] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(100, AI_ACTION.SET_STATE)
    action.SetParam(46) -- 主状态
    g_AIDebugInfo[100] = '[100] SetParam(State46)'

    -- 主状态
    state = ai.NewState(46)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 42)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 42)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 54)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 45)

    -- 设置副定时器
    action = ai.NewAction(54, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(45)
    g_AIDebugInfo[54] = '[54] SET_SECONDARY_TIMER(16)'

    ai.SetInitState(5)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 编写者：hejingxian
-- 版本号：4

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
    state = ai.NewState(46)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 66)

    -- 记录原点
    action = ai.NewAction(66, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(17)
    g_AIDebugInfo[66] = '[66] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(17, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(86)
    g_AIDebugInfo[17] = '[17] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(86, AI_ACTION.SET_STATE)
    action.SetParam(14) -- 待机状态
    g_AIDebugInfo[86] = '[86] SetParam(State14)'

    -- 待机状态
    state = ai.NewState(14)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 57)

    -- 搜索敌人
    action = ai.NewAction(57, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(26, 17)
    g_AIDebugInfo[57] = '[57] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(26, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(55)
    g_AIDebugInfo[26] = '[26] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(55, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(21)
    g_AIDebugInfo[55] = '[55] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(21, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(2)
    g_AIDebugInfo[21] = '[21] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(2, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(88)
    g_AIDebugInfo[2] = '[2] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(88, AI_ACTION.SET_STATE)
    action.SetParam(23) -- 战斗状态
    g_AIDebugInfo[88] = '[88] SetParam(State23)'

    -- 战斗状态
    state = ai.NewState(23)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 13)

    -- 选择第一威胁
    action = ai.NewAction(13, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(50, 17)
    g_AIDebugInfo[13] = '[13] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(50, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(3, 17)
    g_AIDebugInfo[50] = '[50] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(3, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(15, 15)
    g_AIDebugInfo[3] = '[3] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(15, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(81, 81)
    g_AIDebugInfo[15] = '[15] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(81, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(90)
    g_AIDebugInfo[81] = '[81] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(90, AI_ACTION.SET_STATE)
    action.SetParam(80) -- 战斗状态
    g_AIDebugInfo[90] = '[90] SetParam(State80)'

    -- 战斗状态
    state = ai.NewState(80)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 81)

    ai.SetInitState(46)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

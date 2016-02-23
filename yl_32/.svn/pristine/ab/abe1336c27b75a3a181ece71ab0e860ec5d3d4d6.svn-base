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

    -- 初始
    state = ai.NewState(5)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)

    -- 记录原点
    action = ai.NewAction(6, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(8)
    g_AIDebugInfo[6] = '[6] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(8, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(43)
    g_AIDebugInfo[8] = '[8] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(43, AI_ACTION.SET_STATE)
    action.SetParam(23) -- 待机
    g_AIDebugInfo[43] = '[43] SetParam(State23)'

    -- 待机
    state = ai.NewState(23)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 15)

    -- 搜索敌人
    action = ai.NewAction(15, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(24, 8)
    g_AIDebugInfo[15] = '[15] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(24, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(41)
    g_AIDebugInfo[24] = '[24] ADD_TARGET_TO_THREAT_LIST()'

    -- 选择第一威胁
    action = ai.NewAction(41, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(27, 8)
    g_AIDebugInfo[41] = '[41] SELECT_FIRST_THREAT()'

    -- 设置主定时器
    action = ai.NewAction(27, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(45)
    g_AIDebugInfo[27] = '[27] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(45, AI_ACTION.SET_STATE)
    action.SetParam(28) -- 释放技能
    g_AIDebugInfo[45] = '[45] SetParam(State28)'

    -- 释放技能
    state = ai.NewState(28)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 21)

    -- Npc标准技能选择
    action = ai.NewAction(21, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(35, 8)
    g_AIDebugInfo[21] = '[21] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(35, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(14, 37)
    g_AIDebugInfo[35] = '[35] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(14, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(8, 1)
    g_AIDebugInfo[14] = '[14] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(1, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(47)
    g_AIDebugInfo[1] = '[1] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(47, AI_ACTION.SET_STATE)
    action.SetParam(7) -- 释放技能结束
    g_AIDebugInfo[47] = '[47] SetParam(State7)'

    -- 释放技能结束
    state = ai.NewState(7)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 8)

    -- 清除目标威胁
    action = ai.NewAction(37, AI_ACTION.CLEAR_THREAT_FOR_TARGET)
    action.SetBranch(8, 8)
    g_AIDebugInfo[37] = '[37] CLEAR_THREAT_FOR_TARGET()'

    ai.SetInitState(5)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

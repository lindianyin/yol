-- 编写者：hejingxian
-- 版本号：17

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
    state = ai.NewState(12)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 8)

    -- 记录原点
    action = ai.NewAction(8, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(3)
    g_AIDebugInfo[8] = '[8] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(3, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(40)
    g_AIDebugInfo[3] = '[3] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(40, AI_ACTION.SET_STATE)
    action.SetParam(16) -- 待机
    g_AIDebugInfo[40] = '[40] SetParam(State16)'

    -- 待机
    state = ai.NewState(16)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 15)

    -- 站立
    action = ai.NewAction(15, AI_ACTION.STAND)
    action.SetBranch(7)
    g_AIDebugInfo[15] = '[15] STAND()'

    -- 搜索敌人
    action = ai.NewAction(7, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(20, 3)
    g_AIDebugInfo[7] = '[7] SEARCH_ENEMY(param[AlertRange])'

    -- Npc标准技能选择
    action = ai.NewAction(20, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(21, 3)
    g_AIDebugInfo[20] = '[20] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(21, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(3, 24)
    g_AIDebugInfo[21] = '[21] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(24, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(42)
    g_AIDebugInfo[24] = '[24] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(42, AI_ACTION.SET_STATE)
    action.SetParam(6) -- 释放技能结束
    g_AIDebugInfo[42] = '[42] SetParam(State6)'

    -- 释放技能结束
    state = ai.NewState(6)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 3)

    ai.SetInitState(12)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

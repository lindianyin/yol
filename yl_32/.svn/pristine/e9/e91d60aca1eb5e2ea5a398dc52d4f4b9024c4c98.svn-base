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
    state = ai.NewState(2)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 13)

    -- 记录原点
    action = ai.NewAction(13, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(7)
    g_AIDebugInfo[13] = '[13] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(7, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(33)
    g_AIDebugInfo[7] = '[7] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(33, AI_ACTION.SET_STATE)
    action.SetParam(8) -- 待机
    g_AIDebugInfo[33] = '[33] SetParam(State8)'

    -- 待机
    state = ai.NewState(8)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 4)
    state.HandleEvent(AI_EVENT.ON_SELF_KILLED, 4)

    -- 站立
    action = ai.NewAction(4, AI_ACTION.STAND)
    action.SetBranch(30)
    g_AIDebugInfo[4] = '[4] STAND()'

    -- 搜索敌人
    action = ai.NewAction(30, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(6, 6)
    g_AIDebugInfo[30] = '[30] SEARCH_ENEMY(param[AlertRange])'

    -- Npc标准技能选择
    action = ai.NewAction(6, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(15, 7)
    g_AIDebugInfo[6] = '[6] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(15, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(7, 11)
    g_AIDebugInfo[15] = '[15] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(11, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(35)
    g_AIDebugInfo[11] = '[11] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(35, AI_ACTION.SET_STATE)
    action.SetParam(17) -- 释放技能结束
    g_AIDebugInfo[35] = '[35] SetParam(State17)'

    -- 释放技能结束
    state = ai.NewState(17)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 7)

    ai.SetInitState(2)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 编写者：hejingxian
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
    ai.RegisterUserAction(2006, 'CustomAction6')

    -- 初始
    state = ai.NewState(14)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 3)

    -- 设置主定时器
    action = ai.NewAction(3, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(18)
    g_AIDebugInfo[3] = '[3] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(18, AI_ACTION.SET_STATE)
    action.SetParam(5) -- 待机
    g_AIDebugInfo[18] = '[18] SetParam(State5)'

    -- 待机
    state = ai.NewState(5)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)

    -- 选中自己
    action = ai.NewAction(6, 2006)
    action.SetBranch(17)
    g_AIDebugInfo[6] = '[6] CustomAction6()'

    -- Npc标准技能选择
    action = ai.NewAction(17, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(16, 16)
    g_AIDebugInfo[17] = '[17] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(16, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(22, 20)
    g_AIDebugInfo[16] = '[16] NPC_CAST_SELECT_SKILL()'

    -- 设置状态
    action = ai.NewAction(20, AI_ACTION.SET_STATE)
    action.SetParam(1) -- 状态
    g_AIDebugInfo[20] = '[20] SetParam(State1)'

    -- 状态
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 8)

    -- 设置状态
    action = ai.NewAction(8, AI_ACTION.SET_STATE)
    action.SetParam(4)
    g_AIDebugInfo[8] = '[8] SET_STATE(4)'

    -- 设置状态
    action = ai.NewAction(22, AI_ACTION.SET_STATE)
    action.SetParam(1) -- 状态
    g_AIDebugInfo[22] = '[22] SetParam(State1)'

    ai.SetInitState(14)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction6(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


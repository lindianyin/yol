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
    ai.RegisterUserAction(2004, 'CustomAction4')

    -- 初始
    state = ai.NewState(2)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 9)

    -- 设置主定时器
    action = ai.NewAction(9, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(50)
    action.SetBranch(21)
    g_AIDebugInfo[9] = '[9] SET_PRIMARY_TIMER(42)'

    -- 设置状态
    action = ai.NewAction(21, AI_ACTION.SET_STATE)
    action.SetParam(10) -- 待机
    g_AIDebugInfo[21] = '[21] SetParam(State10)'

    -- 待机
    state = ai.NewState(10)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 4)

    -- 选中自己
    action = ai.NewAction(4, 2004)
    action.SetBranch(18)
    g_AIDebugInfo[4] = '[4] CustomAction4()'

    -- Npc标准技能选择
    action = ai.NewAction(18, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(15, 15)
    g_AIDebugInfo[18] = '[18] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(15, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(23, 25)
    g_AIDebugInfo[15] = '[15] NPC_CAST_SELECT_SKILL()'

    -- 设置状态
    action = ai.NewAction(23, AI_ACTION.SET_STATE)
    action.SetParam(6) -- 状态
    g_AIDebugInfo[23] = '[23] SetParam(State6)'

    -- 状态
    state = ai.NewState(6)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 12)

    -- 设置状态
    action = ai.NewAction(12, AI_ACTION.SET_STATE)
    action.SetParam(4)
    g_AIDebugInfo[12] = '[12] SET_STATE(4)'

    -- 设置状态
    action = ai.NewAction(25, AI_ACTION.SET_STATE)
    action.SetParam(6) -- 状态
    g_AIDebugInfo[25] = '[25] SetParam(State6)'

    ai.SetInitState(2)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction4(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


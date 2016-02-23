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
    ai.RegisterUserAction(2001, 'CustomAction1')
    ai.RegisterUserAction(2002, 'CustomAction2')

    -- 初始
    state = ai.NewState(21)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 8)

    -- 设置主定时器
    action = ai.NewAction(8, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(27)
    g_AIDebugInfo[8] = '[8] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(27, AI_ACTION.SET_STATE)
    action.SetParam(11) -- 待机
    g_AIDebugInfo[27] = '[27] SetParam(State11)'

    -- 待机
    state = ai.NewState(11)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1)

    -- 选中自己
    action = ai.NewAction(1, 2001)
    action.SetBranch(17)
    g_AIDebugInfo[1] = '[1] CustomAction1()'

    -- Npc标准技能选择
    action = ai.NewAction(17, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(4, 4)
    g_AIDebugInfo[17] = '[17] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(4, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(29, 33)
    g_AIDebugInfo[4] = '[4] NPC_CAST_SELECT_SKILL()'

    -- 设置状态
    action = ai.NewAction(29, AI_ACTION.SET_STATE)
    action.SetParam(9) -- 待机
    g_AIDebugInfo[29] = '[29] SetParam(State9)'

    -- 待机
    state = ai.NewState(9)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 10)

    -- 设置主定时器
    action = ai.NewAction(10, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(64)
    action.SetBranch(2)
    g_AIDebugInfo[10] = '[10] SET_PRIMARY_TIMER(64)'

    -- 选中自己
    action = ai.NewAction(2, 2002)
    action.SetBranch(22)
    g_AIDebugInfo[2] = '[2] CustomAction2()'

    -- 释放技能
    action = ai.NewAction(22, AI_ACTION.CAST_SKILL)
    action.SetParam(1116, 1)
    action.SetBranch(31, 10)
    g_AIDebugInfo[22] = '[22] CAST_SKILL(1116, 1)'

    -- 设置状态
    action = ai.NewAction(31, AI_ACTION.SET_STATE)
    action.SetParam(14) -- 待机
    g_AIDebugInfo[31] = '[31] SetParam(State14)'

    -- 待机
    state = ai.NewState(14)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 20)

    -- 设置状态
    action = ai.NewAction(20, AI_ACTION.SET_STATE)
    action.SetParam(4)
    g_AIDebugInfo[20] = '[20] SET_STATE(4)'

    -- 设置状态
    action = ai.NewAction(33, AI_ACTION.SET_STATE)
    action.SetParam(9) -- 待机
    g_AIDebugInfo[33] = '[33] SetParam(State9)'

    ai.SetInitState(21)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction1(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

function AI:CustomAction2(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


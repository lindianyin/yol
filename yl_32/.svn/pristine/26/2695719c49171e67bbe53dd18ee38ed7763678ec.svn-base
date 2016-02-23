-- 编写者：hejingxian
-- 版本号：11

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
    ai.RegisterUserAction(2025, 'AI:CustomAction25')
    ai.RegisterUserAction(2031, 'AI:CustomAction31')
    ai.RegisterUserAction(2045, 'CustomAction45')

    -- 初始
    state = ai.NewState(7)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 14)

    -- 记录原点
    action = ai.NewAction(14, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(44)
    g_AIDebugInfo[14] = '[14] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(44, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(74)
    g_AIDebugInfo[44] = '[44] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(74, AI_ACTION.SET_STATE)
    action.SetParam(47) -- 对自己放火转技能
    g_AIDebugInfo[74] = '[74] SetParam(State47)'

    -- 对自己放火转技能
    state = ai.NewState(47)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 45)

    -- 选中自己
    action = ai.NewAction(45, 2045)
    action.SetBranch(46)
    g_AIDebugInfo[45] = '[45] CustomAction45()'

    -- 释放技能
    action = ai.NewAction(46, AI_ACTION.CAST_SKILL)
    action.SetParam(389, 1)
    action.SetBranch(6, 44)
    g_AIDebugInfo[46] = '[46] CAST_SKILL(389, 1)'

    -- 设置主定时器
    action = ai.NewAction(6, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(76)
    g_AIDebugInfo[6] = '[6] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(76, AI_ACTION.SET_STATE)
    action.SetParam(13) -- 待机
    g_AIDebugInfo[76] = '[76] SetParam(State13)'

    -- 待机
    state = ai.NewState(13)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 12)

    -- 站立
    action = ai.NewAction(12, AI_ACTION.STAND)
    action.SetBranch(19)
    g_AIDebugInfo[12] = '[12] STAND()'

    -- 搜索敌人
    action = ai.NewAction(19, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(8, 8)
    g_AIDebugInfo[19] = '[19] SEARCH_ENEMY(param[AlertRange])'

    -- Npc标准技能选择
    action = ai.NewAction(8, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(10, 6)
    g_AIDebugInfo[8] = '[8] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(10, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(24, 9)
    g_AIDebugInfo[10] = '[10] NPC_CAST_SELECT_SKILL()'

    -- 三选项随机
    action = ai.NewAction(24, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(10, 10, 80)
    action.SetBranch(25, 31, 6)
    g_AIDebugInfo[24] = '[24] RANDOM_TRI_BRANCH(10, 10, 80)'

    -- NPC喊话
    action = ai.NewAction(25, 2025)
    action.SetBranch(26)
    g_AIDebugInfo[25] = '[25] AI:CustomAction25()'

    -- 设置主定时器
    action = ai.NewAction(26, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(78)
    g_AIDebugInfo[26] = '[26] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(78, AI_ACTION.SET_STATE)
    action.SetParam(27) -- 喊话结束
    g_AIDebugInfo[78] = '[78] SetParam(State27)'

    -- 喊话结束
    state = ai.NewState(27)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)

    -- NPC喊话
    action = ai.NewAction(31, 2031)
    action.SetBranch(29)
    g_AIDebugInfo[31] = '[31] AI:CustomAction31()'

    -- 设置主定时器
    action = ai.NewAction(29, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(80)
    g_AIDebugInfo[29] = '[29] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(80, AI_ACTION.SET_STATE)
    action.SetParam(30) -- 喊话结束
    g_AIDebugInfo[80] = '[80] SetParam(State30)'

    -- 喊话结束
    state = ai.NewState(30)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)

    -- 设置主定时器
    action = ai.NewAction(9, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(82)
    g_AIDebugInfo[9] = '[9] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(82, AI_ACTION.SET_STATE)
    action.SetParam(21) -- 释放技能结束
    g_AIDebugInfo[82] = '[82] SetParam(State21)'

    -- 释放技能结束
    state = ai.NewState(21)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)

    ai.SetInitState(7)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction45(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

function AI:CustomAction25(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction31(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end


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
    ai.RegisterUserAction(2015, 'CustomAction15')
    ai.RegisterUserAction(2055, 'CustomAction55')

    -- 初始
    state = ai.NewState(17)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 13)

    -- 记录原点
    action = ai.NewAction(13, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(1)
    g_AIDebugInfo[13] = '[13] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(1, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(60)
    g_AIDebugInfo[1] = '[1] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(60, AI_ACTION.SET_STATE)
    action.SetParam(16) -- 待机
    g_AIDebugInfo[60] = '[60] SetParam(State16)'

    -- 待机
    state = ai.NewState(16)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 11)

    -- 站立
    action = ai.NewAction(11, AI_ACTION.STAND)
    action.SetBranch(3)
    g_AIDebugInfo[11] = '[11] STAND()'

    -- 搜索敌人
    action = ai.NewAction(3, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(15, 55)
    g_AIDebugInfo[3] = '[3] SEARCH_ENEMY(param[AlertRange])'

    -- NPC喊话
    action = ai.NewAction(55, 2055)
    action.SetBranch(56)
    g_AIDebugInfo[55] = '[55] CustomAction55()'

    -- 设置主定时器
    action = ai.NewAction(56, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(62)
    g_AIDebugInfo[56] = '[56] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(62, AI_ACTION.SET_STATE)
    action.SetParam(54) -- 喊话结束
    g_AIDebugInfo[62] = '[62] SetParam(State54)'

    -- 喊话结束
    state = ai.NewState(54)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1)

    -- NPC喊话
    action = ai.NewAction(15, 2015)
    action.SetBranch(2)
    g_AIDebugInfo[15] = '[15] CustomAction15()'

    -- 设置主定时器
    action = ai.NewAction(2, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(64)
    g_AIDebugInfo[2] = '[2] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(64, AI_ACTION.SET_STATE)
    action.SetParam(10) -- 喊话结束
    g_AIDebugInfo[64] = '[64] SetParam(State10)'

    -- 喊话结束
    state = ai.NewState(10)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 43)

    -- Npc标准技能选择
    action = ai.NewAction(43, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(50, 50)
    g_AIDebugInfo[43] = '[43] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(50, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(45, 45)
    g_AIDebugInfo[50] = '[50] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(45, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(66)
    g_AIDebugInfo[45] = '[45] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(66, AI_ACTION.SET_STATE)
    action.SetParam(51) -- 释放技能结束
    g_AIDebugInfo[66] = '[66] SetParam(State51)'

    -- 释放技能结束
    state = ai.NewState(51)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1)

    ai.SetInitState(17)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction55(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction15(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end


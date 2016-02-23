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
    ai.RegisterUserAction(2006, 'CustomAction6')
    ai.RegisterUserAction(2007, 'CustomAction7')

    -- 初始
    state = ai.NewState(12)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 27)

    -- 记录原点
    action = ai.NewAction(27, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(15)
    g_AIDebugInfo[27] = '[27] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(15, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(40)
    g_AIDebugInfo[15] = '[15] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(40, AI_ACTION.SET_STATE)
    action.SetParam(10) -- 搜索玩家
    g_AIDebugInfo[40] = '[40] SetParam(State10)'

    -- 搜索玩家
    state = ai.NewState(10)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 38)

    -- 搜索玩家 不带任何效果
    action = ai.NewAction(38, AI_ACTION.SEARCH_PLAYER_WITHOUT_ANY_AFFECT)
    action.SetParam(param['AlertRange'])
    action.SetBranch(6, 15)
    g_AIDebugInfo[38] = '[38] SEARCH_PLAYER_WITHOUT_ANY_AFFECT(param[AlertRange])'

    -- 选中自己
    action = ai.NewAction(6, 2006)
    action.SetBranch(13)
    g_AIDebugInfo[6] = '[6] CustomAction6()'

    -- Npc标准技能选择
    action = ai.NewAction(13, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(25, 25)
    g_AIDebugInfo[13] = '[13] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(25, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(2, 2)
    g_AIDebugInfo[25] = '[25] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(2, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(42)
    g_AIDebugInfo[2] = '[2] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(42, AI_ACTION.SET_STATE)
    action.SetParam(28) -- 释放技能结束
    g_AIDebugInfo[42] = '[42] SetParam(State28)'

    -- 释放技能结束
    state = ai.NewState(28)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 7)

    -- 选中自己
    action = ai.NewAction(7, 2007)
    action.SetBranch(21)
    g_AIDebugInfo[7] = '[7] CustomAction7()'

    -- 释放技能
    action = ai.NewAction(21, AI_ACTION.CAST_SKILL)
    action.SetParam(350, 1)
    action.SetBranch(44, 46)
    g_AIDebugInfo[21] = '[21] CAST_SKILL(350, 1)'

    -- 设置状态
    action = ai.NewAction(44, AI_ACTION.SET_STATE)
    action.SetParam(14) -- 自杀
    g_AIDebugInfo[44] = '[44] SetParam(State14)'

    -- 自杀
    state = ai.NewState(14)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 31)

    -- 设置状态
    action = ai.NewAction(31, AI_ACTION.SET_STATE)
    action.SetParam(4)
    g_AIDebugInfo[31] = '[31] SET_STATE(4)'

    -- 设置状态
    action = ai.NewAction(46, AI_ACTION.SET_STATE)
    action.SetParam(14) -- 自杀
    g_AIDebugInfo[46] = '[46] SetParam(State14)'

    ai.SetInitState(12)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction6(pNpc, ai)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

function AI:CustomAction7(pNpc, ai)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


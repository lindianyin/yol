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
    ai.RegisterUserAction(2026, 'AI:CustomAction26')
    ai.RegisterUserAction(2048, 'AI:CustomAction48')

    -- 初始
    state = ai.NewState(29)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 18)

    -- 记录原点
    action = ai.NewAction(18, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(15)
    g_AIDebugInfo[18] = '[18] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(15, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(52)
    g_AIDebugInfo[15] = '[15] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(52, AI_ACTION.SET_STATE)
    action.SetParam(19) -- 搜索玩家
    g_AIDebugInfo[52] = '[52] SetParam(State19)'

    -- 搜索玩家
    state = ai.NewState(19)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 44)

    -- 搜索敌人
    action = ai.NewAction(44, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(48, 15)
    g_AIDebugInfo[44] = '[44] SEARCH_ENEMY(param[AlertRange])'

    -- 选中自己
    action = ai.NewAction(48, 2048)
    action.SetBranch(38)
    g_AIDebugInfo[48] = '[48] AI:CustomAction48()'

    -- Npc标准技能选择
    action = ai.NewAction(38, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(11, 11)
    g_AIDebugInfo[38] = '[38] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(11, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(31, 31)
    g_AIDebugInfo[11] = '[11] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(31, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(54)
    g_AIDebugInfo[31] = '[31] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(54, AI_ACTION.SET_STATE)
    action.SetParam(20) -- 释放技能结束
    g_AIDebugInfo[54] = '[54] SetParam(State20)'

    -- 释放技能结束
    state = ai.NewState(20)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 26)

    -- 选中自己
    action = ai.NewAction(26, 2026)
    action.SetBranch(2)
    g_AIDebugInfo[26] = '[26] AI:CustomAction26()'

    -- 释放技能
    action = ai.NewAction(2, AI_ACTION.CAST_SKILL)
    action.SetParam(370, 1)
    action.SetBranch(58, 56)
    g_AIDebugInfo[2] = '[2] CAST_SKILL(370, 1)'

    -- 设置状态
    action = ai.NewAction(56, AI_ACTION.SET_STATE)
    action.SetParam(30) -- 自杀
    g_AIDebugInfo[56] = '[56] SetParam(State30)'

    -- 自杀
    state = ai.NewState(30)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 40)

    -- 设置状态
    action = ai.NewAction(40, AI_ACTION.SET_STATE)
    action.SetParam(4)
    g_AIDebugInfo[40] = '[40] SET_STATE(4)'

    -- 设置状态
    action = ai.NewAction(58, AI_ACTION.SET_STATE)
    action.SetParam(30) -- 自杀
    g_AIDebugInfo[58] = '[58] SetParam(State30)'

    ai.SetInitState(29)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction48(pNpc, ai)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

function AI:CustomAction26(pNpc, ai)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


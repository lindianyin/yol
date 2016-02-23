-- 编写者：hejingxian
-- 版本号：18

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
    ai.RegisterUserAction(2038, 'AI:CustomAction38')

    -- 初始
    state = ai.NewState(17)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 24)

    -- 进入巡逻路径
    action = ai.NewAction(24, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(25)
    g_AIDebugInfo[24] = '[24] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(25, AI_ACTION.DO_PATROL)
    action.SetBranch(18, 38)
    g_AIDebugInfo[25] = '[25] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(18, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(59)
    g_AIDebugInfo[18] = '[18] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(59, AI_ACTION.SET_STATE)
    action.SetParam(1) -- 待机
    g_AIDebugInfo[59] = '[59] SetParam(State1)'

    -- 待机
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 16)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 27)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 25)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 25)

    -- 搜索敌人
    action = ai.NewAction(16, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(42, 18)
    g_AIDebugInfo[16] = '[16] SEARCH_ENEMY(param[AlertRange])'

    -- 设置主定时器
    action = ai.NewAction(42, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(61)
    g_AIDebugInfo[42] = '[42] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(61, AI_ACTION.SET_STATE)
    action.SetParam(40) -- 对自己放技能
    g_AIDebugInfo[61] = '[61] SetParam(State40)'

    -- 对自己放技能
    state = ai.NewState(40)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 38)

    -- 选中自己
    action = ai.NewAction(38, 2038)
    action.SetBranch(39)
    g_AIDebugInfo[38] = '[38] AI:CustomAction38()'

    -- 释放技能
    action = ai.NewAction(39, AI_ACTION.CAST_SKILL)
    action.SetParam(369, 1)
    action.SetBranch(57, 38)
    g_AIDebugInfo[39] = '[39] CAST_SKILL(369, 1)'

    -- 设置主定时器
    action = ai.NewAction(57, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(63)
    g_AIDebugInfo[57] = '[57] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(63, AI_ACTION.SET_STATE)
    action.SetParam(55) -- 自杀
    g_AIDebugInfo[63] = '[63] SetParam(State55)'

    -- 自杀
    state = ai.NewState(55)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 54)

    -- 设置状态
    action = ai.NewAction(54, AI_ACTION.SET_STATE)
    action.SetParam(4)
    g_AIDebugInfo[54] = '[54] SET_STATE(4)'

    -- 设置副定时器
    action = ai.NewAction(27, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(4)
    action.SetBranch(18)
    g_AIDebugInfo[27] = '[27] SET_SECONDARY_TIMER(4)'

    ai.SetInitState(17)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction38(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


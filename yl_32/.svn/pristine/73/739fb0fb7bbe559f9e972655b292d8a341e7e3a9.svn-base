-- 编写者：hejingxian
-- 版本号：16

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
    ai.RegisterUserAction(2053, 'CustomAction53')

    -- 初始
    state = ai.NewState(22)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 26)

    -- 记录原点
    action = ai.NewAction(26, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(25)
    g_AIDebugInfo[26] = '[26] RECORD_ORIGIN_POSITION()'

    -- 记录返回点
    action = ai.NewAction(25, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(58)
    g_AIDebugInfo[25] = '[25] RECORD_RETURN_POSITION()'

    -- 设置状态
    action = ai.NewAction(58, AI_ACTION.SET_STATE)
    action.SetParam(9) -- 播放动作
    g_AIDebugInfo[58] = '[58] SetParam(State9)'

    -- 播放动作
    state = ai.NewState(9)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 35)

    -- 设置主定时器
    action = ai.NewAction(35, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(60)
    g_AIDebugInfo[35] = '[35] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(60, AI_ACTION.SET_STATE)
    action.SetParam(12) -- 进入寻路
    g_AIDebugInfo[60] = '[60] SetParam(State12)'

    -- 进入寻路
    state = ai.NewState(12)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 31)

    -- 进入巡逻路径
    action = ai.NewAction(31, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(20)
    g_AIDebugInfo[31] = '[31] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(20, AI_ACTION.DO_PATROL)
    action.SetBranch(34, 53)
    g_AIDebugInfo[20] = '[20] DO_PATROL()'

    -- 选中自己
    action = ai.NewAction(53, 2053)
    action.SetBranch(54)
    g_AIDebugInfo[53] = '[53] CustomAction53()'

    -- 释放技能
    action = ai.NewAction(54, AI_ACTION.CAST_SKILL)
    action.SetParam(392, 1)
    action.SetBranch(41, 34)
    g_AIDebugInfo[54] = '[54] CAST_SKILL(392, 1)'

    -- 设置主定时器
    action = ai.NewAction(41, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(62)
    g_AIDebugInfo[41] = '[41] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(62, AI_ACTION.SET_STATE)
    action.SetParam(40) -- 释放技能结束
    g_AIDebugInfo[62] = '[62] SetParam(State40)'

    -- 释放技能结束
    state = ai.NewState(40)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 34)

    -- 设置主定时器
    action = ai.NewAction(34, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(64)
    g_AIDebugInfo[34] = '[34] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(64, AI_ACTION.SET_STATE)
    action.SetParam(29) -- 主状态
    g_AIDebugInfo[64] = '[64] SetParam(State29)'

    -- 主状态
    state = ai.NewState(29)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 46)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 20)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 20)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 32)

    -- 搜索敌人
    action = ai.NewAction(46, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(53, 34)
    g_AIDebugInfo[46] = '[46] SEARCH_ENEMY(param[AlertRange])'

    -- 设置副定时器
    action = ai.NewAction(32, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(34)
    g_AIDebugInfo[32] = '[32] SET_SECONDARY_TIMER(16)'

    ai.SetInitState(22)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction53(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


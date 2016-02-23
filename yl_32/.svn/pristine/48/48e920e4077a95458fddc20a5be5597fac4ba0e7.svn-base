-- 编写者：hejingxian
-- 版本号：10

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

    -- 初始
    state = ai.NewState(12)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 10)

    -- 记录原点
    action = ai.NewAction(10, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(15)
    g_AIDebugInfo[10] = '[10] RECORD_ORIGIN_POSITION()'

    -- 记录返回点
    action = ai.NewAction(15, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(21)
    g_AIDebugInfo[15] = '[15] RECORD_RETURN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(21, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(64)
    action.SetBranch(40)
    g_AIDebugInfo[21] = '[21] SET_PRIMARY_TIMER(64)'

    -- 设置状态
    action = ai.NewAction(40, AI_ACTION.SET_STATE)
    action.SetParam(36) -- 播放动作
    g_AIDebugInfo[40] = '[40] SetParam(State36)'

    -- 播放动作
    state = ai.NewState(36)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 25)

    -- 选中自己
    action = ai.NewAction(25, 2025)
    action.SetBranch(27)
    g_AIDebugInfo[25] = '[25] AI:CustomAction25()'

    -- Npc标准技能选择
    action = ai.NewAction(27, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(24, 24)
    g_AIDebugInfo[27] = '[27] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(24, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(38, 38)
    g_AIDebugInfo[24] = '[24] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(38, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(42)
    g_AIDebugInfo[38] = '[38] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(42, AI_ACTION.SET_STATE)
    action.SetParam(31) -- 进入寻路
    g_AIDebugInfo[42] = '[42] SetParam(State31)'

    -- 进入寻路
    state = ai.NewState(31)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 20)

    -- 进入巡逻路径
    action = ai.NewAction(20, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(18)
    g_AIDebugInfo[20] = '[20] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(18, AI_ACTION.DO_PATROL)
    action.SetBranch(19, 19)
    g_AIDebugInfo[18] = '[18] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(19, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(44)
    g_AIDebugInfo[19] = '[19] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(44, AI_ACTION.SET_STATE)
    action.SetParam(5) -- 主状态
    g_AIDebugInfo[44] = '[44] SetParam(State5)'

    -- 主状态
    state = ai.NewState(5)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 18)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 18)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 19)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 7)

    -- 设置副定时器
    action = ai.NewAction(7, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(19)
    g_AIDebugInfo[7] = '[7] SET_SECONDARY_TIMER(16)'

    ai.SetInitState(12)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction25(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


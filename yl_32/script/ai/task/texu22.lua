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
    ai.RegisterUserAction(2030, 'AI:CustomAction30')

    -- 初始
    state = ai.NewState(27)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 8)

    -- 记录原点
    action = ai.NewAction(8, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(23)
    g_AIDebugInfo[8] = '[8] RECORD_ORIGIN_POSITION()'

    -- 记录返回点
    action = ai.NewAction(23, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(6)
    g_AIDebugInfo[23] = '[23] RECORD_RETURN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(6, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(64)
    action.SetBranch(37)
    g_AIDebugInfo[6] = '[6] SET_PRIMARY_TIMER(64)'

    -- 设置状态
    action = ai.NewAction(37, AI_ACTION.SET_STATE)
    action.SetParam(36) -- 播放动作
    g_AIDebugInfo[37] = '[37] SetParam(State36)'

    -- 播放动作
    state = ai.NewState(36)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 30)

    -- 选中自己
    action = ai.NewAction(30, 2030)
    action.SetBranch(1)
    g_AIDebugInfo[30] = '[30] AI:CustomAction30()'

    -- Npc标准技能选择
    action = ai.NewAction(1, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(25, 25)
    g_AIDebugInfo[1] = '[1] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(25, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(24, 24)
    g_AIDebugInfo[25] = '[25] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(24, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(39)
    g_AIDebugInfo[24] = '[24] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(39, AI_ACTION.SET_STATE)
    action.SetParam(7) -- 进入寻路
    g_AIDebugInfo[39] = '[39] SetParam(State7)'

    -- 进入寻路
    state = ai.NewState(7)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 22)

    -- 进入巡逻路径
    action = ai.NewAction(22, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(10)
    g_AIDebugInfo[22] = '[22] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(10, AI_ACTION.DO_PATROL)
    action.SetBranch(17, 17)
    g_AIDebugInfo[10] = '[10] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(17, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(41)
    g_AIDebugInfo[17] = '[17] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(41, AI_ACTION.SET_STATE)
    action.SetParam(33) -- 主状态
    g_AIDebugInfo[41] = '[41] SetParam(State33)'

    -- 主状态
    state = ai.NewState(33)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 17)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 10)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 10)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 15)

    -- 设置副定时器
    action = ai.NewAction(15, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(17)
    g_AIDebugInfo[15] = '[15] SET_SECONDARY_TIMER(16)'

    ai.SetInitState(27)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction30(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


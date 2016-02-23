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
    ai.RegisterUserAction(2030, 'AI:CustomAction30')

    -- 初始
    state = ai.NewState(6)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 19)

    -- 进入巡逻路径
    action = ai.NewAction(19, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(18)
    g_AIDebugInfo[19] = '[19] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(18, AI_ACTION.DO_PATROL)
    action.SetBranch(3, 3)
    g_AIDebugInfo[18] = '[18] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(3, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(18)
    action.SetBranch(36)
    g_AIDebugInfo[3] = '[3] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(36, AI_ACTION.SET_STATE)
    action.SetParam(2) -- 待机
    g_AIDebugInfo[36] = '[36] SetParam(State2)'

    -- 待机
    state = ai.NewState(2)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 30)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 18)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 32)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 18)

    -- 选中自己
    action = ai.NewAction(30, 2030)
    action.SetBranch(4)
    g_AIDebugInfo[30] = '[30] AI:CustomAction30()'

    -- Npc标准技能选择
    action = ai.NewAction(4, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(13, 13)
    g_AIDebugInfo[4] = '[4] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(13, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(40, 38)
    g_AIDebugInfo[13] = '[13] NPC_CAST_SELECT_SKILL()'

    -- 设置状态
    action = ai.NewAction(38, AI_ACTION.SET_STATE)
    action.SetParam(11) -- 状态
    g_AIDebugInfo[38] = '[38] SetParam(State11)'

    -- 状态
    state = ai.NewState(11)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 8)

    -- 设置状态
    action = ai.NewAction(8, AI_ACTION.SET_STATE)
    action.SetParam(4)
    g_AIDebugInfo[8] = '[8] SET_STATE(4)'

    -- 设置状态
    action = ai.NewAction(40, AI_ACTION.SET_STATE)
    action.SetParam(11) -- 状态
    g_AIDebugInfo[40] = '[40] SetParam(State11)'

    -- 设置副定时器
    action = ai.NewAction(32, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(0)
    action.SetBranch(18)
    g_AIDebugInfo[32] = '[32] SET_SECONDARY_TIMER(16)'

    ai.SetInitState(6)
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


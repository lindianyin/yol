-- 编写者：chenkangyin
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
    ai.RegisterUserAction(2003, 'AI:MuFouYuYaoTongDeathCallNpc')

    -- 初始
    state = ai.NewState(2)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 7)

    -- 设置主定时器
    action = ai.NewAction(7, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(12)
    g_AIDebugInfo[7] = '[7] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(12, AI_ACTION.SET_STATE)
    action.SetParam(5) -- 待机
    g_AIDebugInfo[12] = '[12] SetParam(State5)'

    -- 待机
    state = ai.NewState(5)
    state.HandleEvent(AI_EVENT.ON_SELF_KILLED, 3)

    -- 自定义动作6
    action = ai.NewAction(3, 2003)
    action.SetBranch(14)
    g_AIDebugInfo[3] = '[3] AI:MuFouYuYaoTongDeathCallNpc()'

    -- 设置状态
    action = ai.NewAction(14, AI_ACTION.SET_STATE)
    action.SetParam(10) -- 状态
    g_AIDebugInfo[14] = '[14] SetParam(State10)'

    -- 状态
    state = ai.NewState(10)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1)

    -- 设置状态
    action = ai.NewAction(1, AI_ACTION.SET_STATE)
    action.SetParam(10)
    g_AIDebugInfo[1] = '[1] SET_STATE(10)'

    ai.SetInitState(2)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:MuFouYuYaoTongDeathCallNpc(pNpc, actionData)
    -- 输入自定义动作实现脚本
	local tbPos = pNpc.GetPosition();
	local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(8060,nSceneId, tbPos.nX,tbPos.nY,tbPos.nZ);
    return 1
end


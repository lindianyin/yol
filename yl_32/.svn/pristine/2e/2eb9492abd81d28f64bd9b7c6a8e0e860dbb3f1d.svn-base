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
    ai.RegisterUserAction(2006, 'AI:MuFouYuHongHuLuDeathCallNpc')

    -- 初始
    state = ai.NewState(7)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 4)

    -- 设置主定时器
    action = ai.NewAction(4, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(12)
    g_AIDebugInfo[4] = '[4] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(12, AI_ACTION.SET_STATE)
    action.SetParam(8) -- 待机
    g_AIDebugInfo[12] = '[12] SetParam(State8)'

    -- 待机
    state = ai.NewState(8)
    state.HandleEvent(AI_EVENT.ON_SELF_KILLED, 6)

    -- 自定义动作6
    action = ai.NewAction(6, 2006)
    action.SetBranch(14)
    g_AIDebugInfo[6] = '[6] AI:MuFouYuHongHuLuDeathCallNpc()'

    -- 设置状态
    action = ai.NewAction(14, AI_ACTION.SET_STATE)
    action.SetParam(10) -- 状态
    g_AIDebugInfo[14] = '[14] SetParam(State10)'

    -- 状态
    state = ai.NewState(10)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 2)

    -- 设置状态
    action = ai.NewAction(2, AI_ACTION.SET_STATE)
    action.SetParam(10)
    g_AIDebugInfo[2] = '[2] SET_STATE(10)'

    ai.SetInitState(7)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:MuFouYuHongHuLuDeathCallNpc(pNpc, actionData)
    -- 输入自定义动作实现脚本
	local tbPos = pNpc.GetPosition();
	local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(8061,nSceneId, tbPos.nX,tbPos.nY,tbPos.nZ);
    return 1
end


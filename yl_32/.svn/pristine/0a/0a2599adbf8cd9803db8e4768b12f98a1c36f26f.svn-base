-- 编写者：chenkangyin
-- 版本号：1

Include('script/ai/customfunctions.lua')
Include('script/ai/ai_param.lua')
Include('script/ai/argumentstrings.lua')

local g_AIDebugInfo = {}

function AI:Setup(ai)
    local state
    local action
    local param = AI.g_AIParam[ai.nAIType]

    -- 注册自定义动作
    ai.RegisterUserAction(2001, 'AI:ShuiMoYanCheDiShiZhuDeathCallNpc')

    -- 初始
    state = ai.NewState(9)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 5)

    -- 设置主定时器
    action = ai.NewAction(5, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(10)
    g_AIDebugInfo[5] = '[5] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(10, AI_ACTION.SET_STATE)
    action.SetParam(3) -- 待机
    g_AIDebugInfo[10] = '[10] SetParam(State3)'

    -- 待机
    state = ai.NewState(3)
    state.HandleEvent(AI_EVENT.ON_SELF_KILLED, 1)

    -- 自定义动作6
    action = ai.NewAction(1, 2001)
    action.SetBranch(4)
    g_AIDebugInfo[1] = '[1] AI:ShuiMoYanCheDiShiZhuDeathCallNpc()'

    -- 设置状态
    action = ai.NewAction(4, AI_ACTION.SET_STATE)
    action.SetParam(1)
    g_AIDebugInfo[4] = '[4] SET_STATE(1)'

    ai.SetInitState(9)
end

-- 调试方法
function AI:DebugOnCallAction(action_id)
    Log('[AI] Call action: ' .. g_AIDebugInfo[action_id])
end

-- 本AI图内自定义脚本
function AI:ShuiMoYanCheDiShiZhuDeathCallNpc(pNpc, actionData)
    -- 输入自定义动作实现脚本
	local tbPos = pNpc.GetPosition();
	local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(8063,nSceneId, tbPos.nX,tbPos.nY,tbPos.nZ);
    return 1
end


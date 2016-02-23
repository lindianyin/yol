-- 编写者：lidianyi
-- 版本号：2

Include('script/ai/customfunctions.lua')
Include('script/ai/ai_param.lua')
Include('script/ai/argumentstrings.lua')

local g_AIDebugInfo = {}

function AI:Setup(ai)
    local state
    local action
    local param = AI.g_AIParam[ai.nAIType]

    -- 注册自定义动作
    ai.RegisterUserAction(2006, 'AI:ShouHaoYinXingZhuZi')

    -- 初始
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 2)

    -- 设置主定时器
    action = ai.NewAction(2, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(10)
    g_AIDebugInfo[2] = '[2] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(10, AI_ACTION.SET_STATE)
    action.SetParam(4) -- 待机
    g_AIDebugInfo[10] = '[10] SetParam(State4)'

    -- 待机
    state = ai.NewState(4)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)

    -- 自定义动作6
    action = ai.NewAction(6, 2006)
    action.SetBranch(8)
    g_AIDebugInfo[6] = '[6] AI:ShouHaoYinXingZhuZi()'

    -- 设置状态
    action = ai.NewAction(8, AI_ACTION.SET_STATE)
    action.SetParam(1)
    g_AIDebugInfo[8] = '[8] SET_STATE(1)'

    ai.SetInitState(1)
end

-- 调试方法
function AI:DebugOnCallAction(action_id)
    Log('[AI] Call action: ' .. g_AIDebugInfo[action_id])
end

-- 本AI图内自定义脚本
function AI:ShouHaoYinXingZhuZi(pNpc, actionData)
    -- 输入自定义动作实现脚本
	BufferApi.AddBufferToCharacter(pNpc.dwId,0,pNpc.dwId,908,1)
    return 1
end


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
    ai.RegisterUserAction(2006, 'AI:ZHZBCheckHP')
    ai.RegisterUserAction(2008, 'AI:ZHZBAddHP')

    -- 初始
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 2)

    -- 设置主定时器
    action = ai.NewAction(2, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(12)
    g_AIDebugInfo[2] = '[2] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(12, AI_ACTION.SET_STATE)
    action.SetParam(4) -- 待机
    g_AIDebugInfo[12] = '[12] SetParam(State4)'

    -- 待机
    state = ai.NewState(4)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)

    -- 自定义动作6
    action = ai.NewAction(6, 2006)
    action.SetBranch(8, 2)
    g_AIDebugInfo[6] = '[6] AI:ZHZBCheckHP()'

    -- 自定义动作8
    action = ai.NewAction(8, 2008)
    action.SetBranch(2)
    g_AIDebugInfo[8] = '[8] AI:ZHZBAddHP()'

    ai.SetInitState(1)
end

-- 调试方法
function AI:DebugOnCallAction(action_id)
    Log('[AI] Call action: ' .. g_AIDebugInfo[action_id])
end

-- 本AI图内自定义脚本
function AI:ZHZBCheckHP(pNpc, actionData)
    -- 输入自定义动作实现脚本
	if pNpc.GetCurrentLife() < pNpc.GetMaxLife() then
    	return 1
    end
    return 2
end

function AI:ZHZBAddHP(pNpc, actionData)
    -- 输入自定义动作实现脚本
	local nLife = pNpc.GetCurrentLife() + 100000;
	local nCurrentLife = (nLife < pNpc.GetMaxLife() and nLife or pNpc.GetMaxLife());
	pNpc.SetCurrentLife(nCurrentLife);
    return 1
end


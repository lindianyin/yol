-- 编写者：lidianyi
-- 版本号：9

Include('script/ai/customfunctions.lua')
Include('script/ai/ai_param.lua')
Include('script/ai/argumentstrings.lua')

local g_AIDebugInfo = {}

-- 自定义附加脚本
Include("\\script\\mission\\level\\room_base.lua");

function AI:Setup(ai)
    local state
    local action
    local param = AI.g_AIParam[ai.nAIType]

    -- 注册自定义动作
    ai.RegisterUserAction(2008, 'AI:ShouHaoGetBoss')

    -- 初始
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 2)

    -- 设置主定时器
    action = ai.NewAction(2, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(17)
    g_AIDebugInfo[2] = '[2] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(17, AI_ACTION.SET_STATE)
    action.SetParam(4) -- 待机
    g_AIDebugInfo[17] = '[17] SetParam(State4)'

    -- 待机
    state = ai.NewState(4)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 8)

    -- 自定义动作8
    action = ai.NewAction(8, 2008)
    action.SetBranch(10, 2)
    g_AIDebugInfo[8] = '[8] AI:ShouHaoGetBoss()'

    -- 释放技能
    action = ai.NewAction(10, AI_ACTION.CAST_SKILL)
    action.SetParam(722, 1)
    action.SetBranch(12, 12)
    g_AIDebugInfo[10] = '[10] CAST_SKILL(722, 1)'

    -- 设置状态
    action = ai.NewAction(12, AI_ACTION.SET_STATE)
    action.SetParam(1)
    g_AIDebugInfo[12] = '[12] SET_STATE(1)'

    ai.SetInitState(1)
end

-- 调试方法
function AI:DebugOnCallAction(action_id)
    Log('[AI] Call action: ' .. g_AIDebugInfo[action_id])
end

-- 本AI图内自定义脚本
function AI:ShouHaoGetBoss(pNpc, actionData)
    -- 输入自定义动作实现脚本
    local tbPos = pNpc.GetPosition();
    local pBoss = KGameBase.GetNpcByNickName(tbPos.dwSceneId,"shouhao");
    if pBoss then
    	pNpc.SetTarget(3,pBoss.dwId);
    	return 1
    end
	return 2
end


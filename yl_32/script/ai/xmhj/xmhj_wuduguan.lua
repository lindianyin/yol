-- 编写者：chenkangyin
-- 版本号：5

Include('script/ai/customfunctions.lua')
Include('script/ai/ai_param.lua')
Include('script/ai/argumentstrings.lua')

local g_AIDebugInfo = {}

function AI:Setup(ai)
    local state
    local action
    local param = AI.g_AIParam[ai.nAIType]

    -- 注册自定义动作
    ai.RegisterUserAction(2006, 'AI:GetWuduguan')
    ai.RegisterUserAction(2011, 'AI:GetBossWuOu')

    -- 初始
    state = ai.NewState(2)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 3)

    -- 设置主定时器
    action = ai.NewAction(3, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(19)
    g_AIDebugInfo[3] = '[3] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(19, AI_ACTION.SET_STATE)
    action.SetParam(1) -- 待机
    g_AIDebugInfo[19] = '[19] SetParam(State1)'

    -- 待机
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)

    -- 选中自己
    action = ai.NewAction(6, 2006)
    action.SetBranch(8)
    g_AIDebugInfo[6] = '[6] AI:GetWuduguan()'

    -- 释放技能
    action = ai.NewAction(8, AI_ACTION.CAST_SKILL)
    action.SetParam(598, 1)
    action.SetBranch(11, 11)
    g_AIDebugInfo[8] = '[8] CAST_SKILL(598, 1)'

    -- 选中巫偶BOSS
    action = ai.NewAction(11, 2011)
    action.SetBranch(13, 13)
    g_AIDebugInfo[11] = '[11] AI:GetBossWuOu()'

    -- 释放技能
    action = ai.NewAction(13, AI_ACTION.CAST_SKILL)
    action.SetParam(597, 1)
    action.SetBranch(15, 15)
    g_AIDebugInfo[13] = '[13] CAST_SKILL(597, 1)'

    -- 设置状态
    action = ai.NewAction(15, AI_ACTION.SET_STATE)
    action.SetParam(1)
    g_AIDebugInfo[15] = '[15] SET_STATE(1)'

    ai.SetInitState(2)
end

-- 调试方法
function AI:DebugOnCallAction(action_id)
    Log('[AI] Call action: ' .. g_AIDebugInfo[action_id])
end

-- 本AI图内自定义脚本
function AI:GetWuduguan(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

function AI:GetBossWuOu(pNpc, actionData)
    -- 输入自定义动作实现脚本
    local tbPos = pNpc.GetPosition();
    local pBoss = KGameBase.GetNpcByNickName(tbPos.dwSceneId,"wuou");
    if pBoss then
    	pNpc.SetTarget(3,pBoss.dwId);
    	return 1
    end
	return 2
end


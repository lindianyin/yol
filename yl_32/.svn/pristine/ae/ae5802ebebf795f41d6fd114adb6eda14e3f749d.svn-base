-- 编写者：hejingxian
-- 版本号：17

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
    ai.RegisterUserAction(2181, 'AI:CustomAction181')
    ai.RegisterUserAction(2183, 'AI:CustomAction183')
    ai.RegisterUserAction(2186, 'AI:CustomAction186')
    ai.RegisterUserAction(2195, 'AI:CustomAction195')
    ai.RegisterUserAction(2205, 'CustomAction205')
    ai.RegisterUserAction(2209, 'CustomAction209')

    -- 多人副本-仙女
    state = ai.NewState(46)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 127)

    -- 记录原点
    action = ai.NewAction(127, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(52)
    g_AIDebugInfo[127] = '[127] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(52, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(231)
    g_AIDebugInfo[52] = '[52] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(231, AI_ACTION.SET_STATE)
    action.SetParam(140) -- 待机状态
    g_AIDebugInfo[231] = '[231] SetParam(State140)'

    -- 待机状态
    state = ai.NewState(140)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 146)

    -- 站立
    action = ai.NewAction(146, AI_ACTION.STAND)
    action.SetBranch(141)
    g_AIDebugInfo[146] = '[146] STAND()'

    -- 检查生命
    action = ai.NewAction(141, AI_ACTION.CHECK_LIFE)
    action.SetParam(99)
    action.SetBranch(186, 186, 115)
    g_AIDebugInfo[141] = '[141] CHECK_LIFE(99)'

    -- NPC喊话
    action = ai.NewAction(186, 2186)
    action.SetBranch(187)
    g_AIDebugInfo[186] = '[186] AI:CustomAction186()'

    -- 设置主定时器
    action = ai.NewAction(187, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(233)
    g_AIDebugInfo[187] = '[187] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(233, AI_ACTION.SET_STATE)
    action.SetParam(188) -- 喊话结束
    g_AIDebugInfo[233] = '[233] SetParam(State188)'

    -- 喊话结束
    state = ai.NewState(188)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 181)

    -- 选中自己
    action = ai.NewAction(181, 2181)
    action.SetBranch(178)
    g_AIDebugInfo[181] = '[181] AI:CustomAction181()'

    -- 释放技能
    action = ai.NewAction(178, AI_ACTION.CAST_SKILL)
    action.SetParam(1621, 1)
    action.SetBranch(123, 123)
    g_AIDebugInfo[178] = '[178] CAST_SKILL(1621, 1)'

    -- 设置主定时器
    action = ai.NewAction(123, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(235)
    g_AIDebugInfo[123] = '[123] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(235, AI_ACTION.SET_STATE)
    action.SetParam(72) -- 身体开始着火
    g_AIDebugInfo[235] = '[235] SetParam(State72)'

    -- 身体开始着火
    state = ai.NewState(72)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 154)

    -- 检查生命
    action = ai.NewAction(154, AI_ACTION.CHECK_LIFE)
    action.SetParam(50)
    action.SetBranch(195, 195, 165)
    g_AIDebugInfo[154] = '[154] CHECK_LIFE(50)'

    -- 设置主定时器
    action = ai.NewAction(165, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(237)
    g_AIDebugInfo[165] = '[165] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(237, AI_ACTION.SET_STATE)
    action.SetParam(157) -- 继续站立
    g_AIDebugInfo[237] = '[237] SetParam(State157)'

    -- 继续站立
    state = ai.NewState(157)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 161)

    -- 站立
    action = ai.NewAction(161, AI_ACTION.STAND)
    action.SetBranch(154)
    g_AIDebugInfo[161] = '[161] STAND()'

    -- NPC喊话
    action = ai.NewAction(195, 2195)
    action.SetBranch(196)
    g_AIDebugInfo[195] = '[195] AI:CustomAction195()'

    -- 设置主定时器
    action = ai.NewAction(196, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(239)
    g_AIDebugInfo[196] = '[196] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(239, AI_ACTION.SET_STATE)
    action.SetParam(192) -- 喊话结束
    g_AIDebugInfo[239] = '[239] SetParam(State192)'

    -- 喊话结束
    state = ai.NewState(192)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 183)

    -- 选中自己
    action = ai.NewAction(183, 2183)
    action.SetBranch(159)
    g_AIDebugInfo[183] = '[183] AI:CustomAction183()'

    -- 释放技能
    action = ai.NewAction(159, AI_ACTION.CAST_SKILL)
    action.SetParam(1622, 1)
    action.SetBranch(158, 158)
    g_AIDebugInfo[159] = '[159] CAST_SKILL(1622, 1)'

    -- 设置主定时器
    action = ai.NewAction(158, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(241)
    g_AIDebugInfo[158] = '[158] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(241, AI_ACTION.SET_STATE)
    action.SetParam(155) -- 身体着大火
    g_AIDebugInfo[241] = '[241] SetParam(State155)'

    -- 身体着大火
    state = ai.NewState(155)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 204)

    -- 检查生命
    action = ai.NewAction(204, AI_ACTION.CHECK_LIFE)
    action.SetParam(20)
    action.SetBranch(205, 205, 224)
    g_AIDebugInfo[204] = '[204] CHECK_LIFE(20)'

    -- NPC喊话
    action = ai.NewAction(205, 2205)
    action.SetBranch(206)
    g_AIDebugInfo[205] = '[205] CustomAction205()'

    -- 设置主定时器
    action = ai.NewAction(206, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(243)
    g_AIDebugInfo[206] = '[206] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(243, AI_ACTION.SET_STATE)
    action.SetParam(201) -- 喊话结束
    g_AIDebugInfo[243] = '[243] SetParam(State201)'

    -- 喊话结束
    state = ai.NewState(201)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 209)

    -- 选中自己
    action = ai.NewAction(209, 2209)
    action.SetBranch(225)
    g_AIDebugInfo[209] = '[209] CustomAction209()'

    -- 释放技能
    action = ai.NewAction(225, AI_ACTION.CAST_SKILL)
    action.SetParam(1622, 1)
    action.SetBranch(227, 227)
    g_AIDebugInfo[225] = '[225] CAST_SKILL(1622, 1)'

    -- 设置主定时器
    action = ai.NewAction(227, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(245)
    g_AIDebugInfo[227] = '[227] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(245, AI_ACTION.SET_STATE)
    action.SetParam(202) -- 身体着大火
    g_AIDebugInfo[245] = '[245] SetParam(State202)'

    -- 身体着大火
    state = ai.NewState(202)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 220)

    -- 设置主定时器
    action = ai.NewAction(220, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(247)
    g_AIDebugInfo[220] = '[220] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(247, AI_ACTION.SET_STATE)
    action.SetParam(214) -- 继续站立
    g_AIDebugInfo[247] = '[247] SetParam(State214)'

    -- 继续站立
    state = ai.NewState(214)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 199)

    -- 站立
    action = ai.NewAction(199, AI_ACTION.STAND)
    action.SetBranch(220)
    g_AIDebugInfo[199] = '[199] STAND()'

    -- 设置主定时器
    action = ai.NewAction(224, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(249)
    g_AIDebugInfo[224] = '[224] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(249, AI_ACTION.SET_STATE)
    action.SetParam(211) -- 继续站立
    g_AIDebugInfo[249] = '[249] SetParam(State211)'

    -- 继续站立
    state = ai.NewState(211)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 218)

    -- 站立
    action = ai.NewAction(218, AI_ACTION.STAND)
    action.SetBranch(204)
    g_AIDebugInfo[218] = '[218] STAND()'

    -- 设置主定时器
    action = ai.NewAction(115, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(251)
    g_AIDebugInfo[115] = '[115] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(251, AI_ACTION.SET_STATE)
    action.SetParam(1) -- 继续站立
    g_AIDebugInfo[251] = '[251] SetParam(State1)'

    -- 继续站立
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 148)

    -- 站立
    action = ai.NewAction(148, AI_ACTION.STAND)
    action.SetBranch(141)
    g_AIDebugInfo[148] = '[148] STAND()'

    ai.SetInitState(46)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction186(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
	return 1
end

function AI:CustomAction181(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

function AI:CustomAction195(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
	return 1
end

function AI:CustomAction183(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

function AI:CustomAction205(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout3)
    return 1
end

function AI:CustomAction209(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


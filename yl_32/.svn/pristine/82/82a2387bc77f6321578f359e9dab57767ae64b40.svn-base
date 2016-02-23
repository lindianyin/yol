-- 编写者：hejingxian
-- 版本号：7

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
    ai.RegisterUserAction(2022, 'AI:CustomAction22')
    ai.RegisterUserAction(2061, 'AI:CustomAction61')
    ai.RegisterUserAction(2106, 'AI:CustomAction106')

    -- 初始
    state = ai.NewState(11)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 19)

    -- 记录原点
    action = ai.NewAction(19, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(2)
    g_AIDebugInfo[19] = '[19] RECORD_ORIGIN_POSITION()'

    -- 记录返回点
    action = ai.NewAction(2, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(16)
    g_AIDebugInfo[2] = '[2] RECORD_RETURN_POSITION()'

    -- 进入巡逻路径
    action = ai.NewAction(16, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(1, param['PatrolOrderIndex'])
    action.SetBranch(9)
    g_AIDebugInfo[16] = '[16] ENTER_PATROL_PATH(1, param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(9, AI_ACTION.DO_PATROL)
    action.SetBranch(10, 22)
    g_AIDebugInfo[9] = '[9] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(10, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(110)
    g_AIDebugInfo[10] = '[10] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(110, AI_ACTION.SET_STATE)
    action.SetParam(1) -- 第一段路
    g_AIDebugInfo[110] = '[110] SetParam(State1)'

    -- 第一段路
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 10)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 9)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 20)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 9)

    -- 设置副定时器
    action = ai.NewAction(20, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(10)
    g_AIDebugInfo[20] = '[20] SET_SECONDARY_TIMER(16)'

    -- 选中自己
    action = ai.NewAction(22, 2022)
    action.SetBranch(23)
    g_AIDebugInfo[22] = '[22] AI:CustomAction22()'

    -- 释放技能
    action = ai.NewAction(23, AI_ACTION.CAST_SKILL)
    action.SetParam(1651, 1)
    action.SetBranch(25, 25)
    g_AIDebugInfo[23] = '[23] CAST_SKILL(1651, 1)'

    -- 设置主定时器
    action = ai.NewAction(25, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(112)
    g_AIDebugInfo[25] = '[25] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(112, AI_ACTION.SET_STATE)
    action.SetParam(24) -- 释放定身技能结束
    g_AIDebugInfo[112] = '[112] SetParam(State24)'

    -- 释放定身技能结束
    state = ai.NewState(24)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 47)

    -- 进入巡逻路径
    action = ai.NewAction(47, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(2, param['PatrolOrderIndex'])
    action.SetBranch(46)
    g_AIDebugInfo[47] = '[47] ENTER_PATROL_PATH(2, param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(46, AI_ACTION.DO_PATROL)
    action.SetBranch(55, 61)
    g_AIDebugInfo[46] = '[46] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(55, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(114)
    g_AIDebugInfo[55] = '[55] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(114, AI_ACTION.SET_STATE)
    action.SetParam(56) -- 第二段路
    g_AIDebugInfo[114] = '[114] SetParam(State56)'

    -- 第二段路
    state = ai.NewState(56)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 45)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 55)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 46)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 46)

    -- 设置副定时器
    action = ai.NewAction(45, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(55)
    g_AIDebugInfo[45] = '[45] SET_SECONDARY_TIMER(16)'

    -- 选中自己
    action = ai.NewAction(61, 2061)
    action.SetBranch(65)
    g_AIDebugInfo[61] = '[61] AI:CustomAction61()'

    -- 释放技能
    action = ai.NewAction(65, AI_ACTION.CAST_SKILL)
    action.SetParam(1651, 1)
    action.SetBranch(64, 64)
    g_AIDebugInfo[65] = '[65] CAST_SKILL(1651, 1)'

    -- 设置主定时器
    action = ai.NewAction(64, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(116)
    g_AIDebugInfo[64] = '[64] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(116, AI_ACTION.SET_STATE)
    action.SetParam(62) -- 释放定身技能结束
    g_AIDebugInfo[116] = '[116] SetParam(State62)'

    -- 释放定身技能结束
    state = ai.NewState(62)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 79)

    -- 进入巡逻路径
    action = ai.NewAction(79, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(3, param['PatrolOrderIndex'])
    action.SetBranch(67)
    g_AIDebugInfo[79] = '[79] ENTER_PATROL_PATH(3, param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(67, AI_ACTION.DO_PATROL)
    action.SetBranch(71, 106)
    g_AIDebugInfo[67] = '[67] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(71, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(118)
    g_AIDebugInfo[71] = '[71] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(118, AI_ACTION.SET_STATE)
    action.SetParam(76) -- 第三段路
    g_AIDebugInfo[118] = '[118] SetParam(State76)'

    -- 第三段路
    state = ai.NewState(76)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 69)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 71)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 67)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 67)

    -- 设置副定时器
    action = ai.NewAction(69, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(71)
    g_AIDebugInfo[69] = '[69] SET_SECONDARY_TIMER(16)'

    -- 选中自己
    action = ai.NewAction(106, 2106)
    action.SetBranch(105)
    g_AIDebugInfo[106] = '[106] AI:CustomAction106()'

    -- 释放技能
    action = ai.NewAction(105, AI_ACTION.CAST_SKILL)
    action.SetParam(1651, 1)
    action.SetBranch(85, 85)
    g_AIDebugInfo[105] = '[105] CAST_SKILL(1651, 1)'

    -- 设置主定时器
    action = ai.NewAction(85, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(120)
    g_AIDebugInfo[85] = '[85] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(120, AI_ACTION.SET_STATE)
    action.SetParam(87) -- 释放定身技能结束
    g_AIDebugInfo[120] = '[120] SetParam(State87)'

    -- 释放定身技能结束
    state = ai.NewState(87)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 89)

    -- 进入巡逻路径
    action = ai.NewAction(89, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(4, param['PatrolOrderIndex'])
    action.SetBranch(94)
    g_AIDebugInfo[89] = '[89] ENTER_PATROL_PATH(4, param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(94, AI_ACTION.DO_PATROL)
    action.SetBranch(88, 88)
    g_AIDebugInfo[94] = '[94] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(88, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(122)
    g_AIDebugInfo[88] = '[88] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(122, AI_ACTION.SET_STATE)
    action.SetParam(91) -- 第四段路
    g_AIDebugInfo[122] = '[122] SetParam(State91)'

    -- 第四段路
    state = ai.NewState(91)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 94)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 88)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 94)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 86)

    -- 设置副定时器
    action = ai.NewAction(86, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(88)
    g_AIDebugInfo[86] = '[86] SET_SECONDARY_TIMER(16)'

    ai.SetInitState(11)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction22(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

function AI:CustomAction61(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

function AI:CustomAction106(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


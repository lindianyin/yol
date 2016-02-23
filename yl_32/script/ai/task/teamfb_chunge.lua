-- 编写者：hejingxian
-- 版本号：19

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
    ai.RegisterUserAction(2115, 'CustomAction115')
    ai.RegisterUserAction(2122, 'CustomAction122')
    ai.RegisterUserAction(2127, 'AI:CustomAction127')
    ai.RegisterUserAction(2129, 'AI:CustomAction129')
    ai.RegisterUserAction(2131, 'AI:CustomAction131')
    ai.RegisterUserAction(2135, 'AI:CustomAction135')

    -- 初始
    state = ai.NewState(45)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 36)

    -- 记录原点
    action = ai.NewAction(36, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(40)
    g_AIDebugInfo[36] = '[36] RECORD_ORIGIN_POSITION()'

    -- 记录返回点
    action = ai.NewAction(40, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(122)
    g_AIDebugInfo[40] = '[40] RECORD_RETURN_POSITION()'

    -- 选中自己
    action = ai.NewAction(122, 2122)
    action.SetBranch(125)
    g_AIDebugInfo[122] = '[122] CustomAction122()'

    -- 释放技能
    action = ai.NewAction(125, AI_ACTION.CAST_SKILL)
    action.SetParam(1626, 1)
    action.SetBranch(50, 50)
    g_AIDebugInfo[125] = '[125] CAST_SKILL(1626, 1)'

    -- 进入巡逻路径
    action = ai.NewAction(50, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(41)
    g_AIDebugInfo[50] = '[50] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(41, AI_ACTION.DO_PATROL)
    action.SetBranch(42, 115)
    g_AIDebugInfo[41] = '[41] DO_PATROL()'

    -- 选中自己
    action = ai.NewAction(115, 2115)
    action.SetBranch(112)
    g_AIDebugInfo[115] = '[115] CustomAction115()'

    -- 释放技能
    action = ai.NewAction(112, AI_ACTION.CAST_SKILL)
    action.SetParam(1620, 1)
    action.SetBranch(140, 138)
    g_AIDebugInfo[112] = '[112] CAST_SKILL(1620, 1)'

    -- 设置状态
    action = ai.NewAction(138, AI_ACTION.SET_STATE)
    action.SetParam(109) -- 自杀
    g_AIDebugInfo[138] = '[138] SetParam(State109)'

    -- 自杀
    state = ai.NewState(109)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 110)

    -- 设置状态
    action = ai.NewAction(110, AI_ACTION.SET_STATE)
    action.SetParam(4)
    g_AIDebugInfo[110] = '[110] SET_STATE(4)'

    -- 设置状态
    action = ai.NewAction(140, AI_ACTION.SET_STATE)
    action.SetParam(109) -- 自杀
    g_AIDebugInfo[140] = '[140] SetParam(State109)'

    -- 设置主定时器
    action = ai.NewAction(42, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(142)
    g_AIDebugInfo[42] = '[42] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(142, AI_ACTION.SET_STATE)
    action.SetParam(43) -- 主状态
    g_AIDebugInfo[142] = '[142] SetParam(State43)'

    -- 主状态
    state = ai.NewState(43)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 41)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 41)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 47)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 42)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 106)

    -- 设置副定时器
    action = ai.NewAction(47, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(42)
    g_AIDebugInfo[47] = '[47] SET_SECONDARY_TIMER(16)'

    -- 选择事件发起者
    action = ai.NewAction(106, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(57, 57)
    g_AIDebugInfo[106] = '[106] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(57, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(62)
    g_AIDebugInfo[57] = '[57] ADD_TARGET_TO_THREAT_LIST()'

    -- Npc初始化技能CD
    action = ai.NewAction(62, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(65)
    g_AIDebugInfo[62] = '[62] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(65, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(144)
    g_AIDebugInfo[65] = '[65] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(144, AI_ACTION.SET_STATE)
    action.SetParam(64) -- 三选一技能
    g_AIDebugInfo[144] = '[144] SetParam(State64)'

    -- 三选一技能
    state = ai.NewState(64)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 63)

    -- 选择第一威胁
    action = ai.NewAction(63, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(133, 133)
    g_AIDebugInfo[63] = '[63] SELECT_FIRST_THREAT()'

    -- 双选项随机
    action = ai.NewAction(133, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(80, 20)
    action.SetBranch(59, 135)
    g_AIDebugInfo[133] = '[133] RANDOM_BI_BRANCH(50, 50)'

    -- 三选项随机
    action = ai.NewAction(59, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(30, 30, 40)
    action.SetBranch(131, 129, 127)
    g_AIDebugInfo[59] = '[59] RANDOM_TRI_BRANCH(30, 30, 40)'

    -- NPC喊话
    action = ai.NewAction(131, 2131)
    action.SetBranch(70)
    g_AIDebugInfo[131] = '[131] AI:CustomAction131()'

    -- 释放技能
    action = ai.NewAction(70, AI_ACTION.CAST_SKILL)
    action.SetParam(1617, 1)
    action.SetBranch(76, 76)
    g_AIDebugInfo[70] = '[70] CAST_SKILL(1617, 1)'

    -- 设置主定时器
    action = ai.NewAction(76, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(146)
    g_AIDebugInfo[76] = '[76] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(146, AI_ACTION.SET_STATE)
    action.SetParam(85) -- 技能1617结束
    g_AIDebugInfo[146] = '[146] SetParam(State85)'

    -- 技能1617结束
    state = ai.NewState(85)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 91)

    -- 设置主定时器
    action = ai.NewAction(91, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(148)
    g_AIDebugInfo[91] = '[91] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(148, AI_ACTION.SET_STATE)
    action.SetParam(96) -- 释放自杀技能
    g_AIDebugInfo[148] = '[148] SetParam(State96)'

    -- 释放自杀技能
    state = ai.NewState(96)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 115)

    -- NPC喊话
    action = ai.NewAction(129, 2129)
    action.SetBranch(71)
    g_AIDebugInfo[129] = '[129] AI:CustomAction129()'

    -- 释放技能
    action = ai.NewAction(71, AI_ACTION.CAST_SKILL)
    action.SetParam(1618, 1)
    action.SetBranch(77, 77)
    g_AIDebugInfo[71] = '[71] CAST_SKILL(1618, 1)'

    -- 设置主定时器
    action = ai.NewAction(77, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(150)
    g_AIDebugInfo[77] = '[77] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(150, AI_ACTION.SET_STATE)
    action.SetParam(87) -- 技能1618结束
    g_AIDebugInfo[150] = '[150] SetParam(State87)'

    -- 技能1618结束
    state = ai.NewState(87)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 91)

    -- NPC喊话
    action = ai.NewAction(127, 2127)
    action.SetBranch(72)
    g_AIDebugInfo[127] = '[127] AI:CustomAction127()'

    -- 释放技能
    action = ai.NewAction(72, AI_ACTION.CAST_SKILL)
    action.SetParam(1619, 1)
    action.SetBranch(78, 78)
    g_AIDebugInfo[72] = '[72] CAST_SKILL(1619, 1)'

    -- 设置主定时器
    action = ai.NewAction(78, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(152)
    g_AIDebugInfo[78] = '[78] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(152, AI_ACTION.SET_STATE)
    action.SetParam(86) -- 技能1619结束
    g_AIDebugInfo[152] = '[152] SetParam(State86)'

    -- 技能1619结束
    state = ai.NewState(86)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 91)

    -- NPC喊话
    action = ai.NewAction(135, 2135)
    action.SetBranch(91)
    g_AIDebugInfo[135] = '[135] AI:CustomAction135()'

    ai.SetInitState(45)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction122(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

function AI:CustomAction115(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

function AI:CustomAction131(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction129(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end

function AI:CustomAction127(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout3)
    return 1
end

function AI:CustomAction135(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, "哼！你打我干嘛啊！")
    return 1
end


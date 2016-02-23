-- 编写者：hejingxian
-- 版本号：26

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
    ai.RegisterUserAction(2070, 'AI:CustomAction70')
    ai.RegisterUserAction(2105, 'AI:CustomAction105')
    ai.RegisterUserAction(2113, 'AI:CustomAction113')

    -- 初始
    state = ai.NewState(63)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 84)

    -- 记录原点
    action = ai.NewAction(84, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(20)
    g_AIDebugInfo[84] = '[84] RECORD_ORIGIN_POSITION()'

    -- 进入巡逻路径
    action = ai.NewAction(20, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(64)
    g_AIDebugInfo[20] = '[20] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(64, AI_ACTION.DO_PATROL)
    action.SetBranch(32, 32)
    g_AIDebugInfo[64] = '[64] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(32, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(216)
    g_AIDebugInfo[32] = '[32] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(216, AI_ACTION.SET_STATE)
    action.SetParam(15) -- 寻路结束
    g_AIDebugInfo[216] = '[216] SetParam(State15)'

    -- 寻路结束
    state = ai.NewState(15)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 49)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 64)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 71)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 64)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 105)

    -- 设置副定时器
    action = ai.NewAction(49, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(32)
    g_AIDebugInfo[49] = '[49] SET_SECONDARY_TIMER(16)'

    -- 搜索敌人
    action = ai.NewAction(71, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(113, 32)
    g_AIDebugInfo[71] = '[71] SEARCH_ENEMY(param[AlertRange])'

    -- 选中自己
    action = ai.NewAction(113, 2113)
    action.SetBranch(112)
    g_AIDebugInfo[113] = '[113] AI:CustomAction113()'

    -- 释放技能
    action = ai.NewAction(112, AI_ACTION.CAST_SKILL)
    action.SetParam(1640, 1)
    action.SetBranch(70, 70)
    g_AIDebugInfo[112] = '[112] CAST_SKILL(1640, 1)'

    -- NPC喊话
    action = ai.NewAction(70, 2070)
    action.SetBranch(23)
    g_AIDebugInfo[70] = '[70] AI:CustomAction70()'

    -- 设置主定时器
    action = ai.NewAction(23, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(218)
    g_AIDebugInfo[23] = '[23] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(218, AI_ACTION.SET_STATE)
    action.SetParam(25) -- 喊话结束
    g_AIDebugInfo[218] = '[218] SetParam(State25)'

    -- 喊话结束
    state = ai.NewState(25)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 109)

    -- 搜索敌人
    action = ai.NewAction(109, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(6000)
    action.SetBranch(69, 32)
    g_AIDebugInfo[109] = '[109] SEARCH_ENEMY(6000)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(69, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(85)
    g_AIDebugInfo[69] = '[69] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(85, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(3)
    g_AIDebugInfo[85] = '[85] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(3, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(86)
    g_AIDebugInfo[3] = '[3] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(86, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(220)
    g_AIDebugInfo[86] = '[86] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(220, AI_ACTION.SET_STATE)
    action.SetParam(43) -- 战斗状态
    g_AIDebugInfo[220] = '[220] SetParam(State43)'

    -- 战斗状态
    state = ai.NewState(43)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 74)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 128)

    -- 选择第一威胁
    action = ai.NewAction(74, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(39, 128)
    g_AIDebugInfo[74] = '[74] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(39, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(19, 86)
    g_AIDebugInfo[39] = '[39] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(19, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(56, 6)
    g_AIDebugInfo[19] = '[19] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(6, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(222)
    g_AIDebugInfo[6] = '[6] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(222, AI_ACTION.SET_STATE)
    action.SetParam(29) -- 追击状态
    g_AIDebugInfo[222] = '[222] SetParam(State29)'

    -- 追击状态
    state = ai.NewState(29)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 27)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 128)

    -- 比较返回点距离
    action = ai.NewAction(27, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(128, 19, 19)
    g_AIDebugInfo[27] = '[27] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 站立
    action = ai.NewAction(128, AI_ACTION.STAND)
    action.SetBranch(135)
    g_AIDebugInfo[128] = '[128] STAND()'

    -- 设置主定时器
    action = ai.NewAction(135, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(224)
    g_AIDebugInfo[135] = '[135] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(224, AI_ACTION.SET_STATE)
    action.SetParam(130) -- 待机状态
    g_AIDebugInfo[224] = '[224] SetParam(State130)'

    -- 待机状态
    state = ai.NewState(130)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 118)

    -- 搜索敌人
    action = ai.NewAction(118, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(6000)
    action.SetBranch(119, 128)
    g_AIDebugInfo[118] = '[118] SEARCH_ENEMY(6000)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(119, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(120)
    g_AIDebugInfo[119] = '[119] ADD_TARGET_TO_THREAT_LIST()'

    -- 选择第一威胁
    action = ai.NewAction(120, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(39, 128)
    g_AIDebugInfo[120] = '[120] SELECT_FIRST_THREAT()'

    -- Npc释放选定技能
    action = ai.NewAction(56, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(86, 66)
    g_AIDebugInfo[56] = '[56] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(66, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(200)
    action.SetBranch(74, 128)
    g_AIDebugInfo[66] = '[66] CHECK_SKILL_FAILED_COUNTER(200)'

    -- 选中自己
    action = ai.NewAction(105, 2105)
    action.SetBranch(104)
    g_AIDebugInfo[105] = '[105] AI:CustomAction105()'

    -- 释放技能
    action = ai.NewAction(104, AI_ACTION.CAST_SKILL)
    action.SetParam(1640, 1)
    action.SetBranch(1, 1)
    g_AIDebugInfo[104] = '[104] CAST_SKILL(1640, 1)'

    -- 选择事件发起者
    action = ai.NewAction(1, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(69, 69)
    g_AIDebugInfo[1] = '[1] SELECT_EVENT_SRC()'

    ai.SetInitState(63)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction113(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

function AI:CustomAction70(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction105(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


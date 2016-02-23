-- 编写者：hejingxian
-- 版本号：10

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
    ai.RegisterUserAction(2123, 'AI:CustomAction123')

    -- 初始
    state = ai.NewState(13)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 37)

    -- 记录原点
    action = ai.NewAction(37, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(15)
    g_AIDebugInfo[37] = '[37] RECORD_ORIGIN_POSITION()'

    -- 记录返回点
    action = ai.NewAction(15, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(123)
    g_AIDebugInfo[15] = '[15] RECORD_RETURN_POSITION()'

    -- 选中自己
    action = ai.NewAction(123, 2123)
    action.SetBranch(124)
    g_AIDebugInfo[123] = '[123] AI:CustomAction123()'

    -- 释放技能
    action = ai.NewAction(124, AI_ACTION.CAST_SKILL)
    action.SetParam(1626, 1)
    action.SetBranch(47, 47)
    g_AIDebugInfo[124] = '[124] CAST_SKILL(1626, 1)'

    -- 进入巡逻路径
    action = ai.NewAction(47, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(73)
    g_AIDebugInfo[47] = '[47] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(73, AI_ACTION.DO_PATROL)
    action.SetBranch(14, 22)
    g_AIDebugInfo[73] = '[73] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(14, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(140)
    g_AIDebugInfo[14] = '[14] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(140, AI_ACTION.SET_STATE)
    action.SetParam(42) -- 主状态
    g_AIDebugInfo[140] = '[140] SetParam(State42)'

    -- 主状态
    state = ai.NewState(42)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 73)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 73)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 93)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 27)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 14)

    -- 选择事件发起者
    action = ai.NewAction(93, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(117, 117)
    g_AIDebugInfo[93] = '[93] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(117, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(116)
    g_AIDebugInfo[117] = '[117] ADD_TARGET_TO_THREAT_LIST()'

    -- Npc初始化技能CD
    action = ai.NewAction(116, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(105)
    g_AIDebugInfo[116] = '[116] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(105, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(142)
    g_AIDebugInfo[105] = '[105] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(142, AI_ACTION.SET_STATE)
    action.SetParam(79) -- 三选一技能
    g_AIDebugInfo[142] = '[142] SetParam(State79)'

    -- 三选一技能
    state = ai.NewState(79)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 76)

    -- 选择第一威胁
    action = ai.NewAction(76, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(100, 100)
    g_AIDebugInfo[76] = '[76] SELECT_FIRST_THREAT()'

    -- 双选项随机
    action = ai.NewAction(100, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(50, 50)
    action.SetBranch(88, 88)
    g_AIDebugInfo[100] = '[100] RANDOM_BI_BRANCH(50, 50)'

    -- 三选项随机
    action = ai.NewAction(88, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(30, 30, 40)
    action.SetBranch(85, 107, 98)
    g_AIDebugInfo[88] = '[88] RANDOM_TRI_BRANCH(30, 30, 40)'

    -- 释放技能
    action = ai.NewAction(107, AI_ACTION.CAST_SKILL)
    action.SetParam(1618, 1)
    action.SetBranch(104, 104)
    g_AIDebugInfo[107] = '[107] CAST_SKILL(1618, 1)'

    -- 设置主定时器
    action = ai.NewAction(104, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(144)
    g_AIDebugInfo[104] = '[104] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(144, AI_ACTION.SET_STATE)
    action.SetParam(87) -- 技能1618结束
    g_AIDebugInfo[144] = '[144] SetParam(State87)'

    -- 技能1618结束
    state = ai.NewState(87)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 101)

    -- 设置主定时器
    action = ai.NewAction(101, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(146)
    g_AIDebugInfo[101] = '[101] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(146, AI_ACTION.SET_STATE)
    action.SetParam(120) -- 释放自杀技能
    g_AIDebugInfo[146] = '[146] SetParam(State120)'

    -- 释放自杀技能
    state = ai.NewState(120)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 130)

    -- 巡逻
    action = ai.NewAction(130, AI_ACTION.DO_PATROL)
    action.SetBranch(128, 22)
    g_AIDebugInfo[130] = '[130] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(128, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(148)
    g_AIDebugInfo[128] = '[128] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(148, AI_ACTION.SET_STATE)
    action.SetParam(129) -- 主状态
    g_AIDebugInfo[148] = '[148] SetParam(State129)'

    -- 主状态
    state = ai.NewState(129)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 130)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 131)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 130)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 128)

    -- 设置副定时器
    action = ai.NewAction(131, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(128)
    g_AIDebugInfo[131] = '[131] SET_SECONDARY_TIMER(16)'

    -- 选中自己
    action = ai.NewAction(22, 2022)
    action.SetBranch(70)
    g_AIDebugInfo[22] = '[22] AI:CustomAction22()'

    -- 释放技能
    action = ai.NewAction(70, AI_ACTION.CAST_SKILL)
    action.SetParam(1620, 1)
    action.SetBranch(152, 150)
    g_AIDebugInfo[70] = '[70] CAST_SKILL(1620, 1)'

    -- 设置状态
    action = ai.NewAction(150, AI_ACTION.SET_STATE)
    action.SetParam(32) -- 自杀
    g_AIDebugInfo[150] = '[150] SetParam(State32)'

    -- 自杀
    state = ai.NewState(32)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 18)

    -- 设置状态
    action = ai.NewAction(18, AI_ACTION.SET_STATE)
    action.SetParam(4)
    g_AIDebugInfo[18] = '[18] SET_STATE(4)'

    -- 设置状态
    action = ai.NewAction(152, AI_ACTION.SET_STATE)
    action.SetParam(32) -- 自杀
    g_AIDebugInfo[152] = '[152] SetParam(State32)'

    -- 释放技能
    action = ai.NewAction(85, AI_ACTION.CAST_SKILL)
    action.SetParam(1617, 1)
    action.SetBranch(77, 77)
    g_AIDebugInfo[85] = '[85] CAST_SKILL(1617, 1)'

    -- 设置主定时器
    action = ai.NewAction(77, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(154)
    g_AIDebugInfo[77] = '[77] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(154, AI_ACTION.SET_STATE)
    action.SetParam(108) -- 技能1617结束
    g_AIDebugInfo[154] = '[154] SetParam(State108)'

    -- 技能1617结束
    state = ai.NewState(108)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 101)

    -- 释放技能
    action = ai.NewAction(98, AI_ACTION.CAST_SKILL)
    action.SetParam(1619, 1)
    action.SetBranch(96, 96)
    g_AIDebugInfo[98] = '[98] CAST_SKILL(1619, 1)'

    -- 设置主定时器
    action = ai.NewAction(96, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(156)
    g_AIDebugInfo[96] = '[96] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(156, AI_ACTION.SET_STATE)
    action.SetParam(106) -- 技能1619结束
    g_AIDebugInfo[156] = '[156] SetParam(State106)'

    -- 技能1619结束
    state = ai.NewState(106)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 101)

    -- 设置副定时器
    action = ai.NewAction(27, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(14)
    g_AIDebugInfo[27] = '[27] SET_SECONDARY_TIMER(16)'

    ai.SetInitState(13)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction123(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end

function AI:CustomAction22(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


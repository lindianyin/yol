-- 编写者：hejingxian
-- 版本号：24

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
    ai.RegisterUserAction(2101, 'CustomAction101')

    -- 初始
    state = ai.NewState(56)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 49)

    -- 记录原点
    action = ai.NewAction(49, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(109)
    g_AIDebugInfo[49] = '[49] RECORD_ORIGIN_POSITION()'

    -- 选择状态
    action = ai.NewAction(109, AI_ACTION.SELECT_STATE)
    action.SetParam(1)
    action.SetBranch(67, 102, 67)
    g_AIDebugInfo[109] = '[109] SELECT_STATE(1)'

    -- 双选项随机
    action = ai.NewAction(102, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(5, 95)
    action.SetBranch(101, 18)
    g_AIDebugInfo[102] = '[102] RANDOM_BI_BRANCH(5, 95)'

    -- NPC喊话
    action = ai.NewAction(101, 2101)
    action.SetBranch(100)
    g_AIDebugInfo[101] = '[101] CustomAction101()'

    -- 设置主定时器
    action = ai.NewAction(100, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(117)
    g_AIDebugInfo[100] = '[100] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(117, AI_ACTION.SET_STATE)
    action.SetParam(99) -- 喊话结束
    g_AIDebugInfo[117] = '[117] SetParam(State99)'

    -- 喊话结束
    state = ai.NewState(99)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 18)

    -- 闲逛一次
    action = ai.NewAction(18, AI_ACTION.WANDER)
    action.SetParam(200, 300)
    action.SetBranch(96)
    g_AIDebugInfo[18] = '[18] WANDER(200, 300)'

    -- 设置副定时器
    action = ai.NewAction(96, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(48)
    action.SetBranch(113)
    g_AIDebugInfo[96] = '[96] SET_SECONDARY_TIMER(48)'

    -- 搜索敌人
    action = ai.NewAction(113, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(53, 67)
    g_AIDebugInfo[113] = '[113] SEARCH_ENEMY(param[AlertRange])'

    -- 设置主定时器
    action = ai.NewAction(67, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(119)
    g_AIDebugInfo[67] = '[67] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(119, AI_ACTION.SET_STATE)
    action.SetParam(33) -- 待机状态
    g_AIDebugInfo[119] = '[119] SetParam(State33)'

    -- 待机状态
    state = ai.NewState(33)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 44)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 68)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 109)

    -- 搜索敌人
    action = ai.NewAction(44, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(53, 67)
    g_AIDebugInfo[44] = '[44] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(53, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(78)
    g_AIDebugInfo[53] = '[53] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(78, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(71)
    g_AIDebugInfo[78] = '[78] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(71, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(54)
    g_AIDebugInfo[71] = '[71] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(54, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(121)
    g_AIDebugInfo[54] = '[54] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(121, AI_ACTION.SET_STATE)
    action.SetParam(75) -- 战斗状态
    g_AIDebugInfo[121] = '[121] SetParam(State75)'

    -- 战斗状态
    state = ai.NewState(75)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 22)

    -- 选择第一威胁
    action = ai.NewAction(22, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(41, 42)
    g_AIDebugInfo[22] = '[22] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(41, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(21, 54)
    g_AIDebugInfo[41] = '[41] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(21, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(50, 50)
    g_AIDebugInfo[21] = '[21] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(50, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(8, 77)
    g_AIDebugInfo[50] = '[50] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(77, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(72, 42)
    g_AIDebugInfo[77] = '[77] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 设置系统无敌
    action = ai.NewAction(42, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(6)
    g_AIDebugInfo[42] = '[42] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(6, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(66, 66)
    g_AIDebugInfo[6] = '[6] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(66, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(123)
    g_AIDebugInfo[66] = '[66] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(123, AI_ACTION.SET_STATE)
    action.SetParam(13) -- 无敌返回状态
    g_AIDebugInfo[123] = '[123] SetParam(State13)'

    -- 无敌返回状态
    state = ai.NewState(13)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 35)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 35)

    -- 返回
    action = ai.NewAction(35, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(29, 29)
    g_AIDebugInfo[35] = '[35] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(29, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(125)
    g_AIDebugInfo[29] = '[29] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(125, AI_ACTION.SET_STATE)
    action.SetParam(28) -- 返回成功
    g_AIDebugInfo[125] = '[125] SetParam(State28)'

    -- 返回成功
    state = ai.NewState(28)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 5)

    -- 清除DEBUFF
    action = ai.NewAction(5, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(65)
    g_AIDebugInfo[5] = '[5] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(65, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(109)
    g_AIDebugInfo[65] = '[65] SET_SYSTEM_SHEILD(0)'

    -- 选择第一威胁
    action = ai.NewAction(72, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(69, 42)
    g_AIDebugInfo[72] = '[72] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(69, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(73, 8)
    g_AIDebugInfo[69] = '[69] NPC_STANDARD_SKILL_SELECTOR()'

    -- 设置主定时器
    action = ai.NewAction(8, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(127)
    g_AIDebugInfo[8] = '[8] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(127, AI_ACTION.SET_STATE)
    action.SetParam(58) -- 战斗状态
    g_AIDebugInfo[127] = '[127] SetParam(State58)'

    -- 战斗状态
    state = ai.NewState(58)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 72)

    -- Npc保持技能释放距离
    action = ai.NewAction(73, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(50, 82)
    g_AIDebugInfo[73] = '[73] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(82, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(129)
    g_AIDebugInfo[82] = '[82] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(129, AI_ACTION.SET_STATE)
    action.SetParam(38) -- 追击状态
    g_AIDebugInfo[129] = '[129] SetParam(State38)'

    -- 追击状态
    state = ai.NewState(38)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 23)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 109)

    -- 比较返回点距离
    action = ai.NewAction(23, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(42, 73, 73)
    g_AIDebugInfo[23] = '[23] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 选择事件发起者
    action = ai.NewAction(68, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(53, 42)
    g_AIDebugInfo[68] = '[68] SELECT_EVENT_SRC()'

    ai.SetInitState(56)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction101(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end


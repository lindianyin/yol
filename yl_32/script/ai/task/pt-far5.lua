-- 编写者：hejingxian
-- 版本号：3

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
    ai.RegisterUserAction(2071, 'CustomAction71')

    -- 初始
    state = ai.NewState(8)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 36)

    -- 记录原点
    action = ai.NewAction(36, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(14)
    g_AIDebugInfo[36] = '[36] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(14, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(79)
    g_AIDebugInfo[14] = '[14] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(79, AI_ACTION.SET_STATE)
    action.SetParam(58) -- 待机状态
    g_AIDebugInfo[79] = '[79] SetParam(State58)'

    -- 待机状态
    state = ai.NewState(58)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 56)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 17)

    -- 搜索敌人
    action = ai.NewAction(56, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(24, 14)
    g_AIDebugInfo[56] = '[56] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(24, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(34)
    g_AIDebugInfo[24] = '[24] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(34, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(15)
    g_AIDebugInfo[34] = '[34] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(15, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(44)
    g_AIDebugInfo[15] = '[15] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(44, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(81)
    g_AIDebugInfo[44] = '[44] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(81, AI_ACTION.SET_STATE)
    action.SetParam(18) -- 战斗状态
    g_AIDebugInfo[81] = '[81] SetParam(State18)'

    -- 战斗状态
    state = ai.NewState(18)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 38)

    -- 选择第一威胁
    action = ai.NewAction(38, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(16, 73)
    g_AIDebugInfo[38] = '[38] SELECT_FIRST_THREAT()'

    -- 设置系统无敌
    action = ai.NewAction(73, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(32)
    g_AIDebugInfo[73] = '[73] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(32, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(74, 74)
    g_AIDebugInfo[32] = '[32] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(74, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(83)
    g_AIDebugInfo[74] = '[74] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(83, AI_ACTION.SET_STATE)
    action.SetParam(42) -- 无敌返回状态
    g_AIDebugInfo[83] = '[83] SetParam(State42)'

    -- 无敌返回状态
    state = ai.NewState(42)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 72)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 72)

    -- 返回
    action = ai.NewAction(72, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(70, 70)
    g_AIDebugInfo[72] = '[72] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(70, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(85)
    g_AIDebugInfo[70] = '[70] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(85, AI_ACTION.SET_STATE)
    action.SetParam(21) -- 返回成功
    g_AIDebugInfo[85] = '[85] SetParam(State21)'

    -- 返回成功
    state = ai.NewState(21)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 41)

    -- 清除DEBUFF
    action = ai.NewAction(41, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(65)
    g_AIDebugInfo[41] = '[41] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(65, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(14)
    g_AIDebugInfo[65] = '[65] SET_SYSTEM_SHEILD(0)'

    -- Npc标准技能选择
    action = ai.NewAction(16, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(30, 44)
    g_AIDebugInfo[16] = '[16] NPC_STANDARD_SKILL_SELECTOR()'

    -- 保持距离
    action = ai.NewAction(30, AI_ACTION.KEEP_RANGE)
    action.SetParam(550, 850)
    action.SetBranch(76, 62)
    g_AIDebugInfo[30] = '[30] KEEP_RANGE(550, 850)'

    -- 设置主定时器
    action = ai.NewAction(62, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(87)
    g_AIDebugInfo[62] = '[62] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(87, AI_ACTION.SET_STATE)
    action.SetParam(78) -- 追击状态
    g_AIDebugInfo[87] = '[87] SetParam(State78)'

    -- 追击状态
    state = ai.NewState(78)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 22)

    -- 比较返回点距离
    action = ai.NewAction(22, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(10000)
    action.SetBranch(73, 76, 76)
    g_AIDebugInfo[22] = '[22] RETURN_POSITION_DISTANCE_COMPARE(10000)'

    -- Npc释放选定技能
    action = ai.NewAction(76, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(63, 19)
    g_AIDebugInfo[76] = '[76] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(19, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(38, 73)
    g_AIDebugInfo[19] = '[19] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 双选项随机
    action = ai.NewAction(63, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(50, 50)
    action.SetBranch(71, 44)
    g_AIDebugInfo[63] = '[63] RANDOM_BI_BRANCH(50, 50)'

    -- NPC喊话
    action = ai.NewAction(71, 2071)
    action.SetBranch(12)
    g_AIDebugInfo[71] = '[71] CustomAction71()'

    -- 设置主定时器
    action = ai.NewAction(12, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(89)
    g_AIDebugInfo[12] = '[12] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(89, AI_ACTION.SET_STATE)
    action.SetParam(67) -- 喊话结束
    g_AIDebugInfo[89] = '[89] SetParam(State67)'

    -- 喊话结束
    state = ai.NewState(67)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 44)

    -- 选择事件发起者
    action = ai.NewAction(17, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(24, 73)
    g_AIDebugInfo[17] = '[17] SELECT_EVENT_SRC()'

    ai.SetInitState(8)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction71(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end


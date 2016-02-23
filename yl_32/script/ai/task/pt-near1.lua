-- 编写者：hejingxian
-- 版本号：56

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
    ai.RegisterUserAction(2041, 'AI:CustomAction41')

    -- 初始
    state = ai.NewState(12)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 11)

    -- 记录原点
    action = ai.NewAction(11, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(68)
    g_AIDebugInfo[11] = '[11] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(68, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(92)
    g_AIDebugInfo[68] = '[68] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(92, AI_ACTION.SET_STATE)
    action.SetParam(2) -- 待机状态
    g_AIDebugInfo[92] = '[92] SetParam(State2)'

    -- 待机状态
    state = ai.NewState(2)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 21)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1)

    -- 选择事件发起者
    action = ai.NewAction(21, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(39, 53)
    g_AIDebugInfo[21] = '[21] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(39, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(34)
    g_AIDebugInfo[39] = '[39] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(34, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(75)
    g_AIDebugInfo[34] = '[34] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(75, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(38)
    g_AIDebugInfo[75] = '[75] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(38, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(94)
    g_AIDebugInfo[38] = '[38] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(94, AI_ACTION.SET_STATE)
    action.SetParam(14) -- 战斗状态
    g_AIDebugInfo[94] = '[94] SetParam(State14)'

    -- 战斗状态
    state = ai.NewState(14)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 45)

    -- 选择第一威胁
    action = ai.NewAction(45, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(26, 53)
    g_AIDebugInfo[45] = '[45] SELECT_FIRST_THREAT()'

    -- 设置系统无敌
    action = ai.NewAction(53, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(67)
    g_AIDebugInfo[53] = '[53] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(67, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(4, 4)
    g_AIDebugInfo[67] = '[67] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(4, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(96)
    g_AIDebugInfo[4] = '[4] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(96, AI_ACTION.SET_STATE)
    action.SetParam(58) -- 无敌返回状态
    g_AIDebugInfo[96] = '[96] SetParam(State58)'

    -- 无敌返回状态
    state = ai.NewState(58)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 46)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 46)

    -- 返回
    action = ai.NewAction(46, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(69, 69)
    g_AIDebugInfo[46] = '[46] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(69, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(98)
    g_AIDebugInfo[69] = '[69] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(98, AI_ACTION.SET_STATE)
    action.SetParam(74) -- 返回成功
    g_AIDebugInfo[98] = '[98] SetParam(State74)'

    -- 返回成功
    state = ai.NewState(74)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 30)

    -- 清除DEBUFF
    action = ai.NewAction(30, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(47)
    g_AIDebugInfo[30] = '[30] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(47, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(68)
    g_AIDebugInfo[47] = '[47] SET_SYSTEM_SHEILD(0)'

    -- Npc标准技能选择
    action = ai.NewAction(26, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(78, 38)
    g_AIDebugInfo[26] = '[26] NPC_STANDARD_SKILL_SELECTOR()'

    -- 双选项随机
    action = ai.NewAction(78, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(5, 95)
    action.SetBranch(41, 25)
    g_AIDebugInfo[78] = '[78] RANDOM_BI_BRANCH(5, 95)'

    -- NPC喊话
    action = ai.NewAction(41, 2041)
    action.SetBranch(3)
    g_AIDebugInfo[41] = '[41] AI:CustomAction41()'

    -- 设置主定时器
    action = ai.NewAction(3, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(100)
    g_AIDebugInfo[3] = '[3] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(100, AI_ACTION.SET_STATE)
    action.SetParam(35) -- 喊话结束
    g_AIDebugInfo[100] = '[100] SetParam(State35)'

    -- 喊话结束
    state = ai.NewState(35)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 25)

    -- Npc保持技能释放距离
    action = ai.NewAction(25, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(8, 51)
    g_AIDebugInfo[25] = '[25] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(51, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(102)
    g_AIDebugInfo[51] = '[51] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(102, AI_ACTION.SET_STATE)
    action.SetParam(54) -- 追击状态
    g_AIDebugInfo[102] = '[102] SetParam(State54)'

    -- 追击状态
    state = ai.NewState(54)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 42)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 68)

    -- 比较返回点距离
    action = ai.NewAction(42, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(53, 25, 25)
    g_AIDebugInfo[42] = '[42] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(8, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(38, 50)
    g_AIDebugInfo[8] = '[8] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(50, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(100)
    action.SetBranch(38, 53)
    g_AIDebugInfo[50] = '[50] CHECK_SKILL_FAILED_COUNTER(100)'

    -- 搜索敌人
    action = ai.NewAction(1, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(39, 68)
    g_AIDebugInfo[1] = '[1] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(12)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction41(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end


-- 编写者：hejingxian
-- 版本号：8

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
    ai.RegisterUserAction(2081, 'CustomAction81')

    -- 初始
    state = ai.NewState(74)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 52)

    -- 记录原点
    action = ai.NewAction(52, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(40)
    g_AIDebugInfo[52] = '[52] RECORD_ORIGIN_POSITION()'

    -- 进入巡逻路径
    action = ai.NewAction(40, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(21)
    g_AIDebugInfo[40] = '[40] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(21, AI_ACTION.DO_PATROL)
    action.SetBranch(15, 15)
    g_AIDebugInfo[21] = '[21] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(15, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(88)
    g_AIDebugInfo[15] = '[15] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(88, AI_ACTION.SET_STATE)
    action.SetParam(8) -- 寻路结束
    g_AIDebugInfo[88] = '[88] SetParam(State8)'

    -- 寻路结束
    state = ai.NewState(8)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 54)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 41)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 21)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 21)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 42)

    -- 搜索敌人
    action = ai.NewAction(54, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(81, 15)
    g_AIDebugInfo[54] = '[54] SEARCH_ENEMY(param[AlertRange])'

    -- NPC喊话
    action = ai.NewAction(81, 2081)
    action.SetBranch(82)
    g_AIDebugInfo[81] = '[81] CustomAction81()'

    -- 设置主定时器
    action = ai.NewAction(82, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(90)
    g_AIDebugInfo[82] = '[82] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(90, AI_ACTION.SET_STATE)
    action.SetParam(83) -- 喊话结束
    g_AIDebugInfo[90] = '[90] SetParam(State83)'

    -- 喊话结束
    state = ai.NewState(83)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 71)

    -- 将目标添加到威胁列表
    action = ai.NewAction(71, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(63)
    g_AIDebugInfo[71] = '[71] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(63, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(59)
    g_AIDebugInfo[63] = '[63] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(59, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(55)
    g_AIDebugInfo[59] = '[59] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(55, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(92)
    g_AIDebugInfo[55] = '[55] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(92, AI_ACTION.SET_STATE)
    action.SetParam(28) -- 战斗状态
    g_AIDebugInfo[92] = '[92] SetParam(State28)'

    -- 战斗状态
    state = ai.NewState(28)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 7)

    -- 选择第一威胁
    action = ai.NewAction(7, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(58, 32)
    g_AIDebugInfo[7] = '[7] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(58, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(27, 55)
    g_AIDebugInfo[58] = '[58] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(27, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(65, 25)
    g_AIDebugInfo[27] = '[27] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(65, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(55, 73)
    g_AIDebugInfo[65] = '[65] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(73, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(7, 32)
    g_AIDebugInfo[73] = '[73] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 设置系统无敌
    action = ai.NewAction(32, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(51)
    g_AIDebugInfo[32] = '[32] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(51, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(56, 56)
    g_AIDebugInfo[51] = '[51] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(56, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(94)
    g_AIDebugInfo[56] = '[56] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(94, AI_ACTION.SET_STATE)
    action.SetParam(23) -- 无敌返回状态
    g_AIDebugInfo[94] = '[94] SetParam(State23)'

    -- 无敌返回状态
    state = ai.NewState(23)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 67)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 67)

    -- 返回
    action = ai.NewAction(67, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(29, 29)
    g_AIDebugInfo[67] = '[67] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(29, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(96)
    g_AIDebugInfo[29] = '[29] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(96, AI_ACTION.SET_STATE)
    action.SetParam(79) -- 返回成功
    g_AIDebugInfo[96] = '[96] SetParam(State79)'

    -- 返回成功
    state = ai.NewState(79)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 78)

    -- 清除DEBUFF
    action = ai.NewAction(78, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(48)
    g_AIDebugInfo[78] = '[78] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(48, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(15)
    g_AIDebugInfo[48] = '[48] SET_SYSTEM_SHEILD(0)'

    -- 设置主定时器
    action = ai.NewAction(25, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(98)
    g_AIDebugInfo[25] = '[25] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(98, AI_ACTION.SET_STATE)
    action.SetParam(43) -- 追击状态
    g_AIDebugInfo[98] = '[98] SetParam(State43)'

    -- 追击状态
    state = ai.NewState(43)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 35)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 15)

    -- 比较返回点距离
    action = ai.NewAction(35, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(32, 27, 27)
    g_AIDebugInfo[35] = '[35] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 选择事件发起者
    action = ai.NewAction(41, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(71, 32)
    g_AIDebugInfo[41] = '[41] SELECT_EVENT_SRC()'

    -- 设置副定时器
    action = ai.NewAction(42, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(15)
    g_AIDebugInfo[42] = '[42] SET_SECONDARY_TIMER(16)'

    ai.SetInitState(74)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction81(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end



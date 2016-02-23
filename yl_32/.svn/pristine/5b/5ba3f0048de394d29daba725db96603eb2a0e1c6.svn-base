-- 编写者：hejingxian
-- 版本号：15

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

    -- 初始
    state = ai.NewState(56)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 29)

    -- 记录原点
    action = ai.NewAction(29, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(20)
    g_AIDebugInfo[29] = '[29] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(20, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(111)
    g_AIDebugInfo[20] = '[20] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(111, AI_ACTION.SET_STATE)
    action.SetParam(62) -- 待机状态
    g_AIDebugInfo[111] = '[111] SetParam(State62)'

    -- 待机状态
    state = ai.NewState(62)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 26)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 19)

    -- 选择事件发起者
    action = ai.NewAction(26, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(64, 44)
    g_AIDebugInfo[26] = '[26] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(64, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(24)
    g_AIDebugInfo[64] = '[64] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(24, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(48)
    g_AIDebugInfo[24] = '[24] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(48, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(61)
    g_AIDebugInfo[48] = '[48] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(61, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(113)
    g_AIDebugInfo[61] = '[61] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(113, AI_ACTION.SET_STATE)
    action.SetParam(63) -- 战斗状态
    g_AIDebugInfo[113] = '[113] SetParam(State63)'

    -- 战斗状态
    state = ai.NewState(63)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 20)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 53)

    -- 选择第一威胁
    action = ai.NewAction(53, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(23, 44)
    g_AIDebugInfo[53] = '[53] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(23, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(28, 44)
    g_AIDebugInfo[23] = '[23] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(28, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(18, 66)
    g_AIDebugInfo[28] = '[28] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 清除目标威胁
    action = ai.NewAction(66, AI_ACTION.CLEAR_THREAT_FOR_TARGET)
    action.SetBranch(104, 104)
    g_AIDebugInfo[66] = '[66] CLEAR_THREAT_FOR_TARGET()'

    -- 搜索敌人
    action = ai.NewAction(104, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(64, 96)
    g_AIDebugInfo[104] = '[104] SEARCH_ENEMY(param[AlertRange])'

    -- 释放技能
    action = ai.NewAction(96, AI_ACTION.CAST_SKILL)
    action.SetParam(1410, 1)
    action.SetBranch(98, 98)
    g_AIDebugInfo[96] = '[96] CAST_SKILL(1410, 1)'

    -- 设置主定时器
    action = ai.NewAction(98, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(115)
    g_AIDebugInfo[98] = '[98] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(115, AI_ACTION.SET_STATE)
    action.SetParam(97) -- 待机状态
    g_AIDebugInfo[115] = '[115] SetParam(State97)'

    -- 待机状态
    state = ai.NewState(97)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 104)

    -- Npc释放选定技能
    action = ai.NewAction(18, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(61, 35)
    g_AIDebugInfo[18] = '[18] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(35, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(61, 44)
    g_AIDebugInfo[35] = '[35] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 站立
    action = ai.NewAction(44, AI_ACTION.STAND)
    action.SetBranch(54)
    g_AIDebugInfo[44] = '[44] STAND()'

    -- 设置主定时器
    action = ai.NewAction(54, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(117)
    g_AIDebugInfo[54] = '[54] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(117, AI_ACTION.SET_STATE)
    action.SetParam(8) -- 待机状态
    g_AIDebugInfo[117] = '[117] SetParam(State8)'

    -- 待机状态
    state = ai.NewState(8)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)

    -- 搜索敌人
    action = ai.NewAction(6, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(6000)
    action.SetBranch(60, 78)
    g_AIDebugInfo[6] = '[6] SEARCH_ENEMY(6000)'

    -- 释放技能
    action = ai.NewAction(78, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(83, 83)
    g_AIDebugInfo[78] = '[78] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(83, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(44, 74)
    g_AIDebugInfo[83] = '[83] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置系统无敌
    action = ai.NewAction(74, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(71)
    g_AIDebugInfo[74] = '[74] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(71, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(69, 69)
    g_AIDebugInfo[71] = '[71] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(69, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(119)
    g_AIDebugInfo[69] = '[69] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(119, AI_ACTION.SET_STATE)
    action.SetParam(80) -- 无敌返回状态
    g_AIDebugInfo[119] = '[119] SetParam(State80)'

    -- 无敌返回状态
    state = ai.NewState(80)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 79)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 79)

    -- 返回
    action = ai.NewAction(79, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(88, 88)
    g_AIDebugInfo[79] = '[79] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(88, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(121)
    g_AIDebugInfo[88] = '[88] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(121, AI_ACTION.SET_STATE)
    action.SetParam(92) -- 返回成功
    g_AIDebugInfo[121] = '[121] SetParam(State92)'

    -- 返回成功
    state = ai.NewState(92)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 82)

    -- 清除DEBUFF
    action = ai.NewAction(82, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(89)
    g_AIDebugInfo[82] = '[82] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(89, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(20)
    g_AIDebugInfo[89] = '[89] SET_SYSTEM_SHEILD(0)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(60, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(31)
    g_AIDebugInfo[60] = '[60] ADD_TARGET_TO_THREAT_LIST()'

    -- 选择第一威胁
    action = ai.NewAction(31, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(23, 44)
    g_AIDebugInfo[31] = '[31] SELECT_FIRST_THREAT()'

    -- 搜索敌人
    action = ai.NewAction(19, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(64, 20)
    g_AIDebugInfo[19] = '[19] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(56)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

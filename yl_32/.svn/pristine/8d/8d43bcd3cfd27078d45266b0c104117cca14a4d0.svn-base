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

    -- 初始
    state = ai.NewState(69)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 20)

    -- 记录原点
    action = ai.NewAction(20, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(51)
    g_AIDebugInfo[20] = '[20] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(51, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(100)
    g_AIDebugInfo[51] = '[51] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(100, AI_ACTION.SET_STATE)
    action.SetParam(42) -- 待机状态
    g_AIDebugInfo[100] = '[100] SetParam(State42)'

    -- 待机状态
    state = ai.NewState(42)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 45)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 44)

    -- 选择事件发起者
    action = ai.NewAction(45, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(50, 30)
    g_AIDebugInfo[45] = '[45] SELECT_EVENT_SRC()'

    -- 站立
    action = ai.NewAction(30, AI_ACTION.STAND)
    action.SetBranch(67)
    g_AIDebugInfo[30] = '[30] STAND()'

    -- 设置主定时器
    action = ai.NewAction(67, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(102)
    g_AIDebugInfo[67] = '[67] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(102, AI_ACTION.SET_STATE)
    action.SetParam(34) -- 待机状态
    g_AIDebugInfo[102] = '[102] SetParam(State34)'

    -- 待机状态
    state = ai.NewState(34)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 21)

    -- 搜索敌人
    action = ai.NewAction(21, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(6000)
    action.SetBranch(6, 70)
    g_AIDebugInfo[21] = '[21] SEARCH_ENEMY(6000)'

    -- 释放技能
    action = ai.NewAction(70, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(30, 71)
    g_AIDebugInfo[70] = '[70] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(71, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(30, 89)
    g_AIDebugInfo[71] = '[71] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置系统无敌
    action = ai.NewAction(89, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(88)
    g_AIDebugInfo[89] = '[89] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(88, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(73, 73)
    g_AIDebugInfo[88] = '[88] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(73, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(104)
    g_AIDebugInfo[73] = '[73] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(104, AI_ACTION.SET_STATE)
    action.SetParam(91) -- 无敌返回状态
    g_AIDebugInfo[104] = '[104] SetParam(State91)'

    -- 无敌返回状态
    state = ai.NewState(91)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 87)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 87)

    -- 返回
    action = ai.NewAction(87, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(83, 83)
    g_AIDebugInfo[87] = '[87] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(83, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(106)
    g_AIDebugInfo[83] = '[83] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(106, AI_ACTION.SET_STATE)
    action.SetParam(79) -- 返回成功
    g_AIDebugInfo[106] = '[106] SetParam(State79)'

    -- 返回成功
    state = ai.NewState(79)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 75)

    -- 清除DEBUFF
    action = ai.NewAction(75, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(72)
    g_AIDebugInfo[75] = '[75] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(72, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(51)
    g_AIDebugInfo[72] = '[72] SET_SYSTEM_SHEILD(0)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(6, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(1)
    g_AIDebugInfo[6] = '[6] ADD_TARGET_TO_THREAT_LIST()'

    -- 选择第一威胁
    action = ai.NewAction(1, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(63, 30)
    g_AIDebugInfo[1] = '[1] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(63, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(13, 25)
    g_AIDebugInfo[63] = '[63] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(13, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(15, 37)
    g_AIDebugInfo[13] = '[13] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(15, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(25, 14)
    g_AIDebugInfo[15] = '[15] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(25, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(108)
    g_AIDebugInfo[25] = '[25] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(108, AI_ACTION.SET_STATE)
    action.SetParam(26) -- 战斗状态
    g_AIDebugInfo[108] = '[108] SetParam(State26)'

    -- 战斗状态
    state = ai.NewState(26)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 16)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 30)

    -- 选择第一威胁
    action = ai.NewAction(16, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(63, 30)
    g_AIDebugInfo[16] = '[16] SELECT_FIRST_THREAT()'

    -- 检查技能失败计数
    action = ai.NewAction(14, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(16, 30)
    g_AIDebugInfo[14] = '[14] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 设置主定时器
    action = ai.NewAction(37, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(110)
    g_AIDebugInfo[37] = '[37] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(110, AI_ACTION.SET_STATE)
    action.SetParam(52) -- 追击状态
    g_AIDebugInfo[110] = '[110] SetParam(State52)'

    -- 追击状态
    state = ai.NewState(52)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 19)
    state.HandleEvent(AI_EVENT.ON_CAST_SKILL_FAILED, 51)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 97)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 30)

    -- 选择事件发起者
    action = ai.NewAction(19, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(41, 25)
    g_AIDebugInfo[19] = '[19] SELECT_EVENT_SRC()'

    -- 比较返回点距离
    action = ai.NewAction(41, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(30, 13, 13)
    g_AIDebugInfo[41] = '[41] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 目标知否存在
    action = ai.NewAction(97, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(41, 16)
    g_AIDebugInfo[97] = '[97] IS_TARGET_EXIST()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(50, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(23)
    g_AIDebugInfo[50] = '[50] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(23, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(9)
    g_AIDebugInfo[23] = '[23] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(9, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(25)
    g_AIDebugInfo[9] = '[9] NPC_INIT_SKILL_CD()'

    -- 搜索敌人
    action = ai.NewAction(44, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(50, 51)
    g_AIDebugInfo[44] = '[44] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(69)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

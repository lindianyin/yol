-- 编写者：hejingxian
-- 版本号：6

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
    state = ai.NewState(15)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 27)

    -- 记录原点
    action = ai.NewAction(27, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(7)
    g_AIDebugInfo[27] = '[27] RECORD_ORIGIN_POSITION()'

    -- 进入巡逻路径
    action = ai.NewAction(7, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(29)
    g_AIDebugInfo[7] = '[7] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(29, AI_ACTION.DO_PATROL)
    action.SetBranch(35, 43)
    g_AIDebugInfo[29] = '[29] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(35, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(107)
    g_AIDebugInfo[35] = '[35] SET_PRIMARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(107, AI_ACTION.SET_STATE)
    action.SetParam(33) -- 寻路结束
    g_AIDebugInfo[107] = '[107] SetParam(State33)'

    -- 寻路结束
    state = ai.NewState(33)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 79)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 29)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 29)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 79)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 72)

    -- 搜索敌人
    action = ai.NewAction(79, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(25, 35)
    g_AIDebugInfo[79] = '[79] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(25, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(71)
    g_AIDebugInfo[25] = '[25] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(71, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(5)
    g_AIDebugInfo[71] = '[71] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(5, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(44)
    g_AIDebugInfo[5] = '[5] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(44, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(109)
    g_AIDebugInfo[44] = '[44] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(109, AI_ACTION.SET_STATE)
    action.SetParam(47) -- 战斗状态
    g_AIDebugInfo[109] = '[109] SetParam(State47)'

    -- 战斗状态
    state = ai.NewState(47)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 66)

    -- 选择第一威胁
    action = ai.NewAction(66, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(55, 58)
    g_AIDebugInfo[66] = '[66] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(55, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(46, 44)
    g_AIDebugInfo[55] = '[55] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(46, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(16, 20)
    g_AIDebugInfo[46] = '[46] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(16, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(44, 30)
    g_AIDebugInfo[16] = '[16] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(30, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(66, 58)
    g_AIDebugInfo[30] = '[30] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 设置系统无敌
    action = ai.NewAction(58, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(81)
    g_AIDebugInfo[58] = '[58] SET_SYSTEM_SHEILD(1)'

    -- 站立
    action = ai.NewAction(81, AI_ACTION.STAND)
    action.SetBranch(39)
    g_AIDebugInfo[81] = '[81] STAND()'

    -- 设置主定时器
    action = ai.NewAction(39, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(111)
    g_AIDebugInfo[39] = '[39] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(111, AI_ACTION.SET_STATE)
    action.SetParam(37) -- 无敌返回状态
    g_AIDebugInfo[111] = '[111] SetParam(State37)'

    -- 无敌返回状态
    state = ai.NewState(37)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 83)

    -- 站立
    action = ai.NewAction(83, AI_ACTION.STAND)
    action.SetBranch(78)
    g_AIDebugInfo[83] = '[83] STAND()'

    -- 设置主定时器
    action = ai.NewAction(78, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(113)
    g_AIDebugInfo[78] = '[78] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(113, AI_ACTION.SET_STATE)
    action.SetParam(63) -- 返回成功
    g_AIDebugInfo[113] = '[113] SetParam(State63)'

    -- 返回成功
    state = ai.NewState(63)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 70)

    -- 清除DEBUFF
    action = ai.NewAction(70, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(32)
    g_AIDebugInfo[70] = '[70] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(32, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(35)
    g_AIDebugInfo[32] = '[32] SET_SYSTEM_SHEILD(0)'

    -- 设置主定时器
    action = ai.NewAction(20, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(115)
    g_AIDebugInfo[20] = '[20] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(115, AI_ACTION.SET_STATE)
    action.SetParam(64) -- 追击状态
    g_AIDebugInfo[115] = '[115] SetParam(State64)'

    -- 追击状态
    state = ai.NewState(64)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 62)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 58)

    -- 比较返回点距离
    action = ai.NewAction(62, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(58, 46, 46)
    g_AIDebugInfo[62] = '[62] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 选择事件发起者
    action = ai.NewAction(72, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(25, 58)
    g_AIDebugInfo[72] = '[72] SELECT_EVENT_SRC()'

    -- 设置副定时器
    action = ai.NewAction(43, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(35)
    g_AIDebugInfo[43] = '[43] SET_SECONDARY_TIMER(16)'

    ai.SetInitState(15)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

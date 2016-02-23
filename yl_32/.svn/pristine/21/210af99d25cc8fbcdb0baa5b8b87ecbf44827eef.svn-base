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

    -- 初始
    state = ai.NewState(27)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 28)

    -- 记录原点
    action = ai.NewAction(28, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(65)
    g_AIDebugInfo[28] = '[28] RECORD_ORIGIN_POSITION()'

    -- 进入巡逻路径
    action = ai.NewAction(65, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(3)
    g_AIDebugInfo[65] = '[65] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(3, AI_ACTION.DO_PATROL)
    action.SetBranch(45, 45)
    g_AIDebugInfo[3] = '[3] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(45, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(85)
    g_AIDebugInfo[45] = '[45] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(85, AI_ACTION.SET_STATE)
    action.SetParam(51) -- 寻路结束
    g_AIDebugInfo[85] = '[85] SetParam(State51)'

    -- 寻路结束
    state = ai.NewState(51)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 76)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 57)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 39)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 3)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 3)

    -- 设置副定时器
    action = ai.NewAction(76, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(45)
    g_AIDebugInfo[76] = '[76] SET_SECONDARY_TIMER(16)'

    -- 选择事件发起者
    action = ai.NewAction(57, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(10, 74)
    g_AIDebugInfo[57] = '[57] SELECT_EVENT_SRC()'

    -- 设置系统无敌
    action = ai.NewAction(74, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(38)
    g_AIDebugInfo[74] = '[74] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(38, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(70, 70)
    g_AIDebugInfo[38] = '[38] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(70, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(87)
    g_AIDebugInfo[70] = '[70] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(87, AI_ACTION.SET_STATE)
    action.SetParam(6) -- 无敌返回状态
    g_AIDebugInfo[87] = '[87] SetParam(State6)'

    -- 无敌返回状态
    state = ai.NewState(6)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 66)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 66)

    -- 返回
    action = ai.NewAction(66, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(41, 41)
    g_AIDebugInfo[66] = '[66] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(41, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(89)
    g_AIDebugInfo[41] = '[41] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(89, AI_ACTION.SET_STATE)
    action.SetParam(54) -- 返回成功
    g_AIDebugInfo[89] = '[89] SetParam(State54)'

    -- 返回成功
    state = ai.NewState(54)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 34)

    -- 清除DEBUFF
    action = ai.NewAction(34, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(21)
    g_AIDebugInfo[34] = '[34] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(21, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(45)
    g_AIDebugInfo[21] = '[21] SET_SYSTEM_SHEILD(0)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(10, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(40)
    g_AIDebugInfo[10] = '[10] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(40, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(55)
    g_AIDebugInfo[40] = '[40] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(55, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(4)
    g_AIDebugInfo[55] = '[55] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(4, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(91)
    g_AIDebugInfo[4] = '[4] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(91, AI_ACTION.SET_STATE)
    action.SetParam(79) -- 战斗状态
    g_AIDebugInfo[91] = '[91] SetParam(State79)'

    -- 战斗状态
    state = ai.NewState(79)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 56)

    -- 选择第一威胁
    action = ai.NewAction(56, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(42, 74)
    g_AIDebugInfo[56] = '[56] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(42, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(58, 4)
    g_AIDebugInfo[42] = '[42] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(58, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(20, 50)
    g_AIDebugInfo[58] = '[58] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(20, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(4, 32)
    g_AIDebugInfo[20] = '[20] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(32, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(56, 74)
    g_AIDebugInfo[32] = '[32] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 设置主定时器
    action = ai.NewAction(50, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(93)
    g_AIDebugInfo[50] = '[50] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(93, AI_ACTION.SET_STATE)
    action.SetParam(75) -- 追击状态
    g_AIDebugInfo[93] = '[93] SetParam(State75)'

    -- 追击状态
    state = ai.NewState(75)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 64)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 45)

    -- 比较返回点距离
    action = ai.NewAction(64, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(74, 58, 58)
    g_AIDebugInfo[64] = '[64] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 搜索敌人
    action = ai.NewAction(39, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(10, 45)
    g_AIDebugInfo[39] = '[39] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(27)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

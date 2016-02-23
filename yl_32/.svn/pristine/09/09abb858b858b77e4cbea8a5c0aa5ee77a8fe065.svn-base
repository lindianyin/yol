-- 编写者：hejingxian
-- 版本号：4

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
    state = ai.NewState(3)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 79)

    -- 记录原点
    action = ai.NewAction(79, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(7)
    g_AIDebugInfo[79] = '[79] RECORD_ORIGIN_POSITION()'

    -- 进入巡逻路径
    action = ai.NewAction(7, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(91)
    g_AIDebugInfo[7] = '[7] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(91, AI_ACTION.DO_PATROL)
    action.SetBranch(86, 94)
    g_AIDebugInfo[91] = '[91] DO_PATROL()'

    -- 搜索敌人
    action = ai.NewAction(94, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(3000)
    action.SetBranch(60, 89)
    g_AIDebugInfo[94] = '[94] SEARCH_ENEMY(3000)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(60, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(25)
    g_AIDebugInfo[60] = '[60] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(25, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(77)
    g_AIDebugInfo[25] = '[25] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(77, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(88)
    g_AIDebugInfo[77] = '[77] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(88, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(100)
    g_AIDebugInfo[88] = '[88] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(100, AI_ACTION.SET_STATE)
    action.SetParam(84) -- 战斗状态
    g_AIDebugInfo[100] = '[100] SetParam(State84)'

    -- 战斗状态
    state = ai.NewState(84)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 35)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 89)

    -- 选择第一威胁
    action = ai.NewAction(35, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(18, 89)
    g_AIDebugInfo[35] = '[35] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(18, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(20, 88)
    g_AIDebugInfo[18] = '[18] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(20, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(36, 63)
    g_AIDebugInfo[20] = '[20] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(63, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(102)
    g_AIDebugInfo[63] = '[63] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(102, AI_ACTION.SET_STATE)
    action.SetParam(67) -- 追击状态
    g_AIDebugInfo[102] = '[102] SetParam(State67)'

    -- 追击状态
    state = ai.NewState(67)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 89)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 97)

    -- 站立
    action = ai.NewAction(89, AI_ACTION.STAND)
    action.SetBranch(13)
    g_AIDebugInfo[89] = '[89] STAND()'

    -- 设置主定时器
    action = ai.NewAction(13, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(104)
    g_AIDebugInfo[13] = '[13] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(104, AI_ACTION.SET_STATE)
    action.SetParam(46) -- 待机状态
    g_AIDebugInfo[104] = '[104] SetParam(State46)'

    -- 待机状态
    state = ai.NewState(46)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 74)

    -- 搜索敌人
    action = ai.NewAction(74, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(6000)
    action.SetBranch(21, 89)
    g_AIDebugInfo[74] = '[74] SEARCH_ENEMY(6000)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(21, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(4)
    g_AIDebugInfo[21] = '[21] ADD_TARGET_TO_THREAT_LIST()'

    -- 选择第一威胁
    action = ai.NewAction(4, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(18, 89)
    g_AIDebugInfo[4] = '[4] SELECT_FIRST_THREAT()'

    -- 目标知否存在
    action = ai.NewAction(97, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(24, 35)
    g_AIDebugInfo[97] = '[97] IS_TARGET_EXIST()'

    -- 比较返回点距离
    action = ai.NewAction(24, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(89, 20, 20)
    g_AIDebugInfo[24] = '[24] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(36, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(88, 15)
    g_AIDebugInfo[36] = '[36] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(15, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(200)
    action.SetBranch(35, 89)
    g_AIDebugInfo[15] = '[15] CHECK_SKILL_FAILED_COUNTER(200)'

    -- 设置主定时器
    action = ai.NewAction(86, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(106)
    g_AIDebugInfo[86] = '[86] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(106, AI_ACTION.SET_STATE)
    action.SetParam(69) -- 寻路结束
    g_AIDebugInfo[106] = '[106] SetParam(State69)'

    -- 寻路结束
    state = ai.NewState(69)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 47)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 28)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 91)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 61)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 91)

    -- 设置副定时器
    action = ai.NewAction(47, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(86)
    g_AIDebugInfo[47] = '[47] SET_SECONDARY_TIMER(16)'

    -- 搜索敌人
    action = ai.NewAction(28, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(60, 86)
    g_AIDebugInfo[28] = '[28] SEARCH_ENEMY(param[AlertRange])'

    -- 选择事件发起者
    action = ai.NewAction(61, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(60, 60)
    g_AIDebugInfo[61] = '[61] SELECT_EVENT_SRC()'

    ai.SetInitState(3)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

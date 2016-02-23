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
    ai.RegisterUserAction(2035, 'CustomAction35')
    ai.RegisterUserAction(2058, 'CustomAction58')

    -- 初始
    state = ai.NewState(88)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 29)

    -- 记录原点
    action = ai.NewAction(29, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(74)
    g_AIDebugInfo[29] = '[29] RECORD_ORIGIN_POSITION()'

    -- 进入巡逻路径
    action = ai.NewAction(74, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(72)
    g_AIDebugInfo[74] = '[74] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(72, AI_ACTION.DO_PATROL)
    action.SetBranch(26, 26)
    g_AIDebugInfo[72] = '[72] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(26, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(97)
    g_AIDebugInfo[26] = '[26] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(97, AI_ACTION.SET_STATE)
    action.SetParam(65) -- 寻路结束
    g_AIDebugInfo[97] = '[97] SetParam(State65)'

    -- 寻路结束
    state = ai.NewState(65)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 72)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 72)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 75)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 30)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 35)

    -- 搜索敌人
    action = ai.NewAction(75, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(58, 26)
    g_AIDebugInfo[75] = '[75] SEARCH_ENEMY(param[AlertRange])'

    -- NPC喊话
    action = ai.NewAction(58, 2058)
    action.SetBranch(25)
    g_AIDebugInfo[58] = '[58] CustomAction58()'

    -- 设置主定时器
    action = ai.NewAction(25, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(99)
    g_AIDebugInfo[25] = '[25] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(99, AI_ACTION.SET_STATE)
    action.SetParam(28) -- 喊话结束
    g_AIDebugInfo[99] = '[99] SetParam(State28)'

    -- 喊话结束
    state = ai.NewState(28)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 16)

    -- 搜索敌人
    action = ai.NewAction(16, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(800)
    action.SetBranch(39, 26)
    g_AIDebugInfo[16] = '[16] SEARCH_ENEMY(800)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(39, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(70)
    g_AIDebugInfo[39] = '[39] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(70, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(85)
    g_AIDebugInfo[70] = '[70] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(85, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(13)
    g_AIDebugInfo[85] = '[85] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(13, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(101)
    g_AIDebugInfo[13] = '[13] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(101, AI_ACTION.SET_STATE)
    action.SetParam(51) -- 战斗状态
    g_AIDebugInfo[101] = '[101] SetParam(State51)'

    -- 战斗状态
    state = ai.NewState(51)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 22)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 17)

    -- 站立
    action = ai.NewAction(22, AI_ACTION.STAND)
    action.SetBranch(61)
    g_AIDebugInfo[22] = '[22] STAND()'

    -- 设置主定时器
    action = ai.NewAction(61, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(103)
    g_AIDebugInfo[61] = '[61] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(103, AI_ACTION.SET_STATE)
    action.SetParam(68) -- 待机状态
    g_AIDebugInfo[103] = '[103] SetParam(State68)'

    -- 待机状态
    state = ai.NewState(68)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 78)

    -- 搜索敌人
    action = ai.NewAction(78, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(6000)
    action.SetBranch(10, 22)
    g_AIDebugInfo[78] = '[78] SEARCH_ENEMY(6000)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(10, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(67)
    g_AIDebugInfo[10] = '[10] ADD_TARGET_TO_THREAT_LIST()'

    -- 选择第一威胁
    action = ai.NewAction(67, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(45, 22)
    g_AIDebugInfo[67] = '[67] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(45, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(91, 13)
    g_AIDebugInfo[45] = '[45] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(91, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(66, 18)
    g_AIDebugInfo[91] = '[91] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(66, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(13, 24)
    g_AIDebugInfo[66] = '[66] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(24, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(200)
    action.SetBranch(17, 22)
    g_AIDebugInfo[24] = '[24] CHECK_SKILL_FAILED_COUNTER(200)'

    -- 选择第一威胁
    action = ai.NewAction(17, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(45, 22)
    g_AIDebugInfo[17] = '[17] SELECT_FIRST_THREAT()'

    -- 设置主定时器
    action = ai.NewAction(18, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(105)
    g_AIDebugInfo[18] = '[18] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(105, AI_ACTION.SET_STATE)
    action.SetParam(43) -- 追击状态
    g_AIDebugInfo[105] = '[105] SetParam(State43)'

    -- 追击状态
    state = ai.NewState(43)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 94)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 22)

    -- 目标知否存在
    action = ai.NewAction(94, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(87, 17)
    g_AIDebugInfo[94] = '[94] IS_TARGET_EXIST()'

    -- 比较返回点距离
    action = ai.NewAction(87, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(22, 91, 91)
    g_AIDebugInfo[87] = '[87] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 设置副定时器
    action = ai.NewAction(30, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(26)
    g_AIDebugInfo[30] = '[30] SET_SECONDARY_TIMER(16)'

    -- 选中自己
    action = ai.NewAction(35, 2035)
    action.SetBranch(1)
    g_AIDebugInfo[35] = '[35] CustomAction35()'

    -- 释放技能
    action = ai.NewAction(1, AI_ACTION.CAST_SKILL)
    action.SetParam(1640, 1)
    action.SetBranch(14, 14)
    g_AIDebugInfo[1] = '[1] CAST_SKILL(1640, 1)'

    -- 选择事件发起者
    action = ai.NewAction(14, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(39, 39)
    g_AIDebugInfo[14] = '[14] SELECT_EVENT_SRC()'

    ai.SetInitState(88)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction58(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction35(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


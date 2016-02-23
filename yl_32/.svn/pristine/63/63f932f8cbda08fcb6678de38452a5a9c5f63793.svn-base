-- 编写者：hejingxian
-- 版本号：22

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
    ai.RegisterUserAction(2131, 'AI:CustomAction131')

    -- 初始
    state = ai.NewState(11)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 29)

    -- 记录原点
    action = ai.NewAction(29, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(2)
    g_AIDebugInfo[29] = '[29] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(2, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(166)
    g_AIDebugInfo[2] = '[2] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(166, AI_ACTION.SET_STATE)
    action.SetParam(67) -- 待机状态
    g_AIDebugInfo[166] = '[166] SetParam(State67)'

    -- 待机状态
    state = ai.NewState(67)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 38)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 30)

    -- 搜索敌人
    action = ai.NewAction(38, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(16, 2)
    g_AIDebugInfo[38] = '[38] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(16, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(36)
    g_AIDebugInfo[16] = '[16] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(36, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(54)
    g_AIDebugInfo[36] = '[36] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(54, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(86)
    g_AIDebugInfo[54] = '[54] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(86, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(168)
    g_AIDebugInfo[86] = '[86] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(168, AI_ACTION.SET_STATE)
    action.SetParam(28) -- 战斗状态
    g_AIDebugInfo[168] = '[168] SetParam(State28)'

    -- 战斗状态
    state = ai.NewState(28)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 68)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 71)

    -- 站立
    action = ai.NewAction(68, AI_ACTION.STAND)
    action.SetBranch(17)
    g_AIDebugInfo[68] = '[68] STAND()'

    -- 设置主定时器
    action = ai.NewAction(17, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(170)
    g_AIDebugInfo[17] = '[17] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(170, AI_ACTION.SET_STATE)
    action.SetParam(8) -- 待机状态
    g_AIDebugInfo[170] = '[170] SetParam(State8)'

    -- 待机状态
    state = ai.NewState(8)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 52)

    -- 搜索敌人
    action = ai.NewAction(52, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(1500)
    action.SetBranch(74, 88)
    g_AIDebugInfo[52] = '[52] SEARCH_ENEMY(1500)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(74, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(6)
    g_AIDebugInfo[74] = '[74] ADD_TARGET_TO_THREAT_LIST()'

    -- 选择第一威胁
    action = ai.NewAction(6, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(22, 68)
    g_AIDebugInfo[6] = '[6] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(22, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(9, 86)
    g_AIDebugInfo[22] = '[22] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(9, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(13, 82)
    g_AIDebugInfo[9] = '[9] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(82, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(172)
    g_AIDebugInfo[82] = '[82] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(172, AI_ACTION.SET_STATE)
    action.SetParam(56) -- 追击状态
    g_AIDebugInfo[172] = '[172] SetParam(State56)'

    -- 追击状态
    state = ai.NewState(56)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 163)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 68)

    -- 目标知否存在
    action = ai.NewAction(163, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(60, 71)
    g_AIDebugInfo[163] = '[163] IS_TARGET_EXIST()'

    -- 比较返回点距离
    action = ai.NewAction(60, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(68, 9, 9)
    g_AIDebugInfo[60] = '[60] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 选择第一威胁
    action = ai.NewAction(71, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(99, 68)
    g_AIDebugInfo[71] = '[71] SELECT_FIRST_THREAT()'

    -- 双选项随机
    action = ai.NewAction(99, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(90, 10)
    action.SetBranch(100, 108)
    g_AIDebugInfo[99] = '[99] RANDOM_BI_BRANCH(90, 10)'

    -- 设置主定时器
    action = ai.NewAction(100, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(174)
    g_AIDebugInfo[100] = '[100] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(174, AI_ACTION.SET_STATE)
    action.SetParam(101) -- 战斗状态
    g_AIDebugInfo[174] = '[174] SetParam(State101)'

    -- 战斗状态
    state = ai.NewState(101)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 22)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 68)

    -- 三选项随机
    action = ai.NewAction(108, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(35, 20, 45)
    action.SetBranch(112, 128, 125)
    g_AIDebugInfo[108] = '[108] RANDOM_TRI_BRANCH(35, 20, 45)'

    -- 释放技能
    action = ai.NewAction(112, AI_ACTION.CAST_SKILL)
    action.SetParam(1635, 1)
    action.SetBranch(115, 40)
    g_AIDebugInfo[112] = '[112] CAST_SKILL(1635, 1)'

    -- 设置主定时器
    action = ai.NewAction(115, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(176)
    g_AIDebugInfo[115] = '[115] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(176, AI_ACTION.SET_STATE)
    action.SetParam(114) -- 待机一会
    g_AIDebugInfo[176] = '[176] SetParam(State114)'

    -- 待机一会
    state = ai.NewState(114)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 116)

    -- 释放技能
    action = ai.NewAction(116, AI_ACTION.CAST_SKILL)
    action.SetParam(1633, 1)
    action.SetBranch(158, 40)
    g_AIDebugInfo[116] = '[116] CAST_SKILL(1633, 1)'

    -- 设置主定时器
    action = ai.NewAction(158, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(178)
    g_AIDebugInfo[158] = '[158] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(178, AI_ACTION.SET_STATE)
    action.SetParam(157) -- 战斗状态
    g_AIDebugInfo[178] = '[178] SetParam(State157)'

    -- 战斗状态
    state = ai.NewState(157)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 100)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 68)

    -- 检查技能失败计数
    action = ai.NewAction(40, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(71, 68)
    g_AIDebugInfo[40] = '[40] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 双选项随机
    action = ai.NewAction(125, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(50, 50)
    action.SetBranch(133, 149)
    g_AIDebugInfo[125] = '[125] RANDOM_BI_BRANCH(50, 50)'

    -- 释放技能
    action = ai.NewAction(133, AI_ACTION.CAST_SKILL)
    action.SetParam(1637, 1)
    action.SetBranch(134, 40)
    g_AIDebugInfo[133] = '[133] CAST_SKILL(1637, 1)'

    -- 设置主定时器
    action = ai.NewAction(134, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(180)
    g_AIDebugInfo[134] = '[134] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(180, AI_ACTION.SET_STATE)
    action.SetParam(135) -- 待机一会
    g_AIDebugInfo[180] = '[180] SetParam(State135)'

    -- 待机一会
    state = ai.NewState(135)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 131)

    -- CALL NPC
    action = ai.NewAction(131, 2131)
    action.SetBranch(100)
    g_AIDebugInfo[131] = '[131] AI:CustomAction131()'

    -- 释放技能
    action = ai.NewAction(149, AI_ACTION.CAST_SKILL)
    action.SetParam(1636, 1)
    action.SetBranch(148, 40)
    g_AIDebugInfo[149] = '[149] CAST_SKILL(1636, 1)'

    -- 设置主定时器
    action = ai.NewAction(148, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(182)
    g_AIDebugInfo[148] = '[148] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(182, AI_ACTION.SET_STATE)
    action.SetParam(145) -- 待机一会
    g_AIDebugInfo[182] = '[182] SetParam(State145)'

    -- 待机一会
    state = ai.NewState(145)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 100)

    -- 释放技能
    action = ai.NewAction(128, AI_ACTION.CAST_SKILL)
    action.SetParam(1634, 1)
    action.SetBranch(141, 40)
    g_AIDebugInfo[128] = '[128] CAST_SKILL(1634, 1)'

    -- 设置主定时器
    action = ai.NewAction(141, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(184)
    g_AIDebugInfo[141] = '[141] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(184, AI_ACTION.SET_STATE)
    action.SetParam(140) -- 待机一会
    g_AIDebugInfo[184] = '[184] SetParam(State140)'

    -- 待机一会
    state = ai.NewState(140)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 100)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 68)

    -- Npc释放选定技能
    action = ai.NewAction(13, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(86, 40)
    g_AIDebugInfo[13] = '[13] NPC_CAST_SELECT_SKILL()'

    -- 释放技能
    action = ai.NewAction(88, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(68, 92)
    g_AIDebugInfo[88] = '[88] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(92, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(68, 21)
    g_AIDebugInfo[92] = '[92] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置系统无敌
    action = ai.NewAction(21, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(32)
    g_AIDebugInfo[21] = '[21] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(32, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(75, 75)
    g_AIDebugInfo[32] = '[32] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(75, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(186)
    g_AIDebugInfo[75] = '[75] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(186, AI_ACTION.SET_STATE)
    action.SetParam(65) -- 无敌返回状态
    g_AIDebugInfo[186] = '[186] SetParam(State65)'

    -- 无敌返回状态
    state = ai.NewState(65)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 50)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 50)

    -- 返回
    action = ai.NewAction(50, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(91, 91)
    g_AIDebugInfo[50] = '[50] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(91, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(188)
    g_AIDebugInfo[91] = '[91] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(188, AI_ACTION.SET_STATE)
    action.SetParam(25) -- 返回成功
    g_AIDebugInfo[188] = '[188] SetParam(State25)'

    -- 返回成功
    state = ai.NewState(25)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 5)

    -- 清除DEBUFF
    action = ai.NewAction(5, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(85)
    g_AIDebugInfo[5] = '[5] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(85, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(2)
    g_AIDebugInfo[85] = '[85] SET_SYSTEM_SHEILD(0)'

    -- 选择事件发起者
    action = ai.NewAction(30, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(16, 68)
    g_AIDebugInfo[30] = '[30] SELECT_EVENT_SRC()'

    ai.SetInitState(11)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction131(pNpc, actionData)
    -- 输入自定义动作实现脚本
     local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(1371,nSceneId, 5239,1892,1048576);
	KGameBase.CallNpc(1371,nSceneId, 5191,2895,1048576);
	KGameBase.CallNpc(1371,nSceneId, 6153,2895,1048576);
	KGameBase.CallNpc(1371,nSceneId, 6153,1892,1048576);
    return 1
end


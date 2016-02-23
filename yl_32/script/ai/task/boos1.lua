-- 编写者：hejingxian
-- 版本号：49

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
    ai.RegisterUserAction(2093, 'CustomAction93')

    -- 初始
    state = ai.NewState(105)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 2)

    -- 记录原点
    action = ai.NewAction(2, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(4)
    g_AIDebugInfo[2] = '[2] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(4, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(186)
    g_AIDebugInfo[4] = '[4] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(186, AI_ACTION.SET_STATE)
    action.SetParam(6) -- 待机状态
    g_AIDebugInfo[186] = '[186] SetParam(State6)'

    -- 待机状态
    state = ai.NewState(6)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 10)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 12)

    -- 选择事件发起者
    action = ai.NewAction(10, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(25, 63)
    g_AIDebugInfo[10] = '[10] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(25, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(15)
    g_AIDebugInfo[25] = '[25] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(15, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(27)
    g_AIDebugInfo[15] = '[15] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(27, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(31)
    g_AIDebugInfo[27] = '[27] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(31, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(188)
    g_AIDebugInfo[31] = '[31] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(188, AI_ACTION.SET_STATE)
    action.SetParam(29) -- 战斗状态
    g_AIDebugInfo[188] = '[188] SetParam(State29)'

    -- 战斗状态
    state = ai.NewState(29)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 108)

    -- 选择第一威胁
    action = ai.NewAction(108, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(33, 63)
    g_AIDebugInfo[108] = '[108] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(33, AI_ACTION.CHECK_LIFE)
    action.SetParam(70)
    action.SetBranch(179, 179, 111)
    g_AIDebugInfo[33] = '[33] CHECK_LIFE(70)'

    -- Npc标准技能选择
    action = ai.NewAction(111, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(120, 31)
    g_AIDebugInfo[111] = '[111] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(120, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(34, 128)
    g_AIDebugInfo[120] = '[120] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 释放技能
    action = ai.NewAction(34, AI_ACTION.CAST_SKILL)
    action.SetParam(302, 1)
    action.SetBranch(117, 117)
    g_AIDebugInfo[34] = '[34] CAST_SKILL(302, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(117, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(33, 74)
    g_AIDebugInfo[117] = '[117] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(74, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(33, 63)
    g_AIDebugInfo[74] = '[74] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 设置系统无敌
    action = ai.NewAction(63, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(52)
    g_AIDebugInfo[63] = '[63] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(52, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(64, 64)
    g_AIDebugInfo[52] = '[52] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(64, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(190)
    g_AIDebugInfo[64] = '[64] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(190, AI_ACTION.SET_STATE)
    action.SetParam(60) -- 无敌返回状态
    g_AIDebugInfo[190] = '[190] SetParam(State60)'

    -- 无敌返回状态
    state = ai.NewState(60)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 71)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 71)

    -- 返回
    action = ai.NewAction(71, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(67, 67)
    g_AIDebugInfo[71] = '[71] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(67, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(192)
    g_AIDebugInfo[67] = '[67] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(192, AI_ACTION.SET_STATE)
    action.SetParam(69) -- 返回成功
    g_AIDebugInfo[192] = '[192] SetParam(State69)'

    -- 返回成功
    state = ai.NewState(69)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 62)

    -- 清除DEBUFF
    action = ai.NewAction(62, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(59)
    g_AIDebugInfo[62] = '[62] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(59, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(4)
    g_AIDebugInfo[59] = '[59] SET_SYSTEM_SHEILD(0)'

    -- 设置主定时器
    action = ai.NewAction(128, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(194)
    g_AIDebugInfo[128] = '[128] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(194, AI_ACTION.SET_STATE)
    action.SetParam(127) -- 追击状态
    g_AIDebugInfo[194] = '[194] SetParam(State127)'

    -- 追击状态
    state = ai.NewState(127)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 129)

    -- 比较返回点距离
    action = ai.NewAction(129, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(2000)
    action.SetBranch(63, 120, 120)
    g_AIDebugInfo[129] = '[129] RETURN_POSITION_DISTANCE_COMPARE(2000)'

    -- 清除目标威胁
    action = ai.NewAction(179, AI_ACTION.CLEAR_THREAT_FOR_TARGET)
    action.SetBranch(45, 45)
    g_AIDebugInfo[179] = '[179] CLEAR_THREAT_FOR_TARGET()'

    -- 设置主定时器
    action = ai.NewAction(45, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(196)
    g_AIDebugInfo[45] = '[45] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(196, AI_ACTION.SET_STATE)
    action.SetParam(40) -- 发飙状态1
    g_AIDebugInfo[196] = '[196] SetParam(State40)'

    -- 发飙状态1
    state = ai.NewState(40)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 114)

    -- Npc标准技能选择
    action = ai.NewAction(114, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(136, 45)
    g_AIDebugInfo[114] = '[114] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(136, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(93, 138)
    g_AIDebugInfo[136] = '[136] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(138, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(198)
    g_AIDebugInfo[138] = '[138] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(198, AI_ACTION.SET_STATE)
    action.SetParam(137) -- 追击状态
    g_AIDebugInfo[198] = '[198] SetParam(State137)'

    -- 追击状态
    state = ai.NewState(137)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 139)

    -- 比较返回点距离
    action = ai.NewAction(139, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(2000)
    action.SetBranch(63, 136, 136)
    g_AIDebugInfo[139] = '[139] RETURN_POSITION_DISTANCE_COMPARE(2000)'

    -- NPC喊话
    action = ai.NewAction(93, 2093)
    action.SetBranch(92)
    g_AIDebugInfo[93] = '[93] CustomAction93()'

    -- 设置主定时器
    action = ai.NewAction(92, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(200)
    g_AIDebugInfo[92] = '[92] SET_PRIMARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(200, AI_ACTION.SET_STATE)
    action.SetParam(91) -- 喊话结束
    g_AIDebugInfo[200] = '[200] SetParam(State91)'

    -- 喊话结束
    state = ai.NewState(91)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 43)

    -- 释放技能
    action = ai.NewAction(43, AI_ACTION.CAST_SKILL)
    action.SetParam(351, 1)
    action.SetBranch(148, 148)
    g_AIDebugInfo[43] = '[43] CAST_SKILL(351, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(148, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(99, 99)
    g_AIDebugInfo[148] = '[148] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(99, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(202)
    g_AIDebugInfo[99] = '[99] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(202, AI_ACTION.SET_STATE)
    action.SetParam(97) -- 攻击状态2
    g_AIDebugInfo[202] = '[202] SetParam(State97)'

    -- 攻击状态2
    state = ai.NewState(97)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 63)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 163)

    -- Npc标准技能选择
    action = ai.NewAction(163, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(157, 157)
    g_AIDebugInfo[163] = '[163] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(157, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(155, 153)
    g_AIDebugInfo[157] = '[157] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(153, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(204)
    g_AIDebugInfo[153] = '[153] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(204, AI_ACTION.SET_STATE)
    action.SetParam(164) -- 追击状态
    g_AIDebugInfo[204] = '[204] SetParam(State164)'

    -- 追击状态
    state = ai.NewState(164)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 167)

    -- 比较返回点距离
    action = ai.NewAction(167, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(2000)
    action.SetBranch(63, 157, 157)
    g_AIDebugInfo[167] = '[167] RETURN_POSITION_DISTANCE_COMPARE(2000)'

    -- 释放技能
    action = ai.NewAction(155, AI_ACTION.CAST_SKILL)
    action.SetParam(302, 1)
    action.SetBranch(151, 151)
    g_AIDebugInfo[155] = '[155] CAST_SKILL(302, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(151, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(99, 156)
    g_AIDebugInfo[151] = '[151] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(156, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(99, 63)
    g_AIDebugInfo[156] = '[156] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 搜索敌人
    action = ai.NewAction(12, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(25, 4)
    g_AIDebugInfo[12] = '[12] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(105)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction93(pNpc, ai)
    -- 输入自定义动作实现脚本
    local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end


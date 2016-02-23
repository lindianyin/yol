-- 编写者：hejingxian
-- 版本号：9

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
    ai.RegisterUserAction(2011, 'CustomAction11')
    ai.RegisterUserAction(2097, 'CustomAction97')
    ai.RegisterUserAction(2114, 'CustomAction114')
    ai.RegisterUserAction(2176, 'CustomAction176')
    ai.RegisterUserAction(2179, 'CustomAction179')

    -- 副本-二郎神
    state = ai.NewState(31)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 148)

    -- 记录原点
    action = ai.NewAction(148, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(85)
    g_AIDebugInfo[148] = '[148] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(85, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(200)
    g_AIDebugInfo[85] = '[85] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(200, AI_ACTION.SET_STATE)
    action.SetParam(27) -- 待机状态
    g_AIDebugInfo[200] = '[200] SetParam(State27)'

    -- 待机状态
    state = ai.NewState(27)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 38)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 89)

    -- 搜索敌人
    action = ai.NewAction(38, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(88, 85)
    g_AIDebugInfo[38] = '[38] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(88, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(3)
    g_AIDebugInfo[88] = '[88] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(3, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(136)
    g_AIDebugInfo[3] = '[3] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(136, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(118)
    g_AIDebugInfo[136] = '[136] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(118, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(202)
    g_AIDebugInfo[118] = '[118] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(202, AI_ACTION.SET_STATE)
    action.SetParam(116) -- 战斗状态
    g_AIDebugInfo[202] = '[202] SetParam(State116)'

    -- 战斗状态
    state = ai.NewState(116)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 64)

    -- 选择第一威胁
    action = ai.NewAction(64, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(93, 85)
    g_AIDebugInfo[64] = '[64] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(93, AI_ACTION.CHECK_LIFE)
    action.SetParam(70)
    action.SetBranch(176, 176, 157)
    g_AIDebugInfo[93] = '[93] CHECK_LIFE(70)'

    -- NPC喊话
    action = ai.NewAction(176, 2176)
    action.SetBranch(58)
    g_AIDebugInfo[176] = '[176] CustomAction176()'

    -- 设置主定时器
    action = ai.NewAction(58, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(204)
    g_AIDebugInfo[58] = '[58] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(204, AI_ACTION.SET_STATE)
    action.SetParam(43) -- 喊话结束
    g_AIDebugInfo[204] = '[204] SetParam(State43)'

    -- 喊话结束
    state = ai.NewState(43)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 65)

    -- 释放技能
    action = ai.NewAction(65, AI_ACTION.CAST_SKILL)
    action.SetParam(385, 1)
    action.SetBranch(119, 58)
    g_AIDebugInfo[65] = '[65] CAST_SKILL(385, 1)'

    -- 设置主定时器
    action = ai.NewAction(119, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(206)
    g_AIDebugInfo[119] = '[119] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(206, AI_ACTION.SET_STATE)
    action.SetParam(126) -- 变大结束
    g_AIDebugInfo[206] = '[206] SetParam(State126)'

    -- 变大结束
    state = ai.NewState(126)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 7)

    -- 检查生命
    action = ai.NewAction(7, AI_ACTION.CHECK_LIFE)
    action.SetParam(40)
    action.SetBranch(97, 97, 164)
    g_AIDebugInfo[7] = '[7] CHECK_LIFE(40)'

    -- NPC喊话
    action = ai.NewAction(97, 2097)
    action.SetBranch(193)
    g_AIDebugInfo[97] = '[97] CustomAction97()'

    -- 设置主定时器
    action = ai.NewAction(193, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(208)
    g_AIDebugInfo[193] = '[193] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(208, AI_ACTION.SET_STATE)
    action.SetParam(191) -- 喊话结束
    g_AIDebugInfo[208] = '[208] SetParam(State191)'

    -- 喊话结束
    state = ai.NewState(191)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 55)

    -- 释放技能
    action = ai.NewAction(55, AI_ACTION.CAST_SKILL)
    action.SetParam(425, 1)
    action.SetBranch(92, 193)
    g_AIDebugInfo[55] = '[55] CAST_SKILL(425, 1)'

    -- 设置主定时器
    action = ai.NewAction(92, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(210)
    g_AIDebugInfo[92] = '[92] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(210, AI_ACTION.SET_STATE)
    action.SetParam(183) -- 变大结束
    g_AIDebugInfo[210] = '[210] SetParam(State183)'

    -- 变大结束
    state = ai.NewState(183)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 69)

    -- 设置主定时器
    action = ai.NewAction(69, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(212)
    g_AIDebugInfo[69] = '[69] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(212, AI_ACTION.SET_STATE)
    action.SetParam(105) -- 暴力状态
    g_AIDebugInfo[212] = '[212] SetParam(State105)'

    -- 暴力状态
    state = ai.NewState(105)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 99)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 77)

    -- 双选项随机
    action = ai.NewAction(99, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(10, 90)
    action.SetBranch(179, 103)
    g_AIDebugInfo[99] = '[99] RANDOM_BI_BRANCH(10, 90)'

    -- NPC喊话
    action = ai.NewAction(179, 2179)
    action.SetBranch(134)
    g_AIDebugInfo[179] = '[179] CustomAction179()'

    -- 设置主定时器
    action = ai.NewAction(134, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(214)
    g_AIDebugInfo[134] = '[134] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(214, AI_ACTION.SET_STATE)
    action.SetParam(187) -- 喊话结束
    g_AIDebugInfo[214] = '[214] SetParam(State187)'

    -- 喊话结束
    state = ai.NewState(187)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 103)

    -- Npc标准技能选择
    action = ai.NewAction(103, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(147, 69)
    g_AIDebugInfo[103] = '[103] NPC_STANDARD_SKILL_SELECTOR()'

    -- 双选项随机
    action = ai.NewAction(147, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(80, 20)
    action.SetBranch(22, 178)
    g_AIDebugInfo[147] = '[147] RANDOM_BI_BRANCH(80, 20)'

    -- 释放技能
    action = ai.NewAction(178, AI_ACTION.CAST_SKILL)
    action.SetParam(425, 1)
    action.SetBranch(63, 152)
    g_AIDebugInfo[178] = '[178] CAST_SKILL(425, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(152, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(50)
    action.SetBranch(69, 77)
    g_AIDebugInfo[152] = '[152] CHECK_SKILL_FAILED_COUNTER(50)'

    -- 设置系统无敌
    action = ai.NewAction(77, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(44)
    g_AIDebugInfo[77] = '[77] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(44, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(120, 120)
    g_AIDebugInfo[44] = '[44] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(120, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(216)
    g_AIDebugInfo[120] = '[120] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(216, AI_ACTION.SET_STATE)
    action.SetParam(19) -- 无敌返回状态
    g_AIDebugInfo[216] = '[216] SetParam(State19)'

    -- 无敌返回状态
    state = ai.NewState(19)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 72)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 72)

    -- 返回
    action = ai.NewAction(72, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(21, 21)
    g_AIDebugInfo[72] = '[72] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(21, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(218)
    g_AIDebugInfo[21] = '[21] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(218, AI_ACTION.SET_STATE)
    action.SetParam(168) -- 返回成功
    g_AIDebugInfo[218] = '[218] SetParam(State168)'

    -- 返回成功
    state = ai.NewState(168)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 80)

    -- 清除DEBUFF
    action = ai.NewAction(80, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(104)
    g_AIDebugInfo[80] = '[80] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(104, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(85)
    g_AIDebugInfo[104] = '[104] SET_SYSTEM_SHEILD(0)'

    -- 设置主定时器
    action = ai.NewAction(63, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(220)
    g_AIDebugInfo[63] = '[63] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(220, AI_ACTION.SET_STATE)
    action.SetParam(30) -- 旋转结束
    g_AIDebugInfo[220] = '[220] SetParam(State30)'

    -- 旋转结束
    state = ai.NewState(30)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 69)

    -- Npc保持技能释放距离
    action = ai.NewAction(22, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(171, 139)
    g_AIDebugInfo[22] = '[22] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(139, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(222)
    g_AIDebugInfo[139] = '[139] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(222, AI_ACTION.SET_STATE)
    action.SetParam(29) -- 追击状态
    g_AIDebugInfo[222] = '[222] SetParam(State29)'

    -- 追击状态
    state = ai.NewState(29)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 15)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 85)

    -- 比较返回点距离
    action = ai.NewAction(15, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(77, 22, 22)
    g_AIDebugInfo[15] = '[15] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(171, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(69, 152)
    g_AIDebugInfo[171] = '[171] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(164, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(224)
    g_AIDebugInfo[164] = '[164] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(224, AI_ACTION.SET_STATE)
    action.SetParam(175) -- 普通攻击
    g_AIDebugInfo[224] = '[224] SetParam(State175)'

    -- 普通攻击
    state = ai.NewState(175)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 115)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 77)

    -- 双选项随机
    action = ai.NewAction(115, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(10, 90)
    action.SetBranch(114, 122)
    g_AIDebugInfo[115] = '[115] RANDOM_BI_BRANCH(10, 90)'

    -- NPC喊话
    action = ai.NewAction(114, 2114)
    action.SetBranch(160)
    g_AIDebugInfo[114] = '[114] CustomAction114()'

    -- 设置主定时器
    action = ai.NewAction(160, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(226)
    g_AIDebugInfo[160] = '[160] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(226, AI_ACTION.SET_STATE)
    action.SetParam(117) -- 喊话结束
    g_AIDebugInfo[226] = '[226] SetParam(State117)'

    -- 喊话结束
    state = ai.NewState(117)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 122)

    -- Npc标准技能选择
    action = ai.NewAction(122, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(181, 119)
    g_AIDebugInfo[122] = '[122] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(181, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(129, 132)
    g_AIDebugInfo[181] = '[181] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(129, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(7, 106)
    g_AIDebugInfo[129] = '[129] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(106, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(50)
    action.SetBranch(7, 77)
    g_AIDebugInfo[106] = '[106] CHECK_SKILL_FAILED_COUNTER(50)'

    -- 设置主定时器
    action = ai.NewAction(132, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(228)
    g_AIDebugInfo[132] = '[132] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(228, AI_ACTION.SET_STATE)
    action.SetParam(189) -- 追击状态
    g_AIDebugInfo[228] = '[228] SetParam(State189)'

    -- 追击状态
    state = ai.NewState(189)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 94)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 85)

    -- 比较返回点距离
    action = ai.NewAction(94, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(77, 181, 181)
    g_AIDebugInfo[94] = '[94] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 设置主定时器
    action = ai.NewAction(157, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(230)
    g_AIDebugInfo[157] = '[157] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(230, AI_ACTION.SET_STATE)
    action.SetParam(24) -- 普通攻击
    g_AIDebugInfo[230] = '[230] SetParam(State24)'

    -- 普通攻击
    state = ai.NewState(24)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 49)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 77)

    -- 双选项随机
    action = ai.NewAction(49, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(10, 90)
    action.SetBranch(11, 39)
    g_AIDebugInfo[49] = '[49] RANDOM_BI_BRANCH(10, 90)'

    -- NPC喊话
    action = ai.NewAction(11, 2011)
    action.SetBranch(167)
    g_AIDebugInfo[11] = '[11] CustomAction11()'

    -- 设置主定时器
    action = ai.NewAction(167, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(232)
    g_AIDebugInfo[167] = '[167] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(232, AI_ACTION.SET_STATE)
    action.SetParam(91) -- 喊话结束
    g_AIDebugInfo[232] = '[232] SetParam(State91)'

    -- 喊话结束
    state = ai.NewState(91)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 39)

    -- Npc标准技能选择
    action = ai.NewAction(39, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(34, 118)
    g_AIDebugInfo[39] = '[39] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(34, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(28, 48)
    g_AIDebugInfo[34] = '[34] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(28, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(93, 53)
    g_AIDebugInfo[28] = '[28] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(53, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(50)
    action.SetBranch(93, 77)
    g_AIDebugInfo[53] = '[53] CHECK_SKILL_FAILED_COUNTER(50)'

    -- 设置主定时器
    action = ai.NewAction(48, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(234)
    g_AIDebugInfo[48] = '[48] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(234, AI_ACTION.SET_STATE)
    action.SetParam(186) -- 追击状态
    g_AIDebugInfo[234] = '[234] SetParam(State186)'

    -- 追击状态
    state = ai.NewState(186)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 74)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 85)

    -- 比较返回点距离
    action = ai.NewAction(74, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(77, 34, 34)
    g_AIDebugInfo[74] = '[74] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 选择事件发起者
    action = ai.NewAction(89, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(88, 85)
    g_AIDebugInfo[89] = '[89] SELECT_EVENT_SRC()'

    ai.SetInitState(31)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction176(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction97(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout3)
    return 1
end

function AI:CustomAction179(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout3)
    return 1
end

function AI:CustomAction114(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end

function AI:CustomAction11(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end



-- 编写者：wushiwen1
-- 版本号：5

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
    ai.RegisterUserAction(2050, 'CustomAction50')

    -- 初始
    state = ai.NewState(65)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 89)

    -- 记录原点
    action = ai.NewAction(89, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(115)
    g_AIDebugInfo[89] = '[89] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(115, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(138)
    g_AIDebugInfo[115] = '[115] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(138, AI_ACTION.SET_STATE)
    action.SetParam(7) -- 待机状态
    g_AIDebugInfo[138] = '[138] SetParam(State7)'

    -- 待机状态
    state = ai.NewState(7)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 59)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 54)

    -- 选择事件发起者
    action = ai.NewAction(59, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(132, 77)
    g_AIDebugInfo[59] = '[59] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(132, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(49)
    g_AIDebugInfo[132] = '[132] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(49, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(73)
    g_AIDebugInfo[49] = '[49] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(73, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(48)
    g_AIDebugInfo[73] = '[73] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(48, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(140)
    g_AIDebugInfo[48] = '[48] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(140, AI_ACTION.SET_STATE)
    action.SetParam(94) -- 战斗状态
    g_AIDebugInfo[140] = '[140] SetParam(State94)'

    -- 战斗状态
    state = ai.NewState(94)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 16)

    -- 选择第一威胁
    action = ai.NewAction(16, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(93, 77)
    g_AIDebugInfo[16] = '[16] SELECT_FIRST_THREAT()'

    -- 设置系统无敌
    action = ai.NewAction(77, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(117)
    g_AIDebugInfo[77] = '[77] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(117, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(81, 81)
    g_AIDebugInfo[117] = '[117] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(81, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(142)
    g_AIDebugInfo[81] = '[81] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(142, AI_ACTION.SET_STATE)
    action.SetParam(15) -- 无敌返回状态
    g_AIDebugInfo[142] = '[142] SetParam(State15)'

    -- 无敌返回状态
    state = ai.NewState(15)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 21)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 21)

    -- 返回
    action = ai.NewAction(21, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(122, 122)
    g_AIDebugInfo[21] = '[21] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(122, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(144)
    g_AIDebugInfo[122] = '[122] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(144, AI_ACTION.SET_STATE)
    action.SetParam(71) -- 返回成功
    g_AIDebugInfo[144] = '[144] SetParam(State71)'

    -- 返回成功
    state = ai.NewState(71)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 18)

    -- 清除DEBUFF
    action = ai.NewAction(18, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(120)
    g_AIDebugInfo[18] = '[18] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(120, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(115)
    g_AIDebugInfo[120] = '[120] SET_SYSTEM_SHEILD(0)'

    -- 检查生命
    action = ai.NewAction(93, AI_ACTION.CHECK_LIFE)
    action.SetParam(50)
    action.SetBranch(10, 10, 75)
    g_AIDebugInfo[93] = '[93] CHECK_LIFE(50)'

    -- 设置主定时器
    action = ai.NewAction(10, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(146)
    g_AIDebugInfo[10] = '[10] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(146, AI_ACTION.SET_STATE)
    action.SetParam(108) -- 发飙状态1
    g_AIDebugInfo[146] = '[146] SetParam(State108)'

    -- 发飙状态1
    state = ai.NewState(108)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 125)

    -- Npc标准技能选择
    action = ai.NewAction(125, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(68, 10)
    g_AIDebugInfo[125] = '[125] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(68, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(50, 133)
    g_AIDebugInfo[68] = '[68] NPC_KEEP_SKILL_CAST_RANGE()'

    -- NPC喊话
    action = ai.NewAction(50, 2050)
    action.SetBranch(22)
    g_AIDebugInfo[50] = '[50] CustomAction50()'

    -- 设置主定时器
    action = ai.NewAction(22, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(148)
    g_AIDebugInfo[22] = '[22] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(148, AI_ACTION.SET_STATE)
    action.SetParam(103) -- 喊话结束
    g_AIDebugInfo[148] = '[148] SetParam(State103)'

    -- 喊话结束
    state = ai.NewState(103)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 30)

    -- 释放技能
    action = ai.NewAction(30, AI_ACTION.CAST_SKILL)
    action.SetParam(133, 1)
    action.SetBranch(64, 64)
    g_AIDebugInfo[30] = '[30] CAST_SKILL(133, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(64, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(51, 51)
    g_AIDebugInfo[64] = '[64] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(51, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(150)
    g_AIDebugInfo[51] = '[51] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(150, AI_ACTION.SET_STATE)
    action.SetParam(110) -- 攻击状态2
    g_AIDebugInfo[150] = '[150] SetParam(State110)'

    -- 攻击状态2
    state = ai.NewState(110)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 77)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 106)

    -- Npc标准技能选择
    action = ai.NewAction(106, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(46, 46)
    g_AIDebugInfo[106] = '[106] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(46, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(136, 35)
    g_AIDebugInfo[46] = '[46] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(35, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(152)
    g_AIDebugInfo[35] = '[35] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(152, AI_ACTION.SET_STATE)
    action.SetParam(17) -- 追击状态
    g_AIDebugInfo[152] = '[152] SetParam(State17)'

    -- 追击状态
    state = ai.NewState(17)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 124)

    -- 比较返回点距离
    action = ai.NewAction(124, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(2000)
    action.SetBranch(77, 46, 46)
    g_AIDebugInfo[124] = '[124] RETURN_POSITION_DISTANCE_COMPARE(2000)'

    -- Npc标准技能选择
    action = ai.NewAction(136, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(11, 11)
    g_AIDebugInfo[136] = '[136] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(11, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(51, 92)
    g_AIDebugInfo[11] = '[11] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(92, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(51, 77)
    g_AIDebugInfo[92] = '[92] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 设置主定时器
    action = ai.NewAction(133, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(154)
    g_AIDebugInfo[133] = '[133] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(154, AI_ACTION.SET_STATE)
    action.SetParam(118) -- 追击状态
    g_AIDebugInfo[154] = '[154] SetParam(State118)'

    -- 追击状态
    state = ai.NewState(118)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 109)

    -- 比较返回点距离
    action = ai.NewAction(109, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(2000)
    action.SetBranch(77, 68, 68)
    g_AIDebugInfo[109] = '[109] RETURN_POSITION_DISTANCE_COMPARE(2000)'

    -- Npc标准技能选择
    action = ai.NewAction(75, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(3, 48)
    g_AIDebugInfo[75] = '[75] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(3, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(63, 90)
    g_AIDebugInfo[3] = '[3] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(90, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(156)
    g_AIDebugInfo[90] = '[90] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(156, AI_ACTION.SET_STATE)
    action.SetParam(98) -- 追击状态
    g_AIDebugInfo[156] = '[156] SetParam(State98)'

    -- 追击状态
    state = ai.NewState(98)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 83)

    -- 比较返回点距离
    action = ai.NewAction(83, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(2000)
    action.SetBranch(77, 3, 3)
    g_AIDebugInfo[83] = '[83] RETURN_POSITION_DISTANCE_COMPARE(2000)'

    -- 释放技能
    action = ai.NewAction(63, AI_ACTION.CAST_SKILL)
    action.SetParam(132, 1)
    action.SetBranch(38, 38)
    g_AIDebugInfo[63] = '[63] CAST_SKILL(132, 1)'

    -- Npc释放选定技能
    action = ai.NewAction(38, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(93, 79)
    g_AIDebugInfo[38] = '[38] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(79, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(93, 77)
    g_AIDebugInfo[79] = '[79] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 搜索敌人
    action = ai.NewAction(54, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(132, 115)
    g_AIDebugInfo[54] = '[54] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(65)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction50(pNpc, talkingStr)
    -- 输入自定义动作实现脚本
    Dialog:NpcBubbleToNearBy(pNpc.dwId, AI.ARGUMENT_STRING_TABLE[18])
    return 1
end


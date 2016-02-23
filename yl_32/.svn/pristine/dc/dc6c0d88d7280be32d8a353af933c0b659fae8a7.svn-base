-- 编写者：hejingxian
-- 版本号：26

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
    ai.RegisterUserAction(2085, 'CustomAction85')

    -- 初始
    state = ai.NewState(146)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 154)

    -- 记录原点
    action = ai.NewAction(154, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(91)
    g_AIDebugInfo[154] = '[154] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(91, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(201)
    g_AIDebugInfo[91] = '[91] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(201, AI_ACTION.SET_STATE)
    action.SetParam(79) -- 待机状态
    g_AIDebugInfo[201] = '[201] SetParam(State79)'

    -- 待机状态
    state = ai.NewState(79)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 134)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 142)

    -- 选择事件发起者
    action = ai.NewAction(134, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(124, 84)
    g_AIDebugInfo[134] = '[134] SELECT_EVENT_SRC()'

    -- 设置系统无敌
    action = ai.NewAction(84, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(121)
    g_AIDebugInfo[84] = '[84] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(121, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(86, 86)
    g_AIDebugInfo[121] = '[121] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(86, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(203)
    g_AIDebugInfo[86] = '[86] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(203, AI_ACTION.SET_STATE)
    action.SetParam(102) -- 无敌返回状态
    g_AIDebugInfo[203] = '[203] SetParam(State102)'

    -- 无敌返回状态
    state = ai.NewState(102)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 116)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 116)

    -- 返回
    action = ai.NewAction(116, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(125, 125)
    g_AIDebugInfo[116] = '[116] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(125, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(205)
    g_AIDebugInfo[125] = '[125] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(205, AI_ACTION.SET_STATE)
    action.SetParam(109) -- 返回成功
    g_AIDebugInfo[205] = '[205] SetParam(State109)'

    -- 返回成功
    state = ai.NewState(109)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 114)

    -- 清除DEBUFF
    action = ai.NewAction(114, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(129)
    g_AIDebugInfo[114] = '[114] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(129, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(91)
    g_AIDebugInfo[129] = '[129] SET_SYSTEM_SHEILD(0)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(124, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(87)
    g_AIDebugInfo[124] = '[124] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(87, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(103)
    g_AIDebugInfo[87] = '[87] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(103, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(147)
    g_AIDebugInfo[103] = '[103] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(147, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(207)
    g_AIDebugInfo[147] = '[147] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(207, AI_ACTION.SET_STATE)
    action.SetParam(94) -- 战斗状态
    g_AIDebugInfo[207] = '[207] SetParam(State94)'

    -- 战斗状态
    state = ai.NewState(94)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 100)

    -- 选择第一威胁
    action = ai.NewAction(100, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(108, 84)
    g_AIDebugInfo[100] = '[100] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(108, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(101, 147)
    g_AIDebugInfo[108] = '[108] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(101, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(85, 85)
    g_AIDebugInfo[101] = '[101] NPC_KEEP_SKILL_CAST_RANGE()'

    -- NPC喊话
    action = ai.NewAction(85, 2085)
    action.SetBranch(140)
    g_AIDebugInfo[85] = '[85] CustomAction85()'

    -- 设置主定时器
    action = ai.NewAction(140, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(209)
    g_AIDebugInfo[140] = '[140] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(209, AI_ACTION.SET_STATE)
    action.SetParam(152) -- 喊话结束
    g_AIDebugInfo[209] = '[209] SetParam(State152)'

    -- 喊话结束
    state = ai.NewState(152)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 88)

    -- Npc释放选定技能
    action = ai.NewAction(88, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(161, 137)
    g_AIDebugInfo[88] = '[88] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(137, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(157, 84)
    g_AIDebugInfo[137] = '[137] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 选择第一威胁
    action = ai.NewAction(157, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(159, 84)
    g_AIDebugInfo[157] = '[157] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(159, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(160, 161)
    g_AIDebugInfo[159] = '[159] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(160, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(88, 187)
    g_AIDebugInfo[160] = '[160] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(187, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(211)
    g_AIDebugInfo[187] = '[187] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(211, AI_ACTION.SET_STATE)
    action.SetParam(188) -- 追击状态
    g_AIDebugInfo[211] = '[211] SetParam(State188)'

    -- 追击状态
    state = ai.NewState(188)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 189)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 91)

    -- 比较返回点距离
    action = ai.NewAction(189, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(84, 160, 160)
    g_AIDebugInfo[189] = '[189] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 设置主定时器
    action = ai.NewAction(161, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(213)
    g_AIDebugInfo[161] = '[161] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(213, AI_ACTION.SET_STATE)
    action.SetParam(158) -- 战斗状态
    g_AIDebugInfo[213] = '[213] SetParam(State158)'

    -- 战斗状态
    state = ai.NewState(158)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 157)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 91)

    -- 搜索敌人
    action = ai.NewAction(142, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(124, 91)
    g_AIDebugInfo[142] = '[142] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(146)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction85(pNpc, ai)
    -- 输入自定义动作实现脚本
    local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end


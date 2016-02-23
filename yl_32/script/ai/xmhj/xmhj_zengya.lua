-- 编写者：lidianyi
-- 版本号：4

Include('script/ai/customfunctions.lua')
Include('script/ai/ai_param.lua')
Include('script/ai/argumentstrings.lua')

local g_AIDebugInfo = {}

function AI:Setup(ai)
    local state
    local action
    local param = AI.g_AIParam[ai.nAIType]

    -- 注册自定义动作
    ai.RegisterUserAction(2025, 'AI:ZengYaGetSelf')

    -- 初始
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)

    -- 记录原点
    action = ai.NewAction(6, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(25)
    g_AIDebugInfo[6] = '[6] RECORD_ORIGIN_POSITION()'

    -- 选中自己
    action = ai.NewAction(25, 2025)
    action.SetBranch(26)
    g_AIDebugInfo[25] = '[25] AI:ZengYaGetSelf()'

    -- 释放技能
    action = ai.NewAction(26, AI_ACTION.CAST_SKILL)
    action.SetParam(614, 1)
    action.SetBranch(28, 28)
    g_AIDebugInfo[26] = '[26] CAST_SKILL(614, 1)'

    -- 释放技能
    action = ai.NewAction(28, AI_ACTION.CAST_SKILL)
    action.SetParam(720, 1)
    action.SetBranch(2, 2)
    g_AIDebugInfo[28] = '[28] CAST_SKILL(720, 1)'

    -- 设置主定时器
    action = ai.NewAction(2, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(83)
    g_AIDebugInfo[2] = '[2] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(83, AI_ACTION.SET_STATE)
    action.SetParam(4) -- 待机状态
    g_AIDebugInfo[83] = '[83] SetParam(State4)'

    -- 待机状态
    state = ai.NewState(4)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 8)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 12)

    -- 搜索敌人
    action = ai.NewAction(8, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(1)
    action.SetBranch(14, 2)
    g_AIDebugInfo[8] = '[8] SEARCH_ENEMY(1)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(14, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(17)
    g_AIDebugInfo[14] = '[14] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(17, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(19)
    g_AIDebugInfo[17] = '[17] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(19, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(23)
    g_AIDebugInfo[19] = '[19] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(23, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(85)
    g_AIDebugInfo[23] = '[23] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(85, AI_ACTION.SET_STATE)
    action.SetParam(21) -- 战斗状态
    g_AIDebugInfo[85] = '[85] SetParam(State21)'

    -- 战斗状态
    state = ai.NewState(21)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 33)

    -- 选择第一威胁
    action = ai.NewAction(33, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(35, 75)
    g_AIDebugInfo[33] = '[33] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(35, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(37, 23)
    g_AIDebugInfo[35] = '[35] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(37, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(39, 50)
    g_AIDebugInfo[37] = '[37] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(39, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(41, 46)
    g_AIDebugInfo[39] = '[39] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(41, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(87)
    g_AIDebugInfo[41] = '[41] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(87, AI_ACTION.SET_STATE)
    action.SetParam(43) -- 技能冷却状态
    g_AIDebugInfo[87] = '[87] SetParam(State43)'

    -- 技能冷却状态
    state = ai.NewState(43)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 33)

    -- 检查技能失败计数
    action = ai.NewAction(46, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(23, 75)
    g_AIDebugInfo[46] = '[46] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置系统无敌
    action = ai.NewAction(75, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(71)
    g_AIDebugInfo[75] = '[75] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(71, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(65, 65)
    g_AIDebugInfo[71] = '[71] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(65, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(80)
    action.SetBranch(89)
    g_AIDebugInfo[65] = '[65] SET_PRIMARY_TIMER(80)'

    -- 设置状态
    action = ai.NewAction(89, AI_ACTION.SET_STATE)
    action.SetParam(77) -- 无敌返回状态
    g_AIDebugInfo[89] = '[89] SetParam(State77)'

    -- 无敌返回状态
    state = ai.NewState(77)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 76)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 76)

    -- 返回
    action = ai.NewAction(76, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(72, 72)
    g_AIDebugInfo[76] = '[76] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(72, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(91)
    g_AIDebugInfo[72] = '[72] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(91, AI_ACTION.SET_STATE)
    action.SetParam(74) -- 返回成功
    g_AIDebugInfo[91] = '[91] SetParam(State74)'

    -- 返回成功
    state = ai.NewState(74)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 59)

    -- 清除DEBUFF
    action = ai.NewAction(59, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(70)
    g_AIDebugInfo[59] = '[59] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(70, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(2)
    g_AIDebugInfo[70] = '[70] SET_SYSTEM_SHEILD(0)'

    -- 设置主定时器
    action = ai.NewAction(50, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(93)
    g_AIDebugInfo[50] = '[50] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(93, AI_ACTION.SET_STATE)
    action.SetParam(52) -- 追击状态
    g_AIDebugInfo[93] = '[93] SetParam(State52)'

    -- 追击状态
    state = ai.NewState(52)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 54)

    -- 比较返回点距离
    action = ai.NewAction(54, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(450)
    action.SetBranch(75, 37, 37)
    g_AIDebugInfo[54] = '[54] RETURN_POSITION_DISTANCE_COMPARE(450)'

    -- 选择事件发起者
    action = ai.NewAction(12, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(14, 75)
    g_AIDebugInfo[12] = '[12] SELECT_EVENT_SRC()'

    ai.SetInitState(1)
end

-- 调试方法
function AI:DebugOnCallAction(action_id)
    Log('[AI] Call action: ' .. g_AIDebugInfo[action_id])
end

-- 本AI图内自定义脚本
function AI:ZengYaGetSelf(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


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
    ai.RegisterUserAction(2020, 'AI:ZhuiFengGetSelf')

    -- 初始
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 16)

    -- 记录原点
    action = ai.NewAction(16, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(2)
    g_AIDebugInfo[16] = '[16] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(2, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(75)
    g_AIDebugInfo[2] = '[2] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(75, AI_ACTION.SET_STATE)
    action.SetParam(4) -- 待机状态
    g_AIDebugInfo[75] = '[75] SetParam(State4)'

    -- 待机状态
    state = ai.NewState(4)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 6)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 10)

    -- 选择事件发起者
    action = ai.NewAction(6, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(8, 65)
    g_AIDebugInfo[6] = '[6] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(8, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(14)
    g_AIDebugInfo[8] = '[8] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(14, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(18)
    g_AIDebugInfo[14] = '[14] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(18, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(20)
    g_AIDebugInfo[18] = '[18] NPC_INIT_SKILL_CD()'

    -- 自定义动作20
    action = ai.NewAction(20, 2020)
    action.SetBranch(22)
    g_AIDebugInfo[20] = '[20] AI:ZhuiFengGetSelf()'

    -- 释放技能
    action = ai.NewAction(22, AI_ACTION.CAST_SKILL)
    action.SetParam(569, 1)
    action.SetBranch(24, 24)
    g_AIDebugInfo[22] = '[22] CAST_SKILL(569, 1)'

    -- 设置主定时器
    action = ai.NewAction(24, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(77)
    g_AIDebugInfo[24] = '[24] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(77, AI_ACTION.SET_STATE)
    action.SetParam(26) -- 战斗状态
    g_AIDebugInfo[77] = '[77] SetParam(State26)'

    -- 战斗状态
    state = ai.NewState(26)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 28)

    -- 选择第一威胁
    action = ai.NewAction(28, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(30, 65)
    g_AIDebugInfo[28] = '[28] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(30, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(32, 24)
    g_AIDebugInfo[30] = '[30] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(32, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(34, 38)
    g_AIDebugInfo[32] = '[32] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(34, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(24, 46)
    g_AIDebugInfo[34] = '[34] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(46, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(28, 65)
    g_AIDebugInfo[46] = '[46] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置系统无敌
    action = ai.NewAction(65, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(53)
    g_AIDebugInfo[65] = '[65] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(53, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(57, 57)
    g_AIDebugInfo[53] = '[53] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(57, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(80)
    action.SetBranch(79)
    g_AIDebugInfo[57] = '[57] SET_PRIMARY_TIMER(80)'

    -- 设置状态
    action = ai.NewAction(79, AI_ACTION.SET_STATE)
    action.SetParam(62) -- 无敌返回状态
    g_AIDebugInfo[79] = '[79] SetParam(State62)'

    -- 无敌返回状态
    state = ai.NewState(62)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 49)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 49)

    -- 返回
    action = ai.NewAction(49, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(61, 61)
    g_AIDebugInfo[49] = '[49] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(61, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(81)
    g_AIDebugInfo[61] = '[61] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(81, AI_ACTION.SET_STATE)
    action.SetParam(66) -- 返回成功
    g_AIDebugInfo[81] = '[81] SetParam(State66)'

    -- 返回成功
    state = ai.NewState(66)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 59)

    -- 清除DEBUFF
    action = ai.NewAction(59, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(54)
    g_AIDebugInfo[59] = '[59] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(54, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(2)
    g_AIDebugInfo[54] = '[54] SET_SYSTEM_SHEILD(0)'

    -- 设置主定时器
    action = ai.NewAction(38, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(83)
    g_AIDebugInfo[38] = '[38] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(83, AI_ACTION.SET_STATE)
    action.SetParam(40) -- 追击状态
    g_AIDebugInfo[83] = '[83] SetParam(State40)'

    -- 追击状态
    state = ai.NewState(40)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 42)

    -- 比较返回点距离
    action = ai.NewAction(42, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(450)
    action.SetBranch(65, 32, 32)
    g_AIDebugInfo[42] = '[42] RETURN_POSITION_DISTANCE_COMPARE(450)'

    -- 搜索敌人
    action = ai.NewAction(10, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['SearchPlayerDistance'])
    action.SetBranch(8, 2)
    g_AIDebugInfo[10] = '[10] SEARCH_ENEMY(param[SearchPlayerDistance])'

    ai.SetInitState(1)
end

-- 调试方法
function AI:DebugOnCallAction(action_id)
    Log('[AI] Call action: ' .. g_AIDebugInfo[action_id])
end

-- 本AI图内自定义脚本
function AI:ZhuiFengGetSelf(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


-- 编写者：hejingxian
-- 版本号：21

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
    ai.RegisterUserAction(2109, 'AI:CustomAction109')

    -- 初始
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 31)

    -- 记录原点
    action = ai.NewAction(31, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(7)
    g_AIDebugInfo[31] = '[31] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(7, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(121)
    g_AIDebugInfo[7] = '[7] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(121, AI_ACTION.SET_STATE)
    action.SetParam(72) -- 待机状态
    g_AIDebugInfo[121] = '[121] SetParam(State72)'

    -- 待机状态
    state = ai.NewState(72)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 78)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 20)

    -- 选择事件发起者
    action = ai.NewAction(78, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(50, 57)
    g_AIDebugInfo[78] = '[78] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(50, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(55)
    g_AIDebugInfo[50] = '[50] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(55, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(10)
    g_AIDebugInfo[55] = '[55] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(10, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(47)
    g_AIDebugInfo[10] = '[10] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(47, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(123)
    g_AIDebugInfo[47] = '[47] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(123, AI_ACTION.SET_STATE)
    action.SetParam(17) -- 战斗状态
    g_AIDebugInfo[123] = '[123] SetParam(State17)'

    -- 战斗状态
    state = ai.NewState(17)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 73)

    -- 选择第一威胁
    action = ai.NewAction(73, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(34, 57)
    g_AIDebugInfo[73] = '[73] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(34, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(69, 47)
    g_AIDebugInfo[34] = '[34] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(69, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(67, 35)
    g_AIDebugInfo[69] = '[69] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(67, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(47, 62)
    g_AIDebugInfo[67] = '[67] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(62, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(73, 57)
    g_AIDebugInfo[62] = '[62] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 设置系统无敌
    action = ai.NewAction(57, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(6)
    g_AIDebugInfo[57] = '[57] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(6, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(61, 61)
    g_AIDebugInfo[6] = '[6] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(61, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(125)
    g_AIDebugInfo[61] = '[61] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(125, AI_ACTION.SET_STATE)
    action.SetParam(21) -- 无敌返回状态
    g_AIDebugInfo[125] = '[125] SetParam(State21)'

    -- 无敌返回状态
    state = ai.NewState(21)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 66)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 66)

    -- 返回
    action = ai.NewAction(66, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(53, 53)
    g_AIDebugInfo[66] = '[66] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(53, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(127)
    g_AIDebugInfo[53] = '[53] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(127, AI_ACTION.SET_STATE)
    action.SetParam(36) -- 返回成功
    g_AIDebugInfo[127] = '[127] SetParam(State36)'

    -- 返回成功
    state = ai.NewState(36)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 74)

    -- 清除DEBUFF
    action = ai.NewAction(74, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(49)
    g_AIDebugInfo[74] = '[74] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(49, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(7)
    g_AIDebugInfo[49] = '[49] SET_SYSTEM_SHEILD(0)'

    -- 设置主定时器
    action = ai.NewAction(35, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(116)
    g_AIDebugInfo[35] = '[35] SET_PRIMARY_TIMER(0)'

    -- 设置副定时器
    action = ai.NewAction(116, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(64)
    action.SetBranch(129)
    g_AIDebugInfo[116] = '[116] SET_SECONDARY_TIMER(64)'

    -- 设置状态
    action = ai.NewAction(129, AI_ACTION.SET_STATE)
    action.SetParam(48) -- 追击状态
    g_AIDebugInfo[129] = '[129] SetParam(State48)'

    -- 追击状态
    state = ai.NewState(48)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 38)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 109)

    -- 比较返回点距离
    action = ai.NewAction(38, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(57, 69, 69)
    g_AIDebugInfo[38] = '[38] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 选中自己
    action = ai.NewAction(109, 2109)
    action.SetBranch(117)
    g_AIDebugInfo[109] = '[109] AI:CustomAction109()'

    -- 释放技能
    action = ai.NewAction(117, AI_ACTION.CAST_SKILL)
    action.SetParam(388, 1)
    action.SetBranch(57, 35)
    g_AIDebugInfo[117] = '[117] CAST_SKILL(388, 1)'

    -- 搜索敌人
    action = ai.NewAction(20, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(50, 7)
    g_AIDebugInfo[20] = '[20] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(1)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction109(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


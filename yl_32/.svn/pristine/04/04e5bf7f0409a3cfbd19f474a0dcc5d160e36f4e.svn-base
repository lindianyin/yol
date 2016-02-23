-- 编写者：hejingxian
-- 版本号：8

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
    state = ai.NewState(22)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 2)

    -- 记录原点
    action = ai.NewAction(2, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(65)
    g_AIDebugInfo[2] = '[2] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(65, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(90)
    g_AIDebugInfo[65] = '[65] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(90, AI_ACTION.SET_STATE)
    action.SetParam(28) -- 待机状态
    g_AIDebugInfo[90] = '[90] SetParam(State28)'

    -- 待机状态
    state = ai.NewState(28)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 73)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 39)

    -- 选择事件发起者
    action = ai.NewAction(73, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(81, 9)
    g_AIDebugInfo[73] = '[73] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(81, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(76)
    g_AIDebugInfo[81] = '[81] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(76, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(36)
    g_AIDebugInfo[76] = '[76] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(36, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(5)
    g_AIDebugInfo[36] = '[36] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(5, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(92)
    g_AIDebugInfo[5] = '[5] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(92, AI_ACTION.SET_STATE)
    action.SetParam(37) -- 战斗状态
    g_AIDebugInfo[92] = '[92] SetParam(State37)'

    -- 战斗状态
    state = ai.NewState(37)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 33)

    -- 选择第一威胁
    action = ai.NewAction(33, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(49, 9)
    g_AIDebugInfo[33] = '[33] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(49, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(18, 5)
    g_AIDebugInfo[49] = '[49] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(18, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(65, 89)
    g_AIDebugInfo[18] = '[18] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(89, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10000)
    action.SetBranch(5, 9)
    g_AIDebugInfo[89] = '[89] CHECK_SKILL_FAILED_COUNTER(10000)'

    -- 设置系统无敌
    action = ai.NewAction(9, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(88)
    g_AIDebugInfo[9] = '[9] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(88, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(16, 16)
    g_AIDebugInfo[88] = '[88] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(16, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(94)
    g_AIDebugInfo[16] = '[16] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(94, AI_ACTION.SET_STATE)
    action.SetParam(50) -- 无敌返回状态
    g_AIDebugInfo[94] = '[94] SetParam(State50)'

    -- 无敌返回状态
    state = ai.NewState(50)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 29)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 29)

    -- 返回
    action = ai.NewAction(29, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(64, 64)
    g_AIDebugInfo[29] = '[29] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(64, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(96)
    g_AIDebugInfo[64] = '[64] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(96, AI_ACTION.SET_STATE)
    action.SetParam(25) -- 返回成功
    g_AIDebugInfo[96] = '[96] SetParam(State25)'

    -- 返回成功
    state = ai.NewState(25)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)

    -- 清除DEBUFF
    action = ai.NewAction(6, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(86)
    g_AIDebugInfo[6] = '[6] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(86, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(65)
    g_AIDebugInfo[86] = '[86] SET_SYSTEM_SHEILD(0)'

    -- 搜索敌人
    action = ai.NewAction(39, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(700)
    action.SetBranch(81, 65)
    g_AIDebugInfo[39] = '[39] SEARCH_ENEMY(700)'

    ai.SetInitState(22)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

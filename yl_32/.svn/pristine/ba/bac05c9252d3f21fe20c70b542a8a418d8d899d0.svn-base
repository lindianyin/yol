-- 编写者：hejingxian
-- 版本号：24

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
    state = ai.NewState(301)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 252)

    -- 记录原点
    action = ai.NewAction(252, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(313)
    g_AIDebugInfo[252] = '[252] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(313, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(404)
    g_AIDebugInfo[313] = '[313] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(404, AI_ACTION.SET_STATE)
    action.SetParam(260) -- 待机状态
    g_AIDebugInfo[404] = '[404] SetParam(State260)'

    -- 待机状态
    state = ai.NewState(260)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 262)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 309)

    -- 搜索敌人
    action = ai.NewAction(262, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(700)
    action.SetBranch(254, 313)
    g_AIDebugInfo[262] = '[262] SEARCH_ENEMY(700)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(254, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(318)
    g_AIDebugInfo[254] = '[254] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(318, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(256)
    g_AIDebugInfo[318] = '[318] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(256, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(281)
    g_AIDebugInfo[256] = '[256] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(281, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(406)
    g_AIDebugInfo[281] = '[281] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(406, AI_ACTION.SET_STATE)
    action.SetParam(288) -- 战斗状态
    g_AIDebugInfo[406] = '[406] SetParam(State288)'

    -- 战斗状态
    state = ai.NewState(288)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 249)

    -- 选择第一威胁
    action = ai.NewAction(249, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(269, 295)
    g_AIDebugInfo[249] = '[249] SELECT_FIRST_THREAT()'

    -- 设置系统无敌
    action = ai.NewAction(295, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(283)
    g_AIDebugInfo[295] = '[295] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(283, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(315, 315)
    g_AIDebugInfo[283] = '[283] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(315, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(408)
    g_AIDebugInfo[315] = '[315] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(408, AI_ACTION.SET_STATE)
    action.SetParam(310) -- 无敌返回状态
    g_AIDebugInfo[408] = '[408] SetParam(State310)'

    -- 无敌返回状态
    state = ai.NewState(310)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 311)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 311)

    -- 返回
    action = ai.NewAction(311, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(298, 298)
    g_AIDebugInfo[311] = '[311] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(298, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(410)
    g_AIDebugInfo[298] = '[298] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(410, AI_ACTION.SET_STATE)
    action.SetParam(276) -- 返回成功
    g_AIDebugInfo[410] = '[410] SetParam(State276)'

    -- 返回成功
    state = ai.NewState(276)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 280)

    -- 清除DEBUFF
    action = ai.NewAction(280, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(277)
    g_AIDebugInfo[280] = '[280] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(277, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(313)
    g_AIDebugInfo[277] = '[277] SET_SYSTEM_SHEILD(0)'

    -- Npc标准技能选择
    action = ai.NewAction(269, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(305, 281)
    g_AIDebugInfo[269] = '[269] NPC_STANDARD_SKILL_SELECTOR()'

    -- 保持距离
    action = ai.NewAction(305, AI_ACTION.KEEP_RANGE)
    action.SetParam(350, 450)
    action.SetBranch(247, 279)
    g_AIDebugInfo[305] = '[305] KEEP_RANGE(350, 450)'

    -- 设置主定时器
    action = ai.NewAction(279, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(412)
    g_AIDebugInfo[279] = '[279] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(412, AI_ACTION.SET_STATE)
    action.SetParam(248) -- 追击状态
    g_AIDebugInfo[412] = '[412] SetParam(State248)'

    -- 追击状态
    state = ai.NewState(248)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 293)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 313)

    -- 比较返回点距离
    action = ai.NewAction(293, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(10000)
    action.SetBranch(295, 247, 247)
    g_AIDebugInfo[293] = '[293] RETURN_POSITION_DISTANCE_COMPARE(10000)'

    -- Npc释放选定技能
    action = ai.NewAction(247, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(281, 306)
    g_AIDebugInfo[247] = '[247] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(306, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(100)
    action.SetBranch(249, 295)
    g_AIDebugInfo[306] = '[306] CHECK_SKILL_FAILED_COUNTER(100)'

    -- 选择事件发起者
    action = ai.NewAction(309, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(254, 295)
    g_AIDebugInfo[309] = '[309] SELECT_EVENT_SRC()'

    ai.SetInitState(301)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

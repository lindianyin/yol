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
    state = ai.NewState(449)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 398)

    -- 记录原点
    action = ai.NewAction(398, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(409)
    g_AIDebugInfo[398] = '[398] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(409, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(466)
    g_AIDebugInfo[409] = '[409] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(466, AI_ACTION.SET_STATE)
    action.SetParam(411) -- 待机状态
    g_AIDebugInfo[466] = '[466] SetParam(State411)'

    -- 待机状态
    state = ai.NewState(411)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 397)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 385)

    -- 选择事件发起者
    action = ai.NewAction(397, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(439, 444)
    g_AIDebugInfo[397] = '[397] SELECT_EVENT_SRC()'

    -- 设置系统无敌
    action = ai.NewAction(444, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(414)
    g_AIDebugInfo[444] = '[444] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(414, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(440, 440)
    g_AIDebugInfo[414] = '[414] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(440, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(468)
    g_AIDebugInfo[440] = '[440] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(468, AI_ACTION.SET_STATE)
    action.SetParam(376) -- 无敌返回状态
    g_AIDebugInfo[468] = '[468] SetParam(State376)'

    -- 无敌返回状态
    state = ai.NewState(376)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 412)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 412)

    -- 返回
    action = ai.NewAction(412, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(443, 443)
    g_AIDebugInfo[412] = '[412] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(443, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(470)
    g_AIDebugInfo[443] = '[443] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(470, AI_ACTION.SET_STATE)
    action.SetParam(403) -- 返回成功
    g_AIDebugInfo[470] = '[470] SetParam(State403)'

    -- 返回成功
    state = ai.NewState(403)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 425)

    -- 清除DEBUFF
    action = ai.NewAction(425, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(373)
    g_AIDebugInfo[425] = '[425] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(373, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(409)
    g_AIDebugInfo[373] = '[373] SET_SYSTEM_SHEILD(0)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(439, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(438)
    g_AIDebugInfo[439] = '[439] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(438, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(426)
    g_AIDebugInfo[438] = '[438] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(426, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(405)
    g_AIDebugInfo[426] = '[426] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(405, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(472)
    g_AIDebugInfo[405] = '[405] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(472, AI_ACTION.SET_STATE)
    action.SetParam(450) -- 战斗状态
    g_AIDebugInfo[472] = '[472] SetParam(State450)'

    -- 战斗状态
    state = ai.NewState(450)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 420)

    -- 选择第一威胁
    action = ai.NewAction(420, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(374, 444)
    g_AIDebugInfo[420] = '[420] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(374, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(437, 405)
    g_AIDebugInfo[374] = '[374] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(437, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(445, 422)
    g_AIDebugInfo[437] = '[437] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(445, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(405, 371)
    g_AIDebugInfo[445] = '[445] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(371, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(420, 444)
    g_AIDebugInfo[371] = '[371] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 设置主定时器
    action = ai.NewAction(422, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(474)
    g_AIDebugInfo[422] = '[422] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(474, AI_ACTION.SET_STATE)
    action.SetParam(380) -- 追击状态
    g_AIDebugInfo[474] = '[474] SetParam(State380)'

    -- 追击状态
    state = ai.NewState(380)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 457)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 379)
    state.HandleEvent(AI_EVENT.ON_CAST_SKILL_FAILED, 409)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 409)

    -- 选择事件发起者
    action = ai.NewAction(457, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(379, 405)
    g_AIDebugInfo[457] = '[457] SELECT_EVENT_SRC()'

    -- 比较返回点距离
    action = ai.NewAction(379, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(444, 437, 437)
    g_AIDebugInfo[379] = '[379] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- 搜索敌人
    action = ai.NewAction(385, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(439, 409)
    g_AIDebugInfo[385] = '[385] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(449)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

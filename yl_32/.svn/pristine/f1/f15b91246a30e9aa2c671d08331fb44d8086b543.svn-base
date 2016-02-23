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

    -- 初始
    state = ai.NewState(24)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 22)

    -- 记录原点
    action = ai.NewAction(22, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(9)
    g_AIDebugInfo[22] = '[22] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(9, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(60)
    g_AIDebugInfo[9] = '[9] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(60, AI_ACTION.SET_STATE)
    action.SetParam(38) -- 待机
    g_AIDebugInfo[60] = '[60] SetParam(State38)'

    -- 待机
    state = ai.NewState(38)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 36)

    -- 站立
    action = ai.NewAction(36, AI_ACTION.STAND)
    action.SetBranch(40)
    g_AIDebugInfo[36] = '[36] STAND()'

    -- 搜索玩家 不带任何效果
    action = ai.NewAction(40, AI_ACTION.SEARCH_PLAYER_WITHOUT_ANY_AFFECT)
    action.SetParam(param['AlertRange'])
    action.SetBranch(26, 9)
    g_AIDebugInfo[40] = '[40] SEARCH_PLAYER_WITHOUT_ANY_AFFECT(param[AlertRange])'

    -- 搜索敌人
    action = ai.NewAction(26, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(2000)
    action.SetBranch(49, 9)
    g_AIDebugInfo[26] = '[26] SEARCH_ENEMY(2000)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(49, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(52)
    g_AIDebugInfo[49] = '[49] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(52, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(53)
    g_AIDebugInfo[52] = '[52] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(53, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(45)
    g_AIDebugInfo[53] = '[53] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(45, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(62)
    g_AIDebugInfo[45] = '[45] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(62, AI_ACTION.SET_STATE)
    action.SetParam(55) -- 战斗状态
    g_AIDebugInfo[62] = '[62] SetParam(State55)'

    -- 战斗状态
    state = ai.NewState(55)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 51)

    -- 选择第一威胁
    action = ai.NewAction(51, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(18, 9)
    g_AIDebugInfo[51] = '[51] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(18, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(3, 9)
    g_AIDebugInfo[18] = '[18] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(3, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(9, 28)
    g_AIDebugInfo[3] = '[3] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(28, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(64)
    g_AIDebugInfo[28] = '[28] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(64, AI_ACTION.SET_STATE)
    action.SetParam(15) -- 释放技能结束
    g_AIDebugInfo[64] = '[64] SetParam(State15)'

    -- 释放技能结束
    state = ai.NewState(15)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 9)

    ai.SetInitState(24)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

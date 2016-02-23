-- 编写者：hejingxian
-- 版本号：23

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
    ai.RegisterUserAction(2035, 'AI:CustomAction35')

    -- 初始
    state = ai.NewState(4)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 5)

    -- 记录原点
    action = ai.NewAction(5, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(2)
    g_AIDebugInfo[5] = '[5] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(2, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(54)
    g_AIDebugInfo[2] = '[2] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(54, AI_ACTION.SET_STATE)
    action.SetParam(8) -- 搜索玩家
    g_AIDebugInfo[54] = '[54] SetParam(State8)'

    -- 搜索玩家
    state = ai.NewState(8)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 22)

    -- 搜索玩家 不带任何效果
    action = ai.NewAction(22, AI_ACTION.SEARCH_PLAYER_WITHOUT_ANY_AFFECT)
    action.SetParam(200)
    action.SetBranch(18, 2)
    g_AIDebugInfo[22] = '[22] SEARCH_PLAYER_WITHOUT_ANY_AFFECT(200)'

    -- 搜索敌人
    action = ai.NewAction(18, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(42, 2)
    g_AIDebugInfo[18] = '[18] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(42, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(48)
    g_AIDebugInfo[42] = '[42] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(48, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(41)
    g_AIDebugInfo[48] = '[48] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(41, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(50)
    g_AIDebugInfo[41] = '[41] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(50, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(56)
    g_AIDebugInfo[50] = '[50] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(56, AI_ACTION.SET_STATE)
    action.SetParam(47) -- 战斗状态
    g_AIDebugInfo[56] = '[56] SetParam(State47)'

    -- 战斗状态
    state = ai.NewState(47)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 49)

    -- 选择第一威胁
    action = ai.NewAction(49, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(25, 2)
    g_AIDebugInfo[49] = '[49] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(25, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(16, 50)
    g_AIDebugInfo[25] = '[25] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc释放选定技能
    action = ai.NewAction(16, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(24, 50)
    g_AIDebugInfo[16] = '[16] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(24, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(58)
    g_AIDebugInfo[24] = '[24] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(58, AI_ACTION.SET_STATE)
    action.SetParam(20) -- 释放技能结束
    g_AIDebugInfo[58] = '[58] SetParam(State20)'

    -- 释放技能结束
    state = ai.NewState(20)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 35)

    -- 选中自己
    action = ai.NewAction(35, 2035)
    action.SetBranch(27)
    g_AIDebugInfo[35] = '[35] AI:CustomAction35()'

    -- 释放技能
    action = ai.NewAction(27, AI_ACTION.CAST_SKILL)
    action.SetParam(370, 1)
    action.SetBranch(60, 62)
    g_AIDebugInfo[27] = '[27] CAST_SKILL(370, 1)'

    -- 设置状态
    action = ai.NewAction(60, AI_ACTION.SET_STATE)
    action.SetParam(37) -- 自杀
    g_AIDebugInfo[60] = '[60] SetParam(State37)'

    -- 自杀
    state = ai.NewState(37)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 34)

    -- 设置状态
    action = ai.NewAction(34, AI_ACTION.SET_STATE)
    action.SetParam(4)
    g_AIDebugInfo[34] = '[34] SET_STATE(4)'

    -- 设置状态
    action = ai.NewAction(62, AI_ACTION.SET_STATE)
    action.SetParam(37) -- 自杀
    g_AIDebugInfo[62] = '[62] SetParam(State37)'

    ai.SetInitState(4)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction35(pNpc, ai)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


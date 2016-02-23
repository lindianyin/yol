-- 编写者：hejingxian
-- 版本号：33

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
    ai.RegisterUserAction(2118, 'AI:CustomAction118')

    -- 初始
    state = ai.NewState(129)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 172)

    -- 记录原点
    action = ai.NewAction(172, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(124)
    g_AIDebugInfo[172] = '[172] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(124, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(208)
    g_AIDebugInfo[124] = '[124] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(208, AI_ACTION.SET_STATE)
    action.SetParam(116) -- 给自己加持续伤害
    g_AIDebugInfo[208] = '[208] SetParam(State116)'

    -- 给自己加持续伤害
    state = ai.NewState(116)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 118)

    -- 选中自己
    action = ai.NewAction(118, 2118)
    action.SetBranch(164)
    g_AIDebugInfo[118] = '[118] AI:CustomAction118()'

    -- 释放技能
    action = ai.NewAction(164, AI_ACTION.CAST_SKILL)
    action.SetParam(403, 1)
    action.SetBranch(131, 131)
    g_AIDebugInfo[164] = '[164] CAST_SKILL(403, 1)'

    -- 设置主定时器
    action = ai.NewAction(131, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(210)
    g_AIDebugInfo[131] = '[131] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(210, AI_ACTION.SET_STATE)
    action.SetParam(126) -- 搜索敌人
    g_AIDebugInfo[210] = '[210] SetParam(State126)'

    -- 搜索敌人
    state = ai.NewState(126)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 130)

    -- 搜索敌人
    action = ai.NewAction(130, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(133, 131)
    g_AIDebugInfo[130] = '[130] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(133, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(167)
    g_AIDebugInfo[133] = '[133] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(167, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(168)
    g_AIDebugInfo[167] = '[167] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(168, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(136)
    g_AIDebugInfo[168] = '[168] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(136, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(212)
    g_AIDebugInfo[136] = '[136] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(212, AI_ACTION.SET_STATE)
    action.SetParam(137) -- 追踪敌人
    g_AIDebugInfo[212] = '[212] SetParam(State137)'

    -- 追踪敌人
    state = ai.NewState(137)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 140)

    -- 选择第一威胁
    action = ai.NewAction(140, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(142, 136)
    g_AIDebugInfo[140] = '[140] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(142, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(143, 136)
    g_AIDebugInfo[142] = '[142] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(143, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(144, 187)
    g_AIDebugInfo[143] = '[143] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(144, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(136, 136)
    g_AIDebugInfo[144] = '[144] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(187, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(214)
    g_AIDebugInfo[187] = '[187] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(214, AI_ACTION.SET_STATE)
    action.SetParam(186) -- 追击状态
    g_AIDebugInfo[214] = '[214] SetParam(State186)'

    -- 追击状态
    state = ai.NewState(186)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 185)

    -- 比较返回点距离
    action = ai.NewAction(185, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(131, 143, 143)
    g_AIDebugInfo[185] = '[185] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    ai.SetInitState(129)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction118(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


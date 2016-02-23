-- 编写者：hejingxian
-- 版本号：2

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
    ai.RegisterUserAction(2033, 'CustomAction33')

    -- 初始
    state = ai.NewState(19)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 22)

    -- 记录原点
    action = ai.NewAction(22, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(29)
    g_AIDebugInfo[22] = '[22] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(29, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(51)
    g_AIDebugInfo[29] = '[29] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(51, AI_ACTION.SET_STATE)
    action.SetParam(34) -- 给自己加持续伤害
    g_AIDebugInfo[51] = '[51] SetParam(State34)'

    -- 给自己加持续伤害
    state = ai.NewState(34)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 33)

    -- 选中自己
    action = ai.NewAction(33, 2033)
    action.SetBranch(11)
    g_AIDebugInfo[33] = '[33] CustomAction33()'

    -- 释放技能
    action = ai.NewAction(11, AI_ACTION.CAST_SKILL)
    action.SetParam(410, 1)
    action.SetBranch(48, 48)
    g_AIDebugInfo[11] = '[11] CAST_SKILL(410, 1)'

    -- 设置主定时器
    action = ai.NewAction(48, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(53)
    g_AIDebugInfo[48] = '[48] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(53, AI_ACTION.SET_STATE)
    action.SetParam(40) -- 搜索敌人
    g_AIDebugInfo[53] = '[53] SetParam(State40)'

    -- 搜索敌人
    state = ai.NewState(40)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 2)

    -- 搜索敌人
    action = ai.NewAction(2, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(18, 48)
    g_AIDebugInfo[2] = '[2] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(18, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(23)
    g_AIDebugInfo[18] = '[18] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(23, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(7)
    g_AIDebugInfo[23] = '[23] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(7, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(49)
    g_AIDebugInfo[7] = '[7] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(49, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(55)
    g_AIDebugInfo[49] = '[49] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(55, AI_ACTION.SET_STATE)
    action.SetParam(41) -- 追踪敌人
    g_AIDebugInfo[55] = '[55] SetParam(State41)'

    -- 追踪敌人
    state = ai.NewState(41)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 14)

    -- 选择第一威胁
    action = ai.NewAction(14, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(30, 49)
    g_AIDebugInfo[14] = '[14] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(30, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(27, 49)
    g_AIDebugInfo[30] = '[30] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(27, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(4, 31)
    g_AIDebugInfo[27] = '[27] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(4, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(49, 49)
    g_AIDebugInfo[4] = '[4] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(31, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(57)
    g_AIDebugInfo[31] = '[31] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(57, AI_ACTION.SET_STATE)
    action.SetParam(13) -- 追击状态
    g_AIDebugInfo[57] = '[57] SetParam(State13)'

    -- 追击状态
    state = ai.NewState(13)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 50)

    -- 比较返回点距离
    action = ai.NewAction(50, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(48, 27, 27)
    g_AIDebugInfo[50] = '[50] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    ai.SetInitState(19)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction33(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


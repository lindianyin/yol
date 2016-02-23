-- 编写者：hejingxian
-- 版本号：1

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
    ai.RegisterUserAction(2016, 'CustomAction16')

    -- 初始
    state = ai.NewState(28)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1)

    -- 记录原点
    action = ai.NewAction(1, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(40)
    g_AIDebugInfo[1] = '[1] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(40, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(51)
    g_AIDebugInfo[40] = '[40] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(51, AI_ACTION.SET_STATE)
    action.SetParam(8) -- 给自己加持续伤害
    g_AIDebugInfo[51] = '[51] SetParam(State8)'

    -- 给自己加持续伤害
    state = ai.NewState(8)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 16)

    -- 选中自己
    action = ai.NewAction(16, 2016)
    action.SetBranch(5)
    g_AIDebugInfo[16] = '[16] CustomAction16()'

    -- 释放技能
    action = ai.NewAction(5, AI_ACTION.CAST_SKILL)
    action.SetParam(1161, 1)
    action.SetBranch(47, 47)
    g_AIDebugInfo[5] = '[5] CAST_SKILL(1161, 1)'

    -- 设置主定时器
    action = ai.NewAction(47, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(53)
    g_AIDebugInfo[47] = '[47] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(53, AI_ACTION.SET_STATE)
    action.SetParam(26) -- 搜索敌人
    g_AIDebugInfo[53] = '[53] SetParam(State26)'

    -- 搜索敌人
    state = ai.NewState(26)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 33)

    -- 搜索敌人
    action = ai.NewAction(33, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(19, 47)
    g_AIDebugInfo[33] = '[33] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(19, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(7)
    g_AIDebugInfo[19] = '[19] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(7, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(3)
    g_AIDebugInfo[7] = '[7] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(3, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(22)
    g_AIDebugInfo[3] = '[3] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(22, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(55)
    g_AIDebugInfo[22] = '[22] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(55, AI_ACTION.SET_STATE)
    action.SetParam(41) -- 追踪敌人
    g_AIDebugInfo[55] = '[55] SetParam(State41)'

    -- 追踪敌人
    state = ai.NewState(41)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 14)

    -- 选择第一威胁
    action = ai.NewAction(14, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(2, 22)
    g_AIDebugInfo[14] = '[14] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(2, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(32, 22)
    g_AIDebugInfo[2] = '[2] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(32, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(12, 38)
    g_AIDebugInfo[32] = '[32] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(38, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(57)
    g_AIDebugInfo[38] = '[38] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(57, AI_ACTION.SET_STATE)
    action.SetParam(9) -- 追击状态
    g_AIDebugInfo[57] = '[57] SetParam(State9)'

    -- 追击状态
    state = ai.NewState(9)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 37)

    -- 比较返回点距离
    action = ai.NewAction(37, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(47, 32, 32)
    g_AIDebugInfo[37] = '[37] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(12, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(22, 22)
    g_AIDebugInfo[12] = '[12] NPC_CAST_SELECT_SKILL()'

    ai.SetInitState(28)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction16(pNpc, ai)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


-- 编写者：hejingxian
-- 版本号：35

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
    ai.RegisterUserAction(2078, 'AI:CustomAction78')

    -- 多人副本-牛魔王
    state = ai.NewState(19)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 5)

    -- 记录原点
    action = ai.NewAction(5, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(40)
    g_AIDebugInfo[5] = '[5] RECORD_ORIGIN_POSITION()'

    -- 记录返回点
    action = ai.NewAction(40, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(3)
    g_AIDebugInfo[40] = '[40] RECORD_RETURN_POSITION()'

    -- 进入巡逻路径
    action = ai.NewAction(3, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(33)
    g_AIDebugInfo[3] = '[3] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(33, AI_ACTION.DO_PATROL)
    action.SetBranch(39, 2)
    g_AIDebugInfo[33] = '[33] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(2, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(89)
    g_AIDebugInfo[2] = '[2] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(89, AI_ACTION.SET_STATE)
    action.SetParam(47) -- 攻击仙女
    g_AIDebugInfo[89] = '[89] SetParam(State47)'

    -- 攻击仙女
    state = ai.NewState(47)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 35)

    -- 搜索敌人
    action = ai.NewAction(35, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(7, 2)
    g_AIDebugInfo[35] = '[35] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(7, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(34)
    g_AIDebugInfo[7] = '[7] ADD_TARGET_TO_THREAT_LIST()'

    -- Npc初始化技能CD
    action = ai.NewAction(34, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(44)
    g_AIDebugInfo[34] = '[34] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(44, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(91)
    g_AIDebugInfo[44] = '[44] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(91, AI_ACTION.SET_STATE)
    action.SetParam(11) -- 普通攻击
    g_AIDebugInfo[91] = '[91] SetParam(State11)'

    -- 普通攻击
    state = ai.NewState(11)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 84)

    -- 释放技能
    action = ai.NewAction(84, AI_ACTION.CAST_SKILL)
    action.SetParam(1603, 1)
    action.SetBranch(86, 2)
    g_AIDebugInfo[84] = '[84] CAST_SKILL(1603, 1)'

    -- 设置主定时器
    action = ai.NewAction(86, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(93)
    g_AIDebugInfo[86] = '[86] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(93, AI_ACTION.SET_STATE)
    action.SetParam(87) -- 普通攻击
    g_AIDebugInfo[93] = '[93] SetParam(State87)'

    -- 普通攻击
    state = ai.NewState(87)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 9)

    -- 检查技能失败计数
    action = ai.NewAction(9, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(50)
    action.SetBranch(44, 2)
    g_AIDebugInfo[9] = '[9] CHECK_SKILL_FAILED_COUNTER(50)'

    -- 设置主定时器
    action = ai.NewAction(39, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(64)
    action.SetBranch(95)
    g_AIDebugInfo[39] = '[39] SET_PRIMARY_TIMER(64)'

    -- 设置状态
    action = ai.NewAction(95, AI_ACTION.SET_STATE)
    action.SetParam(23) -- 主状态
    g_AIDebugInfo[95] = '[95] SetParam(State23)'

    -- 主状态
    state = ai.NewState(23)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 25)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 81)

    -- 设置副定时器
    action = ai.NewAction(25, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(64)
    action.SetBranch(39)
    g_AIDebugInfo[25] = '[25] SET_SECONDARY_TIMER(64)'

    -- Npc标准技能选择
    action = ai.NewAction(81, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(78, 33)
    g_AIDebugInfo[81] = '[81] NPC_STANDARD_SKILL_SELECTOR()'

    -- 选中自己
    action = ai.NewAction(78, 2078)
    action.SetBranch(80)
    g_AIDebugInfo[78] = '[78] AI:CustomAction78()'

    -- Npc释放选定技能
    action = ai.NewAction(80, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(33, 33)
    g_AIDebugInfo[80] = '[80] NPC_CAST_SELECT_SKILL()'

    ai.SetInitState(19)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction78(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


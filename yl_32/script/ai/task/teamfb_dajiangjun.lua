-- 编写者：hejingxian
-- 版本号：3

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
    ai.RegisterUserAction(2031, 'AI:CustomAction31')

    -- 多人副本-大将军
    state = ai.NewState(45)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 37)

    -- 记录原点
    action = ai.NewAction(37, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(21)
    g_AIDebugInfo[37] = '[37] RECORD_ORIGIN_POSITION()'

    -- 记录返回点
    action = ai.NewAction(21, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(4)
    g_AIDebugInfo[21] = '[21] RECORD_RETURN_POSITION()'

    -- 进入巡逻路径
    action = ai.NewAction(4, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(15)
    g_AIDebugInfo[4] = '[4] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(15, AI_ACTION.DO_PATROL)
    action.SetBranch(42, 43)
    g_AIDebugInfo[15] = '[15] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(42, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(64)
    action.SetBranch(52)
    g_AIDebugInfo[42] = '[42] SET_PRIMARY_TIMER(64)'

    -- 设置状态
    action = ai.NewAction(52, AI_ACTION.SET_STATE)
    action.SetParam(25) -- 主状态
    g_AIDebugInfo[52] = '[52] SetParam(State25)'

    -- 主状态
    state = ai.NewState(25)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 36)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 13)

    -- 设置副定时器
    action = ai.NewAction(36, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(64)
    action.SetBranch(42)
    g_AIDebugInfo[36] = '[36] SET_SECONDARY_TIMER(64)'

    -- Npc标准技能选择
    action = ai.NewAction(13, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(31, 15)
    g_AIDebugInfo[13] = '[13] NPC_STANDARD_SKILL_SELECTOR()'

    -- 选中自己
    action = ai.NewAction(31, 2031)
    action.SetBranch(33)
    g_AIDebugInfo[31] = '[31] AI:CustomAction31()'

    -- Npc释放选定技能
    action = ai.NewAction(33, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(15, 15)
    g_AIDebugInfo[33] = '[33] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(43, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(54)
    g_AIDebugInfo[43] = '[43] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(54, AI_ACTION.SET_STATE)
    action.SetParam(14) -- 攻击仙女
    g_AIDebugInfo[54] = '[54] SetParam(State14)'

    -- 攻击仙女
    state = ai.NewState(14)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 34)

    -- 搜索敌人
    action = ai.NewAction(34, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(38, 43)
    g_AIDebugInfo[34] = '[34] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(38, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(28)
    g_AIDebugInfo[38] = '[38] ADD_TARGET_TO_THREAT_LIST()'

    -- Npc初始化技能CD
    action = ai.NewAction(28, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(3)
    g_AIDebugInfo[28] = '[28] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(3, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(56)
    g_AIDebugInfo[3] = '[3] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(56, AI_ACTION.SET_STATE)
    action.SetParam(26) -- 普通攻击
    g_AIDebugInfo[56] = '[56] SetParam(State26)'

    -- 普通攻击
    state = ai.NewState(26)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 24)

    -- 释放技能
    action = ai.NewAction(24, AI_ACTION.CAST_SKILL)
    action.SetParam(1609, 1)
    action.SetBranch(29, 43)
    g_AIDebugInfo[24] = '[24] CAST_SKILL(1609, 1)'

    -- 设置主定时器
    action = ai.NewAction(29, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(58)
    g_AIDebugInfo[29] = '[29] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(58, AI_ACTION.SET_STATE)
    action.SetParam(51) -- 普通攻击
    g_AIDebugInfo[58] = '[58] SetParam(State51)'

    -- 普通攻击
    state = ai.NewState(51)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 12)

    -- 检查技能失败计数
    action = ai.NewAction(12, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(50)
    action.SetBranch(3, 43)
    g_AIDebugInfo[12] = '[12] CHECK_SKILL_FAILED_COUNTER(50)'

    ai.SetInitState(45)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction31(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


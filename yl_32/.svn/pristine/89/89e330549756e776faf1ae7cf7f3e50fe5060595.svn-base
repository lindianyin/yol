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
    ai.RegisterUserAction(2040, 'AI:CustomAction40')

    -- 多人副本-大教主
    state = ai.NewState(22)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 31)

    -- 记录原点
    action = ai.NewAction(31, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(34)
    g_AIDebugInfo[31] = '[31] RECORD_ORIGIN_POSITION()'

    -- 记录返回点
    action = ai.NewAction(34, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(25)
    g_AIDebugInfo[34] = '[34] RECORD_RETURN_POSITION()'

    -- 进入巡逻路径
    action = ai.NewAction(25, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(15)
    g_AIDebugInfo[25] = '[25] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(15, AI_ACTION.DO_PATROL)
    action.SetBranch(13, 7)
    g_AIDebugInfo[15] = '[15] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(13, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(64)
    action.SetBranch(52)
    g_AIDebugInfo[13] = '[13] SET_PRIMARY_TIMER(64)'

    -- 设置状态
    action = ai.NewAction(52, AI_ACTION.SET_STATE)
    action.SetParam(35) -- 主状态
    g_AIDebugInfo[52] = '[52] SetParam(State35)'

    -- 主状态
    state = ai.NewState(35)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 41)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 51)

    -- Npc标准技能选择
    action = ai.NewAction(41, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(40, 15)
    g_AIDebugInfo[41] = '[41] NPC_STANDARD_SKILL_SELECTOR()'

    -- 选中自己
    action = ai.NewAction(40, 2040)
    action.SetBranch(32)
    g_AIDebugInfo[40] = '[40] AI:CustomAction40()'

    -- Npc释放选定技能
    action = ai.NewAction(32, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(15, 15)
    g_AIDebugInfo[32] = '[32] NPC_CAST_SELECT_SKILL()'

    -- 设置副定时器
    action = ai.NewAction(51, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(64)
    action.SetBranch(13)
    g_AIDebugInfo[51] = '[51] SET_SECONDARY_TIMER(64)'

    -- 设置主定时器
    action = ai.NewAction(7, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(54)
    g_AIDebugInfo[7] = '[7] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(54, AI_ACTION.SET_STATE)
    action.SetParam(10) -- 攻击仙女
    g_AIDebugInfo[54] = '[54] SetParam(State10)'

    -- 攻击仙女
    state = ai.NewState(10)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 30)

    -- 搜索敌人
    action = ai.NewAction(30, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(12, 7)
    g_AIDebugInfo[30] = '[30] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(12, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(39)
    g_AIDebugInfo[12] = '[12] ADD_TARGET_TO_THREAT_LIST()'

    -- Npc初始化技能CD
    action = ai.NewAction(39, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(36)
    g_AIDebugInfo[39] = '[39] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(36, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(56)
    g_AIDebugInfo[36] = '[36] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(56, AI_ACTION.SET_STATE)
    action.SetParam(16) -- 普通攻击
    g_AIDebugInfo[56] = '[56] SetParam(State16)'

    -- 普通攻击
    state = ai.NewState(16)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 23)

    -- 释放技能
    action = ai.NewAction(23, AI_ACTION.CAST_SKILL)
    action.SetParam(1611, 1)
    action.SetBranch(45, 7)
    g_AIDebugInfo[23] = '[23] CAST_SKILL(1611, 1)'

    -- 设置主定时器
    action = ai.NewAction(45, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(58)
    g_AIDebugInfo[45] = '[45] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(58, AI_ACTION.SET_STATE)
    action.SetParam(42) -- 普通攻击
    g_AIDebugInfo[58] = '[58] SetParam(State42)'

    -- 普通攻击
    state = ai.NewState(42)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1)

    -- 检查技能失败计数
    action = ai.NewAction(1, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(50)
    action.SetBranch(36, 7)
    g_AIDebugInfo[1] = '[1] CHECK_SKILL_FAILED_COUNTER(50)'

    ai.SetInitState(22)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction40(pNpc, actionData)
    -- 输入自定义动作实现脚本
	pNpc.SetTarget(3,pNpc.dwId);
    return 1
end


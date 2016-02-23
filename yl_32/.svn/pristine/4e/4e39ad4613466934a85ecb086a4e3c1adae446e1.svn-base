-- 编写者：hejingxian
-- 版本号：22

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
    ai.RegisterUserAction(2031, 'CustomAction31')
    ai.RegisterUserAction(2036, 'CustomAction36')

    -- 初始
    state = ai.NewState(57)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 85)

    -- 记录原点
    action = ai.NewAction(85, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(17)
    g_AIDebugInfo[85] = '[85] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(17, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(119)
    g_AIDebugInfo[17] = '[17] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(119, AI_ACTION.SET_STATE)
    action.SetParam(48) -- 待机状态
    g_AIDebugInfo[119] = '[119] SetParam(State48)'

    -- 待机状态
    state = ai.NewState(48)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 53)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 4)

    -- 搜索敌人
    action = ai.NewAction(53, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(700)
    action.SetBranch(68, 17)
    g_AIDebugInfo[53] = '[53] SEARCH_ENEMY(700)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(68, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(62)
    g_AIDebugInfo[68] = '[68] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(62, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(30)
    g_AIDebugInfo[62] = '[62] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(30, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(59)
    g_AIDebugInfo[30] = '[30] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(59, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(121)
    g_AIDebugInfo[59] = '[59] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(121, AI_ACTION.SET_STATE)
    action.SetParam(8) -- 战斗状态
    g_AIDebugInfo[121] = '[121] SetParam(State8)'

    -- 战斗状态
    state = ai.NewState(8)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 77)

    -- 选择第一威胁
    action = ai.NewAction(77, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(39, 67)
    g_AIDebugInfo[77] = '[77] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(39, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(31, 59)
    g_AIDebugInfo[39] = '[39] NPC_STANDARD_SKILL_SELECTOR()'

    -- NPC喊话
    action = ai.NewAction(31, 2031)
    action.SetBranch(43)
    g_AIDebugInfo[31] = '[31] CustomAction31()'

    -- 设置主定时器
    action = ai.NewAction(43, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(123)
    g_AIDebugInfo[43] = '[43] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(123, AI_ACTION.SET_STATE)
    action.SetParam(42) -- 喊话结束
    g_AIDebugInfo[123] = '[123] SetParam(State42)'

    -- 喊话结束
    state = ai.NewState(42)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 13)

    -- Npc释放选定技能
    action = ai.NewAction(13, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(10, 15)
    g_AIDebugInfo[13] = '[13] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(10, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(125)
    g_AIDebugInfo[10] = '[10] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(125, AI_ACTION.SET_STATE)
    action.SetParam(20) -- 战斗状态
    g_AIDebugInfo[125] = '[125] SetParam(State20)'

    -- 战斗状态
    state = ai.NewState(20)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 116)

    -- 选择第一威胁
    action = ai.NewAction(116, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(81, 81)
    g_AIDebugInfo[116] = '[116] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(81, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(12, 10)
    g_AIDebugInfo[81] = '[81] NPC_STANDARD_SKILL_SELECTOR()'

    -- 双选项随机
    action = ai.NewAction(12, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(20, 80)
    action.SetBranch(80, 6)
    g_AIDebugInfo[12] = '[12] RANDOM_BI_BRANCH(20, 80)'

    -- 释放技能
    action = ai.NewAction(80, AI_ACTION.CAST_SKILL)
    action.SetParam(356, 1)
    action.SetBranch(36, 6)
    g_AIDebugInfo[80] = '[80] CAST_SKILL(356, 1)'

    -- 释放技能
    action = ai.NewAction(6, AI_ACTION.CAST_SKILL)
    action.SetParam(358, 1)
    action.SetBranch(13, 10)
    g_AIDebugInfo[6] = '[6] CAST_SKILL(358, 1)'

    -- NPC喊话
    action = ai.NewAction(36, 2036)
    action.SetBranch(2)
    g_AIDebugInfo[36] = '[36] CustomAction36()'

    -- 设置主定时器
    action = ai.NewAction(2, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(64)
    action.SetBranch(127)
    g_AIDebugInfo[2] = '[2] SET_PRIMARY_TIMER(64)'

    -- 设置状态
    action = ai.NewAction(127, AI_ACTION.SET_STATE)
    action.SetParam(18) -- 喊话结束
    g_AIDebugInfo[127] = '[127] SetParam(State18)'

    -- 喊话结束
    state = ai.NewState(18)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)

    -- 检查技能失败计数
    action = ai.NewAction(15, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(116, 67)
    g_AIDebugInfo[15] = '[15] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 设置系统无敌
    action = ai.NewAction(67, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(87)
    g_AIDebugInfo[67] = '[67] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(87, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(41, 41)
    g_AIDebugInfo[87] = '[87] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(41, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(129)
    g_AIDebugInfo[41] = '[41] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(129, AI_ACTION.SET_STATE)
    action.SetParam(83) -- 无敌返回状态
    g_AIDebugInfo[129] = '[129] SetParam(State83)'

    -- 无敌返回状态
    state = ai.NewState(83)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 45)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 45)

    -- 返回
    action = ai.NewAction(45, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(72, 72)
    g_AIDebugInfo[45] = '[45] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(72, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(131)
    g_AIDebugInfo[72] = '[72] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(131, AI_ACTION.SET_STATE)
    action.SetParam(51) -- 返回成功
    g_AIDebugInfo[131] = '[131] SetParam(State51)'

    -- 返回成功
    state = ai.NewState(51)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 27)

    -- 清除DEBUFF
    action = ai.NewAction(27, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(21)
    g_AIDebugInfo[27] = '[27] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(21, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(17)
    g_AIDebugInfo[21] = '[21] SET_SYSTEM_SHEILD(0)'

    -- 选择事件发起者
    action = ai.NewAction(4, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(68, 67)
    g_AIDebugInfo[4] = '[4] SELECT_EVENT_SRC()'

    ai.SetInitState(57)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction31(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction36(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end


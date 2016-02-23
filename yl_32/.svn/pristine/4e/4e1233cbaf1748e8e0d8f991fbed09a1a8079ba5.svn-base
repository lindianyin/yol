-- 编写者：hejingxian
-- 版本号：4

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
    ai.RegisterUserAction(2005, 'AI:CustomAction5')
    ai.RegisterUserAction(2012, 'AI:CustomAction12')

    -- 初始
    state = ai.NewState(3)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 34)

    -- 记录原点
    action = ai.NewAction(34, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(11)
    g_AIDebugInfo[34] = '[34] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(11, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(88)
    g_AIDebugInfo[11] = '[11] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(88, AI_ACTION.SET_STATE)
    action.SetParam(27) -- 待机状态
    g_AIDebugInfo[88] = '[88] SetParam(State27)'

    -- 待机状态
    state = ai.NewState(27)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 44)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 80)

    -- 搜索敌人
    action = ai.NewAction(44, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(53, 11)
    g_AIDebugInfo[44] = '[44] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(53, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(28)
    g_AIDebugInfo[53] = '[53] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(28, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(2)
    g_AIDebugInfo[28] = '[28] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(2, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(56)
    g_AIDebugInfo[2] = '[2] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(56, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(90)
    g_AIDebugInfo[56] = '[56] SET_PRIMARY_TIMER(1)'

    -- 设置状态
    action = ai.NewAction(90, AI_ACTION.SET_STATE)
    action.SetParam(36) -- 战斗状态
    g_AIDebugInfo[90] = '[90] SetParam(State36)'

    -- 战斗状态
    state = ai.NewState(36)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 4)

    -- 选择第一威胁
    action = ai.NewAction(4, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(42, 8)
    g_AIDebugInfo[4] = '[4] SELECT_FIRST_THREAT()'

    -- 设置系统无敌
    action = ai.NewAction(8, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(15)
    g_AIDebugInfo[8] = '[8] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(15, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(76, 76)
    g_AIDebugInfo[15] = '[15] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(76, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(92)
    g_AIDebugInfo[76] = '[76] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(92, AI_ACTION.SET_STATE)
    action.SetParam(31) -- 无敌返回状态
    g_AIDebugInfo[92] = '[92] SetParam(State31)'

    -- 无敌返回状态
    state = ai.NewState(31)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 9)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 9)

    -- 返回
    action = ai.NewAction(9, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(35, 35)
    g_AIDebugInfo[9] = '[9] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(35, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(94)
    g_AIDebugInfo[35] = '[35] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(94, AI_ACTION.SET_STATE)
    action.SetParam(18) -- 返回成功
    g_AIDebugInfo[94] = '[94] SetParam(State18)'

    -- 返回成功
    state = ai.NewState(18)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 60)

    -- 清除DEBUFF
    action = ai.NewAction(60, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(49)
    g_AIDebugInfo[60] = '[60] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(49, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(11)
    g_AIDebugInfo[49] = '[49] SET_SYSTEM_SHEILD(0)'

    -- Npc标准技能选择
    action = ai.NewAction(42, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(17, 56)
    g_AIDebugInfo[42] = '[42] NPC_STANDARD_SKILL_SELECTOR()'

    -- 三选项随机
    action = ai.NewAction(17, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(5, 5, 90)
    action.SetBranch(5, 12, 63)
    g_AIDebugInfo[17] = '[17] RANDOM_TRI_BRANCH(5, 5, 90)'

    -- NPC喊话
    action = ai.NewAction(5, 2005)
    action.SetBranch(69)
    g_AIDebugInfo[5] = '[5] AI:CustomAction5()'

    -- 设置主定时器
    action = ai.NewAction(69, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(96)
    g_AIDebugInfo[69] = '[69] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(96, AI_ACTION.SET_STATE)
    action.SetParam(24) -- 喊话结束
    g_AIDebugInfo[96] = '[96] SetParam(State24)'

    -- 喊话结束
    state = ai.NewState(24)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 63)

    -- Npc保持技能释放距离
    action = ai.NewAction(63, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(16, 25)
    g_AIDebugInfo[63] = '[63] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(16, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(56, 55)
    g_AIDebugInfo[16] = '[16] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(55, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(100)
    action.SetBranch(4, 8)
    g_AIDebugInfo[55] = '[55] CHECK_SKILL_FAILED_COUNTER(100)'

    -- 设置主定时器
    action = ai.NewAction(25, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(98)
    g_AIDebugInfo[25] = '[25] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(98, AI_ACTION.SET_STATE)
    action.SetParam(39) -- 追击状态
    g_AIDebugInfo[98] = '[98] SetParam(State39)'

    -- 追击状态
    state = ai.NewState(39)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 20)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 8)
    state.HandleEvent(AI_EVENT.ON_CAST_SKILL_FAILED, 11)

    -- 比较返回点距离
    action = ai.NewAction(20, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(8, 63, 63)
    g_AIDebugInfo[20] = '[20] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- NPC喊话
    action = ai.NewAction(12, 2012)
    action.SetBranch(6)
    g_AIDebugInfo[12] = '[12] AI:CustomAction12()'

    -- 设置主定时器
    action = ai.NewAction(6, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(100)
    g_AIDebugInfo[6] = '[6] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(100, AI_ACTION.SET_STATE)
    action.SetParam(23) -- 喊话结束
    g_AIDebugInfo[100] = '[100] SetParam(State23)'

    -- 喊话结束
    state = ai.NewState(23)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 63)

    -- 选择事件发起者
    action = ai.NewAction(80, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(53, 8)
    g_AIDebugInfo[80] = '[80] SELECT_EVENT_SRC()'

    ai.SetInitState(3)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction5(pNpc, ai)
    -- 输入自定义动作实现脚本
    local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction12(pNpc, ai)
    -- 输入自定义动作实现脚本
    local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end



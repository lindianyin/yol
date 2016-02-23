-- 编写者：hejingxian
-- 版本号：38

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
    ai.RegisterUserAction(2391, 'CustomAction391')
    ai.RegisterUserAction(2471, 'CustomAction471')

    -- 初始
    state = ai.NewState(388)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 443)

    -- 记录原点
    action = ai.NewAction(443, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(406)
    g_AIDebugInfo[443] = '[443] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(406, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(502)
    g_AIDebugInfo[406] = '[406] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(502, AI_ACTION.SET_STATE)
    action.SetParam(383) -- 待机状态
    g_AIDebugInfo[502] = '[502] SetParam(State383)'

    -- 待机状态
    state = ai.NewState(383)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 430)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 389)

    -- 选择事件发起者
    action = ai.NewAction(430, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(436, 398)
    g_AIDebugInfo[430] = '[430] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(436, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(408)
    g_AIDebugInfo[436] = '[436] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(408, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(409)
    g_AIDebugInfo[408] = '[408] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(409, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(390)
    g_AIDebugInfo[409] = '[409] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(390, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(504)
    g_AIDebugInfo[390] = '[390] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(504, AI_ACTION.SET_STATE)
    action.SetParam(396) -- 战斗状态
    g_AIDebugInfo[504] = '[504] SetParam(State396)'

    -- 战斗状态
    state = ai.NewState(396)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 423)

    -- 选择第一威胁
    action = ai.NewAction(423, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(380, 398)
    g_AIDebugInfo[423] = '[423] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(380, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(391, 390)
    g_AIDebugInfo[380] = '[380] NPC_STANDARD_SKILL_SELECTOR()'

    -- NPC喊话
    action = ai.NewAction(391, 2391)
    action.SetBranch(410)
    g_AIDebugInfo[391] = '[391] CustomAction391()'

    -- 设置主定时器
    action = ai.NewAction(410, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(0)
    action.SetBranch(506)
    g_AIDebugInfo[410] = '[410] SET_PRIMARY_TIMER(0)'

    -- 设置状态
    action = ai.NewAction(506, AI_ACTION.SET_STATE)
    action.SetParam(422) -- 喊话结束
    g_AIDebugInfo[506] = '[506] SetParam(State422)'

    -- 喊话结束
    state = ai.NewState(422)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 432)

    -- Npc释放选定技能
    action = ai.NewAction(432, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(418, 437)
    g_AIDebugInfo[432] = '[432] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(418, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(508)
    g_AIDebugInfo[418] = '[418] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(508, AI_ACTION.SET_STATE)
    action.SetParam(376) -- 战斗状态
    g_AIDebugInfo[508] = '[508] SetParam(State376)'

    -- 战斗状态
    state = ai.NewState(376)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 362)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 406)

    -- 选择第一威胁
    action = ai.NewAction(362, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(404, 398)
    g_AIDebugInfo[362] = '[362] SELECT_FIRST_THREAT()'

    -- Npc标准技能选择
    action = ai.NewAction(404, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(493, 418)
    g_AIDebugInfo[404] = '[404] NPC_STANDARD_SKILL_SELECTOR()'

    -- 双选项随机
    action = ai.NewAction(493, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(40, 60)
    action.SetBranch(468, 495)
    g_AIDebugInfo[493] = '[493] RANDOM_BI_BRANCH(40, 60)'

    -- 释放技能
    action = ai.NewAction(468, AI_ACTION.CAST_SKILL)
    action.SetParam(352, 1)
    action.SetBranch(471, 495)
    g_AIDebugInfo[468] = '[468] CAST_SKILL(352, 1)'

    -- NPC喊话
    action = ai.NewAction(471, 2471)
    action.SetBranch(472)
    g_AIDebugInfo[471] = '[471] CustomAction471()'

    -- 设置主定时器
    action = ai.NewAction(472, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(64)
    action.SetBranch(510)
    g_AIDebugInfo[472] = '[472] SET_PRIMARY_TIMER(64)'

    -- 设置状态
    action = ai.NewAction(510, AI_ACTION.SET_STATE)
    action.SetParam(470) -- 喊话结束
    g_AIDebugInfo[510] = '[510] SetParam(State470)'

    -- 喊话结束
    state = ai.NewState(470)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 495)

    -- 释放技能
    action = ai.NewAction(495, AI_ACTION.CAST_SKILL)
    action.SetParam(357, 1)
    action.SetBranch(432, 418)
    g_AIDebugInfo[495] = '[495] CAST_SKILL(357, 1)'

    -- 设置系统无敌
    action = ai.NewAction(398, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(420)
    g_AIDebugInfo[398] = '[398] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(420, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(403, 403)
    g_AIDebugInfo[420] = '[420] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(403, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(512)
    g_AIDebugInfo[403] = '[403] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(512, AI_ACTION.SET_STATE)
    action.SetParam(369) -- 无敌返回状态
    g_AIDebugInfo[512] = '[512] SetParam(State369)'

    -- 无敌返回状态
    state = ai.NewState(369)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 399)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 399)

    -- 返回
    action = ai.NewAction(399, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(440, 440)
    g_AIDebugInfo[399] = '[399] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(440, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(514)
    g_AIDebugInfo[440] = '[440] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(514, AI_ACTION.SET_STATE)
    action.SetParam(400) -- 返回成功
    g_AIDebugInfo[514] = '[514] SetParam(State400)'

    -- 返回成功
    state = ai.NewState(400)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 386)

    -- 清除DEBUFF
    action = ai.NewAction(386, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(407)
    g_AIDebugInfo[386] = '[386] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(407, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(406)
    g_AIDebugInfo[407] = '[407] SET_SYSTEM_SHEILD(0)'

    -- 检查技能失败计数
    action = ai.NewAction(437, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(5)
    action.SetBranch(362, 398)
    g_AIDebugInfo[437] = '[437] CHECK_SKILL_FAILED_COUNTER(5)'

    -- 搜索敌人
    action = ai.NewAction(389, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(700)
    action.SetBranch(436, 406)
    g_AIDebugInfo[389] = '[389] SEARCH_ENEMY(700)'

    ai.SetInitState(388)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction391(pNpc, ai)
    -- 输入自定义动作实现脚本
    local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout1)
    return 1
end

function AI:CustomAction471(pNpc, ai)
    -- 输入自定义动作实现脚本
    local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, param.Shout2)
    return 1
end



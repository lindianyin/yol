-- 编写者：hejingxian
-- 版本号：170

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
    ai.RegisterUserAction(2166, 'CustomAction166')
    ai.RegisterUserAction(2219, 'CustomAction219')
    ai.RegisterUserAction(2295, 'CustomAction295')
    ai.RegisterUserAction(2358, 'CustomAction358')
    ai.RegisterUserAction(2417, 'CustomAction417')
    ai.RegisterUserAction(2445, 'AI:CustomAction445')
    ai.RegisterUserAction(2548, 'CustomAction548')
    ai.RegisterUserAction(2641, 'CustomAction641')
    ai.RegisterUserAction(2672, 'CustomAction672')
    ai.RegisterUserAction(2711, 'CustomAction711')
    ai.RegisterUserAction(2743, 'CustomAction743')
    ai.RegisterUserAction(2803, 'CustomAction803')
    ai.RegisterUserAction(2834, 'CustomAction834')
    ai.RegisterUserAction(2880, 'CustomAction880')
    ai.RegisterUserAction(2917, 'CustomAction917')
    ai.RegisterUserAction(3005, 'CustomAction1005')
    ai.RegisterUserAction(3014, 'CustomAction1014')
    ai.RegisterUserAction(3079, 'CustomAction1079')
    ai.RegisterUserAction(3155, 'CustomAction1155')
    ai.RegisterUserAction(3163, 'CustomAction1163')
    ai.RegisterUserAction(3345, 'AI:CustomAction1345')
    ai.RegisterUserAction(3347, 'AI:CustomAction1347')
    ai.RegisterUserAction(3349, 'AI:CustomAction1349')
    ai.RegisterUserAction(3351, 'AI:CustomAction1351')

    -- 关卡-大将军
    state = ai.NewState(145)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 327)

    -- 记录原点
    action = ai.NewAction(327, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(272)
    g_AIDebugInfo[327] = '[327] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(272, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1505)
    g_AIDebugInfo[272] = '[272] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1505, AI_ACTION.SET_STATE)
    action.SetParam(69) -- 待机状态
    g_AIDebugInfo[1505] = '[1505] SetParam(State69)'

    -- 待机状态
    state = ai.NewState(69)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 11)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 169)

    -- 选择事件发起者
    action = ai.NewAction(11, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(49, 272)
    g_AIDebugInfo[11] = '[11] SELECT_EVENT_SRC()'

    -- 将目标添加到威胁列表
    action = ai.NewAction(49, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(95)
    g_AIDebugInfo[49] = '[49] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(95, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(98)
    g_AIDebugInfo[95] = '[95] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(98, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(243)
    g_AIDebugInfo[98] = '[98] NPC_INIT_SKILL_CD()'

    -- 设置主定时器
    action = ai.NewAction(243, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1507)
    g_AIDebugInfo[243] = '[243] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1507, AI_ACTION.SET_STATE)
    action.SetParam(137) -- 战斗状态
    g_AIDebugInfo[1507] = '[1507] SetParam(State137)'

    -- 战斗状态
    state = ai.NewState(137)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 228)

    -- 选择第一威胁
    action = ai.NewAction(228, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(162, 272)
    g_AIDebugInfo[228] = '[228] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(162, AI_ACTION.CHECK_LIFE)
    action.SetParam(90)
    action.SetBranch(342, 342, 230)
    g_AIDebugInfo[162] = '[162] CHECK_LIFE(90)'

    -- 设置主定时器
    action = ai.NewAction(342, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1509)
    g_AIDebugInfo[342] = '[342] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1509, AI_ACTION.SET_STATE)
    action.SetParam(48) -- 召唤陨石
    g_AIDebugInfo[1509] = '[1509] SetParam(State48)'

    -- 召唤陨石
    state = ai.NewState(48)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 166)

    -- NPC喊话
    action = ai.NewAction(166, 2166)
    action.SetBranch(156)
    g_AIDebugInfo[166] = '[166] CustomAction166()'

    -- 设置主定时器
    action = ai.NewAction(156, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(1511)
    g_AIDebugInfo[156] = '[156] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(1511, AI_ACTION.SET_STATE)
    action.SetParam(201) -- 喊话结束
    g_AIDebugInfo[1511] = '[1511] SetParam(State201)'

    -- 喊话结束
    state = ai.NewState(201)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 78)

    -- 释放技能
    action = ai.NewAction(78, AI_ACTION.CAST_SKILL)
    action.SetParam(1157, 1)
    action.SetBranch(32, 32)
    g_AIDebugInfo[78] = '[78] CAST_SKILL(1157, 1)'

    -- 设置主定时器
    action = ai.NewAction(32, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1513)
    g_AIDebugInfo[32] = '[32] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1513, AI_ACTION.SET_STATE)
    action.SetParam(190) -- 待机一会
    g_AIDebugInfo[1513] = '[1513] SetParam(State190)'

    -- 待机一会
    state = ai.NewState(190)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 114)

    -- 设置主定时器
    action = ai.NewAction(114, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1515)
    g_AIDebugInfo[114] = '[114] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1515, AI_ACTION.SET_STATE)
    action.SetParam(205) -- 进入下一状态
    g_AIDebugInfo[1515] = '[1515] SetParam(State205)'

    -- 进入下一状态
    state = ai.NewState(205)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 365)

    -- 选择第一威胁
    action = ai.NewAction(365, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(74, 1389)
    g_AIDebugInfo[365] = '[365] SELECT_FIRST_THREAT()'

    -- 站立
    action = ai.NewAction(1389, AI_ACTION.STAND)
    action.SetBranch(1391)
    g_AIDebugInfo[1389] = '[1389] STAND()'

    -- 设置主定时器
    action = ai.NewAction(1391, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1517)
    g_AIDebugInfo[1391] = '[1391] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1517, AI_ACTION.SET_STATE)
    action.SetParam(1390) -- 待机状态
    g_AIDebugInfo[1517] = '[1517] SetParam(State1390)'

    -- 待机状态
    state = ai.NewState(1390)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1204)

    -- 搜索敌人
    action = ai.NewAction(1204, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(1198, 1386)
    g_AIDebugInfo[1204] = '[1204] SEARCH_ENEMY(param[AlertRange])'

    -- 释放技能
    action = ai.NewAction(1386, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(1389, 1382)
    g_AIDebugInfo[1386] = '[1386] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(1382, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(1389, 1387)
    g_AIDebugInfo[1382] = '[1382] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置系统无敌
    action = ai.NewAction(1387, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(1388)
    g_AIDebugInfo[1387] = '[1387] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(1388, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(1361, 1361)
    g_AIDebugInfo[1388] = '[1388] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(1361, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(1519)
    g_AIDebugInfo[1361] = '[1361] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(1519, AI_ACTION.SET_STATE)
    action.SetParam(1362) -- 无敌返回状态
    g_AIDebugInfo[1519] = '[1519] SetParam(State1362)'

    -- 无敌返回状态
    state = ai.NewState(1362)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1367)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 1367)

    -- 返回
    action = ai.NewAction(1367, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(1360, 1360)
    g_AIDebugInfo[1367] = '[1367] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(1360, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(1521)
    g_AIDebugInfo[1360] = '[1360] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(1521, AI_ACTION.SET_STATE)
    action.SetParam(1370) -- 返回成功
    g_AIDebugInfo[1521] = '[1521] SetParam(State1370)'

    -- 返回成功
    state = ai.NewState(1370)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1384)

    -- 清除DEBUFF
    action = ai.NewAction(1384, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(1371)
    g_AIDebugInfo[1384] = '[1384] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(1371, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(272)
    g_AIDebugInfo[1371] = '[1371] SET_SYSTEM_SHEILD(0)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(1198, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(365)
    g_AIDebugInfo[1198] = '[1198] ADD_TARGET_TO_THREAT_LIST()'

    -- 检查生命
    action = ai.NewAction(74, AI_ACTION.CHECK_LIFE)
    action.SetParam(80)
    action.SetBranch(314, 314, 480)
    g_AIDebugInfo[74] = '[74] CHECK_LIFE(80)'

    -- 设置主定时器
    action = ai.NewAction(480, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1523)
    g_AIDebugInfo[480] = '[480] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1523, AI_ACTION.SET_STATE)
    action.SetParam(456) -- 普+闪+旋砍+陨石
    g_AIDebugInfo[1523] = '[1523] SetParam(State456)'

    -- 普+闪+旋砍+陨石
    state = ai.NewState(456)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 449)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 114)

    -- 三选项随机
    action = ai.NewAction(449, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(85, 10, 5)
    action.SetBranch(465, 445, 492)
    g_AIDebugInfo[449] = '[449] RANDOM_TRI_BRANCH(85, 10, 5)'

    -- NPC喊话
    action = ai.NewAction(445, 2445)
    action.SetBranch(455)
    g_AIDebugInfo[445] = '[445] AI:CustomAction445()'

    -- 设置主定时器
    action = ai.NewAction(455, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(1525)
    g_AIDebugInfo[455] = '[455] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(1525, AI_ACTION.SET_STATE)
    action.SetParam(483) -- 喊话结束
    g_AIDebugInfo[1525] = '[1525] SetParam(State483)'

    -- 喊话结束
    state = ai.NewState(483)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 473)

    -- 设置主定时器
    action = ai.NewAction(473, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1527)
    g_AIDebugInfo[473] = '[473] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1527, AI_ACTION.SET_STATE)
    action.SetParam(457) -- 待一会儿
    g_AIDebugInfo[1527] = '[1527] SetParam(State457)'

    -- 待一会儿
    state = ai.NewState(457)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 489)

    -- 释放技能
    action = ai.NewAction(489, AI_ACTION.CAST_SKILL)
    action.SetParam(1152, 1)
    action.SetBranch(454, 452)
    g_AIDebugInfo[489] = '[489] CAST_SKILL(1152, 1)'

    -- 设置主定时器
    action = ai.NewAction(454, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1529)
    g_AIDebugInfo[454] = '[454] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1529, AI_ACTION.SET_STATE)
    action.SetParam(471) -- 旋风砍结束
    g_AIDebugInfo[1529] = '[1529] SetParam(State471)'

    -- 旋风砍结束
    state = ai.NewState(471)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 74)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1389)

    -- 检查技能失败计数
    action = ai.NewAction(452, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(74, 1389)
    g_AIDebugInfo[452] = '[452] CHECK_SKILL_FAILED_COUNTER(10)'

    -- Npc标准技能选择
    action = ai.NewAction(465, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(464, 74)
    g_AIDebugInfo[465] = '[465] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(464, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(478, 443)
    g_AIDebugInfo[464] = '[464] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(443, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1531)
    g_AIDebugInfo[443] = '[443] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1531, AI_ACTION.SET_STATE)
    action.SetParam(462) -- 追击状态
    g_AIDebugInfo[1531] = '[1531] SetParam(State462)'

    -- 追击状态
    state = ai.NewState(462)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 459)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1389)

    -- 比较返回点距离
    action = ai.NewAction(459, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(114, 464, 464)
    g_AIDebugInfo[459] = '[459] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(478, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(74, 452)
    g_AIDebugInfo[478] = '[478] NPC_CAST_SELECT_SKILL()'

    -- 双选项随机
    action = ai.NewAction(492, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(50, 50)
    action.SetBranch(484, 500)
    g_AIDebugInfo[492] = '[492] RANDOM_BI_BRANCH(50, 50)'

    -- 释放技能
    action = ai.NewAction(484, AI_ACTION.CAST_SKILL)
    action.SetParam(1153, 1)
    action.SetBranch(472, 452)
    g_AIDebugInfo[484] = '[484] CAST_SKILL(1153, 1)'

    -- 设置主定时器
    action = ai.NewAction(472, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1533)
    g_AIDebugInfo[472] = '[472] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1533, AI_ACTION.SET_STATE)
    action.SetParam(479) -- 闪现结束
    g_AIDebugInfo[1533] = '[1533] SetParam(State479)'

    -- 闪现结束
    state = ai.NewState(479)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 74)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1389)

    -- 设置主定时器
    action = ai.NewAction(500, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(1535)
    g_AIDebugInfo[500] = '[500] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(1535, AI_ACTION.SET_STATE)
    action.SetParam(495) -- 待一会儿
    g_AIDebugInfo[1535] = '[1535] SetParam(State495)'

    -- 待一会儿
    state = ai.NewState(495)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 502)

    -- 释放技能
    action = ai.NewAction(502, AI_ACTION.CAST_SKILL)
    action.SetParam(1157, 1)
    action.SetBranch(504, 452)
    g_AIDebugInfo[502] = '[502] CAST_SKILL(1157, 1)'

    -- 设置主定时器
    action = ai.NewAction(504, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1537)
    g_AIDebugInfo[504] = '[504] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1537, AI_ACTION.SET_STATE)
    action.SetParam(505) -- 陨石结束
    g_AIDebugInfo[1537] = '[1537] SetParam(State505)'

    -- 陨石结束
    state = ai.NewState(505)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 74)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1389)

    -- 设置主定时器
    action = ai.NewAction(314, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(1539)
    g_AIDebugInfo[314] = '[314] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(1539, AI_ACTION.SET_STATE)
    action.SetParam(268) -- 返回中央
    g_AIDebugInfo[1539] = '[1539] SetParam(State268)'

    -- 返回中央
    state = ai.NewState(268)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 302)

    -- 进入巡逻路径
    action = ai.NewAction(302, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(333)
    g_AIDebugInfo[302] = '[302] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(333, AI_ACTION.DO_PATROL)
    action.SetBranch(199, 219)
    g_AIDebugInfo[333] = '[333] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(199, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1541)
    g_AIDebugInfo[199] = '[199] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1541, AI_ACTION.SET_STATE)
    action.SetParam(77) -- 主状态
    g_AIDebugInfo[1541] = '[1541] SetParam(State77)'

    -- 主状态
    state = ai.NewState(77)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 333)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 88)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 199)

    -- 设置副定时器
    action = ai.NewAction(88, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(4)
    action.SetBranch(219)
    g_AIDebugInfo[88] = '[88] SET_SECONDARY_TIMER(4)'

    -- NPC喊话
    action = ai.NewAction(219, 2219)
    action.SetBranch(117)
    g_AIDebugInfo[219] = '[219] CustomAction219()'

    -- 设置主定时器
    action = ai.NewAction(117, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(1543)
    g_AIDebugInfo[117] = '[117] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(1543, AI_ACTION.SET_STATE)
    action.SetParam(154) -- 喊话结束
    g_AIDebugInfo[1543] = '[1543] SetParam(State154)'

    -- 喊话结束
    state = ai.NewState(154)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 111)

    -- 释放技能
    action = ai.NewAction(111, AI_ACTION.CAST_SKILL)
    action.SetParam(1158, 1)
    action.SetBranch(1345, 1345)
    g_AIDebugInfo[111] = '[111] CAST_SKILL(1158, 1)'

    -- CALL NPC
    action = ai.NewAction(1345, 3345)
    action.SetBranch(186)
    g_AIDebugInfo[1345] = '[1345] AI:CustomAction1345()'

    -- 设置主定时器
    action = ai.NewAction(186, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(1545)
    g_AIDebugInfo[186] = '[186] SET_PRIMARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(1545, AI_ACTION.SET_STATE)
    action.SetParam(150) -- 召唤兵结束
    g_AIDebugInfo[1545] = '[1545] SetParam(State150)'

    -- 召唤兵结束
    state = ai.NewState(150)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 363)

    -- 设置主定时器
    action = ai.NewAction(363, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1547)
    g_AIDebugInfo[363] = '[363] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1547, AI_ACTION.SET_STATE)
    action.SetParam(369) -- 进入下一状态
    g_AIDebugInfo[1547] = '[1547] SetParam(State369)'

    -- 进入下一状态
    state = ai.NewState(369)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 280)

    -- 选择第一威胁
    action = ai.NewAction(280, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(136, 1403)
    g_AIDebugInfo[280] = '[280] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(136, AI_ACTION.CHECK_LIFE)
    action.SetParam(60)
    action.SetBranch(295, 295, 578)
    g_AIDebugInfo[136] = '[136] CHECK_LIFE(60)'

    -- 设置主定时器
    action = ai.NewAction(578, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1549)
    g_AIDebugInfo[578] = '[578] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1549, AI_ACTION.SET_STATE)
    action.SetParam(583) -- 普+闪+旋砍+陨石
    g_AIDebugInfo[1549] = '[1549] SetParam(State583)'

    -- 普+闪+旋砍+陨石
    state = ai.NewState(583)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 533)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1403)

    -- 三选项随机
    action = ai.NewAction(533, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(85, 10, 5)
    action.SetBranch(582, 548, 537)
    g_AIDebugInfo[533] = '[533] RANDOM_TRI_BRANCH(85, 10, 5)'

    -- 双选项随机
    action = ai.NewAction(537, AI_ACTION.RANDOM_BI_BRANCH)
    action.SetParam(50, 50)
    action.SetBranch(523, 546)
    g_AIDebugInfo[537] = '[537] RANDOM_BI_BRANCH(50, 50)'

    -- 设置主定时器
    action = ai.NewAction(546, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(1551)
    g_AIDebugInfo[546] = '[546] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(1551, AI_ACTION.SET_STATE)
    action.SetParam(565) -- 待一会儿
    g_AIDebugInfo[1551] = '[1551] SetParam(State565)'

    -- 待一会儿
    state = ai.NewState(565)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 522)

    -- 释放技能
    action = ai.NewAction(522, AI_ACTION.CAST_SKILL)
    action.SetParam(1157, 1)
    action.SetBranch(534, 520)
    g_AIDebugInfo[522] = '[522] CAST_SKILL(1157, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(520, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(136, 1403)
    g_AIDebugInfo[520] = '[520] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 站立
    action = ai.NewAction(1403, AI_ACTION.STAND)
    action.SetBranch(1401)
    g_AIDebugInfo[1403] = '[1403] STAND()'

    -- 设置主定时器
    action = ai.NewAction(1401, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1553)
    g_AIDebugInfo[1401] = '[1401] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1553, AI_ACTION.SET_STATE)
    action.SetParam(1399) -- 待机状态
    g_AIDebugInfo[1553] = '[1553] SetParam(State1399)'

    -- 待机状态
    state = ai.NewState(1399)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1209)

    -- 搜索敌人
    action = ai.NewAction(1209, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(1211, 1444)
    g_AIDebugInfo[1209] = '[1209] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(1211, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(280)
    g_AIDebugInfo[1211] = '[1211] ADD_TARGET_TO_THREAT_LIST()'

    -- 释放技能
    action = ai.NewAction(1444, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(1403, 1443)
    g_AIDebugInfo[1444] = '[1444] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(1443, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(1403, 1387)
    g_AIDebugInfo[1443] = '[1443] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置主定时器
    action = ai.NewAction(534, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1555)
    g_AIDebugInfo[534] = '[534] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1555, AI_ACTION.SET_STATE)
    action.SetParam(536) -- 陨石结束
    g_AIDebugInfo[1555] = '[1555] SetParam(State536)'

    -- 陨石结束
    state = ai.NewState(536)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 136)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1403)

    -- 释放技能
    action = ai.NewAction(523, AI_ACTION.CAST_SKILL)
    action.SetParam(1153, 1)
    action.SetBranch(557, 520)
    g_AIDebugInfo[523] = '[523] CAST_SKILL(1153, 1)'

    -- 设置主定时器
    action = ai.NewAction(557, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1557)
    g_AIDebugInfo[557] = '[557] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1557, AI_ACTION.SET_STATE)
    action.SetParam(545) -- 闪现结束
    g_AIDebugInfo[1557] = '[1557] SetParam(State545)'

    -- 闪现结束
    state = ai.NewState(545)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 136)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1403)

    -- NPC喊话
    action = ai.NewAction(548, 2548)
    action.SetBranch(550)
    g_AIDebugInfo[548] = '[548] CustomAction548()'

    -- 设置主定时器
    action = ai.NewAction(550, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(1559)
    g_AIDebugInfo[550] = '[550] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(1559, AI_ACTION.SET_STATE)
    action.SetParam(581) -- 喊话结束
    g_AIDebugInfo[1559] = '[1559] SetParam(State581)'

    -- 喊话结束
    state = ai.NewState(581)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 570)

    -- 设置主定时器
    action = ai.NewAction(570, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1561)
    g_AIDebugInfo[570] = '[570] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1561, AI_ACTION.SET_STATE)
    action.SetParam(535) -- 待一会儿
    g_AIDebugInfo[1561] = '[1561] SetParam(State535)'

    -- 待一会儿
    state = ai.NewState(535)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 538)

    -- 释放技能
    action = ai.NewAction(538, AI_ACTION.CAST_SKILL)
    action.SetParam(1152, 1)
    action.SetBranch(576, 520)
    g_AIDebugInfo[538] = '[538] CAST_SKILL(1152, 1)'

    -- 设置主定时器
    action = ai.NewAction(576, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1563)
    g_AIDebugInfo[576] = '[576] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1563, AI_ACTION.SET_STATE)
    action.SetParam(558) -- 旋风砍结束
    g_AIDebugInfo[1563] = '[1563] SetParam(State558)'

    -- 旋风砍结束
    state = ai.NewState(558)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 136)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1403)

    -- Npc标准技能选择
    action = ai.NewAction(582, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(528, 136)
    g_AIDebugInfo[582] = '[582] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(528, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(547, 551)
    g_AIDebugInfo[528] = '[528] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(551, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1565)
    g_AIDebugInfo[551] = '[551] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1565, AI_ACTION.SET_STATE)
    action.SetParam(563) -- 追击状态
    g_AIDebugInfo[1565] = '[1565] SetParam(State563)'

    -- 追击状态
    state = ai.NewState(563)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 589)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1403)

    -- 比较返回点距离
    action = ai.NewAction(589, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(363, 528, 528)
    g_AIDebugInfo[589] = '[589] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(547, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(136, 520)
    g_AIDebugInfo[547] = '[547] NPC_CAST_SELECT_SKILL()'

    -- NPC喊话
    action = ai.NewAction(295, 2295)
    action.SetBranch(368)
    g_AIDebugInfo[295] = '[295] CustomAction295()'

    -- 设置主定时器
    action = ai.NewAction(368, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(1567)
    g_AIDebugInfo[368] = '[368] SET_PRIMARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(1567, AI_ACTION.SET_STATE)
    action.SetParam(75) -- 喊话结束
    g_AIDebugInfo[1567] = '[1567] SetParam(State75)'

    -- 喊话结束
    state = ai.NewState(75)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 82)

    -- 释放技能
    action = ai.NewAction(82, AI_ACTION.CAST_SKILL)
    action.SetParam(1154, 1)
    action.SetBranch(25, 25)
    g_AIDebugInfo[82] = '[82] CAST_SKILL(1154, 1)'

    -- 设置主定时器
    action = ai.NewAction(25, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(160)
    action.SetBranch(1569)
    g_AIDebugInfo[25] = '[25] SET_PRIMARY_TIMER(160)'

    -- 设置状态
    action = ai.NewAction(1569, AI_ACTION.SET_STATE)
    action.SetParam(151) -- 龙卷风释放结束
    g_AIDebugInfo[1569] = '[1569] SetParam(State151)'

    -- 龙卷风释放结束
    state = ai.NewState(151)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 10)

    -- 选择第一威胁
    action = ai.NewAction(10, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(60, 1409)
    g_AIDebugInfo[10] = '[10] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(60, AI_ACTION.CHECK_LIFE)
    action.SetParam(50)
    action.SetBranch(817, 817, 654)
    g_AIDebugInfo[60] = '[60] CHECK_LIFE(50)'

    -- 设置主定时器
    action = ai.NewAction(654, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1571)
    g_AIDebugInfo[654] = '[654] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1571, AI_ACTION.SET_STATE)
    action.SetParam(611) -- 普+闪+旋砍+陨石+龙卷风
    g_AIDebugInfo[1571] = '[1571] SetParam(State611)'

    -- 普+闪+旋砍+陨石+龙卷风
    state = ai.NewState(611)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 612)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1409)

    -- 三选项随机
    action = ai.NewAction(612, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(70, 10, 20)
    action.SetBranch(646, 641, 659)
    g_AIDebugInfo[612] = '[612] RANDOM_TRI_BRANCH(70, 10, 20)'

    -- NPC喊话
    action = ai.NewAction(641, 2641)
    action.SetBranch(616)
    g_AIDebugInfo[641] = '[641] CustomAction641()'

    -- 设置主定时器
    action = ai.NewAction(616, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(1573)
    g_AIDebugInfo[616] = '[616] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(1573, AI_ACTION.SET_STATE)
    action.SetParam(642) -- 喊话结束
    g_AIDebugInfo[1573] = '[1573] SetParam(State642)'

    -- 喊话结束
    state = ai.NewState(642)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 600)

    -- 设置主定时器
    action = ai.NewAction(600, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1575)
    g_AIDebugInfo[600] = '[600] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1575, AI_ACTION.SET_STATE)
    action.SetParam(629) -- 待一会儿
    g_AIDebugInfo[1575] = '[1575] SetParam(State629)'

    -- 待一会儿
    state = ai.NewState(629)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 645)

    -- 释放技能
    action = ai.NewAction(645, AI_ACTION.CAST_SKILL)
    action.SetParam(1152, 1)
    action.SetBranch(633, 625)
    g_AIDebugInfo[645] = '[645] CAST_SKILL(1152, 1)'

    -- 设置主定时器
    action = ai.NewAction(633, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1577)
    g_AIDebugInfo[633] = '[633] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1577, AI_ACTION.SET_STATE)
    action.SetParam(606) -- 旋风砍结束
    g_AIDebugInfo[1577] = '[1577] SetParam(State606)'

    -- 旋风砍结束
    state = ai.NewState(606)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 60)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1409)

    -- 站立
    action = ai.NewAction(1409, AI_ACTION.STAND)
    action.SetBranch(1405)
    g_AIDebugInfo[1409] = '[1409] STAND()'

    -- 设置主定时器
    action = ai.NewAction(1405, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1579)
    g_AIDebugInfo[1405] = '[1405] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1579, AI_ACTION.SET_STATE)
    action.SetParam(1408) -- 待机状态
    g_AIDebugInfo[1579] = '[1579] SetParam(State1408)'

    -- 待机状态
    state = ai.NewState(1408)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1216)

    -- 搜索敌人
    action = ai.NewAction(1216, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(1220, 1449)
    g_AIDebugInfo[1216] = '[1216] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(1220, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(10)
    g_AIDebugInfo[1220] = '[1220] ADD_TARGET_TO_THREAT_LIST()'

    -- 释放技能
    action = ai.NewAction(1449, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(1409, 1450)
    g_AIDebugInfo[1449] = '[1449] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(1450, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(1409, 1387)
    g_AIDebugInfo[1450] = '[1450] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 检查技能失败计数
    action = ai.NewAction(625, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(60, 1409)
    g_AIDebugInfo[625] = '[625] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 三选项随机
    action = ai.NewAction(659, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(30, 40, 30)
    action.SetBranch(638, 650, 672)
    g_AIDebugInfo[659] = '[659] RANDOM_TRI_BRANCH(30, 40, 30)'

    -- 设置主定时器
    action = ai.NewAction(650, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(1581)
    g_AIDebugInfo[650] = '[650] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(1581, AI_ACTION.SET_STATE)
    action.SetParam(622) -- 待一会儿
    g_AIDebugInfo[1581] = '[1581] SetParam(State622)'

    -- 待一会儿
    state = ai.NewState(622)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 639)

    -- 释放技能
    action = ai.NewAction(639, AI_ACTION.CAST_SKILL)
    action.SetParam(1157, 1)
    action.SetBranch(597, 625)
    g_AIDebugInfo[639] = '[639] CAST_SKILL(1157, 1)'

    -- 设置主定时器
    action = ai.NewAction(597, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1583)
    g_AIDebugInfo[597] = '[597] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1583, AI_ACTION.SET_STATE)
    action.SetParam(649) -- 陨石结束
    g_AIDebugInfo[1583] = '[1583] SetParam(State649)'

    -- 陨石结束
    state = ai.NewState(649)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 60)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1409)

    -- NPC喊话
    action = ai.NewAction(672, 2672)
    action.SetBranch(660)
    g_AIDebugInfo[672] = '[672] CustomAction672()'

    -- 设置主定时器
    action = ai.NewAction(660, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1585)
    g_AIDebugInfo[660] = '[660] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1585, AI_ACTION.SET_STATE)
    action.SetParam(670) -- 喊话结束
    g_AIDebugInfo[1585] = '[1585] SetParam(State670)'

    -- 喊话结束
    state = ai.NewState(670)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 662)

    -- 设置主定时器
    action = ai.NewAction(662, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1587)
    g_AIDebugInfo[662] = '[662] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1587, AI_ACTION.SET_STATE)
    action.SetParam(668) -- 待一会儿
    g_AIDebugInfo[1587] = '[1587] SetParam(State668)'

    -- 待一会儿
    state = ai.NewState(668)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 665)

    -- 释放技能
    action = ai.NewAction(665, AI_ACTION.CAST_SKILL)
    action.SetParam(1154, 1)
    action.SetBranch(663, 625)
    g_AIDebugInfo[665] = '[665] CAST_SKILL(1154, 1)'

    -- 设置主定时器
    action = ai.NewAction(663, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(160)
    action.SetBranch(1589)
    g_AIDebugInfo[663] = '[663] SET_PRIMARY_TIMER(160)'

    -- 设置状态
    action = ai.NewAction(1589, AI_ACTION.SET_STATE)
    action.SetParam(673) -- 龙卷风结束
    g_AIDebugInfo[1589] = '[1589] SetParam(State673)'

    -- 龙卷风结束
    state = ai.NewState(673)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 60)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1409)

    -- 释放技能
    action = ai.NewAction(638, AI_ACTION.CAST_SKILL)
    action.SetParam(1153, 1)
    action.SetBranch(626, 625)
    g_AIDebugInfo[638] = '[638] CAST_SKILL(1153, 1)'

    -- 设置主定时器
    action = ai.NewAction(626, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1591)
    g_AIDebugInfo[626] = '[626] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1591, AI_ACTION.SET_STATE)
    action.SetParam(636) -- 闪现结束
    g_AIDebugInfo[1591] = '[1591] SetParam(State636)'

    -- 闪现结束
    state = ai.NewState(636)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 60)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1409)

    -- Npc标准技能选择
    action = ai.NewAction(646, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(617, 60)
    g_AIDebugInfo[646] = '[646] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(617, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(657, 628)
    g_AIDebugInfo[617] = '[617] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(628, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1593)
    g_AIDebugInfo[628] = '[628] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1593, AI_ACTION.SET_STATE)
    action.SetParam(648) -- 追击状态
    g_AIDebugInfo[1593] = '[1593] SetParam(State648)'

    -- 追击状态
    state = ai.NewState(648)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 630)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1409)

    -- 比较返回点距离
    action = ai.NewAction(630, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(10, 617, 617)
    g_AIDebugInfo[630] = '[630] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(657, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(60, 625)
    g_AIDebugInfo[657] = '[657] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(817, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(1595)
    g_AIDebugInfo[817] = '[817] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(1595, AI_ACTION.SET_STATE)
    action.SetParam(814) -- 返回中央
    g_AIDebugInfo[1595] = '[1595] SetParam(State814)'

    -- 返回中央
    state = ai.NewState(814)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 824)

    -- 进入巡逻路径
    action = ai.NewAction(824, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param['PatrolPathID'], param['PatrolOrderIndex'])
    action.SetBranch(812)
    g_AIDebugInfo[824] = '[824] ENTER_PATROL_PATH(param[PatrolPathID], param[PatrolOrderIndex])'

    -- 巡逻
    action = ai.NewAction(812, AI_ACTION.DO_PATROL)
    action.SetBranch(819, 358)
    g_AIDebugInfo[812] = '[812] DO_PATROL()'

    -- 设置主定时器
    action = ai.NewAction(819, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1597)
    g_AIDebugInfo[819] = '[819] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1597, AI_ACTION.SET_STATE)
    action.SetParam(816) -- 主状态
    g_AIDebugInfo[1597] = '[1597] SetParam(State816)'

    -- 主状态
    state = ai.NewState(816)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 819)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 823)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 812)

    -- 设置副定时器
    action = ai.NewAction(823, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(4)
    action.SetBranch(358)
    g_AIDebugInfo[823] = '[823] SET_SECONDARY_TIMER(4)'

    -- NPC喊话
    action = ai.NewAction(358, 2358)
    action.SetBranch(81)
    g_AIDebugInfo[358] = '[358] CustomAction358()'

    -- 设置主定时器
    action = ai.NewAction(81, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(1599)
    g_AIDebugInfo[81] = '[81] SET_PRIMARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(1599, AI_ACTION.SET_STATE)
    action.SetParam(188) -- 喊话结束
    g_AIDebugInfo[1599] = '[1599] SetParam(State188)'

    -- 喊话结束
    state = ai.NewState(188)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 130)

    -- 释放技能
    action = ai.NewAction(130, AI_ACTION.CAST_SKILL)
    action.SetParam(1156, 1)
    action.SetBranch(1347, 1347)
    g_AIDebugInfo[130] = '[130] CAST_SKILL(1156, 1)'

    -- CALL NPC
    action = ai.NewAction(1347, 3347)
    action.SetBranch(93)
    g_AIDebugInfo[1347] = '[1347] AI:CustomAction1347()'

    -- 设置主定时器
    action = ai.NewAction(93, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(1601)
    g_AIDebugInfo[93] = '[93] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(1601, AI_ACTION.SET_STATE)
    action.SetParam(57) -- 变身+召唤结束
    g_AIDebugInfo[1601] = '[1601] SetParam(State57)'

    -- 变身+召唤结束
    state = ai.NewState(57)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 690)

    -- 选择第一威胁
    action = ai.NewAction(690, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(725, 1415)
    g_AIDebugInfo[690] = '[690] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(725, AI_ACTION.CHECK_LIFE)
    action.SetParam(35)
    action.SetBranch(803, 803, 697)
    g_AIDebugInfo[725] = '[725] CHECK_LIFE(35)'

    -- 设置主定时器
    action = ai.NewAction(697, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1603)
    g_AIDebugInfo[697] = '[697] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1603, AI_ACTION.SET_STATE)
    action.SetParam(768) -- 普+闪+旋砍+陨石+龙卷风
    g_AIDebugInfo[1603] = '[1603] SetParam(State768)'

    -- 普+闪+旋砍+陨石+龙卷风
    state = ai.NewState(768)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 728)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1415)

    -- 三选项随机
    action = ai.NewAction(728, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(70, 10, 20)
    action.SetBranch(762, 711, 701)
    g_AIDebugInfo[728] = '[728] RANDOM_TRI_BRANCH(70, 10, 20)'

    -- NPC喊话
    action = ai.NewAction(711, 2711)
    action.SetBranch(753)
    g_AIDebugInfo[711] = '[711] CustomAction711()'

    -- 设置主定时器
    action = ai.NewAction(753, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(1605)
    g_AIDebugInfo[753] = '[753] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(1605, AI_ACTION.SET_STATE)
    action.SetParam(727) -- 喊话结束
    g_AIDebugInfo[1605] = '[1605] SetParam(State727)'

    -- 喊话结束
    state = ai.NewState(727)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 718)

    -- 设置主定时器
    action = ai.NewAction(718, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1607)
    g_AIDebugInfo[718] = '[718] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1607, AI_ACTION.SET_STATE)
    action.SetParam(772) -- 待一会儿
    g_AIDebugInfo[1607] = '[1607] SetParam(State772)'

    -- 待一会儿
    state = ai.NewState(772)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 703)

    -- 释放技能
    action = ai.NewAction(703, AI_ACTION.CAST_SKILL)
    action.SetParam(1152, 1)
    action.SetBranch(713, 752)
    g_AIDebugInfo[703] = '[703] CAST_SKILL(1152, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(752, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(725, 1415)
    g_AIDebugInfo[752] = '[752] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 站立
    action = ai.NewAction(1415, AI_ACTION.STAND)
    action.SetBranch(1414)
    g_AIDebugInfo[1415] = '[1415] STAND()'

    -- 设置主定时器
    action = ai.NewAction(1414, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1609)
    g_AIDebugInfo[1414] = '[1414] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1609, AI_ACTION.SET_STATE)
    action.SetParam(1412) -- 待机状态
    g_AIDebugInfo[1609] = '[1609] SetParam(State1412)'

    -- 待机状态
    state = ai.NewState(1412)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1260)

    -- 搜索敌人
    action = ai.NewAction(1260, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(1261, 1452)
    g_AIDebugInfo[1260] = '[1260] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(1261, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(690)
    g_AIDebugInfo[1261] = '[1261] ADD_TARGET_TO_THREAT_LIST()'

    -- 释放技能
    action = ai.NewAction(1452, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(1415, 1451)
    g_AIDebugInfo[1452] = '[1452] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(1451, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(1415, 1387)
    g_AIDebugInfo[1451] = '[1451] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置主定时器
    action = ai.NewAction(713, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1611)
    g_AIDebugInfo[713] = '[713] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1611, AI_ACTION.SET_STATE)
    action.SetParam(710) -- 旋风砍结束
    g_AIDebugInfo[1611] = '[1611] SetParam(State710)'

    -- 旋风砍结束
    state = ai.NewState(710)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 725)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1415)

    -- 三选项随机
    action = ai.NewAction(701, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(30, 40, 30)
    action.SetBranch(732, 698, 743)
    g_AIDebugInfo[701] = '[701] RANDOM_TRI_BRANCH(30, 40, 30)'

    -- 释放技能
    action = ai.NewAction(732, AI_ACTION.CAST_SKILL)
    action.SetParam(1153, 1)
    action.SetBranch(756, 752)
    g_AIDebugInfo[732] = '[732] CAST_SKILL(1153, 1)'

    -- 设置主定时器
    action = ai.NewAction(756, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1613)
    g_AIDebugInfo[756] = '[756] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1613, AI_ACTION.SET_STATE)
    action.SetParam(747) -- 闪现结束
    g_AIDebugInfo[1613] = '[1613] SetParam(State747)'

    -- 闪现结束
    state = ai.NewState(747)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 725)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1415)

    -- 设置主定时器
    action = ai.NewAction(698, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(1615)
    g_AIDebugInfo[698] = '[698] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(1615, AI_ACTION.SET_STATE)
    action.SetParam(693) -- 待一会儿
    g_AIDebugInfo[1615] = '[1615] SetParam(State693)'

    -- 待一会儿
    state = ai.NewState(693)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 759)

    -- 释放技能
    action = ai.NewAction(759, AI_ACTION.CAST_SKILL)
    action.SetParam(1157, 1)
    action.SetBranch(754, 752)
    g_AIDebugInfo[759] = '[759] CAST_SKILL(1157, 1)'

    -- 设置主定时器
    action = ai.NewAction(754, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1617)
    g_AIDebugInfo[754] = '[754] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1617, AI_ACTION.SET_STATE)
    action.SetParam(714) -- 陨石结束
    g_AIDebugInfo[1617] = '[1617] SetParam(State714)'

    -- 陨石结束
    state = ai.NewState(714)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 725)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1415)

    -- NPC喊话
    action = ai.NewAction(743, 2743)
    action.SetBranch(694)
    g_AIDebugInfo[743] = '[743] CustomAction743()'

    -- 设置主定时器
    action = ai.NewAction(694, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1619)
    g_AIDebugInfo[694] = '[694] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1619, AI_ACTION.SET_STATE)
    action.SetParam(692) -- 喊话结束
    g_AIDebugInfo[1619] = '[1619] SetParam(State692)'

    -- 喊话结束
    state = ai.NewState(692)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 707)

    -- 设置主定时器
    action = ai.NewAction(707, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1621)
    g_AIDebugInfo[707] = '[707] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1621, AI_ACTION.SET_STATE)
    action.SetParam(767) -- 待一会儿
    g_AIDebugInfo[1621] = '[1621] SetParam(State767)'

    -- 待一会儿
    state = ai.NewState(767)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 712)

    -- 释放技能
    action = ai.NewAction(712, AI_ACTION.CAST_SKILL)
    action.SetParam(1154, 1)
    action.SetBranch(723, 752)
    g_AIDebugInfo[712] = '[712] CAST_SKILL(1154, 1)'

    -- 设置主定时器
    action = ai.NewAction(723, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(160)
    action.SetBranch(1623)
    g_AIDebugInfo[723] = '[723] SET_PRIMARY_TIMER(160)'

    -- 设置状态
    action = ai.NewAction(1623, AI_ACTION.SET_STATE)
    action.SetParam(749) -- 龙卷风结束
    g_AIDebugInfo[1623] = '[1623] SetParam(State749)'

    -- 龙卷风结束
    state = ai.NewState(749)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 725)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1415)

    -- Npc标准技能选择
    action = ai.NewAction(762, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(731, 725)
    g_AIDebugInfo[762] = '[762] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(731, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(730, 773)
    g_AIDebugInfo[731] = '[731] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(773, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1625)
    g_AIDebugInfo[773] = '[773] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1625, AI_ACTION.SET_STATE)
    action.SetParam(737) -- 追击状态
    g_AIDebugInfo[1625] = '[1625] SetParam(State737)'

    -- 追击状态
    state = ai.NewState(737)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 735)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1415)

    -- 比较返回点距离
    action = ai.NewAction(735, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(690, 731, 731)
    g_AIDebugInfo[735] = '[735] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(730, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(725, 752)
    g_AIDebugInfo[730] = '[730] NPC_CAST_SELECT_SKILL()'

    -- NPC喊话
    action = ai.NewAction(803, 2803)
    action.SetBranch(805)
    g_AIDebugInfo[803] = '[803] CustomAction803()'

    -- 设置主定时器
    action = ai.NewAction(805, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(1627)
    g_AIDebugInfo[805] = '[805] SET_PRIMARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(1627, AI_ACTION.SET_STATE)
    action.SetParam(774) -- 喊话结束
    g_AIDebugInfo[1627] = '[1627] SetParam(State774)'

    -- 喊话结束
    state = ai.NewState(774)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 787)

    -- 释放技能
    action = ai.NewAction(787, AI_ACTION.CAST_SKILL)
    action.SetParam(1158, 1)
    action.SetBranch(1349, 1349)
    g_AIDebugInfo[787] = '[787] CAST_SKILL(1158, 1)'

    -- CALL NPC
    action = ai.NewAction(1349, 3349)
    action.SetBranch(797)
    g_AIDebugInfo[1349] = '[1349] AI:CustomAction1349()'

    -- 设置主定时器
    action = ai.NewAction(797, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(80)
    action.SetBranch(1629)
    g_AIDebugInfo[797] = '[797] SET_PRIMARY_TIMER(80)'

    -- 设置状态
    action = ai.NewAction(1629, AI_ACTION.SET_STATE)
    action.SetParam(796) -- 召唤四个龙卷风
    g_AIDebugInfo[1629] = '[1629] SetParam(State796)'

    -- 召唤四个龙卷风
    state = ai.NewState(796)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 808)

    -- 选择第一威胁
    action = ai.NewAction(808, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(868, 1420)
    g_AIDebugInfo[808] = '[808] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(868, AI_ACTION.CHECK_LIFE)
    action.SetParam(25)
    action.SetBranch(834, 834, 927)
    g_AIDebugInfo[868] = '[868] CHECK_LIFE(25)'

    -- 设置主定时器
    action = ai.NewAction(927, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1631)
    g_AIDebugInfo[927] = '[927] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1631, AI_ACTION.SET_STATE)
    action.SetParam(923) -- 普+闪+旋砍+陨石+龙卷风
    g_AIDebugInfo[1631] = '[1631] SetParam(State923)'

    -- 普+闪+旋砍+陨石+龙卷风
    state = ai.NewState(923)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 881)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1420)

    -- 三选项随机
    action = ai.NewAction(881, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(70, 10, 20)
    action.SetBranch(900, 880, 889)
    g_AIDebugInfo[881] = '[881] RANDOM_TRI_BRANCH(70, 10, 20)'

    -- Npc标准技能选择
    action = ai.NewAction(900, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(864, 868)
    g_AIDebugInfo[900] = '[900] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(864, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(837, 847)
    g_AIDebugInfo[864] = '[864] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(837, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(868, 904)
    g_AIDebugInfo[837] = '[837] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(904, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(868, 1420)
    g_AIDebugInfo[904] = '[904] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 站立
    action = ai.NewAction(1420, AI_ACTION.STAND)
    action.SetBranch(1419)
    g_AIDebugInfo[1420] = '[1420] STAND()'

    -- 设置主定时器
    action = ai.NewAction(1419, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1633)
    g_AIDebugInfo[1419] = '[1419] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1633, AI_ACTION.SET_STATE)
    action.SetParam(1417) -- 待机状态
    g_AIDebugInfo[1633] = '[1633] SetParam(State1417)'

    -- 待机状态
    state = ai.NewState(1417)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1295)

    -- 搜索敌人
    action = ai.NewAction(1295, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(1297, 1453)
    g_AIDebugInfo[1295] = '[1295] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(1297, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(808)
    g_AIDebugInfo[1297] = '[1297] ADD_TARGET_TO_THREAT_LIST()'

    -- 释放技能
    action = ai.NewAction(1453, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(1420, 1454)
    g_AIDebugInfo[1453] = '[1453] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(1454, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(1420, 1387)
    g_AIDebugInfo[1454] = '[1454] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置主定时器
    action = ai.NewAction(847, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1635)
    g_AIDebugInfo[847] = '[847] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1635, AI_ACTION.SET_STATE)
    action.SetParam(891) -- 追击状态
    g_AIDebugInfo[1635] = '[1635] SetParam(State891)'

    -- 追击状态
    state = ai.NewState(891)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 879)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1420)

    -- 比较返回点距离
    action = ai.NewAction(879, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(808, 864, 864)
    g_AIDebugInfo[879] = '[879] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- NPC喊话
    action = ai.NewAction(880, 2880)
    action.SetBranch(928)
    g_AIDebugInfo[880] = '[880] CustomAction880()'

    -- 设置主定时器
    action = ai.NewAction(928, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(1637)
    g_AIDebugInfo[928] = '[928] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(1637, AI_ACTION.SET_STATE)
    action.SetParam(885) -- 喊话结束
    g_AIDebugInfo[1637] = '[1637] SetParam(State885)'

    -- 喊话结束
    state = ai.NewState(885)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 883)

    -- 设置主定时器
    action = ai.NewAction(883, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1639)
    g_AIDebugInfo[883] = '[883] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1639, AI_ACTION.SET_STATE)
    action.SetParam(866) -- 待一会儿
    g_AIDebugInfo[1639] = '[1639] SetParam(State866)'

    -- 待一会儿
    state = ai.NewState(866)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 871)

    -- 释放技能
    action = ai.NewAction(871, AI_ACTION.CAST_SKILL)
    action.SetParam(1152, 1)
    action.SetBranch(839, 904)
    g_AIDebugInfo[871] = '[871] CAST_SKILL(1152, 1)'

    -- 设置主定时器
    action = ai.NewAction(839, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1641)
    g_AIDebugInfo[839] = '[839] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1641, AI_ACTION.SET_STATE)
    action.SetParam(865) -- 旋风砍结束
    g_AIDebugInfo[1641] = '[1641] SetParam(State865)'

    -- 旋风砍结束
    state = ai.NewState(865)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 868)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1420)

    -- 三选项随机
    action = ai.NewAction(889, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(30, 40, 30)
    action.SetBranch(841, 851, 917)
    g_AIDebugInfo[889] = '[889] RANDOM_TRI_BRANCH(30, 40, 30)'

    -- 设置主定时器
    action = ai.NewAction(851, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(1643)
    g_AIDebugInfo[851] = '[851] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(1643, AI_ACTION.SET_STATE)
    action.SetParam(913) -- 待一会儿
    g_AIDebugInfo[1643] = '[1643] SetParam(State913)'

    -- 待一会儿
    state = ai.NewState(913)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 907)

    -- 释放技能
    action = ai.NewAction(907, AI_ACTION.CAST_SKILL)
    action.SetParam(1157, 1)
    action.SetBranch(869, 904)
    g_AIDebugInfo[907] = '[907] CAST_SKILL(1157, 1)'

    -- 设置主定时器
    action = ai.NewAction(869, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1645)
    g_AIDebugInfo[869] = '[869] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1645, AI_ACTION.SET_STATE)
    action.SetParam(878) -- 陨石结束
    g_AIDebugInfo[1645] = '[1645] SetParam(State878)'

    -- 陨石结束
    state = ai.NewState(878)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 868)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1420)

    -- 释放技能
    action = ai.NewAction(841, AI_ACTION.CAST_SKILL)
    action.SetParam(1153, 1)
    action.SetBranch(886, 904)
    g_AIDebugInfo[841] = '[841] CAST_SKILL(1153, 1)'

    -- 设置主定时器
    action = ai.NewAction(886, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1647)
    g_AIDebugInfo[886] = '[886] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1647, AI_ACTION.SET_STATE)
    action.SetParam(894) -- 闪现结束
    g_AIDebugInfo[1647] = '[1647] SetParam(State894)'

    -- 闪现结束
    state = ai.NewState(894)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 868)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1420)

    -- NPC喊话
    action = ai.NewAction(917, 2917)
    action.SetBranch(933)
    g_AIDebugInfo[917] = '[917] CustomAction917()'

    -- 设置主定时器
    action = ai.NewAction(933, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1649)
    g_AIDebugInfo[933] = '[933] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1649, AI_ACTION.SET_STATE)
    action.SetParam(863) -- 喊话结束
    g_AIDebugInfo[1649] = '[1649] SetParam(State863)'

    -- 喊话结束
    state = ai.NewState(863)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 906)

    -- 设置主定时器
    action = ai.NewAction(906, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1651)
    g_AIDebugInfo[906] = '[906] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1651, AI_ACTION.SET_STATE)
    action.SetParam(830) -- 待一会儿
    g_AIDebugInfo[1651] = '[1651] SetParam(State830)'

    -- 待一会儿
    state = ai.NewState(830)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 858)

    -- 释放技能
    action = ai.NewAction(858, AI_ACTION.CAST_SKILL)
    action.SetParam(1154, 1)
    action.SetBranch(874, 904)
    g_AIDebugInfo[858] = '[858] CAST_SKILL(1154, 1)'

    -- 设置主定时器
    action = ai.NewAction(874, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(160)
    action.SetBranch(1653)
    g_AIDebugInfo[874] = '[874] SET_PRIMARY_TIMER(160)'

    -- 设置状态
    action = ai.NewAction(1653, AI_ACTION.SET_STATE)
    action.SetParam(924) -- 龙卷风结束
    g_AIDebugInfo[1653] = '[1653] SetParam(State924)'

    -- 龙卷风结束
    state = ai.NewState(924)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 868)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1420)

    -- NPC喊话
    action = ai.NewAction(834, 2834)
    action.SetBranch(856)
    g_AIDebugInfo[834] = '[834] CustomAction834()'

    -- 设置主定时器
    action = ai.NewAction(856, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(1655)
    g_AIDebugInfo[856] = '[856] SET_PRIMARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(1655, AI_ACTION.SET_STATE)
    action.SetParam(892) -- 喊话结束
    g_AIDebugInfo[1655] = '[1655] SetParam(State892)'

    -- 喊话结束
    state = ai.NewState(892)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 887)

    -- 选择第一威胁
    action = ai.NewAction(887, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(986, 1426)
    g_AIDebugInfo[887] = '[887] SELECT_FIRST_THREAT()'

    -- 检查生命
    action = ai.NewAction(986, AI_ACTION.CHECK_LIFE)
    action.SetParam(10)
    action.SetBranch(1014, 1014, 1019)
    g_AIDebugInfo[986] = '[986] CHECK_LIFE(10)'

    -- 设置主定时器
    action = ai.NewAction(1019, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1657)
    g_AIDebugInfo[1019] = '[1019] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1657, AI_ACTION.SET_STATE)
    action.SetParam(998) -- 普+闪+旋砍+陨石+龙卷风
    g_AIDebugInfo[1657] = '[1657] SetParam(State998)'

    -- 普+闪+旋砍+陨石+龙卷风
    state = ai.NewState(998)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 976)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1426)

    -- 三选项随机
    action = ai.NewAction(976, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(70, 10, 20)
    action.SetBranch(983, 1005, 1072)
    g_AIDebugInfo[976] = '[976] RANDOM_TRI_BRANCH(70, 10, 20)'

    -- 三选项随机
    action = ai.NewAction(1072, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(30, 40, 30)
    action.SetBranch(1016, 1044, 1079)
    g_AIDebugInfo[1072] = '[1072] RANDOM_TRI_BRANCH(30, 40, 30)'

    -- 释放技能
    action = ai.NewAction(1016, AI_ACTION.CAST_SKILL)
    action.SetParam(1153, 1)
    action.SetBranch(1028, 1023)
    g_AIDebugInfo[1016] = '[1016] CAST_SKILL(1153, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(1023, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(986, 1426)
    g_AIDebugInfo[1023] = '[1023] CHECK_SKILL_FAILED_COUNTER(10)'

    -- 站立
    action = ai.NewAction(1426, AI_ACTION.STAND)
    action.SetBranch(1425)
    g_AIDebugInfo[1426] = '[1426] STAND()'

    -- 设置主定时器
    action = ai.NewAction(1425, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1659)
    g_AIDebugInfo[1425] = '[1425] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1659, AI_ACTION.SET_STATE)
    action.SetParam(1423) -- 待机状态
    g_AIDebugInfo[1659] = '[1659] SetParam(State1423)'

    -- 待机状态
    state = ai.NewState(1423)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1306)

    -- 搜索敌人
    action = ai.NewAction(1306, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(1304, 1455)
    g_AIDebugInfo[1306] = '[1306] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(1304, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(887)
    g_AIDebugInfo[1304] = '[1304] ADD_TARGET_TO_THREAT_LIST()'

    -- 释放技能
    action = ai.NewAction(1455, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(1426, 1456)
    g_AIDebugInfo[1455] = '[1455] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(1456, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(1426, 1387)
    g_AIDebugInfo[1456] = '[1456] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置主定时器
    action = ai.NewAction(1028, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1661)
    g_AIDebugInfo[1028] = '[1028] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1661, AI_ACTION.SET_STATE)
    action.SetParam(1049) -- 闪现结束
    g_AIDebugInfo[1661] = '[1661] SetParam(State1049)'

    -- 闪现结束
    state = ai.NewState(1049)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 986)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1426)

    -- 设置主定时器
    action = ai.NewAction(1044, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(1663)
    g_AIDebugInfo[1044] = '[1044] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(1663, AI_ACTION.SET_STATE)
    action.SetParam(1081) -- 待一会儿
    g_AIDebugInfo[1663] = '[1663] SetParam(State1081)'

    -- 待一会儿
    state = ai.NewState(1081)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1071)

    -- 释放技能
    action = ai.NewAction(1071, AI_ACTION.CAST_SKILL)
    action.SetParam(1157, 1)
    action.SetBranch(1004, 1023)
    g_AIDebugInfo[1071] = '[1071] CAST_SKILL(1157, 1)'

    -- 设置主定时器
    action = ai.NewAction(1004, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1665)
    g_AIDebugInfo[1004] = '[1004] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1665, AI_ACTION.SET_STATE)
    action.SetParam(1015) -- 陨石结束
    g_AIDebugInfo[1665] = '[1665] SetParam(State1015)'

    -- 陨石结束
    state = ai.NewState(1015)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 986)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1426)

    -- NPC喊话
    action = ai.NewAction(1079, 3079)
    action.SetBranch(1082)
    g_AIDebugInfo[1079] = '[1079] CustomAction1079()'

    -- 设置主定时器
    action = ai.NewAction(1082, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1667)
    g_AIDebugInfo[1082] = '[1082] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1667, AI_ACTION.SET_STATE)
    action.SetParam(1039) -- 喊话结束
    g_AIDebugInfo[1667] = '[1667] SetParam(State1039)'

    -- 喊话结束
    state = ai.NewState(1039)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1038)

    -- 设置主定时器
    action = ai.NewAction(1038, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1669)
    g_AIDebugInfo[1038] = '[1038] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1669, AI_ACTION.SET_STATE)
    action.SetParam(1026) -- 待一会儿
    g_AIDebugInfo[1669] = '[1669] SetParam(State1026)'

    -- 待一会儿
    state = ai.NewState(1026)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1032)

    -- 释放技能
    action = ai.NewAction(1032, AI_ACTION.CAST_SKILL)
    action.SetParam(1154, 1)
    action.SetBranch(1074, 1023)
    g_AIDebugInfo[1032] = '[1032] CAST_SKILL(1154, 1)'

    -- 设置主定时器
    action = ai.NewAction(1074, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(160)
    action.SetBranch(1671)
    g_AIDebugInfo[1074] = '[1074] SET_PRIMARY_TIMER(160)'

    -- 设置状态
    action = ai.NewAction(1671, AI_ACTION.SET_STATE)
    action.SetParam(1031) -- 龙卷风结束
    g_AIDebugInfo[1671] = '[1671] SetParam(State1031)'

    -- 龙卷风结束
    state = ai.NewState(1031)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 986)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1426)

    -- NPC喊话
    action = ai.NewAction(1005, 3005)
    action.SetBranch(991)
    g_AIDebugInfo[1005] = '[1005] CustomAction1005()'

    -- 设置主定时器
    action = ai.NewAction(991, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(1673)
    g_AIDebugInfo[991] = '[991] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(1673, AI_ACTION.SET_STATE)
    action.SetParam(997) -- 喊话结束
    g_AIDebugInfo[1673] = '[1673] SetParam(State997)'

    -- 喊话结束
    state = ai.NewState(997)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 987)

    -- 设置主定时器
    action = ai.NewAction(987, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1675)
    g_AIDebugInfo[987] = '[987] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1675, AI_ACTION.SET_STATE)
    action.SetParam(993) -- 待一会儿
    g_AIDebugInfo[1675] = '[1675] SetParam(State993)'

    -- 待一会儿
    state = ai.NewState(993)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 985)

    -- 释放技能
    action = ai.NewAction(985, AI_ACTION.CAST_SKILL)
    action.SetParam(1152, 1)
    action.SetBranch(1033, 1023)
    g_AIDebugInfo[985] = '[985] CAST_SKILL(1152, 1)'

    -- 设置主定时器
    action = ai.NewAction(1033, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1677)
    g_AIDebugInfo[1033] = '[1033] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1677, AI_ACTION.SET_STATE)
    action.SetParam(980) -- 旋风砍结束
    g_AIDebugInfo[1677] = '[1677] SetParam(State980)'

    -- 旋风砍结束
    state = ai.NewState(980)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 986)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1426)

    -- Npc标准技能选择
    action = ai.NewAction(983, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(1056, 887)
    g_AIDebugInfo[983] = '[983] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(1056, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(1017, 1003)
    g_AIDebugInfo[1056] = '[1056] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(1017, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(986, 1023)
    g_AIDebugInfo[1017] = '[1017] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(1003, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1679)
    g_AIDebugInfo[1003] = '[1003] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1679, AI_ACTION.SET_STATE)
    action.SetParam(1075) -- 追击状态
    g_AIDebugInfo[1679] = '[1679] SetParam(State1075)'

    -- 追击状态
    state = ai.NewState(1075)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1029)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1426)

    -- 比较返回点距离
    action = ai.NewAction(1029, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(887, 1056, 1056)
    g_AIDebugInfo[1029] = '[1029] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- NPC喊话
    action = ai.NewAction(1014, 3014)
    action.SetBranch(999)
    g_AIDebugInfo[1014] = '[1014] CustomAction1014()'

    -- 设置主定时器
    action = ai.NewAction(999, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(48)
    action.SetBranch(1681)
    g_AIDebugInfo[999] = '[999] SET_PRIMARY_TIMER(48)'

    -- 设置状态
    action = ai.NewAction(1681, AI_ACTION.SET_STATE)
    action.SetParam(1063) -- 喊话结束
    g_AIDebugInfo[1681] = '[1681] SetParam(State1063)'

    -- 喊话结束
    state = ai.NewState(1063)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1041)

    -- 释放技能
    action = ai.NewAction(1041, AI_ACTION.CAST_SKILL)
    action.SetParam(1158, 1)
    action.SetBranch(1351, 1351)
    g_AIDebugInfo[1041] = '[1041] CAST_SKILL(1158, 1)'

    -- CALL NPC
    action = ai.NewAction(1351, 3351)
    action.SetBranch(1054)
    g_AIDebugInfo[1351] = '[1351] AI:CustomAction1351()'

    -- 设置主定时器
    action = ai.NewAction(1054, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(80)
    action.SetBranch(1683)
    g_AIDebugInfo[1054] = '[1054] SET_PRIMARY_TIMER(80)'

    -- 设置状态
    action = ai.NewAction(1683, AI_ACTION.SET_STATE)
    action.SetParam(1007) -- 召唤四个龙卷风
    g_AIDebugInfo[1683] = '[1683] SetParam(State1007)'

    -- 召唤四个龙卷风
    state = ai.NewState(1007)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 989)

    -- 选择第一威胁
    action = ai.NewAction(989, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(1107, 1432)
    g_AIDebugInfo[989] = '[989] SELECT_FIRST_THREAT()'

    -- 站立
    action = ai.NewAction(1432, AI_ACTION.STAND)
    action.SetBranch(1429)
    g_AIDebugInfo[1432] = '[1432] STAND()'

    -- 设置主定时器
    action = ai.NewAction(1429, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1685)
    g_AIDebugInfo[1429] = '[1429] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1685, AI_ACTION.SET_STATE)
    action.SetParam(1433) -- 待机状态
    g_AIDebugInfo[1685] = '[1685] SetParam(State1433)'

    -- 待机状态
    state = ai.NewState(1433)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1312)

    -- 搜索敌人
    action = ai.NewAction(1312, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(1307, 1458)
    g_AIDebugInfo[1312] = '[1312] SEARCH_ENEMY(param[AlertRange])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(1307, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(989)
    g_AIDebugInfo[1307] = '[1307] ADD_TARGET_TO_THREAT_LIST()'

    -- 释放技能
    action = ai.NewAction(1458, AI_ACTION.CAST_SKILL)
    action.SetParam(311, 1)
    action.SetBranch(1432, 1457)
    g_AIDebugInfo[1458] = '[1458] CAST_SKILL(311, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(1457, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(1432, 1387)
    g_AIDebugInfo[1457] = '[1457] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置主定时器
    action = ai.NewAction(1107, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1687)
    g_AIDebugInfo[1107] = '[1107] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1687, AI_ACTION.SET_STATE)
    action.SetParam(1085) -- 普+闪+旋砍+陨石+龙卷风
    g_AIDebugInfo[1687] = '[1687] SetParam(State1085)'

    -- 普+闪+旋砍+陨石+龙卷风
    state = ai.NewState(1085)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1091)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1432)

    -- 三选项随机
    action = ai.NewAction(1091, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(70, 10, 20)
    action.SetBranch(1120, 1155, 1148)
    g_AIDebugInfo[1091] = '[1091] RANDOM_TRI_BRANCH(70, 10, 20)'

    -- Npc标准技能选择
    action = ai.NewAction(1120, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(1139, 1107)
    g_AIDebugInfo[1120] = '[1120] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(1139, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(1145, 1130)
    g_AIDebugInfo[1139] = '[1139] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(1130, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1689)
    g_AIDebugInfo[1130] = '[1130] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1689, AI_ACTION.SET_STATE)
    action.SetParam(1106) -- 追击状态
    g_AIDebugInfo[1689] = '[1689] SetParam(State1106)'

    -- 追击状态
    state = ai.NewState(1106)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1088)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1432)

    -- 比较返回点距离
    action = ai.NewAction(1088, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(989, 1139, 1139)
    g_AIDebugInfo[1088] = '[1088] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(1145, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(989, 1127)
    g_AIDebugInfo[1145] = '[1145] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(1127, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(10)
    action.SetBranch(1107, 1432)
    g_AIDebugInfo[1127] = '[1127] CHECK_SKILL_FAILED_COUNTER(10)'

    -- NPC喊话
    action = ai.NewAction(1155, 3155)
    action.SetBranch(1138)
    g_AIDebugInfo[1155] = '[1155] CustomAction1155()'

    -- 设置主定时器
    action = ai.NewAction(1138, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(1691)
    g_AIDebugInfo[1138] = '[1138] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(1691, AI_ACTION.SET_STATE)
    action.SetParam(1135) -- 喊话结束
    g_AIDebugInfo[1691] = '[1691] SetParam(State1135)'

    -- 喊话结束
    state = ai.NewState(1135)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1156)

    -- 设置主定时器
    action = ai.NewAction(1156, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1693)
    g_AIDebugInfo[1156] = '[1156] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1693, AI_ACTION.SET_STATE)
    action.SetParam(1143) -- 待一会儿
    g_AIDebugInfo[1693] = '[1693] SetParam(State1143)'

    -- 待一会儿
    state = ai.NewState(1143)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1104)

    -- 释放技能
    action = ai.NewAction(1104, AI_ACTION.CAST_SKILL)
    action.SetParam(1152, 1)
    action.SetBranch(1161, 1127)
    g_AIDebugInfo[1104] = '[1104] CAST_SKILL(1152, 1)'

    -- 设置主定时器
    action = ai.NewAction(1161, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1695)
    g_AIDebugInfo[1161] = '[1161] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1695, AI_ACTION.SET_STATE)
    action.SetParam(1129) -- 旋风砍结束
    g_AIDebugInfo[1695] = '[1695] SetParam(State1129)'

    -- 旋风砍结束
    state = ai.NewState(1129)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1107)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1432)

    -- 三选项随机
    action = ai.NewAction(1148, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(30, 40, 30)
    action.SetBranch(1128, 1118, 1163)
    g_AIDebugInfo[1148] = '[1148] RANDOM_TRI_BRANCH(30, 40, 30)'

    -- 设置主定时器
    action = ai.NewAction(1118, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(24)
    action.SetBranch(1697)
    g_AIDebugInfo[1118] = '[1118] SET_PRIMARY_TIMER(24)'

    -- 设置状态
    action = ai.NewAction(1697, AI_ACTION.SET_STATE)
    action.SetParam(1113) -- 待一会儿
    g_AIDebugInfo[1697] = '[1697] SetParam(State1113)'

    -- 待一会儿
    state = ai.NewState(1113)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1114)

    -- 释放技能
    action = ai.NewAction(1114, AI_ACTION.CAST_SKILL)
    action.SetParam(1157, 1)
    action.SetBranch(1109, 1127)
    g_AIDebugInfo[1114] = '[1114] CAST_SKILL(1157, 1)'

    -- 设置主定时器
    action = ai.NewAction(1109, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1699)
    g_AIDebugInfo[1109] = '[1109] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1699, AI_ACTION.SET_STATE)
    action.SetParam(1131) -- 陨石结束
    g_AIDebugInfo[1699] = '[1699] SetParam(State1131)'

    -- 陨石结束
    state = ai.NewState(1131)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1107)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1432)

    -- NPC喊话
    action = ai.NewAction(1163, 3163)
    action.SetBranch(1144)
    g_AIDebugInfo[1163] = '[1163] CustomAction1163()'

    -- 设置主定时器
    action = ai.NewAction(1144, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1701)
    g_AIDebugInfo[1144] = '[1144] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1701, AI_ACTION.SET_STATE)
    action.SetParam(1159) -- 喊话结束
    g_AIDebugInfo[1701] = '[1701] SetParam(State1159)'

    -- 喊话结束
    state = ai.NewState(1159)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1117)

    -- 设置主定时器
    action = ai.NewAction(1117, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1703)
    g_AIDebugInfo[1117] = '[1117] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1703, AI_ACTION.SET_STATE)
    action.SetParam(1154) -- 待一会儿
    g_AIDebugInfo[1703] = '[1703] SetParam(State1154)'

    -- 待一会儿
    state = ai.NewState(1154)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1150)

    -- 释放技能
    action = ai.NewAction(1150, AI_ACTION.CAST_SKILL)
    action.SetParam(1154, 1)
    action.SetBranch(1165, 1127)
    g_AIDebugInfo[1150] = '[1150] CAST_SKILL(1154, 1)'

    -- 设置主定时器
    action = ai.NewAction(1165, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(160)
    action.SetBranch(1705)
    g_AIDebugInfo[1165] = '[1165] SET_PRIMARY_TIMER(160)'

    -- 设置状态
    action = ai.NewAction(1705, AI_ACTION.SET_STATE)
    action.SetParam(1119) -- 龙卷风结束
    g_AIDebugInfo[1705] = '[1705] SetParam(State1119)'

    -- 龙卷风结束
    state = ai.NewState(1119)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1107)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1432)

    -- 释放技能
    action = ai.NewAction(1128, AI_ACTION.CAST_SKILL)
    action.SetParam(1153, 1)
    action.SetBranch(1141, 1127)
    g_AIDebugInfo[1128] = '[1128] CAST_SKILL(1153, 1)'

    -- 设置主定时器
    action = ai.NewAction(1141, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1707)
    g_AIDebugInfo[1141] = '[1141] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1707, AI_ACTION.SET_STATE)
    action.SetParam(1151) -- 闪现结束
    g_AIDebugInfo[1707] = '[1707] SetParam(State1151)'

    -- 闪现结束
    state = ai.NewState(1151)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 1107)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 1432)

    -- 设置主定时器
    action = ai.NewAction(230, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1709)
    g_AIDebugInfo[230] = '[230] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1709, AI_ACTION.SET_STATE)
    action.SetParam(27) -- 普+闪+旋砍
    g_AIDebugInfo[1709] = '[1709] SetParam(State27)'

    -- 普+闪+旋砍
    state = ai.NewState(27)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 399)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 272)

    -- 三选项随机
    action = ai.NewAction(399, AI_ACTION.RANDOM_TRI_BRANCH)
    action.SetParam(90, 5, 5)
    action.SetBranch(318, 417, 404)
    g_AIDebugInfo[399] = '[399] RANDOM_TRI_BRANCH(90, 5, 5)'

    -- Npc标准技能选择
    action = ai.NewAction(318, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(263, 243)
    g_AIDebugInfo[318] = '[318] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(263, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(12, 292)
    g_AIDebugInfo[263] = '[263] NPC_KEEP_SKILL_CAST_RANGE()'

    -- 设置主定时器
    action = ai.NewAction(292, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1711)
    g_AIDebugInfo[292] = '[292] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1711, AI_ACTION.SET_STATE)
    action.SetParam(9) -- 追击状态
    g_AIDebugInfo[1711] = '[1711] SetParam(State9)'

    -- 追击状态
    state = ai.NewState(9)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 73)
    state.HandleEvent(AI_EVENT.ON_TARGET_KILLED, 272)

    -- 比较返回点距离
    action = ai.NewAction(73, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['EscapeRange'])
    action.SetBranch(272, 263, 263)
    g_AIDebugInfo[73] = '[73] RETURN_POSITION_DISTANCE_COMPARE(param[EscapeRange])'

    -- Npc释放选定技能
    action = ai.NewAction(12, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(162, 13)
    g_AIDebugInfo[12] = '[12] NPC_CAST_SELECT_SKILL()'

    -- 检查技能失败计数
    action = ai.NewAction(13, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(50)
    action.SetBranch(162, 272)
    g_AIDebugInfo[13] = '[13] CHECK_SKILL_FAILED_COUNTER(50)'

    -- NPC喊话
    action = ai.NewAction(417, 2417)
    action.SetBranch(415)
    g_AIDebugInfo[417] = '[417] CustomAction417()'

    -- 设置主定时器
    action = ai.NewAction(415, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(1713)
    g_AIDebugInfo[415] = '[415] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(1713, AI_ACTION.SET_STATE)
    action.SetParam(416) -- 喊话结束
    g_AIDebugInfo[1713] = '[1713] SetParam(State416)'

    -- 喊话结束
    state = ai.NewState(416)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 396)

    -- 设置主定时器
    action = ai.NewAction(396, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(1715)
    g_AIDebugInfo[396] = '[396] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(1715, AI_ACTION.SET_STATE)
    action.SetParam(395) -- 待一会儿
    g_AIDebugInfo[1715] = '[1715] SetParam(State395)'

    -- 待一会儿
    state = ai.NewState(395)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 387)

    -- 释放技能
    action = ai.NewAction(387, AI_ACTION.CAST_SKILL)
    action.SetParam(1152, 1)
    action.SetBranch(388, 13)
    g_AIDebugInfo[387] = '[387] CAST_SKILL(1152, 1)'

    -- 设置主定时器
    action = ai.NewAction(388, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(1717)
    g_AIDebugInfo[388] = '[388] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(1717, AI_ACTION.SET_STATE)
    action.SetParam(386) -- 旋风砍结束
    g_AIDebugInfo[1717] = '[1717] SetParam(State386)'

    -- 旋风砍结束
    state = ai.NewState(386)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 162)

    -- 释放技能
    action = ai.NewAction(404, AI_ACTION.CAST_SKILL)
    action.SetParam(1153, 1)
    action.SetBranch(407, 13)
    g_AIDebugInfo[404] = '[404] CAST_SKILL(1153, 1)'

    -- 设置主定时器
    action = ai.NewAction(407, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(1719)
    g_AIDebugInfo[407] = '[407] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(1719, AI_ACTION.SET_STATE)
    action.SetParam(408) -- 闪现
    g_AIDebugInfo[1719] = '[1719] SetParam(State408)'

    -- 闪现
    state = ai.NewState(408)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 162)

    -- 搜索敌人
    action = ai.NewAction(169, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['AlertRange'])
    action.SetBranch(49, 272)
    g_AIDebugInfo[169] = '[169] SEARCH_ENEMY(param[AlertRange])'

    ai.SetInitState(145)
end

-- 调试方法
function AI:DebugOnCallAction(nActiondId, nAIType)
    print('[AI] Call action: ' .. AI[nAIType][nActiondId])
end

-- 本AI图内自定义脚本
function AI:CustomAction166(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1206[Lang.Idx])
    return 1
end


function AI:CustomAction445(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1207[Lang.Idx])
	return 1
end

function AI:CustomAction219(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1208[Lang.Idx])
    return 1
end

function AI:CustomAction1345(pNpc, actionData)
    -- 输入自定义动作实现脚本
     local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(1205,nSceneId, 2786,3365,1048576);
	KGameBase.CallNpc(1205,nSceneId, 2332,3816,1048576);
	KGameBase.CallNpc(1205,nSceneId, 2721,4224,1048576);
	KGameBase.CallNpc(1205,nSceneId, 3235,3809,1048576);
    return 1
end

function AI:CustomAction548(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1207[Lang.Idx])
    return 1
end


function AI:CustomAction295(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1209[Lang.Idx])
    return 1
end


function AI:CustomAction641(pNpc, ai)
    -- 输入自定义动作实现脚本
   	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1207[Lang.Idx])
    return 1
end

function AI:CustomAction672(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1210[Lang.Idx])
    return 1
end


function AI:CustomAction358(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1211[Lang.Idx])
    return 1
end

function AI:CustomAction1347(pNpc, actionData)
    -- 输入自定义动作实现脚本
     local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(1205,nSceneId, 2786,3365,1048576);
	KGameBase.CallNpc(1205,nSceneId, 2332,3816,1048576);
	KGameBase.CallNpc(1205,nSceneId, 2721,4224,1048576);
	KGameBase.CallNpc(1205,nSceneId, 3235,3809,1048576);
    return 1
end


function AI:CustomAction711(pNpc, ai)
    -- 输入自定义动作实现脚本
   	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1207[Lang.Idx])
	return 1
end

function AI:CustomAction743(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1212[Lang.Idx])
    return 1
end

function AI:CustomAction803(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1213[Lang.Idx])
    return 1
end

function AI:CustomAction1349(pNpc, actionData)
    -- 输入自定义动作实现脚本
     local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(1206,nSceneId, 1915,3754,1048576);
	KGameBase.CallNpc(1206,nSceneId, 2814,2870,1048576);
	KGameBase.CallNpc(1206,nSceneId, 3736,3824,1048576);
    return 1
end

function AI:CustomAction880(pNpc, ai)
    -- 输入自定义动作实现脚本
   	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1207[Lang.Idx])
    return 1
end

function AI:CustomAction917(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1210[Lang.Idx])
    return 1
end

function AI:CustomAction834(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1214[Lang.Idx])
    return 1
end

function AI:CustomAction1079(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1210[Lang.Idx])
    return 1
end

function AI:CustomAction1005(pNpc, ai)
    -- 输入自定义动作实现脚本
   	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1207[Lang.Idx])
    return 1
end

function AI:CustomAction1014(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1215[Lang.Idx])
    return 1
end

function AI:CustomAction1351(pNpc, actionData)
    -- 输入自定义动作实现脚本
     local nSceneId = pNpc.dwSceneId;
	KGameBase.CallNpc(1206,nSceneId, 1915,3754,1048576);
	KGameBase.CallNpc(1206,nSceneId, 2814,2870,1048576);
	KGameBase.CallNpc(1206,nSceneId, 3736,3824,1048576);
    return 1
end

function AI:CustomAction1155(pNpc, ai)
    -- 输入自定义动作实现脚本
   	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1207[Lang.Idx])
    return 1
end

function AI:CustomAction1163(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1210[Lang.Idx])
    return 1
end

function AI:CustomAction417(pNpc, ai)
    -- 输入自定义动作实现脚本
	local param = AI.g_AIParam[ai.nAIType]
    Dialog:NpcBubbleToNearBy(pNpc.dwId, Lang.mission.str1207[Lang.Idx])
    return 1
end



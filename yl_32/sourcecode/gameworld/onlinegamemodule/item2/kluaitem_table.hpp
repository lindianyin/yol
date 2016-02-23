/* -------------------------------------------------------------------------
//	�ļ���		��	kluaitem_table.hpp
//	������		��	LuaScriptTool
//	��������	��	���ļ���VisualStudio�����ɣ��벻Ҫ�ֶ��޸ģ���
//	ʹ�÷���	��	����Ӧ��CPP�ļ�ĩ�ˣ�include���ļ�
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(KLuaItem)
	REGISTER_LUA_STRING_READONLY(Name, "�������ƣ�������׺����")
	REGISTER_LUA_STRING_READONLY(OrgName, "����ԭ����������׺����")
	REGISTER_LUA_STRING_READONLY(Suffix, "���ߺ�׺��")
	REGISTER_LUA_INTEGER_READONLY(Genre, "�������� (����? ҩƷ? ��ʯ?)")
	REGISTER_LUA_INTEGER_READONLY(Detail, "������ϸ���")
	REGISTER_LUA_INTEGER_READONLY(Particular, "���߾������")
	REGISTER_LUA_INTEGER_READONLY(Level, "���ߵȼ�")
	REGISTER_LUA_INTEGER_READONLY(ReqLevel, "")
	REGISTER_LUA_INTEGER_READONLY(Quality, "����Ʒ��")
	REGISTER_LUA_INTEGER_READONLY(FightPower, "ս����")
	REGISTER_LUA_DWORD_READONLY(RandSeed, "�����������")
	REGISTER_LUA_DWORD_READONLY(RandSeedEx, "�����������")
	REGISTER_LUA_INTEGER_READONLY(Count, "���ߵ�ǰ������Ŀ")
	REGISTER_LUA_INTEGER_READONLY(MaxCount, "������������Ŀ")
	REGISTER_LUA_INTEGER_READONLY(Price, "���߼۸�")
	REGISTER_LUA_INTEGER_READONLY(BindType, "���߰�����")
	REGISTER_LUA_INTEGER(EnhTimes, "������ǿ������")
	REGISTER_LUA_INTEGER_READONLY(SlotSize, "װ���ı�ʯ������")
	REGISTER_LUA_INTEGER_READONLY(Strengthen, "���߸���ȼ�")
	REGISTER_LUA_INTEGER_READONLY(EquipPos, "װ��Ӧ������װ����λ��")
	REGISTER_LUA_INTEGER_READONLY(Value, "���ߵļ�ֵ��")
	REGISTER_LUA_INTEGER_READONLY(StarLevel, "���ߵļ�ֵ���Ǽ�")
	REGISTER_LUA_INTEGER_READONLY(NameColor, "���ߵ�������ɫ")
	REGISTER_LUA_INTEGER_READONLY(CdType, "���ߵ�CDʱ������")
	REGISTER_LUA_INTEGER_READONLY(CustomType, "���ߵ��Զ����ַ�������")
	REGISTER_LUA_INTEGER_READONLY(EquipCategory, "װ�����")
	REGISTER_LUA_DWORD_READONLY(Id, "����ID")
	REGISTER_LUA_STRING_READONLY(Class, "Class����")
	REGISTER_LUA_STRING_READONLY(CustomString, "�����Զ����ַ���")
	REGISTER_LUA_STRING_READONLY(ForbidType, "��������")
	REGISTER_LUA_FUNC(GetTempTable, "ȡ�ýű��õ���ʱTable", "", "", "�����󶨵���ʱTable")
	REGISTER_LUA_FUNC(IsEquip, "�жϸõ����Ƿ�һ��װ��", "", "", "1-��װ��")
	REGISTER_LUA_FUNC(IsStone, "�жϸõ����Ƿ�ʯ", "", "", "1-�Ǳ�ʯ")
	REGISTER_LUA_FUNC(IsInvalid, "�жϸõ����Ƿ���ʧЧ״̬", "", "", "1-ʧЧ")
	REGISTER_LUA_FUNC(IsDuped, "�ж��Ƿ��Ƶ���", "", "", "1-�Ǹ��Ƶ���")
	REGISTER_LUA_FUNC(IsTemp, "�ж��Ƿ���ʱ����", "", "", "1-����ʱ����")
	REGISTER_LUA_FUNC(GetLockIntervale, "ȡ��������ڣ�Ϊ0��ʾ�����Զ����", "", "", "dwTime������")
	REGISTER_LUA_FUNC(IsLock, "�ж��Ƿ�������", "", "", "1-������")
	REGISTER_LUA_FUNC(IsBind, "�ж��Ƿ��Ѱ�", "", "", "1-�Ѱ�")
	REGISTER_LUA_FUNC(CanBeShortcut, "�Ƿ���ԷŽ������", "", "", "1����")
	REGISTER_LUA_FUNC(GetBaseAttrib, "��ȡ��Ʒ�Ļ�������", "", "", "ħ�����Ա�")
	REGISTER_LUA_FUNC(GetReqAttrib, "��ȡ��Ʒ����������", "", "", "ħ�����Ա�")
	REGISTER_LUA_FUNC(GetExtParam, "��ȡ��Ʒ����չ������", "[d]", "nIndex	���ص�nIndex����չ������0��ʾ����������չ������;", "ָ����չ��������չ������")
	REGISTER_LUA_FUNC(IsStackable, "�Ƿ�ɵ�����Ʒ", "", "", "1�ɵ�����Ʒ")
	REGISTER_LUA_FUNC(IsDestoriable, "�ɷ�����", "", "", "1������")
	REGISTER_LUA_FUNC(IsForbitThrow, "�ɷ���", "", "", "1���ɶ���")
	REGISTER_LUA_FUNC(IsForbitSell, "�ɷ�����", "", "", "1��������")
	REGISTER_LUA_FUNC(GetOwner, "��õ��������Ľ�ɫ����", "", "", "�������κν�ɫ�򷵻�nil")
	REGISTER_LUA_FUNC(Remove, "�����߶������Ϸ����ɾ�������ߴ�ʱӦ�ò������κ�һ����ɫ��", "", "", "�ɹ�����1")
	REGISTER_LUA_FUNC(SetTimeOut, "���ù���ʱ��", "dd", "�������ͣ�0����ʱ�䣨��������1���ʱ�䣨Ҳ��������;ʱ��;", "")
	REGISTER_LUA_FUNC(SetCount, "������Ʒ�ĸ���", "d[d]", "���õĸ���;�����������;��;", "1�ɹ�")
	REGISTER_LUA_FUNC(SetGenerateMoneyType, "���õ��߲���ʱѡ��Ľ�Ǯ����", "d", "���ý�Ǯ����;", "")
	REGISTER_LUA_FUNC(GetSellMoneyType, "��������ɻ�õ���������", "", "", "0:������; 1:��ͨ����")
	REGISTER_LUA_FUNC(HasStone, "����װ����λ�����Ƿ�����Ƕ��ʯ", "d", "��ʯ��λ��1-3;", "1:�� 0:û��")
	REGISTER_LUA_FUNC(GetEquipScore, "", "", "", "װ������")
	REGISTER_LUA_FUNC(GetResourceId, "", "", "", "")
	REGISTER_LUA_FUNC(GetChangeColorScheme, "", "", "", "")
	REGISTER_LUA_FUNC(GetTimeOut, "���װ����ʱʱ��", "", "", "0:����ʱ�䣨������; 1:���ʱ�䣨Ҳ��������")
	REGISTER_LUA_FUNC(Bind, "�󶨵���", "d", "bForce:ǿ��;", "1�ɹ���")
	REGISTER_LUA_FUNC(GetGDPLStr, "�õ���Ʒ��gdpl�ַ���", "", "", "1 �ɹ�")
	REGISTER_LUA_FUNC(StoneClean, "��ʯժȡ���������Ѻ���ı�ʯ�ᵽǰ�棩", "", "", "1 �ɹ�")
	REGISTER_LUA_FUNC(StonePunch, "��ʯ���", "d", "��ʯ��λ��;", "1 �ɹ�")
	REGISTER_LUA_FUNC(GetStoneInfo, "�õ�һ��װ���ı�ʯ��Ϣ", "", "", "��ʯ��Ϣ { {nStoneId = ?, nStoneLevel = ?}, 0, -1 } // 0:δ��ʯ�Ŀף�-1:û�п�")
	REGISTER_LUA_FUNC(GetAppendAttr, "�õ�һ��װ���ĸ���������Ϣ", "", "", "����������Ϣ { {nAttr = ?, nValue = ?}, ...}")
	REGISTER_LUA_FUNC(GetUpgradeItems, "�õ�װ���������������Ϣ", "", "", "")
	REGISTER_LUA_FUNC(GetSaveAppendAttr, "�õ�һ��װ����ϴ����ĸ���������Ϣ", "", "", "����������Ϣ { {nAttr = ?, nValue = ?}, ...}")
	REGISTER_LUA_FUNC(GetBagPosLimit, "��ñ���λ������", "", "", "1�ɹ���")
	REGISTER_LUA_FUNC(GetNextReqLevel, "�����һ�ȼ�װ��������ȼ�", "", "", "")
	REGISTER_LUA_FUNC(GetStoneType, "��ñ�ʯ����", "", "", "")

#ifdef GAME_SERVER
	REGISTER_LUA_INTEGER(BuyPrice, "Ib��ֵ")
	REGISTER_LUA_STRING_READONLY(GUID, "GUID���ַ���")
	REGISTER_LUA_DWORD_READONLY(GenTime, "��������ʱ��")
	REGISTER_LUA_FUNC(IsIbItem, "�Ƿ�IB��Ʒ", "", "", "1:��,0:��")
	REGISTER_LUA_FUNC(Regenerate, "��������һ������", "ddddd???uu", "nGenre		;nDetailType	;nParticularType;nLevel:���ߵȼ�;nEnhTimes:������ǿ������;����table���ͣ���ΪStoneInfo��Table;����table���ͣ���Ϊ����������Ϣ;����table���ͣ���Ϊϴ��ʱ��ס������( {0, 0, 1, 1, 0, 0} ��ʾ��3��4������������ס�� );uRandSeed;uRandSeedEx;", "�ɹ�����1")
	REGISTER_LUA_FUNC(ClearSaveAppendAttr, "����ѱ����ϴ����������", "d", "nSaveOri;", "1�ɹ�")
	REGISTER_LUA_FUNC(Sync, "ͬ���������ݵ��ͻ���", "", "", "1�ɹ�")
	REGISTER_LUA_FUNC(ChangeColorScheme, "����װ����ɫ����", "d", "nScheme:�µĻ�ɫ����;", "1�ɹ�")
	REGISTER_LUA_FUNC(SetCustom, "���õ����Զ����ַ���", "ds", "nCustomType:���ͣ���;szCustom:�Զ����ַ���;", "1�ɹ�")
	REGISTER_LUA_FUNC(SetExtParam, "���õ���ExtParam", "dd", "λ��;����;", "1�ɹ�")
	REGISTER_LUA_FUNC(ApplyMagicAttrib, "Ӧ��ħ�����ԣ�Ӧ���Ƶ�����ħ�������", "s{dd}[d]", "szAttrib:ħ��������;nValue1:ħ�����Բ���1;nValue2:ħ�����Բ���2;bSync:�Ƿ�ͬ���ͻ���;", "")
	REGISTER_LUA_FUNC(RemoveMagicAttrib, "�Ƴ�NPC��ħ������", "s{dd}", "szAttrib:ħ��������;nValue1:ħ�����Բ���1;nValue2:ħ�����Բ���2;", "��")
#endif // #ifdef GAME_SERVER

#ifdef GAME_CLIENT
	REGISTER_LUA_STRING_READONLY(Intro, "���ߵ�������Ϣ")
	REGISTER_LUA_STRING_READONLY(Help, "���ߵ�������Ϣ")
	REGISTER_LUA_INTEGER_READONLY(Index, "���ߵ�ȫ��������")
	REGISTER_LUA_STRING_READONLY(IconImage, "")
	REGISTER_LUA_STRING_READONLY(ViewImage, "")
	REGISTER_LUA_STRING_READONLY(TransparencyIcon, "����͸��ͼ��ͼ��")
	REGISTER_LUA_STRING_READONLY(MaskLayerIcon, "������ͼ��ͼ��")
#endif // #ifdef GAME_CLIENT
DEFINE_LUA_CLASS_END()
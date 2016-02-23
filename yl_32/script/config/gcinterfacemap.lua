if not (MODULE_GAMESERVER or MODULE_GC_SERVER) then
	return
end

-- svn://10.20.86.6/s1base/branches/ninesky_dev_20100420/sourcecode/include/serverbase/kinterfacedef.h ÀïÃæµÄKE_INTERFACE_TYPE_ID
local nId = 0;
local function IdAccumulator(nSegment)
	nId = nSegment or nId + 1;
	return nId;
end

GCSetting.tbInterfaceTypeId = 
{
	emKINTERFACE_TID_NONE	 		= IdAccumulator(0),
	emKINTERFACE_TID_TEAM_POST		= IdAccumulator(),
	emKINTERFACE_TID_KIN_POST		= IdAccumulator(),
	emKINTERFACE_TID_GROUP_POST		= IdAccumulator(),
}


function GCSetting:GetInterfaceTypeId(szInterfaceType)
	print("szInterfaceType", szInterfaceType);
	return GCSetting.tbInterfaceTypeId[szInterfaceType];
end

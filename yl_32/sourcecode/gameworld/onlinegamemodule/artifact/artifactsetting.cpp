#include "stdafx.h"
#include "artifactsetting.h"
#include "artifactdef.h"
#include "Runtime/qstring.h"
#include "trade/kbuysellsetting.h"

ArtifactSetting g_cArtifactSetting;
ArtifactSetting::ArtifactSetting()
{

}

ArtifactSetting::~ArtifactSetting()
{

}

BOOL ArtifactSetting::Init()
{
	return TRUE;
}

BOOL ArtifactSetting::UnInit()
{
	return TRUE;
}
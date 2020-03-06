/**
 * UIResourcePool
 * UI 구성에 필요한 이미지 파일등을 읽어 저장해둔다.
 */

#pragma once

#include "Singleton.h"
class UIResourcePool : public Singleton<UIResourcePool>
{
	void								Initialize();
	void								Load();
};


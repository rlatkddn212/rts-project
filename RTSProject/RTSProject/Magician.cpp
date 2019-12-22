#include "Precompiled.h"
#include "Magician.h"


Magician::Magician()
{
	mSkinnedMesh = std::make_shared<SkinnedMesh>();
	mSkinnedMesh->LoadModel("Assets/Model/magician.X");
	MakeBoxObject();
}


Magician::~Magician()
{
}

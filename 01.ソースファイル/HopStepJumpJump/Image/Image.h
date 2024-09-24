#pragma once
#include "ImageBase.h"
class Image : public ImageBase
{
public:
	Image();
	virtual ~Image();

	void Update();
	void Draw() override;

private:
};


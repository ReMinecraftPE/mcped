/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/
#pragma once

#include "DynamicTexture.hpp"

class WaterTexture : public DynamicTexture
{
public:
	WaterTexture();
	~WaterTexture();

	void tick() override;

public:
	int field_40C = 0;
	int field_410 = 0;
	float* m_data1 = nullptr;
	float* m_data2 = nullptr;
	float* m_data3 = nullptr;
	float* m_data4 = nullptr;
};

/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "Item.hpp"

class DoorItem : public Item
{
public:
	DoorItem(int id, Material* pMtl);

	virtual bool useOn(ItemInstance*, Player*, Level*, int, int, int, int);

public:
	Material* material;
};

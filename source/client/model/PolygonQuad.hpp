/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include <utility>
#include "GameMods.hpp"
#include "VertexPT.hpp"
#include "client/renderer/Tesselator.hpp"

class PolygonQuad
{
public:
	PolygonQuad() {};
	PolygonQuad(VertexPT* a, VertexPT* b, VertexPT* c, VertexPT* d);
	PolygonQuad(VertexPT* a, VertexPT* b, VertexPT* c, VertexPT* d, float u1, float v1, float u2, float v2);
	PolygonQuad(VertexPT* a, VertexPT* b, VertexPT* c, VertexPT* d, int u1i, int v1i, int u2i, int v2i);

	void flipNormal();
	void mirror();
	void render(Tesselator& t, float scale, int unused);

#ifdef ENH_ENTITY_SHADING
	void setColor(float r, float g, float b);
#endif

private:
	VertexPT m_verts[4];
	bool m_bFlipNormals = false;

#ifdef ENH_ENTITY_SHADING
	int m_color = 0xFFFFFF;
#endif
};

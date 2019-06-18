#pragma once

#include <string>

struct D3D11_INPUT_ELEMENT_DESC;

struct ShaderLayoutDesc
{
	static D3D11_INPUT_ELEMENT_DESC Vertex[1];
	static unsigned v_layout_size;

	static D3D11_INPUT_ELEMENT_DESC VertexColor[2];
	static unsigned vc_layout_size;

	static D3D11_INPUT_ELEMENT_DESC VertexNormalUv[3];
	static unsigned vnu_layout_size;

	static D3D11_INPUT_ELEMENT_DESC VertexNormalUvTangent[4];
	static unsigned vnut_layout_size;

	static D3D11_INPUT_ELEMENT_DESC VertexNormalUvTangentBitangent[5];
	static unsigned vnutb_layout_size;
};
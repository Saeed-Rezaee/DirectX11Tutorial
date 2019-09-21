#include "Header.hlsli"

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.pos = input.pos;
	output.color = input.color;
	output.uv = input.uv;

	return output;
}
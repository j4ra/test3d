struct VSOut
{
    float4 col : COLOR;
    float4 pos : SV_POSITION;
};

cbuffer CBuf
{
    matrix transform;
};

VSOut main(float2 pos : POSITION, float4 col : COLOR)
{
    VSOut vout;
    vout.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    vout.col = col;
    return vout;
}
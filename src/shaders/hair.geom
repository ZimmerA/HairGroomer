#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 70) out;

in VS_OUT {
    vec2 texCoord;
    vec2 uv;
    vec3 normal;
    vec3 worldPos;
} gs_in[];

out vec4 vertColor;
uniform mat4 model;
uniform mat4 vp;

// Hair uniforms
uniform sampler2D hairMap;
uniform float maxHairLength;

const float PI = 3.14159265359f;
const float maxHairBendAngle = 25.0f;

float degToRad(float deg)
{
   return deg * PI/180.0f;
}

mat3 rotXMatrix(float rad)
{
    mat3 mat;
    mat[0] = vec3(1,0,0);
    mat[1] = vec3(0, cos(rad), -sin(rad));
    mat[2] = vec3(0,sin(rad),cos(rad));

    return transpose(mat);
}

mat3 rotYMatrix(float rad)
{
    mat3 mat;
    mat[0] = vec3(cos(rad),0,-sin(rad));
    mat[1] = vec3(0,1,0);
    mat[2] = vec3(sin(rad),0,cos(rad));

    return transpose(mat);
}

// Compute tangent and bitangent for a given triangle
vec3[2] ComputeTangentSpace(vec3 pos[3], vec2 uv[3])
{
    vec3 deltaPos1  = pos[1] - pos[0];
    vec3 deltaPos2 = pos[2] - pos[0];

    vec2 deltaUV1 = uv[1] - uv[0];
    vec2 deltaUV2 = uv[2] - uv[0];

    float r = 1.0/(deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

    vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2* deltaUV1.y) *r;
    vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

    vec3 ts[2];

    ts[0] = tangent;
    ts[1] = bitangent;

    return ts;
}

// Maps a color with a range from 0 to 1 to a range of -maxHairBendAngle to maxHairBendAngle (in radians)
float reMapColorValue(float colorValue)
{
    // Map to -1 to 1
    float minusOneToOne = colorValue * 2 - 1.0;

    // Map from -1 to 1 to -maxHairBendAngle to maxHairBendAngle (radians)
    float minusPiToPi = (minusOneToOne+1.0)/(1.0+1.0) * (degToRad(maxHairBendAngle)+degToRad(maxHairBendAngle)) - degToRad(maxHairBendAngle);

    return minusPiToPi;
}

// Helper function so a color value of 128 (RGB) is equal to exactly .5 (no hair bend)
float commaSmooth(float value)
{
    float alteredValue = value;

    if(alteredValue > 0.4999 && alteredValue < 0.51)
        alteredValue = 0.5;

    return alteredValue;
}

// Calculates a rotation matrix that rotates hair arround the x and y axis (tangent/bitangent)
mat3 calculateHairBendMatrix(float greenChannel, float blueChanel)
{
    greenChannel = commaSmooth(greenChannel);
    blueChanel = commaSmooth(blueChanel);
    
    float tangentBend = reMapColorValue(greenChannel);
    float bitangentBend = reMapColorValue(blueChanel);

    // Generate rotationMatrices
    mat3 mRotT = rotXMatrix(tangentBend);
    mat3 mRotB = rotYMatrix(bitangentBend);

    mat3 hairRot = mRotB * mRotT;
    return hairRot;
}

void main() {

    vec2 averageUV = (gs_in[0].texCoord + gs_in[1].texCoord + gs_in[2].texCoord)/3.0;
    float hair = texture(hairMap,averageUV).r;

	if(hair < 0.01)
    	return;
    mat3 mHairRot = calculateHairBendMatrix(texture(hairMap,averageUV).g,texture(hairMap,averageUV).b);

    // Compute tangent space matrix for hair bending
    vec3 vecPositions[3];
    vec2 vecTexcoords[3];
    for(int i = 0; i < 3; i++)
    {
        vecPositions[i] = gs_in[i].worldPos;
        vecTexcoords[i] = gs_in[i].uv;
    }

    vec3 tangentBitangent[2]  = ComputeTangentSpace(vecPositions,vecTexcoords);
    vec3 tangent = tangentBitangent[0];
    vec3 biTangent = tangentBitangent[1];

    // Generate Hair
    for(int i = 0; i < 3; ++i) 
    {
        vec3 worldPos = gs_in[i].worldPos;
        vec3 worldNorm = gs_in[i].normal;

        worldNorm = normalize(worldNorm);
        vec3 worldTangent = normalize(tangent);
        vec3 worldBiTangent = normalize(biTangent);

        // Tangent space matrix;
        mat3 tbn;
        tbn[0] = worldTangent;
        tbn[1] = worldBiTangent;
        tbn[2] = worldNorm;

        // Hair startpoint
        vertColor = vec4(0,0,0,1);
        vec3 d = vec3(0,0,1);
        gl_Position = vp * vec4(worldPos,1.0);
        EmitVertex();

        // Hair segments
        int numSegments =5;
        for(int i = 0; i < numSegments ; i++)
        {
            vertColor = vec4(0.06, 0.39,0.31,1);
            d = mHairRot*  d;
            worldPos = worldPos + maxHairLength / numSegments * tbn * d;
            gl_Position = vp * vec4(worldPos,1.0);
            EmitVertex(); 
        }
        EndPrimitive();
    }
} 
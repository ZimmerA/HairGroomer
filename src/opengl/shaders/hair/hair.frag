#version 330 core

out vec4 color;

in vec3 worldPosOut;
in vec4 vertColor;

// Direction vector of hair segment
in vec3 dOut;

const float kD = .7;
const float kS = .4;
const float shininess = 64;

uniform bool lighting;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main()
{
	if(lighting)
	{
		vec3 lightDir = normalize(lightPos - worldPosOut);
		float lDotD = dot(lightDir, dOut);
		float lDotN = sqrt(1 - (lDotD*lDotD));

		// Diffuse factor
		float lambert = kD * max(lDotN, 0.0);
		// Specular factor
		float specular = 0.0;

		if(lambert > 0.0) 
		{
			vec3 viewDir = normalize(cameraPos-worldPosOut);       
			vec3 h = normalize(lightDir + viewDir);        
			float dDotH = dot(dOut, h);
			float nDotH = sqrt(1-(dDotH*dDotH));

			float specAngle = max(nDotH, 0.0);
			specular = kS * pow(specAngle, shininess);
		}

    	color = vertColor * vec4( lambert *lightColor + specular * lightColor,1.0);
	}else
	{
		color = vertColor;
	}

}
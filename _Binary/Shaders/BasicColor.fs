#version 330

in vec3 Color;

uniform vec3 SolidColor = vec3(-1,-1,-1);
uniform bool Complimentary;

out vec4 Fragment;

void main()
{
	if(Complimentary){
		Fragment = vec4(vec3(1,1,1) - Color,1);
	}
	else{
		Fragment = vec4(Color,1);
	}

	if(SolidColor.r != -1.0 && SolidColor.g != -1.0 && SolidColor.b != -1.0){
		if(Complimentary){
			Fragment = vec4(vec3(1,1,1) - SolidColor, 1);
		}
		else{
			Fragment = vec4(SolidColor, 1);
		}
	}
	return;
}
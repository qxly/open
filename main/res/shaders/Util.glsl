#define PI 3.14159265

float unpackUnorm12(float u) {
	return u / 4095.0;
}

float packUnorm12Float(float f) {
	return round(clamp(f, 0.0, 1.0) * float(4095));
}

vec2 twoNorm12sEncodedAsUVec3InVec3FormatToPackedVec2(vec3 v) {
	vec2 s;
	// Roll the (*255s) in during the quasi bit shifting. This causes two of the three multiplications to happen at compile time
	float temp = v.y * (255.0 / 16.0);
	s.x = v.x * (255.0*16.0) + floor(temp);
	s.y = fract(temp) * (16 * 256) + (v.z * 255.0);
	return s;
}


vec2 twoUnorm12sEncodedAsUVec3InVec3FormatToVec2(vec3 v) {
	vec2 s = twoNorm12sEncodedAsUVec3InVec3FormatToPackedVec2(v);
	return vec2(unpackUnorm12(s.x), unpackUnorm12(s.y));
}

vec3 twoNorm12sEncodedAs3Unorm8sInVec3Format(vec2 s) {
	vec3 u;
	u.x = s.x * (1.0 / 16.0);
	float t = floor(s.y*(1.0 / 256));
	u.y = (fract(u.x) * 256) + t;
	u.z = s.y - (t * 256);
	// Instead of a floor, you could just add vec3(-0.5) to u, 
	// and the hardware will take care of the flooring for you on save to an RGB8 texture
	return floor(u) * (1.0 / 255.0);
}

vec3 vec2To2Unorm12sEncodedAs3Unorm8sInVec3Format(vec2 v) {
	vec2 s = vec2(packUnorm12Float(v.x), packUnorm12Float(v.y));
	return twoNorm12sEncodedAs3Unorm8sInVec3Format(s);
}

vec3 sphericalDecode(in vec2 p) {
	float theta = p.y * PI;
	float phi = (p.x * (2.0 * PI) - PI);

	float sintheta = sin(theta);
	return vec3(sintheta * sin(phi), cos(theta), sintheta * cos(phi));
}

vec2 sphericalEncode(in vec3 v) {
	// Assuming rho is always 1 (normalized v)
	float thetaNormalized = acos(v.y) / PI;
	float phiNormalized = (atan(v.x, v.z) / PI) * 0.5 + 0.5;
	return vec2(phiNormalized, thetaNormalized);
}

vec3 decode24(in vec3 p) {
	vec2 v = twoUnorm12sEncodedAsUVec3InVec3FormatToVec2(p);
	return sphericalDecode(v);
}

vec3 encode24(in vec3 p) {
	return vec2To2Unorm12sEncodedAs3Unorm8sInVec3Format(sphericalEncode(p));
}

void unProj(float depth, vec2 sp, mat4 mvpInverse, out vec4 worldPos)
{
	///世界坐标
	depth = 2.0 * depth - 1;///映射到 -1,1
	worldPos = mvpInverse * vec4(sp * 2 - 1, depth, 1);
	worldPos.xyzw /= worldPos.w;
}

vec4 point2Eye(vec4 p, mat4 mv) {
	return mv * p;
}

//Default BRDF
vec3 BRDF(vec3 lightDir, vec3 viewDir, float NdotV, vec3 lightDiffuse, vec3 lightSpecular)
{
	vec3 halfWay = normalize(lightDir + viewDir);
	float NdotL = clamp(dot(nNormal, lightDir), 0.0, 1.0);
	float NdotH = clamp(dot(nNormal, halfWay), 0.0, 1.0);
	float VdotH = clamp(dot(viewDir, halfWay), 0.0, 1.0);
	float sqR = ROUGHNESS * ROUGHNESS;
	//Roughness/Distribution/NDF term (GGX)
	//Formula:
	//    Where alpha = roughness
	//    R = alpha^2 / [ PI * [ ( NdotH^2 * (alpha^2 - 1) ) + 1 ]^2 ]
	float f = (NdotH * sqR - NdotH) * NdotH + 1.0;
	float R = sqR / (f * f + 1e-6f);
	//Geometric/Visibility term (Smith GGX Height-Correlated)
	float Lambda_GGXV = NdotL * sqrt((-NdotV * sqR + NdotV) * NdotV + sqR);
	float Lambda_GGXL = NdotV * sqrt((-NdotL * sqR + NdotL) * NdotL + sqR);
	float G = 0.5 / ((Lambda_GGXV + Lambda_GGXL + 1e-6f) * 3.141592654);
	//Formula:
	//    fresnelS = lerp( (1 - V*H)^5, 1, F0 )
	float fresnelS = material.F0.x + pow(1.0 - VdotH, 5.0) * (1.0 - material.F0.x);
	//We should divide Rs by PI, but it was done inside G for performance
	vec3 Rs = (fresnelS * (R * G)) * material.kS.xyz.xyz * lightSpecular;

	//Diffuse BRDF (*Normalized* Disney, see course_notes_moving_frostbite_to_pbr.pdf
	//"Moving Frostbite to Physically Based Rendering" Sebastien Lagarde & Charles de Rousiers)
	float energyBias = ROUGHNESS * 0.5;
	float energyFactor = mix(1.0, 1.0 / 1.51, ROUGHNESS);
	float fd90 = energyBias + 2.0 * VdotH * VdotH * ROUGHNESS;
	float lightScatter = 1.0 + (fd90 - 1.0) * pow(1.0 - NdotL, 5.0);
	float viewScatter = 1.0 + (fd90 - 1.0) * pow(1.0 - NdotV, 5.0);
	float fresnelD = 1.0 - fresnelS;
	//We should divide Rd by PI, but it is already included in kD
	vec3 Rd = (lightScatter * viewScatter * fresnelD) * material.kD.xyz * lightDiffuse;

	return NdotL * (Rs + Rd);
}

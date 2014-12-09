#include "lighting.h"
#include "renderingEngine.h"
#include "shader.h"
#include "coreEngine.h"

#define COLOR_DEPTH 256

void BaseLight::AddToEngine(CoreEngine* engine)
{
	engine->GetRenderingEngine()->AddLight(this);
}

BaseLight::~BaseLight()
{
	if(m_shader) delete m_shader;
	if (m_shadowInfo) delete m_shadowInfo;
}

void BaseLight::SetShader(Shader* shader)
{
	if(m_shader) delete m_shader;
	m_shader = shader; 
}

void BaseLight::SetShadowInfo(ShadowInfo* shadowInfo)
{
	if (m_shadowInfo) delete m_shadowInfo;
	m_shadowInfo = shadowInfo;
}

ShadowCameraTransform BaseLight::CalcShadowCameraTransform(const Vector3f& mainCameraPos, const Quaternion& mainCameraRot)
{
	ShadowCameraTransform result;
	result.pos = GetTransform().GetTransformedPos();
	result.rot = GetTransform().GetTransformedRot();
	return result;
}

DirectionalLight::DirectionalLight(const Vector3f& color, float intensity, int shadowMapSizeAsPowerOf2, float shadowArea,
	float shadowSoftness,
	float lightBleedReductionAmount,
	float minVariance) :
	BaseLight(color, intensity)
{
	SetShader(new Shader("forward-directional"));
	
	this->halfShadowArea = shadowArea / 2.0f;

	if (shadowMapSizeAsPowerOf2 != 0){
		SetShadowInfo(new ShadowInfo(Matrix4f().InitOrthographic(-halfShadowArea, halfShadowArea, -halfShadowArea, halfShadowArea, -halfShadowArea, halfShadowArea), true, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedReductionAmount, minVariance));
	}
}

ShadowCameraTransform DirectionalLight::CalcShadowCameraTransform(const Vector3f& mainCameraPos, const Quaternion& mainCameraRot)
{
	ShadowCameraTransform result;
	result.pos = mainCameraPos + mainCameraRot.GetForward()* halfShadowArea;
	result.rot = GetTransform().GetTransformedRot();

	//Fix shimmering.
	float worldSpaceShadowMapTexelSize = (halfShadowArea * 2.0f)/((float) (1 << GetShadowInfo()->GetShadowMapSizeAsPowerOf2()));
	Vector3f altCameraPos = result.pos.Rotate(result.rot.Conjugate());

	altCameraPos.SetX(worldSpaceShadowMapTexelSize * floor(altCameraPos.GetX() / worldSpaceShadowMapTexelSize));
	altCameraPos.SetY(worldSpaceShadowMapTexelSize * floor(altCameraPos.GetY() / worldSpaceShadowMapTexelSize));

	result.pos = altCameraPos.Rotate(result.rot);

	return result;
}


PointLight::PointLight(const Vector3f& color, float intensity, const Attenuation& atten,
	bool isPointLight, 
	int shadowMapSizeAsPowerOf2,
	float shadowSoftness,
	float lightBleedReductionAmount,
	float minVariance) :
BaseLight(color, intensity, isPointLight),
atten(atten)
{
	float a = atten.exponent;
	float b = atten.linear;
	float c = atten.constant - COLOR_DEPTH * intensity * color.Max();

	range = (-b + sqrtf(b*b - 4 * a*c)) / (2 * a);

	SetShader(new Shader("forward-point"));

	if (shadowMapSizeAsPowerOf2 != 0){
		SetShadowInfo(new ShadowInfo(Matrix4f().InitPerspective(cos(ToRadians(90.0f)), 1.0, 0.1, this->range), false, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedReductionAmount, minVariance));
	}
}


SpotLight::SpotLight(const Vector3f& color, float intensity, const Attenuation& atten, float viewAngle, int shadowMapSizeAsPowerOf2,
	float shadowSoftness,
	float lightBleedReductionAmount,
	float minVariance) :
	PointLight(color, intensity, atten, true),
	cutoff(cos(viewAngle / 2))
{
	SetShader(new Shader("forward-spot"));
	if (shadowMapSizeAsPowerOf2 != 0){
	//cutoff stores a value between -1 and 1. 
	SetShadowInfo(new ShadowInfo(Matrix4f().InitPerspective(viewAngle, 1.0, 0.1, this->range), false, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedReductionAmount, minVariance));

	}
}

#ifndef LIGHTING_H
#define LIGHTING_H

#include "math3d.h"
#include "gameComponent.h"

class CoreEngine;

struct ShadowCameraTransform
{
	Vector3f pos;
	Quaternion rot;
};

class ShadowInfo
{


public:
	ShadowInfo(const Matrix4f& projection, bool flipFaces, int shadowMapSizeAsPowerOf2, float shadowSoftness = 1.0f, float lightBleedReductionAmount = 0.2f, float minVariance = 0.00002f) :
		m_projection(projection),
		//m_bias(bias),
		m_flipFaces(flipFaces),
		m_shadowMapSizeAsPowerOf2(shadowMapSizeAsPowerOf2),
		m_shadowSoftness(shadowSoftness),
		m_lightBleedingReductionAmount(lightBleedReductionAmount),
		m_minVariance(minVariance) {}

	inline Matrix4f GetProjection(){ return m_projection; }
	
	//Artifact handling
	// Used for PCF
	//	inline float GetBias(){ return m_bias; }
	inline bool GetFlipFaces(){ return m_flipFaces; }
	inline float GetShadowSoftness() { return m_shadowSoftness; }
	inline int GetShadowMapSizeAsPowerOf2(){ return m_shadowMapSizeAsPowerOf2; }
	inline float GetLightBleedReductionAmount() { return m_lightBleedingReductionAmount; }
	inline float GetMinVariance() { return m_minVariance; }

protected:
private:
	
	//Artifact handling
	//Used for PCF
	//float m_bias;
	bool m_flipFaces;

	int m_shadowMapSizeAsPowerOf2;

	float m_shadowSoftness;
	float m_lightBleedingReductionAmount;
	float m_minVariance;

	Matrix4f m_projection;
};

class BaseLight : public GameComponent
{
public:
	Vector3f color;
	float intensity;
	
	BaseLight(const Vector3f& color = Vector3f(0,0,0), float intensity = 0) :
		color(color),
		intensity(intensity),
		m_shader(0),
		m_shadowInfo(0) {}
	
	virtual ~BaseLight();

	virtual ShadowCameraTransform CalcShadowCameraTransform(const Vector3f& mainCameraPos, const Quaternion& mainCameraRot);
	
	virtual void AddToEngine(CoreEngine* engine);	
	inline Shader* GetShader() { return m_shader; }
	inline ShadowInfo* GetShadowInfo() { return m_shadowInfo; }

protected:
	void SetShader(Shader* shader);
	void SetShadowInfo(ShadowInfo* shadowinfo);
private:
	BaseLight(BaseLight& other) {}
	void operator=(BaseLight& other) {}

	Shader* m_shader;
	//Should cast shadow?
	ShadowInfo* m_shadowInfo;
};

struct DirectionalLight : public BaseLight
{
	DirectionalLight(const Vector3f& color = Vector3f(0,0,0), float intensity = 0, int shadowMapSizeAsPowerOf2 = 0, float shadowArea = 80.0f, float shadowSoftness = 1.0f, float lightBleedReductionAmount = 0.2f, float minVariance = 0.000002f);
	
	virtual ShadowCameraTransform CalcShadowCameraTransform(const Vector3f& mainCameraPos, const Quaternion& mainCameraRot);

	float halfShadowArea;

};

struct Attenuation
{
	float constant;
	float linear;
	float exponent;

	Attenuation(float constant = 0, float linear = 0, float exponent = 1) :
		constant(constant),
		linear(linear),
		exponent(exponent) {}
};

struct PointLight : public BaseLight
{
	Attenuation atten;
	float range;

	PointLight(const Vector3f& color = Vector3f(0,0,0), float intensity = 0, const Attenuation& atten = Attenuation());
};

struct SpotLight : public PointLight
{
	float cutoff;

	SpotLight(const Vector3f& color = Vector3f(0, 0, 0), 
		float intensity = 0, 
		const Attenuation& atten = Attenuation(), 
		float viewAngle = ToRadians(179.0f),
		int shadowMapSizeAsPowerOf2 = 0, 
		float shadowSoftness = 1.0f, 
		float lightBleedReductionAmount = 0.2f, 
		float minVariance = 0.00002f);
};

#endif

#ifndef _MODEL_H
#define  _MODEL_H

#include "mesh.h"
#include "global.h"

class Model
{
public:
	Model(std::string modelName, bool isGlass = false, bool isAxes = false);
	~Model();

	void SetDiffuseTexture(ork::ptr<ork::Texture2D> diffuseTexture);
	void SetNormalTexture(ork::ptr<ork::Texture2D> normalTexture);
	void SetSpecularTexture(ork::ptr<ork::Texture2D> specularTexture);

	void SetTextureCube(ork::ptr<ork::TextureCube> textureCube);
	void SetMaterial(Material material);
	void SetAlpha(float alpha);
	void SetBlend(bool flag);
	void SetShadowMap(bool flag);

	void Render(ork::ptr<ork::FrameBuffer> fb,
		ork::vec3f translate = ork::vec3f::ZERO, ork::vec3f rotate = ork::vec3f::ZERO, ork::vec3f scale = ork::vec3f(1.0, 1.0, 1.0));

	AABB boundAABB;

	ork::mat4f M;
	ork::vec3f translate;

	static void SetRenderDepth(bool flag);

private:
	ork::ptr<ork::Program> program;
	ork::ptr<ork::Program> renderDepthProgram;

	std::vector<OrkMesh> meshes;

	ork::vec3f rotate;
	ork::vec3f scale;

	bool isGlass;//�Ƿ�Ϊ��������
	bool isAxes;//�Ƿ�Ϊ������

	bool isBlend;//�Ƿ�Ҫ���
	bool isShadowMap;//�Ƿ�ͶӰ

	float alpha;//͸����

	static bool renderDepth;//�Ƿ�д�����

	void CalcBoundAABB();//�����Χ��
};

#endif //_MODEL_H
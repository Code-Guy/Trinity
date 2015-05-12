#include "fileloader.h"
#include <cstdio>

using namespace std;
using namespace ork;

//�����ļ��������ļ����ݣ�char *�����ļ���С
unsigned char* FileLoader::LoadFile(const string filePath, int &size)
{
	FILE *file = fopen(filePath.c_str(), "rb");

	if (file == NULL)//���ļ����������ļ�·�������ڣ�ֱ�ӷ���NULL
	{
		return NULL;
	}

	//�Ȼ�ȡ�ļ���С
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	//�ص��ļ�ͷ
	fseek(file, 0, SEEK_SET);

	char *data = new char[size + 1];
	fread(data, size, 1, file);
	data[size] = 0;//���һ���ַ�Ϊ0��\0

	fclose(file);

	return (unsigned char *)data;
}

//�����ļ��������ļ����ݣ�char *��
unsigned char* FileLoader::LoadFile(const string filePath)
{
	FILE *file = fopen(filePath.c_str(), "rb");

	if (file == NULL)//���ļ����������ļ�·�������ڣ�ֱ�ӷ���NULL
	{
		return NULL;
	}

	//�Ȼ�ȡ�ļ���С
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	//�ص��ļ�ͷ
	fseek(file, 0, SEEK_SET);

	char *data = new char[size + 1];
	fread(data, size, 1, file);
	data[size] = 0;//���һ���ַ�Ϊ0��\0

	fclose(file);

	return (unsigned char *)data;
}

//����program
ptr<Program> FileLoader::LoadProgram(const string filePath)
{
	return new Program(new Module(330, (char *)LoadFile(filePath)));
}

//������պ�
ptr<TextureCube> FileLoader::LoadTextureCube(const string filePath, Texture::Parameters para)
{
	ptr<Buffer> buffers[6];
	Buffer::Parameters texpar[6] = { Buffer::Parameters() };
	int size, w, h, channels;
	string curpath = filePath + "/posx.jpg";
	unsigned char* chardata = LoadFile(curpath.c_str(), size);
	if (chardata == NULL)
	{
		return NULL;
	}
	unsigned char* tempdata = stbi_load_from_memory(chardata, size, &w, &h, &channels, 0);
	buffers[0] = new CPUBuffer(tempdata);

	curpath = filePath + "/negx.jpg";
	chardata = LoadFile(curpath.c_str(), size);
	if (chardata == NULL)
	{
		return NULL;
	}
	tempdata = stbi_load_from_memory(chardata, size, &w, &h, &channels, 0);
	buffers[1] = new CPUBuffer(tempdata);

	curpath = filePath + "/posy.jpg";
	chardata = LoadFile(curpath.c_str(), size);
	if (chardata == NULL)
	{
		return NULL;
	}
	tempdata = stbi_load_from_memory(chardata, size, &w, &h, &channels, 0);
	buffers[2] = new CPUBuffer(tempdata);

	curpath = filePath + "/negy.jpg";
	chardata = LoadFile(curpath.c_str(), size);
	if (chardata == NULL)
	{
		return NULL;
	}
	tempdata = stbi_load_from_memory(chardata, size, &w, &h, &channels, 0);
	buffers[3] = new CPUBuffer(tempdata);

	curpath = filePath + "/posz.jpg";
	chardata = LoadFile(curpath.c_str(), size);
	if (chardata == NULL)
	{
		return NULL;
	}
	tempdata = stbi_load_from_memory(chardata, size, &w, &h, &channels, 0);
	buffers[4] = new CPUBuffer(tempdata);

	curpath = filePath + "/negz.jpg";
	chardata = LoadFile(curpath.c_str(), size);
	if (chardata == NULL)
	{
		return NULL;
	}
	tempdata = stbi_load_from_memory(chardata, size, &w, &h, &channels, 0);
	buffers[5] = new CPUBuffer(tempdata);

	if (channels == 3)
		return new TextureCube(w, h, RGB8, RGB, PixelType::UNSIGNED_BYTE, para, texpar, buffers);
	return new TextureCube(w, h, RGBA8, RGBA, PixelType::UNSIGNED_BYTE, para, texpar, buffers);
}

//������ͼ
ptr<Texture2D> FileLoader::LoadTexture(const string filePath, Texture::Parameters para)
{
	int w, h, channels, size;
	unsigned char* chardata = LoadFile(filePath, size);
	if (chardata == NULL)
	{
		return NULL;
	}
	unsigned char* tempdata = stbi_load_from_memory(chardata, size, &w, &h, &channels, 0);

	if (channels == 3)
		return new Texture2D(w, h, RGB8, RGB, UNSIGNED_BYTE, para, Buffer::Parameters(), CPUBuffer(tempdata));
	return new Texture2D(w, h, RGBA8, RGBA, UNSIGNED_BYTE, para, Buffer::Parameters(), CPUBuffer(tempdata));
}

//����obj mesh�ļ�
bool FileLoader::LoadObjMesh(const string filePath, vector<ObjMesh> &meshes, string &mtlName)
{
	printf("Loading mesh %s\n", filePath);

	meshes.clear();
	ObjMesh objMesh;//һ��obj mesh

	bool isNextMesh = false;

	int vOffset = VBO::vertices.size();
	int uvOffset = VBO::uvs.size();
	int nOffset = VBO::normals.size();

	FILE *file = fopen(filePath.c_str(), "r");
	if (file == NULL)
	{
		printf("Could not open this obj file!\n");
		getchar();
		return false;
	}

	while (true)
	{
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "g") == 0)
		{
			fscanf(file, "%s\n", objMesh.ID);
			isNextMesh = true;
		}
		else if (strcmp(lineHeader, "mtllib") == 0)
		{
			char mtlNameStr[MAX_ID_LEN];//mtl�ļ���
			fscanf(file, "%s\n", mtlNameStr);
			mtlName = mtlNameStr;
		}
		else if (strcmp(lineHeader, "usemtl") == 0)
		{
			fscanf(file, "%s\n", objMesh.MID);
		}
		else if (strcmp(lineHeader, "v") == 0)
		{
			if (isNextMesh)
			{
				meshes.push_back(objMesh);
				objMesh.Clear();
				isNextMesh = false;
			}
			vec3f v;
			fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z);
			VBO::vertices.push_back(v);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			vec2f uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			VBO::uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			vec3f n;
			fscanf(file, "%f %f %f\n", &n.x, &n.y, &n.z);
			VBO::normals.push_back(n);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			//��������������ֻ��������^_^
			Face face;
			int vertexIndex[3], uvIndex[3], normalIndex[3];

			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			face.refers.push_back(Refer(vertexIndex[0] - 1 + vOffset, uvIndex[0] - 1 + uvOffset, normalIndex[0] - 1 + nOffset));
			face.refers.push_back(Refer(vertexIndex[1] - 1 + vOffset, uvIndex[1] - 1 + uvOffset, normalIndex[1] - 1 + nOffset));
			face.refers.push_back(Refer(vertexIndex[2] - 1 + vOffset, uvIndex[2] - 1 + uvOffset, normalIndex[2] - 1 + nOffset));

			objMesh.faces.push_back(face);
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	fclose(file);

	meshes.push_back(objMesh);

	return true;
}

//����ork mesh�ļ�
bool FileLoader::LoadOrkMesh(const string filePath, vector<OrkMesh> &meshes)
{
	ObjMesh objMesh;//һ��obj mesh
	vector<ObjMesh> objMeshes;//����obj mesh
	string mtlName = "";

	LoadObjMesh(filePath, objMeshes, mtlName);

	if (mtlName != "")
		LoadMaterialTex(mtlName, objMeshes);//������ʺ���ͼ��objMeshes

	FillMeshes(objMeshes, meshes);//���Զ����objmesh�����ork mesh

	VBO::Clear();//���vbo

	return true;
}

//������ʺ���ͼ
bool FileLoader::LoadMaterialTex(const string mtlName, vector<ObjMesh> &objMeshes)
{
	printf("  %s\n", GetModelPath(mtlName));

	FILE *file = fopen(GetModelPath(mtlName).c_str(), "r");
	if (file == NULL)
	{
		getchar();
		return false;
	}

	vector<ObjMesh>::iterator mIter;//һ��mesh������

	while (true)
	{
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "newmtl") == 0)
		{
			char MID[MAX_ID_LEN];
			fscanf(file, "%s\n", MID);

			mIter = ObjMesh::GetObjMeshWithMID(objMeshes, MID);
		}
		else if (strcmp(lineHeader, "Kd") == 0 && mIter != objMeshes.end())
		{
			fscanf(file, "%f %f %f\n", &(mIter->material.diffuseColor.x), &(mIter->material.diffuseColor.y), &(mIter->material.diffuseColor.z));
		}
		else if (strcmp(lineHeader, "Ka") == 0 && mIter != objMeshes.end())
		{
			fscanf(file, "%f %f %f\n", &(mIter->material.ambientColor.x), &(mIter->material.ambientColor.y), &(mIter->material.ambientColor.z));
		}
		else if (strcmp(lineHeader, "Ks") == 0 && mIter != objMeshes.end())
		{
			fscanf(file, "%f %f %f\n", &(mIter->material.specularColor.x), &(mIter->material.specularColor.y), &(mIter->material.specularColor.z));
		}
		else if (strcmp(lineHeader, "map_Kd") == 0 && mIter != objMeshes.end())
		{
			char texName[MAX_ID_LEN];
			fscanf(file, "%s\n", texName);
			mIter->diffuseTexName = string(texName);
		}
		else if (strcmp(lineHeader, "map_Ka") == 0 && mIter != objMeshes.end())
		{
			char texName[MAX_ID_LEN];
			fscanf(file, "%s\n", texName);
			mIter->ambientTexName = string(texName);
		}
		else if (strcmp(lineHeader, "map_Ks") == 0 && mIter != objMeshes.end())
		{
			char texName[MAX_ID_LEN];
			fscanf(file, "%s\n", texName);
			mIter->specularTexName = string(texName);
		}
		else if (strcmp(lineHeader, "bump") == 0 && mIter != objMeshes.end())
		{
			char texName[MAX_ID_LEN];
			fscanf(file, "%s\n", texName);
			mIter->normalTexName = string(texName);
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	return true;
}

//���Զ����objmesh�����ork mesh
void FileLoader::FillMeshes(vector<ObjMesh> &objMeshes, vector<OrkMesh> &meshes)
{
	//��������obj mesh
	for (vector<ObjMesh>::iterator oIter = objMeshes.begin(); oIter != objMeshes.end(); oIter++)
	{
		AABB aabb;
		aabb = oIter->CalcAABB();//�����Χ��

		ptr< Mesh<V_UV_N_T_B, unsigned int> > mesh = new Mesh<V_UV_N_T_B, unsigned int>(TRIANGLES, GPU_STATIC);
		mesh->addAttributeType(0, 3, A32F, false);
		mesh->addAttributeType(1, 2, A32F, false);
		mesh->addAttributeType(2, 3, A32F, false);
		mesh->addAttributeType(3, 2, A32F, false);
		mesh->addAttributeType(4, 3, A32F, false);

		//����obj mesh������face
		for (vector<Face>::const_iterator fIter = oIter->faces.begin(); fIter != oIter->faces.end(); fIter++)
		{
			vec3f T, B;
			CalcTB(T, B, *fIter);//����Tangent��Bitangent����
			for (vector<Refer>::const_iterator rIter = fIter->refers.begin(); rIter != fIter->refers.end(); rIter++)
			{
				mesh->addVertex(V_UV_N_T_B(VBO::vertices[rIter->v].x, VBO::vertices[rIter->v].y, VBO::vertices[rIter->v].z,
					VBO::uvs[rIter->uv].x, VBO::uvs[rIter->uv].y,
					VBO::normals[rIter->n].x, VBO::normals[rIter->n].y, VBO::normals[rIter->n].z,
					T.x, T.y, T.z, B.x, B.y, B.z));

			}
		}

		if (oIter->diffuseTexName != "")
		{
			printf("    Diffuse Texture: %s\n", oIter->diffuseTexName.c_str());
		}
		if (oIter->normalTexName != "")
		{
			printf("    Normal Texture: %s\n", oIter->normalTexName.c_str());
		}
		if (oIter->specularTexName != "")
		{
			printf("    Specular Texture: %s\n", oIter->specularTexName.c_str());
		}
		if (oIter->ambientTexName != "")
		{
			printf("    Ambient Texture: %s\n", oIter->ambientTexName.c_str());
		}

		meshes.push_back(OrkMesh(mesh, oIter->material, aabb, 
			LoadTexture(GetTexturePath(oIter->diffuseTexName)),
			LoadTexture(GetTexturePath(oIter->normalTexName)),
			LoadTexture(GetTexturePath(oIter->specularTexName))));
	}
}

bool FileLoader::LoadPlanetParam(const std::string filePath, std::vector<Planet> &planets)
{
	planets.clear();//���
	FILE *file = fopen(filePath.c_str(), "r");
	if (file == NULL)
	{
		printf("Could not open this obj file!\n");
		getchar();
		return false;
	}

	while (true)
	{
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "p") == 0)
		{
			Planet p;
			fscanf(file, "%f %f %f %f %f %f %f %f\n", 
				&(p.pos.x), &(p.pos.y), &(p.pos.z),
				&(p.vec.x), &(p.vec.y), &(p.vec.z),
				&(p.mass), &(p.size));
			planets.push_back(p);
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	fclose(file);
	return true;
}

//��ȡ��Դ·��
string GetModelPath(string fileName)
{
	return BaseModelPath + fileName;
}

string GetTexturePath(string fileName)
{
	return BaseTexturePath + fileName;
}

string GetShaderPath(string fileName)
{
	return BaseShaderPath + fileName;
}

string GetParamPath(std::string fileName)
{
	return BaseParamPath + fileName;
}
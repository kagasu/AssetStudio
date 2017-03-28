#include <stdlib.h>
#include <Windows.h>
#pragma comment(lib, "AssetsTools.lib")

#pragma warning(disable:4200)

#include <AssetBundleExtractor\IPluginInterface.h>
#include <AssetsTools\TextureFileFormat.h>
#include <AssetsTools\AssetsReplacer.h>
#include <AssetsTools\AssetsFileTable.h>

QWORD MyAssetsReaderFromFile(QWORD pos, QWORD count, void *pBuf, LPARAM par)
{
	if (pos != -1)
	{
		auto fpos = (fpos_t)pos;
		if (fsetpos((FILE*)par, &fpos) != 0)
		{
			memset(pBuf, 0, (size_t)count);
			return 0;
		}
	}
	return fread(pBuf, 1, (size_t)count, (FILE*)par);
}

AssetTypeTemplateField *GetAssetTypeTemplateField(AssetsFile *assetsFile, int classId)
{
	auto *pBaseField = new AssetTypeTemplateField;
	for (DWORD i = 0; i < assetsFile->typeTree.fieldCount; i++)
	{
		if (assetsFile->typeTree.pTypes_Unity5[i].classId == classId)
		{
			pBaseField->From0D(&assetsFile->typeTree.pTypes_Unity5[i], 0);
			return pBaseField;
		}
	}

	return NULL;
}

void GetFilePath(const char* file, long long id, char *path)
{
	FILE *pFile;
	fopen_s(&pFile, file, "rb");

	printf("[LOAD] %s\n", file);

	AssetsFile assetsFile(MyAssetsReaderFromFile, (LPARAM)pFile);
	AssetsFileTable assetsFileTable(&assetsFile, false);
	AssetFileInfoEx *assetsFileInfo = assetsFileTable.getAssetInfo(1);
	auto pBaseField = GetAssetTypeTemplateField(&assetsFile, 0x8E);

	// Get Values
	auto ati = AssetTypeInstance(1, &pBaseField, MyAssetsReaderFromFile, (LPARAM)pFile, assetsFile.header.endianness, assetsFileInfo->absolutePos);
	auto container = ati.GetBaseField()->Get("m_Container");
	auto array = container->Get("Array");
	auto children = array->GetChildrenList();

	for (DWORD j = 0; j < array->GetValue()->AsArray()->size; j++)
	{
		auto filePath = children[j]->Get("first")->GetValue()->AsString();
		auto pathId = children[j]->Get("second")->Get("asset")->Get("m_PathID")->GetValue()->AsInt64();

		if (pathId == id)
		{
			//printf("filePath: %s\n", filePath);
			strcpy_s(path, 1024, filePath);
			break;
		}
	}
}
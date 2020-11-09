#pragma once
#include "myfunc.h"
#pragma comment (lib,"Gdiplus.lib")
#include <cstring>
#include "tex.h"
#include "resource2.h"

inline Gdiplus::Bitmap* getFileIStream(const char lpszPath[]) {
	Gdiplus::GdiplusStartupInput gpSI;
	ULONG_PTR lpToken;
	Gdiplus::GdiplusStartup(&lpToken, &gpSI, NULL);

	/* �摜�t�@�C����ǂݍ���IStream���쐬 */
	HANDLE hFile;
	HGLOBAL hBuf;
	LPVOID lpBuf;

	IStream* isFile;

	DWORD dwFileSize, dwLoadSize;

	/* �摜�t�@�C���I�[�v�� */
	hFile = CreateFile(lpszPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	/* �t�@�C���T�C�Y�擾 */
	dwFileSize = GetFileSize(hFile, NULL);

	/* �摜�t�@�C���f�[�^�i�[�p�������u���b�N�m�� */
	hBuf = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);

	/* �������u���b�N�����b�N���A�h���X���擾 */
	lpBuf = GlobalLock(hBuf);

	/* �摜�t�@�C���̃f�[�^���������u���b�N�ɓǂݍ��� */
	ReadFile(hFile, lpBuf, GetFileSize(hFile, NULL), &dwLoadSize, NULL);

	CloseHandle(hFile);

	/* �������u���b�N�̃��b�N���� */
	GlobalUnlock(hBuf);

	/* �������u���b�N����IStream���쐬 */
	CreateStreamOnHGlobal(hBuf, TRUE, &isFile);

	/* IStream��Ԃ� */
	IStream* is = isFile;
	return 	new Gdiplus::Bitmap(is);
}

inline void getTex(std::vector<std::vector<std::vector<Gdiplus::Bitmap*>>>& tex) {
	std::vector<std::string> filename;
	std::string nam = "C:\\Users\\roxas1533\\Downloads\\yukari\\yukari\\png";
	getFileNmaes(nam, filename);
	for (auto name : filename) {
		std::vector<std::vector<Gdiplus::Bitmap*>> temp;
		std::vector<Gdiplus::Bitmap*> temp2;
		std::vector<std::string> pngname;
		getFileNmaes(name, pngname);
		bool start = false;
		bool flag = false;

		for (auto n : pngname) {
			//std::cout << n<<"\n";
;			if ('a' <= n[n.size() - 5] && 'z' >= n[n.size() - 5]) {
				temp2.push_back(getFileIStream(n.c_str()));
				flag = true;
				continue;
			}
			if (flag) {
				temp.push_back(temp2);
				temp2.clear();
				temp2.push_back(getFileIStream(n.c_str()));
				flag = false;
			}
			else {
				if (start&&!temp2.empty()) {
					temp.push_back(temp2);
					temp2.clear();
				}
				temp2.push_back(getFileIStream(n.c_str()));
				if (start&& !temp2.empty()) {
					temp.push_back(temp2);
					temp2.clear();
				}
			}
			start = true;

		}
		//temp2.clear();
		//std::cout << pngname[pngname.size() - 1].c_str()<<"\n";
		//temp2.push_back(getFileIStream(pngname[pngname.size()-1].c_str()));
		temp.push_back(temp2);
		tex.push_back(temp);
	}
	//for(int i=0; i<tex[OTHER].size(); i++)
	//std::cout << tex[OTHER][i].size()<<"\n";
}

inline void getTexF(Gdiplus::Bitmap* tex) {
	std::string nam = "C:\\Users\\roxas1533\\Downloads\\yukari\\yukari\\png\\5313.png";
	tex= getFileIStream(nam.c_str());

}

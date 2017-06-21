/*
	jwqe764241 ¿€º∫
*/

#ifndef __RELEASE_H
#define __RELEASE_H

template <typename T>
void ReleaseCOM(T ** pTy) {
	if ((*pTy)) {
		(*pTy)->Release();
		(*pTy) = nullptr;
	}
}

#endif
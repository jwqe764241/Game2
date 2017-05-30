#pragma once

template <typename T>
void ReleaseCOM(T ** pTy) {
	if ((*pTy)) {
		(*pTy)->Release();
		(*pTy) = nullptr;
	}
}
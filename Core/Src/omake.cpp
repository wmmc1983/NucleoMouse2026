/*
 * omake.cpp
 *
 *  Created on: 2026/08/17
 *      Author: Karasawa
 */
/*
-------------------------------------------------------------------
○標準コード　***** WMMC Nucleo Mouse 2026 *****

C++をCで使用する方法とか

WMMC 柄澤
2026 / 08 / 17
-------------------------------------------------------------------
*/
//このファイルは読まなくても大丈夫です。C++に興味がある方は読んでみてください。omake.hppの方に詳細が書いてあります。
#include "global.h"
#include "omake.hpp"
//valueを初期化リストを用いて初期化している。
SampleClass::SampleClass() :value(3) {
	/*
	value = 3;
	このように書いても初期化出来る。
	*/
	data.push_back(1);
	data.push_back(2);
	data.push_back(3);
}

int SampleClass::GetValue() const {
	return value;
}

void SampleCppFunction() {
	SampleClass sample;
	int value = sample.GetValue();
	printf("SampleCppFunction: Value = %d\n", value);
}
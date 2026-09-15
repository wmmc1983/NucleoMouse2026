/*
 * omake.hpp
 *
 *  Created on: 2026/08/17
 *      Author: Karasawa
 */
 //このファイルは読まなくても大丈夫です。C++に興味がある方は読んでみてください。
#ifndef INC_OMAKE_HPP_
#define INC_OMAKE_HPP_

/*
* __cplusplus マクロはC++環境でのみ定義されるため、
#ifdef __cplusplus と #endif で囲まれたコードはC++コンパイラでのみコンパイルされる。よって、
#ifdef __cplusplus
と、
#endif
の間に挟まれたコードがC++コンパイラでのみ有効になり、Cコンパイラでは無視される。
extern "C"{}内のコードはC言語側からも呼び出せるようになる。
*/
#ifdef __cplusplus
extern "C" {
#endif
//この中にかいた宣言はCから呼ぶことができる。

	void SampleCppFunction();


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
// ここのコードはCコンパイラでは無視される。C++コンパイラでのみ有効になる。
#include <vector>
//サンプル
class SampleClass {
private:
	int value;
	std::vector <int> data;
public:
	SampleClass();
	int GetValue() const; //constを付けると、メンバ関数内でメンバ変数を変更できなくなる。
};

//ここまでC++コンパイラでのみ有効になるコード
#endif

#endif /* INC_OMAKE_HPP_ */

#pragma once

namespace Easing
{
	
	/// <summary>
	/// テンプレートクラス
	/// イージング関数をテンプレート関数にして
	/// 引数の型を変えられるようにする
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="t">経過時間</param>
	/// <param name="b">初期値</param>
	/// <param name="c">変化量</param>
	/// <param name="d">総時間</param>
	/// <returns>変化後の値</returns>
	template<typename T>
	inline T Linear(T t, T b, T c, T d)
	{
		return c * t / d + b;
	}

	template<typename T>
	inline T InQuad(T t, T b, T c, T d)
	{
		t /= d;
		return c * t * t + b;
	}

	template<typename T>
	inline T OutQuad(T t, T b, T c, T d)
	{
		t /= d;
		return -c * t * (t - 2) + b;
	}

	template<typename T>
	inline T InOutQuad(T t, T b, T c, T d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t * t + b;
		--t;
		return -c / 2 * (t * (t - 2) - 1) + b;
	}

	template<typename T>
	inline T InCubic(T t, T b, T c, T d)
	{
		t /= d;
		return c * t * t * t + b;
	}

	template<typename T>
	inline T OutCubic(T t, T b, T c, T d)
	{
		t /= d;
		--t;
		return c * (t * t * t + 1) + b;
	}

	template<typename T>
	inline T InOutCubic(T t, T b, T c, T d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t * t * t + b;
		t -= 2;
		return c / 2 * (t * t * t + 2) + b;
	}

	template<typename T>
	inline T InQuart(T t, T b, T c, T d)
	{
		t /= d;
		return c * t * t * t * t + b;
	}

	template<typename T>
	inline T OutQuart(T t, T b, T c, T d)
	{
		t /= d;
		--t;
		return -c * (t * t * t * t - 1);
	}

	template<typename T>
	inline T EaseOutCubic(T t, T b, T c, T d)
	{
		t /= d;
		t--;
		return c * (t * t * t + 1) + b * (1 - (t * t * t + 1));
	}
}
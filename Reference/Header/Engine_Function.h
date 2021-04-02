#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	//Obj 메모리 해제
	template<typename T>
	void Safe_Delete(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete Pointer;
			Pointer = nullptr;
		}
	}

	//배열 메모리 해제
	template<typename T>
	void Safe_Delete_Array(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete [] Pointer;
			Pointer = nullptr;
		}
	}

	//인스턴스 메모리 해제
	template<typename T>
	unsigned long Safe_Release(T& pInstance)
	{

		unsigned long dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}

		return dwRefCnt;
	}


	// 함수객체 : 인스턴스 해제
	class CDeleteObj
	{
	public:
		explicit CDeleteObj() {}
		~CDeleteObj() {}

	public:
		template<typename T>
		void		operator()(T& Pointer)
		{
			_ulong	dwRefCnt = 0;
			
			dwRefCnt = Pointer->Release();

			if (0 == dwRefCnt)
				Pointer = nullptr;
		}

	};

	//함수객체 : Map의 Second 해제
	class CDeleteMap
	{
	public:
		explicit CDeleteMap() {}
		~CDeleteMap() {}

	public:
		template<typename T>
		void		operator()(T& Pair)
		{
			_ulong	dwRefCnt = 0;

			dwRefCnt = Pair.second->Release();

			if (0 == dwRefCnt)
				Pair.second = nullptr;
		}
	};

	//함수객체 : TCHAR* 문자열 탐색 - Map의 Key값 확인
	class CTagFinder
	{
	public:
		explicit CTagFinder(const _tchar* pTag)
			: m_pTargetTag(pTag){		}
		~CTagFinder() {}

	public:
		template<typename T>
		_bool		operator()(const T& Pair)
		{
			if (0 == lstrcmp(m_pTargetTag, Pair.first))
				return true;
			
			return false;
		}

	private:
		const _tchar*	m_pTargetTag;

	};

	//함수객체 : TCHAR* 문자열 탐색 - Obj의 m_pObjTag값 확인
	class CObjTagFinder
	{
	public:
		explicit CObjTagFinder(const _tchar* pTag)
			: m_pTargetTag(pTag) {}
		~CObjTagFinder() {}

	public:
		template<typename T>
		_bool		operator()(const T& pObj)
		{
			if (0 == lstrcmp(m_pTargetTag, pObj->Get_ObjTag()))
				return true;

			return false;
		}

	private:
		const _tchar*	m_pTargetTag;
	};

}

#endif // Engine_Function_h__

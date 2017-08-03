//#ifndef WYIN_FIND_H
//#define WYIN_FIND_H 1
//
//#include <open/openDef.h>
//
//namespace open
//{
//	template<typename T>
//	struct Less
//	{
//		inline static bool compare(const T& l, const T& r)
//		{
//			return l < r;
//		}
//	};
//
//	template<typename T, typename key_compare = Less<T> >
//	inline int32 lower_bound(T* list, int32 size, const T& key)
//	{
//		if (size == 0)
//			return -1;
//
//		int32 count = size;
//		int32 first = 0;
//		for (; 0 < count;)
//		{
//			int32 count2 = count / 2;
//			T& mid = list[first + count2];
//
//			if (key_compare::compare(mid, key))
//			{
//				first += (count2 + 1);
//				count -= count2 + 1;
//			}
//			else
//				count = count2;
//		}
//
//		return first;
//	}
//
//	template<typename T,typename key_compare = Less<T> >
//	inline int32 binary_find(T* list,int32 size,const T& key)
//	{
//		int32 first = lower_bound(list, size, key);
//		if (first == size)
//			return -1;
//
//		if (first != size && key_compare::compare(key, list[first]))
//			return -1;
//		
//		return first;
//	}
//}
//
//#endif
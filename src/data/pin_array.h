#pragma once

#include "array.h"

namespace VI
{

template<s16 size> struct Bitmask
{
	u32 data[(size / (sizeof(u32) * 8)) + 1];
	s16 start;
	s16 end;

	Bitmask()
		: start(size), end(0)
	{
	}

	inline b8 get(s32 i) const
	{
		vi_assert(i >= 0 && i < size);
		s32 index = i / (sizeof(u32) * 8);
		return data[index] & (1 << (i - (index * sizeof(u32) * 8)));
	}

	inline b8 any() const
	{
		return start < end;
	}

	inline s32 next(s32 i) const
	{
		i++;
		while (i < end)
		{
			if (get(i))
				break;
			i++;
		}
		return i;
	}

	void clear()
	{
		start = size;
		end = 0;
		memset(data, 0, sizeof(data));
	}

	void set(s32 i, b8 value)
	{
		vi_assert(i >= 0 && i < size);
		s32 index = i / (sizeof(u32) * 8);
		u32 mask = 1 << (i - (index * sizeof(u32) * 8));
		if (value)
		{
			data[index] |= mask;
			start = start < i ? start : i;
			end = end > i + 1 ? end : s16(i + 1);
		}
		else
		{
			data[index] &= ~mask;

			if (i + 1 == end)
			{
				s32 j;
				for (j = i - 1; j > start; j--)
				{
					if (get(j))
						break;
				}
				end = s16(j + 1);
			}
			if (i == start)
			{
				s32 j;
				for (j = i + 1; j < end; j++)
				{
					if (get(j))
						break;
				}
				start = s16(j);
			}
			if (start >= end)
			{
				start = size;
				end = 0;
			}
		}
	}
};

template<typename T, s16 size>
struct PinArray
{
	StaticArray<T, size> data;
	Bitmask<size> mask;
	StaticArray<ID, size> free_list;

	struct Iterator
	{
		PinArray<T, size>* array;
		ID index;

		inline void next()
		{
			index = array->mask.next(index);
		}

		inline b8 is_last() const
		{
			return index >= array->mask.end;
		}

		inline T* item() const
		{
			vi_assert(!is_last() && array->active(index));
			return &array->data[index];
		}
	};

	PinArray()
		: data(), mask(), free_list()
	{
		data.length = size;
		free_list.length = size;
		for (s32 i = 0; i < size; i++)
			free_list[i] = ID((size - 1) - i);
	}

	inline b8 active(s32 i) const
	{
		return mask.get(i);
	}

	inline void active(s32 i, b8 value)
	{
		mask.set(i, value);
	}

	inline s32 count() const
	{
		return size - free_list.length;
	}

	inline T operator [] (const s32 i) const
	{
		vi_assert(i >= 0 && i < size);
		return data.data[i];
	}

	inline T& operator [] (const s32 i)
	{
		vi_assert(i >= 0 && i < size);
		return data.data[i];
	}

	Iterator iterator()
	{
		Iterator i;
		i.index = mask.start;
		i.array = this;
		return i;
	}

	void clear()
	{
		mask.clear();
		free_list.length = size;
		for (s32 i = 0; i < size; i++)
			free_list[i] = (size - 1) - i;
	}

	T* add()
	{
		vi_assert(free_list.length > 0);
		s32 index = free_list[free_list.length - 1];
		vi_assert(!active(index));
		free_list.remove(free_list.length - 1);
		active(index, true);
		return &data[index];
	}

	ID add(const T& t)
	{
		T* i = add();
		*i = t;
		return ID(((char*)i - (char*)&data[0]) / sizeof(T));
	}

	void remove(s32 i)
	{
		vi_assert(active(i));
		free_list.add(i);
		active(i, false);
	}
};

}

#pragma once

namespace sys
{

//Smart-Pointer class
template<class T>
class Ref
{
public:
	Ref()
	{
		obj_ = NULL;
	}

	Ref(T* Obj)
	{
		obj_ = Obj;
	}
	Ref(const Ref<T> & o)
	{
		obj_ = o.obj_;
	}

	~Ref()
	{
		Delete();
	}

	inline T* Get()
	{
		return obj_;
	}

	inline void operator ~()
	{
		Delete();
	}

	inline void operator =(T* Obj)
	{
		Delete();
		obj_ = Obj;
	}

	inline void operator =(const Ref<T> & o)
	{
		Delete();
		obj_ = o.obj_;
	}

	inline T* operator ->()
	{
		return obj_;
	}

	inline const T* operator ->() const
	{
		return obj_;
	}

	inline operator bool() const
	{
		return obj_ != NULL;
	}

	inline T& operator *()
	{
		return *obj_;
	}

	inline const T& operator *() const
	{
		return *obj_;
	}

	inline bool operator == (const Ref & o) const
	{
		return obj_ == o.obj_;
	}

	inline bool operator != (const Ref & o) const
	{
		return obj_ != o.obj_;
	}

	inline bool operator != (void * o) const
	{
		return obj_ != o;
	}

private:
	inline void Delete()
	{
		if(obj_)
		{
			delete obj_;
			obj_ = NULL;
		}
	}

	T* obj_;
};

}

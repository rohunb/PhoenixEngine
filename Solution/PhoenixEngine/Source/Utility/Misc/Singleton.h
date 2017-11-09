#ifndef PHOENIX_SINGLETON_H
#define PHOENIX_SINGLETON_H

namespace Phoenix
{
	template <class T>
	class TSingleton
	{
	public:
		TSingleton(const TSingleton&) = delete;
		TSingleton(TSingleton&&) = delete;
		TSingleton& operator=(const TSingleton&) = delete;
		TSingleton& operator=(TSingleton&&) = delete;

		static T& Get();

	protected:
		TSingleton() = default;

	private:
	};

	template <class T>
	T& TSingleton<T>::Get()
	{
		static T Item;
		return Item;
	}
}

#define F_DeclareTSingleton(Class)				\
	friend class TSingleton<Class>;				\
public:											\
	Class(const Class&) = delete;				\
	Class(Class&&) = delete;					\
	Class& operator=(const Class&) = delete;	\
	Class& operator=(Class&&) = delete;			\
private:										\
	Class() = default;

#endif

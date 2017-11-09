#ifndef PHOENIX_STATIC_OBJECT_H
#define PHOENIX_STATIC_OBJECT_H

/*! \brief Adds a static instance of the class that can be retrieved.
*
*	@desc This can be used in the form of Class::GetStaticObject().
*	@warning This is a single object that can be accessed from any thread.
*/
#define F_AddStaticObjectToClass(Type)	\
public:									\
	static Type& GetStaticObject()		\
	{									\
		static Type StaticObject_;		\
		return StaticObject_;			\
	}									\
private:

#endif

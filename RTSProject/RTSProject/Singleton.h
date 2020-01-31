#pragma once

template<typename T>
class Singleton
{
private:
										Singleton(const Singleton&);
protected:
										Singleton() {}
public:

	static T* GetInstance() 
	{
		static T* instance = nullptr;

		if (instance == nullptr)
		{
			instance = new T;
		}

		return instance; 
	}
};


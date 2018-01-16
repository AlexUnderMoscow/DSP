#include "thread.h"
#include "struct.h"

class ObjectPool 
{
private:
	struct PoolRecord				 //структура записи в векторе объектов
	{
		procThread* instance;
		bool    in_use;
	};

	QObject *obj;					//для передачи классу потока
public:
	std::vector<PoolRecord> m_pool; //вектор записей
    marray** arr;
	unsigned int *size;
    ObjectPool(QObject* o,marray** _arr, unsigned int *_size)			// конструктор - инициализация
	{
		obj = o;
		arr=_arr;
		size=_size;
		unsigned char trdCount = 1;		//количество потоков в пуле по умолчанию
		for (size_t i = 0; i < trdCount; ++i)
		{
			PoolRecord rec;
			rec.in_use = false;	
			rec.instance = new procThread(arr,size);
			m_pool.push_back(rec);
		}

	}
	procThread* createNewObject()			//передача укзателя на свободный объект
	{
		for (size_t i = 0; i < m_pool.size(); ++i)
		{
			if (! m_pool[i].in_use)
			{
				m_pool[i].in_use = true; // переводим объект в список используемых
				return m_pool[i].instance;
			}
		}
		// если не нашли свободный объект, то расширяем пул
		PoolRecord record;
		record.in_use   = true;
		record.instance = new procThread(arr,size);
		m_pool.push_back(record);
		return record.instance;
	}

	void deleteObject(procThread* object)
	{
		// в реальности не удаляем, а лишь помечаем, что объкт свободен
		for (size_t i = 0; i < m_pool.size(); ++i)
		{
			if (m_pool[i].instance == object)
			{
				m_pool[i].in_use = false;
				break;
			}
		}
	}

	 ~ObjectPool()
	{

	}
};

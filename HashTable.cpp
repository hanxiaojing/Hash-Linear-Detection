#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
using namespace std;

#include<string>
//����̽����ػ��������Դ����Զ������͵�����
enum State
{
	EMPTY,
	EXIST,
	DELETE,
};

//�����������
template<class K>
struct DefaultFuncer
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

//�����Զ�������
template<>
struct DefaultFuncer<string>
{
	size_t value = 0;
	size_t operator()(const string& str)
	{
		for (int i = 0; i < str.size(); i++)
		{
			value += str[i];
		}
		return value;
	}	
};


template<class K, template<class>class HashFuncer = DefaultFuncer>
class HashTable
{
public:
	HashTable()
		:_size(0)
		, _capacity(0)
		, _state(NULL)
		, _table(NULL)
	{}

	HashTable(size_t size)
		:_size(0)
		, _capacity(size)
		, _state(new State[size])
		, _table(new K[size])
	{
		for (int i = 0; i < _capacity; i++)//ȫ��״̬��ʼ����EMPTY
		{
			_state[i] = EMPTY;
		}
	}


	//����̽������Ԫ�ش��λ�ã����費��ϣ��ͻ��
	int _HashFunc(const K& key)
	{
		HashFuncer<K> hf;
		return hf(key) % _capacity;

		//�����������operator()
		/*return HashFuncer<K>()(key) % _capacity;*/
	}

	void Swap(HashTable<K> tmp)
	{
		swap(_size, tmp._size);
		swap(_capacity, tmp._capacity);
		swap(_state, tmp._state);
		swap(_table, tmp._table);
	}


	void _CheckCapacity()
	{				
		HashTable<K> tmp(2*_capacity);
		for (int i = 0; i < _capacity; i++)
		{
			tmp.Insert(_table[i]);
		}
		Swap(tmp);
	}
	

	bool Insert(const K& key)
	{
		//��̬��ϣ��
		/*if (_size == _capacity)
		{
			cout<<"HashTable is full!"<<endl;
			return false;
		}*/

		//��̬��ϣ��
		//��Ч��ϣ����غ����Ӵ���ȶ���0.7-0.8�Ϻ�
		if (10 * _size >= 7 * _capacity)
		{
			_CheckCapacity();
		}

		int index = _HashFunc(key);
	
		while (_state[index] == EXIST)
		{		
			index++;
			if (index == _capacity)
			{
				index = 0;
			}
		}

		_table[index] = key;
		_state[index] = EXIST;
		_size++;
		return true;	
	}


	int Find(const K& key)
	{
		int index = _HashFunc(key);
		while (_state[index] == EXIST || _state[index]== DELETE)
		//while(_state[index] != EMPTY)	//��״̬�Ҳ������ǿ�״̬�ҵõ�
		{
			if (_table[index] == key && _state[index] == EXIST)
			{
				return index;
			}
			++index;
			if (index == _capacity)
			{
				index = 0;
			}
		}
		return -1;	
	}


	bool Remove(const K& key)
	{
		int index = Find(key);
		if (index != -1)
		{
			_state[index] = DELETE;
			--_size;
			return true;
		}
		return false;
	}


	void PrintTable()
	{
		for (int i = 0; i < _capacity; i++)
		{
			if (_state[i] == EXIST )
			{
				cout << i << "(EXIST):" << _table[i] << endl;
			}
			/*�ҽ�DELETE״̬Ԫ��Ҳ��ӡ���������ڹ۲졣
			��Insert����ʱ��DELETE״̬�µ�λ�ÿ��Բ����µ�Ԫ��*/
			else if (_state[i] == DELETE)
			{
				cout << i << "(DELETE):" << _table[i] << endl;
			}
			else
			{
				cout << i << "(EMPTY):" << _table[i] << endl;
			}
		}
	}

private:
	size_t _size;//ʵ�ʴ��Ԫ�ظ���
	size_t _capacity;//��ϣ����
	State* _state;
	K* _table;
};


//POD(��������)�Ĳ�������
void TestHashTablePOD()
{
	HashTable<int> ht(10);
	ht.Insert(89);
	ht.Insert(18);
	ht.Insert(49);
	ht.Insert(58);
	ht.Insert(9);
	ht.PrintTable();

	int ret = ht.Find(89);
	cout << ret << endl; 

	ht.Remove(89);
	ht.PrintTable();

	ht.Remove(18);
	ht.PrintTable();
}


//�Զ������͵Ĳ�������
void TestHashTable()
{
	HashTable<string,DefaultFuncer> ht(10);
	ht.Insert("��Ϣ��");
	ht.Insert("ʱ��");
	ht.Insert("����");
	ht.Insert("���Թ���ʦ");
	ht.PrintTable();

	int ret = ht.Find("���Թ���ʦ");
	cout << ret << endl;

	ht.Remove("����");
	ht.PrintTable();

	ht.Remove("ʱ��");
	ht.PrintTable();
}




int main()
{
	TestHashTable();
	system("pause");
	return 0;
}
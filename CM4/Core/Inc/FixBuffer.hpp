#ifndef Fixed_Buffer_h__
#define Fixed_Buffer_h__

#include <string.h>

template <class CType, int CSize>
class FixBuffer
{
public:
	FixBuffer()
	{
		this->buffer_size = CSize + 1 ;

		this->front = 0;
		this->rear = 0;
	}

	// return 1:成功放入 0:失敗 緩充滿了
	int Push(CType iData)
	{
		//printf("[FixBuffer] debug %d\r\n", this->rear);
		if (((this->rear + 1) % this->buffer_size) == this->front)
		{//[Error] The buffer is full
			return 0;
		}
		else
		{
			this->data[this->rear] = iData;
			this->rear = (this->rear + 1) % this->buffer_size;
			return 1;
		}
	}

	// return 1:成功移除 0:失敗 空的
	int Pop(void)
	{
		if (this->rear == this->front)
		{//[Error] The buffer is empty
			return 0;
		}
		else
		{
			this->front = (this->front + 1) % this->buffer_size;
			return 1;
		}
	}

	int GetAmount(void)
	{
		return (this->buffer_size + this->rear - this->front) % this->buffer_size;
	}

	CType GetFront(void)
	{
		return this->datas[this->front];
	}

	CType GetTarget(int index)
	{
		return this->datas[(this->front + index) % this->buffer_size];
	}

	CType* GetData(void)
	{
		if (this->rear >= this->front)
		{
			memcpy(this->sort_data, this->data + this->front, this->rear - this->front);
		}
		else
		{
			int cpy_size_first_half = this->buffer_size - this->front;
			memcpy(this->sort_data, this->data + this->front, cpy_size_first_half); // 複製前半段
			memcpy(this->sort_data + cpy_size_first_half, this->data, this->rear); // 複製後半段
		}
		return this->sort_data;
	}

	int MaxSize(void)
	{
		return this->buffer_size - 1;
	}

	void Clear(void)
	{
		this->front = 0;
		this->rear = 0;
	}
//private:
	int buffer_size;
	int front, rear;

	CType data[CSize + 1];
	CType sort_data[CSize + 1];
};



#endif // Fixed_Buffer_h__

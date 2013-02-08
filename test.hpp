#include <atomic>

void* null_ptr = NULL;

template<unsigned int block_size_t>
class CBlock
{
public:
	CBlock(CBlock<block_size_t>* b) : head(0), tail(0), deleted(false), next(b)
	{
		for (int i = 0; i < block_size_t; i++) nodes[i].store(null_ptr);
	}

	std::atomic<void*> nodes[block_size_t];
	std::atomic<int> head;
	std::atomic<int> tail;
	bool deleted;
	std::atomic<CBlock<block_size_t>*> next;
protected:
private:
};


template<typename node_t, unsigned int block_size_t>
class TQueue
{
public:
	TQueue(void) : null_block(NULL)
	{
		block_t* b = new block_t(null_block);
		queue_head.store(b);
		queue_tail.store(b);
	}

	void enqueue(node_t* item)
	{
		block_t* block = queue_head.load();
		int head = block->head.load();
		for(;;) 
		{
			if (head == block_size_t) 
			{
				block_t* old_block = block;
				block = block->next.load();
				if (block == null_block) 
				{
					block = new block_t(null_block);
					if(old_block->next.compare_exchange_strong(null_block, block))
					{
						queue_head.exchange(block);
					}
					else 
					{
						delete block;
						block = NULL;
						block = old_block->next.load();
					}
				}
				else
				{
					block = queue_head.load();
				}
				head = block->head.load();
			}
			else if(block->nodes[head].load() == null_ptr) 
			{
				if(block->nodes[head].compare_exchange_strong(null_ptr, (void*)item)) 
				{
					block->head++;
					return;
				}
			}
			else head++;
		}
	}
protected:
private:
	typedef CBlock<block_size_t> block_t;

	block_t* null_block;

	std::atomic<block_t*> queue_head;
	std::atomic<block_t*> queue_tail;
};
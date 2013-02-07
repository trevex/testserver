#include <atomic>

void* null_ptr = NULL;

template<unsigned int block_size_t>
class CBlock
{
public:
	CBlock(void) : head(0), tail(0), deleted(false)
	{
		next.store((CBlock*)null_ptr);
		for (int i = 0; i < block_size_t; i++) nodes[i].store(null_ptr);
	}

	std::atomic<void*> nodes[block_size_t];
	int head;
	int tail;
	bool deleted;
	std::atomic<CBlock*> next;
protected:
private:
};


template<unsigned int block_size_t>
struct SQueueState
{
	SQueueState(void) {}
	SQueueState(CBlock<block_size_t>* bh, CBlock<block_size_t>* bt) : block_head(bh), block_tail(bt), node_head(block_head->head), node_tail(block_tail->tail) {}
	CBlock<block_size_t>* block_head;
	CBlock<block_size_t>* block_tail;
	int node_head;
	int node_tail;
};


template<typename node_t, unsigned int block_size_t>
class TQueue
{
public:
	TQueue(void)
	{
		block_t* b = new block_t();
		queue_head.store(b);
		queue_tail.store(b);
	}
	inline CBlock<block_size_t>* loadHead(void) const
	{
		return std::atomic_load(&queue_head); 
	}
	inline CBlock<block_size_t>* loadTail(void) const
	{
		return std::atomic_load(&queue_tail);
	}
	inline SQueueState<block_size_t>* createState(void)
	{
		SQueueState<block_size_t>* state = new SQueueState<block_size_t>(loadHead(), loadTail());
		return state;
	}
	void enqueue(SQueueState<block_size_t>* state, node_t* item)
	{
		int head = state->node_head;
		auto block = state->block_head;
		for(;;) 
		{
			if (head == block_size_t) 
			{
				block_t* old_block = block;
				block->head = head;
				block = std::atomic_load(&block->next);
				if (block == NULL) 
				{
					block = new block_t();
					block_t* head_block = std::atomic_load(&queue_head);
					while (head_block != old_block  &&  std::atomic_load(&old_block->next) == (block_t*)null_ptr) 
					{
						head_block = std::atomic_load(&queue_head);
						if(std::atomic_load(&head_block->next) != old_block) break;
						if(std::atomic_compare_exchange_weak(&queue_head, &head_block, old_block)) break;
					}
					if(std::atomic_compare_exchange_strong(&old_block->next, (block_t**)&null_ptr, block))
					{
						std::atomic_exchange(&queue_head, block);
					}
					else 
					{
						delete block;
						block = NULL;
						block = std::atomic_load(&old_block->next);
					}
				}
				else if(block->head == block_size_t && std::atomic_load(&block->next) != NULL)
				{
					block = std::atomic_load(&queue_head);
				}
				state->block_head = block;
				head = block->head;
			}
			else if(std::atomic_load(&block->nodes[head]) == null_ptr) 
			{
				if(std::atomic_compare_exchange_strong(&block->nodes[head], &null_ptr, (void*)item)) 
				{
					state->node_head = head+1;
					return;
				}
			}
			else head++;
		}
	}
protected:
private:
	typedef CBlock<block_size_t> block_t;

	std::atomic<block_t*> queue_head;
	std::atomic<block_t*> queue_tail;
};
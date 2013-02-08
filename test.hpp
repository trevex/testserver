#include <atomic>

void* null_ptr = NULL;

template<unsigned int block_size_t>
class CBlock
{
public:
	CBlock(CBlock<block_size_t>* b) : head(0), tail(0), deleted(false)
	{
		next.store(b);
		for (int i = 0; i < block_size_t; i++) nodes[i].store(null_ptr);
	}

	std::atomic<void*> nodes[block_size_t];
	int head;
	int tail;
	bool deleted;
	std::atomic<CBlock<block_size_t>*> next;
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
	TQueue(void) : null_block(NULL)
	{
		block_t* b = new block_t(null_block);
		queue_head.store(b);
		queue_tail.store(b);
	}
	
	SQueueState<block_size_t>* createState(void)
	{
		SQueueState<block_size_t>* state = new SQueueState<block_size_t>(queue_head.load(), queue_tail.load());
		return state;
	}

	void enqueue(SQueueState<block_size_t>* state, node_t* item)
	{
		int head = state->node_head;
		block_t* block = state->block_head;
		for(;;) 
		{
			if (head == block_size_t) 
			{
				block_t* old_block = block;
				block->head = head;
				block = block->next.load();
				if (block == NULL) 
				{
					block = new block_t(null_block);
					//while (queue_head.load() != old_block  &&  old_block->next.load() == NULL) 
					//{
					//	block_t* head_block = queue_head.load();
					//	if(head_block->next.load() != old_block) break;
					//	if(queue_head.compare_exchange_strong(head_block, old_block)) break;
					//}
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
				else if(block->head == block_size_t && block->next.load() != null_block)
				{
					block = queue_head.load();
				}
				state->block_head = block;
				head = block->head;
			}
			else if(block->nodes[head].load() == null_ptr) 
			{
				if(block->nodes[head].compare_exchange_strong(null_ptr, (void*)item)) 
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

	block_t* null_block;

	std::atomic<block_t*> queue_head;
	std::atomic<block_t*> queue_tail;
};
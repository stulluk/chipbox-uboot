/*
// Author: Jiasheng.chen@celestialsemi.com
// Version0.1 Inital @2008.02.15
*/

#include "x_clib.h"

//#define DEBUG_ENA

enum _MALLOC_CFG_
{
	M_HEAP_SIZE = 1024 * 8,
	M_TAG_SIZE  = 32,
	M_BYTE_ALIGNED_SIZE = 4, //32Bit Aligned, Start Address and Size Aligned Both
};
typedef struct _MALLOC_TAG_
{
	unsigned char *ptr;
	int size;
	int is_alloc;
}mem_tag;


static unsigned char _MemHeap[M_HEAP_SIZE];
static mem_tag _MemTags[M_TAG_SIZE];
static int _HeadIdx = -1;

#ifdef DEBUG_ENA
#include <assert.h>
#include <stdio.h>

#define MALLOC_TRACE(Type) MallocTrace(Type)
#define MTRACE_FILE "malloc_trace.log"

int MallocTrace( int IsMallocOrFree)
{
	static FILE *fp = NULL;
	static int MallocCnt = 0;
	static int FreeCnt   = 0;
	char TypeStrM[] = "M";
	char TypeStrF[]	= "F";
	char *Title = NULL;
	int cnt = 0;
	if (fp == NULL)
		fp = fopen(MTRACE_FILE, "w+"); 
	assert(fp);
	if (IsMallocOrFree)
	{	
		Title = TypeStrM;
		MallocCnt++;
		cnt = MallocCnt;		
		//fprintf(fp, "malloc(%4d): _HeadIdx (%d)\n", MallocCnt++, _HeadIdx);
	}
	else
	{
		Title = TypeStrF;		
		FreeCnt++;
		cnt = FreeCnt;
		//fprintf(fp, "Free  (%4d): _HeadIdx (%d)\n",   FreeCnt++, _HeadIdx);
	}
	
	{
		int i = 0;
		fprintf(fp, "%2s(%4d)-%10s :", Title,cnt, "is_alloc");
		for (i = 0; i < M_TAG_SIZE; i++)
		{
			fprintf(fp, "%4x ",_MemTags[i].is_alloc);
		}
		fprintf(fp, "\n");
		fprintf(fp, "%2s(%4d)-%10s :", Title,cnt, "Size");
		for (i = 0; i < M_TAG_SIZE; i++)
		{
			fprintf(fp, "%4x ",_MemTags[i].size & 0xFFFF);
		}
		fprintf(fp, "\n");
		fprintf(fp, "%2s(%4d)-%10s :", Title,cnt, "OffSet");
		for (i = 0; i < M_TAG_SIZE; i++)
		{
			int OffSet = 0;
			if(_MemTags[i].ptr != NULL)

				OffSet = _MemTags[i].ptr - _MemHeap;
			else
				OffSet = i | 0xFF00;
				
			fprintf(fp, "%04X ",OffSet & 0xFFFF);
		}
		fprintf(fp, "\n");
		
	}
	return (int)fp;
}
#else
#define assert(exp) do{}while(0)
#define MALLOC_TRACE(Type) do{}while(0)
#endif


static int x_m_init()
{
	int i = 0;
	for (i = 0; i < M_TAG_SIZE; i++)
	{
		_MemTags[i].is_alloc = 0;
		_MemTags[i].size     = 0;
		_MemTags[i].ptr      = NULL;
	}
	_MemTags[0].is_alloc = 0;
	_MemTags[0].size     = M_HEAP_SIZE;
	_MemTags[0].ptr      = _MemHeap;
	_HeadIdx = 0;
	return 1;
}

static int x_m_find_empty_tag()
{
	int i = 0;
	for (i = 0; i < M_TAG_SIZE; i++)
	{
		int idx = 0;
		idx = (_HeadIdx + i) % M_TAG_SIZE;
		if (_MemTags[idx].size == 0)
		{
			assert(_MemTags[idx].is_alloc == 0 &&
				   _MemTags[idx].ptr      == NULL);
			return idx;
		}
	}
	assert(i < M_TAG_SIZE); //No Empty Tags
	return -1;
}

static int x_m_find_fit_tag(int size)
{
	int i = 0;
	for (i = 0; i < M_TAG_SIZE; i++)
	{
		int idx = 0;
		idx = (_HeadIdx + i) % M_TAG_SIZE;
		if (_MemTags[idx].size >= size &&
			_MemTags[idx].is_alloc == 0)
		{
			assert( _MemTags[idx].ptr >= _MemHeap && 
				    _MemTags[idx].ptr <  _MemHeap + M_HEAP_SIZE);
			return idx;
		}
	}
	assert(i < M_TAG_SIZE); //No Valid Size Tags
	return -1;
}
static int x_m_find_tag(unsigned char *ptr)
{
	int i = 0;
	for (i = 0; i < M_TAG_SIZE; i++)
	{
		int idx = 0;
		idx = (_HeadIdx + M_TAG_SIZE - i) % M_TAG_SIZE;
		if (_MemTags[idx].is_alloc == 1 &&
			_MemTags[idx].ptr <= ptr    &&
			ptr < _MemTags[idx].ptr + _MemTags[idx].size)
		{
			return idx;
		}
	}
	assert(i < M_TAG_SIZE); //Can not Find the ReqMem
	return -1;	
}
static int x_m_merg_tag(int free_idx)
{
	int i = 0;
	mem_tag *tag = NULL;
	assert(!(free_idx < 0 || free_idx >= M_TAG_SIZE));
	if (free_idx < 0 || free_idx >= M_TAG_SIZE)
		return -1;
	tag = &_MemTags[free_idx];
	if (tag->is_alloc) return -1;
	assert(tag->size > 0);
	if (tag->size == 0) return -1;

	//Find Pos
	do
	{
		for (i = 0; i < M_TAG_SIZE; i++)
		{
			int idx = 0;
			idx = (_HeadIdx + M_TAG_SIZE - i) % M_TAG_SIZE;
			if (_MemTags[idx].is_alloc == 0 &&
				_MemTags[idx].size      > 0)
			{
				if (_MemTags[idx].ptr == tag->ptr + tag->size ) //Can Merge
				{
					if (_MemTags[idx].size >= tag->size)
					{
						_MemTags[idx].size += tag->size;
						_MemTags[idx].ptr   = tag->ptr;
						tag->is_alloc = 0;
						tag->size     = 0;
						tag->ptr      = NULL;
						tag = &_MemTags[idx];
					}
					else
					{
						tag->size += _MemTags[idx].size;
						_MemTags[idx].is_alloc = 0;
						_MemTags[idx].size = 0;
						_MemTags[idx].ptr  = NULL;
					}
					break;
				}
			}
		}
	}while(i < M_TAG_SIZE);
	//Find Pre
	do
	{
		for (i = 0; i < M_TAG_SIZE; i++)
		{
			int idx = 0;
			idx = (_HeadIdx + M_TAG_SIZE - i) % M_TAG_SIZE;
			if (_MemTags[idx].is_alloc == 0 &&
				_MemTags[idx].size      > 0)
			{
				if (_MemTags[idx].ptr + _MemTags[idx].size == tag->ptr) //Can Merge
				{
					if (_MemTags[idx].size >= tag->size)
					{
						_MemTags[idx].size += tag->size;
						tag->is_alloc = 0;
						tag->size     = 0;
						tag->ptr      = NULL;
						tag = &_MemTags[idx];
					}
					else
					{
						tag->ptr   = _MemTags[idx].ptr;
						tag->size += _MemTags[idx].size;
						_MemTags[idx].is_alloc = 0;
						_MemTags[idx].size = 0;
						_MemTags[idx].ptr  = NULL;						
					}
					break;
				}
			}
		}
	}while(i < M_TAG_SIZE);	
	return tag - _MemTags;
}

void* x_malloc(int size)
{
	int FitIdx = 0;
	int EmptyIdx = 0;
	//Initial
	if (_HeadIdx < 0) //First Call
	{
		x_m_init();
	}
	if (size <= 0) return NULL;
	size = ((size + M_BYTE_ALIGNED_SIZE - 1) / M_BYTE_ALIGNED_SIZE) * M_BYTE_ALIGNED_SIZE;
	EmptyIdx = x_m_find_empty_tag(size);
	if (EmptyIdx < 0 || EmptyIdx >= M_TAG_SIZE) return NULL;
	FitIdx = x_m_find_fit_tag(size);
	if (FitIdx < 0 || FitIdx >= M_TAG_SIZE) return NULL;
	_MemTags[EmptyIdx].is_alloc = 1;
	_MemTags[EmptyIdx].size     = size;
	_MemTags[EmptyIdx].ptr      = _MemTags[FitIdx].ptr;

	_MemTags[FitIdx].ptr  += size;
	_MemTags[FitIdx].size -= size;
	assert(_MemTags[FitIdx].size >= 0);
	if (_MemTags[FitIdx].size <= 0)
	{//Make Empty
		_MemTags[FitIdx].is_alloc = 0;
		_MemTags[FitIdx].size = 0;
	}
	//Update the HeadIdx;
	_HeadIdx = FitIdx;
	MALLOC_TRACE(1);
	return (void *)_MemTags[EmptyIdx].ptr;
}
void  x_free(void *ptr)
{
	int FreeIdx = 0;
	assert(ptr != NULL);
	if (ptr == NULL) return;
	FreeIdx = x_m_find_tag((unsigned char *) ptr);
	if (FreeIdx < 0 || FreeIdx >= M_TAG_SIZE) return;
	_MemTags[FreeIdx].is_alloc = 0;
	x_m_merg_tag(FreeIdx);
	MALLOC_TRACE(0);
	return;
}

#include <os.h>

extern "C" {
		
	void *ksbrk(int n)
	{
		struct kmalloc_header *chunk;
		char *p_addr;
		int i;

		if ((kern_heap + (n * PAGESIZE)) > (char *) KERN_HEAP_LIM) {
			io.print
			    ("PANIC: ksbrk(): no virtual memory left for kernel heap !\n");
			return (char *) -1;
		}

		chunk = (struct kmalloc_header *) kern_heap;

		for (i = 0; i < n; i++) {
			p_addr = get_page_frame();
			if ((int)(p_addr) < 0) {
				io.print
				    ("PANIC: ksbrk(): no free page frame available !\n");
				return (char *) -1;
			}

			pd0_add_page(kern_heap, p_addr, 0);

			kern_heap += PAGESIZE;
		}

		chunk->size = PAGESIZE * n;
		chunk->used = 0;

		return chunk;
	}

	void *kmalloc(unsigned long size)
	{
		if (size==0)
			return 0;
			
		unsigned long realsize;
		struct kmalloc_header *chunk, *other;

		if ((realsize =
		     sizeof(struct kmalloc_header) + size) < KMALLOC_MINSIZE)
			realsize = KMALLOC_MINSIZE;

		chunk = (struct kmalloc_header *) KERN_HEAP;
		while (chunk->used || chunk->size < realsize) {
			if (chunk->size == 0) {
				io.print
				    ("\nPANIC: kmalloc(): corrupted chunk on %x with null size (heap %x) !\nSystem halted\n",
				     chunk, kern_heap);
					 //error
					 asm("hlt");
					 return 0;
			}

			chunk =
			    (struct kmalloc_header *) ((char *) chunk +
						       chunk->size);

			if (chunk == (struct kmalloc_header *) kern_heap) {
				if ((int)(ksbrk((realsize / PAGESIZE) + 1)) < 0) {
					io.print
					    ("\nPANIC: kmalloc(): no memory left for kernel !\nSystem halted\n");
					 asm("hlt");
					return 0;
				}
			} else if (chunk > (struct kmalloc_header *) kern_heap) {
				io.print
				    ("\nPANIC: kmalloc(): chunk on %x while heap limit is on %x !\nSystem halted\n",
				     chunk, kern_heap);
				 asm("hlt");
				return 0;
			}
		}

		if (chunk->size - realsize < KMALLOC_MINSIZE)
			chunk->used = 1;
		else {
			other =
			    (struct kmalloc_header *) ((char *) chunk + realsize);
			other->size = chunk->size - realsize;
			other->used = 0;

			chunk->size = realsize;
			chunk->used = 1;
		}

		kmalloc_used += realsize;

		/* Return a pointer to the memory area */
		return (char *) chunk + sizeof(struct kmalloc_header);
	}

	/* free memory block */
	void kfree(void *v_addr)
	{
		if (v_addr==(void*)0)
			return;
			
		struct kmalloc_header *chunk, *other;

		chunk =
		    (struct kmalloc_header *) ((u32)v_addr -
					       sizeof(struct kmalloc_header));
		chunk->used = 0;

		kmalloc_used -= chunk->size;

		while ((other =
			(struct kmalloc_header *) ((char *) chunk + chunk->size))
		       && other < (struct kmalloc_header *) kern_heap
		       && other->used == 0)
			chunk->size += other->size;
	}
}

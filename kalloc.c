// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
  uint pgrefcount[PHYSTOP >> PTXSHIFT];
  uint num_free_pages;
} kmem;

/*
HW4 step 1. getNumFreePages syscall
*/

int
getNumFreePages(void){
  return kmem.num_free_pages;
}
// HW4 step 2. reference Counter

// get page Reference count for Physical Address
uint 
get_refcount(uint pa)
{
  return kmem.pgrefcount[pa>>PTXSHIFT];
}
void
inc_refcount(uint pa)
{
  if(kmem.use_lock)
      acquire(&kmem.lock);
  kmem.pgrefcount[pa>>PTXSHIFT] += 1;
  if(kmem.use_lock)
      release(&kmem.lock);
}

void
dec_refcount(uint pa)
{
  if(kmem.use_lock)
      acquire(&kmem.lock);
  kmem.pgrefcount[pa>>PTXSHIFT] -= 1;
  if(kmem.use_lock)
      release(&kmem.lock);
}

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  kmem.num_free_pages = 0;
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE) {
    kfree(p);
    kmem.pgrefcount[V2P(p)>>PTXSHIFT] = 0;
  }
}
//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");

  if(get_refcount(V2P(v)) > 0)
    dec_refcount(V2P(v));
  
  // HW4 Step 2. if no proc reference page v
  if(get_refcount(V2P(v)) == 0) {
    // Fill with junk to catch dangling refs.
    memset(v, 1, PGSIZE);
    // HW4. Step 1.
    kmem.num_free_pages++;
    //
    if(kmem.use_lock)
      acquire(&kmem.lock);
    r = (struct run*)v;
    r->next = kmem.freelist;
    kmem.freelist = r;
    if(kmem.use_lock)
      release(&kmem.lock);
  }
  
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

  if(kmem.use_lock)
    acquire(&kmem.lock);
  //HW4. Step 1.
  
  r = kmem.freelist;
  
  if(r) {
    kmem.freelist = r->next;
    // HW4 Step 2.
    kmem.pgrefcount[V2P(r) >> PTXSHIFT] = 1;
    kmem.num_free_pages--;
  }
  if(kmem.use_lock)
    release(&kmem.lock);
  return (char*)r;
}


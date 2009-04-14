/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2007  Paul Baecher & Markus Koetter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * 
 *             contact nepenthesdev@users.sourceforge.net  
 *
 *******************************************************************************/


#ifndef HAVE_EMU_LIST_H
#define HAVE_EMU_LIST_H


#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>


/* credit http://dotat.at/prog/lists/list.h
 * $Copyright: (C) 2001, 2002 Tony Finch <dot@dotat.at> $
 */


#define xalloc malloc
#define xfree(ptr, size) free(ptr)



#define list__magic(ptr, t_src, src_field, t_dst, dst_field)		\
	((t_dst *)(	(char *)(ptr)					\
			+ offsetof(t_src, src_field)			\
			- offsetof(t_dst, dst_field)	))

#define header_list_types(root_tag, elem_tag, link_tag)			\
									\
struct link_tag { struct elem_tag *next, *prev; };			\
struct root_tag { struct link_tag  head,  tail; };			\
									\
struct list__hack


#define header_list_typedefs(t_root, t_elem, t_link)		\
													\
header_list_types(t_root, t_elem, t_link);					\
													\
typedef struct t_root t_root;						\
typedef struct t_elem t_elem;						\
typedef struct t_link t_link;						\
													\
struct list__hack


#define header_list_functions(name, t_root, t_elem, link) \
 \
/* internal pointer magic utilities */ \
/* static inline */ t_elem *name##__head2elem(t_root *root); \
/* static inline */ t_elem *name##__tail2elem(t_root *root); \
/* static inline */ t_root *name##__elem2head(t_elem *elem); \
/* static inline */ t_root *name##__elem2tail(t_elem *elem); \
 \
/* initializers */							 \
/* static inline */ void name##_init(t_root *root); \
/* static inline */ void name##_init_link(t_elem *elem); \
									 \
/* accessor functions */				 \
/* static inline */ t_elem *name##_first(t_root *root); \
/* static inline */ t_elem *name##_last(t_root *root); \
/* static inline */ t_elem *name##_next(t_elem *elem); \
/* static inline */ t_elem *name##_prev(t_elem *elem); \
 \
	/* predicates */							 \
/* static inline */ bool name##_empty(t_root *root); \
/* static inline */ bool name##_iselem(t_elem *elem); \
/* static inline */ bool name##_ishead(t_elem *elem); \
/* static inline */ bool name##_istail(t_elem *elem); \
/* static inline */ bool name##_isunlinked(t_elem *elem); \
 \
/* faster but less safe predicates for looping */			 \
/* static inline */ bool name##_athead(t_elem *elem); \
/* static inline */ bool name##_attail(t_elem *elem); \
 \
/* slightly more complicated predicates */				 \
/* static inline */ bool name##_isfirst(t_elem *elem); \
/* static inline */ bool name##_islast(t_elem *elem); \
 \
/* internal list manipulation */					 \
/* static inline */ void name##__splice_before(t_elem *elem, t_elem *newX0, t_elem *newXN); \
/* static inline */ void name##__splice_after(t_elem *elem, t_elem *newX0, t_elem *newXN); \
/* static inline */ void name##__unsplice(t_elem *old0, t_elem *oldN); \
									 \
/* single element manipulation */		 \
/* static inline */ void name##_insert_before(t_elem *elem, t_elem *newX); \
/* static inline */ void name##_insert_after(t_elem *elem, t_elem *newX); \
/* static inline */ void name##_insert_first(t_root *root, t_elem *newX); \
/* static inline */ void name##_insert_last(t_root *root, t_elem *newX); \
/* static inline */ void name##_remove(t_elem *elem); \
/* static inline */ t_elem *name##_remove_first(t_root *root); \
/* static inline */ t_elem *name##_remove_last(t_root *root); \
/* static inline */ void name##_remove_next(t_elem *elem); \
/* static inline */ void name##_remove_prev(t_elem *elem); \
									 \
/* concatenation etc. */				 \
/* static inline */ void name##_concat(t_root *dst, t_root *src); \
									 \
/* safe magic for the user */			 \
/* static inline */ t_root *name##_getroot(t_elem *elem); \
									 \
/* looping operations */						 \
/* static inline */ t_root *name##_findroot_fwd(t_elem *elem); \
/* static inline */ t_root *name##_findroot_rev(t_elem *elem); \
/* static inline */ int name##_length(t_root *root); \
/* static */ void name##_qsort_r(t_root *root, void *arg, int (*cmp)(void *, t_elem *, t_elem *)); \
/* static */ int name##_qsort_cmp(void *a, t_elem *e1, t_elem *e2); \
/* static inline */ void name##_qsort(t_root *root, int (*cmp)(t_elem *, t_elem *)); \
/* static inline */ t_root *name##_create(void); \
/* static inline */ void name##_destroy(t_root *root); \
struct list__hack






/*!
 * \brief Define a list's operations.
 *
 * \param name   The name of the list
 *               (written NAME in the function documentation).
 * \param t_root The name of the type of the list's root.
 * \param t_elem The name of the type of the list's elements.
 * \param link   The name of the link structure in each element.
 *
 * \hideinitializer
 */
#define source_list_functions(name, t_root, t_elem, link)			\
									\
/* internal pointer magic utilities */					\
/* static inline */ t_elem *name##__head2elem(t_root *root) {			\
	return(list__magic(root, t_root, head, t_elem, link));		\
}									\
/* static inline */ t_elem *name##__tail2elem(t_root *root) {			\
	return(list__magic(root, t_root, tail, t_elem, link));		\
}									\
/* static inline */ t_root *name##__elem2head(t_elem *elem) {			\
	return(list__magic(elem, t_elem, link, t_root, head));		\
}									\
/* static inline */ t_root *name##__elem2tail(t_elem *elem) {			\
	return(list__magic(elem, t_elem, link, t_root, tail));		\
}									\
									\
/* initializers */							\
/* static inline */ void name##_init(t_root *root) {				\
	root->head.next = name##__tail2elem(root);			\
	root->head.prev = root->tail.next = NULL;			\
	root->tail.prev = name##__head2elem(root);			\
}									\
/* static inline */ void name##_init_link(t_elem *elem) {			\
	elem->link.next = elem->link.prev = NULL;			\
}									\
									\
/* accessor functions */						\
/* static inline */ t_elem *name##_first(t_root *root) {			\
	return(root->head.next);					\
}									\
/* static inline */ t_elem *name##_last(t_root *root) {			\
	return(root->tail.prev);					\
}									\
/* static inline */ t_elem *name##_next(t_elem *elem) {			\
	return(elem->link.next);					\
}									\
/* static inline */ t_elem *name##_prev(t_elem *elem) {			\
	return(elem->link.prev);					\
}									\
									\
/* predicates */							\
/* static inline */ bool name##_empty(t_root *root) {				\
	return(root->head.next->link.next == NULL);			\
}									\
/* static inline */ bool name##_iselem(t_elem *elem) {			\
	return(elem->link.next != NULL && elem->link.prev != NULL);	\
}									\
/* static inline */ bool name##_ishead(t_elem *elem) {			\
	return(elem->link.next != NULL && elem->link.prev == NULL);	\
}									\
/* static inline */ bool name##_istail(t_elem *elem) {			\
	return(elem->link.next == NULL && elem->link.prev != NULL);	\
}									\
/* static inline */ bool name##_isunlinked(t_elem *elem) {			\
	return(elem->link.next == NULL && elem->link.prev == NULL);	\
}									\
/* faster but less safe predicates for looping */			\
/* static inline */ bool name##_athead(t_elem *elem) {			\
	return(elem->link.prev == NULL);				\
}									\
/* static inline */ bool name##_attail(t_elem *elem) {			\
	return(elem->link.next == NULL);				\
}									\
/* slightly more complicated predicates */				\
/* static inline */ bool name##_isfirst(t_elem *elem) {			\
	return(name##_athead(name##_prev(elem)));			\
}									\
/* static inline */ bool name##_islast(t_elem *elem) {			\
	return(name##_attail(name##_next(elem)));			\
}									\
									\
/* internal list manipulation */					\
/* static inline */ void name##__splice_before				\
		(t_elem *elem, t_elem *newX0, t_elem *newXN) {		\
	newX0->link.prev = elem->link.prev;				\
	newXN->link.next = elem;						\
	elem->link.prev->link.next = newX0;				\
	elem->link.prev = newXN;						\
}									\
/* static inline */ void name##__splice_after					\
		(t_elem *elem, t_elem *newX0, t_elem *newXN) {		\
	newXN->link.next = elem->link.next;				\
	newX0->link.prev = elem;						\
	elem->link.next->link.prev = newXN;				\
	elem->link.next = newX0;						\
}									\
/* static inline */ void name##__unsplice(t_elem *old0, t_elem *oldN) {	\
	old0->link.prev->link.next = oldN->link.next;			\
	oldN->link.next->link.prev = old0->link.prev;			\
}									\
									\
/* single element manipulation */					\
/* static inline */ void name##_insert_before(t_elem *elem, t_elem *newX) {	\
	name##__splice_before(elem, newX, newX);				\
}									\
/* static inline */ void name##_insert_after(t_elem *elem, t_elem *newX) {	\
	name##__splice_after(elem, newX, newX);				\
}									\
/* static inline */ void name##_insert_first(t_root *root, t_elem *newX) {	\
	name##_insert_before(root->head.next, newX);			\
	/* or name##_insert_after(name##__head2elem(root), newX); */	\
}									\
/* static inline */ void name##_insert_last(t_root *root, t_elem *newX) {	\
	name##_insert_after(root->tail.prev, newX);			\
	/* or name##_insert_before(name##__tail2elem(root), newX); */	\
}									\
/* static inline */ void name##_remove(t_elem *elem) {			\
	name##__unsplice(elem, elem);					\
	name##_init_link(elem);						\
}									\
/* static inline */ t_elem *name##_remove_first(t_root *root) {		\
	t_elem *elem = name##_first(root);				\
	if(name##_attail(elem)) return(NULL);				\
	name##_remove(elem);						\
	return(elem);							\
}									\
/* static inline */ t_elem *name##_remove_last(t_root *root) {		\
	t_elem *elem = name##_last(root);				\
	if(name##_athead(elem)) return(NULL);				\
	name##_remove(elem);						\
	return(elem);							\
}									\
/* static inline */ void name##_remove_next(t_elem *elem) {			\
	elem = name##_next(elem);					\
	if(!name##_attail(elem)) name##_remove(elem);			\
}									\
/* static inline */ void name##_remove_prev(t_elem *elem) {			\
	elem = name##_prev(elem);					\
	if(!name##_athead(elem)) name##_remove(elem);			\
}									\
									\
/* concatenation etc. */						\
/* static inline */ void name##_concat(t_root *dst, t_root *src) {		\
	t_elem *elem0 = name##_first(src);				\
	t_elem *elemN = name##_last(src);				\
	if(name##_attail(elem0)) return;				\
	name##__unsplice(elem0, elemN);					\
	name##__splice_after(dst->tail.prev, elem0, elemN);		\
}									\
									\
/* safe magic for the user */						\
/* static inline */ t_root *name##_getroot(t_elem *elem) {			\
	if(name##_ishead(elem))	return(name##__elem2head(elem));	\
	if(name##_istail(elem))	return(name##__elem2tail(elem));	\
	return(NULL);							\
}									\
									\
/* looping operations */						\
/* static inline */ t_root *name##_findroot_fwd(t_elem *elem) {		\
	while(!name##_attail(elem))					\
		elem = name##_next(elem);				\
	return(name##__elem2tail(elem));				\
}									\
/* static inline */ t_root *name##_findroot_rev(t_elem *elem) {		\
	while(!name##_athead(elem))					\
		elem = name##_prev(elem);				\
	return(name##__elem2head(elem));				\
}									\
/* static inline */ int name##_length(t_root *root) {				\
	t_elem *elem; int i;						\
	for(elem = name##_first(root), i = 0;				\
	    !name##_attail(elem);					\
	    elem = name##_next(elem), i++);				\
	return(i);							\
}									\
/* static */ void name##_qsort_r(t_root *root, void *arg,			\
		int (*cmp)(void *, t_elem *, t_elem *)) {		\
	t_elem *pivot, *elem;						\
	t_root one, two, three;						\
	int c;								\
									\
	name##_init(&one);						\
	name##_init(&two);						\
	name##_init(&three);						\
									\
	pivot = name##_remove_first(root);				\
	if(pivot == NULL) return;					\
	name##_insert_last(&two, pivot);				\
									\
	while(elem = name##_remove_first(root), elem != NULL) {		\
		c = cmp(arg, pivot, elem);				\
		if(c > 0)						\
			name##_insert_last(&one, elem);			\
		else							\
		if(c < 0)						\
			name##_insert_last(&three, elem);		\
		else							\
			name##_insert_last(&two, elem);			\
	}								\
	name##_qsort_r(&one, arg, cmp);					\
	name##_qsort_r(&three, arg, cmp);				\
	name##_concat(root, &one);					\
	name##_concat(root, &two);					\
	name##_concat(root, &three);					\
}									\
/* static */ int name##_qsort_cmp(void *a, t_elem *e1, t_elem *e2) {		\
	/* function pointers can't be portably cast to void pointers */	\
	int (**arg)(t_elem *, t_elem *) = a;				\
	int (*cmp)(t_elem *, t_elem *) = *arg;				\
	return(cmp(e1,e2));						\
}									\
/* static inline */ void name##_qsort						\
		(t_root *root, int (*cmp)(t_elem *, t_elem *)) {	\
	name##_qsort_r(root, &cmp, name##_qsort_cmp);			\
}									\
									\
/* memory handling */							\
/* static inline */ t_root *name##_create(void) {				\
	t_root *root = xalloc(sizeof(*root));				\
	name##_init(root);						\
	return(root);							\
}									\
/* static inline */ void name##_destroy(t_root *root) {			\
	t_elem *elem;							\
	if(root == NULL) return;					\
	while(elem = name##_remove_first(root), elem != NULL)		\
		xfree(elem, sizeof(*elem));				\
	xfree(root, sizeof(*root));					\
}									\
									\
struct list__hack



/* emu_list_* */

header_list_typedefs(emu_list_root,emu_list_item,emu_list_link);

struct emu_list_item
{
	union
	{
		void 		*data;
		uint32_t 	uint32;
		int32_t 	int32;
		char 		*str;
	};

	emu_list_link link;
};

header_list_functions(emu_list, emu_list_root, emu_list_item, link);
struct emu_list_item *emu_list_item_create(void);

#endif // HAVE_EMU_LIST_H

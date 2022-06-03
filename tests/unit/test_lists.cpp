/*
 * test_lists.cpp
 * 
 * Created: 04.06.2022 12:45:03
 * Author: ThePetrovich
 */


#ifdef __INTELLISENSE__
	#pragma diag_suppress 384
#endif

#include <CppUTest/TestHarness.h>
#include <kernel/kernel.h>
#include <kernel/common/lists.h>

TEST_GROUP(TestLists) 
{
	void setup() 
	{
		// This gets run before every test
	}

	void teardown() 
	{
		// This gets run after every test
	}
};

size_t test_countListItems(kLinkedList_t *list)
{
	kLinkedListItem_t *head = list->head;
	size_t count = 0;
			
	while (head != NULL) {
		head = head->next;
		count++;
	}

	return count;
}

TEST(TestLists, Test_ListAddFrontBack) 
{
	kLinkedList_t list;
	kLinkedListItem_t items[10];
	int data[10] = {0};

	list.head = NULL;
	list.tail = NULL;

	/* Setting up linked list */
	for (int i = 0; i < 10; i++) {
		data[i] = i;
		items[i].list = NULL;
		items[i].next = NULL;
		items[i].prev = NULL;
		items[i].data = (void *)&data[i];
	}

	/* Add item to back, check if list is 1 item long */
	common_listAddBack(&list, &items[0]);

	CHECK_EQUAL(1, test_countListItems(&list));

	/* Check if head & tail are set correctly */
	CHECK_EQUAL(&items[0], list.head);
	CHECK_EQUAL(&items[0], list.tail);

	/* Add item to back, check if list is 2 items long */
	common_listAddBack(&list, &items[1]);

	CHECK_EQUAL(2, test_countListItems(&list));

	/* Check if head & tail are set correctly */
	CHECK_EQUAL(&items[0], list.head);
	CHECK_EQUAL(&items[1], list.tail);

	/* Add item to front */
	common_listAddFront(&list, &items[2]);

	/* Check if head & tail are set correctly */
	CHECK_EQUAL(&items[2], list.head);
	CHECK_EQUAL(&items[1], list.tail);

	/* Add item to front */
	common_listAddFront(&list, &items[3]);

	/* Check if head & tail are set correctly */
	CHECK_EQUAL(&items[3], list.head);
	CHECK_EQUAL(&items[1], list.tail);

	/* Add item to back, check if list is 5 items long */
	common_listAddBack(&list, &items[4]);

	CHECK_EQUAL(5, test_countListItems(&list));

	/* Check if head & tail are set correctly */
	CHECK_EQUAL(&items[3], list.head);
	CHECK_EQUAL(&items[4], list.tail);
}

TEST(TestLists, Test_ListDropFrontBack) 
{
	kLinkedList_t list;
	kLinkedListItem_t items[10];
	int data[10] = {0};

	list.head = NULL;
	list.tail = NULL;

	/* Setting up linked list and adding 10 items */
	for (int i = 0; i < 10; i++) {
		data[i] = i;
		items[i].list = NULL;
		items[i].next = NULL;
		items[i].prev = NULL;
		items[i].data = (void *)&data[i];

		common_listAddBack(&list, &items[i]);
	}

	CHECK_EQUAL(10, test_countListItems(&list));

	/* Drop item from back & check if list is 9 items long*/
	common_listDropBack(&list);

	CHECK_EQUAL(9, test_countListItems(&list));
	
	/* Check if head & tail are set correctly */
	CHECK_EQUAL(&items[0], list.head);
	CHECK_EQUAL(&items[8], list.tail);

	/* Drop item from front & check if list is 8 items long*/
	common_listDropFront(&list);

	CHECK_EQUAL(8, test_countListItems(&list));

	/* Check if head & tail are set correctly */
	CHECK_EQUAL(&items[1], list.head);
	CHECK_EQUAL(&items[8], list.tail);

	/* Drop 4 items from front & check if list is 4 items long*/
	for (int i = 0; i < 4; i++) {
		common_listDropFront(&list);
	}

	CHECK_EQUAL(4, test_countListItems(&list));

	/* Check if head & tail are set correctly */
	CHECK_EQUAL(&items[5], list.head);
	CHECK_EQUAL(&items[8], list.tail);

	/* Drop 4 items from back & check if list is empty */
	for (int i = 0; i < 4; i++) {
		common_listDropBack(&list);
	}

	CHECK_EQUAL(0, test_countListItems(&list));

	/* Check if head & tail are NULL as should be in an empty list */
	CHECK_EQUAL(NULL, list.head);
	CHECK_EQUAL(NULL, list.tail);

	/* Re-add 10 items to the list */
	for (int i = 0; i < 10; i++) {
		data[i] = i;
		items[i].list = NULL;
		items[i].next = NULL;
		items[i].prev = NULL;
		items[i].data = (void *)&data[i];

		common_listAddBack(&list, &items[i]);
	}

	CHECK_EQUAL(10, test_countListItems(&list));

	/* Drop the entire list from the front & check if it is empty*/
	for (int i = 0; i < 10; i++) {
		common_listDropFront(&list);
	}

	CHECK_EQUAL(0, test_countListItems(&list));

	/* Check if head & tail are NULL as should be in an empty list */
	CHECK_EQUAL(NULL, list.head);
	CHECK_EQUAL(NULL, list.tail);
}

TEST(TestLists, Test_ListDropAny) 
{
	kLinkedList_t list;
	kLinkedListItem_t items[10];
	int data[10] = {0};

	list.head = NULL;
	list.tail = NULL;

	/* Setting up linked list and adding 10 items */
	for (int i = 0; i < 10; i++) {
		data[i] = i;
		items[i].list = NULL;
		items[i].next = NULL;
		items[i].prev = NULL;
		items[i].data = (void *)&data[i];

		common_listAddBack(&list, &items[i]);
	}

	CHECK_EQUAL(10, test_countListItems(&list));

	/* Delete item in the middle & check if list is 9 items long*/
	common_listDeleteAny(&list, &items[5]);

	CHECK_EQUAL(9, test_countListItems(&list));

	/* Check if head & tail are not changed */
	CHECK_EQUAL(&items[0], list.head);
	CHECK_EQUAL(&items[9], list.tail);

	/* Iterate through the list and try to find the deleted item */
	kLinkedListItem_t *head = list.head;
	bool failed = false;
			
	while (head != NULL) {
		if (head->data == &items[5].data) {
			failed = true;
			break;
		}

		head = head->next;
	}

	/* Pass if items[5] is not in the list */
	CHECK_FALSE(failed);

	/* Delete the first item, check if head is set correctly and tail is not changed*/
	common_listDeleteAny(&list, &items[0]);

	CHECK_EQUAL(&items[1], list.head);
	CHECK_EQUAL(&items[9], list.tail);

	CHECK_EQUAL(8, test_countListItems(&list));

	/* Delete the last item, check if tail is set correctly and head is not changed*/
	common_listDeleteAny(&list, &items[9]);

	CHECK_EQUAL(&items[1], list.head);
	CHECK_EQUAL(&items[8], list.tail);

	CHECK_EQUAL(7, test_countListItems(&list));

	/* Drop everything, check if head == tail == NULL and the list is empty */
	for (int i = 0; i < 10; i++) {
		common_listDeleteAny(&list, &items[i]);
	}

	CHECK_EQUAL(0, test_countListItems(&list));

	CHECK_EQUAL(NULL, list.head);
	CHECK_EQUAL(NULL, list.tail);
}

TEST(TestLists, Test_ListGetNth) 
{
	kLinkedList_t list;
	kLinkedListItem_t items[10];
	kLinkedListItem_t *itemsRet[10];
	int data[10] = {0};

	list.head = NULL;
	list.tail = NULL;

	/* Setting up linked list and adding 10 items */
	for (int i = 0; i < 10; i++) {
		data[i] = i;
		items[i].list = NULL;
		items[i].next = NULL;
		items[i].prev = NULL;
		items[i].data = (void *)&data[i];

		common_listAddBack(&list, &items[i]);
	}

	CHECK_EQUAL(10, test_countListItems(&list));

	/* Check common_listGetNthElement for every item */
	for (int i = 0; i < 10; i++) {
		itemsRet[i] = common_listGetNthElement(&list, i);
		CHECK_EQUAL(&items[i], itemsRet[i]);
	}

	/* Check common_listGetNthElement for out-of-bounds, should return NULL */
	CHECK_EQUAL(NULL, common_listGetNthElement(&list, 11));

	list.head = NULL;
	list.tail = NULL;

	/* Check common_listGetNthElement on an empty list, should return NULL */
	CHECK_EQUAL(NULL, common_listGetNthElement(&list, 0));
}
dict3:
	gcc main3.c MyDualCircularLinkedListsWithSentinels.c -o dict3
dict4:
	gcc main4.c Patricia.c MyInfo.c helper_functions.c  -o dict4
.PHONY: clean
clean:
	rm dict3 dict4

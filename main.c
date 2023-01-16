#include "9cc.h"

int main(int argc, char **argv)
{
	if (argc != 2){
		fprintf(stderr, "引数の個数が正しくありません。");
		return 1;
	}

	Token *token = tokenize(argv[1]);
	Node *head = expr(&token);

	printf(".intel_syntax noprefix\n");
	printf(".globl main\n");
	printf("main:\n");
	gen(head);
	printf("  pop rax\n");
	printf("  ret\n");
	return 0;
}
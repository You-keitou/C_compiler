#include "9cc.h"

void error(char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

// void error(char *loc, char *fmt, ...){
// 	va_list ap;
// 	va_start(ap, fmt);

// 	int pos = loc - user_input;
// 	fprintf(stderr, "%s\n", user_input);
// 	fprintf(stderr, "%*s", pos, " ");
// 	fprintf(stderr, "^ ");
// 	vfprintf(stderr, fmt, ap);
// 	fprintf(stderr, "\n");
// 	exit(1);
// }


bool at_eof(Token *token) {
	return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *cur, char *str, int str_len)
{
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	tok->len = str_len;
	cur->next = tok;
	return tok;
}

Token *tokenize(char *p) {
	Token head;
	head.next = NULL;
	Token *cur = &head;

	while(*p)
	{
		if (isspace(*p)){
			p++;
			continue;
		}

		if (!memcmp(p, "==", 2) || !memcmp(p, "!=", 2) || !memcmp(p, "<=", 2) || !memcmp(p, ">=", 2)){
			cur = new_token(TK_RESERVED, cur, p, 2);
			p += 2;
			continue;
		}

		// single-letter punctuator
		if (strchr("+-*/()<>", *p)) {
			cur = new_token(TK_RESERVED, cur, p++, 1);
			continue;
		}

		if (isdigit(*p)) {
			cur = new_token(TK_NUM, cur, p, 0);
			cur->val = strtol(p, &p, 10);
			continue;
		}

		error("トークナイズできません");
	}

	new_token(TK_EOF, cur, p, 0);
	return head.next;
}
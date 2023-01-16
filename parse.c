#include "9cc.h"

static bool consume(Token **token, char *op);
static void expect(Token **token, char *op);
static int expect_number(Token **token);
static Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
static Node *new_node_num(int val);
static Node *mul(Token **token);
static Node *primary(Token **token);
static Node *unary(Token **token);
static Node *equality(Token **token);
static Node *relational(Token **token);
static Node *add(Token **token);

static bool consume(Token **token, char *op){
	if ((*token)->kind != TK_RESERVED || strlen(op) != (*token)->len || memcmp((*token)->str, op, (*token)->len))
		return false;
	*token = (*token)->next;
	return true;
}

static void expect(Token **token, char *op){
	if ((*token)->kind != TK_RESERVED || strlen(op) != (*token)->len || memcmp((*token)->str, op, (*token)->len))
		error("'%c'ではありません",op);
	*token = (*token)->next;

}

static int expect_number(Token **token) {
	if ((*token)->kind != TK_NUM)
		error("数ではありません");
	int val = (*token)->val;
	*token = (*token)->next;
	return val;
}

static Node *new_node(NodeKind kind, Node *lhs, Node *rhs)
{
	Node *node = calloc(1, sizeof(Node));
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	return node;
}

static Node *new_node_num(int val)
{
	Node *node = calloc(1, sizeof(Node));
	node->kind = ND_NUM;
	node->val = val;
	return node;
}

static Node *mul(Token **token)
{
	Node *node = unary(token);

	for (;;){
		if (consume(token, "*"))
			node = new_node(ND_MUL, node, unary(token));
		else if (consume(token, "/"))
			node = new_node(ND_DIV, node, unary(token));
		else
			return node;
	}
}

static Node *primary(Token **token) 
{
	if (consume(token, "(")){
		Node *node = expr(token);
		expect(token, ")");
		return node;
	}
	return new_node_num(expect_number(token));
}

static Node *unary(Token **token)
{
	if (consume(token, "+"))
		return unary(token);
	if (consume(token, "-"))
		return new_node(ND_SUB, new_node_num(0), unary(token));
	return primary(token);
}

static Node *equality(Token **token)
{
	Node *node = relational(token);

	for(;;){
		if (consume(token ,"=="))
			node = new_node(ND_EQ, node, relational(token));
		else if (consume(token, "!="))
			node = new_node(ND_NE, node, relational(token));
		else
			return node;
	}
}

static Node *relational(Token **token)
{
	Node *node = add(token);

	for(;;){
		if (consume(token ,"<"))
			node = new_node(ND_LT, node, add(token));
		else if (consume(token ,"<="))
			node = new_node(ND_LE, node, add(token));
		else if (consume(token ,">"))
			node = new_node(ND_LT, add(token), node); 
		else if (consume(token ,">="))
			node = new_node(ND_LE, add(token), node);
		else
			return node;
	}
}

static Node *add(Token **token)
{
	Node *node = mul(token);

	for(;;)
	{
		if (consume(token, "+"))
			node = new_node(ND_ADD, node, mul(token));
		else if (consume(token, "-"))
			node = new_node(ND_SUB, node, mul(token));
		else
			return node;
	}
}

Node *expr(Token **token) {
	Token **ptr;
	ptr = token;
	return equality(ptr);
}

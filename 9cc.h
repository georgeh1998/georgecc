#ifndef NINECC_H
#define NINECC_H

// トークンの種類
typedef enum {
  TK_RESERVED, // 記号
  TK_IDENT,    // 識別子
  TK_RETURN,   // return
  TK_IF,       // if
  TK_ELSE,     // else
  TK_WHILE,    // while
  TK_FOR,      // for
  TK_NUM,      // 整数トークン
  TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

// トークン型
struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
  int len;
};


// 抽象構文木のノードの種類
typedef enum {
  ND_ADD,    // +            // 0
  ND_SUB,    // -            // 1
  ND_MUL,    // *            // 2
  ND_DIV,    // /            // 3
  ND_ASSIGN, // =            // 4
  ND_LVAR,   // ローカル変数   // 5
  ND_FUNC,   // 関数　　　　   // 6
  ND_EQUAL,  // ==           // 7
  ND_NEQUAL, // !=           // 8
  ND_GT,     // >            // 9
  ND_EGT,    // =>           // 10
  ND_LT,     // <            // 11
  ND_ELT,    // =<           // 12
  ND_RETURN, // return       // 13
  ND_IF,     // if           // 14
  ND_WHILE,  // while        // 15
  ND_FOR,    // for          // 16
  ND_BLOCK,  // block        // 17
  ND_NUM,    // 整数          // 18
} NodeKind;

typedef struct Node Node;

struct Node {
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;        // kindがND_NUMの場合のみ使う
  int offset;     // kindがDN_LVARの場合のみ使う
  Node *branch[10]; // kindがND_IFやforの場合使う(ifはif-elseのみ対応)
  char *name;    // 関数の場合
  int len;       // 関数の場合
};

typedef struct LVar LVar;

// ローカル変数の型
struct LVar {
  LVar *next;
  char *name;
  int len;
  int offset;
};

// LabelのStack番号管理用
typedef struct {
    int stack[100];
    int top;
    int currentLabel;
} LabelStack;


void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);

Token *tokenize();

void *program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();  
Node *primary();


void gen(Node *node);


LabelStack *create_label_stack();
int push_label(LabelStack *stack);
int pop_label(LabelStack *stack);


// 現在着目しているトークン
extern Token *token;

// 入力プログラム
extern char *user_input;

// 複数行分のNode
extern Node *code[100];

// ローカル変数
extern LVar *locals;

// Label記憶用のStack
extern LabelStack *labelStackIf;
extern LabelStack *labelStackWhile;
extern LabelStack *labelStackFor;

// Debug用関数
void printTokens(Token token);
void printToken(Token token);

#endif
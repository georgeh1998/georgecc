#include "./georgecc.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// 現在着目しているトークン
Token *token;

// file name
char *filename;

// 入力プログラム
char *file;

// 複数行分のNode
Node *code[100];

// 関数のsignature情報を入れる
FunctionSig *function_sig;

// 現在見ている関数のToken
Node *current_func_token;

// 文字列
char *chars[100];

// Label記憶用のStack
LabelStack *labelStackIf;
LabelStack *labelStackWhile;
LabelStack *labelStackFor;


int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "引数の個数が正しくありません\n");
    return 1;
  }

  filename = argv[1];

  // トークナイズする
  file = read_file(argv[1]);
  token = tokenize();

  program();

  // アセンブリの前半部分を出力
  printf(".intel_syntax noprefix\n");

  // dataセクション出力
  gen_data();

  labelStackIf = create_label_stack();
  labelStackWhile = create_label_stack();
  labelStackFor = create_label_stack();
  

  printf(".globl main\n");
  printf(".text\n");
  // 抽象構文木を下りながらコード生成
  for (int i = 0; code[i]; i++) {
    gen_top_level_def(code[i]);
  }
  return 0;
}
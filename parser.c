/* front.c - a lexical analyzer system for simple
arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
/* Global declarations */
/* Build AST*/
typedef struct Node {
    int node_type; // 0 = operator, 1 = operand
    union {
        char operator; // +, -, *, /, **, ++, --
        int value;     // integer literal
    } data;
    struct Node* left;
    struct Node* right;
} Node;
/* Variables */
int charClass;
char lexeme[100];
int nextChar;
int lexLen;
int token;
int nextToken;
int exp_value;
FILE *in_fp;
/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
Node* term();
Node* expr();
Node* power();
Node* factor();
int lex();
/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define PLUS 21
#define MINUS 22
#define TIMES 23
#define DIVIDE 24
#define LPAREN 25
#define RPAREN 26
#define EXPONENT 27
#define INCREMENT 28
#define DECREMENT 29
#define END_OF_FILE -1
/* Build AST*/


Node* makeNode(int type, char op, int val, Node* left, Node* right) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->node_type = type;
    if (type == 0) n->data.operator = op;
    if (type == 1) n->data.value = val;
    n->left = left;
    n->right = right;
    return n;
}

void printAST(Node* root, int depth) {
    if (root == NULL) return;
    for (int i = 0; i < depth; i++) printf("  "); // indent
    if (root->node_type == 1)
        printf("%d (int)\n", root->data.value);
    else
        printf("%c (op)\n", root->data.operator);
    printAST(root->left, depth + 1);
    printAST(root->right, depth + 1);
}

int eval(Node* root) {
    if (root == NULL) return 0;

    if (root->node_type == 1) {
        // Operand
        return root->data.value;
    }

    // Operator
    int leftVal = eval(root->left);
    int rightVal = eval(root->right);

    switch (root->data.operator) {
    case '+': return leftVal + rightVal;
    case '-': return leftVal - rightVal;
    case '*': return leftVal * rightVal;
    case '/': return leftVal / rightVal;
    case '^': { // exponentiation
        int result = 1;
        for (int i = 0; i < rightVal; i++) result *= leftVal;
        return result;
    }
    default:
        printf("Error: unknown operator %c\n", root->data.operator);
        return 0;
    }
}


/******************************************************/

/* main driver */
int main()
{
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("front.in.txt", "r")) == NULL)
        printf("ERROR - cannot open front.in.txt \n");
    else
    {
        getChar();
        lex(); // get first token
        Node* tree = expr();

        printf("\nAST:\n");
        printAST(tree, 0);

        printf("\nResult = %d\n", eval(tree));
    }
    return 0;

}
/*****************************************************/
/* lookup - a function to lookup operators and parentheses
and return the token */

int lookup(char ch)
{
    switch (ch)
    {
    case '(':
        addChar();
        nextToken = LPAREN;
        break;
    case ')':
        addChar();
        nextToken = RPAREN;
        break;
    case '+':
        addChar(); // this modified version checks if it is  ++ or +
        getChar();
        if (nextChar == '+') {
            addChar();
            nextToken = INCREMENT;
        } else {
            nextToken = PLUS;
        }
        break;
    case '-':
        addChar();
        getChar();
        if (nextChar == '-')
        {
            addChar();
            nextToken = DECREMENT;
        } else {
            nextToken = MINUS;
        }
        break;
    case '*':
        addChar();
        getChar();

        if (nextChar == '*')
        {
            addChar();
            nextToken = EXPONENT;
        }
        else {
            nextToken = TIMES;
        }
        break;
    case '/':
        addChar();
        nextToken = DIVIDE;
        break;
    default:
        addChar();
        nextToken = END_OF_FILE;
        break;
    }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar()
{
    if (lexLen <= 98)
    {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
        printf("Error - lexeme is too long \n");
}

/*****************************************************/
/* getChar - a function to get the next character of
input and determine its character class */
void getChar()
{
    if ((nextChar = getc(in_fp)) != END_OF_FILE)
    {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    }
    else
        charClass = END_OF_FILE;
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank()
{
    while (isspace(nextChar) && nextChar != END_OF_FILE)
        getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex()
{
    lexLen = 0;
	lexeme[0] = '\0';
    getNonBlank();
    switch (charClass)
    {
    /* Parse identifiers */
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT)
        {
            addChar();
            getChar();
        }
        nextToken = IDENT;
        break;
    /* Parse integer literals */
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT)
        {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        break;
    /* Parentheses and operators */
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        break;
    /* END_OF_FILE */
    case END_OF_FILE:
        nextToken = END_OF_FILE;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = 0;
        break;
    } /* End of switch */
    printf("Next token is: %d, Next lexeme is %s\n",
           nextToken, lexeme);
    return nextToken;
} /* End of function lex */
/* expr
   Parses strings in the language generated by the rule:
   <expr> -> <term> {(+ | -) <term>}
   */
Node*  expr() {
    printf("Enter <expr>\n");
    Node* left = term();
    while (nextToken == PLUS || nextToken == MINUS) {
        char op = (nextToken == PLUS ? '+' : '-');
        lex();
        Node* right = term();
        left = makeNode(0, op, 0, left, right);
    }
    printf("Exit <expr>\n");
    return left;
}  /* End of function expr */
/* term
   Parses strings in the language generated by the rule:
   <term> -> <factor> {(* | /) <factor>) 
   */
Node*  term() {
  printf("Enter <term>\n");
  Node* left = power();
  while (nextToken == TIMES || nextToken == DIVIDE) {
      char op = (nextToken == TIMES ? '*' : '/');
      lex();
      Node* right = power();
      left = makeNode(0, op, 0, left, right);
  }
  printf("Exit <term>\n");
  return left;
}  /* End of function term */

/* factor
   Parses strings in the language generated by the rule:
   <factor> -> id | int_constant | ( <expr )
   */
Node* factor() {
  printf("Enter <factor>\n");
  Node* result = NULL;



  if (nextToken == INT_LIT) {
      int val = atoi(lexeme);   // convert string to int
      printf("factor(): token=%d, lexeme=%s -> val=%d\n", nextToken, lexeme, val);
      result = makeNode(1, 0, val, NULL, NULL);
      lex();
  }
  else if (nextToken == IDENT) {
      // for now treat identifiers as 0 or placeholder
      int val = lexeme[0] - 'a' + 1;   // a=1, b=2, c=3 ...
      result = makeNode(1, 0, val, NULL, NULL);
      lex();
  }
  else if (nextToken == INCREMENT || nextToken == DECREMENT) {
      char op = (nextToken == INCREMENT ? '+' : '-');
      lex();
      Node* child = factor();
      result = makeNode(0, op, 0, child, NULL);
  }
  else if (nextToken == LPAREN) {
      lex();
      result = expr();
      if (nextToken == RPAREN) lex();
      else printf("Error: missing closing parenthesis\n");
  }
  else {
      printf("Error: invalid factor\n");
  }

  printf("Exit <factor>\n");
  return result;

} /* End of function factor */

/* power
   Parses string in the language generated by the rule:
   <power> -> ** <factor>      
*/
Node* power() {
    printf("Enter <power>\n");
    Node* left = factor();
    while (nextToken == EXPONENT) {
        lex();
        Node* right = power();
        left = makeNode(0, '^', 0, left, right);
    }
    printf("exit <power>\n");

    return left;
}
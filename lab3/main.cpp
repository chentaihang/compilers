#include "grammar.h"
extern Grammar eraserecursion(Grammar grammar);
int main()
{
Grammar g;
g.addproduction("A->BCD");
g.addproduction("B->b|bc");

Grammar result = eraserecursion(g);
return 0;

}
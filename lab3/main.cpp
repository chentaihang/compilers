#include"eraserecursion.cpp"
extern Grammar eraserecursion(Grammar grammar);
int main()
{
Grammar g;
g.addproduction("S->S+T|T");
g.addproduction("T->T*F|F");
g.addproduction ("F->(E)|id");

Grammar result = eraserecursion(g);
 for (auto it : result.productions) {
        for (auto it1 = it.second.rules.begin(); it1 != it.second.rules.end(); it1++) {
            if (it.first > 'Z') {  
                cout << char(it.first-39) << "'" << "->" << *it1 << endl;
            } else {
                cout << it.first << "->" << *it1 << endl;
            }
        }
    }
return 0;

}
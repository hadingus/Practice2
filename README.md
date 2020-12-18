**Практическое задание для курса "Формальные языки". Реализация алгоритма Эрли.**
==========

Формат ввода
----------
1. В первой строке вводится число n - количетво правил в грамматике.
2. В следующих n строках вводятся правила, имеющие вид S->R<sub>1</sub>|R<sub>2</sub>|...|R<sub>k</sub>, где S - нетерминал, R<sub>i</sub> - простое правило, т. е. состоит только из символов алфавита(строчные латинские буквы) и нетерминалов(заглавных латинских символов). 
3. Далее на вход подается k - количество интересующих строк.
4. В следующих k строках вводятся слова для проверки принадлежности грамматике.

Формат вывода
----------
Для каждой строки - запроса выводится "yes", если слово лежит в грамматике, "no" - иначе.

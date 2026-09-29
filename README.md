# `TEMA2 SDA - Urse Andrei 312CB`


## Structuri

`File`: un singur fisier din lista; contine datele acestuia (id, scor, vector de pointeri catre cuvintele-cheie, numar cuvinte-cheie, pointeri), cat si pointeri catre fisierul anterior si cel urmator.

`FileList`: lista dublu inlantuita de fisiere; contine pointeri catre inceputul (**head**) si sfarsitul (**tail****) listei.

`RefList`: lista simplu inlantuita care retine referinte, pointeri catre fisiere.

`Trie` arborele multicai de regasire; fiecare nod contine litera, pointer catre lista de referinte catre fisierele care contin cuvantul-cheie curent (acest aspect este valabil doar pentru nodurile terminale, nodurie interioare au lista de referinte goala) si pointeri catre fiul din stanga si fratele din dreapta. 

`System`: contine pointerii catre lista de fisiere, arborele de tip `Trie` si fisierele de input/output pentru a simplifica operatiile cu functii.

## Functii de Initializare

`initFileList`: aloca memorie, creeaza lista de fisiere goala si seteaza pointerii **head** si **tail** la *NULL*.

`initTrie`: aloca memorie, creeaza un singur nod pentru arborele `Trie` si seteaza atat **letter** la caracterul nul, cat si pointerii **files**, **left** si **right** la *NULL*. 

`initSystem`: aloca memorie, apeleaza cele doua functii descrise mai sus si deschide fisierele de intrare (**indexare.in**) si iesire (**indexare.out**).

## Functii de Eliberare a Memoriei

`freeFile`: elibereaza memoria alocata pentru identificator si pentru fiecare cuvant-cheie in parte, iar la final pentru intreg fisierul.

`freeFileList`: parcurge intreaga lista de fisiere si apeleaza functia de mai sus pentru fiecare element in parte, iar la final elibereaza memoria alocata pentru toata structura `FileList`.

`freeTrie`: elibereaza memoria alocata pentru fiecare nod al arborelui in mod recursiv, cat si lista de referinte din fiecare nod terminal.

`destroySystem`: apeleaza ultimele doua functii descrise mai sus, inchide fisierele de intrare si iesire, iar la final elibereaza memoria alocata pentru toata structura `System`.

## Functii Auxiliare

`searchFile`: cauta in lista de fisiere identificatorul primit si returneaza fisierul gasit, sau *NULL* in caz contrar; functia este apelata in **ADD**, **DEL**, **ADDKW** si **DELKW**. 

`consumeKeyWords`: pentru operatia **ADD**; daca fisierul cu identificatorul primit exista, cuvintele-cheie care ar fi trebuit introduse in vectorul de pointeri al fisierului primit se citesc pentru a nu afecta inputul urmatoarei comenzi; functia este apelata in **ADD**.

`allocNewFile`: pentru operatia **ADD**; aloca memorie pentru noul fisier, retine informatiile primite, verifica daca la citire cuvintele-cheie apar deja in vectorul de pointeri, iar la final adauga fisierul in lista; functia returneaza fisierul nou creat; functia este apelata in **ADD**.

`insertKWTrie`: pentru un cuvant-cheie, parcurge arborele; daca nodul curent nu are fiu, il creeaza cu ajutorul functiei `initTrieNode`, sotcheaza litera curenta din cuvantul-cheie si pointerul trece la nodul de jos; daca exista fiu, se cauta litera curenta pornind de la el si continuand cu fratii lui, iar pointerul trece ori la nodul gasit sau ori la un altul nou creat; in ultimul caz mentionat, daca nodul de la care s-a pornit nu are fii, se introduce nodul nou la inceput sau la dreapta celor existente in caz contrar; functia returneaza nodul terminal al cuvantului-cheie; functia este apelata in **ADD** si **ADDKW**.

`addReference`: se adauga referinta catre fisier la inceputul listei de referinte din nodul terminal al cuvantului-cheie; functia este apelata in **ADD** si **ADDKW**.

`findTerminalNode`: returneaza nodul terminal al cuvantului-cheie primit; functia este apelata in **DEL**, **DELKW**, **TOPK** si **PREFIX**.

`delTrieNodes`: sterge nodurile inutile, de jos in sus in mod recursiv; functia este apelata in **DEL** si **DELKW**.

`delReference`: cauta pointerul catre fisier in lista de referinte a nodului terminal si il elimina; functia este apelata in **DEL** si **DELKW**.

`delFile`: elimina un fisier din lista de fisiere si se elibereaza memoria prin functia `freeFile`; functia este apelata in **DEL**, **DELKW** si `delFileKW`.

`allocKWFile`: mareste vectorul de pointeri catre cuvintele-cheie asociat unui fisier si adauga noul cuvant-cheie; functia este apelata in **ADDKW**.

`delKWFile`: cauta cuvantul-cheie in vectorul de pointeri asociat unui fisier si il elimina; la final verifica daca fisierul nu mai are cuvinte-cheie asociate si il elimina si pe el din lista de fisiere; functia este apelata in **DELKW**.

`countFiles`: numara cati pointeri catre fisiere exista in lista de referinte a unui nod terminal; functia este apelata in **FIND**, **TOPK**, **PREFIX**, `printTrie` si `countPrefixFiles`.

`storeIDs`: parcurge lista de referinte a unui nod terminal si retine fiecare identificator intr-un vector de pointeri; functia este apelata in **FIND**, **PREFIX** si `printTrie`.

`sortIDs`: parcurge vectorul de pointeri care retine identificatorii si ii aranjeaza in ordine lexicografica; functia este apelata in **FIND**, **PREFIX** si `printTrie`.

`printIDs`: afiseaza identificatorii din vectorul de pointeri; functia este apelata in **FIND**, **PREFIX** si `printTrie`.

`buildMaxHeap`: transforma vectorul de pointeri catre identificatori intr-un max-heap, pornind de la mijlocul vectorului (ultimul nod care nu e frunza), in sens invers, pana la radacina arborelui; functia este apelata in **TOPK**.

`heapify`: aplica proprietatea de max-heap a unui subarbore; aduce in radacina curenta cea mai mare valoare dintre radacina si cei doi fii din stanga si din dreapta; daca radacina curenta se modifica, se repara subarborele corespunzator fiului cu valoarea cea mai mare; functia este apelata in `buildMaxHeap` si `printKFiles`.

`checkPriority`: compara scorul a doua fisiere, iar daca acesta este egal compara identificatorul; functia este apelata in `heapify`.

`printKFiles`: afiseaza primele **K** cele mai relevante fisiere; de fiecare data se afiseaza primul element din heap, dupa care se muta ultimul element in locul lui si se apeleaza `heapify` pentru a reface heap-ul; functia este apelata in **TOPK**

`printTrie`: afiseaza toate cuvintele-cheie impreuna cu identificatoarele fisierelor asociate din arbore in mod recursiv; construieste cuvantul-cheie litera cu litera si il afiseaza cand ajunge la nodul terminal, impreuna cu numarul de fisiere asociate si identificatorii lor; parametrul **empty** ramane 1 daca arborele este gol; functia este apelata in **PRINT**

`countPrefixFiles`: parcurge in mod recursiv arborele pornind de la ultimul nod al prefixului si returneaza suma numarului de referinte din lista fiecarui nod terminal; functia este apelata in **PREFIX**.

`storePrefixIDs`: parcurge in mod recursiv arborele pornind de la ultimul nod al prefixului si retine in vectorul de pointeri identificatorul fiecarui fisier regasit in lista de referinte; se transmite prin referinta parametrul *i* pentru a introduce corect urmatoarele referinte in vector; functia este apelata in **PREFIX**.

`filterIDs`: parcurge vectorul de pointeri si aranjeaza identificatorii in ordine lexicografica; daca exista mai multe aparitii a unor identificatori, se elimina astfel incat toate sa fie unice; functia este apelata in **PREFIX**.
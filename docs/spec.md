% Ekstrakcja treści ze stron WWW
% Przemysław Kopański

## Wstęp

### Opis zakładanej funkcjonalności

Biblioteka ma na celu konfigurowalne zbieranie danych ze stron HTML.

Użytkownik podaje schemat, w którym opisuje z którego miejsca na stronie HTML
chce pobrać dane.
Bilbioteka przetwarza schemat użytkownika oraz stronę HTML, zwracając poszukiwane dane.

Biblioteka zrealizowana w C++, udostępnia interferjs dla programisty - przyjmujący ciąg znaków opisujący schemat, kod strony HTML a zwracająca
kolekcje posiadającą iterator.

W schemacie można umieścić następujące zapytania:  

 * zapytanie o zawartość (tekstową, elementów pochodnych lub obu) wszystkich węzłów HTML które spełniają dany schemat
 * w każdej warstwie ścieżki można określić
     + nazwę węzła (lub też wskazać, że może być dowolna)
     + atrybuty węzła
         - nazwę atrybutu
         - wartość atrybutu (można też wskazać, że wartość atrybutu jest szukana, lub że atrybut może mieć dowolną wartość)

### API

Biblioteka udostępnia funkcję:
```cpp
std::list<wstring> findInfo(std::wstring htmlCode, std::wstring schema);
```

W zwracanej liście znajdują się informacje o poszukiwanych danych,
funkcja zwróci wyjątek w przypadku błędnych danych wejściowych z opisem błędu.
Jeśli z jednego węzła będa pochodzić więcej niż jedna informacja, każda będzie stanowiła osobny
element listy.


### Przykłady testowe

Dla przykładowej strony HTML
```html
<html>
<body>
<div id="main">
  <div id="items">
    <div class="item">
      <a class="price">54</a>zł
    </div>
    <div class="item">
      <a class="price">12</a>zł
    </div>
    <div class="item">
      <a class="price">34</a>zł
    </div>
  </div>
  <div id="images">
    <img src="kot.jpg"/>
    <img src="pies.jpg"/>
    <img src="kurczak.jpg"/>
  </div>
  <div lolcontent='true'>
    <img src="ppap.jpg"/>
  </div>
</body>
</html>
```

##### Przykład 1
schemat: `/div class='item'/a class='price'/`  
schemat: `/div/* class='price'/`  

rezultat:  
54  
12  
34  


##### Przykład 2
schemat: `/div class=?/a class='price'/`  

rezultat:  
item  
item  
item  

##### Przykład 3
schemat: `/div id='item'/a class='price'//`  
schemat: `/div/* class='price'//`  
schemat: `/div id='item'/a class='price'///`  
schemat: `/div/* class='price'///`  

rezultat:  
```
<a class="price">54</a>zł
<a class="price">12</a>zł
<a class="price">34</a>zł
```

##### Przykład 4
schemat: `/img src=?/`  
schemat: `/div/img src=?/`  

rezultat:  
kot.jpg  
pies.jpg  
kurczak.jpg  
ppap.jpg  

##### Przykład 5
schemat: `/* lolcontent=!/img src=?/`  

rezultat:  
ppap.jpg

## Język zapytań

### Gramatyka

```ebnf
start = '/' , {expr , '/'} , query;

query      = (* query_content - epsilon*)
           | '/' , query_next
           ;
query_next = '/'
           | (* query_domelems - epsilon)
           ;

expr  = (tag_name | tag_any) ,
        [ { (attr_name , '=' , expr_attr_query ) } ]
        ;
expr_attr_query = attr_quotedval
                | attr_query
                | attr_mustexist
                ;
attr_quotedval  = "'" , attr_val , "'";

tag_name  = string;
attr_name = string;
attr_val  = string;

string = { digit | letter }

(* symbole terminalne *)
digit  = [0-9];
letter = [a-zA-Z];

tag_any        = '*';
attr_query     = '?';
attr_mustexist = '!';
```

### Drzewo dokumentu
Drzewo języka zapytań ze względu na swoją jednotorową strukturę
będzie reprezentowane przez listę/vector pojedynczych obiektów opisujących
oraz kilka zmiennych pomocniczych zawierających w sobie informacje o specyfice zapytania
(czy zapytanie tyczy się potomków względem podanego miejsca, contentu, czy wyszukiwane są wartości atrybutów).

Obiekt opisujący składa się z informacji o nazwie tagu oraz jego atrybutach.

```cpp
struct qstart {
  int questionType;
  vector<qnode> children;
};


struct qnode {
  map<wstring, wstring> read_attributes;
  vector<wstring> unknown_attributes; // wartosci atrybutow do wypisania
  wstring tagname;
  bool tagNameKnown; // czy nazwa tagu moze byc dowolna
  wstring content;
};
```

### Tokeny leksykalne

- TOK_SLASH - /
- TOK_ATTRQUERY - ?
- TOK_MUSTEXIST - !
- TOK_ANYTAG - *
- TOK_SINGLEQUOTEVAL - '<wartosc>'
- TOK_DOUBLEQUOTEVAL - "<wartosc>"
- TOK_EQUALS - =
- TOK_STRING - string

## HTML

### Gramatyka HTML
```ebnf
start   = [ doctype ] , { node_content };

doctype = tag_begin , '!DOCTYPE' , { doctype_letter } , tag_end ;


node_begin_noncomment = tag_name , tag_attrs;


node = tag_begin , node_begin_noncomment,
      ((tag_end , node_content , [ tag_closingbegin, tag_name, tag_end ])
       | tag_semiclose)


tag_begin        = '<';
tag_closingbegin = '</';
tag_end          = '>';
tag_semiclose    = '/>';

node_content     = [ { node | text_content | comment } ];

tag_attrs      = [ { tag_attr_begin , tag_attr } ];

tag_attr_begin = attr_name;
tag_attr       = '=' , tag_attr_val;
               |
               ;

tag_attr_val   = attr_doublequote_value
               | attr_singlequote_value
               | attr_value
               ;

attr_singlequote_value = "'' , attr_value , "'";
attr_doublequote_value = '"' , attr_value , '"';

attr_value         = string;
text_content       = { digit | letter | '&' | '#' | ';' };

string             = { digit | letter };

doctype_letter = letter | digit | '-' | '/' | '"' ;

(* symbole terminalne *)
digit  = [0-9];
letter = [a-zA-Z];

```

### Drzewo dokumentu

```cpp
struct htmlstart {
  list<unique_ptr<node>> nodes;
};

struct node {
  virtual ~node() {}
  virtual void accept(Visitor &v) const = 0;
};

struct htmlnode : node {
  wstring tag_name;
  map<wstring, wstring> attributes;
  list<unique_ptr<node>> children;
};

struct emptyhtmlnode : node {
  wstring tag_name;
  map<wstring, wstring> attributes;
};

struct textnode : node {
  wstring content;
};
```

### Tokeny leksykalne

- TOK_TAGOPEN - `<`
- TOK_CLOSINGTAGOPEN - `</`
- TOK_TAGCLOSE - `>`
- TOK_TAGSELFCLOSE - `/>`
- TOK_SINGLEQUOTEVAL - `'<wartosc>'`
- TOK_DOUBLEQUOTEVAL - `"<wartosc>"`
- TOK_NOQUOTEVAL - `<wartosc>`
- TOK_EQUALS - `=`
- TOK_COMMENT- `<!-- <komentarz> -->`
- TOK_DOCTYPE- `<!DOCTYPE <docstring> >`
- TOK_TEXTCONTENT - string
- TOK_HTMLCONTENT - string

### Moduł parsera do JS/CSS

Lexer, w przypadku natrafienia na otwierający tag __style__ lub __script__ uruchomi moduł, który przeskanuje kod źródłowy i przesunie wskaźnik tekstu na tag końcowy, w celu uniknięcia próby interpretacji JS/CSS jako kodu dokumentu HTML).


## Projekt realizacji

### Moduły bilbioteki
Biblioteka wewnętrznie wykorzystuje:

- analizator leksykalny, składniowy i semantyczny HTML
- analizator leksykalny, składniowy i semantyczny języka zapytań
- algorytmu wyszukującego dane


### Algorytm znajdujący schemat
Po przetworzeniu zadanego kodu HTML oraz zapytania, algorytm przeszukuje drzewo struktury HTML
algorytmem DFS.
Dla każdego węzła sprawdza, czy pasuje on (wraz z potomstwem) do poszukiwanego wzorca, jeżeli tak to zapisuje wynik przeszukania.


## Wnioski

### Podsumowanie
Projekt zrealizowany w całości. Podczas wykonywania projektu okazało się, że HTML ma
mnóstwo różnych standardów, został zaimplementowany jego najistosniejszy podzbiór, umożliwiający przetworzenie interii.
W trakcie wykonywania kod przechodził kilka gruntownych przeróbek (przerobienie gramatyki, wyrzucenie zbędnych maszyn stanowych,
przenoszenie odpowiedzialności zadań pomiędzy lexerem a parserem, wsparcie niedomkniętych tagów i _void_ tagów (np. br),
zapewnienie braku wycieków pamięci stosująć _sprytne_ wskaźniki z bilbioteki standardowej C++).
Jak widać w załączonych w kodzie testach, program wykonuje swoje zadanie.
Parser RD jest wygodny w implementacji oraz pozwala na szybką adaptacje kody po drobnych zmianach gramatyki
(choć obecnie raczej stosuje się generatory parserów niż pisze własny parser od zera).

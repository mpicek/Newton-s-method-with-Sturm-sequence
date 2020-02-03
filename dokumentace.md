## Uživatelská dokumentace

Spustitelný program __sturmAndNewton__ je program pro Linux. V případě potřeby je možno zkompilovat main.cpp pomocí `g++ -o sturmAndNewton main.cpp -std=c++11`.

Do programu zapíšete polynom, u kterého chcete nalézt kořeny. Vstup zadejte v tomto formátu: `5x4-2.65x2+3x-2.75` tzn. "5 krát x na čtvrtou" se zapíše jednoduše jako 5x4
1) Na pořadí členů nezáleží
2) U prvního členu může být +, - nebo nic, v tom případě je člen kladný
3) Pro desetinná místa použijte tečku
4) Koeficienty jsou reálná čísla (místo desetinné čárky pište tečku) a exponenty jsou přirozená čísla

Program vypíše interval, ve kterém se nachází všechny kořeny, dále vypíše Sturmovy posloupnosti a menší intervaly, ve kterých se kořeny nacházejí. Pokud má polynom kořeny, vypíše je.

Program používá floating-point aritmetiku, proto všechny výsledky __nemusí být přesné__ a je nutno s tím předem počítat. Sturmova metoda taktéž __nehledá vícenásobné kořeny__, pokud je tedy *x* vícenásobný kořen, vypíše ho to pouze jednou. __Metoda hledá pouze reálné kořeny__.

Doporučuje se počítat s polynomy o maximálním exponentu zhruba 50 a přiměřeně velkými koeficienty (do 100000) u členů polynomu. Program zvládne pracovat s exponenty i koeficienty mnohonásobně vyššími než je toto doporučení, výsledky pak ale nejsou zaručené, jejich kvalita může být horší, jelikož celkový interval, ve kterém hledáme, je mnohonásobně zvětšený.

## Technická dokumentace

#### Co je Sturmova metoda a k čemu slouží?
Sturmova metoda je numerická metoda Jacquese Sturma pro spočítání rozdílných reálných kořenů v reálném polynomu. Používá se především pro spočítání kořenů v námi specifikovaném intervalu. V této implementaci je spojena Sturmova metoda s metodou Newtonovo, kdy se nejdříve naleznou intervaly, kde se kořeny nachází, a následně se pomocí Newtonovy metody naleznou. Samotná Newtonova metoda by intervaly nenašla, je třeba kořeny nějak izolovat. Proto Sturmova metoda společně s metodou Newtonovo nám dávají vcelku vysokou záruku nalezení všech rozdílných (tzn. ne několikanásobných) reálných kořenů.

Celý program používá floating-point aritmetiku. V programu je tedy občas použito zaokrouhlování (především na nulu). Stávalo se, že koeficient členu polynomu byl extrémně malý a blížil se k nule. V tom případě se samozřejmě program zacyklil, protože nové Sturmovy posloupnosti se vytváří do té doby, než by byla poslední posloupnost rovna nějaké konstantě (viz níže), koeficient by ale stále zapojoval do hry člen polynomu o vyšším exponentu. Bylo tedy nutno použít zaokrouhlení. Konečné výsledky zaokrouhlované ale být nemusí, uživatel by s nimi měl naložit podle svého uvážení. Proto pokud kořen má vyjít 0, je možné, že hodnota vypsána programem bude blízká nule, ale nebude rovna nule.

Program je rozdělen do dvou souborů: `main.cpp` a `sturmLib.h`.
V souboru *main.cpp* je uložena řídící funkce *main()* a v souboru *sturmLib.h* jsou definované pomocné funkce popsané níže.

#### Funkce readInput()
Funkce načte a naparsuje vstup, ošetří špatně zapsané vstupy. Program jsem původně dělal pro zábavu, nevěděl jsem, že ho odevzdám jako zápočtový program a v té době se mi nabídl můj kamarád Dennis Pražák s tím, že mi zlepší parsování vstupu. Jeho parsování je dobré v tom, že akceptuje více zápisů, je možné zadávat členy polynomu v různém pořadí, můžeme zapsat členy stejného řádu několikrát, v tom případě je to sečte.

Tato funkce je tedy dílem mého kamaráda Dennise Pražáka. Zbytek programu jsem již psal sám.

#### Funkce getMaxInterval()
Existuje odhad intervalu, ve kterém se nachází všechny reálné kořeny polynomu. Je to interval `<-A-1; A+1>`, kde A je maximum z absolutních hodnot koeficientů členů polynomu. Tento interval následně rozkouskujeme na určitý počet intervalů, který je uložen v proměnné *numberOfIntervals*. Číslo by mělo být dostatečně velké, aby nám Newtonova metoda mohla zaručit dobrý výsledek. Tyto intervaly pak budeme vyhodnocovat pomocí Sturmovy metody, ta nám spočítá, kolik se nachází v daném intervalu rozdílných kořenů. Díky tomu dokážeme kořeny izolovat.

#### Funkce getFirstSequence()
Vrátí nám první Sturmovu posloupnost, což je vlastně sám polynom. Proto nám pouze zkopíruje sekvenci z *ex* (kde je polynom uložen) do sturm[0] (kde jsou posloupnosti uloženy).

#### Funkce getDerivative()
Toto je funkce vracející první derivaci polynomu, což je zároveň druhá Sturmova posloupnost.

#### Funkce getNextSequence(int numberOfSequence)
Vytvoří další Sturmovu posloupnost. Kolikátou posloupnost? Ta je určena v parametru. Pozor, počítá se od nuly (tzn. 2. posloupnost znamená, že *numberOfSequence* = 1). Tato funkce slouží pro spočtení posloupností od druhé dál, jelikož první posloupnost je daný polynom a druhá posloupnost je jeho derivace. N-tá posloupnost se vytvoří vydělením (n-2)-hé posloupnosti posloupností (n-1)-ní. Záporný zbytek po dělení je naše n-tá posloupnost. Takto se pokračuje, dokud není zbytek po dělení roven nějaké konstantě. V tom případě je daná posloupnost poslední. K vypsání dané posloupnosti slouží funkce __printSturmSequence(int numberOfSequence)__.

#### evaluateForOneValue(double x)
Po vytvoření všech posloupností vezmu hodnotu *x* z každého intervalu a dosadím ji postupně do všech posloupností. Toto *x* je nejmenší hodnota intevalu. Za dosazení se stará funkce __evaluate(int sequence, double x)__, takže funkce evaluateForOneValue() postupně volá tuto funkci pro každou posloupnost. Funkce evaluate() používá pro vyhodnocování Hornerovo schéma.
Pro každé *x* tedy získáme *n* hodnot. Důležitý je ale pouze počet změn znaménka hodnoty. Tento počet funkce evaluateForOneValue() vrací.

#### Funkce main()
Funkce main vše řídí - načte vstup, zavolá funkce pro spočtení všech posloupností a pro všechny intervaly získá počet změn znamének.
Počet změn znamének v bodě *a* pojmenujme A a počet změn znamének v bodě *b* pojmenujme B. Počet rozdílných kořenů v intervalu <a,b] je pak roven hodnotě A-B. My se ale rozdílu většímu než jedna snažíme vyhnout, Newtonova metoda by pak měla problém naleznout správný kořen. Problém odstraňuji dostatečným počtem intervalů, ve kterých je Sturmova metoda vyhodnocena. Naše *a* a *b* jsou vždy začátky a konce intervalů.
Funkce main() si zapamatuje intevaly, ve kterých jsou kořeny, a zavolá na ně funkci newton().

#### Funkce newton()
Intervaly uložené v *intervalsWithRoots* prohledá Newtonovo metodou. Ta pomocí první derivace (tzn. tečny na křivku) se snaží přiblížit ke kořeni. Derivace může mít ale špatný sklon a kvůli tomu se můžeme dostat mimo interval, což může způsobit, že kořen vůbec nenalezneme. Proto v této funkci interval rozdělíme ještě na menší a zkouším Newtonovu metodu spustit z různých hodnot na daném intervalu. Těchto začátečních bodů je 6 a jsou rovnoměrně rozmístěné. Newtonova metoda se opakuje do té doby, než je kořen nalezen s určitou přesností. Tato přesnost je uložena v *newtonPrecision*.
Funkce nalezené kořeny vypíše.


#encoding "utf-8"    // кодировка
#GRAMMAR_ROOT S

Break -> EOSent | AnyWord<wff=";">;

LimitDef -> LBracket AnyWord+ RBracket; //река-море (статья 2440?) 

Limits -> 'как' NP; //Noun<gram="acc"> ANP<gram="gen">; 
Limits -> 'как' ANP<gram="acc"> SimConjAnd ANP<gram="acc">; 

LimPart -> Word<gram="partcp"> AnyWord+;
LimitPart -> Comma LimPart interp (Definition.Comment::not_norm) Comma; 

//-------------Некоторые именные группы--------------------
//можно попробовать создать нетерминал прилагательное НЕ "другой"
//не помню, как указывается отрицание "леммы или нетерминала"
//Adj -> 

NP -> Noun;
NP -> Noun Noun<gram="gen">+;
NP -> NP ANP;
NP -> ANP;
//NP -> Noun PP;
ANP -> Adj<gnc-agr[1]>+ Noun<gnc-agr[1]>;

PPS -> PP+;
PP -> Prep Adj* Noun+;

//-------------Здесь должны быть правила для терминов------------------
//---------------------------------------------------------------------

NP_Def -> Noun<gram="nom"> interp (Definition.KeyWord); 
//NP_Def -> NP_Def (Adj<gnc-agr[1]>) Noun<gnc-agr[1]>+;
NP_Def -> NP_Def NP+;
ANP_Def -> Adj<gnc-agr[1]>+ NP_Def<gnc-agr[1]>; 
ANPDef -> Adj<gnc-agr[1]>+ LimitDef interp (Definition.Limitation) NP_Def<gnc-agr[1]>;
NP_Def -> NP_Def ANP_Def+;
NP_Def -> NP_Def ANPDef;
NP_Def -> ANP_Def;

Term -> NP_Def; 
Term -> NP_Def PPS interp (Definition.Comment); 
Term -> NP_Def LimitDef interp (Definition.Comment); 

Term2 -> Adj interp (Definition.Term) Pred interp (Definition.Pred) ANP interp (+Definition.Term; Definition.KeyWord; Content.KeyWord); //делимым является земельный участок

Pred -> 'являться' | 'пониматься';
Pred_no_interp -> Hyphen;

FullTerm -> Term interp (Definition.Term) (Limits interp (Definition.Limitation)) Pred_no_interp;  

T_not_nom -> Noun interp (Definition.KeyWord);
T_not_nom -> T_not_nom NP; 
FullTerm -> ('под') T_not_nom interp (Definition.Term) (Limits interp (Definition.Limitation)) Pred interp (Definition.Pred);  

//-------------Здесь должны быть правила для содержания определения----
//---------------------------------------------------------------------

NP_Cont -> Noun interp (Content.KeyWord);
NP_Cont -> NP_Cont NP+;
NP_Cont -> NP_Cont ANP_Cont;
NP_Cont -> ANP_Cont;
NP_Cont -> NP_Cont PP_Cont;
ANP_Cont -> Adj<gnc-agr[1]>+ NP_Cont<gnc-agr[1]>;

PP_Cont -> Prep (Adj) Noun+;

//-------Первый тип перечислений (правая часть опр. с каждой новой строки)
NumDelim -> RBracket | Punct;
Number -> AnyWord<wff="\\d+">;

//Cont_NP -> NP_Cont Btw NP_Cont;
SimpleBtw -> SimConjAnd | Or | ComplexOr;
//NP_Cont -> NP_Cont SimpleBtw NP_Cont;
Cont_NP -> NP_Cont;

LoseStrength -> 'утратить' 'сила'<gram="acc">;
EnumItem1 -> Number interp (Content.Marker) NumDelim Cont_NP interp (Content.KeyNP) (LimBrac interp (Content.Comment)) (Limits interp (Content.Limitation)) (Comma) (Comm interp (Content.Comment::not_norm)); // Break;
EnumItem1 -> Number interp (Content.Marker) NumDelim LoseStrength interp (Content.Depricated = 'true') AnyWord* interp (Content.Comment::not_norm); // Break;

//-------Второй тип перечислений (правая часть на той же строке)-------
Or -> (LBracket) 'или' (RBracket);
ComplexOr -> SimConjAnd Or;
Btw -> Comma | SimConjAnd | Or | ComplexOr;
EnumItem -> NP_Cont (Comm);

EnumItem -> 'другой' NP_Cont (Comm); //!!!нужно как-то "сказать", что если это прил. "другой", то мы интерпретируем не как им. группу, а как enum item

EnumIter -> EnumItem interp (Content.PlainText; Content.KeyNP) Btw;
Enum -> EnumIter+ EnumItem interp (Content.PlainText; Content.KeyNP); // Break; 

//------Содержание без перечисления---------------

LimBrac -> LBracket AnyWord+ RBracket;
Comm -> ('не') Word<gram="partcp"> AnyWord+;
Comm -> Prep 'который' AnyWord+;
Comm -> Noun 'который'<gram="gen"> AnyWord+;
Comm2 -> 'который' AnyWord+ (Cont_stop);

NP_Cont -> Adj Comma Word<gram="ADV"> Adj Noun interp (Content.KeyWord);
Cont_stop -> Break | Comma;
Content -> NP_Cont interp (Content.KeyNP) Comma Comm interp (Content.Comment::not_norm) Cont_stop;
Content -> NP_Cont interp (Definition.Term; Definition.KeyWord; Definition.PlainText;Content.KeyNP) LimBrac interp (Content.Limitation; Content.PlainText) Comma Comm interp (Content.Comment::not_norm) Cont_stop;
//Content -> Noun interp (Content.KeyWord) AnyWord+ interp (Content.Comment::not_norm) Cont_stop;

//-----------Корневые правила------------------------------------------
//---------------------------------------------------------------------

//---------------для первого типа-----------------
FullTerm1 -> T_not_nom<gram="ins"> interp (Definition.Term) (LimitPart) Pred interp (Definition.Pred) Colon interp (Definition.EnumExpected = 'true');
S -> FullTerm1 interp (Definition.PlainText);
S -> EnumItem1 interp (Content.PlainText) (AnyWord<wff=";">); 

//---------------для второго типа-----------------

S -> FullTerm interp (Definition.PlainText) Enum; //Break;

//---------------для типа без перечисления--------
S -> FullTerm interp (Definition.PlainText) Content interp (Content.PlainText); //Break;
S -> Term2 interp (Definition.PlainText) Comma Comm2 interp (Content.PlainText; Content.Comment);


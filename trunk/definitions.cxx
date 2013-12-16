#encoding "utf-8"    // кодировка
#GRAMMAR_ROOT S

Break -> EOSent | AnyWord<wff=";">;

LimitDef -> LBracket AnyWord+ RBracket; //река-море (статья 2440?) 

Limits -> 'как' NP; //Noun<gram="acc"> ANP<gram="gen">; 
Limits -> 'как' ANP<gram="acc"> SimConjAnd ANP<gram="acc">; 

LimPart -> Word<gram="partcp"> InClause+;
LimitPart -> Comma LimPart interp (Definition.Comment::not_norm) Comma; 

//-------------Некоторые именные группы--------------------
//можно попробовать создать нетерминал прилагательное НЕ "другой"
//не помню, как указывается отрицание "леммы или нетерминала"
//Adj -> 
AinBr -> LBracket Adj RBracket;

NP -> Noun;
NP -> Noun Noun<gram="gen">+;
NP -> NP ANP;
NP -> ANP;
//NP -> Noun PP;
ANP -> Adj<gnc-agr[1]>+ (AinBr) Noun<gnc-agr[1], rt>;
ANP -> Adj<gnc-agr[1]> 'и' ANP<gnc-agr[1], rt>;
ANP -> Adj<gnc-agr[1]> ',' ANP<gnc-agr[1], rt>;


PPS -> PP+;
//PP -> Prep Adj* Noun+;
PP -> Prep NP;
PP -> Prep Word<gram="SPRO">;

//-------------Здесь должны быть правила для терминов------------------
//---------------------------------------------------------------------

NP_Def -> Noun<gram="nom"> interp (Definition.KeyWord); 
//NP_Def -> NP_Def (Adj<gnc-agr[1]>) Noun<gnc-agr[1]>+;
NP_Def -> NP_Def NP+;
ANP_Def -> Adj<gnc-agr[1]>+ NP_Def<gnc-agr[1], rt>; 
ANPDef -> Adj<gnc-agr[1]>+ LimitDef interp (Definition.Limitation) NP_Def<gnc-agr[1]>;
NP_Def -> NP_Def ANP_Def+;
NP_Def -> NP_Def ANPDef;
NP_Def -> ANP_Def;

Term -> NP_Def; 
Term -> NP_Def PPS interp (Definition.Comment); 
Term -> NP_Def LimitDef interp (Definition.Comment); 

//некрасивое правило, надо будет переписать
Term2 -> Adj<gram="ins"> interp (Definition.Term) Pred interp (Definition.Pred) Adj<gnc-agr[1]> interp (+Definition.Term; Content.KeyNP) Noun<gnc-agr[1]> interp (+Definition.Term; Definition.KeyWord; +Content.KeyNP; Content.KeyWord); //делимым является земельный участок

NotOne -> 'один'<rt> 'из';
Pred -> 'являться' (NotOne<gram="ins">);
Pred_pr -> 'пониматься' | 'относиться'; //для предиката с предлогом
Pred_no_interp -> Hyphen;

FullTerm -> Term interp (Definition.Term) (Limits interp (Definition.Limitation)) Pred_no_interp;  
FullTerm -> Term interp (Definition.Term) (Limits interp (Definition.Limitation)) Pred interp (Definition.Pred);  

T_not_nom -> Noun interp (Definition.KeyWord);
T_not_nom -> T_not_nom NP;
T_not_nom -> Adj<gnc-agr[1]> T_not_nom<gnc-agr[1]>;
Pr -> 'под' | 'к'; 
FullTerm -> (Pr) T_not_nom interp (Definition.Term) (Limits interp (Definition.Limitation)) Pred_pr interp (Definition.Pred);  
FullTerm -> T_not_nom<gram="ins"> interp (Definition.Term) (Limits interp (Definition.Limitation)) (LimitPart) Pred interp (Definition.Pred);  

//-------------Здесь должны быть правила для содержания определения----
//---------------------------------------------------------------------

NP_Cont -> Noun<rt, gram="nom"> interp (Content.KeyWord);
NP_Cont -> NP_Cont NP+;
NP_Cont -> NP_Cont ANP_Cont;
NP_Cont -> ANP_Cont;
NP_Cont -> NP_Cont PP_Cont;
ANP_Cont -> Adj<gnc-agr[1]>+ NP_Cont<gnc-agr[1], rt>;

PP_Cont -> Prep (Adj) Noun+;

//-------Первый тип перечислений (правая часть опр. с каждой новой строки)
NumDelim -> RBracket | Punct;
Number -> AnyWord<wff="\\d+">;

SimpleBtw -> SimConjAnd | Or | ComplexOr;
//NP_Cont -> NP_Cont SimpleBtw NP_Cont;
//Cont_NP -> NP_Cont;

LoseStrength -> 'утратить' 'сила'<gram="acc">;
EnumItem1 -> Number interp (Content.Marker) NumDelim Content;
EnumItem1 -> Number interp (Content.Marker) NumDelim NP_Cont interp (Content.KeyNP) (Limits interp (Content.Limitation));
EnumItem1 -> Number interp (Content.Marker) NumDelim LoseStrength interp (Content.Depricated = 'true') AnyWord* interp (Content.Comment); // Break;

//-------Второй тип перечислений (правая часть на той же строке)-------
Or -> (LBracket) 'или' (RBracket);
ComplexOr -> SimConjAnd Or;
Btw -> Comma | SimConjAnd | Or | ComplexOr;
EnumItem -> NP_Cont (Comm);

EnumItem -> 'другой' NP_Cont (Comm); //!!!нужно как-то "сказать", что если это прил. "другой", то мы интерпретируем не как им. группу, а как enum item

EnumIter -> EnumItem interp (Content.PlainText; Content.KeyNP) Btw;
Enum -> EnumIter+ EnumItem interp (Content.PlainText; Content.KeyNP); // Break; 

//------Содержание без перечисления---------------

//Inside Definitions on-------------------------

Incl -> 'в' 'то'<gram="abl"> 'число'<gram="abl">;
Further -> 'далее' Hyphen;  
InBrac -> Incl NP; //не надо интерпретировать

InBrac -> (Further) NP_Def interp (Definition.Term); //ПЕРЕД скобками нужно интерпретировать в Content
InBrac -> Enum; //ПЕРЕД скобками нужно интерпретировать в Def
InBrac -> EnumItem interp (Content.PlainText; Content.KeyNP);

InBrac -> NP Comma Comm;

LimBrac -> LBracket InBrac RBracket;

//Ins.Def. off----------------------------------
//то, что после запятой в оборотах
InClause -> NP<rt>+;
InClause -> InClause PPS;
InClause -> PPS;
InClause -> PPS InClause;
InClause -> InClause Btw InClause;
InClause -> (Adv) Word<gram="praet"> (InClause);
InClause -> InClause Word<gram="partcp"> Word<gram="SPRO"> InClause;

InClause -> Word<gram="V">+ InClause;

AftCom -> ('не') Word<gram="partcp">;
AftCom -> Word<gram="partcp", gnc-agr[1]> Word<gram="CONJ"> AftCom<gnc-agr[1]>;

Comm -> AftCom InClause+;
Comm -> Prep 'который' InClause;
Comm_N -> Noun 'который'<gram="gen"> InClause;
Comm -> Comm LimBrac (InClause);

//InClause2 -> Word<gram="V">+ Comm+ InClause2;
//InClause2 -> InClause;
Comm2 -> 'который' InClause Comm*; //Cont_stop; //в этом случае нужно включать и глаголы?

NP_Cont -> Adj Comma Word<gram="ADV"> Adj Noun interp (Content.KeyWord);
Cont_stop -> Break | Comma;
Content -> NP_Cont<rt, gnc-agr[1], gram="n"> interp (Content.KeyNP::norm="sg") Comma Comm<gnc-agr[1]> interp (Content.Comment::norm="n,sg");// Cont_stop;
Content -> NP_Cont<rt, gnc-agr[1], gram="f"> interp (Content.KeyNP::norm="sg") Comma Comm<gnc-agr[1]> interp (Content.Comment::norm="f,sg");// Cont_stop;
Content -> NP_Cont<rt, gnc-agr[1]> interp (Content.KeyNP::norm="sg") Comma Comm<gnc-agr[1]> interp (Content.Comment);// Cont_stop;
Content -> NP_Cont<rt> interp (Content.KeyNP::norm="sg") (LimBrac interp (Content.Limitation)) Comma Comm_N interp (Content.Comment::not_norm); // Cont_stop;
//без запятой
//Content -> Noun interp (Content.KeyWord::norm="sg") InClause interp (Content.Comment) Cont_stop;

//-----------Корневые правила------------------------------------------
//---------------------------------------------------------------------

//---------------для первого типа-----------------
FullTerm1 -> FullTerm Colon interp (Definition.EnumExpected = 'true');
S -> FullTerm1 interp (Definition.PlainText);
S -> EnumItem1 interp (Content.PlainText) (Break); //(AnyWord<wff=";">); 

//---------------для второго типа-----------------

S -> FullTerm interp (Definition.PlainText) Enum; //Break;

//---------------для типа без перечисления--------
S -> FullTerm interp (Definition.PlainText) Content interp (Content.PlainText); //Break;
S -> Term2 interp (Definition.PlainText) Comma Comm2+ interp (Content.PlainText; Content.Comment) Cont_stop;


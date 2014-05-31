#encoding "utf-8"    // кодировка
#GRAMMAR_ROOT S
#include "np.cxx"

Break -> EOSent | AnyWord<wff=";"> | Punct;

LimitDef -> LBracket EnumNP+ RBracket; //река-море (статья 2440?) 

Limits -> 'как' NP;  
Limits -> 'как' ANP<gram="acc"> SimConjAnd ANP<gram="acc">; 
Limits -> PPS;

//LimPart -> Word<gram="partcp"> InClause+;
LimPart -> PartcpPhrase;
LimitPart -> Comma LimPart interp (Definition.Comment::not_norm) Comma; 

//---------------------Правила для терминов----------------------------
//---------------------------------------------------------------------

NP_Def -> Noun<gram="nom",kwtype=~[bad_np_start]> interp (Definition.KeyWord); 
NP_Def -> NP_Def NP+;
ANP_Def -> Adj<gnc-agr[1]>+ NP_Def<gnc-agr[1], rt>; 
ANPDef -> Adj<gnc-agr[1]>+ LimitDef interp (Definition.Limitation) NP_Def<gnc-agr[1]>;
NP_Def -> NP_Def ANP_Def+;
NP_Def -> NP_Def ANPDef;
NP_Def -> ANP_Def;

Term -> NP_Def; 
Term -> NP_Def PPS interp (Definition.Comment::not_norm); 
Term -> NP_Def LimitDef interp (Definition.Comment); 

//отредактировать правило
Term2 -> Adj<gram="ins"> interp (Definition.Term) DefPred interp (Definition.Pred) Adj<gnc-agr[1]> interp (+Definition.Term; Content.KeyNP) Noun<gnc-agr[1]> interp (+Definition.Term; Definition.KeyWord; +Content.KeyNP; Content.KeyWord); //делимым является земельный участок

PenalTerm -> NP_Def interp (Definition.Term; Content.KeyNP; Content.KeyWord) DefPredInt PartcpPhrase interp (+Definition.Term);

DefPred -> 'являться' | 'признаваться';
DefPred_pr -> 'пониматься' | 'относиться'; //для предиката с предлогом
DefPredInt -> DefPred interp (Definition.Pred) | Hyphen;

FullTerm -> Term interp (Definition.Term) (Limits interp (Definition.Limitation)) DefPredInt;  

T_not_nom -> Noun<rt,kwtype=~[bad_np_start]> interp (Definition.KeyWord); //не однословные
T_not_nom -> T_not_nom<rt> NP;
T_not_nom -> Adj<gnc-agr[1]> T_not_nom<rt,gnc-agr[1]>;
Pr -> 'под' | 'к'; 
FullTerm -> Pr T_not_nom interp (Definition.Term) (Limits interp (Definition.Limitation::not_norm)) DefPred_pr interp (Definition.Pred);  
FullTerm -> T_not_nom<gram="ins"> interp (Definition.Term) (Limits interp (Definition.Limitation::not_norm)) (LimitPart) DefPred interp (Definition.Pred);  

//-------------Здесь должны быть правила для содержания определения----
//---------------------------------------------------------------------

NotOne -> 'один'<rt> 'из';
NP_Cont -> NotOne interp (Content.NotTheOnly="True") (Adj) Noun<rt> interp (Content.KeyWord);
NP_Cont -> Noun<rt, gram="nom", kwtype=~[bad_np_start]> interp (Content.KeyWord);
NP_Cont -> NP_Cont NP+;
NP_Cont -> NP_Cont ANP_Cont;
NP_Cont -> ANP_Cont;
NP_Cont -> NP_Cont PP;
ANP_Cont -> Adj<gnc-agr[1]>+ NP_Cont<gnc-agr[1], rt>;

//-------Первый тип перечислений (правая часть опр. с каждой новой строки)
NumDelim -> RBracket | Punct;
Number -> AnyWord<wff="\\d+">;
Ordered -> Number interp (Content.Marker) NumDelim;

//SimpleBtw -> SimConjAnd | Or | ComplexOr;

LoseStrength -> 'утратить' 'сила'<gram="acc">;
EnumItem1 -> Ordered Content;
EnumItem1 -> Ordered NP_Cont interp (Content.KeyNP) (Limits interp (Content.Limitation));
EnumItem1 -> Ordered LoseStrength interp (Content.Repealed = 'true') AnyWord* interp (Content.Comment);

//-------Второй тип перечислений (правая часть на той же строке)-------
Btw -> Comma | Conj;
EnumItem -> NP_Cont interp (Content.KeyNP);

_EnumItem -> 'другой' NP_Cont interp (Content.KeyNP) Comma Comm interp (Content.Comment::norm="sg"); 

EnumItem -> EnumItem interp (Content.PlainText) Btw;
//EnumIter -> EnumItem interp (Content.PlainText; Content.KeyNP) Btw EnumItem interp (Content.PlainText; Content.KeyNP) (Comma) (Comm<gram="pl"> interp (Content.Comment::norm="sg"));
EnumIter -> EnumItem+ EnumItem interp (Content.PlainText) Comma Comm<gram="pl"> interp (Content.Comment::norm="sg"); 
EnumIter -> EnumItem<gram="sg">+ EnumItem<gram="sg"> interp (Content.PlainText) Comma Comm<gram="sg"> interp (Content.Comment); 
EnumIter -> EnumItem+ _EnumItem interp (Content.PlainText); 
EnumIter -> EnumItem+ EnumItem; 
Enum -> EnumIter interp (Content.PlainText); 

//------Содержание без перечисления---------------

//Inside Definitions on-------------------------
Incl -> 'в' 'то'<gram="abl"> 'число'<gram="abl">;
Further -> 'далее' Hyphen;  
InBrac -> Incl NP; //не надо интерпретировать

InBrac -> (Further) NP_Def interp (Definition.Term); //ПЕРЕД скобками нужно интерпретировать в Content
InBrac -> Enum; //ПЕРЕД скобками нужно интерпретировать в Def
InBrac -> EnumItem interp (Content.PlainText; Content.KeyNP);
//Ins.Def. off----------------------------------

InBrac -> NP Comma Comm;
LimBrac -> LBracket InBrac RBracket {weight=1.5};
LimBracCut -> LBracket<cut> InBrac RBracket<cut> {weight=1.5};

InClause -> BigNP;
Comm -> PartcpPhrase+ (Comma) (NP);
Comm -> PartcpPhrase<gnc-agr[1]> Btw PartcpPhrase<gnc-agr[1]>;
Comm -> (Noun) Subordinate;
Comm -> Comm LimBrac PPS;
Comm -> Comm Comma EnumNP;
Comm -> Comm Comma Comm;
Comm -> Comm Comma 'а' 'также' PPS;
//Comm_N -> Noun Subordinate;

//Comm2 -> BigNP;

NP_Cont -> Adj Comma Word<gram="ADV"> Adj Noun interp (Content.KeyWord);
Cont_stop -> Break | Comma;
Content -> NP_Cont<rt, gnc-agr[1], gram="n"> interp (Content.KeyNP::norm="sg") Comma Comm<gnc-agr[1]> interp (Content.Comment::norm="n,sg");
Content -> NP_Cont<rt, gnc-agr[1], gram="f"> interp (Content.KeyNP::norm="sg") Comma Comm<gnc-agr[1]> interp (Content.Comment::norm="f,sg");
Content -> NP_Cont<rt, gnc-agr[1]> interp (Content.KeyNP::norm="sg") Comma Comm<gnc-agr[1]> interp (Content.Comment);
//Content -> NP_Cont<rt> interp (Content.KeyNP::norm="sg") (LimBrac interp (Content.Limitation)) Comma Comm_N interp (Content.Comment::not_norm);
Content -> NP_Cont<rt> interp (Content.KeyNP::norm="sg") (LimBracCut interp (Content.Limitation)) Comma Comm interp (Content.Comment::not_norm);

//без запятой
Content -> Noun<gram=~"ins"> interp (Content.KeyWord::norm="sg") InClause interp (Content.KeyNP);

PenalComm -> PP Word<gram="V"> BigNP;
PenalComm -> NP Word<gram="V"> BigNP;
PenalContent -> Comma<cut> 'если' PenalComm;

//-----------Корневые правила------------------------------------------
//---------------------------------------------------------------------

//---------------для первого типа-----------------
FullTerm1 -> FullTerm<fw> Colon interp (Definition.EnumExpected = 'true');
S -> FullTerm1 interp (Definition.PlainText);
S -> EnumItem1 interp (Content.PlainText) (Break); 

//---------------для второго типа-----------------

FullTermInv ->  DefPredInt Term interp (Definition.Term) (Limits interp (Definition.Limitation));  
S -> FullTerm<fw> interp (Definition.PlainText) Enum;
S -> Enum<fw> FullTermInv interp (Definition.PlainText);

//---------------для типа без перечисления--------
S -> FullTerm interp (Definition.PlainText) Content interp (Content.PlainText);
S -> Term2<fw> interp (Definition.PlainText) Comma Comm+ interp (Content.PlainText; Content.Comment); //Cont_stop;

//--------------для УК----------------------------
S -> PenalTerm interp (Definition.PlainText) PenalContent interp (Content.PlainText; Content.Comment);

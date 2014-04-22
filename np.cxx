#encoding "utf-8"    // кодировка
#GRAMMAR_ROOT S
#include "references.cxx"

//-------------Некоторые именные группы--------------------
Or -> (LBracket) 'или' (RBracket);
ComplexOr -> SimConjAnd Or;
Conj -> 'и' | 'или' | 'либо' | ComplexOr;
PRO -> Word<gram="SPRO"> | Word<gram="APRO">;

NPinBr -> LBracket EnumNP RBracket;
NP -> Noun | PRO ;
NP -> NP<rt> Noun+;
NP -> NP<rt> Noun<gram="gen">+ {weight=1.4};
NP -> PRO NP<rt>;
NP -> NP ANP;
NP -> NP PPS;
NP -> NP NPinBr;
NP -> ANP;
AinBr -> LBracket Adj RBracket;
ANP -> (Adv+) Adj<gnc-agr[1]>+ (AinBr) Noun<gnc-agr[1], rt>;
ANP -> (Adv+) Adj<gnc-agr[1]> Conj ANP<gnc-agr[1], rt>;
ANP -> (Adv+) Adj<gnc-agr[1]> ',' ANP<gnc-agr[1], rt>;

PP -> Prep (Reference+) NP<rt>;
PPS -> PP+;

EnumNP -> NP | PPS;
EnumNP -> EnumNP<c-agr[1]> Comma EnumNP<c-agr[1]>;
EnumNP -> EnumNP<c-agr[1]> Conj EnumNP<c-agr[1]>;

Neg -> 'не';
Partcp -> (Neg) Word<gram="partcp">; 
Partcp -> Partcp<gnc-agr[1]> Conj Partcp<gnc-agr[1]>;
PartcpPhrase -> Partcp (Reference+) (NP) EnumNP; 
//PartcpPhrase -> Partcp PPS; 
PartcpPhraseC -> Comma PartcpPhrase (Comma);

NP -> NP<c-agr[1]> Conj NP<c-agr[1]>;
BigNP -> NP;
BigNP -> BigNP<c-agr[1]> Comma BigNP<c-agr[1]>;
BigNP -> NP PartcpPhraseC+;
BigNP -> NP SubordinateC;
BigNP -> BigNP Conj NP;
BigNP -> BigNP Comma 'а' 'также' NP;

ComplVerb -> Word<gram="V">+;
ComplVerb -> ComplVerb Conj ComplVerb;
ComplVAdv -> (Adv) ComplVerb;
ComplVAdv -> ComplVerb Adv;
Which -> (Prep) 'который';
SubStop -> Comma | AnyWord<wff=";"> | EOSent;
Subordinate -> Which ComplVAdv PP NP;
Subordinate -> Which ComplVAdv EnumNP;
Subordinate -> (PP) Which NP ComplVAdv (NP);
SubordinateC -> Comma Subordinate SubStop<cut>;

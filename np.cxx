#encoding "utf-8"    // кодировка
#GRAMMAR_ROOT S
#include "references.cxx"

//-------------Некоторые именные группы--------------------
Or -> (LBracket) 'или' (RBracket);
ComplexOr -> SimConjAnd Or;
Conj -> 'и' | 'или' | 'либо' | ComplexOr;
PRO -> Word<gram="SPRO", kwtype=~[bad_np_start]>; // | Word<gram="APRO">;

NPinBr -> LBracket EnumNP RBracket;
NP -> Noun<gram="~PR"> | PRO ;
NP -> NP<rt> Noun<gram=~"nom">+;
NP -> NP<rt> PRO<gram=~"nom">+;
NP -> NP<rt> Noun<gram="gen">+ {weight=1.4};
NP -> NP<rt> (Adj<gram="gen">) Noun<gram="gen"> Comma Noun<gram="gen"> {weight=1.4};
NP -> NP<rt> (Adj<gram="gen">) Noun<gram="gen"> Conj Noun<gram="gen"> {weight=1.4};
NP -> Word<gram="NUM">+ NP<rt>;
NP -> NP ANP;
NP -> NP PPS;
NP -> NP NPinBr;
NP -> ANP;
AinBr -> LBracket Adj<rt> RBracket;
ANP -> (Adv+) Adj<gnc-agr[1]>+ (AinBr) NP<gnc-agr[1], rt>;
ANP -> (Adv+) Adj<gnc-agr[1]> (AinBr<gnc-agr[1]>) Conj ANP<gnc-agr[1], rt>;
ANP -> (Adv+) Adj<gnc-agr[1]> ',' ANP<gnc-agr[1], rt>;
//ANP -> (Adv+) Adj<gnc-agr[1]> 'до' ANP<gnc-agr[1], rt>;
NP -> Word<gram="NUM"> 'до' NP;

PP -> Prep NP<rt>;
PP -> Prep ReferInt;
PP -> ReferInt1;
PPS -> PP+;

EnumNP -> NP | PPS;
EnumNP -> EnumNP<c-agr[1]> Comma EnumNP<c-agr[1]>;
EnumNP -> EnumNP<c-agr[1]> Conj EnumNP<c-agr[1]>;
EnumNP -> Prep Adj<gnc-agr[1]> NP Noun<gnc-agr[1]>;

Neg -> 'не';
Partcp -> (Adv) (Neg) Word<gram="partcp",rt>; 
Partcp -> Partcp<gnc-agr[1]> Conj Partcp<gnc-agr[1]>;
PartcpPhrase -> Partcp (Reference+) (NP) EnumNP; 
PartcpPhrase -> Partcp ReferInt; 
PartcpPhrase -> Partcp Prep ReferInt; 
PartcpPhrase -> PartcpPhrase Conj PartcpPhrase;
PartcpPhrase -> Partcp PPS; 
PartcpPhraseC -> Comma PartcpPhrase (SubStop);

NP -> NP<c-agr[1]> Conj NP<c-agr[1]>;
BigNP -> NP;
BigNP -> PartcpPhrase<gnc-agr[1]> NP<gnc-agr[1]>;
BigNP -> Prep PartcpPhrase<gnc-agr[1]> NP<gnc-agr[1]>;
BigNP -> BigNP<c-agr[1]> Comma BigNP<c-agr[1]>;
BigNP -> NP BigNP;
BigNP -> NP<gn-agr[1]> PartcpPhraseC<gn-agr[1]>+;
BigNP -> NP SubordinateC;
BigNP -> BigNP Conj NP;
BigNP -> BigNP Comma 'а' 'также' NP;

ComplVerb -> Word<gram="V">+;
ComplVerb -> ComplVerb Conj ComplVerb;
ComplVAdv -> (Adv) ComplVerb;
ComplVAdv -> ComplVerb Adv;
Which -> (Prep) 'который'<rt>;
SubStop -> Comma | AnyWord<wff=";"> | EOSent | Punct;
Subordinate -> Which ComplVAdv PP NP;
Subordinate -> Which ComplVAdv ReferInt;
Subordinate -> Which ComplVAdv EnumNP;
Subordinate -> (PP) Which NP ComplVAdv (NP);
SubordinateC -> Comma Subordinate SubStop<cut>;

When -> 'когда';
SubWhen -> When EnumNP ComplVerb NP;
SubWhenC -> Comma When EnumNP SubStop;

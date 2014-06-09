#encoding "utf-8"    // кодировка
#GRAMMAR_ROOT S
#include "definitions.cxx"

//правила для модальности/характера
Permission -> 'вправе' | 'мочь';
Permission -> 'по' 'свой'<gnc-agr[1]> 'усмотрение'<gnc-agr[1]>;
Permission -> 'иметь' 'право';

Obligation -> 'должный'<gram="brev"> | 'обязанный'<gram="brev"> | 'обязанность' | 'обязывать'; //срок исполнения ... обязанности ... какой

Neg -> 'не';
Prohibition -> 'запрещать' | 'не' 'допускать' | 'не' 'должный'<gram="brev">;
Prohibition -> Neg Permission;

Modality -> Permission interp (LegalInfo.Modality="Permission") | Obligation interp (LegalInfo.Modality="Obligation") | Prohibition interp (LegalInfo.Modality="Prohibition");

//предикат
Pred -> Word<gram="V,~partcp,~ger", kwtype=~[def_pred]>;
Pred -> Word<gram="V,partcp,pass,brev">;

Pred -> Pred Pred<gram="inf">;
Pred -> Neg Pred;
Pred -> Pred Conj Pred;
Pred -> 'принимать' 'участие';
Byt -> 'быть';
PassPred -> Byt Word<gram="partcp">;
PassPred -> Verb<gram="ipf">; 
PassPred -> PassPred Conj PassPred;
UniPred -> Pred | PassPred interp (LegalInfo.Inversion="true");
PredInt -> UniPred interp (LegalInfo.Pred::not_norm);
PredInt -> Modality UniPred interp (LegalInfo.Pred::not_norm);
PredInt -> (Modality) Byt interp (LegalInfo.Pred::not_norm) Adv interp (LegalInfo.Method) Word<gram="partcp"> interp (+LegalInfo.Pred::not_norm);

//ключевая иг субъекта
NP_S -> Noun<gram="nom", wff=~"^[А-Я]$",kwtype=~[bad_np_start]> | PRO<gram="nom">;
NP_S -> 'право' PP<rt> {weight=1.3};
NP_S -> 'осужденный' PP {weight=1.3};
NP_S -> NP_S<rt> NP_S<gram="gen">+;
NP_S -> NP_S<rt> NP_S<gram="gen"> Conj ANP_S<gram="gen">;
NP_S -> NP_S<rt> Noun+;
NP_S -> NP_S<rt> ANP_S+;
NP_S -> ANP_S;
NP_S -> NP_S NPinBr;
ANP_S -> (Adv+) Adj<gnc-agr[1]>+ (AinBr) Noun<gnc-agr[1], rt>;
ANP_S -> (Adv+) Adj<gnc-agr[1]> (AinBr<gnc-agr[1]>) Conj ANP_S<gnc-agr[1], rt>;
ANP_S -> (Adv+) Adj<gnc-agr[1]> ',' ANP_S<gnc-agr[1], rt>;
NP_S_Int -> NP_S<rt> interp (LegalInfo.SubjectKey);
NP_S_Int -> NP_S<rt> interp (LegalInfo.SubjectKey) PPS;

//полная иг субъекта
AgentS -> NP_S_Int;
AgentS -> NP_S_Int ANP;
AgentS -> NP_S_Int NP<gram="gen"> Comma 'а' 'также' NP<gram="gen">;
AgentS -> PartcpPhrase<gnc-agr[1]> NP_S_Int<gnc-agr[1],rt>;
AgentS -> NP_S_Int<gn-agr[1]> PartcpPhraseC<gn-agr[1]>+;
AgentS -> NP_S_Int SubordinateC;
AgentS -> 'для' NP<gnc-agr[1]> PartcpPhraseC<gnc-agr[1]> NP_S_Int<rt>;

Agent -> BigNP (SubordinateC); 
Agent -> Conj Agent;

//полная иг объекта
AgentO -> Agent; 
AgentO -> PP; 
AgentO -> PP<gn-agr[1]> PartcpPhraseC<gn-agr[1]>; 
AgentO -> Partcp<gram="plen"> Agent;

//дополнительно
PartcpPhraseUPD -> PartcpPhraseC;
PartcpPhraseUPD -> Comma 'необходимый' (NP) EnumNP (Comma);

//условие
In -> 'в' | 'во';
With -> 'с' | 'со';
CondWord -> 'случай' | 'сила';
CondMarker -> In CondWord {weight=1.4};
CondMarker -> In 'зависимость' 'от';
CondMarker -> 'при' {weight=1.5};
Cond -> In Adj<gnc-agr[1]> 'случай'<gnc-agr[1]> {weight=1.4};
Cond -> CondMarker SubordinateC;
Cond -> CondMarker PartcpPhraseC;
Cond -> CondMarker SubWhenC;
Cond -> Cond PP NP;
Cond -> CondMarker NP<gram="gen"> (SubordinateC) {weight=1.2};
Cond -> CondMarker BigNP (PartcpPhrase) {weight=1.2};
Cond -> Cond Conj Cond;
Cond -> CondMarker Comma ExcMarker ExcPart  Comma 'но' Neg ExcPart Comma {weight=1.3};
CondStrong -> CondMarker Comma ExcMarker ExcPart;
CondInt -> Cond interp (LegalInfo.Condition::not_norm);
CondStrongInt -> CondStrong interp (LegalInfo.Condition::not_norm);

//исключение
ExcMarker -> 'если' | 'если' 'иной'<gram="n"> (Neg);
ExcMarker -> 'за' 'исключение' 'случай' Comma;
ExcPart -> Word<gram="V"> EnumNP;
ExcPart -> PartcpPhrase; 
ExcPart -> NP Word<gram="V">; 
ExcPart -> ExcPart Conj Word<gram="V"> NP;
Exception -> ExcMarker (Neg) ExcPart; 
ExcInt -> Exception interp (LegalInfo.Exception::not_norm);

//метод/способ
MethMarker1 -> 'по' 'решение';
MethMarker -> 'путь'<gram="ins">;
MethMarker -> In 'порядок' {weight=1.3};
Method -> In Word<gram="partcp", nc-agr[1]> NP 'порядок'<nc-agr[1]>;
Method -> MethMarker (ReferInt) NP (SubordinateC);
Method -> MethMarker (ReferInt) NP (PartcpPhraseUPD);
Method -> MethMarker PartcpPhraseC (ReferInt);
Method -> MethMarker Comma Partcp ReferInt (Comma);
Method -> Adv<gram=~"parenth"> (PP) (MethMarker) (NP);
Method -> MethMarker1 NP<gram="gen">;
Method -> MethMarker1 NP<gram="gen"> Conj NP<gram="gen">;
MethodInt -> Method interp (LegalInfo.Method::not_norm);

WeakMethMarker -> With | 'без' {weight=1.2};
WeakMethod -> WeakMethMarker NP (PP);
WeakMethodInt -> WeakMethod interp (LegalInfo.Method::not_norm);

//время
TPr -> 'на' | In | With;
TimeWord -> Word<kwtype="time">;
TimeWord -> TimeWord Comma TimeWord;
TimeLen -> 'период' | 'срок' | 'момент' | 'дата';
TimeMarker1 -> TPr (Word) TimeLen;
TimeMarker1 -> In 'течение';
TimeMarker -> 'до' | 'после' {weight=1.4};
TimeMarker2 -> 'не' 'ранее';
TimeMarker2 -> 'не' 'поздно';
TimeMarker2 -> TimeMarker2 'чем';
TimeMarker2 -> TPr TimeWord<gram="gen">;
TimeMarker -> TimeMarker1 | TimeMarker2;
Time -> TimeMarker NP<gram="gen"> {weight=1.4}; 
Time -> TimeMarker2 PPS;
Time -> TimeMarker1 (TimeMarker2) (PPS);
Time -> TimeMarker1 TimeMarker2 NP<gram="gen"> {weight=1.4};
Time -> TimeMarker1 PartcpPhraseC;
Time -> TimeMarker1 PartcpPhraseUPD;
Time -> 'за' TimeWord;
Time -> 'по истечение' Adj<gnc-agr[1]> TimeWord<gnc-agr[1]> Time;
Time -> 'пожизненно' {weight=1.3}; 
TimeInt -> Time+ interp (LegalInfo.Time);

//подробности
DetMarker -> TPr Noun<gram="abl"> Conj MethMarker;
DetMarker -> MethMarker Conj TPr Noun<gram="abl">;
Detail -> DetMarker SubordinateC;
Detail -> DetMarker PartcpPhraseC;
DetInt -> Detail interp (LegalInfo.Details::not_norm); 

//санкция
Penalty -> 'наказываться' interp (LegalInfo.Pred::not_norm) EnumNP interp (LegalInfo.Penalty::not_norm);

//вариации из нескольких дополнительных (не субъект, предикат, объект, характер)
Extra -> Extra (Comma) Extra;
Extra -> CondInt | ExcInt | MethodInt | TimeInt | ReferInt | DetInt;

//интерпретация субъектов и объектов
Subj -> AgentS interp (LegalInfo.Subject);
Subj -> Subj Comma Subj;
Subj -> Subj Conj Subj;
Obj -> AgentO interp (LegalInfo.ObjectPlainText::not_norm);
ComplexObj -> Obj Extra;
ComplexObj -> ComplexObj Comma ComplexObj;

//---------------корневые правила--------------------
Marker -> Nums<fw> interp (LegalInfo.Marker) RBracket;

S -> Marker CondInt;
S -> Marker Obj (Comma) (MethodInt);
S -> Marker WeakMethodInt; 

S -> Subj<sp-agr[1]> PredInt<sp-agr[1]> Obj;
S -> Subj<sp-agr[1]> ReferInt PredInt<sp-agr[1]> Obj;
S -> Subj<sp-agr[1]> PredInt<sp-agr[1]> CondInt DetInt;
S -> (CondInt) Subj<sp-agr[1]> (MethodInt) PredInt<sp-agr[1]> (WeakMethodInt) (Obj) (Extra); 
S -> (CondInt) Subj<sp-agr[1]> PredInt<sp-agr[1]> ComplexObj (Extra); 
S -> (CondInt) Subj Modality Obj PredInt (Extra); 
S -> CondInt PredInt<sp-agr[1]> Subj<sp-agr[1]> (Extra); 
S -> TimeInt Subj<sp-agr[1]> PredInt<sp-agr[1]> Obj (Extra); 
S -> TimeInt Subj Modality MethodInt PredInt  Obj; // Extra; 
S -> Subj Modality Extra PredInt (Obj);
S -> Obj<gram="~nom"> interp (LegalInfo.Object1::not_norm) Subj<sp-agr[1]> PredInt<sp-agr[1]> interp (LegalInfo.Inversion="false") Obj interp (LegalInfo.Object2::not_norm;+LegalInfo.ObjectPlainText::not_norm);

S -> Comma ExcInt;
S -> CondStrongInt;
S -> Penalty;

S -> Subj Modality Colon;
S -> PredInt<fw,l-reg> (CondInt) Obj (Comma) (Extra);
S -> Conj PredInt<l-reg> (CondInt) Obj (Comma) (Extra);
S -> Conj PredInt<l-reg> Extra;
S -> Comma 'но' interp (LegalInfo.Condition="True") (MethodInt) (Modality) PassPred interp (LegalInfo.Pred::not_norm) Subj;


//---------"явное условие"--------------------------
CCMarkerStart -> 'если';
CCMarkerStop -> 'то';

Antecedent -> PP interp (ConditionClause.AObj) UniPred interp (ConditionClause.APred) NP interp (ConditionClause.ASubj);
Antecedent -> BigNP interp (ConditionClause.ASubj) Pred interp (ConditionClause.APred) AgentO interp (ConditionClause.AObj);
Consequent -> NP<rt> interp (ConditionClause.CSubj) UniPred interp (ConditionClause.CPred) BigNP;

S -> CCMarkerStart Antecedent interp (ConditionClause.Antecedent) Comma CCMarkerStop Consequent interp (ConditionClause.Consequent) {weight=1.5}; 
S -> CCMarkerStart Antecedent interp (ConditionClause.Antecedent) Comma Consequent<gram="nom"> interp (ConditionClause.Consequent) {weight=1.5}; 

#encoding "utf-8"    // кодировка
#GRAMMAR_ROOT S
#include "definitions.cxx"

Permission -> 'вправе' | 'мочь';
Permission -> 'по' 'свой'<gnc-agr[1]> 'усмотрение'<gnc-agr[1]>;
Permission -> 'иметь' 'право';

Obligation -> 'должный'<gram="brev"> | 'обязанный'<gram="brev"> | 'обязанность' | 'обязывать'; //срок исполнения ... обязанности ... какой
Prohibition -> 'запрещать' | 'не' 'допускать';
Prohibition -> Neg Permission;

Modality -> Permission interp (LegalInfo.Modality="Permission") | Obligation interp (LegalInfo.Modality="Obligation") | Prohibition interp (LegalInfo.Modality="Prohibition");

Pred -> Word<gram="V,~partcp,~ger", kwtype=~[def_pred]>;
Pred -> Word<gram="V,partcp,pass,brev">;

Pred -> Pred Pred<gram="inf">;
Pred -> Neg Pred;
Pred -> Pred Conj Pred;
Pred -> 'участвовать' 'в';
Pred -> 'принимать' 'участие' 'в';
Pred -> 'знакомиться' 'с';
Pred -> 'сохраняться' 'за';
PassPred -> 'быть' Word<gram="partcp">;
//PassPred -> Pred<gram="ipf"> | Pred<gram="reflex"> | Pred<gram="pass">;
PassPred -> PassPred Conj PassPred;
UniPred -> Pred | PassPred;
PredInt -> UniPred interp (LegalInfo.Pred::not_norm);

Agent -> BigNP (SubordinateC); //нужно будет добавить про зпт и кавычки
Agent -> Conj Agent;

//маленький костыль
PartcpPhraseUPD -> PartcpPhraseC;
PartcpPhraseUPD -> Comma 'необходимый' (NP) EnumNP (Comma);

CondWord -> 'случай' | 'порядок';
CondMarker -> 'в' CondWord {weight=1.4};
CondMarker -> 'для' | 'при' | 'только' | 'до' | 'по';
CondMarker -> 'по' 'соглашение';
CondMarker -> CondMarker Conj CondMarker;
CondMarker -> CondMarker Conj CondWord;
Cond -> CondMarker SubordinateC;
Cond -> Cond PP NP;
Cond -> CondMarker NP (SubordinateC) {weight=1.2};
Cond -> CondMarker BigNP (PartcpPhrase) {weight=1.2};
Cond -> Cond Conj Cond;
Cond -> CondMarker Comma ExcMarker NP PassPred Comma 'но' Neg PassPred PPS Comma;
CondInt -> Cond interp (LegalInfo.Condition::not_norm);

ExcMarker -> 'если' | 'если' 'иной'<gram="n"> (Neg);
ExcMarker -> 'за' 'исключение' 'случай' Comma;
ExcPart -> Word<gram="V"> EnumNP (Reference+); 
ExcPart -> Word<gram="V"> Prep Reference+ (NP); 
ExcPart -> ExcPart Conj Word<gram="V"> NP;
Exception -> ExcMarker (Neg) ExcPart; 
ExcInt -> Exception interp (LegalInfo.Exception::not_norm);

With -> 'с' | 'со';
MethMarker -> 'в' 'соответствие' With {weight=1.2};
MethMarker -> 'в' 'сила';
MethMarker -> 'в' 'течение';
MethMarker -> 'в' 'зависимость' 'от';
MethMarker -> 'в';
MethMarker -> 'до' | 'после' {weight=1.2};
MethMarker -> 'на' Word<c-agr[1]> 'срок'<c-agr[1]> {weight=1.5};
Method -> 'в' Word<gram="partcp", nc-agr[1]> NP 'порядок'<nc-agr[1]>;
Method -> MethMarker (Reference+) NP (SubordinateC);
Method -> MethMarker (Reference+) NP (PartcpPhraseUPD);
Method -> Adv<gram=~"parenth"> (PP);
Method -> Reference+ NP;
Method -> Comma WeakMethMarker NP;
MethodInt -> Method interp (LegalInfo.Method::not_norm);

WeakMethMarker -> With | 'без' {weight=1.2};
WeakMethod -> WeakMethMarker NP (PP);
WeakMethodInt -> WeakMethod interp (LegalInfo.Method::not_norm);

Extra -> Extra (Comma) Extra;
/*Extra -> Extra1 (Comma) ExcInt;
Extra -> Extra2 (Comma) CondInt;
Extra -> Extra3 MethodInt;
Extra1 -> CondInt | MethodInt;
Extra2 -> ExcInt | MethodInt;
Extra3 -> CondInt | ExcInt;
*/Extra -> CondInt | ExcInt | MethodInt;

Subj -> Agent interp (LegalInfo.Subject::not_norm);
Obj -> Agent interp (LegalInfo.Object::not_norm);
Obj -> Partcp<gram="plen"> interp (LegalInfo.Object::not_norm) Agent interp (+LegalInfo.Object::not_norm);
ComplexObj -> Obj MethodInt;
ComplexObj -> ComplexObj Comma ComplexObj;

S -> Nums RBracket CondInt;
S -> Nums RBracket Obj (Comma) (MethodInt);
S -> Nums RBracket WeakMethodInt; 

S -> (CondInt) Subj (MethodInt) (Modality) PredInt (WeakMethodInt) (Obj) (Extra); 
S -> (CondInt) Subj (Modality) PredInt ComplexObj (Extra); 
S -> (CondInt) Subj Modality Obj PredInt (Extra); 
//S -> (CondInt) (Modality) Pred interp (LegalInfo.Pred::not_norm) Subj (Extra); //вообще это MethodInt?
//S -> Obj 'у' Subj (Modality) Pred interp (LegalInfo.Pred::not_norm) (Extra) {weight=1.3};
//S -> (CondInt) (Modality) PassPred interp (LegalInfo.Pred::not_norm) Subj {weight=1.2};

S -> Subj Modality Colon;
S -> PredInt<fw,l-reg> (CondInt) Obj (Comma) (Extra);
S -> Conj PredInt<l-reg> (CondInt) Obj (Comma) (Extra);
S -> Conj PredInt<l-reg> Extra;
S -> Comma 'но' interp (LegalInfo.Condition="True") (MethodInt) (Modality) PassPred interp (LegalInfo.Pred::not_norm) Subj;

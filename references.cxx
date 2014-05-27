#encoding "utf-8"    // кодировка
#GRAMMAR_ROOT S

//References---
RF -> 'россия';
RF -> 'российский'<gnc-agr[1]> 'федерация'<gnc-agr[1],rt> {weight=1.2};
About -> 'о' | 'об';
Quote -> QuoteDbl | QuoteSng;
Law_sup -> 'президент' | 'правительство' | RF;
//Law_sup -> RF;
Law_sup -> Law_sup Law_sup;
Law_name -> 'международный' 'договор'; //отдельно, ибо "договоров" в ГК много
Law_name -> Noun<kwtype=law_names>;
Law -> Law_name;
Law -> Adj<gnc-agr[1]>+ Law_name<gnc-agr[1]>;
Law -> Law_name<rt> (Quote) About NP (Quote) {weight=1.3};
Law -> Law Law_sup<gram="gen"> {weight=1.5};

Parts -> Noun<kwtype=parts_of_law>;
PartsA -> Adj<gnc-agr[1]> Parts<gnc-agr[1],rt>;
Parts -> PartsA;
Nums -> AnyWord<wff="[0-9]+">;
SeveralN -> Nums;
SeveralN -> Nums Hyphen Nums;
SeveralN -> SeveralN Conj Nums;
SeveralN -> SeveralN Comma Nums;

Reference -> Parts<gc-agr[1]>  Adj<gram='ANUM', gc-agr[1]>;
Reference -> Parts<gc-agr[1]>  Adj<gram='ANUM', gc-agr[1]> Reference<gram="gen">;
Reference -> Parts SeveralN;
Reference -> Parts SeveralN Reference<gram="gen">+ {weight=1.7};
Reference -> Law;
Reference -> PartsA;
Reference -> Reference+ Comma Reference+;
Reference -> Reference<c-agr[1]>+ 'и' Reference<c-agr[1]>+;
ReferInt -> Reference interp (LegalInfo.Reference::not_norm);

//-----------supplementary----
With -> 'с' | 'со';
RefMarker -> 'в' 'соответствие' With {weight=2};
ReferInt1 -> RefMarker interp (LegalInfo.Reference) Reference interp (+LegalInfo.Reference::not_norm) {weight=1.3};
ReferInt -> ReferInt1;

#encoding "utf-8"    // кодировка
#GRAMMAR_ROOT S

//References---
RF -> 'россия';
RF -> 'российский'<gnc-agr[1]> 'федерация'<gnc-agr[1],rt>;
Law_sup -> 'президент' | 'правительство';
Law_sup -> RF;
Law_sup -> Law_sup Law_sup;
Law_name -> 'международный' 'договор';
Law_name -> Noun<kwtype=law_names>;
Law -> Law_name;
Law -> Adj<gnc-agr[1]>+ Law_name<gnc-agr[1]>;
Law -> Law_name PP;
Law -> Law Law_sup<gram="gen"> {weight=1.3};

Parts -> Noun<kwtype=parts_of_law>;
PartsA -> Adj<gnc-agr[1]> Parts<gnc-agr[1],rt>;
Parts -> PartsA;
Nums -> AnyWord<wff="[0-9]+">;
SeveralN -> Nums;
SeveralN -> Nums Hyphen Nums;
SeveralN -> SeveralN Conj Nums;
SeveralN -> SeveralN Comma Nums;

Reference -> Parts<gc-agr[1]>  Adj<gram='ANUM', gc-agr[1]> (Parts<gram="gen">) (Law<gram="gen">);
Reference -> Parts SeveralN;
Reference -> Parts SeveralN Law<gram="gen">;
Reference -> Parts SeveralN Parts<gram="gen"> (Law<gram="gen">) {weight=1.5};
Reference -> Law;
Reference -> PartsA;
Reference -> Reference+ Comma Reference+;
Reference -> Reference+ 'и' Reference+;
ReferInt -> Reference+ interp (LegalInfo.Reference::not_norm);

//-----------supplementary----
RefMarker -> 'в' 'соответствие' With {weight=2};
ReferInt -> RefMarker interp (LegalInfo.Reference) Reference+ interp (+LegalInfo.Reference::not_norm);


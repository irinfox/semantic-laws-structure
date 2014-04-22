#encoding "utf-8"    // кодировка
#GRAMMAR_ROOT S

//References---
Parts -> Noun<kwtype=parts_of_law>;
Nums -> AnyWord<wff="[0-9]+">;
SeveralN -> Nums;
SeveralN -> Nums Hyphen Nums;
SeveralN -> SeveralN Conj Nums;
SeveralN -> SeveralN Comma Nums;
Reference -> Parts<gc-agr[1]>  Adj<gram='ANUM', gc-agr[1]>;
Reference -> Parts SeveralN;
